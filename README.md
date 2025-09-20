# de3 
This is the tool for our ASE 2024 paper "*Shoot Yourself in the Foot -- Efficient Code Causes Inefficiency in Compiler Optimizations*".

**de3** is an automated defects detector for compiler optimizations. Given a valid LLVM-IR program, **de3** can perform deoptimizations randomly on it and produce a new valid equivalent program. 

## Structure of the project
```
  |-- build                   # The binary of performance checker and mutation
  |-- scripts                 # The python driver of modules of mutation and performance checker 
  |   |-- run.py              # The implementation of random mutation of given program
  |   |-- reduce_all.py       # The reduce script
  |   | -- xxxx.py            # Other helper scripts
  |-- src                     # The implementation of performance checker and mutation
```

## Use de3
**Step1: Install necessary packages**

- Linux
- Python >= 3.10.12
- GCC/G++ >= 13.1.0 (or any C++ compiler supporting C++20)

- llvm-project: commit at [37eb9c9632fb5e82827d1a0559f2279e9a9f1969](https://github.com/llvm/llvm-project/commit/37eb9c9632fb5e82827d1a0559f2279e9a9f1969) or newer. Below is the suggested build script.
  ```
  git clone https://github.com/llvm/llvm-project.git /path/to/llvm-project
  cd /path/to/llvm-project
  git checkout 37eb9c9632fb5e82827d1a0559f2279e9a9f1969
  sudo cmake -GNinja -Bbuild -Hllvm \
    -DLLVM_ENABLE_PROJECTS="clang" \
    -DLLVM_TARGETS_TO_BUILD="all" \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=true \
    -DLLVM_CCACHE_BUILD=true \
    -DLLVM_USE_LINKER=lld
  sudo cmake --build ./build --target install
  ```
  

**Step2: Compile the project**

```sh
cd /path/to/de3
cmake .
make
```

**Step3: Use de3**

```sh
python3 ./scripts/run.py -c ./config/O3_O3_deoptimize.toml -o /path/to/output /path/to/input
```
`/path/to/output` is the directory where **de3** put the detection result.
`/path/to/input` is the directory containing the seeds in form of llvm-ir.
This script will mutate programs in `/path/to/input`, and then invoke `opt` to optimize them, and finally apply performance checkers to filter them. All testcases related to missed optimization are put into `/path/to/input/missed-opt`.

To reduce the seed programs in `/path/to/output`, we apply:

```sh
python3 ./scripts/reduce_all.py /path/to/output/missed-opt
```

This script invokes `llvm-reduce` to reduce seeds in `/path/to/output/missed-opt`.

## More

### Mutate a given program with deoptimizations
We also provide a tool for directly mutating a given seed program in form of llvm-ir.
The seed program can be indeterministic and inexecutable,
as long as it is valid to compile.

You can mutate a given program by running:
```sh
./build/deoptgen -pipeline-type=1 -m 15 /path/to/seed -o /path/to/mutant
```

`-m 15` means that the maximum number of mutation passes is 15. The mutant of `/path/to/seed` will be put into `/path/to/mutant`.

For normal C program `a.c`, you can run:

```sh
./scripts/c2llvm.sh a.c /path/to/seed
./build/deoptgen -pipeline-type=1 -m 15 /path/to/seed -o /path/to/mutant
```



## USE de3 in docker
Under src code directory of de3:
docker build -t my_image .
docker run -it -v /path/on/host:/path/in/container my_image /bin/bash

Then use de3 in docker:
python3 ./scripts/run.py -c ./config/O3_O3_deoptimize.toml -o /path/to/output /path/to/input
