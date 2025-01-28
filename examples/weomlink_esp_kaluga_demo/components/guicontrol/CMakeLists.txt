idf_component_register(
    SRCS 
        guicontrol.cpp
        menuitems.cpp
        displaylockguard.cpp
        menu.cpp
        connectionscreen.cpp
        guiaction.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        esp32_s2_kaluga_kit
        led
        buttons
        uart
)

target_link_libraries(${COMPONENT_TARGET} PUBLIC WEOM::link)
target_link_libraries(${COMPONENT_TARGET} PUBLIC etl::etl) 