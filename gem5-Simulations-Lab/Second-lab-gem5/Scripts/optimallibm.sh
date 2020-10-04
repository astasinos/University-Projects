####################################################### libm ############################################
# Block Size = 2048
# L1D size 16 - 128 #16 max assoc = 8
# L1i size 64-128
# L2 size 4 mb
# L1d assoc 2-4
# l1i assoc 4-8
# l2 assoc 1-4

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc2_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc2_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc2_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc2_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc4_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc4_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc4_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_64_dassoc4_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc2_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc2_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc2_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc2_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc4_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc4_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc4_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_16_l1i_128_dassoc4_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc2_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc2_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc2_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc2_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc4_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc4_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc4_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_64_dassoc4_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=64kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc2_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc2_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc2_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc2_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=2 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc4_Iassoc4_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc4_Iassoc4_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=4 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000


./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc4_Iassoc8_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000

./build/ARM/gem5.opt -d spec_results/optimals/libms/specslibm_l1d_128_l1i_128_dassoc4_Iassoc8_l2assoc4 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=4MB --l1i_assoc=8 --l1d_assoc=4 --l2_assoc=4 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000
