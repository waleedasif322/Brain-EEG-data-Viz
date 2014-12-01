from neo import io
import matplotlib.pyplot as plt
import numpy as np
<<<<<<< HEAD
import sys

def readEEGFile():
  r = io.BrainVisionIO( filename= 'eeg_suj1_wm2_day1.eeg', filename2='eeg_suj1_wm2_day1.vhdr')
  seg = r.read_segment(lazy=False, cascade= True, )
  siglist = seg.analogsignals[0:64]
  print siglist[0]



=======
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
>>>>>>> 7de4550b7abd19572bfe31e5e9d997189ed94706



readEEGFile()

<<<<<<< HEAD
avg_values = [631.40013579208608, 619.81196879292838, 610.67334996356658, 625.82804395016137, 649.46131417410652, 608.36669171333051, 589.51666045827733, 585.71347170570516, 588.90346647350816, 577.43729937170428, 503.87994529129355, 467.08950995562918, 497.10483152353845, 514.35149458036176, 545.00672964793193, 533.98438582342351, 825.53460427989194, 874.09542108528694, 890.54392884871049, 848.14115217090875, 650.77275847009878, 461.82428955294017, 449.5249375987114, 433.06191397656949, 418.90221428965742, 369.11994831396441, 441.89677650530939, 471.32731859285559, 483.0352932513124, 430.04635533331748, 468.93711470717972, 535.82364944578876, 456.52126500226586, 386.28900853665255, 405.89851555561472, 415.34213326101599, 401.6339709412714, 356.97225082041405, 318.22928582283203, 341.23064534358127, 346.46106926874927, 315.43398560503556, 313.99963536595169, 296.58158153446857, 250.71636791232595, 205.91022643240285, 193.99421149060072, 182.59465504327818, 157.97601011839288, 172.614506745158, 176.52775438005483, 195.93857481981104, 213.81669547122146, 174.15726244188409, 151.60747391208133, 169.14415041011671, 179.2937961395437, 172.66313632951642, 174.59361956283101, 203.21298091667995, 183.87366036768071, 168.763671005785, 155.93351342478854, 135.17466185757803, 108.00571264901373, 90.131277164946368, 95.795204391513835, 75.009431912250875, 69.526271860893758, 101.66392461182113, 130.82603242717596, 133.0262227756175, 128.12272952443891, 94.382402959796309, 62.200665490468964, 32.135384488086856, 13.091873596800724, -1.731962934420153, -7.9058357269095723, -5.6225342381294467, 42.529291708000528, 42.651778257240949, -9.2471144242517767, -41.058146877760009, -46.064047133440909, -63.41458228326519, -119.06905131760141, -148.49015321065235, -149.19508383307402, -156.75092156085157, -147.51036143388774, -175.96020851760841, -181.77668605619692, -188.64700931530388, -202.38593190465326, -213.24264932265942, -242.85956032471586, -277.05158159722487, -274.2884714525062, -265.63003022631892, -304.23708561890089, -386.71375237483153, -404.00323365066288, -432.90762241985067, -419.42854692256515, -404.88603274922934, -413.16370332435326, -420.30373195267748, -413.30830733767652, -395.79181581393641, -418.94941946640029, -473.3525738809476, -520.43224367977382, -555.95423568484694, -557.55849596508779, -558.35899786868686, -559.60074322456057, -582.66457677879953, -572.37979639104014, -557.06421296551707, -560.18995843675657, -568.69166220042098, -579.53917965252913, -589.74755494505371, -594.62105255709321, -601.09235074627213, -591.18469560083759, -600.81437302301492, -594.08555835059087, -603.53000781599258, -618.70197630646726, -634.84170195511979, -665.56606292040669, -674.2195615997407, -700.03705753334361, -687.05052978946333, -682.2278771528072, -685.34642080872436, -738.36339123680227]
=======
>>>>>>> 7de4550b7abd19572bfe31e5e9d997189ed94706

