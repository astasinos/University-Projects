#!/bin/bash

# Block Size = 256
# L1D size 128
# L1i size 128
# L2 size 1-4 mb
# L1d assoc 32-64
# l1i assoc 8-16
# l2 assoc 1-8


./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc32_LIassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=8 --l2_assoc=1 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc32_LIassoc8_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=8 --l2_assoc=8 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc32_LIassoc16_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=16 --l2_assoc=1 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000


./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc32_LIassoc16_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=16 --l2_assoc=8 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc64_LIassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=8 --l2_assoc=1 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc64_LIassoc8_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=8 --l2_assoc=8 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc64_LIassoc16_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=16 --l2_assoc=1 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_4_dassoc64_LIassoc16_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=16 --l2_assoc=8 --l2_size=4MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000


./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_1_dassoc32_LIassoc16_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=16 --l2_assoc=8 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_1_dassoc64_LIassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=8 --l2_assoc=1 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_1_dassoc64_LIassoc8_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=32 --l1i_assoc=8 --l2_assoc=8 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_1_dassoc64_LIassoc16_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=16 --l2_assoc=1 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/hmmers/specshmmer_l2_1_dassoc64_LIassoc16_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=128kB --l1i_size=128kB  --l1d_assoc=64 --l1i_assoc=16 --l2_assoc=8 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

####################################################### mcf ############################################
# Block Size = 128
# L1D size 128 
# L1i size 32-128
# L2 size 4 mb
# L1d assoc 16-32
# l1i assoc 2-4
# l2 assoc 4-8

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc16_Iassoc2_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=16 --l1i_assoc=2 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc16_Iassoc2_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=16 --l1i_assoc=2 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc16_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=16 --l1i_assoc=4 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc16_Iassoc4_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=16 --l1i_assoc=4 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc32_Iassoc2_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=32 --l1i_assoc=2 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc32_Iassoc2_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=32 --l1i_assoc=2 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc32_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=32 --l1i_assoc=4 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_32_dassoc32_Iassoc4_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=32 --l1i_assoc=4 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc16_Iassoc2_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=16 --l1i_assoc=2 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc16_Iassoc2_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=32kB --l1d_assoc=16 --l1i_assoc=2 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc16_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=16 --l1i_assoc=4 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc16_Iassoc4_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=16 --l1i_assoc=4 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc32_Iassoc2_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=32 --l1i_assoc=2 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc32_Iassoc2_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=32 --l1i_assoc=2 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc32_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=32 --l1i_assoc=4 --l2_assoc=4 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000

./build/ARM/gem5.opt -d spec_results/optimals/mcfs/specsmcf_LI_128_dassoc32_Iassoc4_l2assoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=128kB --l1d_assoc=32 --l1i_assoc=4 --l2_assoc=8 --l2_size=4MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000
