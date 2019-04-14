#include <stdio.h>

/*
 * Prints the matrix in a table style
 */
void print_matrix(char *matrix, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%c ",*(matrix+i*n+j));
        }
        printf("\n");
    }
    printf("\n\n");
}



/*
 * Fills the matrix with the given character
 */
void fill_matrix(char *matrix, int n, char content){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            *(matrix+i*n+j) = content;
        }
    }
}



/*
 * Copies the orig matrix to the dest matrix
 * Both must be squared
 */
void copy_matrix(char *dest, char *orig, int n){
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



int main() {

    int n = 5;
    char matrix[5][5] = {
            {'-','-','-','-','-'},
            {'X','-','-','-','-'},
            {'X','-','-','-','-'},
            {'X','-','-','-','-'},
            {'-','-','-','-','-'},
    };

    while(1) {
//        printf("\e[1;1H\e[2J");
        print_matrix((char *) matrix, n);
        printf("Enter r to rotate or enter x to finish:");
        char c[2];
        scanf("%s", c);
        if (c[0] != 'x') {
            rotate((char *) matrix, n);
            printf("\n");
        } else{
            break;
        }
    }
    return 0;

}