#ifndef _TIMESTAMP_H_

#include <time.h>
#include <sys/timeb.h>

//printf("%u.%3u\n", (unsigned int)tp.time, (unsigned int)tp.millitm);

#define TIMEFORMAT  "%04d%02d%02d%02d%02d%02d" //year month day hour min sec


#define GMT_YEAR(gmt) (gmt->tm_year)
#define GMT_MON(gmt)  (gmt->tm_mon)
#define GMT_DAY(gmt)  (gmt->tm_mday)


#define DEBUG(format, ...) do{\
	struct timeb tp;\
	struct tm *gmt;\
	ftime(&tp);\
	gmt = gmtime(&tp.time);\
	fprintf(stdout, "[ %04d%02d%02d.%3u ]"format, gmt->tm_year + 1900, gmt->tm_mon,gmt->tm_mday, (unsigned int)tp.millitm, ##__VA_ARGS__);\
}while(0)

#endif