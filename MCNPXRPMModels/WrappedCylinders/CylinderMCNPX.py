#!/usr/bin/env python

""" 
MCNPX Model for Cylindrical RPM8
"""
import sys
sys.path.append('../MCNPTools/')
sys.path.append('../')
from MCNPMaterial import Materials 
import subprocess
import math
import mctal
import numpy as np
import itertools
import os
class CylinderRPM(object):
    # Material Dictionaries
    cellForStr = '{:5d} {:d} -{:4.3f} {:d} -{:d} u={:d}\n'
    surfForStr = '{:5d} cz {:5.3f}\n'
    tranForStr = '*tr{:d} {:4.3f} {:4.3f} 0.000\n'
    geoParam={'RPM8Size':12.7,'DetectorThickness':0.01,'DetectorSpacing':0.8,
               'CylinderLightGuideRadius':0.5,'CylinderRadius':2.5}
    
    def __init__(self,inp='INP.mcnp'):
        """ Wrapped Cylinder MCNPX Model of RPM8

        Keywords:
        inp -- desired name of the input deck
        """
        # Material dictionary for the moderator, light guide, and detector
        self.material =  {'Moderator':None,'Detector':None,'LightGuide':None}
        self.material['Detector'] = {'name':'Detector','mt': 3, 'rho': 1.1,'matString':None}       # detector
        self.material['LightGuide'] = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
        self.material['Moderator'] = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE
        
        # Cell and Surface Inital Numbering
        self.CellStartNum = 600
        self.SurfaceStartNum = 600
        self.ZeroSurfaceNum = 500
        self.UniverseNum = 200
        self.surfGeo = None
        self.inp = inp
        self.name = 'OUT_'+self.inp.strip('.mcnp')+'.'
        self.setMaterial(0.1,'PS')

    def __str__(self):
        s = '\tMCNPX Model of Wrapped Cylinder\n'
        s += '\t Cell Number Starts: {0:d}\n'.format(self.CellStartNum)
        s += '\t Surface Number Starts: {0:d}\n'.format(self.SurfaceStartNum)
        return s

    def getInteractionRate(self):
        """ Returns the interaction rate """
        m = mctal.MCTAL(self.name+'.m')
        t = m.tallies[4]
        # Returing the total
        return t.data[-1],t.errors[-1]
    
    def setMaterial(self,massFraction,polymer):
        """
        Sets the detector material
        """
        M = Materials()
        num = self.material['Detector']['mt']
        if polymer == 'PS':
            self.material['Detector']['matString'] = M.GetPSLiF(massFraction,num)
        elif polymer == 'PEN':
            self.material['Detector']['matString'] = M.GetPENLiF(massFraction,num)
        else:
           raise ValueError('Polymer {} is not in the material database'.format(polymer)) 

    def createSurfaceGeo(self):
        """
        Creates a dictionary of surface positions and cylinders
        """
        self.surfGeo = dict()
        r = self.geoParam['CylinderLightGuideRadius']
        self.surfGeo[r] = 'LightGuide'
        #self.material =  {'Moderator':None,'Detector':None,'LightGuide':None}
        while(r + self.geoParam['DetectorThickness'] < self.geoParam['CylinderRadius']):
            r += self.geoParam['DetectorThickness']
            self.surfGeo[r] = 'Detector'
            r += self.geoParam['DetectorSpacing']
            if (r < self.geoParam['CylinderRadius']):
                self.surfGeo[r] = 'LightGuide'
        return self.surfGeo
    
    def calculateDetectorArea(self):
        """
        Calculates the area used in a detector
        """
        area = 0.0
        r = self.geoParam['CylinderLightGuideRadius']
        while(r + self.geoParam['DetectorThickness'] < self.geoParam['CylinderRadius']):
            area -= math.pow(r,2)
            r += self.geoParam['DetectorThickness']
            area += math.pow(r,2)
            r += self.geoParam['DetectorSpacing']
        return math.pi*area

    def createDetectorCylinder(self,uNum=1):
        """
        Creates a detector cylinder 
        
        Returns an ntuple of s,c,detectorCells
        s - the surface string
        c - the cell string
        detectorCells - a list of the numbers corresponding to the detectors cells
        """
        cellsCreated = 0
        sNum = self.SurfaceStartNum
        cNum = self.CellStartNum
        detectorCells = list()
        s = '{:5d} rcc 0 0 0 0 0 217.7 {}\n'.format(self.SurfaceStartNum,self.geoParam['CylinderRadius'])
        c = ''
        keyList = sorted(self.surfGeo.keys(), key = lambda x: float(x))
        for key in keyList:
            sPrev = sNum
            sNum += 1
            cNum += 1
            s += self.surfForStr.format(sNum,key)
            m = self.material[self.surfGeo[key]]
            if cNum == self.CellStartNum+1:
                c+= '{:5d} {:d} -{:4.3f} -{:d} u={:d}\n'.format(cNum,m['mt'],m['rho'],sNum,uNum)
            else:
                c += self.cellForStr.format(cNum,m['mt'],m['rho'],sPrev,sNum,uNum)
            # List of cells for the detector
            if self.surfGeo[key] is 'Detector':
                detectorCells.append(cNum)
            cellsCreated += 1
        # Last cell up to universe boundary
        m = self.material['Moderator']
        c += '{:5d} {:d} -{:4.3f} {:d} u={:d}\n'.format(cNum+1,m['mt'],m['rho'],sNum,uNum)
        cellsCreated += 1
        return s,c,detectorCells,cellsCreated
    
    def runModel(self):
        """ 
        Runs the Model by submission to Tourqe / Maui
        """
        qsub= subprocess.check_output('which qsub',shell=True).strip()
        cmd = '#!/bin/bash\n'
        cmd += '#PBS -N {0}\n#PBS -V\n#PBS -q gen1\n#PBS -l nodes=1:ppn=1\n'
        cmd += 'cd $PBS_O_WORKDIR\nmpirun mcnpx inp={1} name={2}\n'
        job = cmd.format('Job_RPMCylinder',self.inp,self.name)
        with open('qsub','w') as o:
            o.write(job)
        subprocess.call(qsub+' qsub',shell=True)
        subprocess.call('rm qsub',shell=True)
    
    def createInputDeck(self,cylinderPositions,inp=None,name=None):
        """ createInputDeck 

        Creates an input deck of the given geometry
        """
        self.inp = inp
        self.name = name
        if not inp:
            self.inp = 'INP_Cylinder.mcnp'
        if not name:
            self.name = 'OUT_Cylinder.'
        oFile = self.inp
        
        # Problem Constants
        cellString = 'c ------------------------- Source ----------------------------------------\n'
        cellString += '70   5  -15.1       -70                               $ 252Cf source      \n'       
        cellString += '71   406  -11.34    -71 70                            $ Lead around source\n'
        cellString += '72   456  -0.93       -72 71                          $ Poly around source\n'

        surfString = 'c  ########################### Surface Cards ##############################\n'
        surfString += 'c ------------------- Encasing Bounds (Size of RPM8) ---------------------\n'
        surfString += '500 rpp 0 12.7 -15.25 15.25 0 217.7                                       \n'

        # Add in other cells here
        numCells = 4 # 3 Source, 1 RPM8 Encasing
        
        ##################################################################
        #               Add in Detector Cells and Surfaces               #
        ##################################################################
        universeNum = 1
        (s,c,detectorCells,cellsCreated) = self.createDetectorCylinder(universeNum)
        surfString += s
        cellString += 'c ------------------- Detector Cylinder Universe ------------------------\n'
        cellString += c
        transNum = 1
        uCellNum = self.UniverseNum
        transString = ''
        cellString += 'c ----------------------- Detector Universe ----------------------------\n'
        for pos in cylinderPositions:
            transString += self.tranForStr.format(transNum,pos[0],pos[1])
            cellString += '{:5d} 0 -{:d} trcl={:d} fill={:d}\n'.format(uCellNum,self.SurfaceStartNum,transNum,universeNum)
            transNum +=1
            uCellNum +=1
        # Adding the PMMA Moderator Block
        m = self.material['Moderator']
        cellString += 'c ------------------------- HDPE Moderator -----------------------------\n'
        cellString += '{:5d} {:d} -{:4.3f} -{:d}  '.format(500,m['mt'],m['rho'],self.ZeroSurfaceNum)
        cellString += ''.join('#{:d} '.format(i) for i in range(self.UniverseNum,uCellNum))
        cellString += '\n'
        # Getting total number of cells
        numCells += cellsCreated + uCellNum-self.UniverseNum +1
        
        ##################################################################
        #                      Write the Tallies                         #
        ##################################################################
        univCells = range(self.UniverseNum,uCellNum)
        tallyString = 'c ------------------------- Tallies Yo! -----------------------------------\n'
        tallies = {'F54:n':{'cells':detectorCells,'comments':'FC54 6Li Reaction Rates\n',
                    'options':' T\nSD54 1 {0:d}R\nFM54 -1 3 105'}}
        for t in tallies:
            # Getting a list of cells
            tallyString += tallies[t]['comments']
            tallyString += str(t)+' '
            j = 0
            for u in univCells:
                cell = list('('+str(c)+'<'+str(u)+') ' for c in tallies[t]['cells'])
                cell = [cell[i:i+6] for i in range(0,len(cell),6)]
                if j > 0:
                    tallyString += '      '+''.join(''.join(i)+'\n' for i in cell)
                else:
                    tallyString += '      '.join(''.join(i)+'\n' for i in cell)
                j +=1
            tallyString = tallyString.rstrip()
            tallyString += tallies[t]['options'].format(len(univCells)*len(tallies[t]['cells']))
            tallyString+='\n'

        # Finish up the problem data
        cellString += 'c ---------------------- Detector  Encasing ------------------------------\n'
        cellString += '700  488  -7.92   701 -700           $ SS-316 Encasing                    \n'
        cellString += 'c -------------------------- Outside World -------------------------------\n'
        cellString += '1000 204  -0.001225   -1000 700 #70 #71 #72        $ Atmosphere           \n'
        cellString += '1001  0              1000                                                 \n'
        
        surfString += 'c ------------------------ Encasing Material -----------------------------\n'
        surfString += '700  rpp -0.3175 13.018 -15.5675 15.5675 -0.3175 218.018                  \n'
        surfString += '701  rpp 0.0     12.7   -15.25   15.25   0.0     217.7                    \n'
        surfString += 'c -------------- Source --------------------------------------------------\n'
        surfString += '70   s  -200 0 108.85    2.510E-04    $ Source                            \n'
        surfString += '71   s  -200 0 108.85   5.0025E-01   $ 0.5 cm lead surrounding source     \n'
        surfString += '72   s  -200 0 108.85    3.00025      $ 2.5 cm poly surrounding source    \n'
        surfString += 'c -------------- Outside World -------------------------------------------\n'
        surfString += '1000 so 250                                                               \n'

        matString = 'c -------------------------- Material Cards -----------------------------\n'
        matString += self.material['Detector']['matString']
        matString += self.getMaterialString()
        with open(oFile,'w') as o:
            o.write('MCNPX Simulation of RPM8 Cylinder\n')
            o.write(cellString)
            o.write('\n')
            o.write(surfString)
            o.write('\n')
            o.write(self.getRunString().format(numCells))
            o.write(self.getSrcString())
            o.write(tallyString)
            o.write(matString)
            o.write(transString)
            o.write('\n')
        
    def getRunString(self):
        runString ='c ------------------------------ Run Info ---------------------------------\n'
        runString +='nps 1E6                                                                   \n'
        runString +='IMP:N      1 {0:d}R 0           $ Particle Importances within cells       \n'
        runString +='c -------------- Output --------------------------------------------------\n'
        runString +='PRDMP j j 1      $ Write a MCTAL File                                     \n'
        runString +='PRINT 40                                                                  \n'
        runString +='c ------------------------------ Physics ---------------------------------\n'
        runString +='MODE N                                                                    \n'
        runString +='PHYS:N 100 4j -1 2                                                        \n'
        runString +='CUT:N  2j 0 0                                                             \n'
        return runString

    def getSrcString(self):
        """
        Returns the MCNPX formated source string
        """
        srcString = 'c -------------------------- Source Defination ----------------------------\n'
        srcString += 'c 1 nanogram Cf-252 source = 1E-9 grams = 6.623E-11 cc                    \n'
        srcString += 'sdef  pos=-200 0 108.85  cel=70  par=SF  rad=d1                           \n'
        srcString += 'si1  0 2.510E-04                                                          \n'
        srcString += 'sp1 -21 1                                                                 \n'
        return srcString

    def getMaterialString(self):
        """
        Returns the MCNXP material string
        """
        matString = 'm10   1001.70c  -0.080538       $Lucite (PMMA / Plexiglass)    rho = 1.19 g/cc\n'
        matString += '      6012.70c  -0.599848    8016.70c       -0.319614                       \n'
        matString += 'm204  7014.70c      -0.755636  $air (US S. Atm at sea level) rho = 0.001225 \n'
        matString += '      8016.70c      -0.231475 18036.70c     -3.9e-005 18038.70c      -8e-006\n'
        matString += '      18040.70c     -0.012842                                               \n'
        matString += 'm5    98252.66c  1  $ Cf-252, rho =15.1 g/cc wiki                           \n'
        matString += 'm406  82204.70c     -0.013781  $Lead,                                       \n'
        matString += '      82206.70c     -0.239557 82207.70c     -0.220743 82208.70c     -0.525919\n'
        matString += 'm456  1001.70c      -0.143716  $Polyethylene - rho = 0.93 g/cc               \n'
        matString += '      6000.70c      -0.856284                                                \n'
        matString += 'm488  14028.70c     -0.009187  $Steel, Stainless 316 rho = 7.92              \n'
        matString += '      14029.70c     -0.000482 14030.70c     -0.000331 24050.70c     -0.007095\n'
        matString += '      24052.70c     -0.142291 24053.70c     -0.016443 24054.70c     -0.004171\n'
        matString += '      25055.70c         -0.02 26054.70c     -0.037326 26056.70c     -0.601748\n'
        matString += '      26057.70c     -0.014024 26058.70c     -0.001903 28058.70c     -0.080873\n'
        matString += '      28060.70c     -0.031984 28061.70c     -0.001408 28062.70c     -0.004546\n'
        matString += '      28064.70c     -0.001189 42092.70c     -0.003554 42094.70c     -0.002264\n'
        matString += '      42095.70c     -0.003937 42096.70c     -0.004169 42097.70c     -0.002412\n'
        matString += '      42098.70c     -0.006157 42100.70c     -0.002507                        \n'
        matString += 'mt3   poly.01t                                                               \n'
        matString += 'mt456 poly.01t                                                               \n'
        matString += 'mt10  poly.01t                                                               \n'
        return matString

def run(loading,polymers):
    """
    Runs a matrix of loading and polymers
    """
    cylinderPositions = ((4.23,10.16),(4.23,-10.16))
    cylinderPositions = ((4.23,7.625),(4.23,0),(4.23,-7.625))
    cylinderPositions = ((4.23,9.15),(4.23,3.05),(4.23,-3.05),(4.23,-9.15))
    cylinderPositions = ((4.23,10.16),(4.23,5.08),(4.23,0.0),(4.23,-5.08),(4.23,-10.16))
    for l in loading:
        for p in polymers:
            RunCylinder(l,p,cylinderPositions)

def RunCylinder(l,p,cylinderPositions):
    """
    Runs an mcnpx model of the cylinder of loading l, polymer p, with
    cylinder positions cylinderPositions.

    Keywords:
        l - loading of the films
        p - polymer
        cylinderPositions - the cylinder positons
    """
    # Creating input and output deck names
    posString = ''
    for pos in cylinderPositions:
        posString += '{:2.1f}-'.format(pos[0])
    posString = posString.rstrip('-')
    inp='Cyl_{}LiF_{}_{}.mcnp'.format(int(l*100),p,posString)
    name='OUTCyl_{}LiF_{}_{}.'.format(int(l*100),p,posString)
    print inp
    # Creating and running the model
    m  = CylinderRPM()
    m.createSurfaceGeo()
    m.setMaterial(l,p)
    m.createDetectorCylinder()
    m.createInputDeck(cylinderPositions,inp,name)
    m.runModel()

def CreatePositions(yPos,numXPertubations):
    """
    Creates and returns an array of positions, using a set array of y 
    positions, with equally spaced number of numXPertubations.

    Keywords:
        yPos - the number of y positions (or spacing of the cylinders). The
            number of elements in this array corresponds to the number of
            cylinders that are simulated.
        numXPertubations - the number of pertubations in x. The arrays 
            positions returned are spaced linerly in the x from 2.54 to 
            10.16 cm
    """
    pos = list()
    xVals = np.linspace(2.54,10,numXPertubations)
    xPos = [i for i in itertools.product(xVals,repeat=len(yPos))]
    for x in xPos:
        pos.append(zip(x,yPos))
    return pos

def PositionOptimization(loading,polymers,positions):
    """
    Runs a matrix of loading, polymers and positions
    """
    for l in loading:
        for p in polymers:
            for pos in positions:
                RunCylinder(l,p,pos)
    
def createInputPlotDecks():
    positions = list()
    positions.append(((4.23,10.16),(4.23,-10.16)))
    positions.append(((4.23,7.625),(4.23,0),(4.23,-7.625)))
    #positions.append(((4.23,9.15),(4.23,3.05),(4.23,-3.05),(4.23,-9.15)))
    for pos in positions:
        m  = CylinderRPM()
        m.createSurfaceGeo()
        m.createDetectorCylinder()
        inp='Cylinder_{}.mcnp'.format(len(pos))
        name='OUTCylinder_{}.'.format(len(pos))
        m.createInputDeck(pos,inp,name)

def computeMassLi(polymer,loading,density=1.1):
    """ 
    Computes the mass of Li for a given polymer and loading
    """
    M = Materials()
    m  = CylinderRPM()
    area = m.calculateDetectorArea()
    massLi = area*217.0*M.GetLiMassFraction(loading,polymer)*density
    return massLi

def extractRunInfo(filename):
    """
    Extracts the loading and polymer from the file name
    """
    tokens = filename.split('_')
    loading = tokens[1].strip('LiF')
    polymer = tokens[2].strip('.m')
    return (float(loading)/100, polymer)

###########################################################################
#                                                                         # 
#                     Summerizes / Analysis                               # 
#                                                                         # 
###########################################################################
def GetInteractionRate(f,tallyNum=54,src=2.3E3):
    """
    Returns the interaction rate of the mctal file
    """
    m = mctal.MCTAL(f)
    t = m.tallies[tallyNum]
    return (t.data[-1]*src,t.errors[-1]*t.data[-1]*src)

import glob
def summerize():
    files = glob.glob('OUTCylinder*.m')
    s = 'Polymer, loading, mass Li, count rate, error, count rate per mass\n'
    for f in files:
        runParam = extractRunInfo(f)
        massLi = computeMassLi(runParam[1],runParam[0])
        countRate = GetInteractionRate(f)
        s += '{}, {:5.2f}  ,  {:5.3f} ,   {:5.3f}   , {:4.2f} ,   {:5.3f}\n'.format(runParam[1].ljust(7),runParam[0],massLi,countRate[0],countRate[1],countRate[0]/massLi)
    print s

def OptimizationSummary(path):
    """
    Summerizes the Optimization Output
    """
    # Getting the files
    if not os.path.isdir(path):
        raise IOError('Path {} is not found'.format(path))
    files = glob.glob(path+'/*.m')
    if not files:
        print 'No files matched the pattern'
        return
    # Parsing the files
    data = dict()
    for f in files:
        name = os.path.splitext(os.path.split(f)[1])[0]
        data[name] = GetInteractionRate(f)
    # Max value
    sortedKeys = sorted(data, key=data.get,reverse=True)
    #sortedKeys = sorted(data.items(), key=lambda x : float(x[1][0]),reverse=True)
    for key in sortedKeys[0:9]:
        print '{} -> {:5.2f} +/- {:5.2f}'.format(key,data[key][0],data[key][1])
    for key in sortedKeys[-6:-1]:
        print '{} -> {:5.2f} +/- {:5.2f}'.format(key,data[key][0],data[key][1])

def cleanup(path):
    files = glob.glob(path+'/OUTCyl_*.m')
    for f in files:
        head,tail = os.path.split(f)
        numCylinders = tail.count('-')+1
        if numCylinders == 3:
            newdir = 'ThreeCylPosOpt'
        elif numCylinders == 4:
            newdir = 'FourCylPosOpt'
        elif numCylinders == 5:
            newdir = 'FiveCylPosOpt'
        os.rename(f,os.path.join(newdir,tail))
###########################################################################
#                                                                         # 
#                                  MAIN                                   # 
#                                                                         # 
###########################################################################
import argparse
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-r','--run',action="store_true",
            default=False,help='Runs the cylinders for multiple polymers and precent loadings')
    parser.add_argument('-p','--plot',action="store_true",
            default=False,help='Creates input decks for plotting')
    parser.add_argument('-c','--clean',action="store_true",
            default=False,help='Cleans up the files')
    parser.add_argument('-a','--analysis',action="store_true",default=False,help="Analyze the results")
    parser.add_argument('path', nargs='?', default='CylPosOpt',help='Specifiy the output directory to summerize')
    parser.add_argument('-o','--optimize',action='store',type=int,default=-1,help='Run a number of optimizations on the positions. If 0 is entered a summary is preformed on the directory provided with path')
    parser.add_argument('loading',metavar='loading',type=float,nargs='*',action="store",default=(0.1,0.2,0.3),help='Precent Loading of LiF')
    args = parser.parse_args()
    if args.run:
        run(args.loading,('PS','PEN'))
    if args.plot:
        createInputPlotDecks()
    if args.optimize > 0:
        yPos = (7.625,0,-7.625)
        yPos = (9.15,3.05,-3.05,-9.15)
        #yPos = (10.16,5.08,0.0,-5.08,-10.16)
        pos = CreatePositions(yPos,args.optimize)
        loading = (0.3,)
        polymers = ('PS',)
        PositionOptimization(loading,polymers,pos)
    if args.optimize == 0:
        OptimizationSummary(args.path)
    if args.analysis:
        summerize()
    if args.clean:
        cleanup(os.getcwd())
