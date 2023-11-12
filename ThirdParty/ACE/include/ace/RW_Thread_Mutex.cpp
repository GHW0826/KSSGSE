/**
 * @file RW_Thread_Mutex.cpp
 *
 * @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */

#include "ace/RW_Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

#if !defined (__ACE_INLINE__)
#include "ace/RW_Thread_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_RW_Thread_Mutex)

ACE_RW_Thread_Mutex::ACE_RW_Thread_Mutex (const ACE_TCHAR *name,
                                          void *arg)
  : ACE_RW_Mutex (USYNC_THREAD, name, arg)
{
// ACE_TRACE ("ACE_RW_Thread_Mutex::ACE_RW_Thread_Mutex");
}

void
ACE_RW_Thread_Mutex::dump () const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_RW_Thread_Mutex::dump");
  ACE_RW_Mutex::dump ();
#endif /* ACE_HAS_DUMP */
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_THREADS */
