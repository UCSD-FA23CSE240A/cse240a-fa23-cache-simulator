# Project 2:  The Cache Simulator

In the second project, we are now going to simulate the behavior of the cache in a processor. This cache will be somewhat similar to the cache present in a real processor. You will simulate an Instruction cache, a Data Cache and a Level 2 cache which is common for both instructions and data. 

## The Assignment:
We have given a framework for running the cache functions in `main.cpp`. You will create required data structures for the 3 caches, and functions to access these caches. Each cache will have 3 configuration settings - number of sets, associativity and hit time. You will need to calculate the statistics based on your understanding of the cache.


## Get Started
Accept the assignment in Github Classroom.

https://classroom.github.com/a/dWnjIYGa

Once you have checked out this repository, start adding your code into this. To compile, run the following command from within the src directory: make all or make. This will compile your code and generate output files. You will also get a executable binary called "cache". 

## Testing
Once you have created the binary, you can run it with the following command:
`bunzip2 -kc trace.bz2 | ./cache <options>`
The options are as follows:
```
  --help                     Print this message
  --icache=sets:assoc:blocksize:hit    I-cache Parameters
  --dcache=sets:assoc:blocksize:hit    D-cache Parameters
  --l2cache=sets:assoc:blocksize:hit   L2-cache Parameters
  --inclusive                Makes L2-cache be inclusive
  --prefetch                 Enable Prefetching
  --memspeed=latency         Latency to Main Memory
```

The testing can be done using the traces given to you in the repository. There are 2 correct outputs given for 2 configurations as follows:
1. **MIPS R10K** - [Reference Manual](https://ieeexplore.ieee.org/abstract/document/491460?casa_token=xRyemPMXCU4AAAAA:qMm86PcKveY_y6TAegQChllzSccO4b6ILZRKKEeO_ml4HjQfav6hBbHDJeHR0TeXZCUPyjOpFQ):
   * I$: 32KB, 2-way, 2 cycles hit latency
   * D$: 32KB, 4-way, 2 cycles hit latency
   * L2: 128KB, 8-way, off-chip, 50 cycles hit latency
   * 128B block size
   * `./cache --icache=128:2:128:2 --dcache=64:4:128:2 --l2cache=128:8:128:50 --memspeed=100`
2. **Alpha A21264** - [Reference Manual](https://course.ece.cmu.edu/~ece447/s15/lib/exe/fetch.php?media=21264hrm.pdf):
   * I$: 64KB, 2-way, 2 cycles hit latency
   * D$: 64KB, 4-way, 2 cycles hit latency
   * L2: 8MB, 8-way, off-chip, 50 cycles hit latency
   * 64B block size
   * `./cache --icache=512:2:64:2 --dcache=256:4:64:2 --l2cache=16384:8:64:50 --memspeed=100`

Please note that we **will not** test on inclusive L2 cache, so you can assume non-inclusive L2 by default. But you're welcome to inclusive L2 on your own.

You need to make sure that your output matches this configuration output with 2% of error margin. There will be some more hidden test cases which will test the simulator against some other configurations and other traces. 

We use the docker image provided by gradescope to build autograder, so you can test on it to make sure your code work on the same environment as the autograder. The image name is `gradescope/autograder-base` The commands to run this would be the same as before: `docker pull gradescope/autograder-base` to pull the image, `docker run -i -t gradescope/autograder-base /bin/bash` to run it on your local machine.

## Prefetching
You also need implement prefetching that enhance the performance of the cache access. 

- You're **allowed** to prefetch **one** additional cache line per memory access and you can assume the prefetching operation is instantaneous (the access time is not counted). 
- You're **not allowed** to create additional storage (prefetch buffer) to store the prefetched data, but you can have some data structure to identity the prefetching pattern.

The baseline prefetcher in the starter code is next line prefetcher, and you goal is to create a prefetcher that is better than this.

## Traces

Your simulator will model a cache hierarchy based on traces of real programs.Each line in the trace file contains the program counter of the memory access instruction, the address of a memory access in hex as well as where the access should be directed, either to the I\$ (I) or D\$ (D), and either the access is READ (R) or WRITE (W).


```
<Program Counter> <Address> <I or D> <R or W>

Sample Trace from bzip2.bz2:
0x790dc030	0x790dc030	I	R
0x790dc034	0x790dc034	I	R
0x790dc034	0x3970a3f0	D	W
0x790dc035	0x790dc035	I	R
0x790dc038	0x790dc038	I	R
0x790dc038	0x3970a3e8	D	W
0x790dc03a	0x790dc03a	I	R
0x790dc03a	0x3970a3e0	D	W
0x790dc03c	0x790dc03c	I	R
0x790dc03c	0x3970a3d8	D	W
0x790dc03e	0x790dc03e	I	R
0x790dc03e	0x3970a3d0	D	W
0x790dc040	0x790dc040	I	R
```


## Academic Integrity
This assignment is to be done individually by every student. Please make sure you do not copy a single line of code from any source. Not from other students, not from the web, not from anywhere. We have very sophisticated tools to discover if you did. This is a graduate class and we have the very highest expectations for integrity. You should expect that if you do so, even in very small amounts, you will be caught, you will be asked to leave the program, and if an international student, required to leave the country. 

## Turning it in
We will be taking only your cache.cpp and cache.hpp files. You can add the entire repository, but you need to have a folder structure like the following at least:

```
├── 📂 src
|   ├── 📄 cache.cpp
|   ├── 📄 cache.hpp
```

You can, of course, submit the entire repository, but we will look for only these files, and all the remaining files used will be our own. We will run the following commands for grading: `make clean` followed by `make`
