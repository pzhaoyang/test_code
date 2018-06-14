#ifndef _ALD_LC_DEBUG_H_
#define _ALD_LC_DEBUG_H_

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/timeb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ANTS_LOG "/tmp/ants.log"
#define ANTS_CFG "/var/ants.cfg"

#define ERRORSTR  "XXXXXX ERROR XXXXXX"
#define DEBUGSTR  "###### DEBUG ######"
#define INFORSTR  "====== INFOR ======"

#define ANTS_PRINT(fmt, ...) do{\
    if (0 != access(ANTS_CFG, 0)){\
          fprintf(stdout, fmt, ##__VA_ARGS__);\
    }else{\
      FILE *fp;\
      if(NULL == (fp = fopen(ANTS_LOG, "a+"))){\
          fprintf(stdout, fmt, ##__VA_ARGS__);\
      }else{\
          fprintf(fp, fmt, ##__VA_ARGS__);\
          fclose(fp);\
      }\
    }\
 }while(0)

#define _ANTS_PRINT(lvl, fmt, ...) do{\
                struct timeb tp;\
                ftime(&tp);\
                ANTS_PRINT("[ %u.%03u ] %s %s():%05d "fmt"\n", (unsigned int)tp.time, tp.millitm, lvl, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
            }while(0)


#define ANTS_ERROR(fmt, ...)  _ANTS_PRINT(ERRORSTR, fmt, ##__VA_ARGS__)
#define ANTS_DEBUG(fmt, ...)  _ANTS_PRINT(DEBUGSTR, fmt, ##__VA_ARGS__)
#define ANTS_INFO(fmt, ...)   _ANTS_PRINT(INFORSTR, fmt, ##__VA_ARGS__)

#define ANTS_MEMDUMP(data, len, title) do{\
    int index; \
    unsigned char *pdata; \
    pdata = data; \
    if(title != NULL){ \
        ANTS_PRINT("%s:\n", title); \
    } \
    for(index = 0; index < len; index ++){ \
        if(index !=0 && index % 16 == 0){ \
            ANTS_PRINT("\n"); \
        } \
        ANTS_PRINT("%02x ", pdata[index]); \
    } \
    ANTS_PRINT("\n"); \
  }while(0);

#ifdef __cplusplus
}
#endif
#endif
