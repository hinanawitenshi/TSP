# Traveling Salesman Problem

The travelling salesman problem (TSP) is a popular NP-hard problem which can be a baseline for a lot of algorithms. For more details, check the [page](https://en.wikipedia.org/wiki/Travelling_salesman_problem) on Wikipedia.

This repository implements several algorithms for solving TSP in C. Current algorithms are

- Hill Climbing
- Random-Restart Hill Climbing
- Simulated Annealing

## Usage

Use `make` to run a algorithm and use `display.py` to display the result. For instance, run the SA by

```bash
$ make SA
```

The standard output can be recognized by `display.py`, so to see the result, one may do as follow,

```bash
$ make SA > result.txt
$ python src/display.py result.txt
```

The python snippet requires `matplotlib` and `numpy` to be installed.
