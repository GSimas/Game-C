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
////////////////////////////////////////////////////////////////////////////////
//GLOBALS
const int WIDTH = 1200; //largura display
const int HEIGHT = 600; //altura display
const int gravity = 1;
float *size_enemy_red;
float *size_enemy_blue;
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, E, R};
bool keys[8] = {false, false, false, false, false, false, false, false};
////////////////////////////////////////////////////////////////////////////////
//prototypes player
void InitPlayer(struct Player *player);
void DrawPlayer(struct Player *player);
void PlayerJump(struct Player *player);
void PlayerRight(struct Player *player);
void PlayerLeft(struct Player *player);
//void PlayerShootQ(struct Player *player);
//void PlayerShootW(struct Player *player);
//void PlayerShootE(struct Player *player);

//prototypes enemies
void InitEnemyRed();
void InitEnemyBlue();
void DrawEnemyRed();
void DrawEnemyBlue();
void UpdateEnemyRed();
void UpdateEnemyBlue();

//prototipos indutor "Q"
void InitShootQ(struct Shoot *shootQ);
void DrawShootQ(struct Shoot *shootQ);
void FireShootQ(struct Shoot *shootQ, struct Player *player);
void UpdateShootQ(struct Shoot *shootQ);

//prototipos capacitor "W"
void InitShootW(struct Shoot *shootW);
void DrawShootW(struct Shoot *shootW);
void FireShootW(struct Shoot *shootW, struct Player *player);
void UpdateShootW(struct Shoot *shootW);

//prototipos resistor "E"
/*void InitShootE(struct Shoot *shootE);
void DrawShootE(struct Shoot *shootE);
void FireShootE(struct Shoot *shootE);
void UpdateShootE(struct Shoot *shootE);*/

//protitpos colisão
void ShootQColisionEnemyRed();
void ShootWColisionEnemyBlue();
void PlayerColisionEnemyRed();

//outros protótipos
void ResetPlayer();

//main
int main()
{
    //primitive variables
    const int FPS = 60;
    bool done = false;
    bool redraw = true;

    //object variables
    struct Player player;
    struct Enemy_red enemyred;
    struct Enemy_blue enemyblue;
    struct Shoot shootQ;
    struct Shoot shootW;
    //struct Shoot shootE;

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
    InitEnemyRed(&enemyred); //chamar função que inicia enemyred
    InitEnemyBlue(&enemyblue); //chamar função que inicia enemyblue
    InitShootQ(&shootQ);
    InitShootW(&shootW);
    //InitShootE(&shootE);

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
            UpdateShootQ(&shootQ);
            UpdateShootW(&shootW);
            //UpdateShootE(&shootE);
            ShootQColisionEnemyRed(&shootQ,&enemyred,&size_enemy_red, &player);
            ShootWColisionEnemyBlue(&shootW,&enemyblue,&size_enemy_blue, &player);
            PlayerColisionEnemyRed(&player, &enemyred, &size_enemy_red);
            UpdateEnemyRed(&enemyred, &size_enemy_red, &player);
            UpdateEnemyBlue(&enemyblue, &size_enemy_blue, &player);
            ResetPlayer(&player);
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
                FireShootQ(&shootQ, &player);
                break;
            case ALLEGRO_KEY_W:
                keys[W] = true;
                FireShootW(&shootW, &player);
                break;
                /*case ALLEGRO_KEY_E:
                    	keys[E] = true;
                        FireShootE(&shootE);
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
            PlayerJump(&player);

            if(keys[RIGHT] == true)
            {
                PlayerRight(&player);
            }

            if(keys[LEFT] == true)
            {
                PlayerLeft(&player);
            }

            redraw = false;

            DrawPlayer(&player);
            DrawShootQ(&shootQ);
            DrawShootW(&shootW);
            //DrawShootE(&shootE);
            DrawEnemyRed(&enemyred, &size_enemy_red, &player);
            DrawEnemyBlue(&enemyblue, &size_enemy_blue, &player);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;
}//final da MAIN!!
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
    player->colision = false;
    player->alive = true;
    player->velx = 40;
    player->vely = 70;
    player->boundx = 40;
    player->boundy = 70;
    player->score = 0;
};

//função para desenhar jogador
void DrawPlayer(struct Player *player)
{
    if(player->alive)
    {
        al_draw_filled_rectangle(player->x, player->y, player->x + 40, player->y - 70 , al_map_rgb(0, 175, 255));
    }
}

//funções poder indutor "Q"//////////////////////////////////////////////////
void InitShootQ(struct Shoot *shootQ)
{
    shootQ -> live = false;
    shootQ -> speed = 10;
}
void DrawShootQ(struct Shoot *shootQ)
{
    if(shootQ -> live)
    {
        al_draw_line(((shootQ -> x)), (shootQ -> y), ((shootQ -> x)), ((shootQ -> y) - 25), al_map_rgb(0, 0, 255), 5);
    }
}
void FireShootQ(struct Shoot *shootQ, struct Player *player)
{
    if (!(shootQ -> live))
    {
        shootQ -> x = (player -> x) + 20;
        shootQ -> y = (player -> y) - 70;
        shootQ -> live = true;
    }
}
void UpdateShootQ(struct Shoot *shootQ)
{
    if(shootQ -> live)
    {
        shootQ -> y -= shootQ -> speed;
        if ((shootQ -> y) < 0)
        {
            shootQ -> live = false;
        }
    }

}

//funções poder capacitor "W"//////////////////////////////////////////////////
void InitShootW(struct Shoot *shootW)
{
    shootW -> live = false;
    shootW -> speed = 10;
}
void DrawShootW(struct Shoot *shootW)
{
    if (shootW -> live)
    {
        al_draw_line(((shootW -> x)), (shootW -> y), ((shootW -> x)), ((shootW -> y) + 25), al_map_rgb(0, 255, 0), 5);
    }
}
void FireShootW(struct Shoot *shootW, struct Player *player)
{
    if(!(shootW -> live))
    {
        shootW -> x = (player -> x) + 20;
        shootW -> y = (player -> y) - 70;
        shootW -> live = true;
    }
}
void UpdateShootW(struct Shoot *shootW)
{
    if(shootW -> live)
    {
        shootW -> y -= shootW -> speed;
        if ((shootW -> y) < 0)
        {
            shootW -> live = false;
        }
    }
}

//funções poder resistor "E"////////////////////////////////////////////////////////
/*void InitShootE(Shoot &shootE) {
    shootE -> ID = SHOOT;
    shootE -> live = false;
    shootE -> speed = 0;
}
void DrawShootE(Shoot &shootE) {
    if (shootE -> live) {
        al_draw_line((shootE -> x), (shootE -> y), ((shootE -> x)), ((shootE -> y) + 5), al_map_rgb(255, 0, 0), 40);
    }
}
void FireShootE(Shoot &shootE, Player &player) {
    if (!(shootE -> live)) {
        shootE -> x = player -> x;
        shootE -> y = player -> y;
        shootE -> live = true;
    }
}*/

//função para iniciar inimigo tipo 1 (vermelho)
void InitEnemyRed(struct Enemy_red *enemyred)
{
    enemyred->x = 800;
    enemyred->y = 200;
    enemyred->speed = 0.4;
    enemyred->velx = 0;
    enemyred->vely = 0;
    enemyred->boundx = 0;
    enemyred->boundy = 0;
    enemyred->jumping = true;
    enemyred->moving = false;
    enemyred->alive = true;
}

//função para desenhar inimigo tipo 1 (vermelho)
void DrawEnemyRed(struct Enemy_red *enemyred, float *size_enemy_red, struct Player *player)
{
    if(enemyred->alive)
    {
        al_draw_filled_circle(enemyred->x, enemyred->y, *size_enemy_red, al_map_rgb(255,0,0));
    }
    else if(enemyred->alive == false)
    {
        enemyred->x=800;
        enemyred->y=200;
        *size_enemy_red=0;
    }
}

void UpdateEnemyRed(struct Enemy_red *enemyred, float *size_enemy_red, struct Player *player)
{
    if(enemyred->alive)
    {
        if(*size_enemy_red<80)
        {
            *size_enemy_red+=enemyred->speed;
            enemyred->y+=1;
        }
    }
}

//função para iniciar inimigo tipo 2 (azul)
void InitEnemyBlue(struct Enemy_blue *enemyblue)
{
    enemyblue->x = 400;
    enemyblue->y = 300;
    enemyblue->speed = 0.5;
    enemyblue->velx = 0;
    enemyblue->vely = 0;
    enemyblue->boundx = 0;
    enemyblue->boundy = 0;
    enemyblue->jumping = true;
    enemyblue->moving = false;
    enemyblue->alive = true;
}

//função para desenhar inimigo tipo 2 (azul)
void DrawEnemyBlue(struct Enemy_blue *enemyblue, float *size_enemy_blue, struct Player *player)
{
    if(enemyblue->alive && player->score >= 6)
    {
        al_draw_filled_circle(enemyblue->x, enemyblue->y, *size_enemy_blue, al_map_rgb(255,0,255));
    }
    else if(enemyblue->alive == false)
    {
        enemyblue->x=400;
        enemyblue->y=300;
        *size_enemy_blue=0;
    }
}

void UpdateEnemyBlue(struct Enemy_blue *enemyblue, float *size_enemy_blue, struct Player *player)
{
    if(enemyblue->alive && player->score >= 6)
    {
        if(*size_enemy_blue<40)
        {
            *size_enemy_blue+=enemyblue->speed;
            enemyblue->y+=1;
        }
    }
}

//função para pulo do jogador
void PlayerJump(struct Player *player)
{
//adicionar gravidade ao pulo
    if (player->vely <= player->jumpSpeed && !player->jump)
    {
        player->vely += gravity;
        player->y += player->vely;
        if (player->vely == player->jumpSpeed)
        {
            player->y = HEIGHT;
            player->vely = 0;
            player->jump = true;
        }
    }
}

//Andar para direita
void PlayerRight(struct Player *player)
{
    player->x += player->speed;
    player->moving = true;
}

//andar para esquerda
void PlayerLeft(struct Player *player)
{
    player->x -= player->speed;
    player->moving = true;
}

//função de colisão de tiro Q com inimigo vermelho
void ShootQColisionEnemyRed(struct Shoot *shootQ, struct Enemy_red *enemyred, float *size_enemy_red, struct Player *player)
{
    enemyred->boundx = *size_enemy_red;
    enemyred->boundy = *size_enemy_red;
    if(enemyred->alive)
    {
        if(shootQ->live &&
                shootQ->x < (enemyred->x + *size_enemy_red) &&
                shootQ->x > (enemyred->x - *size_enemy_red) &&
                shootQ->y < (enemyred->y + *size_enemy_red) &&
                shootQ->y > (enemyred->y - *size_enemy_red))
        {
            enemyred->alive = false;
            player->score += 2;
        }
    }
    if(shootQ->live == false)
    {
        enemyred->alive = true;
    }
}

//função de colisão de tiro W com inimigo 1
void ShootWColisionEnemyBlue(struct Shoot *shootW, struct Enemy_blue *enemyblue, float *size_enemy_blue, struct Player *player)
{
    enemyblue->boundx = *size_enemy_blue;
    enemyblue->boundy = *size_enemy_blue;
    if(enemyblue->alive)
    {
        if(shootW->live &&
                shootW->x < (enemyblue->x + *size_enemy_blue) &&
                shootW->x > (enemyblue->x - *size_enemy_blue) &&
                shootW->y < (enemyblue->y + *size_enemy_blue) &&
                shootW->y > (enemyblue->y - *size_enemy_blue))
        {
            enemyblue->alive = false;
            player->score += 1;
        }
    }
    if(shootW->live == false)
    {
        enemyblue->alive = true;
    }
}

//função para colisão de jogador com inimigo 2
void PlayerColisionEnemyRed(struct Player *player, struct Enemy_red *enemyred, float *size_enemy_red)
{
    enemyred->boundx = *size_enemy_red;
    enemyred->boundy = *size_enemy_red;
    if(enemyred->alive && player->alive)
    {
        if((enemyred->x + *size_enemy_red) > player->x &&
                (enemyred->x - *size_enemy_red) < player->x + player->boundx &&
                (enemyred->y + *size_enemy_red) > player->y - player->boundy &&
                (enemyred->y - *size_enemy_red) < player->y)
        {
            enemyred->alive = false;
            player->score += 2;
            player->lives -= 1;
        }
    }
}

void ResetPlayer(struct Player *player)
{
    if(player->lives <= 0)
    {
        player->alive = false;
    }
    if(player->alive == false)
    {
        player->x = 10;
        player->y = HEIGHT;
        player->lives = 3;
        player->speed = 7;
        player->jumpSpeed = 15;
        player->jump = true;
        player->moving = false;
        player->colision = false;
        player->alive = true;
        player->velx = 40;
        player->vely = 70;
        player->boundx = 40;
        player->boundy = 70;
        player->score = 0;
    }
}

//void PlayerShootQ(struct Player *player); //Habilidade indutor

//void PlayerShootW(struct Player *player); //Habilidade capacitor

//void PlayerShootE(struct Player *player); //Habilidade resistencia
