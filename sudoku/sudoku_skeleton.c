#include "stdio.h"
#include "string.h"

#define TABLESIZE               (9)                           // the 9box table size
#define BOXSIZE                 (3)                           //there has 9 box, each of they is 3
#define CONFIRMED_NUM            TABLESIZE                    //while a box's all cell are fixed, the confirmed number is 9(TABLESIZE)

#define NumNULL                 (0x30)
#define SPACE_13                "             "
#define SPACE_3                 "   "

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

char values[TABLESIZE]={'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9'};

void debug_stat(){
    int row, colum;

    for(row=0; row<TABLESIZE; row++){
        for(colum=0; colum<TABLESIZE; colum++){
            //int r = row%BOXSIZE;
            //int c = colum%BOXSIZE;            
            int boxid = row/BOXSIZE*3 + colum/BOXSIZE;
            
            int index;
            printf("-------------\n");
            printf("boxid:\t\t\t%d\n", boxid);
            
            printf("confirmed_num:\t\t");
            printf("%d", issue[boxid].stat.confirmed_num);
            printf("\n");
            
            printf("confirmed_rows[0~2]:\t");
            for(index=0; index<BOXSIZE; index++){
                printf("%d", issue[boxid].stat.confirmed_rows[index]);
            }
            printf("\n");

            printf("confirmed_colums[0~2]:\t");
            for(index=0; index<BOXSIZE; index++){
                printf("%d", issue[boxid].stat.confirmed_colums[index]);
            }
            printf("\n");
            
            printf("unconfirmed_value_num[0~8]:\t");
            for(index=0; index<TABLESIZE; index++){
                printf("%d(%d) ", issue[boxid].stat.unconfirmed_value_num[index]);
            }
            printf("\n");
        }
    }
    
    printf("\n");
}


void print_sudoku(int unconfirmed){
    int row, colum;

    for(row=0; row<TABLESIZE; row++){
        if(unconfirmed == 0){
            printf(" ");   
        }
        
        for(colum=0; colum<TABLESIZE; colum++){
            int r = row%BOXSIZE;
            int c = colum%BOXSIZE;            
            int boxid = row/BOXSIZE*3 + colum/BOXSIZE;

            
            
            
            if(issue[boxid].cell[r][c].confirmed == 1){
                if(unconfirmed == 1){
                    printf("%c%s", issue[boxid].cell[r][c].value[0], SPACE_13);
                }else{
                    printf("%c", issue[boxid].cell[r][c].value[0]);
                }
            }else{
                if(unconfirmed == 1){
                    int v;

                    printf("{");
                    for(v=0; v<TABLESIZE; v++){
                        if(issue[boxid].cell[r][c].value[v] == NumNULL){
                            printf(" ");
                            continue;
                        }
                        printf("%c", issue[boxid].cell[r][c].value[v]);
                    }
                    printf("}%s", SPACE_3);
                }else{
                    printf("%c", NumNULL);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
    
}

void print_result(){
    print_sudoku(1);
}

void print_template(){
    print_sudoku(0);
}

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
                if(line[issue[boxid].position.row + r ][issue[boxid].position.colum + c] != NumNULL){
                    issue[boxid].cell[r][c].value[0] = line[issue[boxid].position.row + r ][issue[boxid].position.colum + c];    
                    issue[boxid].cell[r][c].confirmed = 1;
                    
                    issue[boxid].stat.confirmed_num += 1;
                    issue[boxid].stat.confirmed_rows[r] += 1;
                    issue[boxid].stat.confirmed_colums[c] += 1;
                    issue[boxid].stat.unconfirmed_value_num[(int)(line[issue[boxid].position.row + r ][issue[boxid].position.colum + c] - NumNULL)] -= 1; //when a value is confirmed the unconfirmed value will minus 1.
                    issue[boxid].stat.unconfirmed_value_num_rows[r] -= 1;
                    issue[boxid].stat.unconfirmed_value_num_colums[c] -= 1;
                }
            }
        }
    }
    
    printf("\nTemplate:\n{\n");
    print_template();
    printf("}\n\n");

    fclose(fp);

    //debug_stat();
    return 0;
}

int init(){
    int boxid;
    
    memset(&issue, 0, sizeof(issue));
    for(boxid=0; boxid<TABLESIZE; boxid++){
        int r, c;
        int value;
        
        issue[boxid].boxid = boxid;
        issue[boxid].position.row = (boxid/BOXSIZE) * BOXSIZE;
        issue[boxid].position.colum = (boxid%BOXSIZE) * BOXSIZE;
        for(r=0; r<BOXSIZE; r++){
            for(c=0; c<BOXSIZE; c++){
                memcpy(issue[boxid].cell[r][c].value, values, TABLESIZE);
                issue[boxid].cell[r][c].confirmed = 0;
            }
        }
        
        for(value=0; value<TABLESIZE; value++){
            issue[boxid].stat.unconfirmed_value_num[value] = TABLESIZE;
        }

        for(value=0; value<BOXSIZE; value++){
            issue[boxid].stat.unconfirmed_value_num_rows[value] = BOXSIZE;
            issue[boxid].stat.unconfirmed_value_num_colums[value] = BOXSIZE;
            
            issue[boxid].stat.confirmed_rows[value] = 0;
            issue[boxid].stat.confirmed_colums[value] = 0;
        }

        issue[boxid].stat.confirmed_num = 0;
    }
    
    return read_template();
}

int main(int argc, char* argv[]){
    if(-1 == init()) return -1;

    print_result();
    return 0;
}