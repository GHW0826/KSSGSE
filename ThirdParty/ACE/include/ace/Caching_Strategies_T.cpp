#ifndef ACE_CACHING_STRATEGIES_T_CPP
#define ACE_CACHING_STRATEGIES_T_CPP

#include "ace/Caching_Strategies_T.h"
#include "ace/Log_Category.h"

#if !defined (__ACE_INLINE__)
#include "ace/Caching_Strategies_T.inl"
#endif /* __ACE_INLINE__ */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<class ATTRIBUTES, class CACHING_UTILITY>
ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>::~ACE_Caching_Strategy ()
{
}

//////////////////////////////////////////////////////////////////////////////////

template<class ATTRIBUTES, class CACHING_UTILITY>
ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>::ACE_LRU_Caching_Strategy ()
  : timer_ (0),
    purge_percent_ (10)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class ATTRIBUTES, class CACHING_UTILITY>
ACE_LFU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>::ACE_LFU_Caching_Strategy ()
  : purge_percent_ (10)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class ATTRIBUTES, class CACHING_UTILITY>
ACE_FIFO_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>::ACE_FIFO_Caching_Strategy ()
  : order_ (0),
    purge_percent_ (10)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////

ACE_ALLOC_HOOK_DEFINE_Tccc(ACE_Caching_Strategy_Adapter)
ACE_ALLOC_HOOK_DEFINE_Tcc(ACE_LRU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE_Tcc(ACE_LFU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE_Tcc(ACE_FIFO_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE_Tcc(ACE_Null_Caching_Strategy)

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_CACHING_STRATEGIES_T_CPP */
