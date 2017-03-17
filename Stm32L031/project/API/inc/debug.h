#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG_MODEL_ON
#ifdef DEBUG_MODEL_ON
#define API_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#endif

#endif //__DEBUG_H__