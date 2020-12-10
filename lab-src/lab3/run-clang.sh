cp ../build/lab3/*.so ./
opt -load ./libLLVMFindArgAndPaths.so -find-arg-and-paths ../tiffcp.bc -o /dev/null
