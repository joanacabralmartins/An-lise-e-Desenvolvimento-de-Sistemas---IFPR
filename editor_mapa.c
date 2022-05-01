#include "util.h"

int mapa[ALT][LAR];
int mapa_esp[ALT][LAR];
int lin_inicial, col_inicial;


int checar_figuras();
void init_espelho();
int achar_caminho(int lin_inicial, int col_inicial);


int main() {
	
	int lin, col, final, px, py;
	int erro_a = 0, erro_b = 0;
	char jogada;
	
	// INICIALIZA O MAPA COM PAREDES
	for(lin=0; lin<ALT; lin++) {
		for(col=0; col<LAR; col++) {
			mapa[lin][col] = PAREDE;
		}
	}

	// EDITOR DE MAPAS - USUARIO MONTA O MAPA A SER USADO NO JOGO
	final = 0;
	px = py = 1;
	while(final == 0) {

		COR_BRANCO
		system("clear");
		printf("\t\t  EDITOR DE MAPAS - GAME RESGATE\n");
		for(lin=0; lin<ALT; lin++) {
			
			for(col=0; col<LAR; col++) {

				if(lin == px && col == py) {
					COR_VERMELHO					
					printf("_");
				}
				else if(mapa[lin][col] == CAMINHO) {
					COR_BRANCO
					printf(" ");
				}
				else if(mapa[lin][col] == PAREDE) {
					COR_BRANCO
					printf("#");
				}
				else if(mapa[lin][col] == INICIO) {
					COR_AZUL
					printf("☻");
				}
				else if(mapa[lin][col] == ANIMAL) {
					COR_LARANJA
					printf("♞");
				} 
			}
			printf("\n");
		}
		
		COR_BRANCO
		printf("    (C: Caminho)(P: Parede)(I: Inicio)(A: Animal)(O: OK)");
		
		if(erro_a == 1) {
		    COR_LARANJA
		    printf("\n    [ERRO] Insira o numero correto de figuras ([1] inicio e [1] animal)!\n");
		}
		else if(erro_b == 1) {
		    COR_LARANJA
			printf("\n    [ERRO] Nao ha caminho para efetuar o resgate!\n");
		}
        COR_BRANCO
		// CAPTURA TECLA
		jogada = getch();

		// TECLA ESPECIAL
		if(jogada == 27) {
			jogada = getch(); 

			if(jogada == 91) {      
				jogada = getch();
				
				// CIMA
				if(jogada == 65) {
					if(px > 0) {
						px--;
					}
				}		
	  			// BAIXO
				else if(jogada == 66) {
		    			if(px < ALT-1) {
						px++;
					}
				}
				// DIREITA
				else if(jogada == 67) {
					if(py < LAR-1) {
						py++;
					}
			    }
				// ESQUERDA
				else if(jogada == 68) {
					if(py > 0) {
						py--;
					}
				}
			}
		}
		// PAREDE
		else if(jogada == 'P' || jogada == 'p') {		
			mapa[px][py] = PAREDE;
		}
		// CAMINHO
		else if(jogada == 'C' || jogada == 'c') {		
			mapa[px][py] = CAMINHO;
		}
		// INICIO
		else if(jogada == 'I' || jogada == 'i') {
			mapa[px][py] = INICIO;
		}
		// ANIMAL
		else if(jogada == 'A' || jogada == 'a') {
			mapa[px][py] = ANIMAL;
		}
		// OK - SAIR
		else if(jogada == 'O' || jogada == 'o') {
		    
		    if (checar_figuras() == 1) {
		        init_espelho();
		        
		        if (!achar_caminho(lin_inicial, col_inicial)) {
		            erro_b = 1;
		        } else {
		            final = 1;
		        }
		    }
            else if (checar_figuras() == -1) {
                erro_a = 1;
            }
		}
	}
	
	char nome[30];
	FILE *nomes, *arq;
    	system("clear");
	printf("\t\t  EDITOR DE MAPAS - GAME RESGATE\n");
	printf("\n\t- Digite um nome para salvar o mapa: ");
	__fpurge(stdin);
	gets(nome);

	nomes = fopen("nomes.bin", "a+");
	fwrite(&nome, sizeof(nome), 1, nomes);
	fclose(nomes);
	
	arq = fopen(nome, "a+");
    	fwrite(&mapa, sizeof(mapa), 1, arq);
    
    	if(arq != NULL) {
        	printf("\n\t- [OK] O cenario [%s] foi salvo com sucesso!", nome);
    	}
    	fclose(arq);

	printf("\n");
	return 0;
}

int checar_figuras() {
    int cont_i = 0, cont_f = 0;
    
    for(int i=0; i<ALT; i++) {
        for(int j=0; j<LAR; j++) {
            
            if (mapa[i][j] == INICIO) {
                cont_i++;
            }
            if (mapa[i][j] == ANIMAL) {
                cont_f++;
            }
        }
    }
    
    if(cont_i == 1 && cont_f == 1) {
        return 1;
    } else {
        return -1;
    }
}

void init_espelho() { // preenche o mapa espelho e tambem guarda a posicao inicial
    
    for (int i=0; i<ALT; i++) {
        for (int j=0; j<LAR; j++) {
            
            if (mapa[i][j] == PAREDE) {
				mapa_esp[i][j] = PAREDE;
			} 
			else if (mapa[i][j] == ANIMAL) {
				mapa_esp[i][j] = ANIMAL;
			} 
			else {
				mapa_esp[i][j] = CAMINHO;
			}
            
            if(mapa[i][j] == INICIO) {
                lin_inicial = i;
                col_inicial = j;
            }
        }
    }
}

/* funcao que usa da recursividade para encontrar o caminho disponivel ate o animal. 
Explora todas as direcoes: esquerda, baixo, direita, cima. Comeca explorando uma ate
encontrar uma parede, entao retorna e passa a explorar a proxima direcao. Retorna 
verdadeiro caso consiga fazer o caminho do inicio ate o animal e do animal de volta ate o inicio. 
Caso a recursao nunca chegar ate o animal, retorna falso e entao finaliza. */ 

int achar_caminho(int lin_inicial, int col_inicial) {

	if (mapa_esp[lin_inicial][col_inicial] == ANIMAL) {
		return 1;
	}

	if (mapa_esp[lin_inicial][col_inicial] == CAMINHO) {
		mapa_esp[lin_inicial][col_inicial] = PAREDE;

		if (achar_caminho(lin_inicial, col_inicial - 1)){
			return 1;
		} 
		if (achar_caminho(lin_inicial + 1, col_inicial)){
			return 1;
		} 
		if (achar_caminho(lin_inicial, col_inicial + 1)){
			return 1;		
		} 
		if (achar_caminho(lin_inicial - 1, col_inicial)){
			return 1;
		}
	}
    
    return 0;
}
