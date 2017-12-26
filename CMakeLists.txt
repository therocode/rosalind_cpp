cmake_minimum_required (VERSION 3.8)
project (rosalind_cpp)

set(01_dna_root "01_dna")
add_executable(01_dna_naive "${01_dna_root}/main_naive.cpp")
add_executable(01_dna_bin "${01_dna_root}/main_bin.cpp")
add_executable(01_dna_bin_multi "${01_dna_root}/main_bin_multi.cpp")

set(tools_root "tools")
add_executable(datageneration "${tools_root}/datageneration/main.cpp")
