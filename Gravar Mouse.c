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
            DrawText("GRAVANDO",683,384,20,RED);
            fprintf (inimigos, "%i %i\n",(int)mouse.x,(int)mouse.y);
        }
        EndDrawing();
    }
    fclose(inimigos);
    CloseWindow();
}
