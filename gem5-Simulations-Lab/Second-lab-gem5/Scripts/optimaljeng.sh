#!/bin/bash
# Block Size = 2048
# L1D size 32 - 128 #32 max assoc = 16
# L1i size 128
# L2 size 4 mb
# L1d assoc 16-32
# l1i assoc 16-64
# l2 assoc 256

./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_32_Iassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=16 --l2_size=4MB --l2_assoc=256 --l1d_assoc=16 --l1d_size=32kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000


./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_32_Iassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=64 --l2_size=4MB --l2_assoc=256 --l1d_assoc=16 --l1d_size=32kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_128_dassoc16_Iassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=16 --l2_size=4MB --l2_assoc=256 --l1d_assoc=16 --l1d_size=128kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_128_dassoc16_Iassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=64 --l2_size=4MB --l2_assoc=256 --l1d_assoc=16 --l1d_size=128kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_128_dassoc32_Iassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=16 --l2_size=4MB --l2_assoc=256 --l1d_assoc=32 --l1d_size=128kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/jengs/specsjeng_L1d_128_dassoc32_Iassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=64 --l2_size=4MB --l2_assoc=256 --l1d_assoc=32 --l1d_size=128kB --l1i_size=128kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000
