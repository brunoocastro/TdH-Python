/* Copyright (C) 2018.

 * Desenvolvedores - All Rights Reserved
 * - Bruno Silva de Castro;
 * - Luis Felipe Camponogara;
 * - Leonardo Felipe Aires;
 * - Sheila Gomes.

 * Licensed under the GNU GPL V3.0 license. All conditions apply.
 * Powered by Allegro: http://liballeg.org/license.html
 */

//BIBLIOTECAS UTILIZADAS
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

bool JOGO     = 1; // Determina se todo o jogo esta aberto.
bool MAINMENU = 1; // Indica se está no menu do jogo.
bool GAME     = 0; // Indica se o jogo começou.

void playgame()
{
    MAINMENU = 0;    // Fecha o menu e abre o jogo.
    GAME = 1;
}
void mmenu()
{
    MAINMENU = 1;    // retorna ao menu
    GAME = 0;
}

typedef struct
{
    ALLEGRO_BITMAP *bitmap;
    int valor;
    int posx,posy,width,height;
} disco;
typedef struct
{
    int x0,y0,x1,y1;
} hitbox;
typedef struct
{
    int posx;
    int posy1;
    int posy2;
    int posy3;
    int posy4;
    int posy5;
} coluna;

//VARIAVEIS E CONSTANTES UTILIZADAS NO PROGRAMA
const float FPS = 90;
const int SCREEN_W = 1024;
const int SCREEN_H = 760;

bool fim = false;
bool fim2 = false;

int click = 0, click2 = 0;
int jogadas = 31;
int tamanho = 5;
int dificuldade = 2;
int victory = 0;
int errado = 0;
int *ptrv1;
int *ptrv2;
int *ptrv3;

//VERIFICA A CONDIÇÃO DE VITORIA DO JOGO, COMO EXPLICADO EM CIMA
//A CONDIÇÃO DE VITORIA É O V3 ESTAR IGUAL O V1 EM POSIÇÃO INICIAL(USANDO O VETORWIN COMO COPIA DO VETOR INICIAL PARA COMPARAR)
int vitoria(int vetorwin[],int v3[])
{
    int i,count=0;
    for(i=0; i<tamanho; i++)
    {
        if(vetorwin[i] == v3[i])
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if(count==tamanho)
    {
        //CRIAR RESULTADO DA VITÓRIA
        printf("Parabens, voce venceu!");
        victory = 1;
        return 1;
    }
}

//PERCORRE O VETOR E COMPARA O PRIMEIRO VALOR DIFERENTE DE ZERO
//FAZ ISSO OLHANDO O EM i E COMPARANDO COM O DE i - 1, OU SEJA, O DE BAIXO
int compara(int vetor[])
{
    int i;
    for(i=tamanho-1; i>0; i--)
    {
        if(vetor[i]!=0)
        {
            if(vetor[i]>vetor[i-1] && vetor[i]>vetor[0])
            {
                errado = 1;
                return 1;
            }
        }
    }

    return 0;
}

int verifica(int click1, int click2)
{
    int i, j,k,a1 = 0,a2 = 0,a3 = 0;
    for(i=tamanho-1; i>=0; i--)
    {
        if(*(ptrv1+i)!=0)
        {
            a1 = *(ptrv1+i);
            break;
        }

    }
    for(j=tamanho-1; j>=0; j--)
    {
        if(*(ptrv2+j)!=0)
        {
            a2 = *(ptrv2+j);
            break;
        }

    }
    for(k=tamanho-1; k>=0; k--)
    {
        if(*(ptrv3+k)!=0)
        {
            a3 = *(ptrv3+k);
            break;
        }

    }

    if(click1 == 1 && click2 == 2)
    {
        if(a1>a2 && a2!=0)
            return 0;
        else
            return 1;
    }
    if(click1 == 1 && click2 == 3)
    {
        if(a1>a3 && a3!=0)
            return 0;
        else
            return 1;
    }
    if(click1==2 && click2 == 1)
    {
        if(a2>a1 && a1!=0)
            return 0;
        else
            return 1;
    }
    if(click1==2 && click2 == 3)
    {
        if(a2>a3 && a3!=0)
            return 0;
        else
            return 1;
    }
    if(click1==3 && click2 == 1)
    {
        if(a3>a1 && a1!=0)
            return 0;
        else
            return 1;
    }
    if(click1==3 && click2 == 2)
    {
        if(a3>a2 && a2!=0)
            return 0;
        else
            return 1;
    }
    else
        return 1;
}
//FUNÇÃO Q PERCORRE O VETOR E COLOCA O NUMERO RECEBIDO DA FUNÇÃO POP
//NO ULTIMO VALOR POSSIVEL(OU SEJA, O BEM DE BAIXO) DA PILHA (NO VETOR RECEIDO EM *REF, O VALOR QUE VAI SER COLOCADO É RECEBIDO DE POP[VALOR])
void push(int *ref, int valor)
{
    int i;
    for(i=0; i<tamanho; i++)
    {
        if(ref[i]==0)
        {
            ref[i]=valor;
            break;
        }
    }
}
//FUNÇÃO Q PERCORRE O VETOR E RETIRA O PRIMEIRO VALOR(OU SEJA, O BEM DE CIMA) DA PILHA (NO VETOR RECEIDO EM *REF)
int pop(int *ref)
{
    int valor1;
    int i;
    for(i=tamanho - 1; i >= 0; i--)
    {
        if(ref[i]!=0)
        {
            valor1 = ref[i];
            ref[i] = 0;
            return valor1;
        }
    }
    return 0;
}

void desenha(int v1[],int v2[], int v3[],disco *disco1,disco *disco2,disco *disco3,disco *disco4,disco *disco5,coluna *coluna1,coluna *coluna2,coluna *coluna3)
{
    int posyc1[] = {coluna1->posy1,coluna1->posy2,coluna1->posy3,coluna1->posy4,coluna1->posy5};
    int posyc2[] = {coluna2->posy1,coluna2->posy2,coluna2->posy3,coluna2->posy4,coluna2->posy5};
    int posyc3[] = {coluna3->posy1,coluna3->posy2,coluna3->posy3,coluna3->posy4,coluna3->posy5};

    int i;
    for(i=4; i>=0; i--)
    {
        if(v1[i]!=0 || v2[i]!=0 || v3[i]!=0)
        {

            if(v1[i] == disco1->valor)
            {
                disco1->posx = coluna1->posx;
                disco1->posy = posyc1[i];
            }
            if(v1[i] == disco2->valor)
            {
                disco2->posx = coluna1->posx;
                disco2->posy = posyc1[i];
            }
            if(v1[i] == disco3->valor)
            {
                disco3->posx = coluna1->posx;
                disco3->posy = posyc1[i];
            }
            if(v1[i] == disco4->valor)
            {
                disco4->posx = coluna1->posx;
                disco4->posy = posyc1[i];
            }
            if(v1[i] == disco5->valor)
            {
                disco5->posx = coluna1->posx;
                disco5->posy = posyc1[i];
            }

            if(v2[i] == disco1->valor)
            {
                disco1->posx = coluna2->posx;
                disco1->posy = posyc2[i];
            }
            if(v2[i] == disco2->valor)
            {
                disco2->posx = coluna2->posx;
                disco2->posy = posyc2[i];
            }
            if(v2[i] == disco3->valor)
            {
                disco3->posx = coluna2->posx;
                disco3->posy = posyc2[i];
            }
            if(v2[i] == disco4->valor)
            {
                disco4->posx = coluna2->posx;
                disco4->posy = posyc2[i];
            }
            if(v2[i] == disco5->valor)
            {
                disco5->posx = coluna2->posx;
                disco5->posy = posyc2[i];
            }

            if(v3[i] == disco1->valor)
            {

                disco1->posx = coluna3->posx;
                disco1->posy = posyc3[i];
            }
            if(v3[i] == disco2->valor)
            {
                disco2->posx = coluna3->posx;
                disco2->posy = posyc3[i];
            }
            if(v3[i] == disco3->valor)
            {
                disco3->posx = coluna3->posx;
                disco3->posy = posyc3[i];
            }
            if(v3[i] == disco4->valor)
            {
                disco4->posx = coluna3->posx;
                disco4->posy = posyc3[i];
            }
            if(v3[i] == disco5->valor)
            {
                disco5->posx = coluna3->posx;
                disco5->posy = posyc3[i];
            }
        }
    }
}

//FUNÇÃO PRINCIPAL DO JOGO CRIADO ANTERIORMENTE NO CONSOLE
//AINDA TENTANDO ADAPTAR PRA ESSE JOGO
int logica(int click, int click2, int v1[], int v2[], int v3[],int vetorwin[],int i)
{
    if (click !=0 && click2 != 0)
    {
        //TIPOS DE JOGADAS
        if(click == 1 && click2 == 2)
        {
            verifica(click,click2);
            push(v2, pop(v1));
            if(compara(v2) || compara(v1))
            {
                push(v1, pop(v2));
            }
            click  = 0;
            click2 = 0;

        }
        else if(click == 1 && click2 == 3)
        {
            push(v3, pop(v1));
            if(compara(v3) || compara(v1))
            {
                push(v1, pop(v3));
            }
            click  = 0;
            click2 = 0;
        }
        else if(click == 2 && click2 == 3)
        {
            push(v3, pop(v2));
            if(compara(v3) || compara(v2))
            {
                push(v2, pop(v3));
            }
            click  = 0;
            click2 = 0;
        }
        else if(click == 2 && click2 == 1)
        {
            push(v1, pop(v2));
            if(compara(v1) || compara(v2))
            {
                push(v2, pop(v1));
            }
            click  = 0;
            click2 = 0;
        }
        else if(click == 3 && click2 == 2)
        {
            push(v2, pop(v3));
            if(compara(v2) || compara(v3))
            {
                push(v3, pop(v2));
            }
            click  = 0;
            click2 = 0;
        }
        else if(click == 3 && click2 == 1)
        {
            push(v1, pop(v3));
            if(compara(v1) || compara(v3))
            {
                push(v3, pop(v1));
            }
            //jogadas --;
            click  = 0;
            click2 = 0;
        }
        else
        {

        }

    }
}
//FUNCÃO RESPONSAVEL PELO TRATAMENTO DO MOUSE ( NO CLICK)
//RECEBE EVENTOS DO MOUSE E AS HITBOX CRIADAS
int tratamouse(ALLEGRO_EVENT ev,hitbox hc1, hitbox hc2, hitbox hc3,hitbox hsair)
{

    int click;
    if(ev.mouse.x > hc1.x0 && ev.mouse.y > hc1.y0 && ev.mouse.x  < hc1.x1 && ev.mouse.y  < hc1.y1)
    {
        click = 1; //Coluna 1
        return click;
    }

    else if(ev.mouse.x > hc2.x0 && ev.mouse.y > hc2.y0 && ev.mouse.x < hc2.x1 && ev.mouse.y < hc2.y1)
    {
        click = 2; //Coluna 2
        return click;
    }
    else if(ev.mouse.x > hc3.x0 && ev.mouse.y > hc3.y0 && ev.mouse.x < hc3.x1 && ev.mouse.y < hc3.y1)
    {
        click = 3; //Coluna 3
        return click;
    }
    else if(ev.mouse.x  > hsair.x0 && ev.mouse.y  > hsair.y0 && ev.mouse.x  < hsair.x1 && ev.mouse.y < hsair.y1)
    {
        printf("Botao de Sair pressionado.\n");
        exit(0);
        return 0;
    }
    else
    {
        click = 0;
        return click;
    }
}
//FUNCÃO RESPONSAVEL PELO TRATAMENTO DO MOUSE ( NO CLICK2)
int tratamouse2(ALLEGRO_EVENT ev,hitbox hc1, hitbox hc2, hitbox hc3,hitbox hsair)
{

    int click2;
    if(ev.mouse.x > hc1.x0 && ev.mouse.y > hc1.y0 && ev.mouse.x  < hc1.x1 && ev.mouse.y  < hc1.y1)
    {
        click2 = 1; //Coluna 1
        return click2;
    }

    else if(ev.mouse.x > hc2.x0 && ev.mouse.y > hc2.y0 && ev.mouse.x < hc2.x1 && ev.mouse.y < hc2.y1)
    {
        click2 = 2; //Coluna 2
        return click2;
    }
    else if(ev.mouse.x > hc3.x0 && ev.mouse.y > hc3.y0 && ev.mouse.x < hc3.x1 && ev.mouse.y < hc3.y1)
    {
        click2 = 3; //Coluna 3
        return click2;
    }
    else if(ev.mouse.x  > hsair.x0 && ev.mouse.y  > hsair.y0 && ev.mouse.x  < hsair.x1 && ev.mouse.y < hsair.y1)
    {
        printf("Botao de Sair pressionado.\n");
        exit(0);
        return 0;
    }
    else
    {
        click2 = 0;
        return click2;
    }
}
int tratamousemenu(ALLEGRO_EVENT ev,hitbox hfacil,hitbox hmedio,hitbox hdificil)
{
    int clickmenu = 0;
    if(ev.mouse.x > hfacil.x0 && ev.mouse.y > hfacil.y0 && ev.mouse.x  < hfacil.x1 && ev.mouse.y  < hfacil.y1)
    {
        printf("\nBotao Facil pressionado.\n");
        clickmenu = 14; //Botao facil
        return clickmenu;
    }

    else if(ev.mouse.x > hmedio.x0 && ev.mouse.y > hmedio.y0 && ev.mouse.x < hmedio.x1 && ev.mouse.y < hmedio.y1)
    {
        printf("\nBotao Medio pressionado.\n");
        clickmenu = 6; //Botao medio
        return clickmenu;
    }
    else if(ev.mouse.x > hdificil.x0 && ev.mouse.y > hdificil.y0 && ev.mouse.x < hdificil.x1 && ev.mouse.y < hdificil.y1)
    {
        printf("\nBotao Dificil pressionado.\n");
        clickmenu = 2; //Botao dificil
        return clickmenu;
    }
    else
    {
        clickmenu = 4;
        return clickmenu;
    }
}

int main(int argc, char **argv)
{
    //TODA PARTE DE INICIALIZAÇÃO
    //COMEÇA AQUI E VAI ATÉ ANTES DO WHILE(1)
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_AUDIO_STREAM *backmusic = NULL;
    ALLEGRO_AUDIO_STREAM *erro = NULL;
    ALLEGRO_AUDIO_STREAM *button1 = NULL;
    ALLEGRO_AUDIO_STREAM *button2 = NULL;
    ALLEGRO_AUDIO_STREAM *winner = NULL;
    ALLEGRO_AUDIO_STREAM *gameover = NULL;
    if(!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro!\n");
        return -1;
    }
    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        fprintf(stderr, "Falha ao criar timer!\n");
        return -1;
    }
    bool redraw = true;
    bool redrawmenu = true;
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "Falha ao criar o display!\n");
        al_destroy_timer(timer);
        return -1;
    }
    al_set_window_title(display, "Torre de Hanoi - THE GAME");
    if(!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar a entrada do mouse!\n");
        return -1;
    }
    if(!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar as imagens!\n");
        return -1;
    }
    al_set_target_bitmap(al_get_backbuffer(display));
    event_queue = al_create_event_queue();
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *win        = NULL;
    ALLEGRO_BITMAP *menubg     = NULL;
    ALLEGRO_BITMAP *lost       = NULL;
    ALLEGRO_BITMAP *icon       = NULL;
    lost      = al_load_bitmap("imagens/gameover.png");
    background= al_load_bitmap("imagens/principal.png");
    win       = al_load_bitmap("imagens/win.png");
    menubg    = al_load_bitmap("imagens/menu.png");
    icon      = al_load_bitmap("imagens/icon.png");
    if (!background || !win)
    {
        fprintf(stderr, "Falha ao carregar fundo ou vitoria\n");
        return -1;
    }
    //Definição dos discos pela Struct
    disco disco1,disco2,disco3,disco4,disco5;
    //Carregando as imagens
    disco1.bitmap = al_load_bitmap("imagens/disco1.png");
    disco2.bitmap = al_load_bitmap("imagens/disco2.png");
    disco3.bitmap = al_load_bitmap("imagens/disco3.png");
    disco4.bitmap = al_load_bitmap("imagens/disco4.png");
    disco5.bitmap = al_load_bitmap("imagens/disco5.png");
    if (!disco1.bitmap || !disco2.bitmap || !disco3.bitmap || !disco4.bitmap || !disco5.bitmap)
    {
        fprintf(stderr, "Falha ao carregar discos!\n");
        return -1;
    }
    //Dando valor aos discos

    disco1.valor = 1;
    disco2.valor = 2;
    disco3.valor = 3;
    disco4.valor = 4;
    disco5.valor = 5;

    //Definindo o tamanho dos discos
    disco1.height = 40;
    disco1.width  = 170;

    disco2.height = 40;
    disco2.width  = 190;

    disco3.height = 40;
    disco3.width  = 210;

    disco4.height = 40;
    disco4.width  = 230;

    disco5.height = 40;
    disco5.width  = 250;

    //DEFINIÇÃO DAS POSIÇOES INICIAIS DO DISCO
    disco1.posx = 220;
    disco1.posy = 377;

    disco2.posx = 220;
    disco2.posy = 420;

    disco3.posx = 220;
    disco3.posy = 463;

    disco4.posx = 220;
    disco4.posy = 507;

    disco5.posx = 220;
    disco5.posy = 550;

    //Definindo as HITBOX
    hitbox hc1,hc2,hc3,hsair,hfacil,hmedio,hdificil;
    //Dando as cordenadas de cada uma delas
    hc1.x0 = 70;
    hc1.y0 = 250;
    hc1.x1 = 360;
    hc1.y1 = 630;

    hc2.x0 = 370;
    hc2.y0 = 250;
    hc2.x1 = 630;
    hc2.y1 = 630;

    hc3.x0 = 630;
    hc3.y0 = 250;
    hc3.x1 = 940;
    hc3.y1 = 630;

    hsair.x0 = 820;
    hsair.y0 = 0;
    hsair.x1 = 1023;
    hsair.y1 = 70;

    hfacil.x0 = 24;
    hfacil.y0 = 372;
    hfacil.x1 = 346;
    hfacil.y1 = 452;

    hmedio.x0 = 360;
    hmedio.y0 = 372;
    hmedio.x1 = 682;
    hmedio.y1 = 452;

    hdificil.x0 = 690;
    hdificil.y0 = 372;
    hdificil.x1 = 1010;
    hdificil.y1 = 452;

    //Structs Colunas
    coluna coluna1,coluna2,coluna3;
    //Definindo as posiçoes de cada uma delas
    coluna1.posx  = 220;
    coluna1.posy1 = 550;
    coluna1.posy2 = 507;
    coluna1.posy3 = 463;
    coluna1.posy4 = 420;
    coluna1.posy5 = 377;

    coluna2.posx  = 490;
    coluna2.posy1 = 550;
    coluna2.posy2 = 507;
    coluna2.posy3 = 463;
    coluna2.posy4 = 420;
    coluna2.posy5 = 377;

    coluna3.posx  = 760;
    coluna3.posy1 = 550;
    coluna3.posy2 = 507;
    coluna3.posy3 = 463;
    coluna3.posy4 = 420;
    coluna3.posy5 = 377;

    //Cria os vetores que vao ser usados no jogo, conforme uma variavel tamanho a ser definido o numero de discos
    int v1[5];
    int v2[5];
    int v3[5];
    int vetorwin[5]; //Vetor utilizado para funcao de vitoria
    int i;
    for(i=0; i<tamanho; i++)
    {
        v1[i]= tamanho - i;
        v2[i]= 0;
        v3[i]= 0;
        vetorwin[i]= tamanho - i;
    }
    ptrv1 = &v1;
    ptrv2 = &v2;
    ptrv3 = &v3;
    al_init_font_addon(); // Inicializador de fontes
    al_init_ttf_addon();// Inicializador de ttf
    ALLEGRO_FONT *font = al_load_ttf_font("8-BIT WONDER.ttf",20,0 );
    if (!font)
    {
        fprintf(stderr, "Nao foi possivel carregar a fonte: '8-BIT WONDER.ttf'.\n");
        return -1;
    }
    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }
    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }
    if (!al_reserve_samples(6))
    {
        fprintf(stderr, "Falha ao reservar samples!\n");
        return -1;
    }
    backmusic = al_load_sample("musicas/backmusic.ogg");
    erro      = al_load_sample("musicas/erro.ogg");
    button1   = al_load_sample("musicas/click1.ogg");
    button2   = al_load_sample("musicas/click2.ogg");
    winner    = al_load_sample("musicas/winner.ogg");
    gameover  = al_load_sample("musicas/gameover.ogg");
    if(!backmusic || !erro || !button1 || !button2 || !win)
    {
        fprintf(stderr, "Falha ao carregar samples!\n");
        return -1;
    }
    if(!event_queue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(disco1.bitmap);
        al_destroy_bitmap(disco2.bitmap);
        al_destroy_bitmap(disco3.bitmap);
        al_destroy_bitmap(disco4.bitmap);
        al_destroy_bitmap(disco5.bitmap);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    //Lista de registro de eventos que devem ser analizados no jogo
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_play_sample(backmusic, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_set_display_icon(display,icon);
    al_start_timer(timer);

    while (JOGO) // inicia o loop jogo
    {
        while (MAINMENU) // inicia o loop do menu principal
        {
            ALLEGRO_EVENT ev; //Cria uma função que armazena eventos
            al_wait_for_event(event_queue, &ev);

            ALLEGRO_MOUSE_STATE state;//Chama mouse
            al_get_mouse_state(&state);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                return 0;
            }

            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                switch(ev.mouse.button)
                {
                case (1):
                    al_play_sample(button1, 1.0, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
                    dificuldade = tratamousemenu(ev,hfacil,hmedio,hdificil);
                    if(dificuldade!= 4)
                    {
                        jogadas += dificuldade;
                        redraw = true;
                        playgame();
                        continue;
                    }
                    else
                        fim = true;
                case (2):
                    al_play_sample(button2, 1.0, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
                    dificuldade = tratamousemenu(ev,hfacil,hmedio,hdificil);
                    if(dificuldade!= 4)
                    {
                        jogadas += dificuldade;
                        redraw = true;
                        playgame();
                        continue;
                    }
                    else
                        fim2 = true;

                }
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                switch(ev.mouse.button)
                {
                case (1):
                    fim = false;
                case (2):
                    fim2 = false;
                }
            }


            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;
                al_draw_bitmap(menubg,0,0,NULL);
                al_draw_text(font, al_map_rgb(0, 0, 0),500,330,ALLEGRO_ALIGN_CENTRE,"Escolha a dificuldade");
                al_flip_display();
            }
        }

        while (GAME)
        {

            ALLEGRO_EVENT ev; //Cria uma função que armazena eventos
            al_wait_for_event(event_queue, &ev);

            ALLEGRO_MOUSE_STATE state;//Chama mouse
            al_get_mouse_state(&state);

            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                return 0;
            }

            //FUNCAO DE CLICK
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if(ev.mouse.button & 1)
                {
                    al_play_sample(button1, 1.0, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
                    click = tratamouse(ev,hc1,hc2,hc3,hsair);
                    fim = true;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(ev.mouse.button & 1)
                {
                    fim = false;
                }
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if(ev.mouse.button & 2)
                {
                    al_play_sample(button2, 1.0, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
                    click2 = tratamouse2(ev,hc1,hc2,hc3,hsair);

                    fim2 = true;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(ev.mouse.button & 2);
                {
                    fim2 = false;
                }
            }

            if(click != 0 && click2 != 0)  //Chama a função logica enviando os parametros necessarios, sem entrar em um loop infinito
            {
                int x = 0;
                x = verifica(click,click2);
                if(x==1)
                    logica(click,click2,v1,v2,v3,vetorwin,i);
                    else
                    errado=1;

                desenha(v1,v2,v3,&disco1,&disco2,&disco3,&disco4,&disco5,&coluna1,&coluna2,&coluna3);
                redraw = true;
                if(vitoria(vetorwin, v3) == 1)
                {
                    al_destroy_sample(backmusic);
                    al_play_sample(winner, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                }
                if(click!=click2)
                    jogadas--;
                if(jogadas==0)
                {
                    al_destroy_sample(backmusic);
                    al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                }
                click  = 0;
                click2 = 0;
            }
            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

                al_draw_bitmap(background,0,0,NULL);
                al_draw_bitmap(disco1.bitmap,(disco1.posx-disco1.width/2),(disco1.posy-disco1.height/2),NULL);
                al_draw_bitmap(disco2.bitmap,(disco2.posx-disco2.width/2),(disco2.posy-disco2.height/2),NULL);
                al_draw_bitmap(disco3.bitmap,(disco3.posx-disco3.width/2),(disco3.posy-disco3.height/2),NULL);
                al_draw_bitmap(disco4.bitmap,(disco4.posx-disco4.width/2),(disco4.posy-disco4.height/2),NULL);
                al_draw_bitmap(disco5.bitmap,(disco5.posx-disco5.width/2),(disco5.posy-disco5.height/2),NULL);
                al_draw_textf(font, al_map_rgb(0, 0, 0),630,30,ALLEGRO_ALIGN_CENTRE,"%d Jogadas Restantes", jogadas);
                al_flip_display();

                if(victory == 1)
                {
                    al_flip_display();
                    al_draw_bitmap(win,384,80,NULL);
                    al_flip_display();
                    errado = 0;
                    al_rest(7);
                    victory = 0;
                    exit(0);
                }
                if(jogadas==0)
                {
                    al_flip_display();
                    al_draw_bitmap(lost,384,80,NULL);
                    al_flip_display();
                    al_rest(7);
                    exit(0);
                }
                if(errado==1)
                {
                    al_play_sample(erro, 1.0, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
                    errado = 0;
                }
            }
        }
    }
}
END_OF_MAIN();
