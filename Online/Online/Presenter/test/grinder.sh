 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-reachable=yes --num-callers=1000 --log-file=/tmp/presenterGrinderTrace.log $PRESENTERROOT/$CMTCONFIG/presenter.exe --login no
