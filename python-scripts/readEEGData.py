from neo import io
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats.stats import pearsonr

def readEEGFile():
	r = io.BrainVisionIO( filename= 'eeg_suj1_wm2_day1.eeg', filename2='eeg_suj1_wm2_day1.vhdr')
	seg = r.read_segment(lazy=False, cascade= True, )
	siglist = seg.analogsignals[0:64]

	with open('intervals.txt') as file:
		lines = file.read().splitlines()

	numbers = [int(e.strip()) for e in lines]
	signal_list = np.asarray(siglist)
	image_index = 0 # which image are we on
	i=0 # will iterate 64 times for all data
	j=0 # for each i (node), j will iterate 1.8 million times which is half an hour of data (1.8 million ms)

	# While we are in the inner while loop, compute the mean signal for two nodes at a time within a timeframe an image was shown
	# and store in a 2D array.
	# e.g. curr_image = numbers[i], next_image = numbers[i+1], np.average(signal_list[curr_image:next_image])
	# store each mean signal you get in a multidimensional array that is 140 X 64. 
	# Then run pearson correlation on array like this:
	# e.g. pearson_cor(mean_signals[0],mean_signals[1]), pearson_cor(mean_signals[0],mean_signals[2])...etc
	# final array should 140 X 140

	mean_signals = np.empty((140,64), dtype=np.float32) # This will store the final correlational data for each 
	final_correlation = np.empty((140,140), np.float32)

	while i < len(signal_list): #iterate over nodes
		j = 0
		image_index = 0
		while j < len(signal_list[i]): # iterate over of ms
		    if((j>= 415730) & (j<=1244423)):
		        if j == numbers[image_index]:
		            if numbers[image_index] == 1244423:
		                curr_image = numbers[image_index]
		                next_image = curr_image + 2000
		            else:
		                curr_image = numbers[image_index]
		                next_image = numbers[image_index + 1]
		            average_val = np.average(signal_list[i][curr_image:next_image])                
		            mean_signals[image_index][i] = average_val # assign an average signal to the current image
		            if j < 1244423:
		                image_index += 1
		    j += 1
		i += 1
		
	print mean_signals

	# Now run pearson corr bw every image
	a = 0
	b = 0
	node = 0
	while a < len(mean_signals): # iterate images
		b = 0
		while b < len(mean_signals): # iterate second image
		    curr_corr = pearsonr(mean_signals[a], mean_signals[b])
		    final_correlation[a][b] = curr_corr[0]
		    node += 1
		    b += 1
		a += 1
		
	print final_correlation
	print final_correlation.size



readEEGFile()


