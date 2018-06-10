#include <stdio.h>
#include <unistd.h>

#include "timestamp.h"


void main(){
    while(1){
        DEBUG("sleep 1s\n");
        sleep(1);
    }
}
