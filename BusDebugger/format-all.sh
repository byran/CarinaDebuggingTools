#!/bin/bash

find . \( -path ./submodules -o -path ./TxENMonitor/mcc_generated_files \) -prune -type f -o -name '*.h' -exec clang-format -style=file -i {} \;
find . \( -path ./submodules -o -path ./TxENMonitor/mcc_generated_files \) -prune -type f -o -name '*.cpp' -exec clang-format -style=file -i {} \;
