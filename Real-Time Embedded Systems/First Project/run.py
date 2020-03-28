#!/usr/bin/env python

# Note this is unfortunately quickly written and really hacky python but it gets the job done


import subprocess
import sys
import time



for cons in range(600,1100,100):
	cmd = "./prod-cons 80 {0} 20000".format(cons)
	with open(r"delay.txt",'r') as f:
		linesnow  = len(f.readlines())
	linesafter = linesnow
	while(linesafter != linesnow + 2):
		output = subprocess.Popen(cmd, shell=True)
		time.sleep(100)
		with open(r"delay.txt",'r') as f:
			linesafter  = len(f.readlines())
		
	with open(r"delay.txt",'r') as f:
		getlines = f.readlines()[::-1][0:2]
		total = 0
		for num in getlines:
			total += float(num.strip())
		average = total / 2.0
		with open(r"final.txt",'a') as final:
			final.write(str(average) + "\n")
	
