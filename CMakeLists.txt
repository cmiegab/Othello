# CMakeList.txt : CMake project for Othello, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 4.0)



# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project (
	Othello
	VERSION 0.1
	DESCRIPTION "Othello game implementation"
)

# Add source to this project's executable.
add_executable (Othello "Othello.cpp" "Othello.h")

set_property(TARGET Othello PROPERTY CXX_STANDARD 23)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

target_compile_features(Othello PRIVATE cxx_std_23)
target_compile_options(Othello PRIVATE /W4)


# TODO: Add tests and install targets if needed.
