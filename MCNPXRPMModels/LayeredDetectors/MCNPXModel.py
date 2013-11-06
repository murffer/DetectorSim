""" 
MCNPX Model for RPM8
"""
import subprocess
from RPMModel import RPMModel
import sys
sys.path.append('../MCNPTools/')
import mctal
class MCNPX(RPMModel):
    # Material Dictionaries
    cellForStr = '{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505\n'
    surForStr = '{:5d} px {:5.3f}\n'
    geoParam={'GenomeLength':13,'LayersPerAssembly':2,
            'RPM8Size':12.7,'DetectorThickness':0.01}
    
    def __init__(self,inp='INP.mcnp'):
        """ MCNPX Model of RPM8

        Keywords:
        inp -- desired name of the input deck
        """
        super(MCNPX,self).__init__(inp) 
        # Material dictionary for the moderator, light guide, and detector
        self.material['Detector'] = {'name':'Detector','mt': 3, 'rho': 1.1}       # detector
        #self.material['LightGuide'] = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
        self.material['LightGuide'] = {'name': 'HDPE','mt':456, 'rho':0.93}   # PMMA
        self.material['Moderator'] = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE
        
        # Cell and Surface Inital Numbering
        self.CellStartNum = 6000
        self.SurfaceStartNum = 6000
        self.ZeroSurfaceNum = 500
        self.geo = None
        self.inp = inp
        self.name = 'OUT_'+self.inp.strip('.mcnp')+'.'
   
    def __str__(self):
        s = super(MCNPX,self).__str__()
        s += '\n\tMCNPX Model\n'
        s += '\t Cell Number Starts: {0:d}\n'.format(self.CellStartNum)
        s += '\t Surface Number Starts: {0:d}\n'.format(self.SurfaceStartNum)
        return s

    def getInteractionRate(self):
        """ Returns the interaction rate """
        m = mctal.MCTAL(self.name+'.m')
        t = m.tallies[4]
        # Returning the total
        return t.data[-1],t.errors[-1]

    
    def runModel(self):
        """ 
        Runs the Model by submission to Tourqe / Maui
        """
        qsub= subprocess.check_output('which qsub',shell=True).strip()
        cmd = '#!/bin/bash\n'
        cmd += '#PBS -N {0}\n#PBS -V\n#PBS -q gen2\n#PBS -l nodes=1:ppn=1\n'
        cmd += 'cd $PBS_O_WORKDIR\nmpirun mcnpx inp={1} name={2}\n'
        job = cmd.format(self.genome,self.inp,self.name)
        with open('qsub','w') as o:
            o.write(job)
        subprocess.call(qsub+' qsub',shell=True)
        subprocess.call('rm qsub',shell=True)
    
    def createInputDeck(self,inpName=None,outName=None):
        """ createInputDeck 

        Creates an input deck of the given geometry
        
        KEYWORDS:
            inpName - input file name (if none then  set as INP_genome_.mcnp)
            outNmae - output file name (if None then set as OUT_genome.)
        """
        self.name = 'OUT_'+self.genome+'.'
        self.inp = 'INP_'+self.genome+'.mcnp'
        if inpName:
            self.inp = inpName
        if  outName:
            self.name = outName
       
        # Error checking on the file name
        if len(self.inp) > 30:
            self.inp = self.inp.replace('0'*4,'M')
        if len(self.name) > 30:
            self.name = self.name.replace('0'*4,'M')
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
###########################################################################
#                                                                         #
#                           MCNPX Population                              #
#                                                                         #
###########################################################################
import os
import glob

class MCNPXPop(object):
    """
    MCNPX RPM Population
    """
    def __init__(self,outputDir=None):
        print 'init in MCNPXPop'
        # File output directory
        if outputDir is None:
            outputDir = 'mcnpx_output'
        self.outputDir = outputDir
        print 'Output directory: '+self.outputDir
        if not os.path.exists(self.outputDir):
            os.makedirs(self.outputDir)
            print 'Should have made the directory' 
        # Previous Run Data
        self.runs = self.loadRuns(self.outputDir)
        self.model = MCNPX 
   
    def isSimIndividual(self,genome):
        """
        Checks if the individual has been simulated
        
        Keywords:
        genome - bit string genome to check if it is in the run dictionary
        """
        return genome in self.runs

    def getIndividual(self,genome):
        """
        returns the interaction rate / activity of an individual genome.

        This is just a wrapper for the self.runs dictionary of runs, and
        will not run the genome if it isn't in the dictionary.
        """
        if genome not in self.runs:
            genomes = [genome]
            run = self.runPopulation(genomes,False)
            self.runs.update(run)
        return self.runs[genome]

    def loadRuns(self,directory):
        """
        Loads the previous run data contained directory. A dictionary
        between the genome and the total reaction rate is returned.

        Keywords:
        directory - the directory containing the output files.  Very little
        type checking for the files is preformed, so this may break if the
        directory contains unexpected files.
        """
        files = glob.glob(directory+'/*.m')
        self.runs = dict()
        for f in files:
            data = self.extractInteractionRate(f)
            print data
            self.runs.update(data)
        return self.runs 

    def extractInteractionRate(self,filename):
        """
        Extracts the interaction rate from the file, returning a dictionary
        between the genome and the reaction rate

        Keywords:
        filename - the file of the mcnpx mctal output
        """
        m = mctal.MCTAL(filename)
        tally = m.tallies[54]
        countRate = tally.data[-1]*2.3E3 
        title = m.header.title
        genome = title.strip('MCNPX Simulation of RPM8 Genome: ')
        return  {genome:countRate} 

    def runPopulation(self,genomes,useQueue=False):
        """
        Runs a population of genomes representing RPM geometries.
        
        If the user doesn't use the queue, than a dictionary between the 
        activities and the genomes is returned.  If the user submits the job
        to the queue then None is returned.
        
        Keywords:
        genomes - a list of bit string geometries that should be run
        useQueue - if true a job is submitted to the Torque / Maui Neclsuter
                    queue.  If false (the default) the job is run as is, and
                    the xsdrn calculation (through batch6.1) blocks until 
                    the calcualtion is completed.
        """
        # Only running unique items that haven't been run already
        # Creating the input deck
        genomes = set(genomes)
        for genome in genomes:
            if genome not in self.runs:
                m  = MCNPX()
                m.createBinaryGeometry(genome)
                inStr = 'INP_'+genome+'.mcnp'
                outStr = self.outputDir+'/OUT_'+genome+'.'
                m.createInputDeck(inStr,outStr)
                # Running the job
                cmd = 'mpirun mcnpx inp={1} name={2}'
                cmd = cmd.format(genome,m.inp,m.name)
                subprocess.call(cmd,shell=True)
                filename = m.name+'m'
                print filename
                data = self.extractInteractionRate(filename)
                # Adding to the run dictionary
                self.runs.update(data)
        return self.runs

    def writeRuns(self,outputName=None):
        """
        Writes the runs to and output file
        """
        if outputName is None:
            outputName = self.outputDir+'MCNPXSummary.csv'
        with open(outputName,'w') as f:
            for genome in self.runs:
                f.write('{0},{1:8.5f}\n'.format(genome,self.runs[genome]))
        print 'Wrote data to : '+outputName

    def UpdatePopulation(self,filename):
        """
        Adds the genome, activity pair in the csv file filename

        Keywords:
        filename - csv file to read in (from writeRuns)
        """
        with open(filename,'r') as f:
            for line in f:
                genome,activity = line.split(',')
                self.runs[genome.strip()] = float(activity) 

def Plot(genomes):
    for genome in genomes:
        model =  RPMModel()
        m  = MCNPX()
        geo = model.getBinaryGeoemtry(genome)
        m.setGeo(genome,geo)
        print 'The geoemtry for {}'.format(genome)
        m.printGeo()
        inStr = 'INP_'+genome+'.mcnp'
        outStr = 'OUT_'+genome+'.'
        m.createInputDeck(inStr,outStr)


import argparse
if __name__ == "__main__":
    """
    Running the 
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-p','--plot',action="store_true",
            default=False,help='Creates input decks for plotting')
    args = parser.parse_args()
    if args.plot:
        genomes = ('01111101110100001000','011101001000000','0011010000')
        Plot(genomes)
    if not args.plot:
        print 'Did not chose an option, nothing to be done'

