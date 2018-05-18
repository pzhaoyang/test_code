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
#define NumNULL   0x30

#define SIZE 9
#define BOXSIZE 3

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
                    if(p[r][c].value[v] == NumNULL){
                        printf(" ");
                        continue;
                    }

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
            if(issue.template[x][y] != NumNULL ){
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

// dist block exclude way:
void block_compare(int row, int colum){
    int boxId, box1Id, box2Id, boxColum, boxRow;
    int cellRow, cellColum;
    int r;
    int c, z;
    int hasno;
    result_t (*p)[SIZE] = issue.sresult;

    if(p[row][colum].confirmed != 1) return;

    boxColum = colum/BOXSIZE;
    boxRow = row/BOXSIZE;
    boxId = boxRow * BOXSIZE + boxColum;
    
    cellRow = row%BOXSIZE;
    cellColum = colum%BOXSIZE;
    
    switch(boxId%BOXSIZE){
        case 0:
            box1Id = boxId + 1;
            box2Id = boxId + 2;
            break;
        case 1:
            box1Id = boxId - 1;
            box2Id = boxId + 1;
            break;
        case 2:
            box1Id = boxId - 1;
            box2Id = boxId - 2;
            break;
    }

    //row block exclude
    
    //three row boxes line cycle
    for(r=0; r<BOXSIZE; r++){ 
        if(cellRow == r) continue;  // skip target line
        
        for(box=0; c<BOXSIZE; c++){
            
        }
    
    }
    
    
    
    
    
    
    
    
    
    hasno=0;
    z=0;
    for(r=0; r<BOXSIZE; r++){ //three row boxes line cycle
        if(cellRow == r) continue;  // skip target line
        
        for(c=0; c<BOXSIZE; c++){
            if(p[box1Id/BOXSIZE + r][box1Id%BOXSIZE + c].confirmed == 1){
                if(p[box1Id/BOXSIZE + r][box1Id%BOXSIZE + c].value[0] == p[row][colum].value[0]){
                    //if row neighbor other lines has same number, needn't judge, skip this row.
                    hasno = 0;
                    break;
                }else{
                    //if row neighbor other lines has confirmed number, needn't judge, skip this cell to next cell.
                    hasno = hasno | (0x01 << c);
                    continue;
                }
                
            }else{
                for(z=0; z<SIZE; z++){
                    if(p[box1Id/BOXSIZE + r][box1Id%BOXSIZE + c].value[z] == p[row][colum].value[0]) {
                        break;
                    }
                }
                if(z == SIZE){
                    hasno = hasno | (0x01 << c);
                }
            }
        }
        
        if(z == SIZE && ){
            //there is no same value, so box1Id row[box1Id/BOXSIZE + r] has no that number, the box2Id another row must has no the value
        }
        
        
    }
    
    

}

void base_compare(int need_compared_row, int need_compared_colum){
    int r, c, z;
    result_t (*p)[SIZE] = issue.sresult;

    for(z=0; z<SIZE; z++){

        //skip invalid number
        if(p[need_compared_row][need_compared_colum].value[z] == NumNULL) continue;


        //row compare r is fixed. acture is colum compare
        for(c=0; c<SIZE; c++){
            //skip unconfirmed number, just using confirmed number to compare specified cell
            if(p[need_compared_row][c].confirmed != 1) continue;

            if(p[need_compared_row][c].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                //if this is equal, then remove it using NumNULL
                p[need_compared_row][need_compared_colum].value[z] = NumNULL;
            }
        }

        //colum compare c is fixed, acture is row compare
        for(r=0; r<SIZE; r++){
            //skip unconfirmed number, just using confirmed number to compare specified cell
            if(p[r][need_compared_colum].confirmed != 1) continue;

            if(p[r][need_compared_colum].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                //if this is equal, then remove it using NumNULL
                p[need_compared_row][need_compared_colum].value[z] = NumNULL;
            }
        }

        //box compare
        r = (need_compared_row/3)*3;
        for(; r < (need_compared_row/3)*3+3; r++){
            c = (need_compared_colum/3)*3;
            for(; c < (need_compared_colum/3)*3+3; c++){
                //skip unconfirmed number, just using confirmed number to compare specified cell
                if(p[r][c].confirmed != 1) continue;

                if(p[r][c].value[0] == p[need_compared_row][need_compared_colum].value[z]){
                    //if this is equal, then remove it using NumNULL
                    p[need_compared_row][need_compared_colum].value[z] = NumNULL;
                }
            }
        }
    }
}


void exclude_exist_num(){
    int x, y;
    result_t (*p)[SIZE] = issue.sresult;

    if( p == NULL) return;

    for(x=0; x<SIZE; x++){
        for(y=0; y<SIZE; y++){
            if( p[x][y].confirmed == 1){
               block_compare(x, y);
               continue;
            }

            base_compare(x, y);

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