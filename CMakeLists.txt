# CMakeList.txt : CMake project for Othello, include source and define
# project specific logic here.
#

# Global CMake settings
cmake_minimum_required (VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (
	Othello
	VERSION 0.1
	DESCRIPTION "Othello game implementation"
)

# Add source to this project's executable.
add_executable (Othello)
# Only for legacy includes
target_include_directories(Othello PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(Othello
    PRIVATE
        src/model/board.cpp
        src/controller/controller.cpp
        src/view/tui.cpp
    PUBLIC
        include/model/board.cppm
)



# MVSC specific settings
if(MSVC)
  add_compile_options(/W4)
  # Enable Hot Reload for MSVC compilers if supported.
  if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endif()


# If Doxygen is found, configure the Doxyfile and create a custom target
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Add Google Test for unit testing
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

if(TARGET gtest_main) 
    add_executable(unit_tests tests/unit_tests.cpp)
    target_link_libraries(unit_tests gtest_main)
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
    # Silence clang-tidy warnings from googletest
    set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")
endif()


