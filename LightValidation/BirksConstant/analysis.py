#!/usr/bin/env python

import glob
import os
def extractAverage(filename):
    lightKey = 'Average Number of Optical Photons Created:'
    matKey = 'Material:'
    mat = ''
	#Average Number of Optical Photons Created: 1320.4 +/- 1603.78
    data = dict()
    with open(filename,'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith(lightKey):
                tokens = line.split()
                data[mat] = (float(tokens[6]),float(tokens[8]))
            if line.startswith(matKey):
                mat = line.split()[1]
    return data 
###########################################################################
#                                                                         #
#                          READS IN THE DATA                              #
#                                                                         #
###########################################################################
data = dict()
files = glob.glob('Birks*Output.txt')
for filename in files:
    lightYield = extractAverage(filename)
    path, filename = os.path.split(filename)
    tokens = filename.split('_')
    birks = float(tokens[1])
    if birks not in data:
        data[birks] = dict()
    if tokens[2].startswith('Gamma'):
        data[birks]['Gamma'] = lightYield
    else:
        data[birks]['Neutron'] = lightYield

###########################################################################
#                                                                         #
#                         WRITES OUT THE DATA                             #
#                                                                         #
###########################################################################
birksValues = sorted(data.keys(), key = lambda x : float(x))
header = 'Birks,'

srcs = sorted(data[birksValues[0]].keys())
for src in srcs:
    materials = sorted(data[birks][src].keys())
    for mat in materials:
        header += '{} {}, ,'.format(str(mat),str(src))
header = header[0:-1]+'\n'
s = ''
for birks in birksValues:
    line = '{:5.4f},'.format(birks)
    srcs = sorted(data[birks].keys())
    for src in srcs:
        materials = sorted(data[birks][src].keys())
        for mat in materials:
            line += '{:6.2f}, {:6.2f}, '.format(data[birks][src][mat][0],data[birks][src][mat][1])
    s += line[0:-2]+'\n'

with open('results.csv','w') as f:
    f.write(header)
    f.write(s)
