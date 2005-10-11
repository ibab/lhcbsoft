/*
** QUEUE ROUTINES BY TIM CHARITY - Sep 89
*/
#include "RTL/rtl.h"
#include "RTL/Thread.h"

RTL::Thread::Thread()  {
//  m_status = lib_rtl_create_thread(m_id);
  if ( !lib_rtl_is_success(m_status) )   {
    ::lib_rtl_signal_message(LIB_RTL_OS,"Cannot aquire Thread. Status=%d", m_status);
  }
}

RTL::Thread::~Thread()  {
//  m_status = lib_rtl_unthread(m_id);
  if ( !m_status )  {
    // throw exception ?
  }
}

/// Create now named Thread
int RTL::Thread::create(const char* name, lib_rtl_thread_t& id)  {
  int status = 0;//lib_rtl_create_thread(name, &id);
  if (!lib_rtl_is_success(status))    {
    ::lib_rtl_signal_message(LIB_RTL_OS,"Error in creating  Thread. Status=%d", status);
  }
  return status;
}
