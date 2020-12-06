cp ../build/lab1/libLLVMFindSucBBPass.so ./
opt -load ./libLLVMFindSucBBPass.so -find-suc-BB ../tiffcp.bc -o /dev/null

#llvm-dis ../tiffcp.bc -o ./tiffcp.ll
#opt -dot-cfg tiffcp.ll 
#dot .TIFFFillTile.dot -Tpng -o cfg.png
