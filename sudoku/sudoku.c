#include "stdio.h"
#include "string.h"

/*
*         =========== Sudoku Frame ===================       
*  
*           1   2   3   4   5   6   7   8   9
*        A  
*        B
*        C
*        D
*        E
*        F
*        G
*        H
*        I

*/

#define SIZE 9

typedef struct sudoku {
    char template[SIZE][SIZE];
    char value[SIZE][SIZE];
}sudoku_t;

static sudoku_t issue;

void hexdump(char *p, int size){
    char *ptr = p;
    int i;
    
    if(ptr == NULL|| size < 0) return;
    
    for(i=0; i<size; i++, ptr++){
        if(i%16 == 0) printf("\n");
        printf("0x%02x ", *ptr);
    }
}
void print_sudoku(int p[9][9]){
    int c,r;
    if( p == NULL){
        printf("\n******\nThe sudoku is invalid!\n******\n");
        return;
    }
    
    
    for(c=0; c<9; c++){
        for(r=0; r<9; r++){
            printf("%d ", p[c][r]+'0');
        }
        printf("\n");
    }
}

int read_template(){
    FILE* fp;
    char line[SIZE+1];
    int i;
    
    fp = fopen("template", "r");
    if(fp == NULL){
        printf("Cannot open the template file!.\n");
        return -1;
    }

    for(i=0; NULL != fgets((void *)line, sizeof(line), fp); i++){
        printf("%s", line);
        memcpy((void *)issue.template[i], (void *)line, strlen(line));
        printf("hexdump (line:%d) (%d):\n", strlen(line),sizeof(issue.template[i]));
        hexdump(issue.template[i], sizeof(issue.template[i]));
    }

    fclose(fp);
    return 0;
}


int main(int argc, char* argv[]){
   int s[9][9] = {0};
   printf("Start SUDOKU...\n");
   
   read_template();

   //print_sudoku(issue.template);
   return 0;  
}