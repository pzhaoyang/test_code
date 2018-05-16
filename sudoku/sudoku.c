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

#define DEBUG 1
#define SPACE_13 "             "
#define SPACE_3 "   "

#define SIZE 9

typedef struct result{
    char value[SIZE]; //if the value is confirmed, the value[0] will be the confirmed value, and others will be 0.
    int confirmed; //0 is not confirmed, 1 is confirmed
}result_t;

typedef struct sudoku {
    char template[SIZE][SIZE];
    result_t sresult[SIZE][SIZE];
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
void print_sudoku(result_t p[9][9]){
    int c,r,v;
    if( p == NULL){
        printf("\n******\nThe sudoku is invalid!\n******\n");
        return;
    }

    printf("\nThe Result:\n");
    for(r=0; r<SIZE; r++){
        for(c=0; c<SIZE; c++){
            if(p[r][c].confirmed == 1){
                printf("%c%s", p[r][c].value[0], SPACE_13);
            }else{
                printf("{");
                for(v=0; v<SIZE; v++){
                    printf("%c", p[r][c].value[v]);
                }
                printf("}%s", SPACE_3);
            }

        }
        printf("\n");
    }
    printf("\n");
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
    printf("}\n\n");

    fclose(fp);

    return 0;
}

void resolve_init(){
    int x, y, z;
    #if DEBUG
        printf("resolve_init start:\n");
    #endif
    for(x=0; x<SIZE; x++){
        for(y=0; y<SIZE; y++){
            if(issue.template[x][y] != '0' ){
                issue.sresult[x][y].value[0] = issue.template[x][y];
                issue.sresult[x][y].confirmed = 1;
                #if DEBUG
                    printf("%c(%d) ", issue.sresult[x][y].value[0], issue.sresult[x][y].confirmed);
                #endif
                continue;
            }

            issue.sresult[x][y].confirmed = 0;
            #if DEBUG
                printf("0(%d) ", issue.sresult[x][y].confirmed);
            #endif
            for(z=0; z<SIZE; z++){
               issue.sresult[x][y].value[z] = z + '1';
            }
        }
        #if DEBUG
            printf("\n");
        #endif
    }
    #if DEBUG
        printf("resolve_init end.\n");
    #endif
}

int update_confirm(int row, int colum){
    int statistics, locate, z;
    result_t (*p)[SIZE] = issue.sresult;

    //statistics and then to update confirmed value
    for(statistics=0, z=0; z<SIZE; z++){
        if(p[row][colum].value[z] != '0'){
            statistics++;
            locate = z;
        }
    }
    if(statistics == 1){
        // fixed a cell number
        p[row][colum].value[0] = p[row][colum].value[locate];
        p[row][colum].confirmed = 1;

        return 0;
    }
    return -1;
}

void cell_compare(int need_compared_row, int need_compared_colum){
    int r, c, z;
    result_t (*p)[SIZE] = issue.sresult;

    for(z=0; z<SIZE; z++){
        if(p[need_compared_row][need_compared_colum].value[z] == '0') continue;
        //row compare r is fixed. acture is colum compare
        for(c=0; c<SIZE; c++){
            if(p[need_compared_row][c].confirmed != 1) continue;

            if(p[need_compared_row][c].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                p[need_compared_row][need_compared_colum].value[z] = '0'; //if this is equal, then remove it using '0'
            }
        }

        //colum compare c is fixed, acture is row compare
        for(r=0; r<SIZE; r++){
            if(p[r][need_compared_colum].confirmed != 1) continue;

            if(p[r][need_compared_colum].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                p[need_compared_row][need_compared_colum].value[z] = '0'; //if this is equal, then remove it using '0'
            }
        }

        //small square compare
        r = (need_compared_row/3)*3;
        c = (need_compared_colum/3)*3;
        for(; r<SIZE/3; r++){
            for(; c<SIZE/3; c++){
                if(p[r][c].confirmed != 1) continue;

                if(p[r][c].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                    p[need_compared_row][need_compared_colum].value[z] = '0'; //if this is equal, then remove it using '0'
                }
            }
        }

        //relate compare


    }
}


void exclude_exist_num(){
    int x, y;
    result_t (*p)[SIZE] = issue.sresult;

    if( p == NULL) return;

    for(x=0; x<SIZE; x++){
        for(y=0; y<SIZE; y++){
            if( p[x][y].confirmed == 1){
               continue;
            }
            cell_compare(x, y);
            if(0 == update_confirm(x, y)){
                exclude_exist_num();
            }
        }
    }


}

void resolve(){
    resolve_init();
    exclude_exist_num();
}

void init(){
    memset(&issue, 0, sizeof(issue));
}


int main(int argc, char* argv[]){
   init();

   read_template();

   resolve();

   print_sudoku(issue.sresult);
   return 0;
}