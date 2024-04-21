set(LIBRARY_OUTPUT_NAME mandelbrot_lib)

set(SOURCE_FILES
  mandelbrot.c
)

#0. SETTING THE OUTPUT DIRECTORY OF THE LIBRARIES---------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#1. FIND EXTERNAL LIBRARIES INSTALLED IN THE SYSTEM-------------------------------
# Set compiler flags for OpenMP
find_package(OpenMP REQUIRED)

#2. INCLUDE DIRECTORIES TO LOOK FOR HEADERS FILES---------------------------------

set(INCLUDE_DIRS 
  ${CMAKE_SOURCE_DIR}/include 
)

include_directories(${INCLUDE_DIRS})

#3. CREATE LIBRARIES'S OBJECT FILES-----------------------------------------------
add_library(${LIBRARY_OUTPUT_NAME} SHARED ${SOURCE_FILES}) 

#Set the version of the libraries
set_target_properties(${LIBRARY_OUTPUT_NAME} PROPERTIES VERSION 1.0.0 SOVERSION 1)

# Set compiler flags for OpenMP
target_link_libraries(${LIBRARY_OUTPUT_NAME} PRIVATE OpenMP::OpenMP_C)