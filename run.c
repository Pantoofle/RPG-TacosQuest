#include"run.h"


int OnInit(){
  int i, j;
  char entry[50];

  /* Chargement de la carte */
  for(i = 0; i < MAP_WIDTH; i++){
    map[i][0] = T_WALL;
    map[i][MAP_HEIGHT -1] = T_WALL;
    for(j = 1; j < MAP_HEIGHT -1; j++){
      map[i][j] = T_FLOOR;
    }
  }
  for(j = 0; j < MAP_HEIGHT; j++){
    map[0][j] = T_WALL;
    map[MAP_WIDTH-1][j] = T_WALL;
  }

  printf("\nChoisir un niveau \n");

  printf("Editeur : 0\nChoisir un niveau : 1~12\nChoisir un niveau dans un fichier : -1\n");
  i = scanf("%i", &level);

  switch(level){
    
  case 0:
    inEditor = 1;
    printf("F1 = ENREGISTRER\nF2 = CHARGER\nf = floor\nb=  box\nw = wall\ne = entry\nx = exit\ni = ice\nh = hole\nt = teleporter\ng = gravel\nc = crack\n");
    break;
    
  case -1:
    strcpy(path, LVL_PATH);
    strcat(path, PERSO_DIR);
    printf("Nom du niveau dans le dossier levels/perso (sans le .xsb à la fin) : \n");
    i = scanf("%s", entry);
    strcat(path, entry);
    strcat(path, EXTENSION);
    load(map, path);
    break;
    
  default:
    strcpy(path, LVL_PATH);
    sprintf(entry, "%d", level);
    strcat(path, entry);
    strcat(path, EXTENSION);
    load(map, path);
    break;
  }



  
  /* INITIALISATION VIDEO */
  printf("Initialisation vidéo...");
  running = 1;
  Surf_ecran = NULL;
  
  if (SDL_Init(SDL_INIT_VIDEO) <0){
    fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
    return -1;
  }

  Surf_ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
  if(Surf_ecran == NULL){
    fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
    return -1;
  }
  SDL_WM_SetCaption("Block : The Tacos Quest - By Simon Fernandez", NULL);

  SDL_EnableKeyRepeat(50, MOVE_TIME);
  
  printf("Done ! \n");


  /* INITIALISATION DES OBJETS */
  printf("Initialisation des objets...");
  P_Pos.x = 3;
  P_Pos.y = 3;
  acquire = 1;

  // Recherche de l'entrée
  for(i = 0; i < MAP_WIDTH; i++){
    for(j = 0; j< MAP_HEIGHT; j++){
      if(  (map[i][j] % (1<<BITS_ID)) == ID_ENTRY){
        P_Pos.x = i;
        P_Pos.y = j;
      }
    }
  }
  
  P_Direction = DOWN;

  /* Chargement des surfaces  */
  for(i = 0; i < NB_SURF; i++){
    Surfaces[i] = SDL_LoadBMP("img/floor.bmp");
  }
  
  
  Surf_Box           = SDL_LoadBMP("img/box.bmp");
  
  Surfaces[ID_WALL]  = SDL_LoadBMP("img/wall.bmp");
  Surfaces[ID_FLOOR] = SDL_LoadBMP("img/floor.bmp");
  Surfaces[ID_EXIT]  = SDL_LoadBMP("img/exit.bmp");
  Surfaces[ID_ICE]   = SDL_LoadBMP("img/ice.bmp");
  Surfaces[ID_EXIT]  = SDL_LoadBMP("img/exit.bmp");
  Surfaces[ID_HOLE]  = SDL_LoadBMP("img/hole.bmp");
  Surfaces[ID_FILLED]= SDL_LoadBMP("img/filled.bmp");
  Surfaces[ID_ENTRY] = SDL_LoadBMP("img/entry.bmp");
  Surfaces[ID_TELE]  = SDL_LoadBMP("img/tele.bmp");
  Surfaces[ID_GLUE]  = SDL_LoadBMP("img/glue.bmp");
  Surfaces[ID_CRACK] = SDL_LoadBMP("img/crack.bmp");
  
  
  Surf_Perso[UP]     = SDL_LoadBMP("img/persoUP.bmp");
  Surf_Perso[DOWN]   = SDL_LoadBMP("img/persoDOWN.bmp");
  Surf_Perso[LEFT]   = SDL_LoadBMP("img/persoLEFT.bmp");
  Surf_Perso[RIGHT]  = SDL_LoadBMP("img/persoRIGHT.bmp");

  Surf_Actu = Surf_Perso[P_Direction];

  Surf_Cursor = SDL_LoadBMP("img/cursor.bmp");

  Surf_Walk[UP][0] = SDL_LoadBMP("img/walkUP1.bmp");
  Surf_Walk[UP][1] = SDL_LoadBMP("img/walkUP2.bmp");

  Surf_Walk[DOWN][0] = SDL_LoadBMP("img/walkDOWN1.bmp");
  Surf_Walk[DOWN][1] = SDL_LoadBMP("img/walkDOWN2.bmp");

  Surf_Walk[LEFT][0] = SDL_LoadBMP("img/walkLEFT1.bmp");
  Surf_Walk[LEFT][1] = SDL_LoadBMP("img/walkLEFT2.bmp");

  Surf_Walk[RIGHT][0] = SDL_LoadBMP("img/walkRIGHT1.bmp");
  Surf_Walk[RIGHT][1] = SDL_LoadBMP("img/walkRIGHT2.bmp");
  
  for(i = 0; i < 4; i++){
    SDL_SetColorKey(Surf_Perso[i], SDL_SRCCOLORKEY, SDL_MapRGB(Surf_Perso[i]->format, 0, 255, 0));
    SDL_SetColorKey(Surf_Walk[i][0], SDL_SRCCOLORKEY, SDL_MapRGB(Surf_Walk[i][0]->format, 0, 255, 0));
    SDL_SetColorKey(Surf_Walk[i][1], SDL_SRCCOLORKEY, SDL_MapRGB(Surf_Walk[i][1]->format, 0, 255, 0));
  }
  SDL_SetColorKey(Surf_Box, SDL_SRCCOLORKEY, SDL_MapRGB(Surf_Box->format, 0, 255, 0));
  SDL_SetColorKey(Surf_Cursor, SDL_SRCCOLORKEY, SDL_MapRGB(Surf_Cursor->format, 0, 255, 0));
  
  printf("Done ! \n");

  if(inEditor){
    Surf_Actu = Surf_Cursor;
  }

  printf("Tracé de la carte...");
  Draw_Map();
  printf("Done !\n");


  return 0;
}


int OnCleanUp(){
  int i;
  printf("Cleaning...");

  for (i = 0; i < NB_SURF; i++ ){
    SDL_FreeSurface(Surfaces[i]);
  }

  for(i = 0; i < 4; i++){
    SDL_FreeSurface(Surf_Perso[i]);
    SDL_FreeSurface(Surf_Walk[i][0]);
    SDL_FreeSurface(Surf_Walk[i][1]);
  }

  SDL_FreeSurface(Surf_Cursor);

  SDL_Quit();
  printf("Done !\n");
  return 0;
}

int OnEvent(){
  int stay, i = 0;
  SDL_Event event;
  char entry[50];
  
  do {
    SDL_WaitEvent(&event);
    stay = 0;
    switch(event.type)
      {
      case(SDL_QUIT):
        running = 0;
        break;
      case SDL_KEYDOWN:

        switch(event.key.keysym.sym)
          {
          case SDLK_ESCAPE :
            running = 0;
            break;
          case SDLK_UP:
            P_Direction = UP;
            break;
          case SDLK_DOWN:
            P_Direction = DOWN;
            break;
          case SDLK_LEFT:
            P_Direction = LEFT;
            break;
          case SDLK_RIGHT:
            P_Direction = RIGHT;
            break;

          case SDLK_f:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_FLOOR;
              Draw_Map();
            }
            stay = 1;
            
            break;
            
          case SDLK_i:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_ICE;
              Draw_Map();
            }
            stay = 1;
            
            break;

            
          case SDLK_b:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] |= BOXED;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_e:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_ENTRY;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_x:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_EXIT;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_c:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_CRACK;
              Draw_Map();
            }
            stay = 1;
            break;

            
          case SDLK_t:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_TELE;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_g:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_GLUE;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_h:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_HOLE;
              Draw_Map();
            }
            stay = 1;
            break;
  

            
          case SDLK_w:
            if(inEditor){
              map[P_Pos.x][P_Pos.y] = T_WALL;
              Draw_Map();
            }
            stay = 1;
            break;

          case SDLK_F1:
            if(inEditor){
              strcpy(path, LVL_PATH);
              strcat(path, PERSO_DIR);
              printf("Nom du niveau à enregistrer (sans le .xsb à la fin) : \n");
              i = scanf("%s", entry);
              strcat(path, entry);
              strcat(path, EXTENSION);
              save(map, path);
            }
            stay = 1;
            break;

          case SDLK_F2:
            if(inEditor){
              strcpy(path, LVL_PATH);
              strcat(path, PERSO_DIR);
              printf("Nom du niveau à charger (sans le .xsb à la fin) : \n");
              i = scanf("%s", entry);
              strcat(path, entry);
              strcat(path, EXTENSION);
              load(map, path);
              Draw_Map();
            }
            stay = 1;
            break;
            
          default:
            stay = 1;
            break;
          }
        break;

        
      default:
        stay = 1;
        break;
      }
  } while (stay == 1);

  if(inEditor == 0)
    Surf_Actu = Surf_Perso[P_Direction];
  
  return i;
}

int OnLoop(){
  Map_Pos target, box;
  int tile;
 
  /* Traitement des collisions */
  /* Déclenchement des mouvements */
  target = Adjacent_Tile(P_Pos, P_Direction);
  tile = map[target.x][target.y];
  
  if((tile & BOXED) && (inEditor == 0)){
    // S'il y a une boite là ou on va
    if (Box_Movable(target, P_Direction)){
      // Si on peut la bouger on deplace le perso et la boite
      box = Move_Box_Perso(target, P_Direction);

      if(map[P_Pos.x][P_Pos.y] & TRIGGER)
        TriggerPerso();

      if(map[box.x][box.y] & TRIGGER){
        TriggerBox(box);
      }
    }
    else {
      // Sinon on ne bouge pas
      Draw_Map();
    }
  }
  else if( ((tile & FREE) == 0) && (inEditor == 0) ){
    Draw_Map();
  }
  else {
    // Si la case est libre
    // 0 car on bouge le perso. Pas une box
    Move_Item(P_Pos, P_Direction, 0);

    if(map[P_Pos.x][P_Pos.y] & TRIGGER)
      if(inEditor == 0)
        TriggerPerso();
  }
  
  return 0;
}

void TriggerPerso(){
  switch(map[P_Pos.x][P_Pos.y] % (1<<BITS_ID) ){
  case ID_EXIT:
    printf("\nVICTOIRE ! \nRelancez le jeu et attaquez vous à un niveau plus dur !\n");
    running = 0;
    break;

  case ID_ICE:
    acquire = 0;
    break;

  case ID_TELE:
    Teleporte();
    break;

  case ID_GLUE:
    map[P_Pos.x][P_Pos.y] = T_FLOOR;
    break;

  case ID_CRACK:
    map[P_Pos.x][P_Pos.y] = T_HOLE;
    break;
  }
}

void Teleporte(){
  int target;
  int i, j;
  Map_Pos dest;

  dest = P_Pos;
  
  for(i = 0; i < MAP_WIDTH; i++){
    for(j = 0; j< MAP_HEIGHT; j++){
      target = map[i][j] % (1<<BITS_ID);
      if(target == ID_TELE){
        if((P_Pos.x != i) || (P_Pos.y != j)){
          dest.x = i;
          dest.y = j;
          printf("Destination trouvée !\n");
        }
      }
    }
  }
  printf("Zap !\n");
  P_Pos = dest;
  Draw_Map();
}

void TriggerBox(Map_Pos p){
  switch(map[p.x][p.y] % (1<<BITS_ID) ){
  case ID_EXIT:
    break;

  case ID_ICE:
    if(Box_Movable(p, P_Direction)){
      // Le 1 dit que c'est une box
      p = Move_Item(p, P_Direction, 1);
      TriggerBox(p);
    }
    break;

  case ID_HOLE:
    map[p.x][p.y] = T_FILLED;
    Draw_Map();
    break;

  case ID_CRACK:
    map[p.x][p.y] = T_FILLED;
    Draw_Map();
    break;
  }
}



Map_Pos Move_Box_Perso(Map_Pos p_box, int direction){
  SDL_Rect perso, box, persoInit, boxInit, target;
  SDL_Surface *s_init, *s_medium, *s_target;
  Map_Pos targetMap, mediumMap;
  int i, id;
  
  perso.x = P_Pos.x * TILES_SIZE;
  perso.y = P_Pos.y * TILES_SIZE;
  box.x = p_box.x * TILES_SIZE;
  box.y = p_box.y * TILES_SIZE;

  // Positions initiales, pour tracer le sol avant de Bliter
  persoInit.x = perso.x;
  persoInit.y = perso.y;
  boxInit.x = box.x;
  boxInit.y = box.y;

  targetMap = Adjacent_Tile(p_box, direction);
  target.x = targetMap.x * TILES_SIZE;
  target.y = targetMap.y * TILES_SIZE;

  mediumMap = Adjacent_Tile(P_Pos, direction);

  id = (map[P_Pos.x][P_Pos.y] % (1<<BITS_ID) );
  s_init = Surfaces[id];

  id = (map[mediumMap.x][mediumMap.y] % (1<<BITS_ID) );
  s_medium = Surfaces[id];
  
  id = (map[targetMap.x][targetMap.y] % (1<<BITS_ID) );
  s_target = Surfaces[id];
  
  for (i = 0; i < TILES_SIZE; i++){
    // Mouvement de marche

    if(inEditor == 0){
      if(i % 8 == 0){
        if(i % TILES_SIZE/2 == 0){
          Surf_Actu = Surf_Perso[P_Direction];
        }
        else if(i < TILES_SIZE/2){
          Surf_Actu = Surf_Walk[P_Direction][0];
        }
        else {
          Surf_Actu = Surf_Walk[P_Direction][1];
        }
      }
    }

    
    // Tracé des sols
    SDL_BlitSurface(s_init, NULL, Surf_ecran, &persoInit);
    SDL_BlitSurface(s_medium, NULL, Surf_ecran, &boxInit);
    SDL_BlitSurface(s_target, NULL, Surf_ecran, &target);

    // Tracé des perso et boite en mouvement
    SDL_BlitSurface(Surf_Box, NULL, Surf_ecran, &box);
    SDL_BlitSurface(Surf_Actu, NULL, Surf_ecran, &perso);

    switch(direction){
    case UP:
      perso.y--;
      box.y--;
      break;

    case DOWN:
      perso.y++;
      box.y++;
      break;
      
    case RIGHT:
      perso.x++;
      box.x++;
      break;
      
    case LEFT:
      perso.x--;
      box.x--;
      break;
    }
    SDL_Flip(Surf_ecran);
    SDL_Delay(MOVE_TIME/TILES_SIZE);
  }

  if(inEditor == 0)
    Surf_Actu = Surf_Perso[P_Direction];

  // MAJ de la carte
  P_Pos.x = p_box.x;
  P_Pos.y = p_box.y;
  map[targetMap.x][targetMap.y] |= BOXED;
  map[p_box.x][p_box.y] ^= BOXED; 
  Draw_Map();

  return targetMap;
}

Map_Pos Move_Item(Map_Pos p, int direction, int box){
  SDL_Rect item, itemInit, target;
  Map_Pos targetMap;
  SDL_Surface *s_init, *s_target, *s_item;
  
  int id;
  int i;
  
  item.x = p.x * TILES_SIZE;
  item.y = p.y * TILES_SIZE;

  // Positions initiales, pour tracer le sol avant de Bliter
  itemInit.x = item.x;
  itemInit.y = item.y;

  targetMap = Adjacent_Tile(p, direction);
  target.x = targetMap.x * TILES_SIZE;
  target.y = targetMap.y * TILES_SIZE;

  id = (map[p.x][p.y] % (1<<BITS_ID) );
  s_init = Surfaces[id];
  
  id = (map[targetMap.x][targetMap.y] % (1<<BITS_ID) );
  s_target = Surfaces[id];

  if(box){
    s_item = Surf_Box; 
  }
  else {
    s_item = Surf_Actu;
  }
  
  for (i = 0; i < TILES_SIZE; i++){
    // Mouvement de marche
    if(inEditor == 0){
      if(i % 8 == 0){
        if(i % TILES_SIZE/2 == 0){
          Surf_Actu = Surf_Perso[P_Direction];
        }
        else if(i < TILES_SIZE/2){
          Surf_Actu = Surf_Walk[P_Direction][0];
        }
        else {
          Surf_Actu = Surf_Walk[P_Direction][1];
        }
      }
    }

    if(box == 0)
      s_item = Surf_Actu;

    
    // Tracé des sols
    SDL_BlitSurface(s_init, NULL, Surf_ecran, &itemInit);
    SDL_BlitSurface(s_target, NULL, Surf_ecran, &target);

    // Tracé du perso
    SDL_BlitSurface(s_item, NULL, Surf_ecran, &item);

    switch(direction){
    case UP:
      item.y--;
      break;

    case DOWN:
      item.y++;
      break;
      
    case RIGHT:
      item.x++;
      break;
      
    case LEFT:
      item.x--;
      break;
    }
    SDL_Flip(Surf_ecran);
    SDL_Delay(MOVE_TIME/TILES_SIZE);
  }

  if(inEditor == 0)
    Surf_Actu = Surf_Perso[P_Direction];

  if(box){
    // On ajoute une box à l'arrivée
    map[targetMap.x][targetMap.y] |= BOXED;
    // On supprime l'ancienne
    map[p.x][p.y] ^= BOXED;
  }
  else {
    P_Pos.x = targetMap.x;
    P_Pos.y = targetMap.y;  
  }
  
  Draw_Map();
  return targetMap;
}



Map_Pos Adjacent_Tile(Map_Pos pos, int direction){
  Map_Pos p;
  p.x = pos.x;
  p.y = pos.y;
  
  switch (direction){
  case UP:
    p.y--;
    break;
  case RIGHT:
    p.x++;
    break;
  case DOWN:
    p.y++;
    break;
  case LEFT:
    p.x--;
    break;
  }
  return p;
}

int Box_Movable(Map_Pos p, int direction){
  Map_Pos pos;
  int target;
  pos = Adjacent_Tile( p, direction);
  target = map[pos.x][pos.y];
  return (  ( (target & FREE) || (target & FILLABLE))
            && ((target & BOXED) == 0)
            && ((target & NO_BOX) == 0));
}

void Draw_Map(){
  SDL_Rect p;
  int i, j;
  SDL_Surface *Surf_local;
  int tile, id;
  
  p.x = 0;
  p.y = 0;

  /* On met tout en noir */
  SDL_FillRect(Surf_ecran, NULL, SDL_MapRGB(Surf_ecran->format, 0, 0, 0));

  /* Et on trace chaque tile */
  for(i = 0; i< MAP_WIDTH; i++){
    for(j = 0; j < MAP_HEIGHT; j++){
      tile = map[i][j];
      
      // On garde juste les 8 premiers bits, codant l'id de la surface
      id = tile % (1<<BITS_ID);
      Surf_local = Surfaces[id];
      
      SDL_BlitSurface(Surf_local, NULL, Surf_ecran, &p);
      
      if(tile & BOXED){
        SDL_BlitSurface(Surf_Box, NULL, Surf_ecran, &p);
      }
      p.y += TILES_SIZE;
    }
    p.y = 0;
    p.x += TILES_SIZE;
  }

  // On trace le perso
  p.x = P_Pos.x * TILES_SIZE;
  p.y = P_Pos.y * TILES_SIZE;

  SDL_BlitSurface(Surf_Actu, NULL, Surf_ecran, &p);
  SDL_Flip(Surf_ecran);
}

