# HIGH PERFORMING COMPUTING 2023-2024 

## Exercise 1

## Student's Info

Name: Luis Fernando Palacios Flores ([luf.palacios@gmail.com](mailto:luf.palacios@gmail.com))([luisfernando.palaciosflores@studenti.units.it](mailto:luisfernando.palaciosflores@studenti.units.it))

Student Number: 302004

Study Curriculum: SM38 DATA SCIENCE AND ARTIFICIAL INTELLIGENCE

## Project's organization

Appart form the files of the project assignment, this project is organized with the following structure:

```bash
.
├── plots # Plots
├── README.md # This file
├── report # Report and Slides of the project
│   ├── PALACIOSFLORES_ex1_slides.pdf
│   └── PALACIOSFLORES_ex1_report.pdf
├── results # Outputs of the latency obtained by running the collective operations in the cluster
└── scripts # Files to make the executables needed, run the executables in the cluster and get the results, and plot and analyze the results
    ├── barrier_latencies.sh
    ├── bcast_latencies.sh
    ├── make.sh
    └── results_and_modelling.ipynb
```

## Compilation

In this project I selected the broacast and barrier collective operations to compare the different algorithms used for their implementation. The `make.sh` script provided in the `scripts` directory allows fast compilation of the executables needed for benchmarking. It is enough to run the file **inside** the `scripts` directory with the following command:

```bash
./make.sh 
```

It will take some time to finish since it needs to download the library, install it and then copy the files into a `bin` directory. This new directory stores the executables and is where the `barrier_latencies.sh` and `bcast_latencies.sh` scripts look for them in order to perform benchmarking in the cluster.

## Benchmarking

Once logged in the cluster, it is sufficient to run the scripts that get the results inside the `scripts` directory.
