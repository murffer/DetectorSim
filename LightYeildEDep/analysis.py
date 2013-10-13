# coding: utf-8
#!/usr/bin/env python
from ROOT import TFile, TH1F, gStyle, TCanvas, TNtuple,TH2F, TAxis
import os
import re
import matplotlib.pyplot as plt
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

def WriteHistograms(files,histKey='eDepPosHist',ext='.eps',tParse=GetThickness):
  """
  Write Histograms - Writes the histogram specified by histKey out to a file

  Keywords:
  files - list of files in which the histogram is in
  histKey - root key string of the histogram
  ext - file extension of the output image
  tParse - thickness parser for filename string
  """
  gStyle.SetOptStat(0)
  gStyle.SetPalette(1)
  c = TCanvas()
  for fname in files:
    f = TFile(fname,'r')
    hist = f.Get(histKey)
    #hist.GetYaxis().SetTileOffset(1.2)
    #hist.GetXaxis().SetTileOffset(1.2)
    hist.Draw("lego2 0")
    #hist.Draw("contz")
    outName = fname.rsplit('_',1)[0]
    outName = outName.rsplit('/',1)[-1]+'{}'.format(tParse(fname))+ext
    c.SetLogz(1)
    c.SaveAs(histKey+outName)


def PlotEDepSummary(gFiles,nFiles,figureName='EDepSummary.png',tParse=GetThickness,
  histKey='eDepHist'):
  """ PlotEDepSummary
  Plotss the energy deposition summary
  """
  # Extrating the average values
  gT = list()
  gDep = list()
  gDepError = list()
  nT = list()
  nDep = list()
  nDepError = list()
  for fname in gFiles:
    f = TFile(fname,'r')
    hist = f.Get(histKey)
    gT.append(GetThickness(fname))
    gDep.append(hist.GetMean())
    gDepError.append(hist.GetMeanError())
  for fname in nFiles:
    f = TFile(fname,'r')
    hist = f.Get(histKey)
    nT.append(GetThickness(fname))
    nDep.append(hist.GetMean())
    nDepError.append(hist.GetMeanError())
  # Plotting
  plt.errorbar(gT,gDep,yerr=gDepError,fmt='r+')
  plt.hold(True)
  plt.errorbar(nT,nDep,yerr=nDepError,fmt='go')
  plt.xlabel("Thickness (mm)")
  plt.ylabel("Average Energy Deposition (MeV)")
  plt.legend(["Co-60","Cf-252"])
  plt.xscale("log")
  plt.yscale("log")
  plt.grid(True)
  plt.savefig(figureName)

def main():
  print "Getting Files"
  [g,n] = GetRootFiles()
  print "Gamma Files: ",str(g)
  print "Neutron Files: ",str(n)
  print "Starting Data Analysis"
  PlotEDepSummary(g,n)
  WriteHistograms(g,histKey='eDepPosHist',ext='.png',tParse=GetThickness)
  WriteHistograms(n,histKey='eDepPosHist',ext='.png',tParse=GetThickness)

if __name__ == "__main__":
  main()
