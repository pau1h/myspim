# MySPIM

**MySPIM** is a 32-bit MIPS processor simulator designed to help users understand and experiment with the MIPS architecture. This simulator allows for the execution of MIPS assembly language programs, providing an educational tool for learning the inner workings of the MIPS instruction set and processor architecture. 

## Features

- **Instruction Set Support**: Simulates a wide range of MIPS instructions including arithmetic, logic, control, and memory access instructions.
- **Debugging Tools**: Step through instructions, set breakpoints, and view register and memory contents.
- **User Interface**: Intuitive command-line interface for interacting with the simulator.

## Installation

To install MySPIM, clone the repository and build the project using the following commands. Ensure you have `gcc` installed on your system.

```bash
# Clone the repository
git clone https://github.com/pau1h/MySPIM.git

# Navigate to the project directory
cd MySPIM

# Build the project
gcc -o myspim spimcore.c project.c
```

## Usage

After building the project, you can run the simulator with the following command:

```bash
./myspim <assembly_file.asm>
```

Example program can be found in the examples/ folder

## Acknowledgements
This project was made for my CDA3103 class at the University of Central Florida. Credit to my professor, John Aedo, for providing spimcore.h, spimcore.c, and for giving me all the knowledge I learned in that class!

## Contributing
This is just a fun project that I worked on, but if anyone wants to contribute feel free to submit a pull request.
