#include "uart.hpp"

#include "esp_err.h"
#include "esp_log.h"

static const char* TAG = "uart";

Uart::Uart() : wl::IDataLinkInterface()
{

} 

Uart::~Uart()
{
    closeConnection();
}

etl::unique_ptr<Uart> Uart::connectUart(int baudrate)
{
    if (uart_is_driver_installed(UART_PORT))
    {
        ESP_ERROR_CHECK(uart_driver_delete(UART_PORT));
    }
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_WORD_LENGTH,
        .parity = UART_PARITY,
        .stop_bits = UART_STOP_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT
    };
    esp_err_t error = ESP_OK;
    if (((error = uart_driver_install(UART_PORT, UART_BUFFER_SIZE, 0, 0, nullptr, 0)) != ESP_OK) ||
        ((error = uart_param_config(UART_PORT, &uart_config)) != ESP_OK) ||
        ((error = uart_set_pin(UART_PORT, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) != ESP_OK) ||
        ((error = uart_set_mode(UART_PORT, UART_MODE_UART)) != ESP_OK))
    {
        ESP_LOGE(TAG, "Failed to setup UART (%s)", esp_err_to_name(error));
        return etl::unique_ptr<Uart>(nullptr);
    }
    return etl::unique_ptr<Uart>(new Uart);
}

bool Uart::isOpened() const
{
    return uart_is_driver_installed(UART_PORT);
}

void Uart::closeConnection()
{
    if (uart_is_driver_installed(UART_PORT))
    {
        ESP_ERROR_CHECK(uart_driver_delete(UART_PORT));
    }
}

size_t Uart::getMaxDataSize() const
{
    return 256;
}

etl::expected<void, wl::Error> Uart::read(etl::span<uint8_t> buffer, const wl::Clock::duration& timeout)
{
    if (!isOpened())
    {
        ESP_LOGE(TAG, "Failed to read data (not connected)");
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__NO_CONNECTION);
    }
    int readSize = uart_read_bytes(UART_PORT, buffer.data(), buffer.size(), pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
    if (readSize != buffer.size())
    {
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
    }
    ESP_LOG_BUFFER_HEX_LEVEL("uart_control:  read", buffer.data(), readSize, ESP_LOG_INFO);
    return {};
}

etl::expected<void, wl::Error> Uart::write(etl::span<const uint8_t> buffer, const wl::Clock::duration& timeout)
{
    if (!isOpened())
    {
        ESP_LOGE(TAG, "Failed to write data (not connected)");
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__NO_CONNECTION);
    }
    int writtenBytes = uart_write_bytes(UART_PORT, buffer.data(), buffer.size());
    if (writtenBytes != buffer.size())
    {
        ESP_LOGE(TAG, "Failed to write data (written %i out of %i bytes)", writtenBytes, buffer.size());
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
    }

    esp_err_t error = uart_wait_tx_done(UART_PORT, pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
    if (error != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write data (%s)", esp_err_to_name(error));
        return etl::unexpected<wl::Error>(wl::Error::DATALINK__TIMEOUT);
    }
    ESP_LOG_BUFFER_HEX_LEVEL("uart_control: write", buffer.data(), buffer.size(), ESP_LOG_INFO);
    return {};  
}

void Uart::dropPendingData()
{
    uart_flush(UART_PORT);
}

bool Uart::isConnectionLost() const
{
    return false;
}
