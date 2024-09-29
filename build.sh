echo "Number of files: $Number_of_files"

if [ $Number_of_files -eq 3 ]; then
    echo "Removing main file"
    rm main
else
    echo "Main file not removed"
fi

gcc cuda_main.c -lpthread -o main -Wall -Wextra -pedantic && echo "Compiled successfully" || echo "Compilation failed"


