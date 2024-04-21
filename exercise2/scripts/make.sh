#!/bin/bash 

# Parse options
while getopts "abc" opt; do
  case $opt in
    a) # Load the OpenMPI module in the cluster
      module load openMPI/4.1.5/gnu/12.2.1
      ;;
    b) # Build binary files with CMake
      cd ..
      cmake -S . -B build || { echo "⛔ CMake failed."; exit 1; }
      cmake --build build -j12 || { echo "⛔ Make failed."; exit 1; }
      if [ $? -eq 0 ]; then
        echo "✅ Executable successfully created in the build/bin directory."
      else
          echo "⛔ Failure in builing th executable. Consider using option -b."
      fi
      ;;
    c) # Build binary files with a simple command line
      mkdir ../bin
      mpicc -fopenmp -O3 -march=native ../apps/main.c ../src/mandelbrot.c -I../include/ -o ../bin/mandelbrot
      if [ $? -eq 0 ]; then
        echo "✅ Executable successfully created in the bin directory."
      else
          echo "⛔ Failure in builing th executable."
      fi
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done