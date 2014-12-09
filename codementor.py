import json
import scipy.cluster.hierarchy as hc
import pandas
from matplotlib import pyplot

# copy the data to the clipboard first
d = pandas.read_clipboard(sep=",", index_col=0)
d.columns = [int(x) for x in d.columns]

link = hc.linkage(d.values, method='centroid')
o1 = hc.leaves_list(link)

mat = d.iloc[o1,:]
mat = mat.iloc[:, o1[::-1]]
pyplot.imshow(mat)

order_of_images = [136,189,3,158,87,90,54,9,10,171,129,11,103,141,216,43,156,39,2,166,58,92,67,78,26,173,101,175,32,102,208,138,48,139,18,199,167,19,40,192,120,106,127,13,165,211,95,118,57,169,203,111,112,200,96,114,35,198,182,145,152,22,168,5,178,150,162,47,122,31,181,213,204,98,44,89,135,159,66,29,8,7,187,1,121,79,23,52,212,17,176,113,20,123,137,144,151,46,147,196,28,84,85,201,172,12,214,131,94,42,190,61,206,80,153,34,69,33,210,119,185,55,108,27,4,160,70,107,76,163,155,91,142,143,64,75,125,130,21,194]

js = {
    'ind': mat.index.values.tolist(),
    'var': mat.columns.values.tolist(),
    'corr': mat.values.tolist(),
    'dat': raw.values.tolist(),
    'group': [1 for x in mat]
}
output = json.dumps(js)
