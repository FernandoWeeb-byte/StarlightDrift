#include "raylib.h"
#include <stdio.h>

int main(void){
    Vector2 mouse;
    FILE * inimigos = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","w");
    int z=0;
    SetTargetFPS(60);
    InitWindow(1366,768,"I Like Good Pussy and I Like Good Trees");
    ToggleFullscreen();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(ORANGE);
        mouse=GetMousePosition();
        DrawText(TextFormat("%i %i",(int)mouse.x,(int)mouse.y),mouse.x-35,mouse.y+20,20,BLUE);
        if (IsKeyPressed(KEY_G)){
            z++;
        }
        if (z>1){
            z=0;
        }
        if (z){
            fprintf (inimigos, "%i %i\n",(int)mouse.x,(int)mouse.y);
        }
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
