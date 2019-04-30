char *matriz[5][40];

void crear_matriz(){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 40; j++){
            if (j == 0){
                if (i == 0){
                    matriz[i][j] = "A";
                }else if (i == 1){
                    matriz[i][j] = "B";
                }else if (i == 2){
                    matriz[i][j] = "C";
                }else if (i == 3){
                    matriz[i][j] = "D";
                }else if (i == 4){
                    matriz[i][j] = "E";
                }
            }else{
                matriz[i][j] = ".";
            }
        }
    }

}


void imprime_matriz(){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 40; j++){
            printf("%s",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}


void mover_derecha(char *elemento){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 40; j++){
            if (matriz[i][j] == elemento){
                matriz[i][j] = ".";
                matriz[i][j+1] = elemento;
                imprime_matriz();
                return;
            }
        }
    }
}
