## About

- Project Title : Generation of Isomorphic Cellular Automata
- Student : Tarun Dittakavi
- Degree : Master of Technology
- Guide : Dr. Kamalika Bhattacharjee
- Department : Computer Science and Engineering
- Institute : National Institute of Technology, Tiruchirappalli

## Minimal System Requirements

- Ubuntu 22.04
- GCC 11.4.0 Compiler
- GNU Make
- OpenMP (included by default with GCC 4.2.0 and above)

## Execution Instructions

In the project directory, open the terminal and run the below command to compile all the files.

```sh
make
```

If there are no errors while compilation, a binary executable file named `main` will be created, along with several `.o` files. Within the same terminal, run the below command to start the application.

```sh
./main
```

Once the execution begins, follow the menu driven program to explore and quit the application. You can delete all the compiled files, as well as the binary executable file, by running the below command in the project directory.

```sh
make clean
```

<br />

> **NOTE** : Running the exhaustive search method - for printing isomorphisms, as well as for checking isomorphism with another automaton - will take a substantially large amount of time for automata of sizes 4 and above. It is advised to not run these two methods for automata of sizes 4 and above. In case you start running them, you can use `Ctrl + C`, to force quit the application. **Menu items that invoke these methods are marked with an asterisk (\*)**.
