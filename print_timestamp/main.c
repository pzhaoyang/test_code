#include <stdio.h>
#include <unistd.h>

#include "timestamp.h"


void main(){
    while(1){
        ANTS_DEBUG("sleep %s 1s", "aa");
        sleep(1);
    }
}
