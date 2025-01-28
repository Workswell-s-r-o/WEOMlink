include(FetchContent)

FetchContent_Declare(
    etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl
    GIT_TAG        20.39.4
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)

set(GIT_DIR_LOOKUP_POLICY ALLOW_LOOKING_ABOVE_CMAKE_SOURCE_DIR)

FetchContent_GetProperties(etl)
FetchContent_MakeAvailable(etl)
