#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"saver.h"

int load(int  map[MAP_WIDTH][MAP_HEIGHT], char target[50]){
  FILE* fichier;
  int i, j;
  int read;
  printf("Ouverture de : %s \n", target);
  fichier = fopen(target, "r");
  if(fichier == NULL)
    printf("Erreur à l'ouverture \n");
  printf("Fichier ouvert \n");

  for(j = 0; j < MAP_HEIGHT; j++){
    for(i = 0; i < MAP_WIDTH; i++){
      read = fscanf(fichier, "%i\n", &(map[i][j]) );
    }
  }

  fclose(fichier);
  printf("Fichier lu !\n");
  return read;
}


int save(int map[MAP_WIDTH][MAP_HEIGHT], char dest[50]){
  FILE* fichier;
  int i, j;
  
  fichier = fopen(dest, "w");
  if(fichier == NULL)
    printf("Erreur à l'ouverture \n");
  printf("Fichier ouvert \n");

  for(j = 0; j < MAP_HEIGHT; j++){
    for(i = 0; i < MAP_WIDTH; i++){
      fprintf(fichier, "%i\n", map[i][j]);
    }
  }

  fclose(fichier);
  printf("Sauvegarde terminée !\n");
  return 0;
}
