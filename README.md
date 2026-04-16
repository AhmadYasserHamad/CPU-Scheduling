# CPU Scheduling Simulator

## Overview
This repository contains a C++ implementation of a CPU scheduling simulator designed to model and evaluate operating system scheduling algorithms, with a specific focus on Shortest Remaining Time (SRT) and Aging (AGE) algorithms. The project includes automated testing against predefined test cases and is containerized using Docker for consistent execution across different environments.

## Features
- **Algorithm Implementation:** Simulates core CPU scheduling algorithms (SRT and Aging) to manage process execution.
- **Performance Metrics:** Calculates standard scheduling metrics including turnaround time, waiting time, and CPU utilization.
- **Automated Testing:** Utilizes a shell script (`run_tests.sh`) to automatically validate the simulator's output against expected results.
- **Containerization:** Features a pre-packaged Docker environment for immediate deployment and testing without requiring local dependency configuration.
- **Streamlined Compilation:** Includes a `Makefile` for efficient compilation of the C++ source code.

## Repository Structure
- `lab7.cpp` & `lab7.h`: Core source code and header files containing the scheduling logic, process structures, and simulator classes.
- `/testcases`: Directory containing input files (e.g., process arrival times, burst times) and the corresponding expected output files for validation.
- `Makefile`: Build instructions for compiling the C++ program.
- `run_tests.sh`: Automated shell script to execute the compiled program against all provided test cases.
- `Dockerfile`: Configuration file for building the project's Docker image.
- `Autograder_Output.png`: Visual verification of the simulator passing automated autograder test cases.

## Prerequisites
To build and run this project locally, the following tools are required:
- A C++ compiler (e.g., GCC/G++)
- Make
- Docker (optional, required only for containerized execution)

## Local Build and Execution

1. Clone the repository:
   ```bash
   git clone [https://github.com/AhmadYasserHamad/CPU-Scheduling.git](https://github.com/AhmadYasserHamad/CPU-Scheduling.git)
   ```
   ```bash
   cd CPU-Scheduling
2. Compile the source code using Make:
   ```bash
   make
   ```
3. Run the automated test script to evaluate the simulator against all test cases:
   ```bash
   ./run_tests.sh
   ```
## Docker Environment

A pre-built Docker image is available on Docker Hub, allowing you to run the simulator in an isolated environment without compiling it locally.

**Docker Hub Repository:**
[ahmadyasserhamad/cpu-scheduling](https://hub.docker.com/repository/docker/ahmadyasserhamad/cpu-scheduling/general)

To run the simulator via Docker:

1. Pull the Docker image:
   ```bash
   docker pull ahmadyasserhamad/cpu-scheduling:general
   ```
2. Run the container interactively:
   ```bash
   docker run -it ahmadyasserhamad/cpu-scheduling:general

## Testing and Validation
The simulator's logic and metric calculations have been rigorously tested and validated against an external autograder system.
