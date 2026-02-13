include(FetchContent)

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG      boost-1.84.0
  GIT_PROGRESS TRUE
)

FetchContent_GetProperties(boost)
FetchContent_MakeAvailable(boost)
