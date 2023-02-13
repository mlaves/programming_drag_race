# Programming language drag race

In this repository, I implemented a simple optimization problem and run it in a bootstrapping
fasion to approximate confidence intervals for the estimated parameters. I use this to drag race
different programming languages by estimating their runtime. I might add different languages
or benchmark programs at a later point. Please create an issue if you spot any further possible
optimizations of my code.

## Results

These results were measured on a MacBook Pro 13" 2018 with a 2.3 GHz Quad-Core Intel Core i5 and
16 GB of RAM.

|               | Python  | Python+numba | Julia   | C/C++   |
|---------------|---------|--------------|---------|---------|
| bootstrapping | 8177 ms | 2845 ms      | 3034 ms | 3543 ms |
