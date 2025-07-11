# CMakeList.txt : CMake project for Othello, include source and define
# project specific logic here.
#

# Global CMake settings
cmake_minimum_required (VERSION 3.31)

project (
	Othello
	VERSION 1.0
	DESCRIPTION "Othello game implementation"
)


find_package(Qt6 REQUIRED COMPONENTS Core Quick Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(SQLite3 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_SCAN_FOR_MODULES ON)
include(CheckCXXCompilerFlag)

# MSVC specific settings
if(MSVC)
  add_compile_options(/std:c++latest /W4)
  check_cxx_compiler_flag("/arch:AVX2" COMPILER_SUPPORTS_AVX2)
  if(COMPILER_SUPPORTS_AVX2)
    add_compile_options(/arch:AVX2)
    add_definitions(-D__AVX2__)
    add_compile_definitions(HAVE_AVX2_SUPPORT)
    message(STATUS "AVX2 support detected and enabled")
  else()
    message(WARNING "AVX2 not supported by the compiler/target architecture")
  endif()

  # Enable Hot Reload for MSVC compilers if supported.
  if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endif()


# Add source to this project's executable.
add_executable (Othello)
target_link_libraries(Othello PRIVATE SQLite::SQLite3 Qt6::Core Qt6::Gui Qt6::Quick)
# Only for legacy includes
target_include_directories(Othello PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(Othello
  PUBLIC
    FILE_SET CXX_MODULES FILES
      "include/model/board.cppm"
      "include/view/view.cppm"
      "include/view/tui.cppm"
      "include/repository/repository.cppm"
      "include/repository/file_repository.cppm"
      "include/repository/db_repository.cppm"
      "include/controller/controller.cppm"
    PRIVATE
        src/model/board.cpp
        src/controller/controller.cpp
        src/view/gui.cpp
        src/view/tui.cpp
        src/repository/db_repository.cpp 
        src/repository/file_repository.cpp
        othello.cpp
)



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
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
if(TARGET gtest_main) 
    add_executable(unit_tests ${TEST_SOURCES})
    target_sources(unit_tests
        PUBLIC
            FILE_SET CXX_MODULES FILES
            "include/model/board.cppm"
        PRIVATE
            src/model/board.cpp
    )
    target_link_libraries(unit_tests gtest_main)
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
    # Silence clang-tidy warnings from googletest
    set_target_properties(unit_tests PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")
endif()


