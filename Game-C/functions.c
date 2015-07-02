#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//inclusao de bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "objects.h" //arquivo de objetos

//GLOBALS
extern int WIDTH; //largura display
extern int HEIGHT; //altura display
extern const int GRAVITY = 1; //gravidade padrao em 1
extern const int FPS = 60;
extern int back_x;
extern int back_y;

extern const int TELA_INICIO = 0;
extern const int TELA_ISNTRU = 1;
extern const int TELA_JOGO = 2;
extern const int TELA_FINAL = 3;



//funcao para iniciar jogador
void InitPlayer(Player &player, int *text_color)
{
    player.ID = PLAYER;
    player.x = back_x;
    player.y = HEIGHT;
    player.lives = 5;
    player.speed = 7;
    player.jumpSpeed = 15;
    player.jump = true;
    player.moving = false;
    player.colision = false;
    player.alive = true;
    player.shield = false;
    player.velx = 0;
    player.vely = 1;
    player.boundx = 40;
    player.boundy = 40;
    player.score = 0;
    player.death_counter = 0;
    player.energy = 0;
    *text_color=0;
    player.sample[0] = al_load_sample("sounds/dead.wav");
    player.sample[1] = al_load_sample("sounds/shield.wav");
    player.sample[2] = al_load_sample("sounds/shieldoff.wav");
    player.sample[3] = al_load_sample("sounds/shieldcolision.wav");
    player.sample[4] = al_load_sample("sounds/playerdamage.wav");
    player.sample[5] = al_load_sample("sounds/lives2.ogg");
    player.sample[6] = al_load_sample("sounds/lives1.ogg");
    player.instance[0] = al_create_sample_instance(player.sample[5]);
    player.instance[1] = al_create_sample_instance(player.sample[6]);
    al_attach_sample_instance_to_mixer(player.instance[0], al_get_default_mixer());
    al_attach_sample_instance_to_mixer(player.instance[1], al_get_default_mixer());
};

void PlayerSample(Player &player, int letra, ALLEGRO_SAMPLE_ID *musica3id, ALLEGRO_SAMPLE *musica3)
{
    if(letra == 4)
    {
        if(!player.alive)
        {
            al_stop_sample_instance(player.instance[0]);
            al_play_sample(musica3, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, musica3id);
        }

        if(player.lives == 2)
        {
            al_stop_sample(musica3id);
            al_play_sample_instance(player.instance[0]);
        }
        if(player.lives == 1)
        {
            al_stop_sample_instance(player.instance[0]);
            al_play_sample_instance(player.instance[1]);
        }
    }
};

void InitScientist(SpriteScientist &scientist)
{
    scientist.frameCount = 0;
    scientist.frameDelay = 6;
    scientist.frameWidth = 62;
    scientist.frameHeight = 80;
    scientist.maxFrame = 2;
    scientist.curFrameA = 0;
    scientist.curFrameB = 0;
    scientist.curFrameC = 0;

}
void DrawScientist(Player &player, SpriteScientist &scientist, bool *LEFT, bool *RIGHT)
{
    if(player.alive)
    {
        if(*LEFT)
        {
            al_draw_bitmap_region(scientist.bitmap, scientist.curFrameB*scientist.frameWidth, 80, scientist.frameWidth, scientist.frameHeight, player.x, player.y - 80, 0);
            scientist.frameCount++;
            if(scientist.frameDelay == scientist.frameCount)
            {
                scientist.curFrameB++;
                scientist.frameCount = 0;
                if (scientist.curFrameB > scientist.maxFrame)
                {
                    scientist.curFrameB = 0;
                }
            }

        }
        else if(*RIGHT)
        {
            al_draw_bitmap_region(scientist.bitmap, scientist.curFrameC*scientist.frameWidth, 160, scientist.frameWidth, scientist.frameHeight, player.x, player.y - 80, 0);
            scientist.frameCount++;
            if(scientist.frameDelay == scientist.frameCount)
            {
                scientist.curFrameC++;
                scientist.frameCount = 0;
                if (scientist.curFrameC > scientist.maxFrame)
                {
                    scientist.curFrameC = 0;
                }
            }
        }
        else
        {
            al_draw_bitmap_region(scientist.bitmap, scientist.curFrameA*scientist.frameWidth, 240, scientist.frameWidth, scientist.frameHeight, player.x, player.y - 80, 0);
            scientist.frameCount++;
            if(scientist.frameDelay == scientist.frameCount)
            {
                scientist.curFrameA++;
                scientist.frameCount = 0;
                if (scientist.curFrameA > scientist.maxFrame)
                {
                    scientist.curFrameA = 0;
                }
            }
        }
    }
}

//funcao para pulo do jogador
void PlayerJump(struct Player &player, bool *UP)
{
    bool vely_max = false;
    if(player.vely <= player.jumpSpeed && !player.jump && *UP)
    {
        player.vely = player.jumpSpeed;
        player.jump = true;
    }
    if(player.jump && !vely_max)
    {
        player.vely -= GRAVITY;
        player.y -= player.vely;
    }
    if(player.jump && player.vely == 0)
        vely_max = true;
    if(player.jump && vely_max && player.y < HEIGHT)
    {
        player.y += player.vely;
    }
    if(player.y == HEIGHT)
    {
        player.vely = 15;
        player.jump = false;
        vely_max = false;
    }
}

//Andar para direita
void PlayerRight(struct Player &player, bool *RIGHT, struct SpriteScientist &scientist)
{
    if(*RIGHT && !((player.x + scientist.frameWidth) >= WIDTH))
    {
        player.x += player.velx;
        player.moving = true;
    }
}
//andar para esquerda
void PlayerLeft(struct Player &player, bool *LEFT)
{
    if(*LEFT && ((player.x > 1)))
    {
        player.x -= player.velx;
        player.moving = true;
    }
}

//funcao para reiniciar jogador e inimigos
void ResetPlayer(int *tela, Player &player, Enemy_red enemyred[],
    int *num_enemyred, Enemy_blue enemyblue[], int *num_enemyblue,
    Obstacle &obstacle, Boss boss[], int *num_boss, int *text_color,
    ALLEGRO_SAMPLE *musica3, ALLEGRO_SAMPLE_ID *musica3id, int letra)
{
    int j;
    if(player.lives <= 0)
    {
        player.alive = false;
        al_play_sample(player.sample[0], 0.8, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    if(player.alive == false)
    {
        *tela = TELA_FINAL;
        player.x = back_x;
        player.y = HEIGHT;
        player.lives = 5;
        player.speed = 7;
        player.jumpSpeed = 15;
        player.jump = true;
        player.moving = false;
        player.colision = false;
        player.alive = true;
        player.shield = false;
        player.velx = 0;
        player.vely = 1;
        player.boundx = 40;
        player.boundy = 50;
        player.score = 0;
        player.death_counter = 0;
        player.energy = 0;
        *text_color=0;
        for(j=0; j< *num_enemyred; j++)
        {
            enemyred[j].alive = false;
        }
        for(j=0; j < *num_enemyblue; j++)
        {
            enemyblue[j].alive = false;
        }
        for(j=0; j < *num_enemyblue; j++)
        {
            boss[j].ID = ENEMY;
            boss[j].x = back_x;
            boss[j].y = back_y;
            boss[j].real_y = boss[j].y;
            boss[j].speed = 0.1;
            boss[j].size_boss = 0;
            boss[j].width = al_get_bitmap_width(boss[j].image);
            boss[j].height = al_get_bitmap_height(boss[j].image);
            boss[j].velx = 2;
            boss[j].vely = 15;
            boss[j].boundx = 0;
            boss[j].boundy = 0;
            boss[j].real_size_boss = 100;
            boss[j].lives = 20;
            boss[j].alive = false;
            boss[j].lived = false;
            boss[j].instance_played = false;
        }
        obstacle.score = 5;
    }
}


//funcoes poder indutor "Q"//////////////////////////////////////////////////
//funcao para iniciar tiro Q
void InitShootQ(struct Shoot &shootQ, int letra)
{
    shootQ.ID = SHOOT;
    shootQ.live = false;
    shootQ.speed = 0;
    if(letra != 666)
        shootQ.bitmap[0] = al_load_bitmap("images/shootQ.png");
    if(letra == 666)
    {
        shootQ.bitmap[1] = al_load_bitmap("images/shoot_samir.png");
        shootQ.bitmap[2] = al_load_bitmap("images/shoot_mussoi.png");
    }
    shootQ.sample = al_load_sample("sounds/laser_fastshot.wav");
}

//funcao para desenhar tiro Q
void DrawShootQ(struct Shoot &shootQ, int letra, struct Boss boss[])
{
    if(shootQ.live)
    {
        if(letra != 666)
            al_draw_scaled_bitmap(shootQ.bitmap[0], 0, 0, 30, 30, shootQ.x, shootQ.y, shootQ.width, shootQ.height, 0);
        if(letra == 666)
        {
            if(!boss[0].alive &&
                    !boss[1].alive &&
                    !boss[2].alive &&
                    !boss[3].alive &&
                    !boss[4].alive)
                al_draw_scaled_bitmap(shootQ.bitmap[1], 0, 0, 30, 30, shootQ.x, shootQ.y, shootQ.width, shootQ.height, 0);
            if(boss[0].alive ||
                    boss[1].alive ||
                    boss[2].alive ||
                    boss[3].alive ||
                    boss[4].alive)
                al_draw_scaled_bitmap(shootQ.bitmap[2], 0, 0, 30, 30, shootQ.x, shootQ.y, shootQ.width, shootQ.height, 0);
        }
    }
}

//funcao para disparar tiro Q
void FireShootQ(struct Shoot &shootQ, struct Player &player)
{
    if (!(shootQ.live))
    {
        shootQ.x = (player.x);
        shootQ.y = (player.y) - 70;
        shootQ.width = 40;
        shootQ.height = 40;
        shootQ.live = true;
        al_play_sample(shootQ.sample, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

//funcao para atualizar tiro Q
void UpdateShootQ(struct Shoot &shootQ, struct Player &player)
{
    if(shootQ.live)
    {
        shootQ.speed += GRAVITY;
        shootQ.y -= shootQ.speed;
        shootQ.width *= 0.96;
        shootQ.height *= 0.96;
        shootQ.boundx = shootQ.width;
        shootQ.boundy = shootQ.height;
        if(shootQ.x > back_x)
            shootQ.x -=shootQ.speed;
        if(shootQ.x < back_x)
            shootQ.x +=shootQ.speed;
        if (shootQ.y < back_y)
        {
            shootQ.live = false;
            shootQ.speed = 0;
        }
    }
}


//funcoes poder capacitor "W"//////////////////////////////////////////////////
//funcao para iniciar tiro W
void InitShootW(struct Shoot &shootW, int letra)
{
    shootW.ID = SHOOT;
    shootW.live = false;
    shootW.speed = 10;
    if(letra != 666)
        shootW.bitmap[0] = al_load_bitmap("images/shootW.png");
    if(letra == 666)
    {
        shootW.bitmap[1] = al_load_bitmap("images/shoot_hugo.png");
        shootW.bitmap[2] = al_load_bitmap("images/shoot_mussoi.png");
    }
    shootW.sample = al_load_sample("sounds/laser_widebeam.wav");
}

//funcao para desenhar tiro W
void DrawShootW(struct Shoot &shootW, int letra, struct Boss boss[])
{if(shootW.live)
{
    if(letra != 666)
        al_draw_scaled_bitmap(shootW.bitmap[0], 0, 0, 30, 30, shootW.x, shootW.y, shootW.width, shootW.height, 0);
    if(letra == 666)
    {
        if(!boss[0].alive &&
                !boss[1].alive &&
                !boss[2].alive &&
                !boss[3].alive &&
                !boss[4].alive)
            al_draw_scaled_bitmap(shootW.bitmap[1], 0, 0, 30, 30, shootW.x, shootW.y, shootW.width, shootW.height, 0);
        if(boss[0].alive ||
                boss[1].alive ||
                boss[2].alive ||
                boss[3].alive ||
                boss[4].alive)
            al_draw_scaled_bitmap(shootW.bitmap[2], 0, 0, 30, 30, shootW.x, shootW.y, shootW.width, shootW.height, 0);
    }
}
}

//funcao para disparar tiro W
void FireShootW(struct Shoot &shootW, struct Player &player)
{
    if (!shootW.live)
    {
        shootW.x = (player.x) + 30;
        shootW.y = (player.y) - 70;
        shootW.live = true;
        shootW.width = 50;
        shootW.height = 50;
        al_play_sample(shootW.sample, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

//funcao para atualizar tiro W
void UpdateShootW(struct Shoot &shootW, struct Player &player)
{
    if(shootW.live)
    {
        shootW.speed += GRAVITY;
        shootW.y -= shootW.speed;
        shootW.width *= 0.96;
        shootW.height *= 0.96;
        shootW.boundx = shootW.width;
        shootW.boundy = shootW.height;
        if(shootW.x > back_x)
        {
            shootW.x -=shootW.speed;
        }
        if(shootW.x < back_x)
        {
            shootW.x +=shootW.speed;
        }
        if(shootW.y < back_y)
        {
            shootW.live = false;
            shootW.speed = 0;
        }
    }
}

//funçoes poder resistor "E"////////////////////////////////////////////////////////
void InitShootE(Shoot &shootE)
{
    shootE.ID = SHOOT;
    shootE.live = false;
    shootE.speed = 0;
    shootE.temp = 0;
    shootE.s = 0;
    shootE.bitmap[0] = al_load_bitmap("images/shield.png");
}

void DrawShootE(Shoot &shootE, Player &player)
{
    if (shootE.live)
    {
        shootE.x = player.x - player.boundx * 0.9;
        shootE.y = player.y - 3 * player.boundy;
        al_draw_bitmap(shootE.bitmap[0], shootE.x, shootE.y, 0);
    }
}

void FireShootE(Shoot &shootE, Player &player)
{
    if (!shootE.live && player.energy>0)
    {
        shootE.x = player.x - player.boundx*0.70;
        shootE.y = player.y - 1.45*player.boundy;
        shootE.live = true;
        shootE.temp = 5;
        player.shield = true;
        player.energy--;
        al_play_sample(player.sample[1], 0.8, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void UpdateShootE(Shoot &shootE, Player &player)
{
    if(shootE.live)
    {
        if(shootE.s < 60)
            shootE.s++;
        if(shootE.s == 60)
        {
            shootE.temp --;
            shootE.s = 0;
            if (!shootE.temp)
            {
                shootE.live = false;
                player.shield = false;
                al_play_sample(player.sample[2], 0.8, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    }
}

//funcao para iniciar inimigo tipo 1 (vermelho)
void InitEnemyRed(struct Enemy_red enemyred[], int *num_enemies, int letra)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        enemyred[j].ID = ENEMY;
        if(letra != 666)
            enemyred[j].image = al_load_bitmap("images/enemyred_0.png");
        if(letra == 666)
            enemyred[j].image = al_load_bitmap("images/samir.png");
        enemyred[j].x = back_x;
        enemyred[j].y = back_y;
        enemyred[j].width = al_get_bitmap_width(enemyred[j].image);
        enemyred[j].height = al_get_bitmap_height(enemyred[j].image);
        enemyred[j].speed = 0.001;
        enemyred[j].speed_size = 0;
        enemyred[j].speedx = 0.005;
        enemyred[j].size_enemy = 0;
        enemyred[j].velx = 0;
        enemyred[j].vely = 0;
        enemyred[j].boundx = 0;
        enemyred[j].boundy = 0;
        enemyred[j].real_size_enemy = 30;
        enemyred[j].moving = false;
        enemyred[j].alive = true;
    }
}

//funcao para desenhar inimigo tipo 1 (vermelho)
void DrawEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player, struct Sprite &enemyred_sprite)
{
    int j;
    for(j = 0; j < *num_enemies; j++)
    {
        if(enemyred[j].alive && player.alive)
        {
            if(++enemyred_sprite.frame_count >= enemyred_sprite.frame_delay)
            {
                if(++enemyred_sprite.frame_atual >= enemyred_sprite.frame_max)
                    enemyred_sprite.frame_atual = 0;
                enemyred_sprite.frame_count = 0;
            }
            al_draw_scaled_bitmap(enemyred[j].image, 0, 0, back_x, back_y, enemyred[j].x, enemyred[j].y, enemyred[j].boundx, enemyred[j].boundy, 0);
        }
        else if(enemyred[j].alive == false)
        {
            enemyred[j].x= back_x;
            enemyred[j].y = back_y;
            enemyred[j].size_enemy = 0;
            enemyred[j].velx = 0;
            enemyred[j].vely = 0;
        }
    }
}

//funcao para atualizar inimigo vermelho
void UpdateEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player, struct Shoot &shootQ)
{
    *num_enemies = 1;
    if(player.score > 10)
        *num_enemies = 2;
    if(player.score > 50)
        *num_enemies = 3;
    if(player.score > 150)
        *num_enemies = 4;
    if(player.score > 200)
        *num_enemies = 5;
    if(player.score > 250)
        *num_enemies = 6;
    if(player.score > 300)
        *num_enemies = 7;
    if(player.score > 400)
        *num_enemies = 8;
    if(player.score > 500)
        *num_enemies = 9;
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(shootQ.live == false)
        {
            enemyred[j].alive = true;
        }
        if(enemyred[j].x - enemyred[j].boundx > WIDTH ||
                enemyred[j].x + enemyred[j].boundx < 0 ||
                enemyred[j].y + enemyred[j].boundy < 0 ||
                enemyred[j].y - enemyred[j].boundy > HEIGHT)
        {
            enemyred[j].alive = false;
            player.lives--;
            al_play_sample(player.sample[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        if(enemyred[j].alive)
        {
            enemyred[j].boundx = enemyred[j].size_enemy*2;
            enemyred[j].boundy = enemyred[j].size_enemy;
            enemyred[j].velx += enemyred[j].speedx;
            enemyred[j].vely += enemyred[j].speed;
            enemyred[j].speed_size += (enemyred[j].speed + enemyred[j].speedx);
            enemyred[j].size_enemy += enemyred[j].speed_size;
            enemyred[j].y += enemyred[j].vely;
            if(enemyred[j].x < player.x)
                enemyred[j].x += enemyred[j].velx;
            if(enemyred[j].x > player.x)
                enemyred[j].x -= enemyred[j].velx;
        }
    }
}

//funcao para iniciar inimigo tipo 2 (azul)
void InitEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, int letra)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        enemyblue[j].ID = ENEMY;
        if(letra != 666)
            enemyblue[j].image = al_load_bitmap("images/enemyblue_0.png");
        if(letra == 666)
            enemyblue[j].image = al_load_bitmap("images/hugo.png");
        enemyblue[j].x = ((back_x) - 10) + (rand() % 20);
        enemyblue[j].y = back_y;
        enemyblue[j].width = al_get_bitmap_width(enemyblue[j].image);
        enemyblue[j].height = al_get_bitmap_height(enemyblue[j].image);
        enemyblue[j].speed = 0.2;
        enemyblue[j].speedx = 0.7;
        enemyblue[j].size_enemy = 0;
        enemyblue[j].velx = 0;
        enemyblue[j].vely = 0;
        enemyblue[j].boundx = 0;
        enemyblue[j].boundy = 0;
        enemyblue[j].real_size_enemy = 60;
        enemyblue[j].moving = false;
        enemyblue[j].alive = true;
    }
}

//funcao para desenhar inimigo tipo 2 (azul)
void DrawEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyblue[j].alive && player.score >= 6)
        {
            al_draw_scaled_bitmap(enemyblue[j].image, 0, 0, back_x, back_y, enemyblue[j].x, enemyblue[j].y, enemyblue[j].boundx, enemyblue[j].boundy, 0);
        }
        else if(enemyblue[j].alive == false)
        {
            enemyblue[j].x = (back_x - 10) + (rand() % 20);
            enemyblue[j].y = back_y;
            enemyblue[j].size_enemy = 0;
        }
    }
}

//funcao para atualizar inimigo azul
void UpdateEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player, struct Shoot &shootW)
{
    *num_enemies = 0;
    if(player.score > 10)
        *num_enemies = 1;
    if(player.score > 100)
        *num_enemies = 2;
    if(player.score > 200)
        *num_enemies = 3;
    if(player.score > 250)
        *num_enemies = 4;
    if(player.score > 300)
        *num_enemies = 5;
    if(player.score > 400)
        *num_enemies = 6;
    if(player.score > 500)
        *num_enemies = 7;
    if(player.score > 600)
        *num_enemies = 8;
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(shootW.live == false)
        {
            enemyblue[j].alive = true;
        }
        if(enemyblue[j].alive && ((enemyblue[j].x + enemyblue[j].boundx) > WIDTH || (enemyblue[j].x - enemyblue[j].boundx) < 0))
        {
            enemyblue[j].alive = false;
            player.lives -= 1;
            al_play_sample(player.sample[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        if(enemyblue[j].alive)
        {
            enemyblue[j].boundx = enemyblue[j].size_enemy*4;
            enemyblue[j].boundy = enemyblue[j].size_enemy*2;
            enemyblue[j].size_enemy += enemyblue[j].speed;
            enemyblue[j].velx = enemyblue[j].speedx;
            enemyblue[j].vely = enemyblue[j].speed;
            enemyblue[j].y += enemyblue[j].vely;
            if(enemyblue[j].x <= back_x)
            {
                enemyblue[j].x -= enemyblue[j].velx;
            }
            if(enemyblue[j].x > back_x)
            {
                enemyblue[j].x += enemyblue[j].velx;
            }
        }
    }
}

//funcao de colisao de tiro Q com inimigo vermelho
void ShootQColisionEnemyRed(struct Shoot &shootQ, struct Enemy_red enemyred[], int *num_enemies, struct Player &player)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyred[j].alive)
        {
            if(shootQ.live &&
                    shootQ.x < (enemyred[j].x + enemyred[j].boundx) &&
                    shootQ.x + shootQ.boundx > (enemyred[j].x) &&
                    shootQ.y < (enemyred[j].y + enemyred[j].boundy) &&
                    shootQ.y - shootQ.boundy > (enemyred[j].y))
            {
                enemyred[j].alive = false;
                player.score += 2;
                shootQ.live = false;
            }
        }
    }
}

//funcao de colisao de tiro W com inimigo azul
void ShootWColisionEnemyBlue(struct Shoot &shootW, struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyblue[j].alive)
        {
            if(shootW.live &&
                    shootW.x < (enemyblue[j].x + enemyblue[j].boundx) &&
                    shootW.x + shootW.boundx > (enemyblue[j].x) &&
                    shootW.y < (enemyblue[j].y + enemyblue[j].boundy) &&
                    shootW.y - shootW.boundy > (enemyblue[j].y))
            {
                enemyblue[j].alive = false;
                player.score += 1;
                shootW.live = false;
            }
        }
    }
}

//funcao para colisao de jogador com inimigo vermelho
void PlayerColisionEnemyRed(struct Player &player, struct Enemy_red enemyred[], int *num_enemies)
{
    int j;
    for(j=0; j< *num_enemies; j++)
    {
        if(enemyred[j].alive && player.alive)
        {
            if((enemyred[j].x + enemyred[j].boundx) > player.x &&
                    (enemyred[j].x) < player.x + player.boundx &&
                    (enemyred[j].y + enemyred[j].boundy) > player.y - player.boundy &&
                    (enemyred[j].y) < player.y)
            {
                enemyred[j].alive = false;
                player.jump = true;
                player.score += 2;
                if(player.shield)
                    al_play_sample(player.sample[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                if(!player.shield)
                {
                    player.lives -= 1;
                    if(player.lives > 0)
                        al_play_sample(player.sample[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
    }
}

//funcao para colisao de jogador com inimigo azul
void PlayerColisionEnemyBlue(struct Player &player, struct Enemy_blue enemyblue[], int *num_enemies)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyblue[j].alive && player.alive)
        {
            if((enemyblue[j].x + enemyblue[j].boundx) > player.x &&
                    (enemyblue[j].x) < player.x + player.boundx &&
                    (enemyblue[j].y + enemyblue[j].boundy) > player.y - player.boundy &&
                    (enemyblue[j].y) < player.y)
            {
                enemyblue[j].alive = false;
                player.jump = true;
                player.score += 1;
                if(player.shield)
                    al_play_sample(player.sample[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                if(!player.shield && player.lives > 1)
                {
                    player.lives -= 1;
                    if(player.lives > 0)
                        al_play_sample(player.sample[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
    }
}

//funcao para iniciar obstaculo
void InitObstacle(Obstacle &obstacle)
{
    obstacle.ID = ENEMY;
    obstacle.x = back_x;
    obstacle.y = back_y;
    obstacle.speed = 0.1;
    obstacle.velx = 1;
    obstacle.vely = 0;
    obstacle.size_obst = 0;
    obstacle.real_size_obst = 250;
    obstacle.score = 5;
    obstacle.alive = false;
}

//funcao para desenhar obstaculo
void DrawObstacle(Obstacle &obstacle)
{
    al_draw_filled_rectangle(obstacle.x, obstacle.y, obstacle.x + obstacle.size_obst, obstacle.y - 20, al_map_rgb(255, 255, 255));
}

//função para atualizar obstáculo
void UpdateObstacle(Obstacle &obstacle, ALLEGRO_FONT *medium_font, Player &player)
{
    if(player.score > 30)
        obstacle.velx = 2;
    if(player.score > 50)
        obstacle.velx = 3;
    if(player.score > 70)
        obstacle.velx = 4;
    if(player.score > 100)
        obstacle.velx = 6;
    if(player.score >= obstacle.score)
        obstacle.alive = true;
    if(obstacle.alive)
    {
        if(obstacle.y>-21 || obstacle.y < HEIGHT+21)
        {
            if(obstacle.size_obst<obstacle.real_size_obst)
                obstacle.size_obst++;
            if(obstacle.x>player.x)
                obstacle.x-=obstacle.velx;
            if(obstacle.x<player.x)
                obstacle.x+=obstacle.velx;
            obstacle.vely += obstacle.speed;
            obstacle.y+=obstacle.vely;
        }
    }
    if(obstacle.y>HEIGHT+20 || obstacle.y < -20)
    {
        obstacle.x = back_x;
        obstacle.y = back_y;
        obstacle.speed = 0.1;
        obstacle.velx = 1;
        obstacle.vely = 0;
        obstacle.size_obst = 0;
        obstacle.real_size_obst = 250;
        obstacle.score = player.score + 10;
        obstacle.alive = false;
        player.score++;
        player.energy++;
    }
}

//funcao para colisao de player com obstaculo
void PlayerColisionObstacle(Player &player, Obstacle &obstacle)
{
    if(obstacle.alive && (obstacle.y<10 || obstacle.y > HEIGHT-10))
    {
        if(player.x>=obstacle.x &&
                player.x+player.boundx <= obstacle.x+obstacle.size_obst && !player.jump)
        {
            obstacle.x = back_x;
            obstacle.y = back_y;
            obstacle.speed = 0.1;
            obstacle.velx = 1;
            obstacle.vely = 0;
            obstacle.size_obst = 0;
            obstacle.real_size_obst = 250;
            obstacle.alive = false;
            obstacle.score = player.score + 5;
            if(player.shield)
                al_play_sample(player.sample[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            if(!player.shield)
            {
                player.lives -= 1;
                if(player.lives > 0)
                    al_play_sample(player.sample[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            player.jump = true;
        }
    }
}

//funcao para escrever textos
void DrawText(ALLEGRO_FONT *title_font, ALLEGRO_FONT *medium_font, Player &player, Boss boss[], int *num_boss, int *text_color, int *text_boss, struct Obstacle &obstacle)
{
    if(*text_color > 1)
        al_draw_text(title_font, al_map_rgb(*text_color, 0, 0), WIDTH/2, 150, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive)
        {
            al_draw_textf(medium_font, al_map_rgb(255, 255, 255), 50, 100, ALLEGRO_ALIGN_LEFT, "Boss: %d", boss[j].lives);
            al_draw_textf(medium_font, al_map_rgb(*text_boss, 0, 0), WIDTH/2, 100, ALLEGRO_ALIGN_CENTRE, "Boss %d", *num_boss);
        }
    }
    al_draw_textf(medium_font, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", player.score);
    al_draw_textf(medium_font, al_map_rgb(255, 255, 255), WIDTH - 50, 20, ALLEGRO_ALIGN_RIGHT, "Lives: %d", player.lives);
    if(obstacle.y>(HEIGHT-50) &&
            player.x>=obstacle.x &&
            player.x+player.boundx <= obstacle.x+obstacle.size_obst)
        al_draw_textf(medium_font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "JUMP!");
    al_draw_textf(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE, "Energy: %d", player.energy);
}

//funcao para mudar valor de color (referente a cor de texto)
void ChangeColor(int *text_color, struct Player &player, struct Boss boss[], int *num_boss, int *text_boss)
{
    if(*text_color > 1)
        *text_color -= 1;
    if(*text_color == 0)
        *text_color = 255;
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive && *text_boss > 1)
            *text_boss -= 1;
        if(*text_boss == 0)
            *text_boss = 255;
    }
}

//funcao para iniciar boss
void InitBoss(struct Boss boss[], int *num_boss, int letra)
{int j;
for(j=0; j < *num_boss; j++)
{
    boss[j].ID = ENEMY;
    if(letra != 666)
        boss[j].image = al_load_bitmap("images/boss_1.png");
    if(letra == 666)
    {
        boss[j].image = al_load_bitmap("images/mussoi.png");
        boss[j].sample[2] = al_load_sample("sounds/starwars.ogg");
        boss[j].instance[2] = al_create_sample_instance(boss[j].sample[2]);
        al_attach_sample_instance_to_mixer(boss[j].instance[2], al_get_default_mixer());
    }
    boss[j].x = back_x;
    boss[j].y = back_y;
    boss[j].real_y = boss[j].y;
    boss[j].speed = 0.1;
    boss[j].size_boss = 0;
    boss[j].width = al_get_bitmap_width(boss[j].image);
    boss[j].height = al_get_bitmap_height(boss[j].image);
    boss[j].velx = 0.5;
    boss[j].vely = 15;
    boss[j].boundx = 0;
    boss[j].boundy = 0;
    boss[j].real_size_boss = 100;
    boss[j].lives = 20;
    boss[j].alive = false;
    boss[j].lived = false;
    boss[j].instance_played = false;

    boss[j].sample[0] = al_load_sample("sounds/songboss.ogg");
    boss[j].sample[1] = al_load_sample("sounds/songboss2.ogg");
    boss[j].instance[0] = al_create_sample_instance(boss[j].sample[0]);
    boss[j].instance[1] = al_create_sample_instance(boss[j].sample[1]);
    al_attach_sample_instance_to_mixer(boss[j].instance[0], al_get_default_mixer());
    al_attach_sample_instance_to_mixer(boss[j].instance[1], al_get_default_mixer());

}
}

//funcao para desenhar boss
void DrawBoss(struct Boss boss[], int *num_boss, struct Player &player)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive)
        {
            al_draw_scaled_bitmap(boss[j].image, 0, 0, back_x, back_y, boss[j].x, boss[j].y, boss[j].boundx, boss[j].boundy, 0);
        }
    }
}

//funcao para atualizar boss
void UpdateBoss(struct Boss boss[], int *num_boss, int *text_boss, struct Player &player, struct Enemy_red enemyred[], int *num_enemyred, struct Enemy_blue enemyblue[], int *num_enemyblue, int letra)
{
    *num_boss = 0;
    if(player.score > 20 && boss[0].lived == false)
    {
        boss[0].alive = true;
        *num_boss = 1;
    }
    if(player.score > 100 && boss[1].lived == false)
    {
        boss[1].alive = true;
        *num_boss = 2;
    }
    if(player.score > 250 && boss[2].lived == false)
    {
        boss[2].alive = true;
        *num_boss = 3;
    }
    if(player.score > 500 && boss[3].lived == false)
    {
        boss[3].alive = true;
        *num_boss = 4;
    }
    int k;
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].lives <= 0)
        {
            boss[j].alive = false;
            boss[j].lived = true;
            boss[j].x = back_x;
            boss[j].y = back_y;
            boss[j].real_y = boss[j].y;
            boss[j].speed = 0.1;
            boss[j].size_boss = 0;
            boss[j].width = al_get_bitmap_width(boss[j].image);
            boss[j].height = al_get_bitmap_height(boss[j].image);
            boss[j].velx = 0.5;
            boss[j].vely = 15;
            boss[j].boundx = 0;
            boss[j].boundy = 0;
            boss[j].real_size_boss = 100;
            boss[j].lives = 20;
            *text_boss = 0;
        }
        if(boss[j].alive)
        {
            boss[j].boundx = boss[j].size_boss*4;
            boss[j].boundy = boss[j].size_boss*2;

            for(k=0; k < *num_enemyred; k++)
                enemyred[k].alive = false;
            for(k=0; k < *num_enemyblue; k++)
                enemyblue[k].alive = false;

            if(boss[j].size_boss < boss[j].real_size_boss)
                boss[j].size_boss += boss[j].speed;

            if(boss[j].y < player.y)
                boss[j].y += boss[j].speed/2;
            if(boss[j].y > player.y)
                boss[j].y -= boss[j].speed/2;

            if(boss[j].x < player.x)
                boss[j].x+=boss[j].velx;
            if(boss[j].x > player.x)
                boss[j].x-=boss[j].velx;
        }
    }
}

void BossSample(struct Boss boss[], int *num_boss, int letra, ALLEGRO_SAMPLE_ID *musica1id, ALLEGRO_SAMPLE *musica1, ALLEGRO_SAMPLE_ID *musica666id, ALLEGRO_SAMPLE *musica666)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive)
        {
            if(boss[j].instance_played == false && letra == 4)
            {
                al_play_sample_instance(boss[j].instance[0]);
                boss[j].instance_played = true;
            }
            if(letra == 2)
            {
                al_stop_sample(musica1id);
                al_play_sample_instance(boss[j].instance[1]);
            }
             if(letra == 666)
            {
                al_stop_sample(musica666id);
                al_play_sample_instance(boss[j].instance[2]);
            }
        }
        if(boss[j].lived && !boss[j].instance_played)
        {
            if(letra == 2)
            {
                al_stop_sample_instance(boss[j].instance[1]);
                al_play_sample(musica1, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, musica1id);
                boss[j].instance_played = true;
            }
            if(letra == 666)
            {
                al_stop_sample_instance(boss[j].instance[2]);
                al_play_sample(musica666, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, musica666id);
                boss[j].instance_played = true;
            }
        }
    }
}

//funcao para colisao de player com boss
void PlayerColisionBoss(struct Player &player, struct Boss boss[], int *num_boss)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive && player.alive)
        {
            if((boss[j].x + boss[j].boundx) > player.x &&
                    boss[j].x < player.x + player.boundx &&
                    (boss[j].y + boss[j].boundy) > player.y &&
                    boss[j].y < player.y + player.boundy)
            {
                player.lives--;
                boss[j].lives--;
                boss[j].real_y = HEIGHT/2;
                boss[j].x = WIDTH * 0.1 + (rand() % WIDTH * 0.8);
                player.jump = true;
                player.score += 1;
                if(player.lives > 0)
                    al_play_sample(player.sample[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    }
}

//funcao para colisao de disparos com boss
void ShootColisionBoss(struct Shoot &shootW, struct Shoot &shootQ, struct Boss boss[], int *num_boss, struct Player &player)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive)
        {
            if(shootW.live &&
                    shootW.x < (boss[j].x + boss[j].boundx) &&
                    (shootW.x + shootW.width) > boss[j].x &&
                    shootW.y < (boss[j].y + boss[j].boundy) &&
                    (shootW.y + shootW.height) > boss[j].y)
            {
                boss[j].lives--;
                boss[j].x = WIDTH * 0.1 + (rand() % WIDTH * 0.8);
                player.score += 1;
            }
            if(shootQ.live &&
                    shootQ.x < (boss[j].x + boss[j].boundx) &&
                    (shootQ.x + shootQ.width) > boss[j].x &&
                    shootQ.y < (boss[j].y + boss[j].boundy) &&
                    (shootQ.y + shootQ.height) > boss[j].y)
            {
                boss[j].lives--;
                boss[j].x = WIDTH * 0.1 + (rand() % WIDTH * 0.8);
                player.score += 1;
            }
        }
    }
}

void InitBackground0(struct Sprite &background0, ALLEGRO_SAMPLE *musica0, int letra)
{
        background0.frame_atual = 0;
        background0.frame_count = 0;
        background0.frame_delay = 2;
        background0.frame_max = 12;

        background0.image[0] = al_load_bitmap("images/background/back_0.png");
        background0.image[1] = al_load_bitmap("images/background/back_2.png");
        background0.image[2] = al_load_bitmap("images/background/back_4.png");
        background0.image[3] = al_load_bitmap("images/background/back_6.png");
        background0.image[4] = al_load_bitmap("images/background/back_8.png");
        background0.image[5] = al_load_bitmap("images/background/back_10.png");
        background0.image[6] = al_load_bitmap("images/background/back_12.png");
        background0.image[7] = al_load_bitmap("images/background/back_14.png");
        background0.image[8] = al_load_bitmap("images/background/back_16.png");
        background0.image[9] = al_load_bitmap("images/background/back_18.png");
        background0.image[10] = al_load_bitmap("images/background/back_20.png");
        background0.image[11] = al_load_bitmap("images/background/back_22.png");

        background0.image[12] = al_load_bitmap("images/telas/tela-inicio0.png");
        background0.image[13] = al_load_bitmap("images/telas/tela-instru0.png");
        background0.image[14] = al_load_bitmap("images/telas/tela-final0.png");

        //carregar musica referente
        switch (letra) {
            case 0:
            musica0 = al_load_sample("sounds/topgearsoundtrack.ogg");
            break;
            case 666:
            musica0 = al_load_sample("sounds/treefriends.ogg");
        }

}

void DrawBackground0(struct Sprite &background0)
{
        if(++background0.frame_count >= background0.frame_delay)
        {
            if(++background0.frame_atual >= background0.frame_max)
                background0.frame_atual = 0;
            background0.frame_count = 0;
        }

        al_draw_bitmap(background0.image[background0.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground1(struct Sprite &background1, ALLEGRO_SAMPLE *musica1)
{
        background1.frame_atual = 0;
        background1.frame_count = 0;
        background1.frame_delay = 5;
        background1.frame_max = 11;

        background1.image[0] = al_load_bitmap("images/background/back5/back5 (1).png");
        background1.image[1] = al_load_bitmap("images/background/back5/back5 (3).png");
        background1.image[2] = al_load_bitmap("images/background/back5/back5 (5).png");
        background1.image[3] = al_load_bitmap("images/background/back5/back5 (7).png");
        background1.image[4] = al_load_bitmap("images/background/back5/back5 (9).png");
        background1.image[5] = al_load_bitmap("images/background/back5/back5 (11).png");
        background1.image[6] = al_load_bitmap("images/background/back5/back5 (13).png");
        background1.image[7] = al_load_bitmap("images/background/back5/back5 (15).png");
        background1.image[8] = al_load_bitmap("images/background/back5/back5 (17).png");
        background1.image[9] = al_load_bitmap("images/background/back5/back5 (19).png");
        background1.image[10] = al_load_bitmap("images/background/back5/back5 (21).png");

        background1.image[11] = al_load_bitmap("images/telas/tela-inicio1.png");
        background1.image[13] = al_load_bitmap("images/telas/tela-final1.png");
        //carregar musica referente
        musica1 = al_load_sample("sounds/nirvana.ogg");
}

void DrawBackground1(struct Sprite &background1)
{
        if(++background1.frame_count >= background1.frame_delay)
        {
            if(++background1.frame_atual >= background1.frame_max)
                background1.frame_atual = 0;
            background1.frame_count = 0;
        }

        al_draw_bitmap(background1.image[background1.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground2(struct Sprite &background2, ALLEGRO_SAMPLE *musica2)
{
        background2.frame_atual = 0;
        background2.frame_count = 0;
        background2.frame_delay = 5;
        background2.frame_max = 12;

        background2.image[0] = al_load_bitmap("images/background/back2/back2 (1).png");
        background2.image[1] = al_load_bitmap("images/background/back2/back2 (3).png");
        background2.image[2] = al_load_bitmap("images/background/back2/back2 (5).png");
        background2.image[3] = al_load_bitmap("images/background/back2/back2 (7).png");
        background2.image[4] = al_load_bitmap("images/background/back2/back2 (9).png");
        background2.image[5] = al_load_bitmap("images/background/back2/back2 (11).png");
        background2.image[6] = al_load_bitmap("images/background/back2/back2 (13).png");
        background2.image[7] = al_load_bitmap("images/background/back2/back2 (15).png");
        background2.image[8] = al_load_bitmap("images/background/back2/back2 (17).png");
        background2.image[9] = al_load_bitmap("images/background/back2/back2 (19).png");
        background2.image[10] = al_load_bitmap("images/background/back2/back2 (21).png");
        background2.image[11] = al_load_bitmap("images/background/back2/back2 (23).png");

        background2.image[12] = al_load_bitmap("images/telas/tela-inicio2-3.png");
        background2.image[14] = al_load_bitmap("images/telas/tela-final2-3.png");
        //carregar musica referente
        musica2 = al_load_sample("sounds/lucy.ogg");
}

void DrawBackground2(struct Sprite &background2)
{
        if(++background2.frame_count >= background2.frame_delay)
        {
            if(++background2.frame_atual >= background2.frame_max)
                background2.frame_atual = 0;
            background2.frame_count = 0;
        }

        al_draw_bitmap(background2.image[background2.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground3(struct Sprite &background3, ALLEGRO_SAMPLE *musica3)
{
        background3.frame_atual = 0;
        background3.frame_count = 0;
        background3.frame_delay = 5;
        background3.frame_max = 11;

        background3.image[0] = al_load_bitmap("images/background/back7/back7 (2).png");
        background3.image[1] = al_load_bitmap("images/background/back7/back7 (4).png");
        background3.image[2] = al_load_bitmap("images/background/back7/back7 (6).png");
        background3.image[3] = al_load_bitmap("images/background/back7/back7 (8).png");
        background3.image[4] = al_load_bitmap("images/background/back7/back7 (10).png");
        background3.image[5] = al_load_bitmap("images/background/back7/back7 (12).png");
        background3.image[6] = al_load_bitmap("images/background/back7/back7 (14).png");
        background3.image[7] = al_load_bitmap("images/background/back7/back7 (16).png");
        background3.image[8] = al_load_bitmap("images/background/back7/back7 (18).png");
        background3.image[9] = al_load_bitmap("images/background/back7/back7 (20).png");
        background3.image[10] = al_load_bitmap("images/background/back7/back7 (22).png");

        background3.image[11] = al_load_bitmap("images/telas/tela-inicio2-3.png");
        background3.image[13] = al_load_bitmap("images/telas/tela-final2-3.png");
        //carregar musica referente
        musica3 = al_load_sample("sounds/immigrant.ogg");
}

void DrawBackground3(struct Sprite &background3)
{
        if(++background3.frame_count >= background3.frame_delay)
        {
            if(++background3.frame_atual >= background3.frame_max)
                background3.frame_atual = 0;
            background3.frame_count = 0;
        }

        al_draw_bitmap(background3.image[background3.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground4(struct Sprite &background4, ALLEGRO_SAMPLE *musica4)
{
        background4.frame_atual = 0;
        background4.frame_count = 0;
        background4.frame_delay = 5;
        background4.frame_max = 11;

        background4.image[0] = al_load_bitmap("images/background/back8/back8 (1).png");
        background4.image[1] = al_load_bitmap("images/background/back8/back8 (3).png");
        background4.image[2] = al_load_bitmap("images/background/back8/back8 (5).png");
        background4.image[3] = al_load_bitmap("images/background/back8/back8 (7).png");
        background4.image[4] = al_load_bitmap("images/background/back8/back8 (9).png");
        background4.image[5] = al_load_bitmap("images/background/back8/back8 (11).png");
        background4.image[6] = al_load_bitmap("images/background/back8/back8 (13).png");
        background4.image[7] = al_load_bitmap("images/background/back8/back8 (15).png");
        background4.image[8] = al_load_bitmap("images/background/back8/back8 (17).png");
        background4.image[9] = al_load_bitmap("images/background/back8/back8 (19).png");
        background4.image[10] = al_load_bitmap("images/background/back8/back8 (21).png");

        background4.image[11] = al_load_bitmap("images/telas/tela-inicio4.png");
        background4.image[13] = al_load_bitmap("images/telas/tela-final4.png");
        //carregar musica referente
        musica4 = al_load_sample("sounds/melancholy.ogg");
}

void DrawBackground4(struct Sprite &background4)
{
        if(++background4.frame_count >= background4.frame_delay)
        {
            if(++background4.frame_atual >= background4.frame_max)
                background4.frame_atual = 0;
            background4.frame_count = 0;
        }

        al_draw_bitmap(background4.image[background4.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground5(struct Sprite &background5, ALLEGRO_SAMPLE *musica5)
{
        background5.frame_atual = 0;
        background5.frame_count = 0;
        background5.frame_delay = 5;
        background5.frame_max = 20;

        background5.image[0] = al_load_bitmap("images/background/back6/back6 (1).png");
        background5.image[1] = al_load_bitmap("images/background/back6/back6 (3).png");
        background5.image[2] = al_load_bitmap("images/background/back6/back6 (5).png");
        background5.image[3] = al_load_bitmap("images/background/back6/back6 (7).png");
        background5.image[4] = al_load_bitmap("images/background/back6/back6 (9).png");
        background5.image[5] = al_load_bitmap("images/background/back6/back6 (11).png");
        background5.image[6] = al_load_bitmap("images/background/back6/back6 (13).png");
        background5.image[7] = al_load_bitmap("images/background/back6/back6 (15).png");
        background5.image[8] = al_load_bitmap("images/background/back6/back6 (17).png");
        background5.image[9] = al_load_bitmap("images/background/back6/back6 (19).png");
        background5.image[10] = al_load_bitmap("images/background/back6/back6 (21).png");
        background5.image[11] = al_load_bitmap("images/background/back6/back6 (23).png");
        background5.image[12] = al_load_bitmap("images/background/back6/back6 (25).png");
        background5.image[13] = al_load_bitmap("images/background/back6/back6 (27).png");
        background5.image[14] = al_load_bitmap("images/background/back6/back6 (29).png");
        background5.image[15] = al_load_bitmap("images/background/back6/back6 (31).png");
        background5.image[16] = al_load_bitmap("images/background/back6/back6 (33).png");
        background5.image[17] = al_load_bitmap("images/background/back6/back6 (35).png");
        background5.image[18] = al_load_bitmap("images/background/back6/back6 (37).png");
        background5.image[19] = al_load_bitmap("images/background/back6/back6 (39).png");

        background5.image[20] = al_load_bitmap("images/telas/tela-inicio5.png");
        background5.image[22] = al_load_bitmap("images/telas/tela-final5.png");
        //carregar musica referente
        musica5 = al_load_sample("sounds/starwars.ogg");
}

void DrawBackground5(struct Sprite &background5)
{
        if(++background5.frame_count >= background5.frame_delay)
        {
            if(++background5.frame_atual >= background5.frame_max)
                background5.frame_atual = 0;
            background5.frame_count = 0;
        }

        al_draw_bitmap(background5.image[background5.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}

void InitBackground6(struct Sprite &background6, ALLEGRO_SAMPLE *musica6)
{
        background6.frame_atual = 0;
        background6.frame_count = 0;
        background6.frame_delay = 5;
        background6.frame_max = 21;

        background6.image[0] = al_load_bitmap("images/background/back9/back9 (1).png");
        background6.image[1] = al_load_bitmap("images/background/back9/back9 (5).png");
        background6.image[2] = al_load_bitmap("images/background/back9/back9 (10).png");
        background6.image[3] = al_load_bitmap("images/background/back9/back9 (15).png");
        background6.image[4] = al_load_bitmap("images/background/back9/back9 (20).png");
        background6.image[5] = al_load_bitmap("images/background/back9/back9 (25).png");
        background6.image[6] = al_load_bitmap("images/background/back9/back9 (30).png");
        background6.image[7] = al_load_bitmap("images/background/back9/back9 (35).png");
        background6.image[8] = al_load_bitmap("images/background/back9/back9 (40).png");
        background6.image[9] = al_load_bitmap("images/background/back9/back9 (45).png");
        background6.image[10] = al_load_bitmap("images/background/back9/back9 (50).png");
        background6.image[11] = al_load_bitmap("images/background/back9/back9 (55).png");
        background6.image[12] = al_load_bitmap("images/background/back9/back9 (60).png");
        background6.image[13] = al_load_bitmap("images/background/back9/back9 (65).png");
        background6.image[14] = al_load_bitmap("images/background/back9/back9 (70).png");
        background6.image[15] = al_load_bitmap("images/background/back9/back9 (75).png");
        background6.image[16] = al_load_bitmap("images/background/back9/back9 (80).png");
        background6.image[17] = al_load_bitmap("images/background/back9/back9 (85).png");
        background6.image[18] = al_load_bitmap("images/background/back9/back9 (90).png");
        background6.image[19] = al_load_bitmap("images/background/back9/back9 (95).png");
        background6.image[20] = al_load_bitmap("images/background/back9/back9 (99).png");

        background6.image[21] = al_load_bitmap("images/telas/tela-inicio6.png");
        background6.image[23] = al_load_bitmap("images/telas/tela-final6.png");
        //carregar musica referente
        musica6 = al_load_sample("sounds/superman.ogg");
}

void DrawBackground6(struct Sprite &background6)
{
        if(++background6.frame_count >= background6.frame_delay)
        {
            if(++background6.frame_atual >= background6.frame_max)
                background6.frame_atual = 0;
            background6.frame_count = 0;
        }

        al_draw_bitmap(background6.image[background6.frame_atual], 0, 0, ALLEGRO_ALIGN_CENTRE);
}


void InitEnemyredSprite(struct Sprite &enemyred_sprite)
{
    enemyred_sprite.frame_atual = 0;
    enemyred_sprite.frame_count = 0;
    enemyred_sprite.frame_delay = 1;
    enemyred_sprite.frame_max = 1;

    enemyred_sprite.image[0] = al_load_bitmap("images/enemyred_0.png");
}

void OpcaoBackground(int &letra)
{
    printf("Digite o numero da opcao e tecle Enter\n 0 - Normal\n 1 - Tunel de espinhos\n 2 - Terra da Speranza (LSD World) \n 3 - Paz e Amor\n 4 - Luz, luz!\n 5 - Preto no Branco\n 6 - Tudo azul...\n");
    scanf("%d", &letra);

    switch(letra){
        case 0:
        WIDTH = 1200;
        HEIGHT = 600;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 1:
        WIDTH = 500;
        HEIGHT = 655;
        back_x = WIDTH*0.3;
        back_y = HEIGHT*0.5;
        break;

        case 2:
        WIDTH = 400;
        HEIGHT = 500;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 3:
        WIDTH = 400;
        HEIGHT = 500;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 4:
        WIDTH = 500;
        HEIGHT = 500;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 5:
        WIDTH = 500;
        HEIGHT = 711;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 6:
        WIDTH = 400;
        HEIGHT = 400;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        case 666:
        WIDTH = 1200;
        HEIGHT = 600;
        back_x = WIDTH/2;
        back_y = HEIGHT/2;
        break;

        default:
        printf("Incorreto!");
        break;
    } //final do switch

}

#endif // FUNCTIONS_H_INCLUDED
