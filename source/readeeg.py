from neo import io
import matplotlib.pyplot as plt
import numpy as np
import sys
import json
import math
	
def readEEGFile():
	r = io.BrainVisionIO( filename= 'eeg_suj1_wm2_day1.eeg', filename2='eeg_suj1_wm2_day1.vhdr')
	seg = r.read_segment(lazy=False, cascade= True, )
	siglist = seg.analogsignals[0:64]
	return siglist

	
	
	
	
#Generates new siglist file with filtered signals based on intervals and sampled at 30fps/sec	
def generate_sig_file_30fps(siglist,channels,interval_file):
	siglist = siglist
	interval = 2000
	cuts = 1
	steps = int(math.floor(interval/cuts))
	num_images  = 140
	with open(interval_file) as f:
		content = f.readlines()
		newarr = []
		for k in range(0,channels):
			sig = siglist[k]
			arr = []	
			newarr.append([])
			ctr=0
			for i in content:
				arr.append([])
				i = int(i)
				#2000 miliseconds intervals
				for j in range(0,interval):
					arr[ctr].append(float(sig[i+j]))
				ctr = ctr+1
			for i in range(0,num_images):
				newarr[k].append([])
				#for cuts 
				for j in range(0,cuts):
					avg = 0
					#2000/60 =33.3
					for fr in range(0,steps):
						avg += arr[i][j*steps+fr]
					newarr[k][i].append(avg/steps)				
		return newarr	
				
#returns seperate sig files filtered for intervals for all channels
def generate_channel_files():
	with open('intervals.txt') as f:
		content = f.readlines()
		for k in range(0,64):
			sig = siglist[k]
			arr = []	
			ctr = 0;
			for i in content:
				arr.append([])
				i = int(i)
				for j in range(0,2000):
					arr[ctr].append(float(sig[i+j]))						
				ctr = ctr+1
			with open('sig'+str(k)+'.txt', 'w') as outfile:
				json.dump(arr, outfile)

#takes channel signal and returnes sampled version of it		
def generate_30_fps_channel_sig():			
	newarr = []			
	with open('sigx.txt') as f:
		data = json.loads(f.read())
		for i in range(0,140):
			newarr.append([])
			ctr = 0;
			for j in range(0,60):
				avg = 0
				for k in range(0,33):
					avg += data[i][j*33+k]			
				newarr[i].append(avg/33)
		print len(newarr)	
		with open('sigxnew.txt', 'w') as outfile:
			json.dump(newarr, outfile)
			
			
sigfiles = readEEGFile()


#for i in range(0,100):
#	print sig1[1334423+i]

	
#leng = len(sig1)
 
#print leng
#print sig1[leng-10000]


rets = generate_sig_file_30fps(sigfiles,64,'intervals.txt')
with open('30fps_data', 'w') as outfile:
			json.dump(rets, outfile)
						
			