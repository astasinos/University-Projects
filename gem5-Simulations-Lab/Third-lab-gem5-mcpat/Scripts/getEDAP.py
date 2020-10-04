import glob
import os

from math import sqrt


def getArea(file):


    with open(file,'r') as txt:
        lines = [line.strip() for line in txt]
        areas = [i for i, x in enumerate(lines) if "Area" in x and x]
        peakpower = [i for i, x in enumerate(lines) if "Peak Power" in x and x]
        if len(areas):
             corearea = float(lines[areas[1]].split("=")[1].split()[0])
             l2area = float(lines[areas[2]].split("=")[1].split()[0])
             peakpower = lines[peakpower[0]]

             #print file + "\tCore = " +  str(corearea)  + "\tL2 = " + str(l2area) +"\t " + peakpower
             totalarea = corearea + l2area
             return totalarea



def getDelay(file):
    with open(file,'r') as txt:
        lines = [line.strip() for line in txt]
        for line in lines:
            parts = line.split()
            if len(parts) > 1:
                if parts[0] == "sim_seconds":
                   # print file + "\tsim_seconds = " + parts[1]
                    return float(parts[1])

benchmarks = ["bzips","jengs","mcfs","libms","hmmers"]

with open("result.txt",'w') as resfile:
    for bench in benchmarks:

        textfiles =  sorted([f for f in glob.glob("my_mcpat/txtres/*.txt") if "specs%s" % bench[:-1] in f])
        ok = [os.popen("basename " + a).read()[:-5] for a in textfiles if int(os.popen("du -b %s" % a).read().split()[0]) > 1000]
        statsfiles = sorted(["my_gem5/spec_results/Default_Exec_Changing_only_One_Parameter/%s/%s/stats.txt" % (bench,statfile) for statfile in ok])

        edaps = []
        for stat,text in zip(statsfiles,ok):
            defparams = {"l1i_size" : 32,"l1d_size":64,"l2_size":2048,"l1i_assoc":2,"l1d_assoc":2,"l2_assoc":8}
            if "L1I_" in text:
                defparams["l1i_size"] = int(text.split("L1I_")[1])
            if "L1D_" in text:
                defparams["l1d_size"] = int(text.split("L1D_")[1])
            if "L2_" in text:
                defparams["l2_size"] = int(text.split("L2_")[1][:-2])
            if "Dassoc" in text:
                defparams["l1d_assoc"] = int(text.split("Dassoc")[1])
            if "LIassoc" in text:
                defparams["l1i_assoc"] = int(text.split("LIassoc")[1])
            if "L2assoc" in text:
                defparams["l2_assoc"] = int(text.split("L2assoc")[1])
            
            cost = (defparams["l1d_size"] + defparams["l1d_size"]) * 120 + defparams["l2_size"] * 10 + (defparams["l1d_size"] * sqrt(defparams["l1d_assoc"]) + defparams["l1i_size"]*sqrt(defparams["l1i_assoc"]) + defparams["l2_size"]*sqrt(defparams["l2_assoc"]))/10
            
            text = "my_mcpat/txtres/" + text + ".txt"
            area = getArea(text)
            delay = getDelay(stat)
            print_energy = os.popen("my_mcpat/Scripts/print_energy.py {mcpat} {stats}".format(mcpat=text,stats=stat)).readlines()
            energy = float(print_energy[3].split("is")[1].split()[0])
            edap = energy * delay * area
            #print "For {0} with {1} Area = {2} Delay = {3} Energy = {4} \n EDAP = {5} COST = {6} \n".format(text,stat,area,delay,energy,edap,cost)
            edaps.append([text,edap,cost])

        for e in edaps:
            resfile.write(e[0] + " " + str(e[1]) + " " + str(e[2]) + "\n")
        #print edaps 
        
        best = [run for run in edaps if run[1] == min([i[1] for i in edaps])]
        print " Best of " + bench + " " + best[0][0][21:] + " with EDAP = " + str(best[0][1]) + " and Cost = " + str(best[0][2])
        



