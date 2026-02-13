# WEOMlink Desktop Demo
This example provides a reference implementation for using the WEOMlink protocol on desktop platforms (Linux and Windows). It demonstrates how to establish communication with a WEOM camera via UART.

To handle asynchronous serial communication, this project utilizes the [Boost.Asio](https://www.boost.org/doc/libs/latest/doc/html/boost_asio.html) library.

## Requirements
Before building, ensure you have the following installed:
- CMake (version 3.16 or higher)
- C++ Compiler: GCC, Clang, or MSVasC (C++17 or newer recommended)
- (Optional) Boost: If not installed system-wide, the build system can fetch it for you.

## Building the example
This is standard CMake project:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> NOTE: Boost will be automatically downloaded by fetchcontent. To prevent this, call cmake with `-DWEOMLINK_DEMO_BUILD_BOOST=OFF`.

## Running the example
The executable expects 2 arguments:
- The first argument is the serial port name (e.g., `/dev/ttyACM0`).
- The second argument is the baud rate (e.g., `115200`).

To run the example, execute from your `build` directory:
```bash
./weomlink_desktop_demo /dev/ttyACM0 115200
```

> NOTE for Linux users: If you encounter permission errors, ensure your user is part of the `dialout` group.