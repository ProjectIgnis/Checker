cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CXX_STANDARD 14)
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
target_sources(script_syntax_check PRIVATE script_syntax_check)
set_property(TARGET script_syntax_check PROPERTY BUILD_RPATH_USE_ORIGIN TRUE)
set_property(TARGET script_syntax_check PROPERTY BUILD_RPATH .)

set(CLANG_OR_GNU $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>>)
set(IGNIS_TESTS_COMPILE_OPTIONS
	# Clang and GNU compilers
	$<${CLANG_OR_GNU}:-Wall -Wextra -pedantic>
	$<$<AND:$<CONFIG:Debug>,${CLANG_OR_GNU}>:-ggdb3 -Og>
	$<$<AND:$<CONFIG:Release>,${CLANG_OR_GNU}>:-O3>
	$<$<AND:$<CONFIG:RelWithDebInfo>,${CLANG_OR_GNU}>:-ggdb3 -O3>
	# Microsoft compiler
	$<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(script_syntax_check PRIVATE ${IGNIS_TESTS_COMPILE_OPTIONS})

