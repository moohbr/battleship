#include<stdio.h>
#include<stdlib.h>

#define HASH_SIZE 32767
#define HASH_NUMBER 32771

unsigned int hash(int x, int y){
    int number = (x >= y) ? (x * x + x + y) % HASH_NUMBER : (y * y + x) % HASH_NUMBER;
    
    if (number < 0) number += HASH_NUMBER;
    
    
    if (number >= HASH_SIZE)  number %= HASH_SIZE;
    
    return (unsigned int) number;
}

typedef struct cell{
  int x;
  int y;
  struct cell * next;
}cell;

typedef cell * cell_ptr;

cell_ptr create_new_cell(int x, int y){
    cell_ptr new_cell = NULL;

    while (new_cell == NULL){
        new_cell = (cell_ptr)malloc(sizeof(cell));
    }

    new_cell->x = x;
    new_cell->y = y;
    new_cell->next = NULL;
    
    return new_cell;
}

void free_table(cell_ptr * table){
    for(int i = 0; i < HASH_SIZE; i++){
        cell_ptr current = table[i];

        while(current != NULL){
            cell_ptr next = current->next;
            free(current);
            current = next;
        }
    }
}

cell_ptr * create_table(){
    cell_ptr * table = NULL;

    while (table == NULL) {
        table = (cell_ptr *)malloc(HASH_SIZE * sizeof(cell_ptr));
    }

    for(int i = 0; i < HASH_SIZE; i++){
        table[i] = NULL;
    }
    
    return table;
}

cell_ptr * add_cell(cell_ptr * table, int x, int y){
    unsigned int index = hash(x, y);

    cell_ptr new_cell = create_new_cell(x, y);

    if(table[index] == NULL){
        table[index] = new_cell;

        return table;
    }

    cell_ptr current = table[index];

    while(current->next != NULL){
        current = current->next;
    }

    current->next = new_cell;

    return table;
}

int search_table(cell_ptr * table, int x, int y){
    unsigned int index = hash(x, y);
    cell_ptr current = table[index];
    while(current != NULL){
        if(current->x == x && current->y == y){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char * argv[]){
  int dimensao;
  int num_pontos;
  int num_tiros;

  cell_ptr * pontosJA = create_table();
  cell_ptr * pontosJB = create_table();


  scanf("%d %d %d", &dimensao, &num_pontos, &num_tiros);
  printf("Campo %d x %d\nCada jogador coloca %d barcos\n\n", dimensao, dimensao, num_pontos);
  
  srand(dimensao);

  printf("Jogador A coloca barcos:\n");
  for(int i = 0; i < num_pontos; i++){
    int x, y;
    do{
      x = rand() % dimensao;
      y = rand() % dimensao;
    }while(search_table(pontosJA, x, y) != 0); //TODO: Procura no oceano A se x, y ja tem barco
    
    if(i<5) printf("(%d, %d)\n", x, y);
    
    pontosJA = add_cell(pontosJA, x, y); //TODO: Adiciona x, y no oceano A
  }

  printf("...\n\nJogador B coloca barcos:\n");
  for(int i = 0; i < num_pontos; i++){
    int x, y;
    do{
      x = i % dimensao;
      y = rand() % dimensao;
    }while(search_table(pontosJB, x, y) != 0);
      
    if(i<5) printf("(%d, %d)\n", x, y);
    
    pontosJB = add_cell(pontosJB, x, y);
  }
  
  printf("...\n\nCada jogador vai dar %d tiros\n", num_tiros);
  
  
  int acertosJA = 0;
  printf("\nJogador A atira:\n");
  for(int i = 0; i < num_tiros; i++){
    int x = rand() % dimensao;
    int y = rand() % dimensao;
    if(i<5) printf("(%d, %d)\n", x, y);
    
    if(search_table(pontosJB, x, y) == 1) acertosJA++;
  }
  
  int acertosJB = 0;
  printf("...\n\nJogador B atira:\n");
  for(int i = 0; i < num_tiros; i++){
    int x = rand() % dimensao;
    int y = rand() % dimensao;
    if(i<5) printf("(%d, %d)\n", x, y);

    if(search_table(pontosJA, x, y) == 1) acertosJB++;
  }
  
  printf("...\n\nResultado: Jogador A acertou %d e Jogador B %d\n", acertosJA, acertosJB);

  free_table(pontosJA); free_table(pontosJB);
  return 0;
}