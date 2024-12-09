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

4 directories, 5 files
```

```shell
cd JulietDatasetGen
./a_setupenv.sh
```

```shell
source ./venv/bin/activate
python3 b_runonallbitc.py
```

```shell
./c_consolidatefeatures.sh
```