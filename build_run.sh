if [ $# -gt 0 ] ; then
    if [ $1 = "install" ] ; then
      echo "---CLANG-FORMAT----"
      clang-format -i $(find src -name "*.c") $(find include -name "*.h")
      echo "------CMAKE--------"
      if sudo cmake -S . -B build -DDebugBuild=OFF ; then
          echo "-------MAKE--------"
          if sudo cmake --build build --target install ; then
              echo "-----EXECUTING-----"
              aftn $*
          else
              echo "-----MAKE FAILURE-----"
          fi
      else
          echo "-----CMAKE FAILURE-----"
      fi
    else
      echo "Argument \"$1\" not recognized"
      exit 1
    fi
else
    echo "---CLANG-FORMAT----"
    clang-format -i $(find src -name "*.c") $(find include -name "*.h")
    echo "------CMAKE--------"
    if cmake -S . -B build -DDebugBuild=ON ; then
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
fi
