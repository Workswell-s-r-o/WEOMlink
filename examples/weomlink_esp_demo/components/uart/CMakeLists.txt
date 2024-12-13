idf_component_register(
    SRCS 
        uart.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        esp32_s2_kaluga_kit
)

target_link_libraries(${COMPONENT_TARGET} PUBLIC WEOM::link)