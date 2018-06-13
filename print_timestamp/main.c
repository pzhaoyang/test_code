#include <stdio.h>
#include <unistd.h>

#include "timestamp.h"


void main(){
    while(1){
	ANTS_MEMDUMP("hello222333444555666", 21, "show");
        ANTS_DEBUG("sleep 1s");
        ANTS_ERROR("sleep 1s");
        ANTS_INFO("sleep 1s");
        sleep(1);
    }
}
