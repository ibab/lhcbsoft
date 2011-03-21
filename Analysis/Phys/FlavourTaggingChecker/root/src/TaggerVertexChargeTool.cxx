#include "TaggerVertexChargeTool.h"
#include "NNetTool_MLP.h"

TaggerVertexChargeTool::TaggerVertexChargeTool() {

  declareProperty( "SecondaryVertexName",
		   m_SecondaryVertexToolName = "SVertexOneSeedTool");

  //"NNet", "Probability"
  declareProperty( "UseObsoleteSV", m_UseObsoleteSV       = false );
  declareProperty( "Vtx_AverageOmega",  m_AverageOmega        = 0.41 );

  declareProperty( "Vtx_PowerK",        m_PowerK              = 0.4 );
  declareProperty( "Vtx_MinimumVCharge",m_MinimumVCharge      = 0.17 );
  declareProperty( "Vtx_ProbMin",   m_ProbMin_vtx         = 0.53);
  declareProperty( "Vtx_P0_Cal",  m_P0_Cal_vtx   = 0.451 ); 
  declareProperty( "Vtx_P1_Cal",  m_P1_Cal_vtx   = 0.358 ); 
  declareProperty( "Vtx_Eta_Cal", m_Eta_Cal_vtx  = 0.398 ); 

  //For CombinationTechnique "Probability":
  declareProperty( "P0",           m_P0                   =  5.255669e-01 );
  declareProperty( "P1",           m_P1                   = -3.251661e-01 );
  declareProperty( "Gt075",        m_Gt075                = 0.35 );
  declareProperty( "TracksEq2",    m_wSameSign2           = 0.4141 );
  declareProperty( "TracksGt2",    m_wSameSignMoreThan2   = 0.3250 );

  NNetTool_MLP nnet;
  tVch = new Tagger();
  if(m_SecondaryVertexToolName=="SVertexOneSeedTool") 
    m_svtool= new SVertexOneSeedTool();
  else err()<<"Unknown SecondaryVertexName: "
	    <<m_SecondaryVertexToolName <<endmsg;

}

///////////////////////////////////////////////////
Tagger* TaggerVertexChargeTool::tag(Event& event) {
  tVch->reset();

  verbose()<<"--VertexCharge Tagger--"<<endreq;

  Particles parts = event.particles();
  Particles::iterator i, ip;
  Particles Pfit; Pfit.clear();
  double maxprobf = 0;
  TVector3 BoppDir;

  ///--- Inclusive Secondary Vertex ---
  //look for a secondary Vtx due to opposite B

  Vertex* mysecvtx=NULL;
  
  if(m_UseObsoleteSV) { //use DaVinci processed information
    for (i=parts.begin(); i!=parts.end(); ++i) {
      if((*i)->ComesFromSVtx()) Pfit.push_back(*i);
    }
  } else { //use seed from DaVinci and add tracks custom
   Vertices allvtx = m_svtool->buildVertex( parts, &event );
   if(allvtx.empty()) return tVch;
   mysecvtx = allvtx.at(0);
    if( mysecvtx->type() != Vertex::Secondary ) return tVch;
    Pfit = mysecvtx->outgoingParticles();
    maxprobf= mysecvtx->likelihood();
    BoppDir = mysecvtx->position();//point and svtau
  }

  if(msgLevel(MSG::DEBUG)) debug()<<"Pfit.size: "<<Pfit.size()<<endreq;

  //if Vertex does not contain any daughters, exit
  if(Pfit.size()<1) return tVch;
  if(msgLevel(MSG::DEBUG)) debug()<<"--- SVTOOL buildVertex returns vertex"
				  <<" with "<<Pfit.size()<<"tracks"<<endreq;
  if(msgLevel(MSG::DEBUG)) debug()<<" -- likelihood seed "<<maxprobf<<endreq;

  //calculate vertex charge and other variables for NN
  double Vch = 0, norm = 0;
  double Vptmin = 0, Vipsmin = 0, Vflaglong = 0, Vdocamax = 0;
  int vflagged = 0;
  TLorentzVector SVmomentum;
  for(ip=Pfit.begin(); ip!=Pfit.end(); ip++) { 
    if(msgLevel(MSG::DEBUG)) debug() <<"SVTOOL  VtxCh, adding track pt= "<<(*ip)->pt()<<endmsg;
    SVmomentum += (*ip)->momentum();//for tau and point
    double a = pow((*ip)->pt()/GeV, m_PowerK);
    Vch += (*ip)->charge() * a;
    norm+= a;
    vflagged++;
    Vptmin  += (*ip)->pt()/GeV;
    Vipsmin += (*ip)->IPs();
    if( (*ip)->type()== Particle::Long || 
        (*ip)->type()== Particle::Matched) Vflaglong++; //old FTC long and matched have trtyp=7
    Vdocamax += (*ip)->DOCA();
    if(msgLevel(MSG::DEBUG)) debug()<<"docaSV:"<<(*ip)->DOCA()<<endmsg;
  }
  if(norm) {
    Vch /= norm;
    tVch->setcharge( Vch ); //to use only for Vch plot
    if(fabs(Vch) < m_MinimumVCharge ) Vch = 0;
    Vptmin  /= vflagged;
    Vipsmin /= vflagged;
    Vdocamax/= vflagged;
  }
  if(msgLevel(MSG::DEBUG)) debug()<<"Vch: "<<Vch<<endreq;

  //****TestNewVariables****//
  double BDphiDir = BoppDir.DeltaPhi(SVmomentum.Vect());
  //if (BDphiDir>1.2) return tVch; 

  //add tau of Bopp
  double SVP = SVmomentum.P();
  double SVM = SVmomentum.M();
  double SVGP = SVP/(0.16*SVM+0.12);
  double SVtau = BoppDir.Mag()*5.28/SVGP/0.299792458;
 
  if(!m_UseObsoleteSV) { //needed by NNtuner
    mysecvtx->setVflagged(vflagged);
    mysecvtx->setVptmin(Vptmin);
    mysecvtx->setVipsmin(Vipsmin);
    mysecvtx->setVdocamax(Vdocamax);
    mysecvtx->setVratio(Vflaglong/(vflagged? vflagged:1));
    mysecvtx->setVCharge(Vch); 
    mysecvtx->setSVtau(SVtau); 
  }
  
  if( Vch==0 ) return tVch;

  //calculate omega
  if(msgLevel(MSG::DEBUG)) debug()<<"calculate omega, use obsolete (0=no): "<<m_UseObsoleteSV<<endreq;
  double omega = m_AverageOmega;
  if(m_UseObsoleteSV) {
    if( fabs(Vch)<0.75 ) omega = m_P0 + m_P1*fabs(Vch) ;
    if( fabs(Vch)>0.75 ) omega = m_Gt075;
    if( fabs(Vch)>0.99 ) { // tracks have all the same charge
      if(Pfit.size()==2) omega = m_wSameSign2;
      if(Pfit.size() >2) omega = m_wSameSignMoreThan2;
    }
    nnet.m_rnet_vtx = fabs(Vch);
  } else {
    std::vector<double> NNinputs(11);
    NNinputs.at(0) = parts.size();
    NNinputs.at(1) = event.pileup();
    NNinputs.at(2) = event.signalB()->pt();
    NNinputs.at(3) = vflagged;
    NNinputs.at(4) = Vptmin;
    NNinputs.at(5) = Vipsmin;
    NNinputs.at(6) = Vdocamax;
    NNinputs.at(7) = maxprobf;
    NNinputs.at(8) = Vflaglong/(vflagged? vflagged:1);
    NNinputs.at(9) = fabs(Vch); 
    NNinputs.at(10) = SVtau;

    omega = 1 - nnet.MLPvtx( NNinputs );
  }

  verbose() <<" VtxCh= "<< Vch <<" with "<< Pfit.size() <<" parts"
            <<", omega= "<< omega <<endmsg;

  //Calibration (w=1-pn) w' = p0 + p1(w-eta)
  omega = m_P0_Cal_vtx + m_P1_Cal_vtx * ( omega-m_Eta_Cal_vtx);
  debug()<<" Vtx pn="<<1-omega<<" w="<<omega<<endmsg;
  
  if( 1-omega < m_ProbMin_vtx ) return tVch;
  if(   omega > m_ProbMin_vtx ) return tVch;

  verbose()<<"Vtx passed"<<endreq;

  tVch->setDecision( Vch>0 ? -1 : 1 );
  tVch->setRawNNetOutput( nnet.m_rnet_vtx );
  tVch->setOmega( omega );
  tVch->setType( Tagger::VtxCharge ); 
  for(ip=Pfit.begin(); ip!=Pfit.end(); ip++) {
    tVch->addToTaggerParts(*ip);
  }
  
  return tVch;
}
