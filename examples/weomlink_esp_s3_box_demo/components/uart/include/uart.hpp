#ifndef UART_HPP
#define UART_HPP

#include "wl/communication/idatalinkinterface.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include <etl/memory.h>

class Uart : public wl::IDataLinkInterface
{
    Uart();

public:
    ~Uart();

    static etl::unique_ptr<Uart> connectUart(int baudrate);

    virtual bool isOpened() const override;
    virtual void closeConnection() override;

    virtual size_t getMaxDataSize() const override;

    [[nodiscard]] virtual etl::expected<void, wl::Error> read(etl::span<uint8_t> buffer, const wl::Clock::duration& timeout) override;
    [[nodiscard]] virtual etl::expected<void, wl::Error> write(etl::span<const uint8_t> buffer, const wl::Clock::duration& timeout) override;

    virtual void dropPendingData() override;

    virtual bool isConnectionLost() const override;
private:
    static constexpr uart_port_t UART_PORT = UART_NUM_1;
    static constexpr int UART_BUFFER_SIZE = 256;
    static constexpr uart_word_length_t UART_WORD_LENGTH = UART_DATA_8_BITS;
    static constexpr uart_parity_t UART_PARITY = UART_PARITY_DISABLE;
    static constexpr uart_stop_bits_t UART_STOP_BITS = UART_STOP_BITS_1;
    static constexpr gpio_num_t UART_RX = GPIO_NUM_44;
    static constexpr gpio_num_t UART_TX = GPIO_NUM_43;
};

#endif // UART_HPP