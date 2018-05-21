#include "stdio.h"
#include "string.h"

#define TABLESIZE               (9)                           // the 9box table size
#define BOXSIZE                 (3)                           //there has 9 box, each of they is 3
#define CONFIRMED_NUM            TABLESIZE                    //while a box's all cell are fixed, the confirmed number is 9(TABLESIZE)

typedef struct box_stat{
    int             confirmed_num;                //confirmed cell numbers in a box    like: box: 6
    int             confirmed_rows[BOXSIZE];      //confirmed row                      like: row1: confirmed(3), row2: unconfirmed(<3), row3: confirmed(3)  
    int             confirmed_colums[BOXSIZE];    //confirmed colum                    like: colum1: confirmed(3), colum2: unconfirmed(<3), colum3: confirmed(3)
    int             unconfirmed_value_num[TABLESIZE];   //unconfirmed value numbers in box    
    int             unconfirmed_value_num_rows[BOXSIZE];  //unconfirmed value numbers of each rows in a box
    int             unconfirmed_value_num_colums[BOXSIZE];  //unconfirmed value numbers of each colums in a box
}stat_t;

typedef struct cell{
    char            value[TABLESIZE];                  //if the value is confirmed, the value[0] will be the confirmed value, and others will be 0.
    int             confirmed;                    //0 is not confirmed, 1 is confirmed
}cell_t;

typedef struct coordinate {
    int             row;
    int             colum;
}coordinate_t;

typedef struct box{
    struct box      *up;
    struct box      *down;
    struct box      *left;
    struct box      *right;

    int             boxid;
    coordinate_t    position;
    cell_t          cell[BOXSIZE][BOXSIZE];
    stat_t          stat;

    
}box_t, *pbox_t;




box_t   issue[TABLESIZE]={0};

int read_template(){
    FILE* fp;
    char line[TABLESIZE][TABLESIZE+1];
    int row;
    int boxid;

    fp = fopen("template.expert", "r");
    if(fp == NULL){
        printf("Cannot open the template file!.\n");
        return -1;
    }

    for(row=0; NULL != fgets((void *)line[row], sizeof(line[row]), fp) && row<TABLESIZE;){
        if(strlen(line[row]) == TABLESIZE){
            row++;
        }
    }
    
    for(boxid=0; boxid<TABLESIZE; boxid++){
        int r,c;
        for(r=0; r<BOXSIZE; r++){
            for(c=0; c<BOXSIZE; c++){
                issue[boxid].cell[r][c].value[0] = line[issue[boxid].position.row + r ][issue[boxid].position.colum + c];
                issue[boxid].cell[r][c].confirmed = 1;
                issue[boxid].stat.confirmed_num += 1;
                issue[boxid].stat.confirmed_rows[r] += 1;
                issue[boxid].stat.confirmed_colums[c] += 1;
            }
        }
    }
    
    printf("\nTemplate:\n{\n");
    printf("}\n\n");

    fclose(fp);

    return 0;
}

int init(){
    int boxid;
    int value;
    
    for(boxid=0; boxid<TABLESIZE; boxid++){
        issue[boxid].boxid = boxid;
        issue[boxid].position.row = (boxid/BOXSIZE) * BOXSIZE;
        issue[boxid].position.colum = (boxid%BOXSIZE) * BOXSIZE;
        for(value=0; value<TABLESIZE; value++){
            issue[boxid].stat.unconfirmed_value_num[value] = TABLESIZE;
        }
        
    }
    
    return read_template();
}

int main(int argc, char* argv[]){
    if(-1 == init()) return -1;

   return 0;
}