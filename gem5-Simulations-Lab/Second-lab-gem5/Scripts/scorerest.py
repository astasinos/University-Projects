#!/usr/bin/env python



from os import system
from itertools import product
from math import sqrt
import subprocess


###########hmmer


l1d = [16,32,64,128]
l1i = [16,32,64,128]
l2 = [1]
l1dassoc = [1,2,4,8,16]

syndiasmoi = list(product(l1d,l1i,l2,l1dassoc))
runshmmer =  []
run = ""
costsize  =  0
costassoc = 0

for i in syndiasmoi:
    costsize = (i[0] + i[1]) * 120 + i[2] * 1024 * 10 
    costassoc = (i[0] * sqrt(8) + i[1] * sqrt(i[3]) + i[2] * 1024 * sqrt(1))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/hmmers/specshmmer_L1d{0}_L1i{1}_L2{2}_dassoc{3} configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size={0}kB --l1i_size={1}kB  --l1d_assoc={3} --l1i_assoc=8 --l2_assoc=1 --l2_size={2}MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000""".format(i[0],i[1],i[2],i[3])
    system(run)
    with open("spec_results/optimalscost/hmmers/specshmmer_L1d{0}_L1i{1}_L2{2}_dassoc{3}/stats.txt".format(i[0],i[1],i[2],i[3])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runshmmer.append([run,costsize,costassoc,cpi]) 


l1iassoc = [1,2,4,8]
l2assoc = [1,2,4,8]


assocs = l1iassoc
costsize  =  0
costassoc = 0

for i in assocs:
    costsize = (16 + 16)  * 120 + 1 * 1024 * 10
    costassoc = (16 * sqrt(8) + 16 * sqrt(i) + 1 * 1024 * sqrt(1))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/hmmers/specshmmer_L1d16_L1i16_L21_dassoc8_iassoc{0}_l2assoc1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_size=16kB --l1i_size=16kB  --l1d_assoc=8 --l1i_assoc={0} --l2_assoc=1 --l2_size=1MB   --cacheline_size=256 -c spec_cpu2006/456.hmmer/src/spechmmer -o  '--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 spec_cpu2006/456.hmmer/data/bombesin.hmm' -I 100000000""".format(i)
    system(run)

    with open("spec_results/optimalscost/hmmers/specshmmer_L1d16_L1i16_L21_dassoc8_iassoc{0}_l2assoc1/stats.txt".format(i)) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runshmmer.append([run,costsize,costassoc,cpi])   

excelist = []
cpicheap = 1.354473
scorefinalhmmer = []


for j in runshmmer:
    scorefinalhmmer.append(j[1] * (j[3]/cpicheap)**10 + j[2] * (j[3]/cpicheap)**10)
    excelist.append([j[1] + j[2],j[3],scorefinalhmmer[-1]])




hmmerbestnow = scorefinalhmmer.index(min(scorefinalhmmer))
print runshmmer[hmmerbestnow]
print hmmerbestnow




with open("spec_results/optimalscost/hmmers/cpihmmer.txt",'w') as cpifile:
    for item in excelist:
        cpifile.write(str(item[1]) + "  " + str(item[0]) + " " + str(item[2]) +"\n")







##################libm

l1d = [16,32,64,128]
l1i = [16,32,64,128]
l2 = [1,2,4]
l1dassoc = [1,2]
l1iassoc = [1,2,4]
syndiasmoi = list(product(l1d,l1i,l2,l1dassoc))
runslibm =  []
run = ""
costsize  =  0
costassoc = 0

for i in syndiasmoi:
    costsize = (i[0] + i[1]) * 120 + i[2] * 1024 * 10 
    costassoc = (i[0] * sqrt(i[3]) + i[1] * sqrt(4) + i[2] * 1024 * sqrt(1))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/libms/specslibm_L1d_{0}_L1i{1}_L2{2}_dassoc{3} configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size={0}kB --l1i_size={1}kB --l2_size={2}MB --l1i_assoc=4 --l1d_assoc={3} --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000""".format(i[0],i[1],i[2],i[3])
    system(run)
    with open("spec_results/optimalscost/libms/specslibm_L1d_{0}_L1i{1}_L2{2}_dassoc{3}/stats.txt".format(i[0],i[1],i[2],i[3])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runslibm.append([run,costsize,costassoc,cpi]) 
    


l1i = [1,2,4]
assocs = l1i

for i in assocs:
    costsize = (16 + 16)  * 120 + 1 * 1024 * 10
    costassoc = (16 * sqrt(2) + 16 * sqrt(i) + 1 * 1024 * sqrt(1))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/libms/specslibm_L1d_16_L1i16_L21_dassoc2_iassoc{0}_l2assoc1 configs/example/se.py --cpu-type=MinorCPU --caches --l2cache --l1d_size=16kB --l1i_size=16kB --l2_size=1MB --l1i_assoc={0} --l1d_assoc=2 --l2_assoc=1 --cacheline_size=2048  -c spec_cpu2006/470.lbm/src/speclibm -o "20 spec_cpu2006/470.lbm/data/lbm.in 0 1 spec_cpu2006/470.lbm/data/100_100_130_cf_a.of" -I 10000000""".format(i)
    system(run)

    with open("spec_results/optimalscost/libms/specslibm_L1d_16_L1i16_L21_dassoc2_iassoc{0}_l2assoc1/stats.txt".format(i)) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runslibm.append([run,costsize,costassoc,cpi])   

excelist = []
cpicheap = 3.712434
scorefinallibm = []


for j in runslibm:
    scorefinallibm.append(j[1] * (j[3]/cpicheap)**10 + j[2] * (j[3]/cpicheap)**10)
    excelist.append([j[1] + j[2],j[3],scorefinallibm[-1]])




libmbestnow = scorefinallibm.index(min(scorefinallibm))
print runslibm[libmbestnow]
print libmbestnow




with open("spec_results/optimalscost/libms/cpilibm.txt",'w') as cpifile:
    for item in excelist:
        cpifile.write(str(item[1]) + "  " + str(item[0]) + " " + str(item[2]) +"\n")





######### mcf

l1d = [16,32,64,128] #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
l1i = [16,32]
l2 = [1,2,4]

syndiasmoi = list(product(l1d,l1i,l2))
runsmcf =  []
run = ""
costsize  =  0
costassoc = 0

for i in syndiasmoi:
    costsize = (i[0] + i[1]) * 120 + i[2] * 1024 * 10 
    costassoc = (i[0] * sqrt(16) + i[1] * sqrt(4) + i[2] * 1024 * sqrt(8))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/mcfs/specsmcf_L1d_{0}_L1i_{1}_L2_{2} configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size={1}kB --l1d_assoc=16 --l1i_assoc=4 --l1d_size={0}kB  --l2_assoc=8 --l2_size={2}MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000""".format(i[0],i[1],i[2])
    
    
    system(run)
    with open("spec_results/optimalscost/mcfs/specsmcf_L1d_{0}_L1i_{1}_L2_{2}/stats.txt".format(i[0],i[1],i[2])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runsmcf.append([run,costsize,costassoc,cpi])




l1iassoc = [1,2,4]
l1dassoc = [1,2,4,8]
l2assoc  = [1,2,4,8]

assocs = product(l1dassoc,l1iassoc,l2assoc)



for i in assocs:
    costsize = (16 + 16)  * 120 + 1 * 1024 * 10
    costassoc = (16 * sqrt(i[0]) + 16 * sqrt(i[1]) + 1 * 1024 * sqrt(i[2]))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/mcfs/specsmcf_L1d_16_L1i_16_L2_1_dassoc{0}_iassoc{1}_l2assoc{2} configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache  --l1i_size=16kB --l1d_assoc={0} --l1i_assoc={1} --l1d_size=16kB  --l2_assoc={2} --l2_size=1MB   --cacheline_size=128 -c spec_cpu2006/429.mcf/src/specmcf -o  'spec_cpu2006/429.mcf/data/inp.in' -I 100000000""".format(i[0],i[1],i[2])
    system(run)

    with open("spec_results/optimalscost/mcfs/specsmcf_L1d_16_L1i_16_L2_1_dassoc{0}_iassoc{1}_l2assoc{2}/stats.txt".format(i[0],i[1],i[2])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runsmcf.append([run,costsize,costassoc,cpi])
















cpicheap = 1.541579

excelist = []
scorefinalmcf = []


for j in runsmcf:
    scorefinalmcf.append(j[1] * (j[3]/cpicheap)**10 + j[2] * (j[3]/cpicheap)**10)
    excelist.append([j[1] + j[2],j[3],scorefinallibm[-1]])




mcfbestnow = scorefinalmcf.index(min(scorefinalmcf))
print runsmcf[mcfbestnow]
print mcfbestnow

with open("spec_results/optimalscost/mcfs/cpimcf.txt",'w') as cpifile:
    for item in excelist:
        cpifile.write(str(item[1]) + "  " + str(item[0]) + " " + str(item[2]) +"\n")
