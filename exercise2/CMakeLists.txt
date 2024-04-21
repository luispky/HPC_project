cmake_minimum_required(VERSION 3.22)
project(Mandelbrot_MPI_OpenMP
  VERSION 1.0
  DESCRIPTION "High Performance Computing project"
  LANGUAGES C
)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Compiler flags for all build types (Debug, Release, etc.)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

# Compiler flags for the Release build type
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native")

add_subdirectory(src)  
add_subdirectory(apps) 
