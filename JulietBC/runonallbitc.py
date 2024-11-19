import os
import sys
import subprocess
import shutil
from tqdm import tqdm

FUZZCC_PATH = "./fuzzcc"
LINUX_BITC_PATH = os.path.abspath(os.path.realpath("./LinuxIRs"))
MAC_BITC_PATH = os.path.abspath(os.path.realpath("./MacIRs"))
TARGET_CSV_DIR = os.path.abspath(os.path.realpath("./CSVS/"))

TQDMBARFORMAT = "{l_bar}{bar}| {n_fmt}/{total_fmt} files"


def get_all_bitc_files(dirpath):
    return [os.path.join(LINUX_BITC_PATH, bitcf) for bitcf in os.listdir(dirpath) if bitcf.endswith('.bitc')]


def operate_on_given_bitcs(bitc_list):
    for bitc in tqdm(bitc_list, desc="Running Fuzzcc", unit="bitc file", bar_format=TQDMBARFORMAT):

        fuzzcc_command = [FUZZCC_PATH, bitc]
        # print(fuzzcc_command)
        try:
            res = subprocess.run(fuzzcc_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        except subprocess.CalledProcessError as e:
            print(f"Error executing the fuzzcc for {bitc}: {e}")


def collect_csvs(CSV_PATH):
    for csvs in tqdm([os.path.join(CSV_PATH, cf) for cf in os.listdir(CSV_PATH) if cf.endswith('.csv')],
                     desc="Moving csvs", unit="csv", bar_format=TQDMBARFORMAT):
        shutil.move(csvs, TARGET_CSV_DIR)


def clear_data(DATAPATH=TARGET_CSV_DIR):
    for csvs in tqdm([os.path.join(DATAPATH, cf) for cf in os.listdir(DATAPATH) if cf.endswith('.csv')],
                     desc="removing csvs", unit="csv", bar_format=TQDMBARFORMAT):
        os.remove(csvs)
        # print(f"Removed {csvs}")


if __name__ == '__main__':
    LinuxBitcs = []
    MacBitcs = []
    clear_data(LINUX_BITC_PATH)
    clear_data(MAC_BITC_PATH)
    clear_data()

    if sys.platform == 'linux':
        LinuxBitcs = get_all_bitc_files(LINUX_BITC_PATH)
        print(f"Found {len(LinuxBitcs)} Linux bitcode files")
        print("Processing all bitcs")
        operate_on_given_bitcs(LinuxBitcs)
        collect_csvs(LINUX_BITC_PATH)


    elif sys.platform == 'darwin':
        MacBitcs = get_all_bitc_files(MAC_BITC_PATH)
        print(f"Found {len(MacBitcs)} Mac bitcode files")
        print("Processing all bitcs")
        operate_on_given_bitcs(MacBitcs)
        collect_csvs(MAC_BITC_PATH)

    else:
        print("Host platform not supported", file=sys.stderr)
