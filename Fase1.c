#include "raylib.h"
#include <math.h>

#define MAX_TIROS 150
#define MAX_METEOROS 50
#define MAX_METEOR_SIZE 50
#define MIN_METEOR_SIZE 25
#define CHANCECONST 11

typedef struct LIGHT
{
    Rectangle Rec;
    bool active;
    float spd;
    Color color;
}LIGHT;

typedef struct METEOR
{
    Vector2 pos;
    float xspd;
    float yspd;
    float rad;
    float rotation;
    bool active;
}METEOR;

typedef struct PBULLET
{
    Vector2 pos;
    float spd;
    float rad;
    bool active;
}PBULLET;

typedef struct PLAYER

{
    Vector2 pos;
    float spd;
    float dmg;
    float hitbox;
    PBULLET bullet[MAX_TIROS];
    bool invincible;
    Color color;
}PLAYER;

static const int Largura_Tela = 720;
static const int Altura_Tela = 800;
static bool gameOver = false;
static bool gaming = true;
static int LightCounter = 0;
static float backgroundScroll = 0;
static int lives = 3;
static int iFrame = 0;
static int firerate = 5;
static int corshield=1;
Texture2D FundoSpace;
Texture2D Nave;
Texture2D Meteoro;

Sound BGM;
Sound Laser;

PLAYER player;
METEOR meteoros[MAX_METEOROS];
LIGHT light;

static void InitFase1(void);
static void UpdateFase1(void);
static void DrawFase1(void);


int main(void)
{
    InitWindow(Largura_Tela, Altura_Tela, "Teste Fase 1");
    InitAudioDevice();
    SetTargetFPS(60);
    SetMasterVolume(0.25);
    
    InitFase1();
    
    while(!gameOver && !WindowShouldClose())
    {
        UpdateFase1();
        DrawFase1();
    }
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        StopSound(BGM);
        ClearBackground(BLACK);
        if(lives<=0)
        {
            DrawText("Game Over\nDefeat", Largura_Tela/2 - 20*9, Altura_Tela/2,20 ,RED);
        } else 
        {
            DrawText("Game Over\nVictory", Largura_Tela/2 - 20*9, Altura_Tela/2,20 ,BLUE);
        }
        EndDrawing();
    }
}

void LightBarrier(float mult)
{
    static int LightFrame = 0;
    static int ShowFrame = 0;
    static bool cancolide = true;
    
    LightFrame++;
    
    if(LightFrame < 60*mult)
    {
        light.active = false;
    } else
    if(LightFrame == 60*mult)
    {
        light.Rec.y = 0;
        light.Rec.height = GetRandomValue(70, 80);
        light.spd = 40/mult;
        int temp = GetRandomValue(1, 3);
        if(temp == 1)
        {
            light.color = RED;
        } else 
        if(temp == 2)
        {
            light.color = GREEN;
        } else 
        {
            light.color = BLUE;
        }
    } else
    if(LightFrame < 60*2*mult)
    {
        if(ShowFrame>3)
        {
            ShowFrame = 0;
        }
        if(ShowFrame == 2 || ShowFrame == 3)
        {
            light.active = true;
        } else 
        {
            light.active = false;
        }
        ShowFrame++;
    } else 
    if(LightFrame == 60*2*mult)
    {
        light.active = true;
    } else 
    {
        light.Rec.y += light.spd;
        if(light.Rec.y > Altura_Tela)
        {
            light.active = false;
            LightFrame = 0;
            cancolide = true;
            LightCounter++;
            if(LightCounter>30)
            {
                gaming = false;
            }
        }
        
        if(CheckCollisionCircleRec(player.pos, player.hitbox, light.Rec) && light.active && ColorToInt(player.color) != ColorToInt(light.color) && !player.invincible && cancolide)
        {
            lives++;
            cancolide = false;
            if(lives <=0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void Tiro()
{
    if(firerate <8) firerate++;
    if(IsKeyDown(KEY_SPACE) && firerate >= 8)
    {
        firerate = 0;
        PlaySound(Laser);
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(player.bullet[i].active == false)
            {
                player.bullet[i].active = true;
                player.bullet[i].pos.x = player.pos.x;
                player.bullet[i].pos.y = player.pos.y - Nave.height/2;
                break;
            }
        }
        
    }
    
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(player.bullet[i].active)
        {
            player.bullet[i].pos.y -= player.bullet[i].spd;
            
            for(int j=0; j<MAX_METEOROS; j++)
            {
                if(meteoros[j].active && CheckCollisionCircles(player.bullet[i].pos, player.bullet[i].rad, meteoros[j].pos, meteoros[j].rad))
                {
                    player.bullet[i].active = false;
                    meteoros[j].rad -= player.dmg;
                    if(meteoros[j].rad <= 5)
                    {
                        meteoros[j].active = false;
                    }
                }
            }
            
            if(player.bullet[i].pos.y < 0)
            {
                player.bullet[i].active = false;
            }
        }
    }
}


void UpdateFase1(void)
{
    backgroundScroll += 3.0f;
    if(backgroundScroll > Altura_Tela) backgroundScroll = 0;
    if(!IsSoundPlaying(BGM)) PlaySound(BGM);
    
    if(IsKeyDown(KEY_W))
        player.pos.y -= player.spd;
    if(IsKeyDown(KEY_S))
        player.pos.y += player.spd;
    if(IsKeyDown(KEY_A))
        player.pos.x -= player.spd;
    if(IsKeyDown(KEY_D))
        player.pos.x += player.spd;
    
    if(IsKeyPressed(KEY_E))
    {
        corshield++;
        if (corshield>3){
            corshield=1;
        }
    }
    
    else if(IsKeyPressed(KEY_Q))
    {
        corshield--;
        if (corshield<1){
            corshield=3;
        }
    }
    
    switch(corshield){
        case 1:{
            player.color = RED;
            break;
        }
        case 2:{
            player.color = GREEN;
            break;
        }
        case 3:{
            player.color = BLUE;
            break;
        }
    }
    
    if(!gaming)
    {
        static int endcounter = 0;
        endcounter++;
        if(endcounter >= 90)
        {
            gameOver = true;
        }
    }
    
    if(player.pos.x - Nave.width/2 < 0)
        player.pos.x = Nave.width/2;
    if(player.pos.x + Nave.width/2 > Largura_Tela)
        player.pos.x = Largura_Tela - Nave.width/2;
    if(player.pos.y - Nave.height/2 < 0)
        player.pos.y = Nave.height/2;
    if(player.pos.y + Nave.height/2 > Altura_Tela)
        player.pos.y = Altura_Tela - Nave.height/2;
    
    Tiro();
    float input = 1+(LightCounter/30);
    
    if(gaming)
    {
        LightBarrier(input);
    }
    
    int chance = GetRandomValue(1, 100);
    if(chance <= CHANCECONST && gaming)
    {
        for(int i=0; i<MAX_METEOROS; i++)
        {
            if(!meteoros[i].active)
            {
                meteoros[i].active = true;
                meteoros[i].rad = (float)GetRandomValue(MIN_METEOR_SIZE, MAX_METEOR_SIZE);
                meteoros[i].pos.x = (float)GetRandomValue(-50, Largura_Tela + 50);
                meteoros[i].pos.y = (float)GetRandomValue(-60, -30);
                meteoros[i].rotation = (float)GetRandomValue(1, 30)/10.0f;
                float targetx = GetRandomValue(0, Largura_Tela);
                float targety = GetRandomValue(Altura_Tela/2, Altura_Tela);
                float tempvx = targetx-meteoros[i].pos.x;
                float tempvy = targety-meteoros[i].pos.y;
                float div = sqrt((tempvx*tempvx)+(tempvy*tempvy));
                float vel = GetRandomValue(8, 10);
                meteoros[i].xspd = tempvx*vel/div;
                meteoros[i].yspd = tempvy*vel/div;
                break;
            }
        }
    }
    
    if(player.invincible)
    {
        iFrame++;
        if(iFrame >= 90)
        {
            iFrame = 0;
            player.invincible = false;
        }
    }
    
    
    for(int i=0; i<MAX_METEOROS; i++)
    {
        if(meteoros[i].active)
        {
            meteoros[i].pos.x += meteoros[i].xspd;
            meteoros[i].pos.y += meteoros[i].yspd;
            meteoros[i].rotation +=meteoros[i].yspd;
            
            if(meteoros[i].pos.y - meteoros[i].rad > Altura_Tela)
            {
                meteoros[i].active = false;
            }
            
            
            /*Collision PHYSICAS sqn
            for(int j=0; j<MAX_METEOROS; j++)
            {
                if(i!=j && meteoros[j].active == true)
                {
                    if(CheckCollisionCircles(meteoros[i].pos, meteoros[i].rad, meteoros[j].pos, meteoros[j].rad))
                    {
                        //meteoros[i].pos.x -= meteoros[i].xspd;
                        //meteoros[i].pos.y -= meteoros[i].yspd;
                        meteoros[i].xspd *= -1;
                        meteoros[j].xspd *= -1;
                        
                    }
                }
            }*/
            
            if(CheckCollisionCircles(player.pos, player.hitbox, meteoros[i].pos, meteoros[i].rad) && !player.invincible)
            {
                lives--;
                if(lives<=0)
                {
                    gameOver = true;
                }
                player.invincible = true;
            }
        }
    }
}

void DrawFase1(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
        DrawTexture(FundoSpace, 0, backgroundScroll, RAYWHITE);
        DrawTexture(FundoSpace, 0, backgroundScroll - FundoSpace.height, RAYWHITE);
        if(!player.invincible)
        {
            DrawTexture(Nave, player.pos.x - Nave.width/2, player.pos.y - Nave.height/2, RAYWHITE);
        } else 
        {
            if(iFrame%3 == 0 || iFrame%4 == 0)
            {
                DrawTexture(Nave, player.pos.x - Nave.width/2, player.pos.y - Nave.height/2, RAYWHITE);
            }
        }
        //DrawCircle(player.pos.x, player.pos.y, player.hitbox, PINK);
        
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(player.bullet[i].active)
            {
                DrawCircleV(player.bullet[i].pos, player.bullet[i].rad, BLUE);
            }
        }
        
        DrawCircleV(player.pos, 5 + (Nave.height/2), Fade(player.color, 0.3));
        DrawCircleLines(player.pos.x, player.pos.y, 5 + (Nave.height/2), Fade(player.color, 0.7));
        
        
        for(int i=0; i<MAX_METEOROS; i++)
        {
            if(meteoros[i].active)
            {
                //DrawCircle(meteoros[i].pos.x, meteoros[i].pos.y, meteoros[i].rad, Fade(BROWN, 0.5));
                Vector2 Location = {meteoros[i].pos.x - meteoros[i].rad, meteoros[i].pos.y - meteoros[i].rad};
                DrawTextureEx(Meteoro, Location, 0,meteoros[i].rad*2/MAX_METEOR_SIZE, RAYWHITE);
            }
        }
        if(light.active)
        {
            DrawRectangleRec(light.Rec, Fade(light.color, 0.9));
        }
        DrawText(FormatText("%i", lives), 0, 0, 20, RAYWHITE);
    EndDrawing();
}

void InitFase1(void)
{
    Image tempnave = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    Image tempspace = LoadImage("/raylib/StarlightDrift/texture/space.png");
    Image tempmeteor = LoadImage("/raylib/StarlightDrift/texture/asteroid.png");
    
    ImageResize(&tempnave, 50, 60);
    ImageResize(&tempspace, Largura_Tela, Altura_Tela);
    ImageResize(&tempmeteor, MAX_METEOR_SIZE, MAX_METEOR_SIZE);
    
    FundoSpace = LoadTextureFromImage(tempspace);
    Nave = LoadTextureFromImage(tempnave);
    Meteoro = LoadTextureFromImage(tempmeteor);
    
    UnloadImage(tempmeteor);
    UnloadImage(tempnave);
    UnloadImage(tempspace);
    
    BGM = LoadSound("/raylib/StarlightDrift/sounds/fase1teste.mp3");
    Laser = LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    
    player.pos.x = Largura_Tela/2;
    player.pos.y = Altura_Tela/2;
    player.spd = 5.0;
    player.dmg = 7;
    player.color = BLUE;
    player.hitbox = 9;
    player.invincible = false;
    for(int i=0; i<MAX_TIROS; i++)
    {
        player.bullet[i].spd = 10;
        player.bullet[i].active = false;
        player.bullet[i].rad = 5;
    }
    
    for(int i=0; i<MAX_METEOROS; i++)
    {
        meteoros[i].active = false;
    }
    
    light.Rec.x = 0;
    light.Rec.width = Largura_Tela;
    light.active = false;
}
