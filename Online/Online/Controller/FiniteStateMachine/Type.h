/*============================================================
**
**      Finite state machine implementation to control 
**      and manipulate process groups
**
**  AUTHORS:
**
**      M.Frank  CERN/LHCb
**
**==========================================================*/
#ifndef ONLINE_FINITESTATEMACHINE_TYPE_H
#define ONLINE_FINITESTATEMACHINE_TYPE_H

// Framework include files
#include "FiniteStateMachine/Rule.h"
#include "FiniteStateMachine/Transition.h"

// C/C++ include files
#include <map>

/* 
 *  FiniteStateMachine namespace declaration
 */
namespace FiniteStateMachine   {

  // Forward declarations
  class State;
  class Transition;

  /**@class Type  Type.h FiniteStateMachine/Type.h
   *
   *    Definition of a FSM protocol type
   *
   *    The FSM type defines a given protocol type.
   *    The protocol type is used by any instantiation of a FSM machine object,
   *    since the type itself defines the possible transitions and states.
   *
   * @author  M.Frank
   * @date    01/03/2013
   * @version 0.1
   */
  class Type   {
  public:
    /// State container definition
    typedef std::map<std::string,State*>       States;
    /// Transition container definition
    typedef std::map<std::string,Transition*>  Transitions;
  protected:
    /// Object name
    std::string m_name;

  protected:
    const State* m_initState;
    States       m_states;
    Transitions  m_transitions;

  public:
    /// Default constructor
    Type(const std::string& name);
    /// Standatrd destructor  
    virtual ~Type();    
    /// Accessor: object name
    const std::string& name()   const        {  return m_name;               }
    /// Accessor: object name as c-string (shortcut for printing)
    const char* c_name()   const             {  return m_name.c_str();       }
    /// Access to the states
    const States& states()   const           {  return m_states;             }
    /// Access to the transitions
    const Transitions& transitions()   const {  return m_transitions;        }
    /// Accessor: Retrieve initial state
    const State* initialState() const        {  return m_initState;          }
    /// Accessor: Set initial state
    Type& setInitialState(const State* state);
    /// Access state by its name. Throws exception if state does not exist
    const State* state(const std::string& nam)  const;
    /// Access transition by its name. Throws exception if transition does not exist
    const Transition* transition(const std::string& nam)  const;
    /// Access transition by its source and target state name. Throws exception if transition does not exist
    const Transition* transition(const std::string& from, const std::string& to)  const;

    /// Add a new rule to a transition
    const Rule*       addRule(Transition* transition, const Type* target_type, const std::string& current_state, const std::string& target_state, Rule::Direction direction=Rule::MASTER2SLAVE);
    /// Add a new predicate to a transition
    const Predicate*  addPredicate(Transition* transition,   const Type* target_type,
				   const std::string& s1,    const std::string& s2="",
				   const std::string& s3="", const std::string& s4="", 
				   const std::string& s5="");

    /// Add a new state to the FSM type
    const State*      addState(const std::string& nam);
    /// Add a new Transition to the FSM type. Intrinsic transition - no command sent to slaves
    Transition*       addTransition(const State* from, const State* to, unsigned int flags=Transition::CHECK);
    /// Add a new Transition to the FSM type
    Transition*       addTransition(const std::string& cmd, const State* from, const State* to, unsigned int flags=Transition::CHECK);
  };   //  End class Type
}      //  End namespace 
#endif //  ONLINE_FINITESTATEMACHINE_TYPE_H

