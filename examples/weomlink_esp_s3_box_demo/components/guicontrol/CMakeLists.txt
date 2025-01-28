idf_component_register(
    SRCS 
        guicontrol.cpp
        menuitems.cpp
        displaylockguard.cpp
        menu.cpp
        connectionscreen.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        esp-box-3
        uart
)

target_link_libraries(${COMPONENT_TARGET} PUBLIC WEOM::link)
target_link_libraries(${COMPONENT_TARGET} PUBLIC etl::etl) 