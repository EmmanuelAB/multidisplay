#include "Language_Parse.h"

/*
Isaac Mena L
27/04/19
After parse the json file you can use the figure_list to iterate over the figures, ask for their
attributes and use them as you want.
-----------------------------
Command to compile and run from shell: gcc main.c Language_Parse.c -ljson-c -o b && ./b
*/

void rotate(char *matrix, int n);
void figure_list_to_string();
void copy_matrix(char *dest, const char *orig, int n);
void print_matrix(const char *matrix, int n);
void delete_new_line(char *matrix, int n, int m);

int main(){

    printf("\nStarting json parse\n");

    parse_file();

    // Example of printing all the attributes of the figures once they are parsed.
    //figure_list_to_string();

    char* matrix = figure_list[0].figure;
    int rows = figure_list[0].rows;
    int cols = figure_list[0].cols;
    delete_new_line(matrix, rows, cols);
    while(1) {
        print_matrix(matrix, rows);
        printf("Enter r to rotate or enter x to finish:");
        char c[2];
        scanf("%s", c);
        if (c[0] != 'x') {
            rotate(matrix, rows);
            printf("\n");
        } else{
            break;
        }
    }

    return 0;
}

void print_matrix(const char *matrix, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%c ",*(matrix+i*n+j));
        }
        printf("\n");
    }
    printf("\n\n");
}

void figure_list_to_string(){
    for (int i = 0; i < num_objects; i++) {
        printf("\nFigure #%d\n", i+1);
        printf("\nType: %d", figure_list[i].type);
        printf("\nShape:\n%s", figure_list[i].figure);
        printf("\nStart time: %d", figure_list[i].start_time);
        printf("\nEnd time: %d", figure_list[i].end_time);
        printf("\nInitial x: %d", figure_list[i].initial_pos_x);
        printf("\nInitial y: %d", figure_list[i].initial_pos_y);
        printf("\nEnd x: %d", figure_list[i].end_pos_x);
        printf("\nEnd y: %d", figure_list[i].end_pos_y);
        printf("\nInitial angle: %d", figure_list[i].initial_angle);
        printf("\nEnd angle: %d", figure_list[i].end_angle);
        printf("\nRows: %d", figure_list[i].rows);
        printf("\nColumns: %d\n", figure_list[i].cols);
    }
}

/*
 * Copies the orig matrix to the dest matrix
 * Both must be squared
 */
void copy_matrix(char *dest, const char *orig, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            *(dest+i*n+j) = *(orig+i*n+j);
        }
    }
}

/*
    Rotates the matrix 90 degrees to clockwise
 */
void rotate(char *matrix, int n){
    // make a copy of the matrix
    char temp_matrix[n][n];
    copy_matrix((char*)temp_matrix, matrix, n);

    // start moving elements from the copy to the original
    // i, j says the current row, column at the copy
    for (int i = 0; i < n; ++i) {
        // all the elements at row i will be at column n -1 -i
        // example row 0 becomes column n-1 (since i = 0)
        int dest_col = n -1 -i;
        for (int j = 0; j < n; ++j){
            // the column at the copy says the row at the destination
            int dest_row = j;
            *(matrix+dest_row*n+dest_col) = temp_matrix[i][j];
        }
    }
}

// Delete '\n' from matrix
void delete_new_line(char *matrix, int n, int m){
    char buffer_temp[n*(m-1)];
    int offset1 = 0, offset2 = 0;
    for (int i = 0; i < n; ++i) {
        strncpy((char *) (buffer_temp + offset1), matrix + offset2, m);
        offset1 += m;
        offset2 += m+1;
    }
    sprintf(matrix, "%s", buffer_temp);
}