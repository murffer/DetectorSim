#!/usr/bin/env python
import argparse

def energyConversion(energy,unit):
    """
    Returns the energy in keV
    """
    if unit == 'keV':
        return float(energy)
    elif unit == 'MeV':
        return float(energy)*1000
    elif unit == 'eV':
        return float(energy)/1000
    else:
        raise ValueError('Unkown unit {}!'.format(unit))
        
def thicknessConversion(thickness,unit):
    if unit == 'um':
        return float(thickness)
    elif unit == 'mm':
        return float(thickness)*1000
    elif unit == 'nm':
        return float(thickness)/1000
    else:
        raise ValueError('Unkown unit {}!'.format(unit))

def extractRunInfo(line):
    tokens = line.split()
    thickness = thicknessConversion(tokens[9],tokens[10])
    energy = energyConversion(tokens[6],tokens[7])
    return (energy,thickness)

def extractEnergyDepInfo(line):
    tokens = line.split()
    eDep = energyConversion(tokens[3],tokens[4])
    eError = energyConversion(tokens[6],tokens[7])
    return (eDep, eError)

def importData(filename):
    readRun = False
    startKey = '======================== run summary ======================'
    endKey = '============================================================'
    runLine = 'The run was:'
    energyLine = 'total energy deposit:'
    data = dict()
    t = None
    e = None
    with open(filename,'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith(startKey):
                readRun = True
            elif readRun and line.startswith(runLine):
                (e,t) = extractRunInfo(line)
                if t not in data:
                    data[t] = dict()
                data[t][e] = dict()
            elif readRun and line.startswith(energyLine):
                (eDep, eError) = extractEnergyDepInfo(line)
                data[t][e] = (eDep,eError)
            elif readRun and line.startswith(endKey):
                readRun = False
    return data

def writeData(data,filename):
    isFirst = True
    header = 'Thickness,'
    s = ''
    thickness = sorted(data.keys(), key = lambda x: float(x))
    for t in thickness:
        line = '{:5.2f},'.format(t)
        energies = sorted(data[t].keys(), key = lambda x: float(x))
        if isFirst:
            for e in energies:
                header +='{:5.2f},,'.format(e)
            header = header[0:-1]
            isFirst = False
        for e in energies:
            line += '{:6.4f},{:6.4f},'.format(data[t][e][0],data[t][e][1])
        s += line[0:-1]+'\n'
    
    with open(filename,'w') as f:
        f.write(header+'\n')
        f.write(s)
                    

if __name__== '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("filename", help="filename of output deck",type=str)
    args = parser.parse_args()
    data = importData(args.filename)
    writeData(data,'Results.csv')