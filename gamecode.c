#include "raylib.h"

typedef struct Player //Estrutura com parâmetros do player e alguns parâmetros fixos dos tiros(como cor e velocidade)
{
    Vector2 nave;
    Vector2 shotspeed;
    Vector2 vel;
    int firerate;
    Color cor_nave;
    Color cor_bala;
    float raio;
    int tipo_tiro;
}Player;

typedef struct Bala //Estrutura com parâmetros para movimentação dos tiros
{
    Vector2 posicao;
    bool ativa;
    int tipo_tiro;
}Bala;

/*typedef struct Inimigu//estrutura para fazer os inimigos e futuramente suas balas
{
    Vector2 posicao;
    bool ativa;
    Color cordoini;
    float raio;
}Inimigu*/


static Player jogador = {0};
static Bala bala[50] = {0}; //O vetor bala é necessário pois cada elemento dele é uma bala na tela
static Bala bala2[50] = {0};
static Bala bala3[50] = {0};
static Bala bala4[50] = {0};

void volume(float* vol){
    if ((IsKeyPressed(KEY_UP))&&(*vol<1.0f)){
        *vol+=0.1f;
    }
    if ((IsKeyPressed(KEY_DOWN))&&(*vol>0.0f)){
        *vol-=0.1f;
    }
    SetMasterVolume(*vol);
}

void creditos()
{   
    while(1)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("CREDITOS",150,50,70,RAYWHITE);
        DrawText("Lucas Fernandes Lins",70,150,50,RAYWHITE);
        DrawText("Matheus Felipe Lima",70,250,50,RAYWHITE);
        DrawText("Jose Rodrigues Neto",70,350,50,RAYWHITE);
        DrawText("Joao Victor Galdino",70,450,50,RAYWHITE);
        DrawText("Luiz Fernando Barbosa",70,550,50,RAYWHITE);
        DrawText("Pressione M para sair",100,750,20,RAYWHITE);
        EndDrawing();
        if(IsKeyPressed('M'))
            break;
        
    }
}


void menuu (Texture2D* luacristal,Texture2D* nave,float* vol,Texture2D* fundo,Sound* menu,Texture2D* FFXV,Texture2D* Fundolua)
{
   int cont;
   float contador2=0.3;
   float contador3=0.365;
   Rectangle nossofundo={0};
   Vector2 fuck ={0};
   //declarando fundo
   nossofundo.x=0;
   nossofundo.y=0;
   nossofundo.width=720;
   nossofundo.height=876;
   
   cont=0;
   PlaySound(*menu);   
   while(1)
        {
           
           volume(vol);
           int posicaodomousex=GetMouseX();
           int posicaodomousey=GetMouseY(); 
          
           nossofundo.x+=contador2;
           nossofundo.y+=contador3;
           cont+=1;
           if(cont==1400)
           {
                contador2=-contador2;
                contador3=-contador3;
                
                cont=0;
               
           }
            BeginDrawing();
        
                    ClearBackground(RAYWHITE);
                        
                    //desenhando o fundo
                    //DrawTexture(*bck,0,0,RAYWHITE);
                    DrawTextureRec(*Fundolua,nossofundo,fuck,RAYWHITE);
                    DrawTexture(*FFXV,-40,-20,RAYWHITE);
                    DrawTexture(*luacristal,400,370,RAYWHITE);
                    DrawText(TextFormat("%i %i",posicaodomousex ,posicaodomousey), 190, 200, 20, LIGHTGRAY); 
                    DrawText(TextFormat("%i",cont), 190, 400, 20, LIGHTGRAY); 
                    
                    
                        if (((posicaodomousex>=415)&&(posicaodomousex<=550))&&(posicaodomousey>=570)&&(posicaodomousey<=595))
                        {
                            DrawText("Novo Jogo",400,560,40,LIGHTGRAY);
                            if(IsMouseButtonDown(0)){
                                StopSound(*menu);
                                break;
                            }
                        }
                        else
                        {
                            DrawText("Novo Jogo",415,570,25,LIGHTGRAY);
                        }
                        if (((posicaodomousex>=415)&&(posicaodomousex<=500))&&(posicaodomousey>=620)&&(posicaodomousey<=645))
                        {
                            DrawText("Créditos",400,610,40,LIGHTGRAY);
                            if(IsMouseButtonDown(0))
                                creditos();
                        }
                        else
                        {
                            DrawText("Créditos",415,620,25,LIGHTGRAY);
                        }
                        if (((posicaodomousex>=415)&&(posicaodomousex<=460))&&(posicaodomousey>=670)&&(posicaodomousey<=695))
                        {
                            DrawText("Sair",400,660,40,LIGHTGRAY);
                            if(IsMouseButtonDown(0))
                            {
                                UnloadTexture(*luacristal); 
                                UnloadTexture(*nave);  
                                UnloadTexture(*fundo); 
                                UnloadTexture(*FFXV);
                                UnloadTexture(*Fundolua);
                                UnloadSound(*menu);
                                CloseWindow();
                            }
                                
                        }
                        else
                        {
                            DrawText("Sair",415,670,25,LIGHTGRAY);
                        }
        
                
                    
                EndDrawing();
        }
}


void Pause(int* pont)
{   
   
        
        while(1)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PAUSE",150,50,50,RAYWHITE);
            DrawText("Para voltar pressione P",70,150,20,RAYWHITE);
            DrawText("Para ir para o menu pressione M",70,250,20,RAYWHITE);
            EndDrawing();
            if(IsKeyPressed(KEY_P))
                break;
            if(IsKeyPressed('M'))
            {
              (*pont) = 1;
              break;
            }
        }
    
}

void Troca_tiro(int *tipo)
{
    if(IsKeyPressed('0'))
    {
        (*tipo) = 0;
    }
    if(IsKeyPressed('1'))
    {
        (*tipo) = 1;
    }
    if(IsKeyPressed('2'))
    {
        (*tipo) = 2;
    }
    if(IsKeyPressed('3'))
    {
        (*tipo) = 3;
    }
}

void Movimento() //Função de movimentação
{
    if(IsKeyDown('W'))
    {
        jogador.nave.y -= jogador.vel.y;
    }
    if(IsKeyDown('A'))
    {
        jogador.nave.x -= jogador.vel.x;
    }
    if(IsKeyDown('S'))
    {
        jogador.nave.y += jogador.vel.y;
    }
    if(IsKeyDown('D'))
    {
        jogador.nave.x += jogador.vel.x;
    }
    
}
float VolumeBalas(float* vol2){
    if (IsKeyPressed(KEY_LEFT)&&*vol2>0.0f){
            *vol2-=0.1f;
        }
        else if (IsKeyPressed(KEY_RIGHT)&&*vol2<1.0f){
            *vol2+=0.1f;
        }
        return *vol2;
}
void SomDasBalas(int tiro,Sound* laser){
    switch (tiro){
        case 0:{
            PlaySound(laser[1]);
            break;
        }
        case 1:{
            PlaySound(laser[1]);
            PlaySound(laser[2]);
            break;
        }
        case 2:{
            PlaySound(laser[1]);
            PlaySound(laser[2]);
            PlaySound(laser[3]);
            break;
        }
        case 3:{
            PlaySound(laser[1]);
            PlaySound(laser[2]);
            PlaySound(laser[3]);
            PlaySound(laser[0]);
            break;
        }
        
    }
}

void BarraEspecial(Player jogador,int* tiro, float* mult, Rectangle barra){
    //atualiza os valores da barra para ficar embaixo da nave
    barra.x= jogador.nave.x+1;
    barra.y= jogador.nave.y+50;
    barra.width= 39;
    barra.height=10;
    //verifica se ta apertando espaço ou não e aumenta ou diminue a barra
    if (IsKeyDown(KEY_SPACE)&&*mult<1.0f){
        *mult+=0.005f;
    }
    else if (!IsKeyDown(KEY_SPACE&&*mult>0.1f)){
        *mult-=0.012f;
    }
    if (*mult<0.0f){
        *mult=0.0f;
    }
    barra.width*=*mult;
    if (*mult<=0.40){
        *tiro=0;
        DrawRectangleRec(barra,GREEN);
    }
    if (*mult>0.40f&&*mult<=0.70f){
        *tiro=1;
        DrawRectangleRec(barra,YELLOW);
    }
    else if (*mult>0.70f&&*mult<=0.90f){
        *tiro=2;
        DrawRectangleRec(barra,ORANGE);
    }
    else if (*mult>0.90f&&*mult<=1.1f){
        *tiro=3;
        DrawRectangleRec(barra,RED);
    }
    
    
    DrawRectangleLines(jogador.nave.x,jogador.nave.y+50,40,10,BLACK);
}
void Tiro(Player jogador,int* firerate)
{
    
    
    if(IsKeyDown(KEY_SPACE)) //Se a barra de espaço está apertada, os tiros saem
    {   
        
        (*firerate)+=5; //A cada frame esse valor aumenta em 5, quando chegar em 20 uma bala é atirada (1 bala a cada 4 frames, pode ser alterado)
        if((*firerate) == 20)
        {
            for(int i = 0;i<50;i++)
            {
                if(!bala[i].ativa)
                {
                    //A bala inicia na posição do player, na "ponta da frente"
                    if(jogador.tipo_tiro == 0)
                    {
                        bala[i].tipo_tiro = 0;
                        bala[i].posicao.x = jogador.nave.x + 20; 
                        bala[i].posicao.y = jogador.nave.y - 8;
                        bala[i].ativa = true;
                    }
                    else if(jogador.tipo_tiro == 1)
                    {
                        bala[i].tipo_tiro = 1;
                        bala[i].posicao.x = jogador.nave.x + 15; 
                        bala[i].posicao.y = jogador.nave.y - 8;
                        bala[i].ativa = true;
                        bala2[i].tipo_tiro = 1;
                        bala2[i].posicao.x = jogador.nave.x + 25; 
                        bala2[i].posicao.y = jogador.nave.y - 8;
                        bala2[i].ativa = true;
                    }
                    else if(jogador.tipo_tiro == 2)
                    {
                        bala[i].tipo_tiro = 2;
                        bala[i].posicao.x = jogador.nave.x + 20; 
                        bala[i].posicao.y = jogador.nave.y - 8;
                        bala[i].ativa = true;
                        bala2[i].tipo_tiro = 2;
                        bala2[i].posicao.x = jogador.nave.x + 20; 
                        bala2[i].posicao.y = jogador.nave.y - 8;
                        bala2[i].ativa = true;
                        bala2[i].tipo_tiro = 2;
                        bala3[i].posicao.x = jogador.nave.x + 20; 
                        bala3[i].posicao.y = jogador.nave.y - 8;
                        bala3[i].ativa = true;
                        
                    }
                    else if(jogador.tipo_tiro == 3)
                    {
                        bala[i].tipo_tiro = 3;
                        bala[i].posicao.x = jogador.nave.x + 15; 
                        bala[i].posicao.y = jogador.nave.y - 8;
                        bala[i].ativa = true;
                        bala2[i].tipo_tiro = 3;
                        bala2[i].posicao.x = jogador.nave.x + 25; 
                        bala2[i].posicao.y = jogador.nave.y - 8;
                        bala2[i].ativa = true;
                        bala3[i].tipo_tiro = 3;
                        bala3[i].posicao.x = jogador.nave.x + 12; 
                        bala3[i].posicao.y = jogador.nave.y - 8;
                        bala3[i].ativa = true;
                        bala4[i].tipo_tiro = 3;
                        bala4[i].posicao.x = jogador.nave.x + 28; 
                        bala4[i].posicao.y = jogador.nave.y - 8;
                        bala4[i].ativa = true;
                    }
                    
                    break;
                }
            }
            (*firerate) = 0;
        }
    }

    for(int i = 0;i<50;i++)
    {
        if(bala[i].tipo_tiro == 0)
        {
            if(bala[i].posicao.y>0 && bala[i].ativa)
            {
                //A bala se movimenta de acordo com a "shotspeed(Velocidade do tiro)"
                bala[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala[i].posicao.x,bala[i].posicao.y,3,jogador.cor_bala);
            }
        }
        else if(bala[i].tipo_tiro == 1)
        {
            if(bala[i].posicao.y>0 && bala[i].ativa && bala2[i].posicao.y>0 && bala2[i].ativa)
            {
                bala[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala[i].posicao.x,bala[i].posicao.y,3,jogador.cor_bala);
                bala2[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala2[i].posicao.x,bala2[i].posicao.y,3,jogador.cor_bala);
            }
        }
        else if(bala[i].tipo_tiro == 2)
        {
            if(bala[i].posicao.y>0 && bala[i].ativa && bala2[i].posicao.y>0 && bala2[i].ativa && bala3[i].posicao.y>0 && bala3[i].ativa)
            {
                bala[i].posicao.y -= jogador.shotspeed.y;
                bala[i].posicao.x -= jogador.shotspeed.x;
                DrawCircle(bala[i].posicao.x,bala[i].posicao.y,3,jogador.cor_bala);
                bala2[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala2[i].posicao.x,bala2[i].posicao.y,3,jogador.cor_bala);
                bala3[i].posicao.y -= jogador.shotspeed.y;
                bala3[i].posicao.x += jogador.shotspeed.x;
                DrawCircle(bala3[i].posicao.x,bala3[i].posicao.y,3,jogador.cor_bala);
            }
        }
        else if(bala[i].tipo_tiro == 3)
        {
            if(bala[i].posicao.y>0 && bala[i].ativa && bala2[i].posicao.y>0 && bala2[i].ativa && bala3[i].posicao.y>0 && bala3[i].ativa && bala4[i].posicao.y>0 && bala4[i].ativa)
            {
                bala[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala[i].posicao.x,bala[i].posicao.y,3,jogador.cor_bala);
                bala2[i].posicao.y -= jogador.shotspeed.y;
                DrawCircle(bala2[i].posicao.x,bala2[i].posicao.y,3,jogador.cor_bala);
                bala3[i].posicao.y -= jogador.shotspeed.y;
                bala3[i].posicao.x -= jogador.shotspeed.x;
                DrawCircle(bala3[i].posicao.x,bala3[i].posicao.y,3,jogador.cor_bala);
                bala4[i].posicao.y -= jogador.shotspeed.y;
                bala4[i].posicao.x += jogador.shotspeed.x;
                DrawCircle(bala4[i].posicao.x,bala4[i].posicao.y,3,jogador.cor_bala);
            }
        }
        if(bala[i].posicao.y<0 || bala[i].posicao.x<0 || bala[i].posicao.x>720)
        {
            //Se a bala sair da tela, ela fica inativa(pra poupar memória e pra ele poder ser "atirada de novo")
            bala[i].ativa = false;
        }
    }
}


int main(void)
{
    //criando variavel para menu ingame
    int checkmenu=0;
    //Definindo resolução da tela
    const int Largura_Tela = 720;
    const int Altura_Tela = 876;
    //criando variavel dos arquivos de audio
    Sound menu;
    Sound laser[4];
    laser[0]=LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    laser[1]=LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    laser[2]=LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    laser[3]=LoadSound("/raylib/StarlightDrift/sounds/laser.ogg");
    //criando var de volume e barra de especial
    float vol=1.0f;
    float vol2=1.0f;
    float mult=0;
    
    //Definindo parâmetros do player(e alguns dos tiros do player)
    jogador.nave.x = 300;
    jogador.nave.y = 650;
    jogador.firerate = 0;
    jogador.vel.x = 8;
    jogador.vel.y = 8;
    jogador.shotspeed.x = 2;
    jogador.shotspeed.y = 15;
    jogador.cor_nave = BLACK;
    jogador.cor_bala = ORANGE;
    jogador.raio = 10;
     jogador.tipo_tiro = 0;
    //declarando float de movimento da tela
     float movbackground = 0.0f;
    //criando retangulo de especial
    Rectangle barra;
    
    //Inicializando janela e áudio
    InitWindow(Largura_Tela,Altura_Tela,"Starlight Drift");
    InitAudioDevice();
    
    //carregando imagens e criando texturas, favor copiar o caminho e copiar
   // Image imageyy = LoadImage("/raylib/StarlightDrift/texture/fundo.png");
    Image background = LoadImage("/raylib/StarlightDrift/texture/space.png");
    Image nave1 = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    Image luadecristal = LoadImage("/raylib/StarlightDrift/texture/luatransparente.png");
    Image fundodecristal = LoadImage("/raylib/StarlightDrift/texture/fundosemluacopiacortada.png");
    Image FFXVdecristal = LoadImage("/raylib/StarlightDrift/texture/imagempqp.png");
    ImageResize(&fundodecristal,1140,1752);
    ImageResize(&FFXVdecristal,720,465);
    ImageResize(&luadecristal,150,150);
    ImageResize(&background,720,876);
    ImageResize(&nave1,40,50);
   // Texture2D bck = LoadTextureFromImage(imageyy);
    Texture2D FFXV = LoadTextureFromImage(FFXVdecristal);
    Texture2D nave = LoadTextureFromImage(nave1);
    Texture2D fundo = LoadTextureFromImage(background);
    Texture2D luacristal = LoadTextureFromImage(luadecristal);
    Texture2D Fundolua = LoadTextureFromImage(fundodecristal);
    //Texture2D Fundolua = LoadTexture("/raylib/StarlightDrift/texture/fundosemluacopiacortada.png");
    UnloadImage(FFXVdecristal);
    UnloadImage(fundodecristal);
    UnloadImage(luadecristal);
    UnloadImage(background);
    UnloadImage(nave1);
    //UnloadImage(imageyy);
    
    //carregando música na ram
    menu=LoadSound("/raylib/StarlightDrift/sounds/Main_Menu.mp3");
    
    SetTargetFPS(60);
    
    while(1)
    {   
      //impedindo loop infinito
        checkmenu=0;
        //funcao menu
        
        menuu(&luacristal,&nave,&vol,&fundo,&menu,&FFXV,&Fundolua);
        
        
    while(1)
    {
        //Começando a desenhar e chamando as funções
        Movimento();
        if(IsKeyPressed(KEY_P))
            {
                Pause(&checkmenu);
            }
        if(checkmenu==1)
            break;
        movbackground += 3.0; //velocidade do background
        if(movbackground >= background.height) movbackground = 0; //looping do background
        
        Troca_tiro(&jogador.tipo_tiro);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(fundo,(Vector2){0,movbackground},0.0f,1.0f,WHITE);
        DrawTextureEx(fundo,(Vector2){0,-background.height + movbackground},0.0f,1.0f,WHITE);
        //DrawCircle(jogador.nave.x,jogador.nave.y,8,jogador.cor_nave);
        DrawTexture(nave,jogador.nave.x,jogador.nave.y,RAYWHITE);
        
        DrawText("0 - Tiro Normal",300,400,20,LIGHTGRAY);
        DrawText("1 - Tiro Duplo",300,430,20,LIGHTGRAY);
        DrawText("2 - Tiro Triplo",300,460,20,LIGHTGRAY);
        DrawText("3 - Tiro Duplo com 2 auxiliares",300,490,20,LIGHTGRAY);
        DrawText(TextFormat("%i",jogador.tipo_tiro),200,200,20,BLACK);
        Tiro(jogador,&jogador.firerate);
        if (IsKeyDown(KEY_SPACE)){
            SomDasBalas(jogador.tipo_tiro,laser);
        }
        vol2=VolumeBalas(&vol2);
        for (int i=0;i<4;i++){
            SetSoundVolume(laser[i],vol2);
        }
        BarraEspecial(jogador,&jogador.tipo_tiro,&mult,barra);
        EndDrawing();
    }
}
   // UnloadTexture(bck); 
    UnloadTexture(nave);    
    return 0;
}
