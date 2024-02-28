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

add_executable(tests src/factorial.cpp src/factorial.test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
# --------------------------

# Executable
# --------------------------
project(HelloWorld)

# Add the executable
add_executable(${PROJECT_NAME} src/factorial.cpp src/main.cpp)
# --------------------------
