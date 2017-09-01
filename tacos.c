#include"run.h"
#include<stdio.h>

int main(int argc, char *argv[])
{
  printf("###### TACOS ######\n");
  OnInit();
  while(running){
    if(acquire){
      OnEvent();
    }
    // On réarme la lecture des entrées
    acquire = 1;
    OnLoop();
    // printf("%i - %i \n" , P_Pos.x, P_Pos.y);
  }
  OnCleanUp();
  return 0;
}
