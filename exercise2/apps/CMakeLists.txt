# Find MPI
find_package(MPI REQUIRED)
# Find OpenMP
find_package(OpenMP REQUIRED)

# Set the executable name and source files
set(EXECUTABLE_NAME mandelbrot)
set(SOURCE_FILES main.c)

# Set the output path of the executables: bin/
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Include header files for the executables
set(INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/include # headers of custom libraries
)

# Include the directories that will be linked to our executables
include_directories(SYSTEM ${MPI_INCLUDE_PATH}
                           ${INCLUDE_DIRS})

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Set compiler flags
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
                      COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS}"
                      LINK_FLAGS "${MPI_C_LINK_FLAGS}")

# Link libraries to the executables
target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenMP::OpenMP_C mandelbrot_lib ${MPI_C_LIBRARIES})
