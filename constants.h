#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAP_WIDTH 23
#define MAP_HEIGHT 9
#define TILES_SIZE 32
#define WIDTH MAP_WIDTH*TILES_SIZE
#define HEIGHT MAP_HEIGHT*TILES_SIZE

#define MOVE_TIME 250

#define NB_SURF 10

#define LVL_PATH "levels/"
#define PERSO_DIR "perso/"
#define EXTENSION ".xsb"


/* Constantes definissant le comportement d'une case.
/  Codage sur les bits d'un int.
/  Les 8 bits de poids faible codent la case (majoritairement pour l'affichage)  
/  Les 8 bits suivants codent le comportement de la case 
*/

#define BITS_ID 8

#define ID_FLOOR  (0)
#define ID_WALL   (1)
#define ID_EXIT   (2)
#define ID_ENTRY  (3)
#define ID_ICE    (4)
#define ID_HOLE   (5)
#define ID_FILLED (6)
#define ID_TELE   (7)
#define ID_GLUE   (8)
#define ID_CRACK  (9)

#define BOXED (1<<8)
#define FREE (1<<9)
#define FILLABLE (1<<10)
#define TRIGGER (1<<11)
#define NO_BOX  (1<<12)

/* Definition des differents blocs */
#define T_FLOOR  (ID_FLOOR | FREE)
#define T_WALL   (ID_WALL)
#define T_EXIT   (ID_EXIT | FREE | TRIGGER | NO_BOX)
#define T_ENTRY  (ID_ENTRY | FREE)
#define T_ICE    (ID_ICE | FREE | TRIGGER)
#define T_HOLE   (ID_HOLE | FILLABLE | TRIGGER)
#define T_FILLED (ID_FILLED | FREE)
#define T_TELE   (ID_TELE | FREE | NO_BOX | TRIGGER)
#define T_GLUE   (ID_GLUE | FREE | NO_BOX | TRIGGER)
#define T_CRACK   (ID_CRACK | FREE | FILLABLE | TRIGGER)

/* Enum Direction */
enum {UP, DOWN, LEFT, RIGHT};


#endif
