#ifndef LED_HPP
#define LED_HPP

#include <cstdint>
#include <atomic>

class Led
{
    Led();

public:
    static Led& instance();

    static void task(void* arg);

    struct RGB
    {
        uint32_t R;
        uint32_t G;
        uint32_t B;
    };

    static constexpr RGB RED = {20, 0, 0};
    static constexpr RGB GREEN = {0, 20, 0};
    static constexpr RGB BLUE = {0, 0, 20};
    static constexpr RGB OFF = {0, 0, 0};

    void setColor(RGB color);
    RGB getColor() const;

    void setFlashing(bool flashing);
    bool getFlashing() const;

    void setRefreshIntervalMs(size_t refreshIntervalMs);
    size_t getRefreshIntervalMs() const;

private:
    std::atomic<RGB> m_color;
    std::atomic<bool> m_flashing;
    std::atomic<size_t> m_refreshIntervalMs;
    bool m_flashingIndicator;
};

#endif // LED_HPP
