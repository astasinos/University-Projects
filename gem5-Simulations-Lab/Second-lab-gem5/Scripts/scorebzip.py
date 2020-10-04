#!/usr/bin/env python

############################# Bzip ################################
# Block Size = 256
# L1D size 128
# L1i size 16 
# L2 size 1-4 mb
# L1d assoc 256
# l1i assoc 4
# l2 assoc 4



from os import system
from itertools import product
from math import sqrt
import subprocess

l1d = [16,32,64,128]
l1i = [16]
l2 = [1,2,4]
l1dassoc = [1,2,4,8,16,128]
cpifound = 0
syndiasmoi = list(product(l1d,l1i,l2,l1dassoc))
runsbzip =  []
run = ""
costsize  =  0
costassoc = 0
print(syndiasmoi)
for i in syndiasmoi:
    costsize = (i[0] + i[1]) * 120 + i[2] * 1024 * 10 + i[0] * 16
    costassoc = (i[0] * sqrt(16) + i[1] * sqrt(4) + i[2] * 1024 * sqrt(256))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/bzips/specsbzip_L1d_{0}_L1i_{1}_L2_{2}_dassoc{3} configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc={3} --l1i_assoc=4 --l1d_size={0}kB --l1i_size={1}kB --l2_size={2}MB   --l2_assoc=4 --cacheline_size=256 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000""".format(i[0],i[1],i[2],i[3])
    
    
    system(run)
    with open("spec_results/optimalscost/bzips/specsbzip_L1d_{0}_L1i_{1}_L2_{2}_dassoc{3}/stats.txt".format(i[0],i[1],i[2],i[3])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    print "Found cpi   " + "spec_results/optimalscost/bzips/specsbzip_L1d_{0}_L1i_{1}_L2_{2}_dassoc{3}/stats.txt".format(i[0],i[1],i[2],i[3])
                    
                    cpi = float(parts[1])
                    runsbzip.append([run,costsize,costassoc,cpi])                  
                


cpicheap = 1.946350
scorefinal = []





#bestrun "./build/ARM/gem5.opt -d spec_results/optimalscost/bzips/specsbzip_L1d_32_L1i_16_L2_1_dassoc16 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=16 --l1i_assoc=4 --l1d_size=16kB --l1i_size=16kB --l2_size=1MB   --l2_assoc=4 --cacheline_size=256 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000" @ 1.737094


#l1d 16 l1i 16 l2 1 l1dassoc 16

l1iassoc = [1,2,4]
l2assoc = [1,2,4]

assocs = product(l1iassoc,l2assoc)
costsize  =  0
costassoc = 0

for i in assocs:
    costsize = (16 + 16)  * 120 + 1 * 1024 * 10
    costassoc = (16 * sqrt(16) + 16 * sqrt(i[0]) + 1 * 1024 * sqrt(i[1]))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/bzips/specsbzip_L1d_32_L1i_16_L2_1_dassoc16_iassoc_{0}_l2assoc{1} configs/example/se.py   --cpu-type=MinorCPU  --caches --l2cache   --l1d_assoc=16 --l1i_assoc={0} --l1d_size=32kB --l1i_size=16kB --l2_size=1MB   --l2_assoc={1} --cacheline_size=256 -c spec_cpu2006/401.bzip2/src/specbzip -o  'spec_cpu2006/401.bzip2/data/input.program 10' -I 100000000""".format(i[0],i[1])
    system(run)

    with open("spec_results/optimalscost/bzips/specsbzip_L1d_16_L1i_16_L2_1_dassoc16_iassoc_{0}_l2assoc{1}/stats.txt".format(i[0],i[1])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runsbzip.append([run,costsize,costassoc,cpi])   

excelist = []



for j in runsbzip:
    scorefinal.append(j[1] * (j[3]/cpicheap)**10 + j[2] * (j[3]/cpicheap)**10)
    excelist.append([j[1] + j[2],j[3],scorefinal[-1]])




bzipbestnow = scorefinal.index(min(scorefinal))
print runsbzip[bzipbestnow]
print bzipbestnow




with open("spec_results/optimalscost/bzips/cpibzip.txt",'w') as cpifile:
    for item in excelist:
        cpifile.write(str(item[1]) + "  " + str(item[0]) + " " + str(item[2]) +"\n")




