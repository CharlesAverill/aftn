echo "---CLANG-FORMAT----"
clang-format -i $(find src -name "*.c") $(find include -name "*.h")
echo "------CMAKE--------"
cd build
echo "CALLING 'sudo cmake ..' in order to copy important game files to /var/games"
if sudo cmake .. ; then
    echo "-------MAKE--------"
    if make -j$(nproc) ; then
        cd ../bin
        echo "-----EXECUTING-----"
		./aftn $*
    else
        echo "-----MAKE FAILURE-----"
    fi
else
    echo "-----CMAKE FAILURE-----"
fi
