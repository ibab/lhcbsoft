class Exclusive_Nominal :
    """
    Threshold settings for Hlt1 and 2 in nominal conditions (list of exclusive)
    
    WARNING :: DO NOT EDIT WITHOUT PERMISSION OF THE AUTHORS
    
    @author P.Koppenburg, M. Gandelman, G. Raven, K. Vervink
    @date 2009-08-05
    """
    
    __all__ = ( 'ActiveHlt2Lines' )
    
    
    def ActiveHlt2Lines(self) :
        """
        Returns a list of active lines
        """

        lines = []
        # Hlt2B2DXLines.py
        lines.append( [ 
              'Hlt2B2D2hhBachelorReq3BodySignal'
            , 'Hlt2B2D2hhBachelorReq4BodySignal'
            , 'Hlt2B2D2hhhBachelorReq3BodySignal'
            , 'Hlt2B2D2hhhBachelorReq4BodySignal'
            , 'Hlt2B2D2hhKstarReq3BodySignal'
            , 'Hlt2B2D2hhKstarReq4BodySignal'
            , 'Hlt2B2D2hhBachelorReq3BodyMW'
            , 'Hlt2B2D2hhBachelorReq4BodyMW'
            , 'Hlt2B2D2hhhBachelorReq3BodyMW'
            , 'Hlt2B2D2hhhBachelorReq4BodyMW'
            , 'Hlt2B2D2hhKstarReq3BodyMW'
            , 'Hlt2B2D2hhKstarReq4BodyMW'
            ] )
        # Hlt2B2JpsiXLines.py
        lines.append( [
              'Bs2JpsiPhiPrescaled'
            , 'Bs2JpsiPhi'
            , 'Bd2JpsiKstar'
            , 'Bu2JpsiK'
            , 'Bd2JpsiMuMuKsLLBiased'
            , 'Bd2JpsiMuMuKsLLUnbiased'
            , 'Bd2JpsiMuMuKsDDBiased'
            , 'Bd2JpsiMuMuKsDDUnbiased'
            , 'B2JpsiX_MuMu'
            ] )
        
        # Hlt2B2LLXLines.py
        lines.append( [
              'Hlt2Bu2eeK'
            , 'Hlt2Bu2eeKSignal'
            , 'Hlt2Bu2eeKJpsi'
            , 'Hlt2Bu2eeKHighMass'
            , 'Hlt2Bu2MuMuK'
            , 'Hlt2Bu2MuMuKSignal'
            , 'Hlt2Bu2MuMuKJpsi'
            , 'Hlt2Bu2MuMuKHighMass'
            ] )
        # Hlt2DisplVerticesLines.py
        lines.append( [
              'Hlt2DisplVertices'
            ] )
        # Hlt2B2XGammaLines.py
        lines.append( [
              'Hlt2Bs2PhiGamma'
            , 'Hlt2Bd2KstGamma'
            ] )
        # Hlt2B2HHLines
        lines.append( [
            'Hlt2B2HH'
            ] )
        # Hlt2B2PhiXLines
        lines.append( [
            'Hlt2Bs2PhiPhi'
            ] )
        
        return lines

   
