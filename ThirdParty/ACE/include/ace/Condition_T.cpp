#ifndef ACE_CONDITION_T_CPP
#define ACE_CONDITION_T_CPP

#include "ace/Condition_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

#include "ace/Log_Category.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

#if !defined (__ACE_INLINE__)
#include "ace/Condition_T.inl"
#include "ace/Time_Value.h"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE_Tc(ACE_Condition)
ACE_ALLOC_HOOK_DEFINE_Tc(ACE_Thread_Condition)

template <class MUTEX> void
ACE_Condition<MUTEX>::dump () const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Condition<MUTEX>::dump");

  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

template <class MUTEX>
ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition (MUTEX &m,
                                                   const ACE_TCHAR *name,
                                                   void *arg)
  : ACE_Condition<MUTEX> (m, USYNC_THREAD, name, arg)
{
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition");
}

template <class MUTEX> void
ACE_Thread_Condition<MUTEX>::dump () const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::dump");

  ACE_Condition<MUTEX>::dump ();
#endif /* ACE_HAS_DUMP */
}

template <class MUTEX>
ACE_Condition<MUTEX>::ACE_Condition (MUTEX &m,
                                     int type,
                                     const ACE_TCHAR *name,
                                     void *arg)
  :
    mutex_ (m)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::ACE_Condition");

  if (ACE_OS::cond_init (&this->cond_,
                         (short) type,
                         name,
                         arg) != 0)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_Condition::ACE_Condition")));
}

template <class MUTEX>
ACE_Condition<MUTEX>::ACE_Condition (MUTEX &m,
                                     const ACE_Condition_Attributes &attributes,
                                     const ACE_TCHAR *name,
                                     void *arg)
  : mutex_ (m)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::ACE_Condition<MUTEX>");
  if (ACE_OS::cond_init (&this->cond_,
                         const_cast<ACE_condattr_t &> (attributes.attributes ()),
                         name, arg) != 0)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_Condition::ACE_Condition")));
}

template <class MUTEX>
ACE_Condition<MUTEX>::~ACE_Condition ()
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::~ACE_Condition");

  if (this->remove () == -1)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_Condition::~ACE_Condition")));
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait ()
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  return ACE_OS::cond_wait (&this->cond_,
                            &this->mutex_.lock ());
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (MUTEX &mutex,
                            const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  if (abstime == 0)
    {
      return ACE_OS::cond_wait (&this->cond_,
                                &mutex.lock ());
    }
  else
    {
      ACE_Time_Value tv = *abstime;
      return ACE_OS::cond_timedwait (&this->cond_,
                                     &mutex.lock (),
                                     &tv);
    }
}

/// Peform an "alertable" timed wait.  If @a abstime == 0
/// then we do a regular cond_wait(), else we do a timed wait for up to
/// @a abstime
template <class MUTEX> int
ACE_Condition<MUTEX>::wait (const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  return this->wait (this->mutex_, abstime);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_THREADS */

#endif /* ACE_CONDITION_T_CPP */
