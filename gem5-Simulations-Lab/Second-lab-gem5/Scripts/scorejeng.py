#!/usr/bin/env python
from os import system
from itertools import product
from math import sqrt
import subprocess

l1d = [16,32,64,128]
l1i = [16,32,64]
l2 = [1,2,4]

syndiasmoi = list(product(l1d,l1i,l2))
runsjeng =  []
run = ""
costsize  =  0
costassoc = 0

for i in syndiasmoi:
    costsize = (i[0] + i[1]) * 120 + i[2] * 1024 * 10 
    costassoc = (i[0] * sqrt(16) + i[1] * sqrt(4) + i[2] * 1024 * sqrt(256))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/jengs/specsjeng_L1d%d_LI_%d_L2_%d configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=8 --l2_size=%dMB --l2_assoc=256 --l1d_assoc=8 --l1d_size=%dkB --l1i_size=%dkB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000""" % (i[0],i[1],i[2],i[2],i[0],i[1])
    
    
    system(run)
    with open("spec_results/optimalscost/jengs/specsjeng_L1d%d_LI_%d_L2_%d/stats.txt" % (i[0],i[1],i[2])) as inf:
        for line in inf:
            parts = line.split() # split line into parts
            if len(parts) > 1:   # if at least 2 parts/columns
                if parts[0] == "system.cpu.cpi":
                    
                    cpi = float(parts[1])
                    a=[run,costsize,costassoc,cpi]
                    print len(a)
                    runsjeng.append(a) 
                   
            
cpicheap = 3.164672





#bestrun = "./build/ARM/gem5.opt -d spec_results/optimalscost/jengs/specsjeng_L1d16_LI_16_L2_1 configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc=8 --l2_size=1MB --l2_assoc=256 --l1d_assoc=8 --l1d_size=16kB --l1i_size=16kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000" @ 3.071951

l1dassoc = [1,2,4,8]
l1iassoc = [1,2,4,8]
l2assoc = [1,2,8,32]


assocs = product(l1dassoc,l1iassoc,l2assoc)

for i in assocs:
    costsize = (16 + 16)  * 120 + 1 * 1024 * 10
    costassoc = (16 * sqrt(16) + 16 * sqrt(i[0]) + 1 * 1024 * sqrt(i[1]))/10
    run = """./build/ARM/gem5.opt -d spec_results/optimalscost/jengs/specsjeng_L1d16_LI_16_L2_1_dassoc{0}_iassoc{1}_l2assoc{2} configs/example/se.py --cpu-type=MinorCPU  --caches --l2cache   --l1i_assoc={1} --l2_size=1MB --l2_assoc={2} --l1d_assoc={0} --l1d_size=16kB --l1i_size=16kB  --cacheline_size=2048 -c spec_cpu2006/458.sjeng/src/specsjeng -o  'spec_cpu2006/458.sjeng/data/test.txt' -I 100000000""".format(i[0],i[1],i[2])
    system(run)

    with open("spec_results/optimalscost/jengs/specsjeng_L1d16_LI_16_L2_1_dassoc{0}_iassoc{1}_l2assoc{2}/stats.txt".format(i[0],i[1],i[2])) as inf:
        for line in inf:
            parts = line.split() 
            if len(parts) > 1:   
                if parts[0] == "system.cpu.cpi":
                    
                    
                    cpi = float(parts[1])
                    runsjeng.append([run,costsize,costassoc,cpi]) 


scorefinal = []



excelist = []



for j in runsjeng:
    scorefinal.append(j[1] * (j[3]/cpicheap)**10 + j[2] * (j[3]/cpicheap)**10)
    excelist.append([j[1] + j[2],j[3],scorefinal[-1]])




jengbestnow = scorefinal.index(min(scorefinal))
print runsjeng[jengbestnow]
print jengbestnow




with open("spec_results/optimalscost/jengs/cpijeng.txt",'w') as cpifile:
    for item in excelist:
        cpifile.write(str(item[1]) + "  " + str(item[0]) + " " + str(item[2]) +"\n")

