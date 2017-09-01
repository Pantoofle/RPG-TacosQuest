#ifndef RUN_H
#define RUN_H

#include<SDL/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"constants.h"
#include"saver.h"
struct Map_Pos{
  int x;
  int y;
};
typedef struct Map_Pos Map_Pos;

/* Main loop */
int OnInit();
int OnEvent();
int OnLoop();
int OnCleanUp();

/* Autre fonctions */
Map_Pos Adjacent_Tile(Map_Pos pos, int direction);
Map_Pos Perso_Colision_Tile(SDL_Rect p, int direction);
int Box_Movable(Map_Pos p, int direction);
Map_Pos Move_Box_Perso(Map_Pos p_box, int direction);
Map_Pos Move_Item(Map_Pos p, int direction, int box);
void Draw_Map();
void TriggerBox(Map_Pos p);
void TriggerPerso();
void Teleporte();


/* Variables globales de la boucle */
int running;
int inMenu;
int inEditor;
int level;

/* Variables du jeu */
/* P = Perso;  B = Box */
Map_Pos P_Pos;
int P_Direction;
int map[MAP_WIDTH][MAP_HEIGHT];

int acquire;

char path[50];

/* Surfaces pour l'affichage */
SDL_Surface *Surfaces [NB_SURF];
SDL_Surface *Surf_Box;
SDL_Surface *Surf_Perso [4];
SDL_Surface *Surf_Actu;
SDL_Surface *Surf_Walk [4][2];
SDL_Surface *Surf_ecran;
SDL_Surface *Surf_Cursor;


#endif
