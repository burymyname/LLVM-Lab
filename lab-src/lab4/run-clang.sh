#clang -emit-llvm logutils.c -S -o logutils.llvm
#llvm-link logutils.llvm tiffcp.bc -o tiffcp-linked.bc

cp ../build/lab4/libLLVMInjectPrint.so ./
opt -load ./libLLVMInjectPrint.so -inject-print ./tiffcp-linked.bc -o ./tiffcp-linked-new.bc
