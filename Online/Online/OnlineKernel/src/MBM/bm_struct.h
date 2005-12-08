#ifndef _MBM_MBM_STRUCTS_H
#define _MBM_MBM_STRUCTS_H

#define MBM_IMPLEMENTATION

#include "RTL/bits.h"
#include "RTL/que.h"
#include "RTL/rtl.h"

typedef Bits::BitMask<4> TriggerMask;
typedef Bits::BitMask<4> UserMask;
typedef Bits::BitMask<4> VetoMask;

namespace MBM  {
  enum BufferStates   {
    S_nil    = -1,
    S_active = 11,
    S_pause   = 10,
    S_wspace =  9,
    S_wevent =  8,
    S_weslot =  7,
    S_wspace_ast_ready   =12,
    S_wspace_ast_queued  = 6,
    S_wevent_ast_queued  = 5,
    S_weslot_ast_queued  = 4,
    S_wspace_ast_handled = 3,
    S_wevent_ast_handled = 2,
    S_weslot_ast_handled = 1
  };

  enum BIDs {
    BID_USER  = 0xfeed0001,
    BID_EVENT = 0xfeed0002
  };

  enum BufferReasons {
    BM_K_INT_EVENT  = 1,
    BM_K_INT_SPACE  = 2,
    BM_K_INT_ESLOT  = 4
  };
};
#define TOPTYP    1
#define NAME_LENGTH 11
#define Bits_p_kByte  2
#define Shift_p_Bit  (10-Bits_p_kByte+1)    // = 9
#define Bytes_p_Bit  ((1<<Shift_p_Bit)-1)   // = 511

struct REQ  {                //requirements structures xx bytes each   
  int           ev_type;     // event type         
  TriggerMask   tr_mask;     // event Mask         
  UserMask      vt_mask;     // veto mask         
  int           masktype;    // requirement type for trigger mask   
  int           user_type;   // Requirement Type (VIP/normal User)   
  int           freqmode;    // requirement mode       
  float         freq;        // Frequency limit       
  int           ev_seen;     // Events seen for this requirement   
};

template <class T> struct QUEUE {
  T* next;
  T* prev;
  QUEUE() : next(0), prev(0) {}
};

struct USER : public qentry_t  {
  qentry wsnext;
  qentry_t wenext;
  qentry_t wesnext;
  int  block_id;  
  int  busy;                     // slot busy flag       
  int  uid;                      // BM user id       
  int  c_state;                  // consumer state (Active,Pause)   
  int  p_state;                  // producer state (Active,Wspace)  
  int  partid;                   // user partition ID       
  char name[16];                 // user name         
  int pid;                       // process id         
  RTL_ast_t c_astadd;            // consumer signal to be send     
  RTL_ast_t p_astadd;            // producer signal to be send     
  int ws_size;                   // size of waiting space     
  int ws_ptr;                    // pointer of waiting space     
  int we_ptr;                    // pointer of waiting exent     
  int we_size;                   // size of waiting exent     
  int we_evtype;                 // evtype of waiting exent     
  TriggerMask we_trmask;         // trmask of waiting exent     
  int **ws_ptr_add;              // pointer of waiting space     
  int **we_ptr_add;              // pointer of waiting exent     
  int *we_size_add;              // size of waiting exent     
  int *we_evtype_add;            // evtype of waiting exent     
  TriggerMask *we_trmask_add;    // trmask of waiting exent     
  int space_add;                 // address of allocated space    
  int space_size;                // size in bytes       
  int held_eid;                  // held event index       
  int ev_produced;               // events produced counter     
  int ev_actual;                 // events matching req        
  int ev_seen;                   // # of events seen         
  int ev_freed;                  // # of events freed         
  int n_req;                     // number of requierements     
  int c_partid;                
  void* c_astpar;              
  void* p_astpar;              
  int reason;                  
  int get_ev_calls;            
  int get_sp_calls;            
  int get_wakeups;             
  int get_asts_run;            
  float  utime;
  float stime;
  int spare1;
  REQ req[8];                    // 8 requirement maximum     
  char wes_flag[32];
  char wev_flag[32];
  char wsp_flag[32];
  inline bool isValid()   const  {
    if ( block_id == int(MBM::BID_USER) )   {
      return true;
    }
    lib_rtl_signal_message(0, "mbmlib: bad USER pointer:%p",this);
    return false;
  }
};

struct EVENT : public qentry_t {
  int   block_id;         // Block identifier
  int   busy;             // event busy flag
  int   eid;              // event ID
  UserMask umask0;        // Mask of privilidged consumers
  UserMask umask1;        // Mask of non-privilidged consumers
  UserMask umask2;        // Mask of privilidged on-only consumers
  UserMask held_mask;     // Mask of users holding the event
  int partid;             // Partition ID of the event
  int ev_type;            // Event type
  TriggerMask tr_mask;    // Trigger Mask
  int ev_add;             // Event pointer
  int ev_size;            // Event size
  int count;
  inline bool isValid()   const  {
    if ( block_id == int(MBM::BID_EVENT) )   {
      return true;
    }
    lib_rtl_signal_message(0, "mbmlib: bad EVENT pointer:%p",this);
    return false;
  }
};

struct CONTROL  {
  qentry_t u_head;        // active consumers
  qentry_t wev_head;      // consumers waiting events
  qentry_t wsp_head;      // producers waiting space
  qentry_t wes_head;      // producers waiting event slots
  qentry_t e_head;        // general event queue
  char *buff_ptr;         // Event Buffer pointer
  int buff_size;          // Event Buffer size
  USER *user;             // user reserved space
  EVENT *event;           // event reserved space
  int p_umax;             // maximum users
  int p_emax;             // maximum events
  int p_base;             // Memory base address
  int spy_base;           // spy memory base address
  int tot_produced;       // events produced counter
  int tot_actual;         // events matching req
  int tot_seen;           // events seen
  int i_events;           // instantaneous event number
  int i_space;            // instantaneous free  space
  int i_users;            // instantaneous sctive users
  int last_bit;           // last bit on the bipmap
  int bm_size;            // size of bit map in bytes
  int last_alloc;         // Byte Offset of last allocation
  int spare1;
};

struct BUFFERS  {
  struct BUFF {
    char name[32];
    int  used;
  };
  int   p_bmax;           // Maximum number of buffers
  int   nbuffer;          // Current number of buffers
  BUFF  buffers[1];       // Buffer descriptors
};

struct BMDESCRIPT : public qentry_t  {
  CONTROL*         ctrl;
  USER*            user;
  EVENT*           event;
  char*            bitmap;
  unsigned int     bitmap_size;
  char*            buffer_add;
  unsigned int     buffer_size;
  int              owner;
  void*            lockid;
  char             bm_name[32];
  char             mutexName[32];
  RTL_ast_t        free_event;
  void*            free_event_param;
  RTL_ast_t        alloc_event;
  void*            alloc_event_param;
  lib_rtl_gbl_t    ctrl_add;
  lib_rtl_gbl_t    event_add;
  lib_rtl_gbl_t    user_add;
  lib_rtl_gbl_t    bitm_add;
  lib_rtl_gbl_t    buff_add;
  lib_rtl_event_t  WES_event_flag;
  lib_rtl_event_t  WEV_event_flag;
  lib_rtl_event_t  WSP_event_flag;
  lib_rtl_event_t  WEVA_event_flag;
  lib_rtl_event_t  WSPA_event_flag;
  lib_rtl_thread_t pThread;
  lib_rtl_thread_t cThread;
  inline USER* _user()  {
    return owner==-1 ? 0 : user + owner;
  }
  inline CONTROL* _control()  {
    return owner==-1 ? 0 : ctrl;
  }
};

#include "bm_internals.h"
#endif // _MBM_MBM_STRUCTS_H
