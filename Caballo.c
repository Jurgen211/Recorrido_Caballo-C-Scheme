#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct Casilla
{
	int estado;
	int numero_movimiento;
	int caballo_aqui;
	
} Casilla;

typedef struct Tablero
{
	Casilla **tablero;
	int row_count; 
	int col_count; 
	int contador_movimientos;
	int antI;
	int antJ;
	
} Tablero;




//------------------------------------------------------------------------------------------------------
void guardarPosicionInicio(int x, int y)
{
	FILE *inicio = fopen("PosInicio.txt", "w");
	
	if (!inicio)
	{
		return;
	}
	
	fprintf(inicio, "%d\n", x);
	fprintf(inicio, "%d", y);
	
	fclose(inicio);
}

int cargarSolucion(int **Solucion)
{
	FILE *solucion = fopen("RespuestaCaballo.txt", "r");
	
	if (!solucion)
	{
		return -1;
	}
	
	fseek(solucion, 0, SEEK_END);
	
	if (ftell(solucion) == 0)
	{
		fclose(solucion);
		return -1;
	}
	
	rewind(solucion);
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fscanf(solucion, "%d", &Solucion[i][j]);
		}
	}
	
	fclose(solucion);
	
	return 1;
}



//-----------------------Tablero-------------------------------------------------------------------------

Tablero table;

void inicializarTablero(){
	table.row_count = 8;
	
	table.col_count = 8;
	
	table.contador_movimientos = 0;
	
	table.tablero = (Casilla **)malloc(8 * sizeof(Casilla *)); 	
	for (int i = 0; i < 8; i++)
	{
		table.tablero[i] = (Casilla *)malloc(8 * sizeof(Casilla));
	}
	
	for (int i = 0; i < table.row_count; i++)
	{
		for (int j = 0; j < table.col_count; j++)
		{
			table.tablero[i][j].numero_movimiento = 0;
		}
	}
}

int posicionarCaballo(int i, int j){
	if (i >= table.row_count || j >= table.col_count || i < 0 || j < 0)
	{
		return -1;
	}
	
	if (table.contador_movimientos != 0)
	{
		return -1;
	}
	
	table.tablero[i][j].caballo_aqui = 1;
	table.tablero[i][j].estado = 1;
	table.tablero[i][j].numero_movimiento = table.contador_movimientos + 1;
	
	table.contador_movimientos++;
	
	table.antI = i;
	
	table.antJ = j;
	
	return 1;
}

int reposicionarCaballo(int i, int j)
{
	if (i >= table.row_count || j >= table.col_count || i < 0 || j < 0)
	{
		return -1;
	}
	
	if (table.contador_movimientos == 0)
	{
		return -1;
	}
	
	table.tablero[table.antI][table.antJ].caballo_aqui = 0;
	
	table.tablero[i][j].caballo_aqui = 1;
	table.tablero[i][j].estado = 1;
	table.tablero[i][j].numero_movimiento = table.contador_movimientos + 1;
	
	table.contador_movimientos++;
	
	table.antI = i;
	
	table.antJ = j;
	
	return 1;
}

int checkIfValidRow(char row)
{
	int total = table.col_count * table.row_count;
	
	int tam = total / 2;
	
	char letras[tam];
	
	char inicio = 'A';
	
	for (int i = 0; i < tam; i++)
	{
		letras[i] = inicio;
		
		inicio++;
	}
	
	for (int i = 0; i < tam; i++)
	{
		if (letras[i] == row)
		{
			return 1;
		}
	}
	
	return -1;
}

int retornarPosSegunChar(char row)
{
	int total = table.col_count * table.row_count;
	
	int tam = total / 2;
	
	char inicio = 'A';
	
	for (int i = 0; i < tam; i++)
	{
		if (inicio == row)
		{
			return i;
		}
		
		inicio++;
	}
	
	return -1;
}
void liberarMemoria()
{
	for (int i = 0; i < table.row_count; i++)
	{
		free(table.tablero[i]);
	}
	
	free(table.tablero);
}



//----------------------Interfaz--------------------------------------------------------------------------------------

int **solucion;

void iniciarSolucion()
{
	solucion = (int **)malloc(8 * sizeof(int *)); // Allocate row pointers
	
	for (int i = 0; i < 8; i++)
	{
		solucion[i] = (int *)malloc(8 * sizeof(int));
	}
}

void liberarSolucion()
{
	for (int i = 0; i < table.row_count; i++)
	{
		free(solucion[i]);
	}
	
	free(solucion);
}

void imprimirTablero()
{

	char row = 'A';
	
	printf("===============xxX RECORRIDO CABALLO BRYAN-JURGEN Xxx===============");
	printf("\n \n");
	
	for (int i = 0; i < table.row_count; i++)
	{
		printf(" %i |", i + 1);
		
		for (int j = 0; j < table.col_count; j++)
		{
			if (table.tablero[i][j].caballo_aqui == 1)
			{
				printf("\t[%2s]", "Z");
			}
			else if(table.tablero[i][j].numero_movimiento != 0)
			{
				if(table.tablero[i][j].numero_movimiento < 10)
				{
					printf("\t[0%d]", table.tablero[i][j].numero_movimiento);
					
				}
				else
				{
					printf("\t[%2d]", table.tablero[i][j].numero_movimiento);
				}
			}
			else
			{
				printf("\t[  ]");
			}
		}
		
		printf("\n \n");
	}
    printf("\t");
	for (int j = 0; j < 60; j++)
	{
		printf("_");
	}
	printf("\n");
	for (int j = 0; j < table.col_count; j++)
	{
		printf("\t %c", row);
		
		row++;
	}
	printf("\n");
}

void doMoves()
{
	if(table.contador_movimientos != 64)
	{
		for (int i = 0; i < table.row_count; i++)
		{
			for (int j = 0; j < table.col_count; j++)
			{
				if(solucion[i][j] == (table.contador_movimientos + 1))
				{
					reposicionarCaballo(i, j);
					imprimirTablero();
					fflush(stdout);
					sleep(1);
					system("cls");
					doMoves();
					return;
				}
			}
		} 
	}
}



void inicioJuego(int typeChosen)
{
	int i = 0;
	char j = 'x';
	int resultado = 0;
	
	
	
	imprimirTablero();
	printf("\n");
	printf("Fila de la posicion inicial del caballo \n");
	
	printf("Opciones validas: ");
	
	for (int i = 0; i < table.col_count; i++)
	{
		if (i == 0)
		{
			printf("( %i,", i + 1);
		}
		else if (i == table.col_count - 1)
		{
			printf(" %i ) \n", i + 1);
		}
		else
		{
			printf(" %i,", i + 1);
		}
	}
	
	printf("-> ");
	
	resultado = scanf("%d", &i);
	
	if (resultado != 1)
	{
		inicioJuego(1);
		return;
	}
	
	printf("\n Columna de la posicion inicial del caballo \n");
	
	printf("Opciones validas: ");
	
	int row_name = 'A';
	
	for (int i = 0; i < table.row_count; i++)
	{
		if (i == 0)
		{
			printf("( %c,", row_name);
		}
		else if (i == table.row_count - 1)
		{
			printf(" %c ) \n", row_name);
		}
		else
		{
			printf(" %c,", row_name);
		}
		
		row_name++;
	}
	
	printf("-> ");
	
	resultado = scanf(" %c", &j);
	
	if (resultado != 1)
	{
		inicioJuego(1);
		return;
	}
	
	j = toupper(j);
	
	if (checkIfValidRow(j) == -1)
	{
		inicioJuego(1);
		return;
	}
	
	int posJ = retornarPosSegunChar(j);
	
	posicionarCaballo(i - 1, posJ);
	
	/*if(i == 1) 
	{
	
	}
	else 
	{
		
	}*/
	
	
	guardarPosicionInicio(i - 1, posJ);
	
	printf("\n\nDigite un caracter para empezar el recorrido.\n");    
	
	printf("-> ");
	
	resultado = scanf(" %c", &j);
	
	cargarSolucion(solucion);
	
	system("cls");	
	imprimirTablero();
	
	fflush(stdout);
	sleep(1.5);
	
	system("cls");
	
	doMoves();
	
	imprimirTablero();
}

void iniciarPrograma()
{
	inicializarTablero();
	
	iniciarSolucion();
	
	inicioJuego(0);
	
	liberarMemoria();
	
	liberarSolucion();
}

//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	iniciarPrograma();
	return 0;
}

