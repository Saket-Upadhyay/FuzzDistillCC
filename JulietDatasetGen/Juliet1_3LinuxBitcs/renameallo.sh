find . -type f -name "*.o" | while read -r file; do
    mv "$file" "${file%.o}.bitc"
done