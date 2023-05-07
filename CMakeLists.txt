cmake_minimum_required(VERSION 3.22)
project(ma_ya_ma)

set(CMAKE_CXX_STANDARD 14)

set(CARGO_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src-tauri)
set(CARGO_TARGET_DIR ${CARGO_SOURCE_DIR}/target)
set(CXXBRIDGE_DIR ${CARGO_TARGET_DIR}/cxxbridge)
set(CPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src-cpp)

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
  COMMAND cxxbridge ${CARGO_SOURCE_DIR}/src/lib.rs --header >
          ${CXXBRIDGE_DIR}/src/lib.rs.h
  DEPENDS ${CARGO_SOURCE_DIR}/src/lib.rs
  USES_TERMINAL
  COMMENT "Bridging src/lib.rs...")

file(GLOB SOURCE_FILES ${CPP_SOURCE_DIR}/*.cc)
set(CXXBRIDGE_FILES ${CXXBRIDGE_DIR}/rust/cxx.h ${CXXBRIDGE_DIR}/src/lib.rs.cc)

add_library(ma_ya_ma STATIC ${SOURCE_FILES} ${CXXBRIDGE_FILES})

target_include_directories(
  ma_ya_ma PRIVATE ${CMAKE_SOURCE_DIR}/src-tauri/
                   ${CMAKE_SOURCE_DIR}/src-tauri/target/cxxbridge/)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(WIN32)
    set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/release/ma-ya-ma.exe)
  else()
    set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/release/ma-ya-ma)
  endif()
  set(TAURI_BUILD_COMMAND pnpm run tauri build)
else()
  if(WIN32)
    set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/debug/ma-ya-ma.exe)
  else()
    set(CARGO_BINARY_FILE ${CARGO_TARGET_DIR}/debug/ma-ya-ma)
  endif()
  set(TAURI_BUILD_COMMAND pnpm run tauri build --debug)
endif()

file(GLOB CARGO_SOURCE_FILES ${CARGO_SOURCE_DIR}/src/*.rs)

add_custom_command(
  OUTPUT ${CARGO_BINARY_FILE}
  COMMAND corepack enable
  COMMAND pnpm i
  COMMAND ${CMAKE_COMMAND} -E env CMAKE_TARGET_LIBRARY=$<TARGET_FILE:ma_ya_ma>
          ${TAURI_BUILD_COMMAND}
  DEPENDS ${CARGO_SOURCE_FILES} ma_ya_ma
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  USES_TERMINAL
  COMMENT "Running Cargo...")

add_custom_target(
  ma_ya_ma-bin ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CARGO_BINARY_FILE} ./ma-ya-ma
  DEPENDS ${CARGO_BINARY_FILE})

if(WIN32)
  target_link_libraries(ma_ya_ma userenv ws2_32 bcrypt)
  set_target_properties(
    ma_ya_ma
    PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
               RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
               RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
endif()
