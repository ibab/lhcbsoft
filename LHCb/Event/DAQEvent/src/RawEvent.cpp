// $Id: RawEvent.cpp,v 1.6 2005-10-14 12:48:50 cattanem Exp $
#include "Event/RawEvent.h"

namespace {
  inline size_t memLength(size_t len)  {
    size_t mem_len = len+sizeof(LHCb::RawBank)-sizeof(LHCb::RawBank::data_type_t);
    if ( mem_len%sizeof(LHCb::RawBank::data_type_t) )  {    // Need padding
      mem_len = (mem_len/sizeof(LHCb::RawBank::data_type_t) + 1)*sizeof(LHCb::RawBank::data_type_t);
    }
    return mem_len;
  }
  inline LHCb::RawBank* allocateBank(size_t len)  {
    size_t mem_len = memLength(len);
    char* mem = (char*)::operator new(mem_len);
    if ( mem_len != len )  {
      ::memset(mem+len,0,mem_len-len);
    }
    return (LHCb::RawBank*)mem;
  }
}

// Default Constructor
LHCb::RawEvent::RawEvent() 
: m_mapped(false)
{
}

// Default Destructor
LHCb::RawEvent::~RawEvent() {
  for(std::vector<Bank>::iterator i=m_banks.begin(); i != m_banks.end(); ++i)  {
    Bank& b = *i;
    if ( b.ownsMemory() && b.buffer() )  {
      ::operator delete(b.buffer());
    }
  }
  m_banks.clear();
}

const std::vector<LHCb::RawBank*>& LHCb::RawEvent::mapBanks(RawBank::BankType bankType)  {
  for(std::vector<Bank>::iterator i=m_banks.begin(); i != m_banks.end(); ++i)  {
    LHCb::RawBank* bank = (LHCb::RawBank*)(*i).buffer();
    m_eventMap[bank->type()].push_back(bank);
  }
  m_mapped = true;
  return m_eventMap[bankType];
}

// For offline use only: copy data into a set of banks, adding bank header internally.
void LHCb::RawEvent::addBank( int srcID, 
                              LHCb::RawBank::BankType typ, 
                              int vsn, 
                              const std::vector<int>& data)  {
  adoptBank( createBank( srcID, typ, vsn, data.size()*sizeof(int), &(*data.begin()) ), true );
}

LHCb::RawBank* LHCb::RawEvent::createBank( int srcID, 
                                           LHCb::RawBank::BankType typ,
                                           int vsn,
                                           size_t len,
                                           const void* data )  {
  LHCb::RawBank* bank = allocateBank(len);
  bank->setMagic();
  bank->setType(typ);
  bank->setVersion(vsn);
  bank->setSourceID(srcID);
  bank->setSize(len);
  if ( data )  {
    ::memcpy(bank->data(), data, len);
  }
  return bank;
}

/// For offline use only: copy data into a bank, adding bank header internally.
void LHCb::RawEvent::addBank(RawBank* data)     {
  size_t len = data->size() + sizeof(LHCb::RawBank) - sizeof(LHCb::RawBank::data_type_t);
  LHCb::RawBank* bank = allocateBank(len);
  ::memcpy(bank, data, len);
  adoptBank(bank, true);
}

/// Take ownership of a bank, including the header
void LHCb::RawEvent::adoptBank(LHCb::RawBank* bank, bool adopt_memory)     {
  size_t len = memLength(bank->size());
  if ( !m_mapped ) mapBanks(bank->type());
  m_eventMap[bank->type()].push_back(bank);
  m_banks.push_back(Bank(len/sizeof(int), adopt_memory, (int*)bank));
}
