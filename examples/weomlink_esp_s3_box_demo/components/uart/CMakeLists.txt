idf_component_register(
    SRCS 
        uart.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        esp-box-3
)

target_link_libraries(${COMPONENT_TARGET} PUBLIC WEOM::link)