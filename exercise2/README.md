# HIGH PERFORMING COMPUTING 2023-2024 

## Exercise 2c

## Student's Info

Name: Luis Fernando Palacios Flores ([luf.palacios@gmail.com](mailto:luf.palacios@gmail.com))([luisfernando.palaciosflores@studenti.units.it](mailto:luisfernando.palaciosflores@studenti.units.it))

Student Number: 302004

Study Curriculum: SM38 DATA SCIENCE AND ARTIFICIAL INTELLIGENCE

## Project's organization

Appart form the files of the project assignment, this project is organized with the following structure:

```bash
.
├── apps # Code of the parallel application and a build file
│   ├── CMakeLists.txt
│   └── main.c
├── report # Report and Slides of the project
│   ├── PALACIOSFLORES_ex2c_slides.pdf
│   └── PALACIOSFLORES_ex2c_report.pdf
├── CMakeLists.txt # Root cmake file to build the parallel application executable
├── include # Header file of the parallel application
│   └── mandelbrot.h
├── plots # Plots
├── README.md # This file
├── results # Outputs of the parallel execution time obtained in the cluster
├── scripts # Files to make the application, submit the scaling jobs to the cluster and making the plots
│   ├── make.sh
│   ├── results-hybrid.ipynb
│   ├── results.ipynb
│   ├── strong_scaling_MPI_EPYC.sh
│   ├── strong_scaling_OMP_EPYC.sh
│   ├── weak_scaling_MPI_EPYC.sh
│   └── weak_scaling_OMP_EPYC.sh
└── src # Source file of the parallel application
    ├── CMakeLists.txt
    └── mandelbrot.c
```

## Compilation

The `make.sh` script provided in the `scripts` directory allows fast compilation of the executable of the parallel application. The file must be run with one of the two options available using the following syntax: 

```bash
./make.sh -<options>
```

The options performs the following operations: 

* `a`: Loads the OpenMPI module in the cluster. 
* `b`: Compile the code using the `cmake` build system inside a `build` directory. The executable then it located in the directory `build/bin`.
* `c`: Compile the code using manual instructions to build the executable. The executable then it located in the directory `bin/`.

In the cluster it is suggested to compile the code using the command `./make.sh -a -b` and in a local computer `./make.sh -c`. A proper version of OpenMPI must be installed in the local machine in order to compile the code. Furthermore, OpenMPI should be installed in the proper directory in order to use the command `./make.sh -b`.

## Usage

In order to use the parallel application it is enough to run it after the executable is built, with the selected option. The program has different optional input arguments from the console. These ordered arguments are:

* `N_THREADS`: number of OpenMP threads to use.
* `n_x`: number of pixels in the x-direction.
* `n_y`: number of pixels in the y-direction.
* `x_L`: leftmost x-coordinate of the complex plane.
* `y_L`: lowest y-coordinate of the complex plane.
* `x_R`: rightmost x-coordinate of the complex plane.
* `y_R`: highest y-coordinate of the complex plane.
* `I_max`: maximum number of iterations to be performed to determine if a complex number a region of the complex plane belongs or not to the Mandelbrot set.

## Measuring Scaling

In order to measure strong and weak scaling of the program in the ORFEO cluster the following files can be used:

* `strong_scaling_MPI_EPYC.sh`
* `strong_scaling_OMP_EPYC.sh`
* `weak_scaling_MPI_EPYC.sh`
* `weak_scaling_OMP_EPYC.sh`

It is worth mentioning that for using these files the executable must the compiled using the command `./make -a -b`. Otherwise, inside these files the variable `executable` has to be properly adjusted.
