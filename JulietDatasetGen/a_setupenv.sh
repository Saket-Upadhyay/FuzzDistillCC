#!/usr/bin/env bash

echo "Setting Up Python3 venv..."
python3 -m venv venv
source ./venv/bin/activate
pip install --upgrade pip
pip install -r ./requirements.txt

echo "Removing Old CC Symlink..."
rm ./fuzzdistillcc

echo "Removing Stale CSVs"
find . -type file -name "*.csv" -delete

echo "Creating New CC Symlink..."
ln -s ../build/src/fuzzdistillcc ./fuzzdistillcc

echo "Done."