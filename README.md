# FuzzDistillCC

## Project Structure
```text
.
├── JulietBC
├── nist_cpp_juliet1.3
├── src
└── test
```


## Build

### Prerequisite

Ensure you have the latest versions of LLVM, CMake, Ninja, and Clang installed on your system. You can install these
tools using package managers or by downloading them from their respective websites.

- On Ubuntu/Debian:
  ```sh
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install cmake ninja-build
  ```
  For LLVM18 (FuzzDistillCC should build on any LLVM15+):
  ```shell
  wget https://apt.llvm.org/llvm.sh
  chmod +x llvm.sh
  sudo ./llvm.sh 18 all
  ```

- On macOS (using Homebrew):
  ```sh
  brew install llvm@18 cmake ninja
  ```
  > After installation in macOS, you might need to follow on screen instruction to add llvm bin and include paths in your environment.

### Build FuzzDistillCC
- Clone
    ```shell
    git clone https://github.com/Saket-Upadhyay/FuzzDistillCC.git
    ```
- Build
    ```shell
    cd FuzzDistillCC
    mkdir build
    cmake -B build -S . -GNinja
    ninja -C build
    ```

### Artifacts

After the build you will have the following artifacts in `build` directory:
1. `build/src/fuzzdistillcc` standalone tool binary
2. `build/src/FnFeaturesPass/libFnFeaturesPass.a` Function feature extraction pass. Can be side-loaded with opt and clang as standalone pass.
3. `build/src/BBFeaturesPass/libBBFeaturesPass.a` Basic Block feature extraction pass. Can be side-loaded with opt and clang as standalone pass.

## Use
There are two ideal ways use the passes, you can either use the standalone binary on compiled llvm-bc or IR files, or you can add the passes in your existing compiler pipeline (aflcc, clang, etc.).

The passes do not change the bitcode, you should invalidate all analysis after run.

For best results, add the passes to the end of pipeline. This makes sure that nothing significant is changed in the code post feature extraction.

### Use fuzzdistillcc
```shell
./fuzzdistillcc <Module to analyze>
```

This will generate 2 SSV files with `*.csv` extension.
Namely, `<module_name>_BBFeatures.csv` and `<module_name>_FunctionFeatures.csv`.

## Test

The `test` folder contains a simple code with a `Makefile` and a symlink to `../build/src/fuzzdistillcc`.
Run the following to check if the tool is working correctly.
```shell
make all
```
This will generate two csv files and some intermediate files to inspect if you wish.
