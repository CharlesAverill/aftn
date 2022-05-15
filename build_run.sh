echo "---CLANG-FORMAT----"
clang-format -i $(find src -name "*.c") $(find include -name "*.h")
echo "------CMAKE--------"
echo "CALLING 'sudo cmake ..' in order to copy important game files to /var/games"
if sudo cmake -S . -B build ; then
    echo "-------MAKE--------"
    if cmake --build build -j$(nproc) ; then
        echo "-----EXECUTING-----"
        bin/aftn $*
    else
        echo "-----MAKE FAILURE-----"
    fi
else
    echo "-----CMAKE FAILURE-----"
fi
