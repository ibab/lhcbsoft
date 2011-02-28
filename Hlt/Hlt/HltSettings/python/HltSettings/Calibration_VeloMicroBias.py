class Calibration_VeloMicroBias :
    """
    Threshold settings for OT aging activity 
    
    WARNING :: DO NOT EDIT WITHOUT PERMISSION OF THE AUTHORS
    
    @author J. Albrecht
    @date 2010-08-09
    """
    
    
    def __init__(self) :
        self.StripEndSequence = ['']

    def HltType(self) :
        return 'Calibration_VeloMicroBias'

    def L0TCK(self) :
        return None

    def ActiveHlt2Lines(self) :
        """
        Returns a list of active Hlt2 lines
        """
        return  []
   
    def ActiveHlt1Lines(self) :
        """
        Returns a list of active Hlt1 lines
        """
        return [ 'Hlt1MBMicroBiasVelo' ]

    def Thresholds(self) :
        """
        Returns a dictionary of cuts
        """
        from Hlt1Lines.Hlt1MBLines             import Hlt1MBLinesConf
        d= { Hlt1MBLinesConf : { 'MicroBiasOdin' : '(ODIN_BXTYP == LHCb.ODIN.BeamCrossing)'
                                 ,'Prescale' : { 'Hlt1MBMicroBias.*' : 0.01 } 
#                                   , 'Postscale': { 'Hlt1MBMicroBias.*' : 'RATE(5000)' }
                                   }

             }
        


        return d
