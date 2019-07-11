#include "raylib.h"
#include <stdio.h>

Vector2 mouse;

void Movimento() //Função de movimentação
{
    if(IsKeyDown('W'))
    {
        mouse.y -= 2;
    }
    if(IsKeyDown('A'))
    {
        mouse.x -= 2;
    }
    if(IsKeyDown('S'))
    {
        mouse.y += 2;
    }
    if(IsKeyDown('D'))
    {
        mouse.x += 2;
    }
    
}

int main(void){
    
    FILE * inimigos = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","w");
    int z=0;
    SetTargetFPS(60);
    InitWindow(720,876,"I Like Good Pussy and I Like Good Trees");
    //ToggleFullscreen();
    while(!WindowShouldClose()){
        Movimento();
        BeginDrawing();
        ClearBackground(ORANGE);
        DrawText(TextFormat("%i %i",(int)mouse.x,(int)mouse.y),200,200,20,BLUE);
        DrawCircle(mouse.x,mouse.y,1,BLUE);
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
