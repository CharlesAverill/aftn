echo "---CLANG-FORMAT----"
clang-format -i $(find src -name "*.c") $(find include -name "*.h")
echo "------CMAKE--------"
if cmake -S . -B build ; then
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
