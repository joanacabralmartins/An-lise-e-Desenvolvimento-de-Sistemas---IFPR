#include "util.h"

int mapa[ALT][LAR];
int mapa_esp[ALT][LAR];
int px, py, px_final, py_final;


void menu(int op);
void ler_mapa(char nome[30]);
void init_espelho();
int num_mapa();
int mostrar_mapa();
void mapa_aut();
int achar_caminho(int lin_inicial, int col_inicial);
void capturar_jogada();
void final();


typedef struct { // estrutura para guardar as posicoes do caminho encontrado na funcao achar_caminho()

    int x[500];
    int y[500];

} pos;
pos caminho;
int i = 0;

int main() {
    int op;

    printf("\t\t\t\t\tGAME RESCUE");
    printf("\n\n- Escolha uma das opcoes de jogo:");
    printf("\n\t(1) Manual");
    printf("\n\t(2) Automatico");
    printf("\n\t> ");
    scanf("%i", &op);

    menu(op);

    printf("\n");
    return 0;
}

void menu(int op) {
    FILE *nomes;
    char nome[30];
    int num, ret, cont=1;
    
    switch (op)
    {
    case 1:
        nomes = fopen("nomes.bin", "rb");
        
        system("clear");
        printf("\t\t\t\t\tGAME RESCUE");
        printf("\n\n- Escolha um dos cenarios a seguir:\n");

        do {
            ret = fread(&nome, sizeof(nome), 1, nomes);
            if(ret == 1) {
                printf("\t(%i) %s\n", cont, nome);
            }
            cont++;
        } while(ret == 1);
        fclose(nomes);
        
        printf("\tDigite o nome do mapa desejado > ");
        __fpurge(stdin);
        gets(nome);

        ler_mapa(nome);
        
        if (mostrar_mapa() == 1) {
            final();
        }
        
        break;
    
    case 2: 
        num = num_mapa();
        
        nomes = fopen("nomes.bin", "rb");
        do {
            ret = fread(&nome, sizeof(nome), 1, nomes);
            if(ret == 1) {
                if(num == cont) {
                    break;
                }
            }
            cont++;
        } while(ret == 1);
        fclose(nomes);
        
        ler_mapa(nome);
        init_espelho();
        achar_caminho(px, py);
        mapa_aut();
        final();
       
        break;
    }
}

int num_mapa() { // gera um numero para um mapa aleatorio
    srand(time(NULL));
    FILE *nomes;
    char nome[30];
    int num_mapa, cont=0, ret;

    nomes = fopen("nomes.bin", "rb");
    
    do {
        ret = fread(&nome, sizeof(nome), 1, nomes);
        if(ret == 1) {
            cont++;
        }
    } while(ret == 1);

    num_mapa = 1 + (rand() % cont); 
    fclose(nomes);
    
    return num_mapa;
}

void ler_mapa(char nome[30]) { // le o mapa e guarda posicoes iniciais e finais
    FILE *arq;
    
    arq = fopen(nome, "rb");
    fread(&mapa, sizeof(mapa), 1, arq);

    for(int l=0; l<ALT; l++) {
        for(int c=0; c<LAR; c++) {

            if(mapa[l][c] == INICIO) {
                px = l;
                py = c;
                mapa[l][c] = CAMINHO;
            }
            if(mapa[l][c] == ANIMAL) {
                px_final = l;
                py_final = c;
            }
        }
    }
    
    fclose (arq);
}

void init_espelho() {
    
    for (int l=0; l<ALT; l++) {
        for (int c=0; c<LAR; c++) {
            mapa_esp[l][c] = mapa[l][c];
        }
    }
}

int mostrar_mapa() {
    
    system("clear");
    printf("\t\t\tGAME RESCUE\n");
    for(int i=0; i<ALT; i++) {
        for(int j=0; j<LAR; j++) {

            if(i == px && j == py) {
				COR_AZUL
				printf("☻");
			}
            else if(mapa[i][j] == CAMINHO) {
                COR_BRANCO
                printf(" ");
            }
            else if(mapa[i][j] == PAREDE) {
                COR_BRANCO
				printf("#");
			}
            else if(mapa[i][j] == ANIMAL) {
                COR_LARANJA
				printf("♞");
			}
        }
        printf("\n");
    }

    if(px == px_final && py == py_final) {
        return 1;
    }
    else {
        capturar_jogada();
    }
}

void capturar_jogada() {
    char jogada;
    jogada = getch();

	if(jogada == 27) {
		jogada = getch(); 

		if(jogada == 91) {      
			jogada = getch();
				
			if(jogada == 65) {
				if(px > 0) {
					px--;
				}
				if(mapa[px][py] == 1) {
					 px++;
				}
			}		
			else if(jogada == 66) {
		    	if(px < ALT-1) {
					px++;
				}
				if(mapa[px][py] == 1) {
					px--;
				}
			}
			else if(jogada == 67) {
				if(py < LAR-1) {
					py++;
				}
				if(mapa[px][py] == 1) {
					py--;
				}
			}
			else if(jogada == 68) {
				if(py > 0) {
					py--;
				}
				if(mapa[px][py] == 1) {
					py++;
				}
			}
		}
	}
    mostrar_mapa();
}

int achar_caminho(int lin_inicial, int col_inicial) {
    
	if (mapa_esp[lin_inicial][col_inicial] == ANIMAL) {
        caminho.x[i] = px_final;
        caminho.y[i] = py_final;
	    i++;
		return 1;
	}
	
	if (mapa_esp[lin_inicial][col_inicial] == CAMINHO) {
		mapa_esp[lin_inicial][col_inicial] = PAREDE;

		if (achar_caminho(lin_inicial, col_inicial - 1)){
		    caminho.x[i] = lin_inicial;
            caminho.y[i] = col_inicial;
	        i++;
			return 1;
		} 
		if (achar_caminho(lin_inicial + 1, col_inicial)){
		    caminho.x[i] = lin_inicial;
            caminho.y[i] = col_inicial;
	        i++;
			return 1;
		} 
		if (achar_caminho(lin_inicial, col_inicial + 1)){
		    caminho.x[i] = lin_inicial;
            caminho.y[i] = col_inicial;
	        i++;
			return 1;		
		} 
		if (achar_caminho(lin_inicial - 1, col_inicial)){
		    caminho.x[i] = lin_inicial;
            caminho.y[i] = col_inicial;
	        i++;
			return 1;
		}
	}
	
    return 0;
}

void mapa_aut() {
    int lin, col;
    caminho.x[i] = px;
    caminho.y[i] = py;
	        
    do {
        sleep(1);
        system("clear");
	    printf("\t\t  EDITOR DE MAPAS - GAME RESGATE\n");
	    for(lin=0; lin<ALT; lin++) {
		    for(col=0; col<LAR; col++) {

				if(lin == caminho.x[i] && col == caminho.y[i]) {
					COR_VERMELHO					
					printf("☻");
				}
				else if(mapa[lin][col] == CAMINHO) {
					COR_BRANCO
					printf(" ");
				}
				else if(mapa[lin][col] == PAREDE) {
					COR_BRANCO
					printf("#");
				}
				else if(mapa[lin][col] == ANIMAL) {
					COR_LARANJA
					printf("♞");
				} 
			}
			printf("\n");
		}
		i--;
    } while(i >= 0);
}

void final() {
    int milli_sec = 1000 * 3;
    clock_t start_time = clock();
    int num = 2;

    while (clock() < start_time + milli_sec) {
        sleep(1);
        system("clear");
        sleep(1);
        
        if (num % 2 == 0) {
            COR_AZUL
        }
        else {
            COR_LARANJA
        }
        printf("\t\t\t\t\tGAME RESCUE");
        printf("\n\n\t\t\t[PARABENS]: Resgate concluido com SUCESSO!\n\n\n");
        
        for(int l=0; l<ALT; l++) {
            for(int c=0; c<LAR; c++) {

                if(mapa[l][c] == ANIMAL) {
                    if (num % 2 == 0) {
                        COR_AZUL
				        printf("☻");
                    }
				    else {
				        COR_LARANJA
				        printf("♞");
				    }
			    }
                else if(mapa[l][c] == CAMINHO) {
                    COR_BRANCO
                    printf(" ");
                }
                else if(mapa[l][c] == PAREDE) {
                    COR_BRANCO
				    printf("#");
			    }
            }
            printf("\n");
        }
        num++;
    }
}