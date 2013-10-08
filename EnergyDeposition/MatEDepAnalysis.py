#!/usr/bin/env python
import sys
import xlwt
import os
import re
import numpy as np
from ROOT import TFile, TH1F, gStyle, TCanvas, TNtuple,TH2F, TAxis

def GetThickness(value):
  """ 
  GetThickness
  
  Parses a filename for a given thickness, returns the thickness in mm
  """
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

def GetEDep(dirName):
  gamma = dict()
  neutron = dict()
  files = [f for f in os.listdir(dirName) if f.endswith('.root')] 
  for fname in files:
    tokens = fname.strip('.root').split('_')
    key = tokens[1]+'_'+tokens[2]
    # Getting the ROOT object
    f = TFile(os.path.join(dirName,fname),'r')
    h = f.Get('eDepHist')
    # Normalizing
    h.Sumw2()
    h.Scale(1.0/h.GetEntries())
    # Saving
    if tokens[0] == 'Co60':
      gamma[key] = [h.GetMean(),h.GetMeanError()]
    else:
      neutron[key] = [h.GetMean(),h.GetMeanError()]
  return (neutron,gamma)

def WriteHeader(ws,matDict,headers):
  # Writting the headers and units
  for key in matDict:
    col = matDict[key]
    for h in range(len(headers)):
      ws.write(0,col+h,headers[h])
    ws.write(1,col,'mm')
    for h in range(1,len(headers)):
      ws.write(2,col+h,key)

def WriteData(neutron,gamma,wb):
  # Creating worksheets
  wsn = wb.add_sheet('Neutron')
  wsg = wb.add_sheet('Gamma')
  wsr = wb.add_sheet('Neutron Gamma Ratio')
  # Getting a list of materials (and corresponding row)
  matDict = dict()
  col = 0
  for key in neutron:
    mat = key.split('_')[0]
    if not mat in matDict:
      matDict[mat] = 0
  for key in sorted(matDict.keys()):
      matDict[key] = col
      col += 3
  mat = sorted(matDict.keys())
  # Getting a list of all the thickness
  tDict = dict()
  row = 3
  for key in neutron:
    t = key.split('_')[1]
    if not t in tDict:
      tDict[t] = 0

  tMap = dict()
  for key in tDict:
    tMap[GetThickness(key)] = key
  for key in sorted([GetThickness(t) for t in tDict]):
    tDict[tMap[key]] = (key,row)
    row += 1
  print "Thickness Dictionary"
  print tDict
  print "Material Dictionary"
  print matDict
    
  # Writing the data
  WriteHeader(wsn,matDict,('Thickness','Neutron Energy Deposition',''))
  WriteHeader(wsg,matDict,('Thickness','Gamma Energy Deposition',''))
  WriteHeader(wsr,matDict,('Thickness','Gamma / Neutron Ratio',''))
  ratio = dict()
  for key in neutron:
    n = neutron[key]
    g = gamma[key]
    mat = key.split('_')[0]
    t = key.split('_')[1]
    col = matDict[mat]
    tval,row = tDict[t]
    qRatio = 2.33/(0.5*1.1732+0.5*1.3325)
    ratio[key] = (g[0]/n[0]*qRatio, g[0]/n[0]*np.sqrt(n[1]*n[1]/(n[0]*n[0])+g[1]*g[1]/(g[0]*g[0])))
    wsn.write(row,col,tval)
    wsg.write(row,col,tval)
    wsr.write(row,col,tval)
    wsn.write(row,col+1,n[0])
    wsn.write(row,col+2,n[1])
    wsg.write(row,col+1,g[0])
    wsg.write(row,col+2,g[1])
    wsr.write(row,col+1,ratio[key][0])
    wsr.write(row,col+2,ratio[key][1])

def main():
  # Reading in the NIST DATA
  print '\tImporting Data'
  (n,g) = GetEDep('.')

  # Analayzing the Data
  wb = xlwt.Workbook()
  WriteData(n,g,wb)
  wb.save('EDepAnalysis.xls')

if __name__ == '__main__':
  print "Running Analysis . . ."
  main()
  print "Finished Analysis"
