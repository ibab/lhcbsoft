// $id:  $

// // from Gaudi
#include "GaudiKernel/ToolFactory.h"

// from Event
#include "Event/State.h"
//#include "Event/RecHeader.h"

#include "Event/L0MuonCandidate.h"
#include "MuonDet/IMuonPosTool.h"
#include "TrackInterfaces/ITrackPtKick.h"

//local
#include "PrepareMuonSeed.h"

using namespace LHCb;

// Declaration of the Tool Factory
// static const  ToolFactory<PrepareMuonSeed>          s_factory ;
// const        IToolFactory& PrepareMuonSeedFactory = s_factory ;


DECLARE_TOOL_FACTORY( PrepareMuonSeed );


// Standard Constructor
PrepareMuonSeed::PrepareMuonSeed(const std::string& type, 
                                         const std::string& name, 
                                         const IInterface* parent) 
  : GaudiTool( type, name, parent )
    
{
  // Declare the algorithm's properties which can be set at run time
    declareInterface<IPrepareMuonSeed>(this);

  
}

// Standard Destructor -------------------------------------
PrepareMuonSeed::~PrepareMuonSeed() {}

//-------------------------------------
StatusCode PrepareMuonSeed::initialize() 
{
  StatusCode sc = GaudiTool::initialize();
  if (sc.isFailure()){
    return sc;
  }
  //  always() << " Initialize PrepareMuonSeed" << endmsg;

  m_iPosTool=tool<IMuonPosTool>( "MuonPosTool" );
  m_fCalcPtKick = tool<ITrackPtKick>("TrackPtKick");
  
  m_debugInfo = false; //@ja put in job options
  
  return StatusCode::SUCCESS;
}

StatusCode PrepareMuonSeed::prepareSeed( const LHCb::L0MuonCandidate& muonL0Cand,
                                         LHCb::State& seedState )
{
  //always()<<"PrepareMuonSeed::execute!"<<endmsg;

  seedState.setState(0,0,0,0,0,0);

  MuonTileID mpad1 = muonL0Cand.pad(0);
  MuonTileID mpad2 = muonL0Cand.pad(1);
  //MuonTileID mpad3 = muonL0Cand.pad(2);

    int regionL0Cand;
    if (mpad2){
      regionL0Cand = mpad2.region();
    }
    // else if( mpad3 ){
    //       regionL0Cand = mpad3.region();
    //     }
    else{
      err()<<"No valid Muon Tile in M2, quit loop"<<endmsg;
      return StatusCode::SUCCESS;
    }

    double xTileM1 , yTileM1, zTileM1;
    double dxTileM1, dyTileM1, dzTileM1;
   
    //positions in M1
    StatusCode sc = m_iPosTool->calcTilePos( mpad1,
                                             xTileM1, dxTileM1,
                                             yTileM1, dyTileM1,
                                             zTileM1, dzTileM1);

    if (!sc)  return StatusCode::SUCCESS;

    double xTileM2 , yTileM2, zTileM2;
    double dxTileM2, dyTileM2, dzTileM2;
    
    sc = m_iPosTool->calcTilePos(mpad2,
                                 xTileM2, dxTileM2,
                                 yTileM2, dyTileM2,
                                 zTileM2, dzTileM2);
    
    if (!sc)  return StatusCode::SUCCESS;

    double zT3 = 9315.;//middle T3
    double dxdz   = (xTileM2 - xTileM1)/ ( zTileM2 - zTileM1);
    double dydz   = (yTileM2 - yTileM1)/ ( zTileM2 - zTileM1);
    
    double xT3 = xTileM2 - dxdz * (zTileM2 - zT3);
    double yT3 = yTileM2 - dydz * (zTileM2 - zT3);

    double sigmaX = 0.;
    double sigmaY = 0.;
    if (regionL0Cand == 0) sigmaX = 8. ; sigmaY = 6.;
    if (regionL0Cand == 1) sigmaX = 15.; sigmaY = 11.;
    if (regionL0Cand == 2) sigmaX = 30.; sigmaY = 22.;
    if (regionL0Cand == 3) sigmaX = 57.; sigmaY = 44.;

    //double L0p = muonL0Cand.pt();//first estimate
    
    //@ja do i need q/p info?
    seedState.setState( xT3 , yT3 , zT3 , dxdz , dydz , 0 );
    m_fCalcPtKick->calculate(&seedState);
    
    
    double L0p = seedState.p(); //this p is more accurate
    double L0pt = muonL0Cand.pt();//get pt with sign!
    double L0q    = (L0pt == 0) ? 0.0 : -1.*L0pt/fabs(L0pt);
    double qOverP = L0q / L0p;
    
    seedState.setQOverP( qOverP );
        
    Gaudi::TrackSymMatrix stateCov = Gaudi::TrackSymMatrix();
    
    stateCov(0,0) = sigmaX*sigmaX;
    stateCov(1,1) = sigmaY*sigmaY; 
    stateCov(2,2) = 2.8e-3; //not used
    stateCov(3,3) = 1.5e-3;
    stateCov(4,4) = 8.41e-6;

    seedState.setCovariance(stateCov);

    return StatusCode::SUCCESS;
    
}
