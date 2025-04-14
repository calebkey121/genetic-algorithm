# Minimum CMake version required (C++17 features like filesystem need ~3.14+)
cmake_minimum_required(VERSION 3.14)

# Define the project name and language
project(GeneticAlgorithmCacheOptimization LANGUAGES CXX)

# Set the C++ standard required (C++17 for <filesystem>)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Optional: Disable compiler-specific extensions for better portability
# set(CMAKE_CXX_EXTENSIONS OFF)

# --- Configuration ---
# Optional: Add definition for constants file (useful if constants need guards)
# add_definitions(-DHAVE_CONSTANTS_H)

# --- Include Directories ---
# Add the 'include' directory so CMake finds your header files
include_directories(include)

# --- Find Source Files ---
# Automatically find all .cpp files in the 'src' directory
file(GLOB SOURCE_FILES "src/*.cpp")

# --- Define Executable ---
# Create an executable named after the project, using the found source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# --- Link Libraries ---
# <filesystem> support might require explicitly linking stdc++fs on some older systems/compilers (e.g., GCC < 9).
# Modern compilers usually handle this automatically when CMAKE_CXX_STANDARD 17 is set.
# Try building without this first. If you get linker errors related to std::filesystem, uncomment the relevant lines below.
#
# Example for GCC < 9:
# if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
#   target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
# endif()
#
# Example for linking on all systems (if needed):
# find_library(STDCPPFS stdc++fs)
# if(STDCPPFS)
#   target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
#   message(STATUS "Linking against stdc++fs")
# else()
#   message(WARNING "stdc++fs library not found, filesystem support might be missing.")
# endif()

# --- Optional: Improve Build Experience ---

# Add compiler warnings (recommended for catching potential issues)
if(MSVC)
  # Microsoft Visual C++ specific warnings
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  # GCC/Clang specific warnings
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Suggest build types like Debug or Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()

# --- Output ---
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCE_FILES}")

# --- Installation ---
# Optional: Define installation rules if you want to install the executable
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)