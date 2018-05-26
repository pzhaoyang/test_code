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
    int             unconfirmed_value_num_rows[BOXSIZE][TABLESIZE];  //unconfirmed values numbers of each rows in a box
    int             unconfirmed_value_num_colums[BOXSIZE][TABLESIZE];  //unconfirmed values numbers of each colums in a box
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




box_t issue[TABLESIZE]={0};

void debug_stat(){
    int row, colum;
    int printed_record[TABLESIZE]={0};

    for(row=0; row<TABLESIZE; row++){
        for(colum=0; colum<TABLESIZE; colum++){
            //int r = row%BOXSIZE;
            //int c = colum%BOXSIZE;
            int boxid = row/BOXSIZE*3 + colum/BOXSIZE;
            int i, index;


            if( printed_record[boxid] == 0){
                printed_record[boxid] = 1;
            }else{
                continue;
            }

            printf("-------------------------------------------------------------------------------------\n");
            printf("boxid:\t\t\t\t\t%d\n", boxid);
            {
                int r, c;
                for(r=0; r<BOXSIZE; r++){
                    for(c=0; c<BOXSIZE; c++){
                        if(issue[boxid].cell[r][c].confirmed == 1){
                            printf("%c", issue[boxid].cell[r][c].value[0]);
                        }else{
                            printf("%c", NumNULL);
                        }
                    }
                    printf("\n");
                }
            }
            printf("confirmed_num:\t\t\t\t");
            printf("%d", issue[boxid].stat.confirmed_num);
            printf("\n");

            printf("confirmed_rows[0~2]:\t\t\t");
            for(index=0; index<BOXSIZE; index++){
                printf("%d", issue[boxid].stat.confirmed_rows[index]);
            }
            printf("\n");

            printf("confirmed_colums[0~2]:\t\t\t");
            for(index=0; index<BOXSIZE; index++){
                printf("%d", issue[boxid].stat.confirmed_colums[index]);
            }
            printf("\n");

            printf("unconfirmed_value_num[0~8]:\t\t");
            for(index=0; index<TABLESIZE; index++){
                if(issue[boxid].stat.unconfirmed_value_num[index] != 0){
                    printf("%d(%d) ", index+1, issue[boxid].stat.unconfirmed_value_num[index]);
                }
            }
            printf("\n");

            for(i=0; i<BOXSIZE; i++){
                printf("unconfirmed_value_num_rows[%d]:\t\t", i);
                for(index=0; index<TABLESIZE; index++){
                    if(issue[boxid].stat.unconfirmed_value_num_rows[i][index] != 0){
                        printf("%d(%d) ", index+1, issue[boxid].stat.unconfirmed_value_num_rows[i][index]);
                    }
                }
                printf("\n");


                printf("unconfirmed_value_num_colums[%d]:\t", i);
                for(index=0; index<TABLESIZE; index++){
                    if(issue[boxid].stat.unconfirmed_value_num_colums[i][index] != 0){
                        printf("%d(%d) ", index+1, issue[boxid].stat.unconfirmed_value_num_colums[i][index]);
                    }
                }
                printf("\n");
            }


        }
    }

    printf("\n");
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

void cell_update(int boxid, int r, int c, char cell_value){
    int tr, tc;
    //set confirmed value
    issue[boxid].cell[r][c].value[0] = cell_value;

    //set confirmed flag
    issue[boxid].cell[r][c].confirmed = 1;

    for(tr=0; tr<BOXSIZE; tr++){
        for(tc=0; tc<BOXSIZE; tc++){
            if(issue[boxid].cell[tr][tc].confirmed != 1){
                issue[boxid].cell[tr][tc].value[(int)(cell_value - NumNULL -1)] = NumNULL;
            }
        }
    }
}

void stat_update(int boxid, int r, int c, char cell_value){
    int index, idx;
    index = (int)(cell_value - NumNULL - 1);

    //confirmed number increase 1
    issue[boxid].stat.confirmed_num += 1;

    //confirmed row numbers of cell_value
    issue[boxid].stat.confirmed_rows[r] += 1;

    //confirmed colum numbers of cell_value
    issue[boxid].stat.confirmed_colums[c] += 1;

    // the confirmed cell_value in unconfirmed array is 0.
    issue[boxid].stat.unconfirmed_value_num[index] = 0;

    // every rows the confirmed cell_value in unconfirmed row array is 0.
    for(idx=0; idx< BOXSIZE; idx++){
        issue[boxid].stat.unconfirmed_value_num_rows[idx][index] = 0;
        issue[boxid].stat.unconfirmed_value_num_colums[idx][index] = 0;
    }

    //because the cell_value is confirmed, so the other values in array of unconfirmed all will be minus 1.
    for(idx=0; idx< TABLESIZE; idx++){
        if(issue[boxid].stat.unconfirmed_value_num[idx] > 0){
            issue[boxid].stat.unconfirmed_value_num[idx] -= 1;
        }

        if(issue[boxid].stat.unconfirmed_value_num_rows[r][idx] >0 ){
            issue[boxid].stat.unconfirmed_value_num_rows[r][idx] -= 1;
        }

        if(issue[boxid].stat.unconfirmed_value_num_colums[c][idx] >0 ){
            issue[boxid].stat.unconfirmed_value_num_colums[c][idx] -= 1;
        }
    }
}

void box_read(int boxid, char line[TABLESIZE][TABLESIZE+1]){
    int r,c;

    for(r=0; r<BOXSIZE; r++){
        for(c=0; c<BOXSIZE; c++){
            int read_row = issue[boxid].position.row;
            int read_colum = issue[boxid].position.colum;
            int read_number = line[ read_row + r ][read_colum + c];

            // a value is confirmed
            if( read_number != NumNULL){
                cell_update(boxid, r, c, read_number);
                stat_update(boxid, r, c, read_number);
            }
        }
    }
}

int read_template(){
    FILE* fp;
    char line[TABLESIZE][TABLESIZE+1];
    int row;
    int boxid;

    fp = fopen("template.easy", "r");
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
        box_read(boxid, line);
    }

    printf("\nTemplate:\n- - - - - - - - \n");
    print_template();
    printf("- - - - - - - - -\n\n");

    fclose(fp);

    //debug_stat();
    return 0;
}

void debug_pointer(){
    int boxid;

    for(boxid = 0; boxid < TABLESIZE; boxid++){
        printf("current boxid: [%02d], up:%02d, down:%02d, left:%02d, right:%02d\n",
            boxid,
            ((issue[boxid].up == 0) ? -1 : issue[boxid].up->boxid),
            ((issue[boxid].down == 0) ? -1 : issue[boxid].down->boxid),
            ((issue[boxid].left == 0) ? -1 : issue[boxid].left->boxid),
            ((issue[boxid].right == 0) ? -1 : issue[boxid].right->boxid));
    }
}
void init_porinter(int boxid){
#define INVALID_NUM -1
    int id_up = (((boxid - BOXSIZE) >= 0) ? (boxid - BOXSIZE) : INVALID_NUM);
    int id_down = (((boxid + BOXSIZE) <= (TABLESIZE - 1)) ? (boxid + BOXSIZE) : INVALID_NUM);
    int id_left = ((boxid%BOXSIZE == 0) ? INVALID_NUM : (boxid - 1));
    int id_right = ((boxid%BOXSIZE == (BOXSIZE - 1)) ? INVALID_NUM :  (boxid + 1));

    issue[boxid].up = ((id_up != -1) ? &issue[id_up] : 0);
    issue[boxid].down = ((id_down != -1) ? &issue[id_down] : 0);
    issue[boxid].left = ((id_left != -1) ? &issue[id_left] : 0);
    issue[boxid].right = ((id_right != -1) ? &issue[id_right] : 0);
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

        init_porinter(boxid);

        for(r=0; r<BOXSIZE; r++){
            for(c=0; c<BOXSIZE; c++){
                int z;
                for(z=0; z<TABLESIZE; z++){
                    issue[boxid].cell[r][c].value[z] = z + '1';
                }
                issue[boxid].cell[r][c].confirmed = 0;
            }
        }

        for(value=0; value<TABLESIZE; value++){
            issue[boxid].stat.unconfirmed_value_num[value] = TABLESIZE;
        }

        for(value=0; value<BOXSIZE; value++){
            int index;

            for(index=0; index<TABLESIZE; index++){
                issue[boxid].stat.unconfirmed_value_num_rows[value][index] = BOXSIZE;
                issue[boxid].stat.unconfirmed_value_num_colums[value][index] = BOXSIZE;
            }

            issue[boxid].stat.confirmed_rows[value] = 0;
            issue[boxid].stat.confirmed_colums[value] = 0;
        }

        issue[boxid].stat.confirmed_num = 0;
    }

    //debug_pointer();
    //debug_stat();
    return read_template();
}

void row_exculde(){

}

void colum_exculde(){

}

void base_exculde(){
    row_exculde();
    colum_exculde();
}

void resolve(){
    base_exculde();
}
int main(int argc, char* argv[]){
    if(-1 == init()) return -1;

    resolve();

    print_result();
    return 0;
}