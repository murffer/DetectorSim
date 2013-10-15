# coding: utf-8
#!/usr/bin/env python
from ROOT import TFile, TH1F
import os
import re
import numpy as np

def GetRootFiles(path=os.getcwd(),gammaKey='Co60',neutronKey='neutron'):
  """ GetRootFiles

    Gets a list of root files, splitting up into gamma and neutrons
    according to gammaKey and neutronKey
  """
  gammaFiles = list()
  neutronFiles = list()
  for filename in os.listdir(path):
    [filename,ext] = os.path.splitext(filename)
    if ext == '.root':
      particleName = filename.split('_')[0].split('run')[0]
      filename = os.path.join(path,filename)+ext
      if particleName == gammaKey:
        gammaFiles.append(filename)
      elif particleName == neutronKey:
        neutronFiles.append(filename)
      else:
        print 'ROOT file '+filename+' is not reconized'
  return [gammaFiles,neutronFiles]


def GetThickness(filename):
  """ 
  GetThickness
  
  Parses a filename for a given thickness, returns the thickness in mm
  """
  [filename,ext] = os.path.splitext(filename)
  value = filename.split('_')[-1]
  # Uses a regular expression to seperate
  pattern = re.compile('(\d?.?\d+)(\w+)')
  tokens = pattern.split(value)
  if tokens[2] == 'm':
    return float(tokens[1])*1000
  elif tokens[2] == 'cm':
    return float(tokens[1])*10
  elif tokens[2] == 'mm':
    return float(tokens[1])
  elif tokens[2] == 'um':
    return float(tokens[1])*0.001
  else:
    print tokens[2]," is not a reconized prefix"
    raise Exception()

def Summary(gFiles,nFiles,tParse=GetThickness):
  """ PlotEDepSummary
  Plotss the energy deposition summary
  """
  # Extrating the average values
  gT = list()
  g = list()
  nT = list()
  n = list()
  for fname in gFiles:
    f = TFile(fname,'r')
    gT.append(GetThickness(fname))
    opHist = f.Get('numPhotons')
    eDepHist = f.Get('eDep')
    data = (opHist.GetMean(),opHist.GetMeanError(),eDepHist.GetMean(),eDepHist.GetMeanError())
    g.append(data)

  for fname in nFiles:
    f = TFile(fname,'r')
    nT.append(GetThickness(fname))
    opHist = f.Get('numPhotons')
    eDepHist = f.Get('eDep')
    data = (opHist.GetMean(),opHist.GetMeanError(),eDepHist.GetMean(),eDepHist.GetMeanError())
    n.append(data)

  # Print
  print "Neutron Energy Deposition"
  for i in range(0,len(nT)):
    print "{} {} {} {} {}".format(nT[i],n[i][0],n[i][1],n[i][2],n[i][3])
  print "Gamma Energy Deposition"
  for i in range(0,len(gT)):
    print "{} {} {} {} {}".format(gT[i],g[i][0],g[i][1],g[i][2],g[i][3])

def main():
  print "Getting Files"
  [g,n] = GetRootFiles()
  print "Starting Data Analysis"
  Summary(g,n)

if __name__ == "__main__":
  main()
