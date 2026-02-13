#include "wl/communication/idatalinkinterface.h"

#include <etl/expected.h>
#include <etl/memory.h>
#include <boost/asio.hpp>

#include <atomic>
#include <string>

class BoostDataLinkInterface : public wl::IDataLinkInterface
{
    BoostDataLinkInterface();
public:
    static etl::unique_ptr<BoostDataLinkInterface> connect(const std::string& deviceLocation, unsigned baudrate);

    virtual bool isOpened() const override;
    virtual void closeConnection() override;
    virtual size_t getMaxDataSize() const override;

    virtual etl::expected<void, wl::Error> read(etl::span<uint8_t> buffer, const wl::Clock::duration& timeout) override;
    virtual etl::expected<void, wl::Error> write(etl::span<const uint8_t> buffer, const wl::Clock::duration& timeout) override;

    virtual void dropPendingData() override;
    virtual bool isConnectionLost() const override;

private:
    boost::asio::io_context m_ioContext;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;
    std::atomic<bool> m_connectionLost;

    bool isConnectionLostIndicator(boost::system::error_code errorCode) const;

    template<typename T>
    bool doAsync(std::chrono::steady_clock::duration timeout,
                 std::span<T> buffer,
                 boost::system::error_code& errorCode,
                 size_t& transferedSize);

    static constexpr unsigned CLOSE_SERIAL_PORT_MIN_TIMEOUT = 500;

};