#!/bin/bash

################################################################# VARIABLES ###########################################################################





Oparams=('spec_cpu2006/458.sjeng/data/test.txt' 
'spec_cpu2006/429.mcf/data/inp.in' '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm'           'spec_cpu2006/401.bzip2/data/input.program 10' '20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of')

benchmarks=(spec_cpu2006/458.sjeng/src/specsjeng 
                spec_cpu2006/429.mcf/src/specmcf spec_cpu2006/456.hmmer/src/spechmmer
                spec_cpu2006/401.bzip2/src/specbzip spec_cpu2006/470.lbm/src/speclibm
                )
directories=(jeng mcf hmmer bzip libm)



############################################################### START #################################################################################




for j in {0..4}
do

# Default execution

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_def configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

############################################################ BLOCK SIZES ########################################################################### 

# Increase cache block size

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_block128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l2_size=2MB   --cacheline_size=128 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Increase cache block size to 256

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_block256 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB   --cacheline_size=256 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000


############################################################ L2 #####################################################################################


# Increase L2 Associativity to 16 

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2assoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB --l2_assoc=16 --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Decrease L2 Associativity to 4 
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2assoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB --l2_assoc=4 --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Decrease L2 Associativity to 1 
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB --l2_assoc=1 --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Increase L2 Size to 4MB

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2_4MB configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l2_size=4MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000




 #Increase L2 Size to 3MB
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2_3MB configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l2_size=3072kB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Decrease L2 Size to 1MB
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2_1MB configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l2_size=1MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000


######################################################################## L1D ##########################################################################

# Increase L1D Associativity to 4 
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_Dassoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=4 --l2_size=2MB   --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Increase L1D Associativity to 8 

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_Dassoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=8 --l2_size=2MB   --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

# Decrease L1D Associativity to 1

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_Dassoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=1 --l2_size=2MB   --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Smaller L1D Cache 32kB

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_small_L1D_32 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1d_size=32kB  --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Smaller L1D Cache 16kB

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_small_L1D_16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1d_size=16kB  --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Bigger L1D Cache 128kB
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_big_L1D_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1d_size=128kB  --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#################################################################### L1I #############################################################################

# Smaller L1I Cache 16kB

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_small_L1I_16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l1i_size=16kB   --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Bigger L1I Cache 64kB
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_big_L1I_64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l1i_size=64kB   --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Bigger L1I Cache 128kB
./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_big_L1I_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache --l1i_size=128kB   --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000


#Increase L1I Associativity to 4

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_LIassoc4 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=4 --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

#Increase L1I Associativity to 8

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_LIassoc8 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=8 --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000


#Decrease L1I Associativity to 1 

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_LIassoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=1 --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

############################################################### EXTRA #############################################################

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_block512 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB   --cacheline_size=512 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_block1024 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB   --cacheline_size=1024 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB --l2_assoc=1 --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_L2assoc32 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_size=2MB --l2_assoc=32 --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_Dassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=16 --l2_size=2MB   --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_Dassoc32 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=32 --l2_size=2MB   --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_LIassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=16 --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/"${directories[$j]}"s/specs"${directories[$j]}"_LIassoc32 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=32 --l2_size=2MB --cacheline_size=64 -c "${benchmarks[$j]}" -o  "${Oparams[$j]}" -I 100000000

./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_Dassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/hmmers/specshmmer_Dassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000

./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_LIassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_LIassoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_L2assoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000


./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_Dassoc_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=128 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_Dassoc_256 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=256 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_LIassoc_256 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=256 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_LIassoc_256 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=256 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_L2assoc_256 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_assoc=256 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_LIassoc_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=128 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000

./build/ARM/gem5.opt -d spec_results/bzips/specsbzip_LIassoc_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=128 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000

./build/ARM/gem5.opt -d spec_results/jengs/specsjeng_L2assoc_128 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_assoc=128 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000


./build/ARM/gem5.opt -d spec_results/mcfs/specsmcf_L2assoc64 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l2_assoc=64 --l2_size=2MB   --cacheline_size=64 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000



######################################################### RESULTS ########################################################################


benchfolds=$(ls "spec_results/${directories[$j]}""s")


echo "[Benchmarks]" > spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini

for directory in "${benchfolds[@]}"
do
	echo "${directory}" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini 
done

echo " " >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "[Parameters]" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "system.cpu.cpi" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "system.cpu.dcache.overall_miss_rate::total" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "system.cpu.icache.overall_miss_rate::total" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "system.l2.overall_miss_rate::total" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "sim_seconds" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo " " >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "[Output]" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini
echo "${directories[$j]}""_results.txt" >> spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini


./spec_results/read_results.sh spec_results/"${directories[$j]}"s/"${directories[$j]}"_conf.ini


done



################################################################## END ###############################################################################






