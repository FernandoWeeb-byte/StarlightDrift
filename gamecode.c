#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

//-------------------------------------
//defines
//-------------------------------------


#define MAX_TIROS 150
#define MAX_METEOROS 50
#define MAX_METEOR_SIZE 50
#define MIN_METEOR_SIZE 25
#define CHANCECONST 11
#define MAX_TIROS 150
#define VIDAS_INIMIGO 34

//-------------------------------------
//Structs
//-------------------------------------

typedef enum {MENU=0, JOGO, CREDITOS, OPCOES, MORTE, SAIR,WIN} GAMESTATE;


typedef struct Musica{
    Sound mus;
    bool ativa;
}Musica;

typedef struct BBULLET
{
    Vector2 pos;
    bool active;
    float raio;
    float xspd;
    float yspd;
    float rad;
    float def;
}BBULLET;

typedef struct BOSS
{
    Vector2 pos;
    Vector2 npos;
    Vector2 spd;
    float hp;
    float hitbox;
    float ang;
    int phase;
}BOSS;

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
static int lives;
static int iFrame = 0;
static int firerate = 5;
static int corshield = 1;
static int skillpoints = 5;
static float movbackground;
static Texture2D Nave;
static Texture2D fundo;
static Texture2D FFXV;
static Texture2D Meteoro;
static Texture2D FundoSpace;
static Texture2D luacristal;
static Texture2D Fundolua;
static Texture2D enemi;
static Texture2D pericles;
static GAMESTATE gameState;
static FILE* inimigo[32];
static int numMusica = 0;
static Musica musica[10];
static Sound menu;
static Sound Laser;
static Sound triste;
static Sound bossbgm;
static Sound BGM;
static Inimigo foe[32]={0};//struct inimigo declarada aqui
static int vida;
static int iFrame;
static int counter;
static int parte_dial = 1; 
static bool vidainf=false;
static bool goodmusic=false;
static bool dial = true;
static int LightFrame = 0;
static int fase = 1;
static int actk=0;
static int actx=0;
static int acty=0;
static int actz=0;
static float bonusbspd = 0;
static float bonusspd = 0;
static float bonusd = 0;
static float bonush = 0;
static Player player;
static METEOR meteoros[MAX_METEOROS];
static LIGHT light;
static Tiro atiradorinimigo[MAX_TIROS];
//static Music musicas;
static float vol = 1;
static BOSS Pericles;
static BBULLET bbullet[MAX_TIROS];
static int Player_Up = 'W';   
static int Player_Down= 'S'; 
static int Player_Left= 'A'; 
static int Player_Right= 'D';
static int Player_TeclaTiro= KEY_SPACE;
static int Player_R= 'R';
static int Player_G= 'G';
static int Player_B= 'B';
static int Player_ColorUp= 'Q';  
static int Player_ColorDown= 'E';


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
static GAMESTATE Fase3(void);
static GAMESTATE Victory(void);
static void InitFase1(void);
static void UpdateFase1(void);
static void DrawFase1(void);
static void UpdateFase2(void);   //Atualiza a matematica do frame
static void DrawFase2(void);
static void InitFase3(void);
static void UpdateFase3(void);
static void DrawFase3(void);
static void LightBarrier(float mult);
static void Atirar(void); //
static void TiroInimigo(void);//tentativa de fzr o tiro do inimigo
static void Cheats(void);
static void SkillPoints (void);
static void Dialogo(void);
static void Pattern1(void);
static void Pattern2(void);
static void Pattern3(void);
static void BossMov(void);
int main(void)
{
    gameState=MENU;
    InitWindow(Largura_Tela, Altura_Tela, "Final Fantasy CC");
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
                else if (fase == 3)
                {
                    gameState = Fase3();
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
                
            case WIN:
                gameState = Victory();
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
    dial = true;
    fase = 1;
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

int wave4()
{
   for(int i=18;i<24;i++)
    {
        if(foe[i].ativo)
        {
            fscanf (inimigo[i], "%f %f\n",&foe[i].posicao.x,&foe[i].posicao.y);
        }
    }
    for(int i=18;i<24;i++)
    {   
        if(foe[i].ativo)
        {
            DrawTexture(enemi,foe[i].posicao.x,foe[i].posicao.y,RAYWHITE);
        
        }
    }
    
    
    for(int i=18;i<24;i++)
    {
        if(foe[i].ativo)
        {
            foe[i].raio=20;
        }
    }

    
    for(int j=18;j<24;j++)
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

int wave5()
{
    for(int i=24;i<32;i++)
    {
        if(foe[i].ativo)
        {
            fscanf (inimigo[i], "%f %f\n",&foe[i].posicao.x,&foe[i].posicao.y);
        }
    }
    for(int i=24;i<32;i++)
    {   
        if(foe[i].ativo)
        {
            DrawTexture(enemi,foe[i].posicao.x,foe[i].posicao.y,RAYWHITE);
        
        }
    }
    
    
    for(int i=24;i<32;i++)
    {
        if(foe[i].ativo)
        {
            foe[i].raio=20;
        }
    }

    
    for(int j=24;j<32;j++)
    {
        if(foe[j].ativo)
        {
                foe[j].chancedetiro=GetRandomValue(0,100);
            if(foe[j].chancedetiro<=11)
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
void Dialogo()
{
    switch(parte_dial)
    {
        case 1:
        {
            DrawRectangle(100,50,520,300,BLACK);
            DrawText(TextFormat("Você está se aproximando de um cinturão de asteroides!\nCuidado com as defesas de luz do inimigo! Troque de cor para passar\nileso\nPressione %c ou %c para trocar de cor ou\n%c para vermelho, %c para azul e %c para verde\n%c - Movimento para cima\n%c - Movimento para baixo\n%c - Movimento para direita\n%c - Movimento para esquerda\nBarra de espaço - Atirar\n\nPressione espaço para continuar",Player_ColorUp,Player_ColorDown,Player_R,Player_B,Player_G,Player_Up,Player_Down,Player_Right,Player_Left),110,70,15,WHITE);
            
            if(IsKeyPressed(KEY_SPACE))
            {
                dial = false;
                parte_dial++;
                SkillPoints();
            }
            break;
        }
        case 2:
        {
            DrawRectangle(100,50,520,140,BLACK);
            DrawText(TextFormat("Seus inimigos detectaram você e enviaram um batalhão para lhe parar!\nDesvie de seus ataques e derrote-os para prosseguir\n\n\nAperte espaço para continuar"),110,70,15,WHITE);
                        if(IsKeyPressed(KEY_SPACE))
            {
                dial = false;
                parte_dial++;

            }
            break;
        }
        case 3:
        {
            DrawRectangle(100,50,520,230,BLACK);
            DrawText(TextFormat("CUIDADO!"),240,70,50,RED);
            DrawText(TextFormat("Seus inimigos enviaram a sua nave mais poderosa!\nA 'Python Emperor Reaper of Imperative Computer Languages\nEnhanced Starship'         AKA P.E.R.I.C.L.E.S\n\nEsta é sua última batalha! Aperte espaço para continuar"),110,150,15,WHITE);
            if(IsKeyPressed(KEY_SPACE))
            {
                dial = false;
                parte_dial++;
                
            }
            break;
        }
    }
}
void Inicializa_jogador(void)
{
    player.pos.x = 360;
    player.pos.y = 700;
    player.spd = 5+bonusspd;
    player.color = BLUE;
    player.firerate = 0;
    player.hitbox = 9-bonush;
    player.dmg = 7+bonusd;
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
     for(int i=0;i<32;i++)
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
    lives = 3;
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
        player.bullet[i].spd = 10+bonusbspd;
        player.bullet[i].raio = 5+bonusd*0.8f;
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
    for(int i=0;i<32;i++)
        {
            fclose(inimigo[i]);
        }

}
void Cheats(void){
    
    while(1){

            if (IsKeyPressed(KEY_ONE)){
                actx++;
            }
            if(actx>1){
                actx=0;
            }
            if (IsKeyPressed(KEY_TWO)){
                acty++;
            }
            if(acty>1){
                acty=0;
            }
            if (IsKeyPressed(KEY_THREE)){
                actz++;
            }
            if (actz>1){
                actz=0;
            }
            if (IsKeyPressed(KEY_FOUR)){
                actk++;
            }
            if (actk>1){
                actk=0;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("CHEATS",250,10,50,RED);
            DrawText("um lugar q vc prova q vc é ruim e n consegue passar de fase, seu merda",100,70,15,RED);
          
          
            if (actx){
                vidainf=true;
                DrawText("1 - Vidas infinitas",30,140,30,GREEN);
            }
            else{
                vidainf=false;
                DrawText("1 - Vidas infinitas",30,140,30,WHITE);
            }
            if (acty){
                goodmusic=true;
                DrawText("2 - Música melhora em 304954.3%",30,180,30,GREEN);
            }
            else{
                goodmusic=false;
                DrawText("2 - Música melhora em 304954.3%",30,180,30,WHITE);
            }
            if (actz){
                skillpoints=500;
                DrawText("3 - SkillPoints p carai",30,220,30,GREEN);
            }
            else{
                skillpoints=5;
                DrawText("3 - SkillPoints p carai",30,220,30,WHITE);
            }
         
            
            if (actk){
                bonusbspd=10.0f;
                bonusspd=5.0f;
                bonush=8.9f;
                bonusd=27.0f;
                
                DrawText("4 - MAX stats",30,260,30,GREEN);
            }
            
            else{
                bonusbspd=0.0f;
                bonusspd=0.0f;
                bonush=0.0f;
                bonusd=0.0f;
                DrawText("4 - MAX stats",30,260,30,WHITE);
            }
            
            
            EndDrawing();
            if(IsKeyPressed(KEY_F1)){
                
                break;
            }
    }
}
void UpdateFase2(void){
    if(!IsSoundPlaying(musica[numMusica].mus)){
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
    if(counter == 18)
    {
        for(int i=18;i<24;i++)
        {
            foe[i].ativo = true;
        }
    }
    if(counter == 24)
    {
        for(int i=24;i<32;i++)
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
    
     for(int i = 0;i<32;i++)
    {
        for(int j = 0;j<MAX_TIROS;j++)
        {
            if(player.bullet[j].active && CheckCollisionCircles(player.bullet[j].pos,player.bullet[j].raio,(Vector2){foe[i].posicao.x+20,foe[i].posicao.y+25},foe[i].raio) && foe[i].ativo)
            {
                
                    foe[i].vida-=player.dmg;
                    player.bullet[j].active = false;
                    
                    if(foe[i].vida <= 0)
                    {
                        foe[i].ativo = false;
                        counter++;
                    }
                
                
            }
        }
    }
    
    for(int i =0;i<32;i++)
    {
        for(int j = 0;j<MAX_TIROS;j++)
        {
                if(atiradorinimigo[j].ativa && CheckCollisionCircles(atiradorinimigo[j].posicao,atiradorinimigo[j].raio,(Vector2){player.pos.x+20,player.pos.y+25},player.hitbox) && !player.invincible)
            {
                if (!vidainf){
                    vida--;
                }
                
                atiradorinimigo[j].ativa=false;
                player.invincible = true;
                    
                
            }
        }
    }   
        
    for(int i=0;i<32;i++)
    {
            if(foe[i].ativo && CheckCollisionCircles(player.pos,player.hitbox,foe[i].posicao,foe[i].raio) && !player.invincible)
         {
                if (!vidainf){
                    vida--;
                }
                counter++;
                foe[i].ativo = false;
                player.invincible = true;
                
                
        }
    }
        

}
void DrawFase2(void){
    ClearBackground(BLACK);
    DrawTextureEx(fundo,(Vector2){0,movbackground},0.0f,1.0f,WHITE);
    DrawTextureEx(fundo,(Vector2){0,-fundo.height + movbackground},0.0f,1.0f,WHITE);
    //DrawText(FormatText("contador: %i", counter), 100, 100, 30, WHITE);
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
    if(IsKeyDown(Player_TeclaTiro) && firerate >= 8)
    {
        firerate = 0;
        PlaySound(Laser);
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(player.bullet[i].active == false)
            {
                player.bullet[i].active = true;
                if(fase == 1 || fase == 3)
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
    if(IsKeyDown(Player_Up) && player.pos.y >= player.hitbox)
        player.pos.y -= player.spd;
    if(IsKeyDown(Player_Left) && player.pos.x >= player.hitbox)
        player.pos.x -= player.spd;
    if(IsKeyDown(Player_Down) && player.pos.y <= Altura_Tela - 45)
        player.pos.y += player.spd;
    if(IsKeyDown(Player_Right) && player.pos.x <= Largura_Tela - 45)
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

void SkillPoints (void){
    
        int velnave = 720,velbala=440,velbb=720;
        while (1){
            if (IsKeyPressed(KEY_ENTER)){
                for (int x=0;x<MAX_TIROS;x++){
                    player.bullet[x].spd+=bonusbspd;
                    player.bullet[x].raio+=bonusd*0.8;
                }
                player.dmg+=bonusd;
                player.spd+=bonusspd;
                player.hitbox-=bonush;
                break;
            }
            if(IsKeyPressed(KEY_F1)){
                Cheats();
            }
            Color a,b,c,d;
            BeginDrawing();
            ClearBackground(BLACK);
            Vector2 rato = GetMousePosition();
            DrawText(TextFormat("%i %i",(int)rato.x,(int)rato.y),0,0,20,WHITE);
            DrawText(TextFormat("SKILL POINTS: %i",skillpoints),150,50,50,WHITE);
            DrawText(TextFormat("Você recebeu alguns pontos de habilidade\n      Distribua-os conforme desejar!"),150,130,20,WHITE);
            DrawText("Velocidade das balas:",30,250,30,BLUE);
            DrawText("Velocidade da nave:",30,300,30,BLUE);
            DrawText("Tamanho da hitbox:",30,350,30,BLUE);
            DrawText("Dano das balas:",30,400,30,BLUE);
            if (bonusbspd>0){
                a=GREEN;
            }
            else{
                a=WHITE;
            }
            if (bonusspd>0){
                b=GREEN;
            }
            else{
                b=WHITE;
            }
            if (bonush>0){
                c=GREEN;
            }
            else{
                c=WHITE;
            }
            if (bonusd>0){
                d=GREEN;
            }
            else{
                d=WHITE;
            }
            DrawText(TextFormat("%.2f",10+bonusbspd),500,250,30,a);
            DrawText(TextFormat("%.2f",5+bonusspd),500,300,30,b);
            DrawText(TextFormat("%.2f",9-bonush),500,350,30,c);
            DrawText(TextFormat("%.2f",7+bonusd),500,400,30,d);
            DrawText("+",480,250,30,LIME);
            DrawText("+",480,300,30,LIME);
            DrawText("+",480,350,30,LIME);
            DrawText("+",480,400,30,LIME);
            DrawText("-",577,250,30,RED);
            DrawText("-",577,300,30,RED);
            DrawText("-",577,350,30,RED);
            DrawText("-",577,400,30,RED);
            DrawText(TextFormat("Pressione ENTER\n   para confirmar"),150,750,40,WHITE);
            if (rato.x>=480&&rato.x<=496&&rato.y>=254&&rato.y<=272&&bonusbspd<8&&IsMouseButtonPressed(0)&&skillpoints>0){
                bonusbspd+=2.0f;
                skillpoints--;
            }
            if (rato.x>=576&&rato.x<=591&&rato.y>=254&&rato.y<=272&&bonusbspd>0&&IsMouseButtonPressed(0)&&skillpoints>=0){
                bonusbspd-=2.0f;
                skillpoints++;
                
            }
            if (rato.x>=576&&rato.x<=591&&rato.y>=304&&rato.y<=322&&bonusspd>0&&IsMouseButtonPressed(0)&&skillpoints>=0){
                bonusspd-=1.0f;
                skillpoints++;
            }
            if (rato.x>=480&&rato.x<=496&&rato.y>=304&&rato.y<=322&&bonusspd<5&&IsMouseButtonPressed(0)&&skillpoints>0){
                bonusspd+=1.0f;
                skillpoints--;
            }
            if (rato.x>=576&&rato.x<=591&&rato.y>=354&&rato.y<=372&&bonush>0&&IsMouseButtonPressed(0)&&skillpoints>=0){
                bonush-=0.5f;
                skillpoints++;
            }
            if (rato.x>=480&&rato.x<=496&&rato.y>=354&&rato.y<=372&&bonush<2.0f&&IsMouseButtonPressed(0)&&skillpoints>0){
                bonush+=0.5f;
                skillpoints--;
            }
            if (rato.x>=576&&rato.x<=591&&rato.y>=404&&rato.y<=422&&bonusd>0&&IsMouseButtonPressed(0)&&skillpoints>=0){
                bonusd-=1.0f;
                skillpoints++;
            }
            if (rato.x>=480&&rato.x<=496&&rato.y>=404&&rato.y<=422&&bonusd<10&&IsMouseButtonPressed(0)&&skillpoints>0){
                bonusd+=1.0f;
                skillpoints--;
            }
            //desenhando bala
            DrawCircle(65,velbala,7,BLUE);
            velbala+=10+bonusbspd;
            if(velbala>=720)
            {
                velbala=440;
            }
                
            
            //desenhando nave
            DrawTexture(Nave,215,velnave,RAYWHITE);
            velnave-=5+bonusspd;
            if(velnave<=440)
            {
                velnave=720;
            }
                
            
            //desenhando hitbox
            DrawTexture(Nave,415,560,RAYWHITE);
            DrawCircle(440,595,9-bonush,GREEN);
            
            //desenhando bala c raio maior
            DrawCircle(560,velbb,7+bonusd,BLUE);
            velbb+=2;
            if(velbb>=720)
            {
                velbb=440;
            }
                
            
            EndDrawing();
            
            
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
            
            EndDrawing();
            if (IsKeyPressed(KEY_P)){
                break;
            }
        }
    
}
void InitMusic(void){
    for (int i=0;i<10;i++){
        musica[i].ativa=false;
    }
    numMusica=2;
    musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Hatsune Miku - Melt PV.mp3");
}
void ChoiceMusic(void){
    if (musica[numMusica].ativa){
        UnloadSound(musica[numMusica].mus);
        musica[numMusica].ativa=false;
    }
    if(!goodmusic){
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
    }
    else{
        numMusica=GetRandomValue(0,1);
        switch (numMusica){
            case 1:
            musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/UDR Bonde de Orgia De Traveco original.mp3");
            musica[numMusica].ativa=true;
            break;
            case 0:
            musica[numMusica].mus=LoadSound("/raylib/StarlightDrift/sounds/Chico Melancia  Vamos jogar domino !!!.mp3");
            musica[numMusica].ativa=true;
            break;
            
        }
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

GAMESTATE Fase1(void) //fase1
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    SetMasterVolume(0.25);
    
    InitFase1();
    
    while(1)
    {
        
        if(IsKeyPressed(KEY_F1)){
            Cheats();
        }
        if(IsKeyPressed('M'))
        {  
            UnloadSound(Laser);
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
                    
                    UnloadSound(Laser);
                    return MORTE;
                }
                if(LightCounter>30)
                {
                    UnloadSound(Laser);
                    fase++;
                    return JOGO;
                }
            }
        }
        
        if(!gameOver)
        {
            BeginDrawing();
            
            DrawFase1();
            if(dial)
            {
                Dialogo();
                
            }
            else
            {
                UpdateFase1();
            }
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

GAMESTATE Fase2(void) //fase2
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    dial = true;
    GAMESTATE returnstate;
    StopSound(BGM);
    //LoadArq();
    inimigo[0] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","r");
    inimigo[1] = fopen ("/raylib/StarlightDrift/enemy/enemies2.txt","r");
    inimigo[2] = fopen ("/raylib/StarlightDrift/enemy/enemies3.txt","r");
    inimigo[3] = fopen ("/raylib/StarlightDrift/enemy/enemies4.txt","r");
    inimigo[4] = fopen ("/raylib/StarlightDrift/enemy/enemies5.txt","r");
    inimigo[5] = fopen ("/raylib/StarlightDrift/enemy/enemies6.txt","r");
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
    inimigo[18] = fopen ("/raylib/StarlightDrift/enemy/enemies7.txt","r");
    inimigo[19] = fopen ("/raylib/StarlightDrift/enemy/enemies8.txt","r");
    inimigo[20] = fopen ("/raylib/StarlightDrift/enemy/enemies9.txt","r");
    inimigo[21] = fopen ("/raylib/StarlightDrift/enemy/enemies10.txt","r");
    inimigo[22] = fopen ("/raylib/StarlightDrift/enemy/enemies11.txt","r");
    inimigo[23] = fopen ("/raylib/StarlightDrift/enemy/enemies12.txt","r");
    inimigo[24] = fopen ("/raylib/StarlightDrift/enemy/wave5-1.txt","r");
    inimigo[25] = fopen ("/raylib/StarlightDrift/enemy/wave5-2.txt","r");
    inimigo[26] = fopen ("/raylib/StarlightDrift/enemy/wave5-3.txt","r");
    inimigo[27] = fopen ("/raylib/StarlightDrift/enemy/wave5-4.txt","r");
    inimigo[28] = fopen ("/raylib/StarlightDrift/enemy/wave5-5.txt","r");
    inimigo[29] = fopen ("/raylib/StarlightDrift/enemy/wave5-6.txt","r");
    inimigo[30] = fopen ("/raylib/StarlightDrift/enemy/wave5-7.txt","r");
    inimigo[31] = fopen ("/raylib/StarlightDrift/enemy/wave5-8.txt","r");
    
    
    while(1)
    {
        if(IsKeyPressed(KEY_F1)){
            Cheats();
        }
        if(IsKeyPressed('M'))
        {  FadeOut = true;
            returnstate = MENU;
           
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
                for(int i=0;i<32;i++)
                {
                    foe[i].ativo = false;
                }
                alpha = 1.0f;
                return MENU;
            }
        }
        
        
        
        BeginDrawing();
        
        DrawFase2();
        //Wave1();
        if (dial){
            Dialogo();
        }
        else{
            UpdateFase2();
            if(Wave1() >= 6)
            {
                
                if(wave2() >= 10)
                {
                    if(wave3() >= 17)
                    {   
                        if(wave4() >=24)
                        {
                            if(wave5() >= 32)
                            {
                                fase++;    
                                return JOGO;
                            }
                        }
                        
                    }
                }
            }
        }
            
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
        // UnloadArq();
        
        if(vida <= 0)
        {
         counter = 0;
         parte_dial--;
         UnloadTexture(enemi);
         UnloadSound(Laser);
         
         for(int i=0;i<32;i++)
         {
             foe[i].ativo = false;
             fclose(inimigo[i]);
         }
          return MORTE;
        }
        
    }   
   
   
}

GAMESTATE Fase3(void) //fase3
{
    InitFase3();
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
        while(1)
        {
            if(IsKeyPressed(KEY_F1)){
            Cheats();
        }
        if(IsKeyPressed('M'))  
        {  
            FadeOut = true;
            StopSound(bossbgm);
           
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
                if(Pericles.hp <= 0 )
                {
                    return WIN;
                }
                else
                {
                    return MENU;
                }
            }
        }
            
            
          
            
            BeginDrawing();
            DrawFase3();
            if(dial)
            {
                Dialogo();
            }
            else
            {
                UpdateFase3();
            }
            
            if(Pericles.hp <= 0)
            {
                FadeOut = true;
                
            }
            else if(gameOver)
            {
                gameOver = false;
                parte_dial--;
                UnloadSound(Laser);
                UnloadTexture(pericles);
                UnloadSound(bossbgm);
                return MORTE;
            }

            DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));


            EndDrawing();
        }
}

GAMESTATE Victory(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    SetMasterVolume(0.25);
    Texture2D fim = LoadTexture("/raylib/StarlightDrift/texture/win.png");
    Sound tumtis=LoadSound("/raylib/StarlightDrift/sounds/tumtis.mp3");
    PlaySound(tumtis);
    while(1)
    {
        
        if(IsKeyPressed(KEY_F1)){
            Cheats();
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
                StopSound(tumtis);
                UnloadSound(tumtis);
                return CREDITOS;
            }
        }
        
        
        BeginDrawing();
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        DrawTexture(fim,0,0,RAYWHITE);
        EndDrawing();
        if(IsKeyPressed(KEY_ENTER))
        {
            FadeOut = true;
        }
    }
}

GAMESTATE Ops(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    Rectangle RecSair = {320, Altura_Tela-40, 80, 40};
    Rectangle RecUp = {50, 200, 380, 40};
    Rectangle RecDown = {50, 300, 400, 40}; 
    Rectangle RecLeft = {50, 400, 460, 40};
    Rectangle RecRight = {50, 500, 440, 40};
    Rectangle RecR = {10, 600, 80, 40};
    Rectangle RecG = {210, 600, 100, 40};
    Rectangle RecB = {500, 600, 100, 40};
    Rectangle RecColorUp = {50, 650, 250, 40};
    Rectangle RecColorDown = {300, 650, 250, 40};
    Rectangle RecTeclaTiro = {50, 720, 500, 40};
    
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
            DrawText(FormatText("Para mudar as teclas segure o botao esquerdo\ndo mouse e aperte a tecla desejada\nAperte Espaço para alterar o volume\nVolume atual: %.f%%", 100*vol),174, 20, 20, RAYWHITE);
            if(CheckCollisionPointRec(PosicaoMouse, RecSair))
            {
                DrawText("Sair", 320, Altura_Tela-40, 40, LIGHTGRAY);
                if(IsMouseButtonPressed(0))
                {
                    FadeOut = true;
                }
            } 
            else 
            {
                DrawText("Sair", 335, Altura_Tela-30, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecUp))
            {
                DrawText(TextFormat("MOVIMENTO PARA CIMA:%c",Player_Up), 40, 190, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_Up=GetKeyPressed();
                        Player_Up=toupper(Player_Up);
                    }
                }
            } 
            else 
            {
                DrawText("Movimento para Cima", 50, 200, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecDown))
            {
                DrawText(TextFormat("MOVIMENTO PARA BAIXO:%c",Player_Down), 40, 290, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_Down=GetKeyPressed();
                        Player_Down=toupper(Player_Down);
                    }
                }
            }
            else 
            {
                DrawText("Movimento para baixo", 50, 300, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecLeft))
            {
                DrawText(TextFormat("MOVIMENTO PARA ESQUERDA:%c",Player_Left), 40,390, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_Left=GetKeyPressed();
                        Player_Left=toupper(Player_Left);
                    }
                }
            } 
            else 
            {
                DrawText("Movimento para esquerda", 50, 400, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecRight))
            {
                DrawText(TextFormat("MOVIMENTO PARA DIREITA:%c",Player_Right), 40, 490, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_Right=GetKeyPressed();
                        Player_Right=toupper(Player_Right);
                    }
                }
            } 
            else 
            {
                DrawText("Movimento para direita", 50, 500, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecR))
            {
                DrawText(TextFormat("RED:%c",Player_R), 20, 590, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_R=GetKeyPressed();
                        Player_R=toupper(Player_R);
                    }
                }
            } 
            else 
            {
                DrawText("Red", 10, 600, 25, GRAY);
            }
            
            if(CheckCollisionPointRec(PosicaoMouse, RecG))
            {
                DrawText(TextFormat("GREEN:%c",Player_G), 200, 590, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_G=GetKeyPressed();
                        Player_G=toupper(Player_G);
                    }
                }
            }
            else 
            {
                DrawText("Green", 210, 600, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecB))
            {
                DrawText(TextFormat("BLUE:%c",Player_B), 490, 590, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_B=GetKeyPressed();
                        Player_B=toupper(Player_B);
                    }
                }
            } 
            else 
            {
                DrawText("Blue", 500, 600, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecColorUp))
            {
                DrawText(TextFormat("COLOR UP:%c",Player_ColorUp), 40, 640, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_ColorUp=GetKeyPressed();
                        Player_ColorUp=toupper(Player_ColorUp);
                    }
                }
            } 
            else 
            {
                DrawText("Color Up", 50, 650, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecColorDown))
            {
                DrawText(TextFormat("COLOR DOWN:%c",Player_ColorDown), 290, 640, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_ColorDown=GetKeyPressed();
                        Player_ColorDown=toupper(Player_ColorDown);
                    }
                }
            }
            else 
            {
                DrawText("Color Down", 300, 650, 25, GRAY);
            }
            if(CheckCollisionPointRec(PosicaoMouse, RecTeclaTiro))
            {
                DrawText(TextFormat("TECLA TIRO:%c",Player_TeclaTiro), 40, 710, 40, LIGHTGRAY);
                if(IsMouseButtonDown(0))
                {
                    if(GetKeyPressed()>=0)
                    {
                        Player_TeclaTiro=GetKeyPressed();
                        Player_TeclaTiro=toupper(Player_TeclaTiro);
                    }
                }
            } 
            else 
            {
                DrawText("Tecla Tiro", 50, 720, 25, GRAY);
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
        
        DrawText("Se Fudeu", 100, 300, 60, RED);
        
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
    
    Movimento();
    
    if(IsKeyPressed(Player_ColorUp))
    {
        corshield++;
        if (corshield>3){
            corshield=1;
        }
    }
    
    if(IsKeyPressed(Player_ColorDown))
    {
        corshield--;
        if (corshield<1){
            corshield=3;
        }
    }
    
    if(IsKeyPressed(Player_R))
    {
        corshield=1;
    }
    
    if(IsKeyPressed(Player_G))
    {
        corshield=2;
    }
    
    if(IsKeyPressed(Player_B))
    {
        corshield=3;
    }

    /*switch(corshield){
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
    }*/
    
    

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
                if (!vidainf){
                    lives--;
                }
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
    dial = true;
    parte_dial = 1;
    LightFrame = 0;
    LightCounter = 0;
    //lives = 3;
    gameOver = false;
    
    UnloadImage(tempmeteor);
    UnloadImage(tempnave);
    UnloadImage(tempspace);
    
    BGM = LoadSound("/raylib/StarlightDrift/sounds/fase1teste.mp3");
    Laser = LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    
    player.pos.x = Largura_Tela/2;
    player.pos.y = Altura_Tela/2;
    player.spd = 5.0+bonusspd;
    player.dmg = 7+bonusd;
    player.color = BLUE;
    player.hitbox = 9-bonush;
    player.invincible = false;
    for(int i=0; i<MAX_TIROS; i++)
    {
        player.bullet[i].spd = 10+bonusbspd;
        player.bullet[i].active = false;
        player.bullet[i].raio = 5+bonusd*0.8f;
    }
    
    for(int i=0; i<MAX_METEOROS; i++)
    {
        meteoros[i].active = false;
    }
    
    light.Rec.x = 0;
    light.Rec.width = Largura_Tela;
    light.active = false;
}

void Pattern1(void)
{
    static int n_bullet=0;
    int chance = GetRandomValue(1, 100);
    if(chance <=10)
    {
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(bbullet[i].active == false)
            {
                bbullet[i].active = true;
                bbullet[i].rad = Pericles.hitbox*1.5;
                bbullet[i].def = GetRandomValue(-20, 20)/10.0f;
                n_bullet++;
                break;
            }
        }
    }

    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            bbullet[i].pos.x = Pericles.pos.x + bbullet[i].rad * cos(Pericles.ang + bbullet[i].def);
            bbullet[i].pos.y = Pericles.pos.y + bbullet[i].rad * sin(Pericles.ang + bbullet[i].def);
            bbullet[i].rad += 2;
            if(bbullet[i].pos.y >= Altura_Tela)
            {
                bbullet[i].active = false;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(bbullet[i].pos, bbullet[i].raio, player.pos, player.hitbox))
        {
            if(!vidainf)
            {
                lives--;
            }
            if(lives <=0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void Pattern2(void)
{
    static float increase = 6;
    float spacing = 2*PI/6;
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j;
                bbullet[j].rad = Pericles.hitbox*1.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    spacing = 2*PI/12;
    for(int i=0; i<12; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j + 10;
                bbullet[j].rad = Pericles.hitbox*2.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    spacing = 2*PI/20;
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j + 30;
                bbullet[j].rad = Pericles.hitbox*3.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            bbullet[i].pos.x = Pericles.pos.x + bbullet[i].rad * cos(Pericles.ang + bbullet[i].def);
            bbullet[i].pos.y = Pericles.pos.y + bbullet[i].rad * sin(Pericles.ang + bbullet[i].def);
            bbullet[i].rad += increase;
            if(bbullet[i].rad >= Altura_Tela || bbullet[i].rad <= Pericles.hitbox)
            {
                increase *= -1;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(bbullet[i].pos, bbullet[i].raio, player.pos, player.hitbox))
        {
            if(!vidainf)
            {
                lives--;
            }
            if(lives <= 0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void Pattern3(void)
{
    int chance = GetRandomValue(1, 100);
    if(chance <=6)
    {
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(bbullet[i].active == false)
            {
                bbullet[i].active = true;
                bbullet[i].pos.x = Pericles.pos.x;
                bbullet[i].pos.y = Pericles.pos.y;
                bbullet[i].xspd = GetRandomValue(-6, 6);
                bbullet[i].yspd = 6;
                break;
            }
        }
    }
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            float delta = player.pos.x - bbullet[i].pos.x;
            bbullet[i].pos.y += bbullet[i].yspd;
            if(delta*bbullet[i].xspd <0)
            {
                bbullet[i].xspd = -(bbullet[i].xspd + 0.5);
            }
            bbullet[i].pos.x += bbullet[i].xspd;
            if(bbullet[i].pos.y >= Altura_Tela)
            {
                bbullet[i].active = false;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(player.pos, player.hitbox, bbullet[i].pos, bbullet[i].raio))
        {
            if(!vidainf)
            {
                lives--;
            }
            if(lives <= 0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void UpdateFase3(void)
{
    if(!IsSoundPlaying(bossbgm)) PlaySound(bossbgm);
    backgroundScroll += 3.0;
    if(backgroundScroll >= Altura_Tela) backgroundScroll = 0;
    if(player.invincible)
    {
        iFrame++;
        if(iFrame >= 90)
        {
        iFrame = 0;
        player.invincible = false;
        }
    }
    Movimento();
    Atirar();
    BossMov();
    Pericles.ang += 0.03;
    if(Pericles.hp>1000)
    {
        Pattern1();
    } else 
    if(Pericles.hp>500)
    {
        Pattern2();
    } else 
    if(Pericles.hp>0)
    {
        Pattern3();
    }

    for(int i=0; i<MAX_TIROS; i++)
    {
        if(player.bullet[i].active && CheckCollisionCircles(player.bullet[i].pos, player.bullet[i].raio, Pericles.pos, Pericles.hitbox))
        {
            player.bullet[i].active = false;
            Pericles.hp -= player.dmg;
            if((Pericles.hp <= 1000 && Pericles.phase ==1) || (Pericles.hp <= 500 && Pericles.phase ==2))
            {
                Pericles.phase++;
                for(int i=0; i<MAX_TIROS; i++)
                {
                    bbullet[i].active = false;
                }
            }

        }
    }
    
    if(CheckCollisionCircles(player.pos,player.hitbox,Pericles.pos, Pericles.hitbox) && !player.invincible)
         {
                if (!vidainf)
                {
                    vida--;
                }
                
                
                player.invincible = true;
                
                
        }
    
}

void DrawFase3(void)
{
    ClearBackground(BLACK);
        DrawTexture(fundo, 0, backgroundScroll, RAYWHITE);
        DrawTexture(fundo, 0, backgroundScroll - fundo.height, RAYWHITE);
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
        //DrawCircleV(player.pos, player.hitbox, PINK);
        DrawTexture(pericles, Pericles.pos.x - pericles.width/2, Pericles.pos.y - pericles.height/2, RAYWHITE);
        //DrawCircleV(Pericles.pos, Pericles.hitbox, PURPLE);
        
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(player.bullet[i].active)
            {
                DrawCircleV(player.bullet[i].pos, player.bullet[i].raio, BLUE);
            }
            if(bbullet[i].active){
                DrawCircleV(bbullet[i].pos, bbullet[i].raio, RED);
            }
        }
        if(Pericles.hp>1000){
            DrawRectangle(20, 20, Largura_Tela-40, 20, GREEN);
            DrawRectangle(20, 20, (Largura_Tela*(Pericles.hp - 1000)/500) - 40, 20, BLUE);
        } else 
    if(Pericles.hp>500)
    {
        DrawRectangle(20, 20, Largura_Tela-40, 20, RED);
        DrawRectangle(20, 20, (Largura_Tela*(Pericles.hp - 500)/500) - 40, 20, GREEN);
    } else 
    {
        DrawRectangle(20, 20, (Largura_Tela*Pericles.hp/500) - 40, 20, RED);
    }
}

void InitFase3(void)
{
    Image tempnave = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    Image tempfundo = LoadImage("/raylib/StarlightDrift/texture/space.png");
    Image tempboss = LoadImage("/raylib/StarlightDrift/texture/boss.png");

    ImageResize(&tempnave, 50, 60);
    ImageResize(&tempfundo, Largura_Tela, Altura_Tela);
    ImageResize(&tempboss,200,250);

    Nave = LoadTextureFromImage(tempnave);
    fundo = LoadTextureFromImage(tempfundo);
    pericles = LoadTextureFromImage(tempboss);

    UnloadImage(tempnave);
    UnloadImage(tempfundo);
    UnloadImage(tempboss);
    
    bossbgm = LoadSound("/raylib/StarlightDrift/sounds/Volatile Reaction.mp3");

    player.pos.x = Largura_Tela/2;
    player.pos.y = Largura_Tela*0.8;
    /*player.spd = 7;
    player.dmg = 2;
    player.hitbox = 6;
    player.firerate = 5;
    player.invincible = false;*/
    dial = true;
    
    for(int i=0; i<MAX_TIROS; i++)
    {
        player.bullet[i].active = false;
        player.bullet[i].spd = 10+bonusbspd;;
        player.bullet[i].raio = 5+bonusd*0.8f;
        bbullet[i].active = false;
        bbullet[i].rad = 7;
        bbullet[i].raio = 7;
    }

    Pericles.pos.x = Largura_Tela/2;
    Pericles.pos.y = Altura_Tela*0.2;
    Pericles.hitbox = 70;
    Pericles.hp = 1500;
    Pericles.ang = 0;
    Pericles.phase = 1;
}

void BossMov(void)
{
    static int BossMovCounter = 0;
    static float x;
    static float y;
    if(BossMovCounter == 0)
    {
        Pericles.npos.x = GetRandomValue(Pericles.hitbox, Largura_Tela - Pericles.hitbox);
        Pericles.npos.y = GetRandomValue(Pericles.hitbox, Altura_Tela/3.0f);

        x = Pericles.npos.x - Pericles.pos.x;
        y = Pericles.npos.y - Pericles.pos.y;

        float norma = sqrt((x*x) + (y*y));

        Pericles.spd.x = x/norma;
        Pericles.spd.y = y/norma;
    } else 
    if(BossMovCounter <=60)
    {
        Pericles.pos.x += Pericles.spd.x;
        Pericles.pos.y += Pericles.spd.y;
    }
    BossMovCounter++;
    if(BossMovCounter>90) BossMovCounter = 0;
        
        
}

void LightBarrier(float mult)
{
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
            if (!vidainf){
                lives--;
            }
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
    
    if(!IsSoundPlaying(menu)) 
        PlaySound(menu);
    
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
            if(IsKeyPressed(KEY_F1)){
                Cheats();
            }
            DrawTextureRec(Fundolua,BordaFundo,PosicaoFundo,RAYWHITE);
            DrawTexture(FFXV,-40,-20,RAYWHITE);
            DrawTexture(luacristal,400,370,RAYWHITE);
           // DrawText(FormatText("Mouse: %.f, %.f\nFrame: %i", PosicaoMouse.x, PosicaoMouse.y, frameCounter), 0, 0, 20, RAYWHITE); //debug
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
