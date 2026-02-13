cmake_minimum_required(VERSION 3.16)

project(weomlink_desktop_demo)

option(WEOMLINK_DEMO_BUILD_BOOST "Build the Boost library?" ON)
if(WEOMLINK_DEMO_BUILD_BOOST)
    include(cmake/boost.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_BINARY_DIR}/weomlink)

add_executable(weomlink_desktop_demo 
    main.cpp
    boostdatalinkinterface.cpp boostdatalinkinterface.h
    elapsedtimer.cpp elapsedtimer.h
)

target_link_libraries(weomlink_desktop_demo PRIVATE
    WEOM::link
    Boost::asio
)