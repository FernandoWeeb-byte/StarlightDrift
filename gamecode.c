#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//-------------------------------------
//defines
//-------------------------------------


#define MAX_TIROS 150
#define MAX_METEOROS 50
#define MAX_METEOR_SIZE 50
#define MIN_METEOR_SIZE 25
#define CHANCECONST 11
#define MAX_TIROS 150
#define VIDAS_INIMIGO 5

//-------------------------------------
//Structs
//-------------------------------------

typedef enum {MENU=0, JOGO, CREDITOS, OPCOES, MORTE, SAIR} GAMESTATE;


typedef struct Musica{
    Sound mus;
    bool ativa;
}Musica;

typedef struct Tiro
{
    Vector2 posicao;
    Vector2 vel_bala;
    Color cor;
    bool ativa;
    int raio;
}Tiro;

typedef struct LIGHT
{
    Rectangle Rec;
    bool active;
    float spd;
    Color color;
}LIGHT;

typedef struct PBULLET
{
    Vector2 pos;
    float spd;
    float raio;
    Color cor;
    bool active;
}PBULLET;

typedef struct METEOR
{
    Vector2 pos;
    float xspd;
    float yspd;
    float rad;
    float rotation;
    bool active;
}METEOR;

typedef struct Player
{
    Vector2 pos;
    float spd;
    float dmg;
    Color color;
    float hitbox;
    float firerate;
    PBULLET bullet[MAX_TIROS];
    int tiro;
    bool invincible;
}Player;

typedef struct Inimigo    //********************************************************************************************************************
{                         //****************************************************************************************************************
    Vector2 posicao;      //****************************************************************************************************************
    Vector2 vel;          //****************************************************************************************************************
    float raio;           //****************************************************************************************************************
    float chancedetiro;   //****************************************************************************************************************
    int tiro;             //****************************************************************************************************************
    int vida;             //****************************************************************************************************************
    int contador;         //****************************************************************************************************************
    bool ativo;
   
}Inimigo;


//-------------------------------------
//Variaveis Globais
//-------------------------------------
static const int Largura_Tela = 720;
static const int Altura_Tela = 876;
static bool gameOver = false;
static bool gaming = true;
static int LightCounter = 0;
static float backgroundScroll = 0;
static int lives = 3;
static int iFrame = 0;
static int firerate = 5;
static int corshield = 1;
static float movbackground;
static Texture2D Nave;
static Texture2D fundo;
static Texture2D FFXV;
static Texture2D Meteoro;
static Texture2D FundoSpace;
static Texture2D luacristal;
static Texture2D Fundolua;
static Texture2D enemi;
static GAMESTATE gameState;
static FILE* inimigo[20];
static int numMusica = 0;
static Musica musica[10];
static Sound menu;
static Sound Laser;
static Sound triste;
static Sound BGM;
static Inimigo foe[20]={0};//struct inimigo declarada aqui
static int vida;
static int iFrame;
static int counter;

static int fase = 1;

static Player player;
static METEOR meteoros[MAX_METEOROS];
static LIGHT light;
static Tiro atiradorinimigo[MAX_TIROS];
//static Music musicas;
static float vol = 1;


//-------------------------------------
//Funcoes Modulares Locais
//-------------------------------------
static void InitMovBackground(void);
static void ChoiceMusic(void);
static void InitMusic(void);
static void InitGame(void);     //Iniciar o jogo
static void UnloadGame(void);   //Descarregar arquivos do jogo
static void Movimento(void);
static void Inicializa_tiro(void);
static void Inicializa_jogador(void);
static void InitMenu(void);     //Iniciar arquivos do menu 
static void UnloadMenu(void);   //Descarregar arquivos do menu
static void Pause(void);    //pausa o jogo ;
static GAMESTATE MenuScreen(void);   //Menu inicial
static GAMESTATE Creditos(void);     //Creditos
static GAMESTATE Ops(void);          //Opções
static GAMESTATE morte(void);
static GAMESTATE Fase1(void);
static GAMESTATE Fase2(void);
static void InitFase1(void);
static void UpdateFase1(void);
static void DrawFase1(void);
static void UpdateFase2(void);   //Atualiza a matematica do frame
static void DrawFase2(void);
static void LightBarrier(float mult);
static void Atirar(void); //
static void TiroInimigo(void);//tentativa de fzr o tiro do inimigo
//static void LoadArq(void); //loada os arq
//static void Wave1(FILE* lado1,FILE* lado2); //wave de teste
//static void UnloadArq(void);//unloada arq



int main(void)
{
    gameState=MENU;
    InitWindow(Largura_Tela, Altura_Tela, "Starlight Drift Limpo");
    InitAudioDevice();
    menu = LoadSound("/raylib/StarlightDrift/sounds/Main_Menu.mp3");
    triste = LoadSound("/raylib/StarlightDrift/sounds/naruto.mp3");
    
    SetTargetFPS(60);
    while(1)
    {
        switch(gameState)
        {
            case MENU:
                InitMenu();
                gameState = MenuScreen();
                UnloadMenu();
            break;
            
            case JOGO:
                InitGame();
                if(fase == 1)
                {
                    gameState = Fase1();
                }
                else if (fase == 2)
                {
                    gameState = Fase2();
                }
                UnloadGame();
            break;
            case CREDITOS:
                gameState = Creditos();
            break;
            
            case OPCOES:
                gameState = Ops();
            break;
            
            case MORTE:
                gameState = morte();
                break;
                
            case SAIR:
                exit(0);
            break;
        }
    }
    return 0;
}

void InitMenu(void)
{
    //Imagens
    
    Image luadecristal = LoadImage("/raylib/StarlightDrift/texture/luatransparente.png");
    Image fundodecristal = LoadImage("/raylib/StarlightDrift/texture/fundosemluacopiacortada.png");
    Image FFXVdecristal = LoadImage("/raylib/StarlightDrift/texture/imagempqp.png");
    
    ImageResize(&fundodecristal,1140,1752);
    ImageResize(&FFXVdecristal,720,465);
    ImageResize(&luadecristal,150,150);
    
    FFXV = LoadTextureFromImage(FFXVdecristal);
    luacristal = LoadTextureFromImage(luadecristal);
    Fundolua = LoadTextureFromImage(fundodecristal);

    UnloadImage(FFXVdecristal);
    UnloadImage(fundodecristal);
    UnloadImage(luadecristal);
    
}

void UnloadMenu(void)
{
    UnloadTexture(FFXV);
    UnloadTexture(luacristal);
    UnloadTexture(Fundolua);
}

/*void LoadArq(void)
{
    FILE* lado1 = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","r");
    FILE* lado2 = fopen ("/raylib/StarlightDrift/enemy/padrao2.txt","r");
}
void UnloadArq(void)
{
    fclose(lado1);
    fclose(lado2);
}*/
int Wave1()
{
    
    for(int i = 0;i<6;i++)
    {
        if(foe[i].ativo)
        {
            fscanf (inimigo[i], "%f %f\n",&foe[i].posicao.x,&foe[i].posicao.y);
        }
    }
    
    
    for(int i=0;i<6;i++)
    {
        if(foe[i].ativo)
        {
            DrawTexture(enemi,foe[i].posicao.x,foe[i].posicao.y,RAYWHITE);
        
        }
    }    
        return counter;
    
    
}

int wave2()
{
    for(int i=6;i<11;i++)
    {
        if(foe[i].ativo)
        {
            fscanf (inimigo[i], "%f %f\n",&foe[i].posicao.x,&foe[i].posicao.y);
        }    
    }
    for(int i=6;i<11;i++)
    {   
        if(foe[i].ativo)
        {
            DrawTexture(enemi,foe[i].posicao.x,foe[i].posicao.y,RAYWHITE);
            if(foe[i].posicao.y>=Altura_Tela || foe[i].posicao.x<0 || foe[i].posicao.x>720)
            {
                counter++;
                foe[i].ativo = false;
            }
        }
    }
    
    for(int i=6;i<11;i++)
    {
        if(foe[i].ativo)
        {
            foe[i].raio=20;
        }
    }

    
    for(int j=6;j<11;j++)
    {
        if(foe[j].ativo)
        {
                foe[j].chancedetiro=GetRandomValue(0,100);
            if(foe[j].chancedetiro<=20)
            {
                foe[j].contador += 5;
                
                if(foe[j].contador == 20)
                {
                    
                        
                            for(int i = 0;i<MAX_TIROS;i++)
                            {
                                if(!atiradorinimigo[i].ativa)
                                {
                                    atiradorinimigo[i].posicao.x = foe[j].posicao.x + 20;
                                    atiradorinimigo[i].posicao.y = foe[j].posicao.y + 16;
                                    atiradorinimigo[i].ativa = true;
                                    break;
                                }
                            }
                        
                        
                    foe[j].contador = 0;
                }
            }
        }
        
    }
    
    return counter;
    
}

int wave3()
{
    
 for(int i=11;i<18;i++)
    {
        if(foe[i].ativo)
        {
            fscanf (inimigo[i], "%f %f\n",&foe[i].posicao.x,&foe[i].posicao.y);
        }
    }
    for(int i=11;i<18;i++)
    {   
        if(foe[i].ativo)
        {
            DrawTexture(enemi,foe[i].posicao.x,foe[i].posicao.y,RAYWHITE);
        
        }
    }
    
    
    for(int i=11;i<18;i++)
    {
        if(foe[i].ativo)
        {
            foe[i].raio=20;
        }
    }

    
    for(int j=11;j<18;j++)
    {
        if(foe[j].ativo)
        {
            foe[j].chancedetiro=GetRandomValue(0,100);
            if(foe[j].chancedetiro<=20)
            {
                foe[j].contador += 5;
                
                if(foe[j].contador == 20)
                {
                    
                        
                            for(int i = 0;i<MAX_TIROS;i++)
                            {
                                if(!atiradorinimigo[i].ativa)
                                {
                                    atiradorinimigo[i].posicao.x = foe[j].posicao.x + 20;
                                    atiradorinimigo[i].posicao.y = foe[j].posicao.y + 16;
                                    atiradorinimigo[i].ativa = true;
                                    break;
                                }
                            }
                        
                        
                    foe[j].contador = 0;
                }
            }
        }
        
    }
       
    return counter;
}
void Inicializa_jogador(void)
{
    player.pos.x = 360;
    player.pos.y = 700;
    player.spd = 5;
    player.color = BLUE;
    player.firerate = 0;
    player.hitbox = 9;
    player.dmg = 7;
    player.tiro = 1;
    player.invincible = false;
}
void Inicializa_inimigo(void)
{
    for(int i=0;i<6;i++)
    {
        foe[i].ativo = true;
                
    }
    
    for(int i=0;i<6;i++)
    {
        if(foe[i].ativo)
        {
            foe[i].raio=20;
        }
    }
     for(int i=0;i<19;i++)
    {
        foe[i].vida = VIDAS_INIMIGO;  
    }
    
}
void InitGame(void){
    StopSound(menu);
    InitMusic();
    
    Image NaveImg = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    Laser = LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    
    ImageResize(&NaveImg,50,60);
   
    Nave = LoadTextureFromImage(NaveImg);
    
    Image enemy = LoadImage("/raylib/StarlightDrift/texture/inimigo.png");
    ImageResize(&enemy,40,50);
    enemi = LoadTextureFromImage(enemy);
    UnloadImage(enemy);
    vida = 3;
    InitMovBackground();
    Inicializa_jogador();
    Inicializa_tiro();
    Inicializa_inimigo();
    
    
    UnloadImage(NaveImg);
}
void InitMovBackground(void){
    Image FundodeFernando = LoadImage("/raylib/StarlightDrift/texture/space.png");
    
    ImageResize(&FundodeFernando,720,876);
    
    fundo=LoadTextureFromImage(FundodeFernando);
    
    UnloadImage(FundodeFernando);
}

void Inicializa_tiro(void)
{
    for(int i = 0;i<MAX_TIROS;i++)
    {
        player.bullet[i].pos.x = player.pos.x;
        player.bullet[i].pos.y = player.pos.y + 8;
        player.bullet[i].spd = 10;
        player.bullet[i].raio = 5;
        player.bullet[i].active = false;
        player.bullet[i].cor = BLUE;
    }
    
    for(int i = 0;i<MAX_TIROS;i++)
    {
        atiradorinimigo[i].posicao.x = foe[0].posicao.x;
        atiradorinimigo[i].posicao.y = foe[0].posicao.y + 8;
        atiradorinimigo[i].vel_bala.x = 2;
        atiradorinimigo[i].vel_bala.y = 15;
        atiradorinimigo[i].raio = 3;
        atiradorinimigo[i].ativa = false;
        atiradorinimigo[i].cor = YELLOW;
    }
}


void UnloadGame(void){
    UnloadTexture(Nave);
    UnloadTexture(fundo);
    if (musica[numMusica].ativa){
        StopSound(musica[numMusica].mus);
        UnloadSound(musica[numMusica].mus);
    }

}
void UpdateFase2(void){
    if(!musica[numMusica].ativa){
        ChoiceMusic();
    }
    movbackground += 3.0; //velocidade do background
    if(movbackground >= fundo.height) {
        movbackground = 0; //looping do background
    }
    
    Movimento();
    Atirar();
    TiroInimigo();
    if(counter == 6)
    {
        for(int i=6;i<11;i++)
        {
            foe[i].ativo = true;
        }
    }
    if(counter == 11)
    {
        for(int i=11;i<18;i++)
        {
            foe[i].ativo = true;
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
    
     for(int i = 0;i<18;i++)
    {
        for(int j = 0;j<MAX_TIROS;j++)
        {
            if(player.bullet[j].active && CheckCollisionCircles(player.bullet[j].pos,player.bullet[j].raio,(Vector2){foe[i].posicao.x+20,foe[i].posicao.y+25},foe[i].raio) && foe[i].ativo)
            {
                
                   foe[i].vida--;
                    player.bullet[j].active = false;
                    
                    if(foe[i].vida == 0)
                    {
                        foe[i].ativo = false;
                        counter++;
                    }
                
                
            }
        }
    }
    
    for(int i =0;i<18;i++)
    {
        for(int j = 0;j<MAX_TIROS;j++)
        {
                if(atiradorinimigo[j].ativa && CheckCollisionCircles(atiradorinimigo[j].posicao,atiradorinimigo[j].raio,(Vector2){player.pos.x+20,player.pos.y+25},player.hitbox) && !player.invincible)
            {
            
                vida--;
                atiradorinimigo[j].ativa=false;
                player.invincible = true;
                    
                
            }
        }
    }   
        
    for(int i=0;i<18;i++)
    {
            if(foe[i].ativo && CheckCollisionCircles(player.pos,player.hitbox,foe[i].posicao,foe[i].raio) && !player.invincible)
         {
                vida--;
                foe[i].ativo = false;
                player.invincible = true;
                
                
        }
    }
        

}
void DrawFase2(void){
    ClearBackground(BLACK);
    DrawTextureEx(fundo,(Vector2){0,movbackground},0.0f,1.0f,WHITE);
    DrawTextureEx(fundo,(Vector2){0,-fundo.height + movbackground},0.0f,1.0f,WHITE);
    DrawText(FormatText("contador: %i", counter), 100, 100, 30, WHITE);
    if(!player.invincible)
    {
        DrawTexture(Nave,player.pos.x,player.pos.y,RAYWHITE);
    }    
    else 
        {
            if(iFrame%3 == 0 || iFrame%4 == 0)
            {
                DrawTexture(Nave,player.pos.x,player.pos.y , RAYWHITE);
            }
        }
    
    
    
    for(int i = 0;i<MAX_TIROS;i++)
    {
        if(player.bullet[i].active)
        {
            DrawCircle(player.bullet[i].pos.x,player.bullet[i].pos.y,player.bullet[i].raio,player.bullet[i].cor);
        }
        if(player.bullet[i].pos.y <=0)
        {
            player.bullet[i].active = false;
        }
    }
    
    for(int i = 0;i<MAX_TIROS;i++)
    {
        if(atiradorinimigo[i].ativa)
        {
            atiradorinimigo[i].posicao.y += atiradorinimigo[i].vel_bala.y;
            DrawCircle(atiradorinimigo[i].posicao.x,atiradorinimigo[i].posicao.y,3,atiradorinimigo[i].cor);
        }
        if(atiradorinimigo[i].posicao.y >=Altura_Tela)
        {
            atiradorinimigo[i].ativa = false;
        }
    }
    DrawText(TextFormat("VIDAS:%i",vida),0,0,50,BLUE);
}

void Atirar(void)
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
                if(fase == 1)
                {
                    player.bullet[i].pos.x = player.pos.x;
                }
                else if (fase == 2)
                {
                    player.bullet[i].pos.x = player.pos.x + 20;
                }
                player.bullet[i].pos.y = player.pos.y;
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
                if(meteoros[j].active && CheckCollisionCircles(player.bullet[i].pos, player.bullet[i].raio, meteoros[j].pos, meteoros[j].rad))
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

void Movimento(void)
{
    if(IsKeyDown('W') && player.pos.y >= player.hitbox)
        player.pos.y -= player.spd;
    if(IsKeyDown('A') && player.pos.x >= player.hitbox)
        player.pos.x -= player.spd;
    if(IsKeyDown('S') && player.pos.y <= Altura_Tela - 45)
        player.pos.y += player.spd;
    if(IsKeyDown('D') && player.pos.x <= Largura_Tela - 45)
        player.pos.x += player.spd;
}

void TiroInimigo(void)
{
    for(int j=0;j<6;j++)
    {
        if(foe[j].ativo)
        {
                foe[j].chancedetiro=GetRandomValue(0,100);
            if(foe[j].chancedetiro<=20)
            {
                foe[j].contador += 5;
                
                if(foe[j].contador == 20)
                {
                    
                        
                            for(int i = 0;i<MAX_TIROS;i++)
                            {
                                if(!atiradorinimigo[i].ativa)
                                {
                                    atiradorinimigo[i].posicao.x = foe[j].posicao.x + 20;
                                    atiradorinimigo[i].posicao.y = foe[j].posicao.y + 16;
                                    atiradorinimigo[i].ativa = true;
                                    break;
                                }
                            }
                        
                            
                    
                        
                        
                    foe[j].contador = 0;
                }
            }
        }
        
    }
            
}


void Pause(void)
{   
   
        
        while(1)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PAUSE",150,50,50,RAYWHITE);
            DrawText("Para voltar pressione P",70,150,20,RAYWHITE);
            DrawText("Para desistir pressione M durante o jogo",70,250,20,RAYWHITE);
            EndDrawing();
            if(IsKeyPressed(KEY_P))
                break;
        }
    
}
void InitMusic(void){
    for (int i=0;i<10;i++){
        musica[i].ativa=false;
    }
    numMusica=2;
}
void ChoiceMusic(void){
    if (musica[numMusica].ativa){
        UnloadSound(musica[numMusica].mus);
    }
    numMusica=GetRandomValue(2,9);
    switch (numMusica){
        case 2:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Hatsune Miku - Melt PV.mp3");
        musica[numMusica].ativa=true;
        break;
        case 3:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/One Punch Man - Official Opening - The Hero!! Set Fire to the Furious Fist.mp3");
        musica[numMusica].ativa=true;
        break;
        case 4:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Dragon Ball Super OST - Ka Ka Kachi Daze  LYRICS  ULTRA INSTINCT THEME.mp3");
        musica[numMusica].ativa=true;
        break;
        case 5:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Death Note Opening 1 completo full HD.mp3");
        musica[numMusica].ativa=true;
        break;
        case 6:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Chopin  Etude No.2 in A minor Op10-2 on guitar.mp3");
        musica[numMusica].ativa=true;
        break;
        case 7:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/10 - Faraway 380,000-Kilometer Voyage.mp3");
        musica[numMusica].ativa=true;
        break;
        case 8:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/01 - The Space Shrine Maiden Appears.mp3");
        musica[numMusica].ativa=true;
        break;
        case 9:
        musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/07 - Eternal Spring Dream.mp3");
        musica[numMusica].ativa=true;
        break;
        
    }
    PlaySound(musica[numMusica].mus);
}

GAMESTATE Creditos(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    Font fonte = GetFontDefault();
    Rectangle Rec = {3, 3, Largura_Tela-3, Altura_Tela-3};
    Rectangle RecSair = {320, Altura_Tela-70, 80, 40};
    Vector2 PosicaoMouse;
    
    if(!IsSoundPlaying(menu)) PlaySound(menu);
    
    char texto[] = "Game Developers:\nJoão Victor Galdino\nJosé Rodrigues Neto\nLucas Fernandes Lins\nLuiz Fernando Barbosa\nMatheus Felipe Lima\n\nThis game was developed as a test for the introduction programming class by students of the Rural Federal University of Pernambuco (UFRPE) with the supervision of Professor Péricles Miranda\n\nThis game is not meant to be commercialized in any way whatsoever. We DO NOT own any of the images or sounds used in this game. Any similarities between the events portraited in this game and the real world are purely coincidental.\n\nSpecial Thanks to @raysan5, creator of Raylib\n\nSpecial Thanks to the coffee that was converted into this game's lines of code\n\n\nHomem Negro Fodase.";
    
    while(1)
    {
    PosicaoMouse = GetMousePosition();
    if(FadeIn)
    {
        alpha -= 0.01f;
        if(alpha<=0)
        {
            alpha = 0;
            FadeIn = false;
        }
    } else
    if(FadeOut)
    {
        alpha += 0.01f;
        if (alpha >= 1)
        {
            alpha = 1.0f;
            return MENU;
        }
    }
        
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTextRec(fonte, texto, Rec, 20, 3, true, RAYWHITE);
        if(CheckCollisionPointRec(PosicaoMouse, RecSair))
        {
            DrawText("Sair", 320, Altura_Tela-70, 40, LIGHTGRAY);
            if(IsMouseButtonPressed(0))
            {
                FadeOut = true;
            }
        } else 
        {
            DrawText("Sair", 335, Altura_Tela-60, 25, GRAY);
        }
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
    EndDrawing();
    }
}

GAMESTATE Fase1(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    
    SetMasterVolume(0.25);
    
    InitFase1();
    
    while(1)
    {
        if(IsKeyPressed('M'))
        {  
            gameOver = true;
            FadeOut = true;
        }
        if(IsKeyPressed('P'))
            Pause();
        if(FadeIn)
        {
            alpha -= 0.01f;
            if(alpha<=0)
            {
                alpha = 0;
                FadeIn = false;
            }
        } else
        if(FadeOut)
        {
            alpha += 0.01f;
            if (alpha >= 1)
            {
                counter = 0;
                alpha = 1.0f;
                
                if(gameOver)
                {
                    return MORTE;
                }
                if(LightCounter>30)
                {
                    fase++;
                    return JOGO;
                }
            }
        }
        
        if(!gameOver)
        {
            BeginDrawing();
            UpdateFase1();
            DrawFase1();
            DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
            EndDrawing();
        }
        else
        {
            StopSound(BGM);
            FadeOut = true;
        }
        
        if(LightCounter > 30)
        {
            player.invincible = true;
            FadeOut = true;
        }
        
    }
    

    
}

GAMESTATE Fase2(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    
    //LoadArq();
    inimigo[0] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","r");
    inimigo[1] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt2","r");
    inimigo[2] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt3","r");
    inimigo[3] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt4","r");
    inimigo[4] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt5","r");
    inimigo[5] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt6","r");
    inimigo[6] = fopen ("/raylib/StarlightDrift/enemy/padraoduplo1.txt","r");
    inimigo[7] = fopen ("/raylib/StarlightDrift/enemy/padraoduplo2.txt","r");
    inimigo[8] = fopen ("/raylib/StarlightDrift/enemy/padraolongo.txt","r");
    inimigo[9] = fopen ("/raylib/StarlightDrift/enemy/padraozig1.txt","r");
    inimigo[10] = fopen ("/raylib/StarlightDrift/enemy/padraozig2.txt","r");
    inimigo[11] = fopen ("/raylib/StarlightDrift/enemy/wave3-1.txt","r");
    inimigo[12] = fopen ("/raylib/StarlightDrift/enemy/wave3-2.txt","r");
    inimigo[13] = fopen ("/raylib/StarlightDrift/enemy/wave3-3.txt","r");
    inimigo[14] = fopen ("/raylib/StarlightDrift/enemy/wave3-4.txt","r");
    inimigo[15] = fopen ("/raylib/StarlightDrift/enemy/wave3-5.txt","r");
    inimigo[16] = fopen ("/raylib/StarlightDrift/enemy/wave3-6.txt","r");
    inimigo[17] = fopen ("/raylib/StarlightDrift/enemy/wave3-7.txt","r");
    
    
    while(1)
    {
        if(IsKeyPressed('M'))
        {  FadeOut = true;
           
        }
        if(IsKeyPressed('P'))
            Pause();
        if(FadeIn)
        {
            alpha -= 0.01f;
            if(alpha<=0)
            {
                alpha = 0;
                FadeIn = false;
            }
        } else
        if(FadeOut)
        {
            alpha += 0.01f;
            if (alpha >= 1)
            {
                counter = 0;
                alpha = 1.0f;
                return MENU;
            }
        }
        
        
        UpdateFase2();
        BeginDrawing();
        DrawFase2();
        //Wave1();
        if(Wave1() >= 6)
        {
            
            if(wave2() >= 10)
            {
                if(wave3() >= 17)
                {   
                    
                    
                }
            }
        }
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
        // UnloadArq();
        for(int i=0;i<11;i++)
        {
            fclose(inimigo[i]);
        }
        if(vida <= 0)
        {
         counter = 0;
          return MORTE;
        }
    }   
   
   
}

GAMESTATE Ops(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    Rectangle RecSair = {320, Altura_Tela-70, 80, 40};
    Vector2 PosicaoMouse;
    
    if(!IsSoundPlaying(menu)) PlaySound(menu);
    
    while(1)
    {
        PosicaoMouse = GetMousePosition();
        
        if(FadeIn)
        {
            alpha -= 0.01f;
            if(alpha<=0)
            {
                alpha = 0;
                FadeIn = false;
            }
        } else
        if(FadeOut)
        {
            alpha += 0.01f;
            if (alpha >= 1)
            {
                alpha = 1.0f;
                return MENU;
            }
        }
        if(IsKeyPressed(KEY_SPACE))
        {
            vol += 0.25f;
            if(vol>1)
            {
                vol = 0.0f;
            }
            SetMasterVolume(vol);
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(FormatText("Aperte Espaço para alterar o volume\nVolume atual: %.f%%", 100*vol), 174, (Altura_Tela/2)-20, 20, RAYWHITE);
            if(CheckCollisionPointRec(PosicaoMouse, RecSair))
            {
                DrawText("Sair", 320, Altura_Tela-70, 40, LIGHTGRAY);
                if(IsMouseButtonPressed(0))
                {
                    FadeOut = true;
                }
            } else 
            {
                DrawText("Sair", 335, Altura_Tela-60, 25, GRAY);
            }
            DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
    }
}


GAMESTATE morte(void)
{
    //triste = LoadSound("/raylib/StarlightDrift/sounds/naruto.mp3");
    bool fade = false;
    bool fadein = true;
    Rectangle recsair = {415, 620, 85, 25};
    Rectangle recretry = {415, 570, 135, 25};
    float alpha = 1.0;
    GAMESTATE returnstate;
    Vector2 posicaoMouse = {0};
    PlaySound(triste);
    
    while(1)
    {
    if(fade)
        {
            alpha+=0.01f;
            if(alpha>=1)
            {
                alpha = 1.0f;
                return returnstate;
            }
        
        }
    else if(fadein)
        {
            alpha -= 0.01f;
            if(alpha<=0)
            {
                alpha = 0;
                fadein = false;
            }
        }
        posicaoMouse = GetMousePosition();
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("homem negro fodase", 100, 300, 60, RED);
        
        if(CheckCollisionPointRec(posicaoMouse,recsair))
        {
            DrawText("sair",405, 610,40,LIGHTGRAY);
            if(IsMouseButtonDown(0))
            {
                StopSound(triste);
                //UnloadSound(triste);
                fade = true;
                returnstate = MENU;
            }
        }
        else
        {
            DrawText("sair",405, 610,40,GRAY);
        }
        
        if(CheckCollisionPointRec(posicaoMouse,recretry))
        {
            DrawText("retry",405, 560, 40, LIGHTGRAY);
            if(IsMouseButtonDown(0))
            {
                StopSound(triste);
                //UnloadSound(triste);
                fade = true;
                returnstate = JOGO;
            }
        }
        else
        {
            DrawText("retry",405, 560, 40,GRAY);
        }
        
        
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
        
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
    
    if(IsKeyPressed(KEY_Q))
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
    
    /*if(!gaming)
    {
        static int endcounter = 0;
        endcounter++;
        if(endcounter >= 90)
        {
            gameOver = true;
        }
    }*/
    
    if(player.pos.x - Nave.width/2 < 0)
        player.pos.x = Nave.width/2;
    if(player.pos.x + Nave.width/2 > Largura_Tela)
        player.pos.x = Largura_Tela - Nave.width/2;
    if(player.pos.y - Nave.height/2 < 0)
        player.pos.y = Nave.height/2;
    if(player.pos.y + Nave.height/2 > Altura_Tela)
        player.pos.y = Altura_Tela - Nave.height/2;
    
    Atirar();
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
                DrawCircleV(player.bullet[i].pos, player.bullet[i].raio, BLUE);
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
    
    LightCounter = 0;
    lives = 3;
    gameOver = false;
    
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
        player.bullet[i].raio = 5;
    }
    
    for(int i=0; i<MAX_METEOROS; i++)
    {
        meteoros[i].active = false;
    }
    
    light.Rec.x = 0;
    light.Rec.width = Largura_Tela;
    light.active = false;
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
            lives--;
            cancolide = false;
            if(lives <=0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}


GAMESTATE MenuScreen(void)
{
    /*-------------------------------------------------------------------------------------
    FUNCIONAMENTO DO FADE IN/FADE OUT:
    Basicamente o Fade In/Fade Out precisa de:
    2 Variaveis booleanas (FadeIn, FadeOut)
    1 float (alpha)
    1 DrawRectangle
    
    Quando entramos em uma "tela" a gente declara as variaveis assim
    FadeIn = true; <-Porque sempre começaremos no FadeIn
    FadeOut = false; <-FadeOut so se torna true quando formos sair da tela
    alpha = 1;
    DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha)); <-Isso é basicamente o que vai fazer o fade
                                                                        nos vamos no final do desenho (por cima de todos os outros Draws), 
                                                                        desenhar um retangulo preto
    A função Fade é uma função do Raylib que pega uma cor e atribui transparencia a ela, alpha é a variavel float q controla a transparencia
    quando alpha == 0 a cor é 100% transparente, quando alpha é 1.0 a cor é 100% opaca
    
    Então a ideia é dentro do loop principal vc ter uma função mais ou menos no estilo:
    
    if(FadeIn)
    {
        alpha -= 0.01f; <-O quanto mudaremos alpha por frame, pelo o que eu vi algo entre 0.01 e 0.02 fica ok
        if(alpha <0)
        {
            alpha = 0; <- Só pra evitar problemas de valores fora do escopo
            FadeIn = false; <- Como alpha chegou em 0 o FadeIn foi concluido, então n precisamos mais dele
        }
    } else <- É bom colocar num if else if pra evitar bugs doidos de fazer FadeIn e FadeOut ao msm tempo, vai que né?
    if(FadeOut)
    {
        alpha += 0.01f; <- Mesma ideia, mas agr ja que estamos saindo nos fazemos o quadrado ficar mais opaco para a tela escurecer
        if(alpha>1)
        {
            alpha = 1.0f;
            FadeOut = false;
            
            *codigo do fim da função. Return ou algo assim* <- Nesse ponto é util colocar o codigo pra finalizar a função, ja que o FadeOut foi concluido
        }
    }
    
    Então no fim do Drawing a gente vai ter a função DrawRectangleRec
    
    -------------------------------------------------------------------------------------*/
    
    //Estado inicial das variaveis do menu
    bool fade = false;
    bool fadeIn = true;
    Rectangle BordaFundo = {0, 0, Largura_Tela, Altura_Tela};
    Vector2 PosicaoFundo = {0};
    Vector2 PosicaoMouse = {0};
    float VelFundoX = 0.3f;
    float VelFundoY = 0.356f;
    int frameCounter = 0;
    Rectangle RecGame = {415, 570, 135, 25};          //Retangulos para checar o mouse
    Rectangle RecCreditos = {415, 620, 85, 25};
    Rectangle RecOpcoes = {415, 670, 65, 25};
    Rectangle RecSair = {415, 720, 45, 25};
    GAMESTATE returnstate;
    float alpha = 1.0f;
    
    if(!IsSoundPlaying(menu)) PlaySound(menu);
    
    while(1)
    {
        //Maths
        
        PosicaoMouse = GetMousePosition();
        BordaFundo.x += VelFundoX;
        BordaFundo.y += VelFundoY;
        frameCounter++;
        if(frameCounter == 1400)
        {
            VelFundoX *= -1;
            VelFundoY *= -1;
            frameCounter = 0;
        }
        if(fade)
        {
            alpha +=0.01f;
            if(alpha>=1)
            {
                alpha = 1.0f;
                return returnstate;
            }
        } else 
        if(fadeIn)
        {
            alpha-=0.01f;
            if(alpha<=0)
            {
                alpha = 0.0f;
                fadeIn = false;
            }
        }
    
        //Draws
        
        BeginDrawing();
            DrawTextureRec(Fundolua,BordaFundo,PosicaoFundo,RAYWHITE);
            DrawTexture(FFXV,-40,-20,RAYWHITE);
            DrawTexture(luacristal,400,370,RAYWHITE);
            DrawText(FormatText("Mouse: %.f, %.f\nFrame: %i", PosicaoMouse.x, PosicaoMouse.y, frameCounter), 0, 0, 20, RAYWHITE); //debug
            if(CheckCollisionPointRec(PosicaoMouse, RecGame))
            {
                DrawText("Novo Jogo",400,560,40,LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    fade = true;
                    returnstate = JOGO;
                }
            } else 
            {
                DrawText("Novo Jogo",415,570,25,GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecCreditos))
            {
                DrawText("Créditos",400,610,40,LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    fade = true;
                    returnstate = CREDITOS;
                }
            } else 
            {
                DrawText("Créditos",415,620,25,GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecOpcoes))
            {
                DrawText("Opções",400,660, 40,LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    fade = true;
                    returnstate = OPCOES;
                }
            } else 
            {
                DrawText("Opções", 415, 670, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecSair))
            {
                DrawText("Sair",400,710,40,LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    fade = true;
                    returnstate = SAIR;
                }
            } else 
            {
                DrawText("Sair",415,720,25,GRAY);
            }
            DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
    }
}
 
