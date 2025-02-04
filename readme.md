# ![logo](workswell_logo_small.png) WEOMlink
WEOMlink is a C++ library designed to manage WEOM cameras on embedded platforms, providing communication, control. It leverages C++20 features and the [ETL (Embedded Template Library)](https://www.etlcpp.com/) to optimize performance in resource-constrained environments.

## Installation
Clone this repository into Your CMake project folder (or add it as submodule)

```sh
git clone https://github.com/Workswell-s-r-o/WEOMlink.git
```

or

```sh
git submodule add https://github.com/Workswell-s-r-o/WEOMlink.git
```

Add WEOMlink as subdirectory into CMake

```cmake
add_subdirectory(weom-link)
```

Link WEOMlink to Your Cmake target:

```cmake
target_link_libraries(<your_target> PUBLIC WEOM::link)
```

## Usage
To use WEOMlink on your embedded platform, you need to implement the `wl::IDataLinkInterface` class to define data transfer methods. 

```cpp
#include "wl/communication/idatalinkitnerface.h"


class MyDataLinkInterface : public wl::IDataLinkInterface
{
public:
    virtual bool isOpened() const override;

    virtual void closeConnection() override;

    virtual size_t getMaxDataSize() const override;

    virtual etl::expected<void, wl::Error> read(etl::span<uint8_t> buffer, const wl::Clock::duration& timeout) override;

    virtual etl::expected<void, wl::Error> write(etl::span<const uint8_t> buffer, const wl::Clock::duration& timeout) override;

    virtual void dropPendingData() override;

    virtual bool isConnectionLost() const override;
};
```

After implementing the data link interface, initialize an instance of wl::WEOM and set the data link interface using setDataLinkInterface.

```cpp
#include "wl/weom.h"

void sleepFunction(const wl::Clock::duration& duration)
{
    // implement sleep function for your plaftorm
    // std::this_thread::sleep_for(duration);
    vTaskDelay(pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()));
}

int main(int argc, char** argv)
{
    wl::WEOM camera(sleepFunction);

    auto dataLink = etl::unique_ptr<MyDataLinkInterface>(new MyDataLinkInterface);

    camera.setDataLinkInterface(etl::move(dataLink));

    // ...
}
```

With the camera interface set, you can now use various functions, such as retrieving the camera's serial number. Below is an example of fetching and handling the serial number.

```cpp
auto serialNumber = camera.getSerialNumber();
if (!serialNumber.has_value())
{
    std::cerr << "Failed to get serial number. Error: " << serialNumber.error().c_str() << std::endl;
}
std::cout << "Serial number: " << serialNumber.value().c_str() << std::endl;
```

## Examples
An example project demonstrating the usage of WEOMlink is provided in the examples directory. You can refer to it for a practical implementation of the library.