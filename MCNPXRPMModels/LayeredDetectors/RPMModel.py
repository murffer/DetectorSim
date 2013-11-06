"""
RPM Model
"""

class RPMModel(object):
    """
    RPMModel - model of a layered RPM8

    Shared data:
    geoParam            - dictionary of geometry parameters
    
    Instance Data:
    inp                 - input deck name
    genome              - genome(s) simulated by this geometry
    geo                 - geometry dictionary for the geometry
                            This is a mapping between a position and a material
    """
    geoParam={'RPM8Size':12.7,'DetectorThickness':0.01}

    def __init__(self,inp=None):
        self.inp = inp
        self.geo = None
        self.genome = None
        self.material =  {'Moderator':None,'Detector':None,'LightGuide':None}
   
    def __str__(self):
        """
        Prints class information
        """
        s = 'RPM Model Information:\n\tself.inp = '+self.inp+'\n'
        if self.material:
            for m in self.material:
                s+= '\t'+str(m)+': '+str(self.material[m])+'\n'
        else:
            s+= '\tNo materials in dictionary\n'
        if self.genome:
            s+= '\tGenome: '+self.genome
        else:
            s+='\tNo genome\n'
        return s
    
    def setInp(self,inp):
        """
        Sets the input deck name of the model
        
        KEYWORDS:
            inp - the new input deck of the file
        """
        self.inp = inp
        
    def setOutputName(self,name):
        """
        Sets the name of the output file
        
        KEYWORDS:
            name - the new output name of the simulation
        """
        self.name = name
        
    def getSliceThickness(self,genomeLength):
        """
        Returns the slice thickness
        
        KEYWORDS:
            genomeLength - the length of the genome
        """
        return self.geoParam['RPM8Size']/float(genomeLength)
    
    def getInteractionRate(self):
        """
        Returns the interaction rate
        """
        raise NotImplementedError()

    def printGeo(self,geo=None):
        """
        Prints the geometry
        
        KEYWORDS:
            geo - geometry to print. If geo is None, than the default
            geoemtry of the model is printed.
        """
        if geo is None:
            geo = self.geo
        keyList = sorted(geo.keys(), key = lambda x : float(x))
        for key in keyList:
            print 'x: {:5.3f} {}'.format(key,geo[key])
        
    def setCompostion(materialName):
        """
        sets the material composition
        
        """
        raise NotImplementedError()
    
    def setGeo(self,newGenome,newGeo):
        """
        Sets a new geometry
        
        KEYWORDS:
            newGenome - the new genome to consider
            newGeo    - the new geometry dictionary
        """
        self.genome = newGenome
        self.geo = newGeo
        
    def runModel(self):
        """
        Runs the model
        """
        raise NotImplementedError()
    
    def getBinaryGeoemtry(self,genome):
        """
        Creates and returns a binary geoemtry
        """
        self.createBinaryGeometry(genome)
        return self.geo

    def createBinaryGeometry(self,genome):
        """
        Creates a binary goemetry
        """
        # Computing parameters
        self.genome = genome
        slices = len(genome)
        sT = self.geoParam['RPM8Size']/slices
        modT = sT
        lGT = sT - self.geoParam['DetectorThickness']
        dT = self.geoParam['DetectorThickness'] 
        
        # Creating the geometry dictionary
        geo = dict()
        numDetector = 0
        x = 0
        for s in genome:
            if s is '1':
                # Detector
                x += dT
                geo[x] = 'Detector'
                numDetector += 1
                x += lGT
                geo[x] = 'LightGuide'
            else:
                # Moderator
                x += modT 
                geo[x] = 'Moderator'
        
        # Checking that we ended correctly
        if (abs(x-self.geoParam['RPM8Size']) >= 0.0001):
            errStr = 'x ({:5.3f}) is not within a micron'
            errstr += 'of the  detector size ({:5.3f})'
            errStr.format(x,self.geoParam['RPM8Size'])
            raise ValueError(errStr)
        
        # Returning the geometry dictionary
        self.geo = geo
        return numDetector

    def createInputDeck(self,oFile=None):
        """
        Creates an input deck of the geometry
        """
        raise NotImplementedError()

    def runModel(self):
        """ 
        Runs the Model
        """
        raise NotImplementedError()
