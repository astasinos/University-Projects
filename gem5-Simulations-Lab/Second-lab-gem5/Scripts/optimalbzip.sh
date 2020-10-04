#!/bin/bash


############################# Bzip ################################
# Block Size = 256
# L1D size 128
# L1i size 16 kai 128
# L2 size 4 mb
# L1d assoc 256
# l1i assoc 4
# l2 assoc 4

./build/ARM/gem5.opt -d spec_results/optimals/bzips/specsbzip_LI_16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=256 --l1i_assoc=4 --l1d_size=128kB --l1i_size=16kB --l2_size=4MB   --l2_assoc=4 --cacheline_size=256 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/bzips/specsbzip_LI128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=256 --l1i_assoc=4 --l1d_size=128kB --l1i_size=16kB --l2_size=4MB   --l2_assoc=4 --cacheline_size=256 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000
