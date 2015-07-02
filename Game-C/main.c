/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V1.6.18
*Titulo: "SHOCK EFFECT"
*********************************/

//inclusao de bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "objects.h" //header de objetos
#include "functions.h" //header de funcoes

////////////////////////////////////////////////////////////////////////////////
//GLOBALS
int WIDTH; //largura display
int HEIGHT; //altura display
const int GRAVITY = 1;
const int FPS = 60;
int back_x; //ponto x do fundo do background
int back_y; //ponto y do fundo do background
enum KEYS {ENTER, UP, DOWN, LEFT, RIGHT, Q, W, E};
bool keys[8] = {false, false, false, false, false, false, false, false};
const int TELA_INICIO = 0;
const int TELA_INSTRU = 1;
const int TELA_JOGO = 2;
const int TELA_FINAL = 3;
////////////////////////////////////////////////////////////////////////////////

//main
int main()
{
    int b;
    int letra;
    OpcaoBackground(letra);

    int tela = TELA_INICIO;

    //primitive variables
    int NUM_ENEMYRED = 10; //quantidade de inimigos vermelhos
    int NUM_ENEMYBLUE = 10; //quantidade de inimigos azuis
    int NUM_BOSS = 5;
    int text_color = 255; //variavel para cor (animacao inicial de jogo - efeito relampago)
    int text_boss = 255; //variavel para cor de texto boss
    int FPS = 60; //frames per second
    bool done = false;
    bool redraw = true;

    //object variables
    struct Player player;
    struct Enemy_red enemyred[NUM_ENEMYRED];
    struct Enemy_blue enemyblue[NUM_ENEMYBLUE];
    struct Boss boss[NUM_BOSS];
    struct Shoot shootQ;
    struct Shoot shootW;
    struct Shoot shootE;
    struct Obstacle obstacle;
    struct SpriteScientist scientist;
    struct Sprite background0;
    struct Sprite background1;
    struct Sprite background2;
    struct Sprite background3;
    struct Sprite background4;
    struct Sprite background5;
    struct Sprite background6;
    struct Sprite enemyred_sprite;

    //allegro variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    //ALLEGRO_BITMAP *reprovado = NULL;
    ALLEGRO_SAMPLE *musica0 = NULL;
    ALLEGRO_SAMPLE *musica1 = NULL;
    ALLEGRO_SAMPLE *musica2 = NULL;
    ALLEGRO_SAMPLE *musica3 = NULL;
    ALLEGRO_SAMPLE *musica4 = NULL;
    ALLEGRO_SAMPLE *musica5 = NULL;
    ALLEGRO_SAMPLE *musica6 = NULL;
    ALLEGRO_SAMPLE *musica666 = NULL;
    ALLEGRO_SAMPLE_ID musica3id;
    ALLEGRO_SAMPLE_ID musica1id;
    ALLEGRO_SAMPLE_ID musica666id;

    ALLEGRO_FONT *title_font = NULL;
    ALLEGRO_FONT *medium_font = NULL;

////////////////////////////////////////////////////////////////////////

    //verificacoes de erro
    if(!al_init())
        return -1; //caso de erro ao inicializar allegro

    display = al_create_display(WIDTH,HEIGHT); //criar display

    if(!display)
        return -1; //se der merda

    //Allegro Module Init
    al_init_primitives_addon();
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        printf("Falha ao inicializar addon allegro_ttf.\n");
        return -1;
    }

    medium_font = al_load_font("fonts/EHSMB.TTF", WIDTH/20, 0);
    if (!medium_font) {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    title_font = al_load_font("fonts/French Electric Techno.ttf", WIDTH/8, 0);
    if (!title_font) {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.1, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.3, ALLEGRO_ALIGN_CENTRE, "LOADING...");
    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.5, ALLEGRO_ALIGN_CENTRE, "99%");
    al_flip_display();

    al_install_keyboard();

    if(!al_init_image_addon()) {
        printf("Falha ao inicializar image addon");
        return -1;
    }

    //reprovado = al_load_bitmap("images/reprovado.png");

    al_install_audio();
    if(!al_install_audio()) {
        printf("Falha ao inicializar audio");
        return -1;
    }

    al_init_acodec_addon();
    if(!al_init_acodec_addon()) {
        printf("Falha ao inicializar acodec addon");
        return -1;
    }

    al_reserve_samples(6); //reserva amostras de som para serem tocadas
    if(!al_reserve_samples(6)) {
        printf("Falha ao reservar samples");
        return -1;
    }

    //ALLEGRO QUEUE
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    //Inicializacao de objetos

    InitShootQ(shootQ, letra); //funcao que inicializa disparo 1 (capacitor)
    if(!shootQ.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootQ.\n");
        return -1;
    }

    InitShootW(shootW, letra); //funcao que inicializa disparo 2 (indutor)
    if(!shootW.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootW.\n");
        return -1;
    }

    InitShootE(shootE); //funcao que inicializa habilidade de escudo (shield / resistor)
    if(!shootE.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shield.\n");
        return -1;
    }

    InitScientist(scientist);
    scientist.bitmap = al_load_bitmap("images/scientist.png");
    if (!scientist.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite scientist.\n");
        return -1;
    }

    InitPlayer(player, &text_color); //funcao que "inicia" player
    InitEnemyRed(enemyred, &NUM_ENEMYRED, letra); //funcao que inicia enemyred
    InitEnemyBlue(enemyblue, &NUM_ENEMYBLUE, letra); //funcao que inicia enemyblue
    InitObstacle(obstacle); //funcao que inicializa obstaculos
    InitBoss(boss, &NUM_BOSS, letra); //funcao que inicializa chefes (bosses)
    InitEnemyredSprite(enemyred_sprite); // funcao que inicializa sprite de inimigo vermelho
    switch (letra) {
        case 0:
        InitBackground0(background0, musica0, letra); //funcao que inicializa sprite de background0
        break;
        case 1:
        InitBackground1(background1, musica1); //funcao que inicializa sprite de background1 alternativo
        break;
        case 2:
        InitBackground2(background2, musica2); //funcao que inicializa sprite de background2 alternativo
        break;
        case 3:
        InitBackground3(background3, musica3); //funcao que inicializa sprite de background3 alternativo
        break;
        case 4:
        InitBackground4(background4, musica4); //funcao que inicializa sprite de background4 alternativo
        break;
        case 5:
        InitBackground5(background5, musica5); //funcao que inicializa sprite de background5 alternativo
        break;
        case 6:
        InitBackground6(background6, musica6); //funcao que inicializa sprite de background6 alternativo
        break;
        case 666:
        InitBackground0(background0, musica666, letra);
    }

    switch (letra) {
        case 0:
        al_play_sample(musica0, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        break;
        case 1:
        al_play_sample(musica1, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &musica1id);
        break;
        case 2:
        al_play_sample(musica2, 1.5, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        break;
        case 3:
        al_play_sample(musica3, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &musica3id);
        break;
        case 4:
        al_play_sample(musica4, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        break;
        case 5:
        al_play_sample(musica5, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        break;
        case 6:
        al_play_sample(musica0, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        break;
        case 666:
        al_play_sample(musica666, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &musica666id);
    }

    //ALLEGRO REGISTER
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.9, ALLEGRO_ALIGN_CENTRE, "Deu!");
    al_flip_display();

    //Loop do jogo
    while (!done)
    {
        ALLEGRO_EVENT ev;
        switch(tela)
        {
/////////////////////////////////////////////////////////////////////////////////
                case TELA_INICIO:

                al_wait_for_event(event_queue, &ev);
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    done = true;

                if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                        tela = TELA_INSTRU;
                }
                switch(letra) {

                    case 0:
                    al_draw_bitmap(background0.image[12], 0, 0, 0);
                    break;
                    case 1:
                    al_draw_bitmap(background1.image[11], 0, 0, 0);
                    break;
                    case 2:
                    al_draw_bitmap(background2.image[12], 0, 0, 0);
                    break;
                    case 3:
                    al_draw_bitmap(background3.image[11], 0, 0, 0);
                    break;
                    case 4:
                    al_draw_bitmap(background4.image[11], 0, 0, 0);
                    break;
                    case 5:
                    al_draw_bitmap(background5.image[20], 0, 0, 0);
                    break;
                    case 6:
                    al_draw_bitmap(background6.image[21], 0, 0, 0);
                    break;
                    case 666:
                    al_draw_bitmap(background0.image[12], 0, 0, 0);
                    break;
                }

                al_flip_display();
                break;
//////////////////////////////////////////////////////////////////////////////////////
                case TELA_INSTRU:

                al_wait_for_event(event_queue, &ev);
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    done = true;

                if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                        tela = TELA_JOGO;
                }
                if(letra == 0 || letra == 666)
                    al_draw_bitmap(background0.image[13], 0, 0, 0);
                else
                    al_clear_to_color(al_map_rgb(0, 255, 0));
                al_flip_display();
                break;

//////////////////////////////////////////////////////////////////////////////////
                case TELA_JOGO:

                al_wait_for_event(event_queue, &ev);

                //se clicar para fechar a janela
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    done = true;

                //evento do timer (vai entrar nesse else if sempre, a nao ser que feche a janela)
                else if(ev.type == ALLEGRO_EVENT_TIMER)
                {
                    redraw = true;

                    if(keys[RIGHT] && !player.moving)
                    {
                        player.velx = player.speed;
                        player.moving = true;
                    }

                    if(keys[LEFT] && !player.moving)
                    {
                        player.velx = player.speed;
                        player.moving = true;
                    }

                    ChangeColor(&text_color, player, boss, &NUM_BOSS, &text_boss);
                    PlayerJump(player, &keys[UP]);
                    PlayerRight(player, &keys[RIGHT], scientist);
                    PlayerLeft(player, &keys[LEFT]);
                    PlayerSample(player, letra, &musica3id, musica3);
                    BossSample(boss, &NUM_BOSS, letra, &musica1id, musica1, &musica666id, musica666);

                    //updates
                    UpdateShootQ(shootQ, player);
                    UpdateShootW(shootW, player);
                    UpdateShootE(shootE, player);
                    UpdateEnemyRed(enemyred, &NUM_ENEMYRED, player, shootQ);
                    UpdateEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player, shootW);
                    UpdateObstacle(obstacle, medium_font, player);
                    UpdateBoss(boss, &NUM_BOSS, &text_boss, player, enemyred, &NUM_ENEMYRED, enemyblue, &NUM_ENEMYBLUE, letra);

                    //colisoes
                    ShootQColisionEnemyRed(shootQ,enemyred, &NUM_ENEMYRED, player);
                    ShootWColisionEnemyBlue(shootW, enemyblue, &NUM_ENEMYBLUE, player);
                    ShootColisionBoss(shootW, shootQ, boss, &NUM_BOSS, player);
                    PlayerColisionEnemyBlue(player, enemyblue, &NUM_ENEMYBLUE);
                    PlayerColisionEnemyRed(player, enemyred, &NUM_ENEMYRED);
                    PlayerColisionObstacle(player,obstacle);
                    PlayerColisionBoss(player, boss, &NUM_BOSS);

                    ResetPlayer(&tela, player, enemyred, &NUM_ENEMYRED, enemyblue, &NUM_ENEMYBLUE, obstacle, boss, &NUM_BOSS, &text_color, musica3, &musica3id, letra);
                }

                else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch(ev.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ESCAPE:
                        done = true;
                        break;
                    case ALLEGRO_KEY_UP:
                        keys[UP] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        keys[RIGHT] = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        keys[LEFT] = true;
                        break;
                    case ALLEGRO_KEY_Q:
                        keys[Q] = true;
                        FireShootQ(shootQ, player);
                        break;
                    case ALLEGRO_KEY_W:
                        keys[W] = true;
                        FireShootW(shootW, player);
                        break;
                    case ALLEGRO_KEY_E:
                        keys[E] = true;
                        FireShootE(shootE, player);
                        break;
                    }
                }

                else if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    switch(ev.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_UP:
                        keys[UP] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        keys[RIGHT] = false;
                        player.moving = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        keys[LEFT] = false;
                        player.moving = false;
                        break;
                    case ALLEGRO_KEY_Q:
                        keys[Q] = false;
                        break;
                    case ALLEGRO_KEY_W:
                        keys[W] = false;
                        break;
                    case ALLEGRO_KEY_E:
                        keys[E] = false;
                        break;
                    }
                }

                if(redraw && al_is_event_queue_empty(event_queue))
                {
                    redraw = false;

                    //desenhar objetos
                    switch (letra) {
                        case 0:
                        DrawBackground0(background0);
                        break;
                        case 1:
                        DrawBackground1(background1);
                        break;
                        case 2:
                        DrawBackground2(background2);
                        break;
                        case 3:
                        DrawBackground3(background3);
                        break;
                        case 4:
                        DrawBackground4(background4);
                        break;
                        case 5:
                        DrawBackground5(background5);
                        break;
                        case 6:
                        DrawBackground6(background6);
                        break;
                        case 666:
                        DrawBackground0(background0);
                        break;
                    }





                    DrawText(title_font, medium_font, player, boss, &NUM_BOSS, &text_color, &text_boss, obstacle);
                    DrawShootQ(shootQ, letra, boss);
                    DrawShootW(shootW, letra, boss);
                    DrawShootE(shootE, player);
                    DrawEnemyRed(enemyred, &NUM_ENEMYRED, player, enemyred_sprite);
                    DrawEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player);
                    DrawBoss(boss, &NUM_BOSS, player);
                    DrawObstacle(obstacle);
                    DrawScientist(player, scientist, &keys[LEFT], &keys[RIGHT]);

                    al_flip_display();
                }
            break;
//////////////////////////////////////////////////////////////////////////////////////
            case TELA_FINAL:
            al_wait_for_event(event_queue, &ev);
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    tela = TELA_INICIO;
            }
            switch(letra) {

                case 0:
                al_draw_bitmap(background0.image[14], 0, 0, 0);
                break;
                case 1:
                al_draw_bitmap(background1.image[13], 0, 0, 0);
                break;
                case 2:
                al_draw_bitmap(background2.image[14], 0, 0, 0);
                break;
                case 3:
                al_draw_bitmap(background3.image[13], 0, 0, 0);
                break;
                case 4:
                al_draw_bitmap(background4.image[13], 0, 0, 0);
                break;
                case 5:
                al_draw_bitmap(background5.image[22], 0, 0, 0);
                break;
                case 6:
                al_draw_bitmap(background6.image[23], 0, 0, 0);
                break;
                case 666:
                al_draw_bitmap(background0.image[14], 0, 0, 0);
                break;
            }
            al_flip_display();
            break;
        } //final do switch
    } //final do while

    //destroi coisas

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(title_font);
    al_destroy_font(medium_font);
    al_destroy_display(display);
    al_destroy_sample(shootQ.sample);
    al_destroy_sample(shootW.sample);
    al_destroy_sample(player.sample[0]);
    al_destroy_sample(player.sample[1]);
    al_destroy_sample(player.sample[2]);
    al_destroy_sample(player.sample[3]);
    al_destroy_sample(player.sample[4]);
    al_destroy_bitmap(scientist.bitmap);
    al_destroy_bitmap(shootE.bitmap[0]);
    al_destroy_bitmap(shootQ.bitmap[0]);
    al_destroy_bitmap(shootQ.bitmap[1]);
    al_destroy_bitmap(shootQ.bitmap[2]);
    al_destroy_bitmap(shootW.bitmap[0]);
    for(b=0; b<NUM_ENEMYRED; b++)
        al_destroy_bitmap(enemyred[b].image);
    for(b=0; b<NUM_ENEMYBLUE; b++)
        al_destroy_bitmap(enemyblue[b].image);

    switch(letra) {
        case 0:
        al_destroy_sample(musica0);

        for(b=0; b<background0.frame_max + 3; b++)
            al_destroy_bitmap(background0.image[b]);
        break;
        case 1:
        al_destroy_sample(musica1);
        for(b=0; b<background1.frame_max + 2; b++)
            al_destroy_bitmap(background1.image[b]);
        break;
        case 2:
        al_destroy_sample(musica2);
        for(b=0; b<background2.frame_max + 2; b++)
            al_destroy_bitmap(background2.image[b]);
        break;
        case 3:
        al_destroy_sample(musica3);
        for(b=0; b<background3.frame_max + 2; b++)
            al_destroy_bitmap(background3.image[b]);
        break;
        case 4:
        al_destroy_sample(musica4);
        for(b=0; b<background4.frame_max + 2; b++)
            al_destroy_bitmap(background4.image[b]);
        break;
        case 5:
        al_destroy_sample(musica5);
        for(b=0; b<background5.frame_max + 2; b++)
            al_destroy_bitmap(background5.image[b]);
        break;
        case 6:
        al_destroy_sample(musica6);
        for(b=0; b<background6.frame_max + 2; b++)
            al_destroy_bitmap(background6.image[b]);
        break;
        case 666:
        al_destroy_sample(musica666);
        for(b=0; b<background0.frame_max + 2; b++)
            al_destroy_bitmap(background0.image[b]);
    }

    return 0;

}//final da MAIN!!
