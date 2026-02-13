#include "boostdatalinkinterface.h"

#include "elapsedtimer.h"

BoostDataLinkInterface::BoostDataLinkInterface()
    : m_ioContext()
    , m_serialPort(nullptr)
    , m_connectionLost(false)
{

}

etl::unique_ptr<BoostDataLinkInterface> BoostDataLinkInterface::connect(const std::string& deviceLocation, unsigned baudrate)
{
    auto connection = etl::unique_ptr<BoostDataLinkInterface>(new BoostDataLinkInterface);
    connection->m_serialPort = std::make_unique<boost::asio::serial_port>(connection->m_ioContext);

    boost::system::error_code errorCode;
    connection->m_serialPort->open(deviceLocation, errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    connection->m_serialPort->set_option(boost::asio::serial_port_base::baud_rate(baudrate), errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    connection->m_serialPort->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::type::none), errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    connection->m_serialPort->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::type::none), errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    connection->m_serialPort->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::type::one), errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    connection->m_serialPort->set_option(boost::asio::serial_port_base::character_size(8), errorCode);
    if (errorCode.failed())
    {
        return etl::unique_ptr<BoostDataLinkInterface>(nullptr);
    }

    return connection;
}

bool BoostDataLinkInterface::isOpened() const
{
    return m_serialPort->is_open();
}

void BoostDataLinkInterface::closeConnection()
{
    if (m_serialPort)
    {
        boost::system::error_code closeError;
        m_serialPort->close(closeError);
        if (closeError.failed())
        {
            std::cerr << "Error on sereal port closing: " << closeError.message();
        }
        m_ioContext.stop();
        m_serialPort.reset();
    }
}

size_t BoostDataLinkInterface::getMaxDataSize() const
{
    return std::numeric_limits<size_t>::max();
}

etl::expected<void, wl::Error> BoostDataLinkInterface::read(etl::span<uint8_t> buffer, const wl::Clock::duration& timeout)
{
    if (!isOpened())
    {
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__NO_CONNECTION);
    }

    std::span<uint8_t> restOfBuffer = buffer;

    const ElapsedTimer timer(timeout);
    while (!timer.timedOut() && !restOfBuffer.empty())
    {
        boost::system::error_code errorCode;
        size_t transferedSize = 0;

        const bool timedOut = doAsync(timer.getRestOfTimeout(), restOfBuffer, errorCode, transferedSize);
        if (errorCode.failed())
        {
            if (isConnectionLostIndicator(errorCode))
            {
                m_connectionLost = true;
            }

            return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
        }

        restOfBuffer = restOfBuffer.last(restOfBuffer.size() - transferedSize);
    }

    if (restOfBuffer.size() > 0)
    {
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
    }

    return {};
}

etl::expected<void, wl::Error> BoostDataLinkInterface::write(etl::span<const uint8_t> buffer, const wl::Clock::duration& timeout)
{
    if (!isOpened())
    {
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__NO_CONNECTION);
    }

    std::span<const uint8_t> restOfBuffer = buffer;

    const ElapsedTimer timer(timeout);
    while (!timer.timedOut() && !restOfBuffer.empty())
    {
        boost::system::error_code errorCode;
        size_t transferedSize = 0;

        const bool timedOut = doAsync(timer.getRestOfTimeout(), restOfBuffer, errorCode, transferedSize);
        if (errorCode.failed())
        {
            if (isConnectionLostIndicator(errorCode))
            {
                m_connectionLost = true;
            }

            return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
        }

        restOfBuffer = restOfBuffer.last(restOfBuffer.size() - transferedSize);
    }

    if (restOfBuffer.size() > 0)
    {
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
    }

    return {};
}

void BoostDataLinkInterface::dropPendingData()
{

}

bool BoostDataLinkInterface::isConnectionLost() const
{

}

bool BoostDataLinkInterface::isConnectionLostIndicator(boost::system::error_code errorCode) const
{
    return errorCode == make_error_code(boost::asio::error::no_permission) // for windows
#ifdef BOOST_WINDOWS_API
            || errorCode == make_error_code(boost::system::windows_error::bad_command) // for windows FX3
#endif
            || errorCode == boost::system::error_code(EIO, boost::asio::error::get_system_category()) // for linux
            || errorCode == boost::system::error_code(ENXIO, boost::asio::error::get_system_category()) // for macOS
            || errorCode == make_error_code(boost::asio::error::eof);      // for linux
}

namespace
{
template<typename Functor>
void doAsyncImpl(boost::asio::serial_port& serialPort, std::span<uint8_t> buffer, Functor&& functor)
{
    boost::asio::async_read(serialPort, boost::asio::buffer(buffer.data(), buffer.size()), std::forward<Functor>(functor));
}

template<typename Functor>
void doAsyncImpl(boost::asio::serial_port& serialPort, std::span<const uint8_t> buffer, Functor&& functor)
{
    boost::asio::async_write(serialPort, boost::asio::buffer(buffer.data(), buffer.size()), std::forward<Functor>(functor));
}
} // namespace

template<typename T>
bool BoostDataLinkInterface::doAsync(std::chrono::steady_clock::duration timeout,
                             std::span<T> buffer,
                             boost::system::error_code& errorCode,
                             size_t& transferedSize)
{
    bool timedOut = false;
    boost::asio::deadline_timer timer(m_ioContext);
    boost::asio::deadline_timer closeTimer(m_ioContext);

    doAsyncImpl(*m_serialPort, buffer,
                [&timer, &closeTimer, &errorCode, &transferedSize](const boost::system::error_code& resultError, size_t result_n)
                {

                    boost::system::error_code cancelError;
                    closeTimer.cancel(cancelError);
                    timer.cancel(cancelError);
                    if (resultError == boost::asio::error::operation_aborted)
                    {
                        return;
                    }
                    errorCode = resultError;
                    transferedSize = result_n;
                });

    timer.expires_from_now(boost::posix_time::milliseconds(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
    timer.async_wait([this, &timedOut](const boost::system::error_code& resultError)
                     {
                         boost::system::error_code cancelError;
                         m_serialPort->cancel(cancelError);

                         if (resultError != boost::asio::error::operation_aborted)
                         {
                             timedOut = true;
                         }
                     });

    closeTimer.expires_from_now(boost::posix_time::milliseconds(std::max<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() * 2, CLOSE_SERIAL_PORT_MIN_TIMEOUT)));
    closeTimer.async_wait([this, &timedOut](const boost::system::error_code& resultError)
                          {
                              if (resultError != boost::asio::error::operation_aborted)
                              {
                                  boost::system::error_code closeError;
                                  m_serialPort->close(closeError);
                              }
                          });

    m_ioContext.restart();
    m_ioContext.run();

    return timedOut;
}
