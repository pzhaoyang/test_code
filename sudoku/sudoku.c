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
        switch(*ptr){
            case 0x0a:
                printf("0x(0A) ");
                break;
            case 0x0d:
                printf("0x0D ");
                break;
            default:
                printf("0x%02x ", *ptr);
        }
    }
    printf("\n");
}
void print_sudoku(char p[9][9]){
    int c,r;
    if( p == NULL){
        printf("\n******\nThe sudoku is invalid!\n******\n");
        return;
    }

    printf("\nThe Result:\n");
    for(c=0; c<9; c++){
        for(r=0; r<9; r++){
            printf("%c ", p[c][r]);
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
    
    printf("\nTemplate:\n{\n");
    for(i=0; NULL != fgets((void *)line, sizeof(line), fp);){
        if(strlen(line) == SIZE){
            memcpy((void *)issue.template[i++], (void *)line, SIZE);
            printf("  %s\n", line);
        }
    }
    printf("}\n");

    fclose(fp);

    return 0;
}


int main(int argc, char* argv[]){
   
   read_template();

   print_sudoku(issue.template);
   return 0;  
}