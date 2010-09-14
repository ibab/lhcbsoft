// $Id$
// ============================================================================
#ifndef LOKI_PHYSSOURCES_H 
#define LOKI_PHYSSOURCES_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
// ============================================================================
// PhysEvent
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// DaVinciInterfaces
// ============================================================================
#include "Kernel/IDVAlgorithm.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
#include "LoKi/PhysTypes.h"
// ============================================================================
class DVAlgorithm ;
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Particles 
  {
    // ========================================================================
    /** @class SourceTES 
     * 
     *  @see LoKi::Cuts::SOURCE
     *  @see LoKi::Cuts::SOURCETES
     *  simple "source for the particles in TES"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceTES 
      : public LoKi::BasicFunctors<const LHCb::Particle*>::Source 
    { 
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::Particle*>::Source _Source;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const std::string&           path                                 , 
        IDataProviderSvc*            svc  = 0                             , 
        const LoKi::PhysTypes::Cuts& cuts = 
        LoKi::BasicFunctors<const LHCb::Particle*>::BooleanConstant(true) ) ; 
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const std::vector<std::string>& path                                , 
        IDataProviderSvc*               svc  = 0                            , 
        const LoKi::PhysTypes::Cuts&    cuts = 
        LoKi::BasicFunctors<const LHCb::Particle*>::BooleanConstant(true) ) ; 
      /// constructor from the service, TES location and cuts 
      SourceTES
      ( const std::string&           path     , 
        const LoKi::PhysTypes::Cuts& cuts     ,  
        IDataProviderSvc*            svc  = 0 ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES
      ( const std::vector<std::string>& path     , 
        const LoKi::PhysTypes::Cuts&    cuts     ,  
        IDataProviderSvc*               svc  = 0 ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::Cuts& cuts        ,  
        const std::string&           path        ,
        IDataProviderSvc*            svc  = 0    ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::Cuts&    cuts        ,  
        const std::vector<std::string>& path        ,
        IDataProviderSvc*               svc  = 0    ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::Cuts& cuts  ,  
        IDataProviderSvc*            svc   ,  
        const std::string&           path  ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::Cuts&    cuts  ,  
        IDataProviderSvc*               svc   ,  
        const std::vector<std::string>& path  ) ;
      /// copy constructor
      SourceTES ( const SourceTES& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~SourceTES() ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  SourceTES* clone() const { return new SourceTES(*this) ; }
      /// MANDATORY: the only essential method:
      virtual result_type operator() () const ;
      /// OPTIONAL: the nice printout
      virtual std::ostream& fillStream ( std::ostream& o ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      SourceTES() ; // the default constructor is disabled 
      // ======================================================================
    public:
      // ======================================================================
      /// get the path 
      const std::vector<std::string>& paths() const { return m_path ; }
      /// get the service
      const LoKi::Interface<IDataProviderSvc>& dataSvc() const 
      { return m_dataSvc ; }      
      // ======================================================================
    public:
      // ======================================================================
      /// set the  paths
      void setPaths  ( const std::vector<std::string>& value ) { m_path = value ; }
      /// set the  paths
      void addToPath ( const std::string& value ) { m_path.push_back( value ) ; }
      /// set the  service 
      void setDataSvc ( const                 IDataProviderSvc*  value ) 
      { m_dataSvc = value ; }
      /// set the  service 
      void setDataSvc ( const LoKi::Interface<IDataProviderSvc>& value ) 
      { m_dataSvc = value ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// get the particles from the certain  TES location 
      std::size_t get ( const std::string&           location , 
                        LHCb::Particle::ConstVector& output   ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// TES locations of Particles 
      std::vector<std::string>  m_path ; // TES locations of Particles 
      /// data provider service 
      mutable LoKi::Interface<IDataProviderSvc>  m_dataSvc ;
      /// 'on-flight' filter
      LoKi::PhysTypes::Cut m_cut ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class SourceDesktop
     * 
     *  @see LoKi::Cuts::SOURCEDESKTOP
     *  @see LoKi::Cuts::SOURCEDV
     *  simple "source for the particles from desktop"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceDesktop
      : public LoKi::BasicFunctors<const LHCb::Particle*>::Source 
    { 
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::Particle*>::Source _Source;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the desktop and cuts 
      SourceDesktop 
      ( const IDVAlgorithm*          desktop = 0                          , 
        const LoKi::PhysTypes::Cuts& cuts    =  
        LoKi::BasicFunctors<const LHCb::Particle*>::BooleanConstant(true) ) ; 
      /// constructor from the desktop and cuts 
      SourceDesktop 
      ( const LoKi::PhysTypes::Cuts& cuts        , 
        const IDVAlgorithm*          desktop = 0 ) ;
      /// copy constructor
      SourceDesktop ( const SourceDesktop& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~SourceDesktop () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  SourceDesktop* clone() const { return new SourceDesktop(*this) ; }
      /// MANDATORY: the only essential method:
      virtual result_type operator() () const ;
      /// OPTIONAL: the nice printout
      virtual std::ostream& fillStream ( std::ostream& o ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the desktop
      const LoKi::Interface<IDVAlgorithm>& desktop() const 
      { return m_desktop ; }      
      // ======================================================================
    public:
      // ======================================================================
      /// set the  desktop
      void setDesktop ( const                 IDVAlgorithm*  value ) 
      { m_desktop = value ; }
      /// set the  desktop 
      void setDesktop ( const LoKi::Interface<IDVAlgorithm>& value ) 
      { m_desktop = value ; }
      // ======================================================================
    private:
      // ======================================================================
      /// data provder service 
      mutable LoKi::Interface<IDVAlgorithm>  m_desktop ;
      /// 'on-flight' filter
      LoKi::PhysTypes::Cut m_cut ;
      // ======================================================================
    } ;    
    // ========================================================================
  } //                                         end of namespace LoKi::Particles  
  // ==========================================================================
  namespace Vertices 
  {
    // ========================================================================
    /** @class SourceTES 
     * 
     *  @see LoKi::Cuts::VSOURCE
     *  simple "source for the particles in TES"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceTES 
      : public LoKi::BasicFunctors<const LHCb::VertexBase*>::Source 
    { 
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::VertexBase*>::Source _Source;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const std::string&            path                                 , 
        IDataProviderSvc*             svc  = 0                             , 
        const LoKi::PhysTypes::VCuts& cuts = 
        LoKi::BasicFunctors<const LHCb::VertexBase*>::BooleanConstant(true) ) ; 
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const std::vector<std::string>&  path                                , 
        IDataProviderSvc*                svc  = 0                            , 
        const LoKi::PhysTypes::VCuts&    cuts = 
        LoKi::BasicFunctors<const LHCb::VertexBase*>::BooleanConstant(true) ) ; 
      /// constructor from the service, TES location and cuts 
      SourceTES
      ( const std::string&            path     , 
        const LoKi::PhysTypes::VCuts& cuts     ,  
        IDataProviderSvc*             svc  = 0 ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES
      ( const std::vector<std::string>&  path     , 
        const LoKi::PhysTypes::VCuts&    cuts     ,  
        IDataProviderSvc*                svc  = 0 ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::VCuts& cuts        ,  
        const std::string&            path        ,
        IDataProviderSvc*             svc  = 0    ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::VCuts&   cuts        ,  
        const std::vector<std::string>& path        ,
        IDataProviderSvc*               svc  = 0    ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::VCuts& cuts  ,  
        IDataProviderSvc*             svc   ,  
        const std::string&            path  ) ;
      /// constructor from the service, TES location and cuts 
      SourceTES 
      ( const LoKi::PhysTypes::VCuts&    cuts  ,  
        IDataProviderSvc*                svc   ,  
        const std::vector<std::string>&  path  ) ;
      /// copy constructor
      SourceTES ( const SourceTES& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~SourceTES() ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  SourceTES* clone() const { return new SourceTES(*this) ; }
      /// MANDATORY: the only essential method:
      virtual result_type operator() () const ;
      /// OPTIONAL: the nice printout
      virtual std::ostream& fillStream ( std::ostream& o ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      SourceTES() ;                      // the default constructor is disabled 
      // ======================================================================
    public:
      // ======================================================================
      /// get the path 
      const std::vector<std::string>& paths() const { return m_path ; }
      /// get the service
      const LoKi::Interface<IDataProviderSvc>& dataSvc() const 
      { return m_dataSvc ; }      
      // ======================================================================
    public:
      // ======================================================================
      /// set the  paths
      void setPaths  ( const std::vector<std::string>& value ) { m_path = value ; }
      /// set the  paths
      void addToPath ( const std::string& value ) { m_path.push_back( value ) ; }
      /// set the  service 
      void setDataSvc ( const                 IDataProviderSvc*  value ) 
      { m_dataSvc = value ; }
      /// set the  service 
      void setDataSvc ( const LoKi::Interface<IDataProviderSvc>& value ) 
      { m_dataSvc = value ; }
      // ======================================================================
    private:
      // ======================================================================
      /// get the particles from the certain  TES location 
      std::size_t get
      ( const std::string&             location , 
        LHCb::VertexBase::ConstVector& output   ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// TES locations of vertices  
      std::vector<std::string>  m_path ; // TES locations of vertices 
      /// data provder service 
      mutable LoKi::Interface<IDataProviderSvc>  m_dataSvc ;
      /// 'on-flight' filter
      LoKi::PhysTypes::VCut m_cut ;
      // ======================================================================
    } ;    
    // ========================================================================
    /** @class SourceDesktop
     * 
     *  @see LoKi::Cuts::VSOURCEDESKTOP
     *  simple "source for the vertices from desktop"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceDesktop
      : public LoKi::BasicFunctors<const LHCb::VertexBase*>::Source 
    { 
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::VertexBase*>::Source _Source;
    public:
      // ======================================================================
      // ======================================================================
      /// constructor from the desktop and cuts 
      SourceDesktop 
      ( const IDVAlgorithm*           desktop = 0                          , 
        const LoKi::PhysTypes::VCuts& cuts    =  
        LoKi::BasicFunctors<const LHCb::VertexBase*>::BooleanConstant(true) ) ; 
      /// constructor from the desktop and cuts 
      SourceDesktop 
      ( const LoKi::PhysTypes::VCuts& cuts        , 
        const IDVAlgorithm*           desktop = 0 ) ;
      /// copy constructor
      SourceDesktop ( const SourceDesktop& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~SourceDesktop () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  SourceDesktop* clone() const { return new SourceDesktop(*this) ; }
      /// MANDATORY: the only essential method:
      virtual result_type operator() () const ;
      /// OPTIONAL: the nice printout
      virtual std::ostream& fillStream ( std::ostream& o ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the desktop
      const LoKi::Interface<IDVAlgorithm>& desktop() const 
      { return m_desktop ; }      
      // ======================================================================
    public:
      // ======================================================================
      /// set the  desktop
      void setDesktop ( const                 IDVAlgorithm*  value ) 
      { m_desktop = value ; }
      /// set the  desktop 
      void setDesktop ( const LoKi::Interface<IDVAlgorithm>& value ) 
      { m_desktop = value ; }
      // ======================================================================
    private:
      // ======================================================================
      /// data provder service 
      mutable LoKi::Interface<IDVAlgorithm>  m_desktop ;
      /// 'on-flight' filter
      LoKi::PhysTypes::VCut m_cut ;
      // ======================================================================
    } ;    
    // ========================================================================
  } //                                         end of namespace LoKi::Vertices 
  // ==========================================================================
  namespace Cuts 
  {
    // ========================================================================
    /** @typedef SOURCE 
     *  simple "source"-functor to get the particle form TES
     * 
     *  @code
     *
     *  const SOURCE source = SOURCE ( "SomeLocationInTES" , ALL ) ;
     *
     *  // get the particles:
     *  const LHCb::Particle::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Particles::SourceTES                                 SOURCE ;
    // ========================================================================
    /** @typedef SOURCETES 
     *  simple "source"-functor to get the particle form TES
     * 
     *  @code
     *
     *  const SOURCETES source = SOURCETES ( "SomeLocationInTES" , ALL ) ;
     *
     *  // get the particles:
     *  const LHCb::Particle::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Particles::SourceTES                              SOURCETES ;
    // ========================================================================
    /** @typedef SOURCEDESKTOP 
     *  Somiple "source"-functor which gets the particles from 
     *  desktop 
     *
     *  @code 
     *    
     *  const SOURCEDESKTOP source ( "pi+" == ABSID ) ;
     *
     *  // get the pions from desktop:
     *  const LHCb::Particle::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @see IPhysDekstop 
     *  
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Particles::SourceDesktop                      SOURCEDESKTOP ;
    // ========================================================================
    /** @typedef SOURCEDV
     *  Somiple "source"-functor which gets the particles from 
     *  desktop 
     *
     *  @code 
     *    
     *  const SOURCEDV source ( "pi+" == ABSID ) ;
     *
     *  // get the pions from desktop:
     *  const LHCb::Particle::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @see IPhysDekstop 
     *  
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Particles::SourceDesktop                          SOURCEDV ;
    // ========================================================================
    /** @typedef VSOURCE 
     *  simple "source"-functor to get the vertices from TES
     * 
     *  @code
     *
     *  const VSOURCE source = VSOURCE ( "SomeLocationInTES" , PRIMARY ) ;
     *
     *  // get the vertices
     *  const LHCb::VertexBase::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Vertices::SourceTES                                 VSOURCE ;
    // ========================================================================
    /** @typedef VSOURCETES 
     *  simple "source"-functor to get the vertices from TES
     * 
     *  @code
     *
     *  const VSOURCETES source ( "SomeLocationInTES" , PRIMARY ) ;
     *
     *  // get the primary vertices:
     *  const LHCb::VertexBase::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Vertices::SourceTES                              VSOURCETES ;
    // ========================================================================
    /** @typedef VSOURCEDESKTOP 
     *  Somiple "source"-functor which gets the vertices from 
     *  desktop 
     *
     *  @code 
     *    
     *  const VSOURCEDESKTOP source ( PRIMARY  ) ;
     *
     *  // get the primary vertices from desktop:
     *  const LHCb::VertexBase::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @see IPhysDekstop 
     *  
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Vertices::SourceDesktop                      VSOURCEDESKTOP ;
    // ========================================================================
    /** @typedef VSOURCEDV
     *  Somiple "source"-functor which gets the vertices from 
     *  desktop 
     *
     *  @code 
     *    
     *  const VSOURCEDV source ( PRIMARY ) ;
     *
     *  // get the primiaries from desktop:
     *  const LHCb::VertexBase::ConstVector result = source() ;
     *
     *  @endcode 
     *
     *  @see IPhysDekstop 
     *  
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-29
     */
    typedef LoKi::Vertices::SourceDesktop                          VSOURCEDV ;
    // ========================================================================
  } // end of namespace LoKi::Cuts 
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_PHYSSOURCES_H
// ============================================================================
