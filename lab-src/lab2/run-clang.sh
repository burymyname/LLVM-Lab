cp ../build/lab2/libLLVMFindCallInst.so ./
opt -load ./libLLVMFindCallInst.so -find-call-inst ../tiffcp.bc -o ./tiffcp.bc

#llvm-dis ../tiffcp.bc -o ./tiffcp.ll
#opt -dot-callgraph tiffcp.ll 
#dot callgraph.dot -Tpng -o callgraph.png
