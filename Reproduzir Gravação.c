#include "raylib.h"
#include <stdio.h>

int main(void){
    FILE * inimigos = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","r");
    int a,b;
    SetTargetFPS(60);
    InitWindow(1366,768,"I Like Good Pussy and I Like Good Trees");
    ToggleFullscreen();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(ORANGE);
        fscanf (inimigos, "%i %i\n",&a,&b);
        DrawText("N",a,b,20,BLACK);
        EndDrawing();
    }
    fclose(inimigos);
    CloseWindow();
}
/*
nt main ()
{
   FILE * pFile;
   int n;
   char name [100];

   pFile = fopen ("myfile.txt","w");
   for (n=0 ; n<3 ; n++)
   {
     puts ("please, enter a name: ");
     gets (name);
     fprintf (pFile, "Name %d [%-10.10s]\n",n+1,name);
   }
   fclose (pFile);

   return 0;
}
*/
