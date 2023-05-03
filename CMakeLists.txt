cmake_minimum_required(VERSION 3.22)
project(ma_ya_ma)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CARGO_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src-tauri)
set(CARGO_TARGET_DIR ${CARGO_SOURCE_DIR}/target)
set(CXXBRIDGE_DIR ${CARGO_TARGET_DIR}/cxxbridge)

file(MAKE_DIRECTORY ${CXXBRIDGE_DIR}/rust ${CXXBRIDGE_DIR}/src)

add_custom_command(
  OUTPUT ${CXXBRIDGE_DIR}/rust/cxx.h
  COMMAND cxxbridge --header > ${CXXBRIDGE_DIR}/rust/cxx.h
  USES_TERMINAL
  COMMENT "Generating rust/cxx.h...")

add_custom_command(
  OUTPUT ${CXXBRIDGE_DIR}/src/lib.rs.cc
  COMMAND cxxbridge ${CARGO_SOURCE_DIR}/src/lib.rs >
          ${CXXBRIDGE_DIR}/src/lib.rs.cc
  DEPENDS ${CARGO_SOURCE_DIR}/src/lib.rs
  USES_TERMINAL
  COMMENT "Bridging src/lib.rs...")

file(GLOB SOURCE_FILES ${CARGO_SOURCE_DIR}/src/*.cc)
set(CXXBRIDGE_FILES ${CXXBRIDGE_DIR}/rust/cxx.h ${CXXBRIDGE_DIR}/src/lib.rs.cc)

add_library(ma_ya_ma STATIC ${SOURCE_FILES} ${CXXBRIDGE_FILES})

target_include_directories(ma_ya_ma PRIVATE src-tauri/
                                            src-tauri/target/cxxbridge/)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/release/ma-ya-ma)
  set(TAURI_BUILD_COMMAND pnpm run tauri build)
else()
  set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/debug/ma-ya-ma)
  set(TAURI_BUILD_COMMAND pnpm run tauri build --debug)
endif()

file(GLOB CARGO_SOURCE_FILES ${CARGO_SOURCE_DIR}/src/*.rs)

add_custom_command(
  OUTPUT ${CARGO_BINARY_FILE}
  COMMAND ${TAURI_BUILD_COMMAND}
  DEPENDS ${CARGO_SOURCE_FILES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  USES_TERMINAL
  COMMENT "Running Cargo...")

add_custom_target(
  ma-ya-ma ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CARGO_BINARY_FILE} ./ma-ya-ma
  DEPENDS ${CARGO_BINARY_FILE})
