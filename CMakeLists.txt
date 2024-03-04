# cmake_minimum_required(VERSION 3.28.2)

# Include(FetchContent)


# # Catch2 Testing framework
# # --------------------------
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.5.2
# )

# FetchContent_MakeAvailable(Catch2)

# set(MAIN_FILE "src/main.cpp")
# file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
# list(FILTER SRC_FILES EXCLUDE REGEX ".*\\.test\\.cpp$")
# list(FILTER SRC_FILES EXCLUDE REGEX "src/main.cpp")
# file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "src/*.test.cpp")

# add_executable(tests ${SRC_FILES} ${TEST_FILES})
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
# # --------------------------

# # Executable
# # --------------------------
# project(HelloWorld)

# # Add the executable
# add_executable(${PROJECT_NAME} ${SRC_FILES} ${MAIN_FILE})
# # --------------------------

cmake_minimum_required(VERSION 3.28.2)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(blink C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
  src/main.cpp
)

pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
  pico_stdlib
)
