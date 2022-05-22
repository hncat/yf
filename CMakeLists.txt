cmake_minimum_required(VERSION 3.16)
project(yf VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_language(C ASM)

message(STATUS "executable file path: ${CMAKE_SOURCE_DIR} $")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ASM_CREATE_SHARED_LIBRARY True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/yf)

list(APPEND fiber_source
    asm/jump_x86_64_sysv_elf_gas.s
    asm/make_x86_64_sysv_elf_gas.s
)

list(APPEND _source
    yf/mutex.cc
    yf/util.cc
    yf/thread.cc
    yf/timestamp.cc
    yf/exception.cc
    yf/fiber.cc
    yf/config.cc
    yf/log/appender.cc
    yf/log/loglevel.cc
    yf/log/basiclayout.cc
    yf/log/simplelayout.cc
    yf/log/passthroughlayout.cc
    yf/log/patternlayout.cc
    yf/log/ostreamappender.cc
    yf/log/fileappender.cc
    yf/log/category.cc
    yf/log/categorymanager.cc
    yf/log/logstream.cc
    yf/json/json_reader.cpp
    yf/json/json_value.cpp
    yf/json/json_valueiterator.inl
    yf/json/json_writer.cpp
)

option(USE_STATIC_LIBRARY "whether to create a static library" OFF)

if(USE_STATIC_LIBRARY)
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "create static library")
else()
    set(BUILD_SHARED_LIBS ON)
    message(STATUS "create shared library")
endif()

add_library(fiber_library STATIC ${fiber_source})
set_target_properties(fiber_library
    PROPERTIES
    OUTPUT_NAME
    "fiber"
)
target_compile_options(fiber_library
    PRIVATE
    "-Wall"
)

add_library(yf_library ${_source})
target_link_libraries(yf_library fiber_library)
set_target_properties(yf_library
    PROPERTIES
    OUTPUT_NAME
    "yf"
)
target_compile_options(yf_library
    PRIVATE
    "-Wall"
)

target_link_libraries(yf_library pthread)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_executable(mutex test/test_mutex.cc)
add_executable(thread test/test_thread.cc)
add_executable(log test/test_log.cc)
add_executable(exception test/test_exception.cc)
add_executable(config test/test_config.cc)

target_link_libraries(mutex yf_library)
target_link_libraries(thread yf_library)
target_link_libraries(log yf_library)
target_link_libraries(exception yf_library)
target_link_libraries(config yf_library)
