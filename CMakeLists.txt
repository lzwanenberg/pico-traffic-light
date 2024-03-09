cmake_minimum_required(VERSION 3.28.2)

function(print_file_list FILE_LIST_NAME)
    message("${FILE_LIST_NAME}:")
    foreach(FILE ${${FILE_LIST_NAME}})
        message("  ${FILE}")
    endforeach()
endfunction()

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

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "src/*.hpp")
list(FILTER HEADER_FILES EXCLUDE REGEX ".*\\.test\\.hpp$")
list(FILTER HEADER_FILES EXCLUDE REGEX "${MAIN_FILE}")
list(FILTER HEADER_FILES EXCLUDE REGEX "${PICO_DIR}/*")

file(GLOB_RECURSE PICO_HEADER_FILES CONFIGURE_DEPENDS "${PICO_DIR}/*.hpp")
file(GLOB_RECURSE PICO_SRC_FILES CONFIGURE_DEPENDS "${PICO_DIR}/*.cpp")
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "src/*.test.cpp")

print_file_list(HEADER_FILES)
print_file_list(SRC_FILES)
print_file_list(PICO_HEADER_FILES)
print_file_list(PICO_SRC_FILES)
print_file_list(TEST_FILES)

if ("${TARGET}" STREQUAL "tests")
  project(pico-traffic-light C CXX ASM)

  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)

  add_executable(tests ${HEADER_FILES} ${SRC_FILES} ${TEST_FILES})
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
endif ()

if ("${TARGET}" STREQUAL "release")
  set(PICO_SDK_FETCH_FROM_GIT on)
  include(pico_sdk_import.cmake)

  project(pico-traffic-light C CXX ASM)

  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)

  pico_sdk_init()

  add_executable(${PROJECT_NAME} ${PICO_HEADERFILES} ${PICO_SRC_FILES} ${HEADER_FILES} ${SRC_FILES} ${MAIN_FILE})

  pico_add_extra_outputs(${PROJECT_NAME})

  target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_cyw43_arch_none
  )
endif ()
