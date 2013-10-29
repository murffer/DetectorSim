#!/usr/bin/env python

""" 
MCNPX Model for Cylindrical RPM8
"""
#import mctal
import subprocess
class CylinderRMP(object):
    # Material Dictionaries
    cellForStr = '{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505\n'
    surForStr = '{:5d} cz {:5.3f}\n'
    geoParam={'RPM8Size':12.7,'DetectorThickness':0.01,'DetectorSpacing':0.4,
               'CylinderLightGuideRadius':0.5,'CylinderRadius':2.5}
    
    def __init__(self,inp='INP.mcnp'):
        """ Wrapped Cylinder MCNPX Model of RPM8

        Keywords:
        inp -- desired name of the input deck
        """
        # Material dictionary for the moderator, light guide, and detector
        self.material =  {'Moderator':None,'Detector':None,'LightGuide':None}
        self.material['Detector'] = {'name':'Detector','mt': 3, 'rho': 1.1}       # detector
        self.material['LightGuide'] = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
        self.material['Moderator'] = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE
        
        # Cell and Surface Inital Numbering
        self.CellStartNum = 6000
        self.SurfaceStartNum = 6000
        self.ZeroSurfaceNum = 500
        self.surfGeo = None
        self.inp = inp
        self.name = 'OUT_'+self.inp.strip('.mcnp')+'.'
   
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

    def createSurfaceGeo(self):
        """
        Creates a dictionary of surface positions and cylinders
        """
        self.surfGeo = dict()
        r = self.geomParam['CylinderLightGuideRadius']
        self.surfGeo[r] = 'LightGuide'
        #self.material =  {'Moderator':None,'Detector':None,'LightGuide':None}
        while(r + self.geoParam['DetectorThickenss'] < self.geoParam['CylinderRadius']):
            r += self.geoParam['DetectorThickness']
            self.surfGeo[r] = 'Detector'
            r += self.geoParam['DetectorSpacing']
            if (r < self.geoParam['CylinderRadius']):
                self.surfGeo[r] = 'LightGuide'
        return self.surfGeo
    
    def createDetectorUniverise(xpos=0,ypos=0,uNum=0):
        """
        Creates a detector univerise string
        """
        s = ''
        return s
    
    def runModel(self):
        """ 
        Runs the Model by submission to Tourqe / Maui
        """
        qsub= subprocess.check_output('which qsub',shell=True).strip()
        cmd = '#PBS -N {0}\n#PBS -V\n#PBS -q gen2\n#PBS -l nodes=1:ppn=1\n'
        cmd += 'cd $PBS_O_WORKDIR\nmpirun mcnpx inp={1} name={2}\n'
        job = cmd.format(self.genome,self.inp,self.name)
        with open('qsub','w') as o:
            o.write(job)
        subprocess.call(qsub+' qsub',shell=True)
        subprocess.call('rm qsub',shell=True)
    
    def createInputDeck(self):
        """ createInputDeck 

        Creates an input deck of the given geometry
        """
        self.inp = 'INP_'+self.genome+'.mcnp'
        self.name = 'OUT_'+self.genome+'.'
        oFile = self.inp
        
        # Problem Constants
        cellString = 'c ------------------------- Source ----------------------------------------\n'
        cellString += '70   5  -15.1       -70                               $ 252Cf source      \n'       
        cellString += '71   406  -11.34    -71 70                            $ Lead around source\n'
        cellString += '72   456  -0.93       -72 71                          $ Poly around source\n'

        surfString = 'c  ########################### Surface Cards ##############################\n'
        surfString += 'c ------------------- Encasing Bounds (Size of RPM8) ---------------------\n'
        surfString += '500 px 0                                                                  \n'
        surfString += '501 px 12.7                                                               \n'
        surfString += '502 py -15.25                                                             \n'
        surfString += '503 py 15.25                                                              \n'
        surfString += '504 pz 0                                                                  \n'
        surfString += '505 pz 217.7                                                              \n'
        
        # Add in other cells here
        numCells = 4 # 3 Source, 1 RPM8 Encasingdd
        detectorCells = list()
        rpmCells = list()
        keyList = sorted(self.geo.keys(), key = lambda x: float(x))
        
        ##################################################################
        #                       Write the cells                          #
        ##################################################################
        cNum = self.CellStartNum
        sNum = self.SurfaceStartNum
        sPrev = self.ZeroSurfaceNum
        
        for key in keyList:
            # Creating the cell
            m = self.material[self.geo[key]] 
            cellString += self.cellForStr.format(cNum,m['mt'],m['rho'],sPrev,sNum)
            rpmCells.append(cNum) 
            # List of cells for the detector
            if self.geo[key] is 'Detector':
                detectorCells.append(cNum)

            # Incrementing counters
            sPrev = sNum
            sNum += 1
            cNum += 1
            numCells +=1

        ##################################################################
        #                      Write the Surface                         #
        ##################################################################
        surfString += 'c ------------------------ Detector Bounds -------------------------------\n'
        sNum = self.SurfaceStartNum
        sPrev = self.ZeroSurfaceNum
        for key in keyList:
            surfString += self.surForStr.format(sNum,float(key))
            sPrev = sNum
            sNum += 1
        
        ##################################################################
        #                      Write the Tallies                         #
        ##################################################################
        tallyString = 'c ------------------------- Tallies Yo! -----------------------------------\n'
        tallies = {'F4:n':{'cells':rpmCells,'comments':'FC4 Cell Flux\n','options':'\nE4 6E-7 10 '},
                'F54:n':{'cells':detectorCells,'comments':'FC54 6Li Reaction Rates\n',
                    'options':' T\nSD54 1 {0:d}R\nFM54 -1 3 105'}}
        for t in tallies:
            # Getting a list of cells
            tallyString += tallies[t]['comments']
            tallyString += str(t)+' '
            cell = list(str(c)+' ' for c in tallies[t]['cells'])
            cell = [cell[i:i+6] for i in range(0,len(cell),6)]
            tallyString += '      '.join(''.join(i)+'\n' for i in cell)
            tallyString = tallyString.rstrip()
            tallyString += tallies[t]['options'].format(len(tallies[t]['cells']))
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

        
        with open(oFile,'w') as o:
            o.write('MCNPX Simulation of RPM8 Genome: '+self.genome+'\n')
            o.write(cellString)
            o.write('\n')
            o.write(surfString)
            o.write('\n')
            o.write(self.getRunString().format(numCells))
            o.write(self.getSrcString())
            o.write(tallyString)
            o.write(self.getMaterialString())
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
        matString = 'c -------------------------- Material Cards -----------------------------\n'
        matString += 'm3    6012.70c  -0.520977  $ 70% PEN, 25% LiF, 5% PPO/POPOP rho=1.1 g/cc\n'
        matString += '      6013.70c  -0.00563475  1001.70c   -0.0316177   1002.70c   -3.63645e-06\n'
        matString += '      8016.70c  -0.188123    8017.70c   -7.16609e-05 8018.70c   -0.000386592\n'
        matString += '      7014.70c  -0.00317338  7015.70c   -1.17212e-05  3006.70c  -0.0574746  \n'
        matString += '      3007.70c  -0.00302205  9019.70c   -0.189503                           \n'
        matString += 'm10   1001.70c  -0.080538       $Lucite (PMMA / Plexiglass)    rho = 1.19 g/cc\n'
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

if __name__ == "__main__":
    genome = '0001001000010010000000000'
    m  = MCNPX()
    print m
    m.createBinaryGeometry(genome)
    m.printGeo()
    m.createInputDeck()
    m.runModel()
