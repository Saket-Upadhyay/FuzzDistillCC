# Reproduce NIST Juliet 1.3 Dataset

```text
JulietDatasetGen
├── CSVS
├── Juliet1_3LinuxBitcs
├── Juliet1_3MacBitcs
├── a_setupenv.sh
├── b_runonallbitc.py
├── c_consolidatefeatures.sh
├── fuzzdistillcc -> ../build/src/fuzzdistillcc
└── requirements.txt
```

### Build notes

There are two variables that you should look at in root `CMakeLists.txt`

```cmake
# Are we using FuzzDistillCC to generate a dataset?
set(ISNISTTRAINING 0)
# Generate Feature CSVs with (1) or without (0) header
set(WRITEHEADERS 1)
```
If you want to generate Juliet1.3 Dataset included in this project, set `ISNISTTRAINING` to `1` and set `WRITEHEADERS` to `0`.
This enables some checks in both passes to generate cleaner features. This implementation is NIST Juliet1.3 specific.

> **THE PROJECT NEEDS TO BE BUILT WITH THIS CONFIGURATION BEFORE GENERATING DATASET**

## Processing Bitcodes
Bitcodes compiled (on Nov'24) from NIST Juliet1.3 are at `JulietDatasetGen/Juliet1_3(Linux/Mac)Bitcs` for Linux and macOS respectively.
The helper scripts are named sequentially (a, b, c) and must be executed in that order.

To initiate the Python environment setup, execute the `a_setupenv.sh` script. This script will subsequently install the necessary dependencies, remove any residual CSV files, and create the required symlink.
```shell
cd JulietDatasetGen
./a_setupenv.sh
```

Subsequently, activate the newly created virtual environment and run `b_runonallbitc.py`. This script will execute `fuzzdistillcc` on Linux or Mac bitcodes based on the host operating system.
It will generate feature files for each bitcode in the corresponding directories.
```shell
source ./venv/bin/activate
python3 b_runonallbitc.py
```

## Generating Final Dataset
Finally, execute `c_consolidatefeatures.sh`. This script will combine all BB and Function feature data into two files: `bigBBFeatures.csv` and `bigFNFeatures.csv`, located in the `CSVS` directory.
```shell
./c_consolidatefeatures.sh
```

Now you can use the datasets in `CSVS` directory to train models in FuzzDistillML.

_**For further details on feature selection, please refer to the paper at [FuzzDistill](https://github.com/Saket-Upadhyay/FuzzDistill)**_