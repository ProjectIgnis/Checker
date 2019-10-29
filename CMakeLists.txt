cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ignis-tests)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(ocgcore)

add_executable(script_syntax_check)
target_include_directories(script_syntax_check PUBLIC ocgcore)
target_link_libraries(script_syntax_check PUBLIC ocgcore)
target_compile_options(script_syntax_check PRIVATE ${OCGCORE_COMPILE_OPTIONS})
target_sources(script_syntax_check PRIVATE script_syntax_check)
