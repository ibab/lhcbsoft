#ifndef NNTuner_H
#define NNTuner_H 1

#include <iostream>
#include <vector>
#include <TLorentzVector.h>
#include <TString.h>
#include <TTree.h>
#include <TFile.h>
#include <cmath>

#include "Tagger.h"
#include "Particle.h"
#include "Event.h"

#include "TaggerElectronTool.h"
#include "TaggerKaonOppositeTool.h"
#include "TaggerKaonSameTool.h"
#include "TaggerMuonTool.h"
#include "TaggerPionSameTool.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TCanvas.h"

/** @class NNTuner
 *
 *  Class used to retune the nnets of taggers and combination.
 *
 *  @author Marco Musy
 *  @date   2010-06-29
 */

class NNTuner {

 public:

  NNTuner( ) { nntrain=0; fnnet=0; name=""; }
  ~NNTuner( ){ } ///< Destructor
  NNTuner(TString& );

  void Fill(Event&, Taggers&);
  void TrainNNet();

  void Write() {   
    if(fnnet) fnnet->Write();
  }
  void Close() {   
    if(fnnet) fnnet->Close(); 
  }

 private:
  TTree*  nntrain;
  TFile*  fnnet;
  TString name;

  Int_t   iscorrect,tagger;
  Float_t mult,ptB,partP,partPt,IPPV,nndeta,nndphi,nndq;

};

#endif 
