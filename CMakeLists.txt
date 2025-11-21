cmake_minimum_required(VERSION 3.10)

project(WEOMlink)

option(WEOMLINK_BUILD_ETL "Build the Embedded Template Library?" ON)
if(WEOMLINK_BUILD_ETL)
    include(cmake/etl.cmake)
endif()

find_package(Doxygen QUIET)

add_library(weomlink
    wl/weom.cpp

    wl/dataclasses/contrastbrightness.cpp
    wl/dataclasses/firmwareversion.cpp
    wl/dataclasses/imageflip.cpp
    wl/dataclasses/presetid.cpp
    wl/dataclasses/status.cpp
    wl/dataclasses/triggers.cpp

    wl/communication/addressrange.cpp
    wl/communication/ideviceinterface.cpp
    wl/communication/protocolinterfacetcsi.cpp
    wl/communication/tcsipacket.cpp

    wl/misc/elapsedtimer.cpp

    wl/weom/deviceinterfaceweom.cpp
    wl/weom/memoryspaceweom.cpp
)

if (DOXYGEN_FOUND)
    function(add_docs target html latex outdir)
        set(DOXYGEN_GENERATE_HTML  ${html})
        set(DOXYGEN_GENERATE_LATEX ${latex})
        set(DOXYGEN_OUTPUT_DIRECTORY ${outdir})

        doxygen_add_docs(${target}  ${ARGN})
    endfunction()

    include(cmake/doxygen-awesome-css.cmake)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE readme.md)
    set(DOXYGEN_PROJECT_LOGO workswell_logo_small.png)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
            ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css
            documentation/workswell_style.css)
    set(DOXYGEN_HTML_EXTRA_FILES workswell_logo_small.png)
    set(DOXYGEN_IMAGE_PATH ${CMAKE_SOURCE_DIR})
    set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR})
    add_docs(weomlink_docs
        YES NO
        ${CMAKE_BINARY_DIR}/docs
        ALL
        readme.md weom.jpg wl documentation/registers.dox
    )

    add_docs(weomlink_docs_latex
        NO YES
        ${CMAKE_BINARY_DIR}/docs/pdf
        readme.md weom.jpg wl documentation/registers.dox
    )

    add_custom_target(weomlink_docs_pdf
      DEPENDS weomlink_docs_latex
      COMMENT "Building PDF documentation from LaTeX sources"

      COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/docs/pdf/latex
              make pdf

      COMMAND ${CMAKE_COMMAND} -E rename
              ${CMAKE_BINARY_DIR}/docs/pdf/latex/refman.pdf
              ${CMAKE_BINARY_DIR}/docs/pdf/weomlink.pdf
    )
endif()

target_include_directories(weomlink PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_features(weomlink PUBLIC cxx_std_20)

target_link_libraries(weomlink PUBLIC etl::etl) 

add_library(WEOM::link ALIAS weomlink)
