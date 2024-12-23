#!/bin/bash
if [[ "$OSTYPE" == "darwin"* ]]; then
    cc -ggdb src/*.c -lm  $(pkg-config --libs --cflags raylib) -lraylib -o clayprobe
else
    cc  -ggdb -Wall -Werror -Wno-unused-variable  -Wno-unknown-pragmas src/*.c -lm -lraylib -o clayprobe  -fsanitize=address 
fi

