cmake_minimum_required (VERSION 3.8)
project (rosalind_cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(01_dna_root "01_dna")
add_executable(01_dna_naive "${01_dna_root}/main_naive.cpp")
add_executable(01_dna_bin "${01_dna_root}/main_bin.cpp")
add_executable(01_dna_bin_multi "${01_dna_root}/main_bin_multi.cpp")
set_property(TARGET 01_dna_bin_multi PROPERTY CXX_STANDARD 17)
add_executable(01_dna_bin_multi_thread "${01_dna_root}/main_bin_multi_thread.cpp")

set_property(TARGET 01_dna_bin_multi_thread PROPERTY CXX_STANDARD 17)
target_link_libraries(01_dna_bin_multi_thread Threads::Threads)
add_executable(01_dna_bin_multi_stat "${01_dna_root}/main_bin_multi_stat.cpp")

set(tools_root "tools")
add_executable(datageneration "${tools_root}/datageneration/main.cpp")
