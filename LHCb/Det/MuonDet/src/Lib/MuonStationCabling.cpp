// $Id: MuonStationCabling.cpp,v 1.1 2004-02-03 11:40:27 asatta Exp $
// Include files 



// local
#include "MuonDet/MuonStationCabling.h"
#include <iostream>

//-----------------------------------------------------------------------------
// Implementation file for class : MuonStationCabling
//
// 2004-01-07 : Alessia Satta
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonStationCabling::MuonStationCabling(  ) {
  m_numberOfL1Board=0;
  
}
//=============================================================================
// Destructor
//=============================================================================
MuonStationCabling::~MuonStationCabling() {}; 

//=============================================================================
StatusCode MuonStationCabling::update(std::string newL1Pointer){
  m_numberOfL1Board++;
  m_listOfL1.push_back(newL1Pointer);
  
};

//long MuonStationCabling::getNumberOfL1Board(){
//std::cout<<"ciao "<<m_numberOfL1Board<<std::endl;
//return m_numberOfL1Board;
//};

