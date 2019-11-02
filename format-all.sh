#!/bin/bash

find . \( -path ./submodules \) -prune -type f -o -name '*.h' -exec clang-format -style=file -i {} \;
find . \( -path ./submodules \) -prune -type f -o -name '*.cpp' -exec clang-format -style=file -i {} \;
