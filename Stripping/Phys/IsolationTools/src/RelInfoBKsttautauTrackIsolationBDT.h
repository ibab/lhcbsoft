#ifndef RELINFOTRACKISOVARIABLESV2_H 
#define RELINFOTRACKISOVARIABLESV2_H 1

// Include files
// from DaVinci, this is a specialized GaudiAlgorithm
#include "Kernel/DaVinciAlgorithm.h"
#include "Kernel/GetIDVAlgorithm.h"
#include "Kernel/IRelatedInfoTool.h"
#include "Event/RelatedInfoMap.h"
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IDistanceCalculator.h"
//needed for TMVA
#include "MVADictTools/TMVATransform.h"
#include "Kernel/IParticleDictTool.h"
#include "Event/Particle.h"
#include "Kernel/IParticleDescendants.h"

/** @class RelInfoBKsttautauTrackIsolationBDT RelInfoBKsttautauTrackIsolationBDT.h
 *
 * \brief Calculate track isolation.
 *    Compare daughter
 *
 *    m_bdt1 : bdt value
 *
 * Options:
 *
 *   None
 *
 *  Converted from ConeVariables by A. Shires 18/06/2014
 *
 *  @author Giampiero Mancinelli
 *  @date   2014-07-25
 *
 */

class IDVAlgorithm;
class IDistanceCalculator;
class IParticleDictTool ;

class RelInfoBKsttautauTrackIsolationBDT : public GaudiTool, virtual public IRelatedInfoTool {
    public:
        /// Standard constructor
        RelInfoBKsttautauTrackIsolationBDT( const std::string& type,
                const std::string& name,
                const IInterface* parent );

        virtual StatusCode initialize();

        virtual StatusCode calculateRelatedInfo( const LHCb::Particle*,
                                         const LHCb::Particle*);

        virtual LHCb::RelatedInfoMap* getInfo(void);
  
        virtual std::string infoPath(void);

        virtual ~RelInfoBKsttautauTrackIsolationBDT( ); ///< Destructor

    private:

        std::vector<std::string> m_variables;
        LHCb::RelatedInfoMap m_map;
        std::vector<short int> m_keys; 

        double m_bdt1;
        double m_bdt2;
        double m_bdt3;

        double   m_bdt1_TauP_piM ;
        double   m_bdt2_TauP_piM ;
        double   m_bdt3_TauP_piM ;

        double   m_bdt1_TauP_piP1 ;
        double   m_bdt2_TauP_piP1 ;
        double   m_bdt3_TauP_piP1 ;

        double   m_bdt1_TauP_piP2 ;
        double   m_bdt2_TauP_piP2 ;
        double   m_bdt3_TauP_piP2 ;

        double   m_bdt1_TauM_piP ;
        double   m_bdt2_TauM_piP ;
        double   m_bdt3_TauM_piP ;

        double   m_bdt1_TauM_piM1 ;
        double   m_bdt2_TauM_piM1 ;
        double   m_bdt3_TauM_piM1 ;

        double   m_bdt1_TauM_piM2 ;
        double   m_bdt2_TauM_piM2 ;
        double   m_bdt3_TauM_piM2 ;

        double   m_bdt1_Kst_K;
        double   m_bdt2_Kst_K;
        double   m_bdt3_Kst_K;

        double   m_bdt1_Kst_pi;
        double   m_bdt2_Kst_pi;
        double   m_bdt3_Kst_pi;

        std::string m_weightsName;
        /// TMVA transform
        //Reader
        //TMVA::Reader *m_Reader;
        std::string m_transformName ;
        //variables
        float var_doca ;
        float var_angle ;
        float var_fc ;
        float var_pt ;
        float var_PVdist ;
        float var_SVdist ;
        float var_ipchisqany ;
  IParticleDescendants* m_descend;
        TMVATransform m_tmva ;
        TMVATransform::optmap m_optmap ;
        IParticleDictTool::DICT m_varmap ; 
        IParticleDictTool::DICT m_out ; 


        //save the vertice
        std::string m_PVInputLocation ;
        LHCb::RecVertex::Container * m_vertices ;

        std::vector<const LHCb::Particle*> m_decayParticles;

        /// Save all particles in your decay descriptor in a vector
        void saveDecayParticles( const LHCb::Particle *top);

        /// Check if your track belongs to your decay or not
        bool isTrackInDecay(const LHCb::Track* track);


        ///============================================================================
        /// Track isolation method
        ///============================================================================

        bool calcBDTValue( const LHCb::Particle * part
                , const LHCb::Particles * tracks
                , const LHCb::VertexBase * PV
                , const LHCb::VertexBase * SV
                ) ;
        ///============================================================================
        /// Helper functions 
        ///============================================================================
        double calcIPToAnyPV( const LHCb::Track * track ) ;
        double  enclosedAngle( Gaudi::XYZVector, Gaudi::XYZVector );
        void    getPerpFeet(  Gaudi::XYZPoint track_pos,
                Gaudi::XYZVector track_p,
                Gaudi::XYZPoint mu_pos,
                Gaudi::XYZVector mu_p, 
                Gaudi::XYZPoint& perpFootTrack, 
                Gaudi::XYZPoint& perpFootMu, 
                Gaudi::XYZPoint& vertex, bool& fail); 
        double calcFC(  Gaudi::XYZVector track_mom, 
                Gaudi::XYZVector mu_mom, 
                Gaudi::XYZPoint mu_track_vertex,
                const LHCb::VertexBase* PV);
        double arcosine (Gaudi::XYZVector, Gaudi::XYZVector);
        double calcVertexDist(Gaudi::XYZPoint muTrack, 
                const LHCb::VertexBase* v);
  



};

#endif // CONEVARIABLES_H
