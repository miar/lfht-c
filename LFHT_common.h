#ifndef _LFHT_COMMON_H
#define _LFHT_COMMON_H

#define LFHT_BoolCAS(PTR, OLD, NEW)       __sync_bool_compare_and_swap((PTR), (OLD), (NEW))
#endif /* _LFHT_COMMON_H */
