#include "ace/Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Addr.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Category.h"
#include "ace/os_include/sys/os_socket.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Note: this object requires static construction and destruction.
/* static */
const ACE_Addr ACE_Addr::sap_any (AF_ANY, -1);

ACE_ALLOC_HOOK_DEFINE(ACE_Addr)


// Initializes instance variables.  Note that 0 is an unspecified
// protocol family type...

ACE_Addr::ACE_Addr (int type, int size) :
  addr_type_ (type),
  addr_size_ (size)
{
}

void *
ACE_Addr::get_addr () const
{
  return 0;
}

void
ACE_Addr::set_addr (const void *, int)
{
}

// Initializes instance variables.

void
ACE_Addr::base_set (int type, int size)
{
  this->addr_type_ = type;
  this->addr_size_ = size;
}

void
ACE_Addr::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Addr::dump");

  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("addr_type_ = %d"), this->addr_type_));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\naddr_size_ = %d"), this->addr_size_));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_END_VERSIONED_NAMESPACE_DECL
