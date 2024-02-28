cmake_minimum_required(VERSION 3.10)

Include(FetchContent)


# Catch2 Testing framework
# --------------------------
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(Catch2)

set(MAIN_FILE "src/main.cpp")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SRC_FILES EXCLUDE REGEX "src/main.cpp")
file(GLOB_RECURSE TEST_FILES "src/*.test.cpp")

add_executable(tests ${SRC_FILES} ${TEST_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
# --------------------------

# Executable
# --------------------------
project(HelloWorld)

# Add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${MAIN_FILE})
# --------------------------
