/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V1.0
*Titulo: "NÃO DEFINIDO"
*********************************/

//inclusão de bibliotecas
#include <allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include <allegro5/base.h>
#include <allegro5/allegro_native_dialog.h>
#include "objects.h" //arquivo de objetos

//GLOBALS
const int WIDTH = 1200; //largura display
const int HEIGHT = 600; //altura display
const int gravity = 1;
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, E};
bool keys[7] = {false, false, false, false, false, false, false};

//prototypes
//função para chamar jogador
void InitPlayer(struct Player *player)
{
    player->ID = PLAYER;
    player->x = 10;
    player->y = HEIGHT;
    player->lives = 3;
    player->speed = 7;
    player->jumpSpeed = 15;
    player->jump = true;
    player->moving = false;
    player->velx = 0;
    player->vely = 0;
    player->boundx = 6;
    player->boundy = 7;
    player->score = 0;
};

//função para desenhar jogador
void DrawPlayer(struct Player *player)
{
    al_draw_filled_rectangle(player->x, player->y, player->x + 40, player->y - 70 , al_map_rgb(255, 0, 0));
};

//função para pulo do jogador
void PlayerJump(struct Player *player){
//adicionar gravidade ao pulo
        if (player->vely <= player->jumpSpeed && !player->jump) {
            player->vely += gravity;
            if (player->vely == player->jumpSpeed) {
                player->y = HEIGHT;
                player->vely = 0;
                player->jump = true;
            }
        }
};

//Andar para direita
void PlayerRight(struct Player *player){
        player->x += player->speed;
        player->moving = true;
};

//andar para esquerda
void PlayerLeft(struct Player *player){
        player->x -= player->speed;
        player->moving = true;
};

void PlayerShootQ(struct Player *player); //Habilidade indutor

void PlayerShootW(struct Player *player); //Habilidade capacitor

void PlayerShootE(struct Player *player); //Habilidade resistencia

//main
int main()
{
    //primitive variables
    const int FPS = 60;
    bool done = false;
    bool redraw = true;

    //object variables
    struct Player player;

    //allegro variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

////////////////////////////////////////////////////////////////////////

    //initializate functions
    if(!al_init())
        return -1; //caso dê erro ao inicializar allegro

    display = al_create_display(WIDTH,HEIGHT); //criar display

    if(!display)
        return -1; //se der merda

    //Allegro Module Init
    al_init_primitives_addon();
    al_install_keyboard();

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    //Game Init
    InitPlayer(&player); //chamar função que "inicia" player

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //se clicar para fechar a janela
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        //evento do timer (vai entrar nesse else if sempre, a não ser que feche a janela)
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if(keys[UP] && player.jump)
            {
                player.vely = -player.jumpSpeed;
                player.jump = false;
            }
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
            //if(keys[Q])
            //PlayerShootQ
            //if(keys[W])
            //PlayerShootW
            //if(keys[E])
            //PlayerShootE
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
                /*case ALLEGRO_KEY_Q:
                	keys[Q] = true;
                	break;
                case ALLEGRO_KEY_W:
                	keys[Q] = true;
                	break;
                case ALLEGRO_KEY_E:
                	keys[E] = true;
                	break;*/

            }
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                //player.vely = 0;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
                /*case ALLEGRO_KEY_Q:
                	keys[Q] = false;
                	break;
                case ALLEGRO_KEY_W:
                	keys[W] = false;
                	break;
                case ALLEGRO_KEY_E:
                	keys[E] = false;
                	break;*/
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            player.y += player.vely;

            PlayerJump(&player);

            if(keys[RIGHT] == true){
            PlayerRight(&player);}

             if(keys[LEFT] == true){
            PlayerLeft(&player);}

            redraw = false;

            DrawPlayer(&player);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;
}
