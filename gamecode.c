#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

//-------------------------------------
//defines
//-------------------------------------

#define MAX_TIROS 150

//-------------------------------------
//Structs
//-------------------------------------

typedef enum {MENU=0, JOGO, CREDITOS, OPCOES, SAIR} GAMESTATE;

typedef struct Tiro
{
    Vector2 posicao;
    Vector2 vel_bala;
    Color cor;
    bool ativa;
}Tiro;

typedef struct Player
{
    Vector2 posicao;
    Vector2 vel;
    Color cor;
    float raio;
    float firerate;
    int tiro;
}Player;

//-------------------------------------
//Variaveis Globais
//-------------------------------------
static const int Largura_Tela = 720;
static const int Altura_Tela = 876;
static bool gameOver = false;
static bool pause = false;

static Rectangle background;
static float movbackground;
static Texture2D Nave;
static Texture2D fundo;
static Texture2D FundodeFernando;
static Texture2D FFXV;
static Texture2D luacristal;
static Texture2D Fundolua;
static Texture2D enemi;
static GAMESTATE gameState;
static FILE* inimigo[20];
static Sound menu;
static Sound Laser;
static int a,b,c,d,e,f,g,h,i,j,k,l;
static Rectangle foe[20] = {0};

static Tiro tiro[MAX_TIROS];
static Player jogador;
//static Music musicas;
static float vol = 1;


//-------------------------------------
//Funcoes Modulares Locais
//-------------------------------------
static void InitMovBackground(void);

static int Level1(int Vidas);       //Level1 
static int Level2(int Vidas);       //Level2 
static int Level3(int Vidas);       //Level3


static void InitGame(void);     //Iniciar o jogo
static void UnloadGame(void);   //Descarregar arquivos do jogo
static void Movimento(void);
static void Inicializa_tiro(void);
static void Inicializa_jogador(void);
static void InitMenu(void);     //Iniciar arquivos do menu 
static void UnloadMenu(void);   //Descarregar arquivos do menu
static void Pause(void);    //pausa o jogo 
static GAMESTATE LevelSelect(void);      //Escolhe a fase
static GAMESTATE Jogo(void);
static GAMESTATE MenuScreen(void);   //Menu inicial
static GAMESTATE Creditos(void);     //Creditos
static GAMESTATE Ops(void);          //Opções
static void UpdateGame(void);   //Atualiza a matematica do frame
static void DrawGame(void);     //Desenha o frame
//static void LoadArq(void); //loada os arq
//static void Wave1(FILE* lado1,FILE* lado2); //wave de teste
//static void UnloadArq(void);//unloada arq



int main(void)
{
    gameState=MENU;
    InitWindow(Largura_Tela, Altura_Tela, "Starlight Drift Limpo");
    InitAudioDevice();
    menu = LoadSound("/raylib/StarlightDrift/sounds/Main_Menu.mp3");
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
                gameState = Jogo();
                UnloadGame();
            break;
            case CREDITOS:
                gameState = Creditos();
            break;
            
            case OPCOES:
                gameState = Ops();
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
void Wave1()
{
    fscanf (inimigo[0], "%i %i\n",&a,&b);
    fscanf (inimigo[1], "%i %i\n",&c,&d);
    fscanf (inimigo[2], "%i %i\n",&e,&f);
    fscanf (inimigo[3], "%i %i\n",&g,&h);
    fscanf (inimigo[4], "%i %i\n",&i,&j);
    fscanf (inimigo[5], "%i %i\n",&k,&l);
    
    for(int i=0;i<6;i++)
    {
        foe[i].height = 40;
        foe[i].width = 50;
        if(i==0)
        {
            foe[i].x = a;
            foe[i].y = b;
        }
        else if(i==1)
        {
            foe[i].x = c;
            foe[i].y = d;
        }
        else if(i==2)
        {
            foe[i].x = e;
            foe[i].y = f;
        }
        else if(i==3)
        {
            foe[i].x = g;
            foe[i].y = h;
        }
        else if(i==4)
        {
            foe[i].x = i;
            foe[i].y = j;
        }
        else if(i==5)
        {
            foe[i].x = k;
            foe[i].y = l;
        }
    }
    
    
    
    
}
void Inicializa_jogador(void)
{
    jogador.posicao.x = 360;
    jogador.posicao.y = 700;
    jogador.vel.x = 8;
    jogador.vel.y = 8;
    jogador.cor = BLUE;
    jogador.firerate = 0;
    jogador.raio = 8;
    jogador.tiro = 1;
}
void InitGame(void){
    StopSound(menu);
    
    Image NaveImg = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    
    ImageResize(&NaveImg,40,50);
   
    Nave = LoadTextureFromImage(NaveImg);
    
    Image enemy = LoadImage("/raylib/StarlightDrift/texture/inimigo.png");
    ImageResize(&enemy,40,50);
    enemi = LoadTextureFromImage(enemy);
    UnloadImage(enemy);
   
    InitMovBackground();
    Inicializa_jogador();
    Inicializa_tiro();
    
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
        tiro[i].posicao.x = jogador.posicao.x;
        tiro[i].posicao.y = jogador.posicao.y + 8;
        tiro[i].vel_bala.y = 15;
        tiro[i].vel_bala.x = 2;
        tiro[i].ativa = false;
        tiro[i].cor = ORANGE;
    }
}


void UnloadGame(void){
    UnloadTexture(Nave);
    UnloadTexture(fundo);
    // UnloadArq();
    for(int i=0;i<6;i++)
    {
        fclose(inimigo[i]);
    }
}
void UpdateGame(void){
    movbackground += 3.0; //velocidade do background
    if(movbackground >= fundo.height) {
        movbackground = 0; //looping do background
    }
    
    Movimento();
    Atirar();
    
}
void DrawGame(void){
    ClearBackground(BLACK);
    DrawTextureEx(fundo,(Vector2){0,movbackground},0.0f,1.0f,WHITE);
    DrawTextureEx(fundo,(Vector2){0,-fundo.height + movbackground},0.0f,1.0f,WHITE);
    DrawTexture(Nave,jogador.posicao.x,jogador.posicao.y,RAYWHITE);
    DrawTexture(enemi,a,b,RAYWHITE);
    DrawTexture(enemi,c,d,RAYWHITE);
    DrawTexture(enemi,e,f,RAYWHITE);
    DrawTexture(enemi,g,h,RAYWHITE);
    DrawTexture(enemi,i,j,RAYWHITE);
    DrawTexture(enemi,k,l,RAYWHITE);
    
    for(int i = 0;i<MAX_TIROS;i++)
    {
        if(tiro[i].ativa)
        {
            tiro[i].posicao.y -= tiro[i].vel_bala.y;
            DrawCircle(tiro[i].posicao.x,tiro[i].posicao.y,3,tiro[i].cor);
        }
        if(tiro[i].posicao.y <=0)
        {
            tiro[i].ativa = false;
        }
    }
}

void Atirar(void)
{
    if(IsKeyDown(KEY_SPACE))
    {
        jogador.firerate += 5;
        
        if(jogador.firerate == 20)
        {
            for(int i = 0;i<MAX_TIROS;i++)
            {
                if(!tiro[i].ativa)
                {
                    tiro[i].posicao.x = jogador.posicao.x + 20;
                    tiro[i].posicao.y = jogador.posicao.y + 8;
                    tiro[i].ativa = true;
                    break;
                }
            }
            jogador.firerate = 0;
        }
    }
}

void Movimento(void)
{
    if(IsKeyDown('W') && jogador.posicao.y >= jogador.raio)
        jogador.posicao.y -= jogador.vel.y;
    if(IsKeyDown('A') && jogador.posicao.x >= jogador.raio)
        jogador.posicao.x -= jogador.vel.x;
    if(IsKeyDown('S') && jogador.posicao.y <= Altura_Tela - 45)
        jogador.posicao.y += jogador.vel.y;
    if(IsKeyDown('D') && jogador.posicao.x <= Largura_Tela - 45)
        jogador.posicao.x += jogador.vel.x;
}

int Level1(int Vidas){
    while (!gameOver){
        UpdateGame();
        DrawGame();
    }
    return Vidas;
}

int Level2(int Vidas){
    while (!gameOver){
        UpdateGame();
        DrawGame();
        
        
    }
    return Vidas;
}

int Level3(int Vidas){
    while (!gameOver){
        UpdateGame();
        DrawGame();
        
        
    }
    return Vidas;
}
void Pause(void)
{   
   
        
        while(1)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PAUSE",150,50,50,RAYWHITE);
            DrawText("Para voltar pressione P",70,150,20,RAYWHITE);
            DrawText("Para ir para o menu pressione M durante o jogo",70,250,20,RAYWHITE);
            EndDrawing();
            if(IsKeyPressed(KEY_P))
                break;
        }
    
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

GAMESTATE Jogo(void)
{
    float alpha = 1.0f;
    bool FadeIn = true;
    bool FadeOut = false;
    //LoadArq();
    inimigo[0] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt","r");
    inimigo[1] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt2","r");
    inimigo[2] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt3","r");
    inimigo[3]= fopen ("/raylib/StarlightDrift/enemy/enemies.txt4","r");
    inimigo[4] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt5","r");
    inimigo[5] = fopen ("/raylib/StarlightDrift/enemy/enemies.txt6","r");
    
    while(1)
    {
        if(IsKeyPressed('M'))
            FadeOut = true;
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
                
                alpha = 1.0f;
                return MENU;
            }
        }
        
        
        UpdateGame();
        BeginDrawing();
        DrawGame();
        
        Wave1();
        DrawRectangle(0, 0, Largura_Tela, Altura_Tela, Fade(BLACK, alpha));
        EndDrawing();
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
