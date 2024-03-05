cmake_minimum_required(VERSION 3.28.2)

if(NOT TARGET)
    set(TARGET "release")
endif()

message("Building for target: ${TARGET}")

if ("${TARGET}" STREQUAL "tests")
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
  )

  FetchContent_MakeAvailable(Catch2)
endif ()

set(MAIN_FILE "src/main.cpp")
set(PICO_DIR "src/pico_w")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SRC_FILES EXCLUDE REGEX "${MAIN_FILE}")
list(FILTER SRC_FILES EXCLUDE REGEX "${PICO_DIR}/*")
file(GLOB_RECURSE PICO_FILES CONFIGURE_DEPENDS "${PICO_DIR}/*.cpp")
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "src/*.test.cpp")


message("SRC_FILES:")
foreach(src_file ${SRC_FILES})
    message("  ${src_file}")
endforeach()

message("PICO_FILES:")
foreach(pico_file ${PICO_FILES})
    message("  ${pico_file}")
endforeach()

message("TEST_FILES:")
foreach(test_file ${TEST_FILES})
    message("  ${test_file}")
endforeach()

if ("${TARGET}" STREQUAL "tests")
  project(pico-traffic-light C CXX ASM)

  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)

  add_executable(tests ${SRC_FILES} ${TEST_FILES})
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
endif ()

if ("${TARGET}" STREQUAL "release")
  set(PICO_SDK_FETCH_FROM_GIT on)
  include(pico_sdk_import.cmake)

  project(pico-traffic-light C CXX ASM)

  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)

  pico_sdk_init()

  add_executable(${PROJECT_NAME} ${PICO_FILES} ${SRC_FILES} ${MAIN_FILE})

  pico_add_extra_outputs(${PROJECT_NAME})

  target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_cyw43_arch_none
  )
endif ()
