#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
def Compton(Ei,theta):
    """
    Compton Scatter of a photon off an electron
    
    Keywords:
    Ei - Inital Photon Energy
    theta - Scattering angle
    
    Returns
    Ef - final energy of the photon
    Ee - final energy of the electron
    """
    m  = 0.5109989 # Mass Electron (MeV)
    Ee = Ei - Ei*m/(m+Ei*(1-np.cos(theta)))
    Ef = Ei - Ee
    return (Ef,Ee)

def diffXSElectrons(Ei,theta):
    """
    Differnetial cross section for scatter of electrons by the Compton Effect
    Keywords:
        Ei - Initial Photon Energy
        theta - scattering angle (compton)
    """
    # Contants
    r = 2.81794E-13 # Classical Electron Radius (cm)
    m  = 0.5109989 # Mass Electron (MeV)
    # Computing Final Photon and Electron Energy
    Ef,Ee = Compton(Ei,theta)
    ratio = np.divide(Ef,Ei)
    g = 0.5*np.power(ratio,2)*(ratio+1/ratio-np.power(np.sin(theta),2))
    firstTerm = 2*np.pi*np.power(r,2)*np.sin(theta)*g
    secTermNum = np.power(1+Ei/m*(1-np.cos(theta)),2)*m
    secTermDen = np.power(Ei,2)*np.sin(theta)
    return firstTerm*secTermNum/secTermDen

def PopulationMedian(pop,x):
    r = list()
    for i in range(len(pop)-1):
        r.append(np.sum(pop[0:i])/np.sum(pop[i:-1]))
    idx = (np.abs(np.array(r)-1.0)).argmin()
    print 'Population Ratio is: ',r[idx]
    return x[idx]

def PlotData(log=False):
    theta = np.linspace(0,np.pi,1000)
    if log:
      theta = np.logspace(-1.5,np.log10(np.pi),100)
    theta = np.delete(theta,[0,np.pi],None)
    ECo60 = np.array([1.1732,1.3325])
    Ef0,Ee0 = Compton(ECo60[0],theta)
    Ef1,Ee1 = Compton(ECo60[1],theta)
    dSdE0 = diffXSElectrons(ECo60[0],theta)
    dSdE1 = diffXSElectrons(ECo60[1],theta)

    # Looking for the median
    print 'Median Energy of {} MeV Photon is {}\n'.format(ECo60[0],PopulationMedian(dSdE0,Ee0))
    print 'Median Energy of {} MeV Photon is {}\n'.format(ECo60[1],PopulationMedian(dSdE1,Ee1))

    # Energy Distribution
    plt.figure()
    plt.plot(theta,Ee0,theta,Ee1)
    plt.grid(True)
    plt.xlabel('Scattering Angle')
    plt.ylabel("Electron Energy (MeV)")
    plt.legend(["1.173","1.332"])
    if log:
      plt.yscale('log')
      plt.xscale('log')
      plt.savefig('LogComptonElectronEnergies.png')
    else:
      plt.savefig('ComptonElectronEnergies.png')

    # Scattering Distribution
    plt.figure()
    plt.rc('text',usetex=True)
    plt.plot(Ee0,dSdE0,Ee1,dSdE1)
    plt.grid(True)
    plt.xlabel('Electron Kinetic Energy (MeV)',fontsize=14)
    plt.ylabel(r'$d\sigma/dE$ (cm${}^2$/MeV)',fontsize=14)
    plt.legend(["1.173","1.332"],loc=4)
    if log: 
      plt.yscale('log')
      plt.xscale('log')
      plt.savefig('LogComptonScatteringXS.png')
    else:
      plt.savefig('ComptonScatteringXS.png')

    # CDF of Scattering Distrubtions
    cdfE0 = CDF(Ee0,dSdE0)
    cdfE1 = CDF(Ee1,dSdE1)
    plt.figure()
    plt.rc('text',usetex=True)
    plt.plot(Ee0,cdfE0,Ee1,cdfE1)
    plt.grid(True)
    plt.title('CDF Compton Scattering Electron Energy') 
    plt.xlabel('Electron Kinetic Energy (MeV)',fontsize=14)
    plt.legend(["1.173","1.332"],loc=4)
    plt.savefig('CDFComptonScatteringXS.png')

def ComptonScattering(theta):
  if np.random.rand(1) < 0.5:
    E = 1.117
  else:
    E = 1.332
  dSdE = diffXSElectrons(E,theta)
  return dSdE

def CDF(x,pdf):
    """
    Calcualtes the CDF of the given pdf
    """
    n = np.trapz(pdf,x) # Normilization
    cdf = np.zeros(np.shape(pdf))
    for i in range(len(x)):
        cdf[i] = np.trapz(pdf[0:i],x[0:i])
    return cdf/n

def SampleRejection(piMax,piFunc):
  # Warning! Here be infinate loop dragons!
  while True:
    x = np.random.rand(1)*np.pi
    y = np.random.rand(1)*piMax
    if y < piFunc(x):
      return x

def Validation():
  numSamples = 1000000
  
  theta = np.random.rand(numSamples)*np.pi
  ECo60 = np.array([1.117,1.332])
  Ef0,Ee0 = Compton(ECo60[0],theta)
  Ef1,Ee1 = Compton(ECo60[1],theta)
  dSdE0 = diffXSElectrons(ECo60[0],theta)
  dSdE1 = diffXSElectrons(ECo60[1],theta)

  # Sampling Values
  values = list()
  piMax = np.max([dSdE0,dSdE1])
  while (len(values) < numSamples):
    values.append(SampleRejection(piMax,ComptonScattering))
  # Binning the data
  bins = np.logspace(-3,0.2,100)
  counts = np.histogram(values,bins)
  counts = counts[0]/float(len(values))
  binCenters = 0.5*(bins[1:]+bins[:-1])
  
  # Plotting
  plt.figure()
  plt.plot(binCenters,counts,ls='steps')
  #plt.bar(binCenters,counts,align='center')
  plt.grid(True)
  plt.xlim((1E-3,1.4))
  plt.xlabel('Electron Energy (MeV)')
  plt.ylabel('Frequency per Photon')
  plt.yscale('log')
  plt.xscale('log')
  plt.savefig('ValComptonScatteringXS.png')
  

def main():
  PlotData(log=False)
  #PlotData(log=True)
  #Validation()

if __name__ == '__main__':
    main()
#plot(theta,Ee)
