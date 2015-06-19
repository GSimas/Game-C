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
#include "objects.h" //arquivo de objetos

//GLOBALS
extern const int WIDTH = 1200; //largura display
extern const int HEIGHT = 600; //altura display
extern const int GRAVITY = 1; //gravidade padrao em 1

//funcao para iniciar jogador
void InitPlayer(Player &player, int *text_color)
{
    player.ID = PLAYER;
    player.x = WIDTH/2;
    player.y = HEIGHT/2;
    player.lives = 3;
    player.speed = 7;
    player.jumpSpeed = 15;
    player.jump = true;
    player.moving = false;
    player.colision = false;
    player.alive = true;
    player.inverted = false;
    player.shield = false;
    player.velx = 0;
    player.vely = 1;
    player.boundx = 40;
    player.boundy = 70;
    player.score = 0;
    player.death_counter = 0;
    *text_color=0;
};

//funcao para desenhar jogador
void DrawPlayer(struct Player &player)
{
    if(player.alive)
    {
        al_draw_filled_rectangle(player.x, player.y, player.x + 40, player.y - 70 , al_map_rgb(0, 175, 255));
    }
}

//funcao para pulo do jogador
void PlayerJump(struct Player &player, bool *UP)
{
    bool vely_max = false;
    //pulo nao invertido
    if(!player.inverted)
    {
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
    //pulo invertido
    if(player.inverted)
    {
        if(player.vely <= player.jumpSpeed && !player.jump && *UP)
        {
            player.vely = player.jumpSpeed;
            player.jump = true;
        }
        if(player.jump && !vely_max)
        {
            player.vely -= GRAVITY;
            player.y += player.vely;
        }
        if(player.jump && player.vely == 0)
            vely_max = true;
        if(player.jump && vely_max && player.y - player.boundy > 0)
        {
            player.y -= player.vely;
        }
        if(player.y - player.boundy == 0)
        {
            player.vely = 15;
            player.jump = false;
            vely_max = false;
        }
    }
}

//Andar para direita
void PlayerRight(struct Player &player, bool *RIGHT)
{
    if(*RIGHT)
    {
        player.x += player.velx;
        player.moving = true;
    }
}

//andar para esquerda
void PlayerLeft(struct Player &player, bool *LEFT)
{
    if(*LEFT)
    {
        player.x -= player.velx;
        player.moving = true;
    }
}

//funcao para reiniciar jogador e inimigos
void ResetPlayer(Player &player, Enemy_red enemyred[], int *num_enemyred, Enemy_blue enemyblue[], int *num_enemyblue, Obstacle &obstacle, Boss boss[], int *num_boss, int *text_color)
{
    int j;
    if(player.lives <= 0)
    {
        player.alive = false;
    }
    if(player.alive == false)
    {
        player.x = WIDTH/2;
        player.y = HEIGHT/2;
        player.lives = 3;
        player.speed = 7;
        player.jumpSpeed = 15;
        player.jump = true;
        player.moving = false;
        player.colision = false;
        player.alive = true;
        player.inverted = false;
        player.shield = false;
        player.velx = 0;
        player.vely = 1;
        player.boundx = 40;
        player.boundy = 70;
        player.score = 0;
        player.death_counter = 0;
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
            boss[j].alive = false;
            boss[j].lived = false;
        }
        obstacle.score = 5;
    }
}

//funcao para teleportar player (inverter)
void TransportPlayer(struct Player &player)
{
    if(!player.inverted && !player.jump)
    {
        if(player.x < -150)
        {
            player.y = player.boundy;
            player.x = WIDTH - player.boundx/2;
            player.inverted = true;
            player.death_counter = 0;
        }
        if(player.x > WIDTH + 150)
        {
            player.y = player.boundy;
            player.x = -(player.boundx/2);
            player.inverted = true;
            player.death_counter = 0;
        }
    }
    if(player.inverted && !player.jump)
    {
        if(player.x < -150)
        {
            player.y = HEIGHT;
            player.x = WIDTH - player.boundx/2;
            player.inverted = false;
            player.death_counter = 0;
        }
        if(player.x > WIDTH + 150)
        {
            player.y = HEIGHT;
            player.x = -(player.boundx/2);
            player.inverted = false;

        }
    }
}

//funcoes poder indutor "Q"//////////////////////////////////////////////////
//funcao para iniciar tiro Q
void InitShootQ(struct Shoot &shootQ)
{
    shootQ.ID = SHOOTQ;
    shootQ.live = false;
    shootQ.speed = 0;
}

//funcao para desenhar tiro Q
void DrawShootQ(struct Shoot &shootQ)
{
    if(shootQ.live)
    {
        al_draw_line(((shootQ.x)), (shootQ.y), ((shootQ.x)), ((shootQ.y) - 25), al_map_rgb(0, 0, 255), 5);
    }
}

//funcao para disparar tiro Q
void FireShootQ(struct Shoot &shootQ, struct Player &player)
{
    if (!(shootQ.live))
    {
        if(!player.inverted)
        {
            shootQ.x = (player.x) + 20;
            shootQ.y = (player.y) - 70;
            shootQ.live = true;
        }
        if(player.inverted)
        {
            shootQ.x = (player.x) + 20;
            shootQ.y = (player.y) + 20;
            shootQ.live = true;
        }
    }
}

//funcao para atualizar tiro Q
void UpdateShootQ(struct Shoot &shootQ, struct Player &player)
{
    if(shootQ.live)
    {
        if(!player.inverted)
        {
            shootQ.speed += GRAVITY;
            shootQ.y -= shootQ.speed;
            if ((shootQ.y) < 0)
            {
                shootQ.live = false;
                shootQ.speed = 0;
            }
        }
        if(player.inverted)
        {
            shootQ.speed += GRAVITY;
            shootQ.y += shootQ.speed;
            if ((shootQ.y) > HEIGHT)
            {
                shootQ.live = false;
                shootQ.speed = 0;
            }
        }
    }
}

//funcoes poder capacitor "W"//////////////////////////////////////////////////
//funcao para iniciar tiro W
void InitShootW(struct Shoot &shootW)
{
    shootW.ID = SHOOTW;
    shootW.live = false;
    shootW.speed = 10;
}

//funcao para desenhar tiro W
void DrawShootW(struct Shoot &shootW)
{
    if (shootW.live)
    {
        al_draw_line(((shootW.x)), (shootW.y), ((shootW.x)), ((shootW.y) + 25), al_map_rgb(0, 255, 0), 5);
    }
}

//funcao para disparar tiro W
void FireShootW(struct Shoot &shootW, struct Player &player)
{
    if (!(shootW.live))
    {
        if(!player.inverted)
        {
            shootW.x = (player.x) + 20;
            shootW.y = (player.y) - 70;
            shootW.live = true;
        }
        if(player.inverted)
        {
            shootW.x = (player.x) + 20;
            shootW.y = (player.y) + 20;
            shootW.live = true;
        }
    }
}

//funcao para atualizar tiro W
void UpdateShootW(struct Shoot &shootW, struct Player &player)
{
    if(shootW.live)
    {
        if(!player.inverted)
        {
            shootW.speed += GRAVITY;
            shootW.y -= shootW.speed;
            if ((shootW.y) < 0)
            {
                shootW.live = false;
                shootW.speed = 0;
            }
        }
        if(player.inverted)
        {
            shootW.speed += GRAVITY;
            shootW.y += shootW.speed;
            if ((shootW.y) > HEIGHT)
            {
                shootW.live = false;
                shootW.speed = 0;
            }
        }
    }
}

//funçoes poder resistor "E"////////////////////////////////////////////////////////
void InitShootE(Shoot &shootE)
{
    shootE.ID = SHOOTE;
    shootE.live = false;
    shootE.speed = 0;
    shootE.temp = 0;
    shootE.s = 0;
}
void DrawShootE(Shoot &shootE, Player &player)
{
    if (shootE.live)
    {
        shootE.x = player.x;
        shootE.y = player.y - player.boundy;
        al_draw_line((shootE.x), (shootE.y), ((shootE.x) + 40), (shootE.y), al_map_rgb(255, 0, 0), 20);
    }
}
void FireShootE(Shoot &shootE, Player &player)
{
    if (!(shootE.live))
    {
        shootE.x = player.x;
        shootE.y = player.y - 70;
        shootE.live = true;
        shootE.temp = 3;
        player.shield = true;
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
            }
        }
    }
}

//funcao para iniciar inimigo tipo 1 (vermelho)
void InitEnemyRed(struct Enemy_red enemyred[], int *num_enemies)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        enemyred[j].ID = ENEMYRED;
        enemyred[j].x = 350 + (rand() % 500);
        enemyred[j].y = 150;
        enemyred[j].speed = 0.005;
        enemyred[j].size_enemy = 0;
        enemyred[j].velx = 0;
        enemyred[j].vely = 0;
        enemyred[j].boundx = 0;
        enemyred[j].boundy = 0;
        enemyred[j].real_size_enemy = 80;
        enemyred[j].moving = false;
        enemyred[j].alive = true;
    }
}

//funcao para desenhar inimigo tipo 1 (vermelho)
void DrawEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyred[j].alive && player.alive)
        {
            al_draw_filled_circle(enemyred[j].x, enemyred[j].y, enemyred[j].size_enemy, al_map_rgb(enemyred[j].size_enemy + 150, 0, 0));
            al_draw_filled_rectangle(enemyred[j].x + 20, enemyred[j].y - 10, enemyred[j].x + enemyred[j].size_enemy, enemyred[j].y - enemyred[j].size_enemy, al_map_rgb(enemyred[j].size_enemy+100, enemyred[j].size_enemy, 0));
            al_draw_filled_rectangle(enemyred[j].x - 20, enemyred[j].y, enemyred[j].x - enemyred[j].size_enemy, enemyred[j].y + enemyred[j].size_enemy, al_map_rgb(enemyred[j].size_enemy+100, enemyred[j].size_enemy, 0));

        }
        else if(enemyred[j].alive == false)
        {
            enemyred[j].x= 350 + (rand() % 500);
            if(player.inverted == false)
                enemyred[j].y = 150;
            if(player.inverted)
                enemyred[j].y = 400;
            enemyred[j].size_enemy=0;
            enemyred[j].velx = 0;
        }
    }
}

//funcao para atualizar inimigo vermelho
void UpdateEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player)
{
    *num_enemies = 1;
    if(player.score > 10)
        *num_enemies = 2;
    if(player.score > 30)
        *num_enemies = 3;
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyred[j].x - enemyred[j].boundx > WIDTH ||
                enemyred[j].x + enemyred[j].boundx < 0 ||
                enemyred[j].y + enemyred[j].boundy < 0 ||
                enemyred[j].y - enemyred[j].boundy > HEIGHT)
            enemyred[j].alive = false;
        if(enemyred[j].alive)
        {
            enemyred[j].size_enemy+=enemyred[j].velx;
            enemyred[j].velx+=enemyred[j].speed;
            if(!player.inverted)
                enemyred[j].y+=1.5;
            if(player.inverted)
                enemyred[j].y-=1.5;
            if(enemyred[j].x > player.x)
                enemyred[j].x-=enemyred[j].velx;
            if(enemyred[j].x < player.x)
                enemyred[j].x+=enemyred[j].velx;
        }
    }
}

//funcao para iniciar inimigo tipo 2 (azul)
void InitEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        enemyblue[j].ID = ENEMYBLUE;
        enemyblue[j].x = 400 + (rand() % 400);
        enemyblue[j].y = 200;
        enemyblue[j].speed = 0.5;
        enemyblue[j].size_enemy = 0;
        enemyblue[j].velx = 0;
        enemyblue[j].vely = 0;
        enemyblue[j].boundx = 0;
        enemyblue[j].boundy = 0;
        enemyblue[j].real_size_enemy = 40;
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
            al_draw_filled_circle(enemyblue[j].x, enemyblue[j].y, enemyblue[j].size_enemy, al_map_rgb(0, enemyblue[j].size_enemy * 6, enemyblue[j].size_enemy * 6));
            //al_draw_filled_rectangle(enemyblue[j].x + 10, enemyblue[j].y - 5, enemyblue[j].x + enemyblue[j].size_enemy, enemyblue[j].y - enemyblue[j].size_enemy, al_map_rgb(150, enemyblue[j].size_enemy, enemyblue[j].size_enemy + 100));
            //al_draw_filled_rectangle(enemyblue[j].x - 10, enemyblue[j].y - 5, enemyblue[j].x - enemyblue[j].size_enemy, enemyblue[j].y + enemyblue[j].size_enemy, al_map_rgb(enemyblue[j].size_enemy + 100, enemyblue[j].size_enemy + 100, enemyblue[j].size_enemy));
        }
        else if(enemyblue[j].alive == false)
        {
            enemyblue[j].x= 400 + (rand() % 400);
            if(!player.inverted)
                enemyblue[j].y = 200;
            if(player.inverted)
                enemyblue[j].y = 400;
            enemyblue[j].size_enemy=0;
        }
    }
}

//funcao para atualizar inimigo azul
void UpdateEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player)
{
    *num_enemies = 1;
    if(player.score > 10)
        *num_enemies = 2;
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        if(enemyblue[j].x + enemyblue[j].boundx > WIDTH || enemyblue[j].x - enemyblue[j].boundx < 0)
            enemyblue[j].alive = false;
        if(enemyblue[j].alive && player.score >= 6)
        {
            if(enemyblue[j].size_enemy<enemyblue[j].real_size_enemy)
            {
                enemyblue[j].size_enemy+=enemyblue[j].speed;
                if(!player.inverted)
                    enemyblue[j].y+=1;
                if(player.inverted)
                    enemyblue[j].y-=1;
                if(enemyblue[j].x<WIDTH/2)
                {
                    if(enemyblue[j].size_enemy<enemyblue[j].real_size_enemy/2)
                    {
                        enemyblue[j].x-=0.6;
                    }
                    if(enemyblue[j].size_enemy>=enemyblue[j].real_size_enemy/2)
                    {
                        enemyblue[j].x+=1.5;
                    }
                }
                if(enemyblue[j].x>WIDTH/2)
                {
                    if(enemyblue[j].size_enemy<enemyblue[j].real_size_enemy/2)
                    {
                        enemyblue[j].x+=0.6;
                    }
                    if(enemyblue[j].size_enemy>=enemyblue[j].real_size_enemy/2)
                    {
                        enemyblue[j].x-=1.5;
                    }
                }
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
        enemyred[j].boundx = enemyred[j].size_enemy;
        enemyred[j].boundy = enemyred[j].size_enemy;
        if(enemyred[j].alive)
        {
            if(shootQ.live &&
                    shootQ.x < (enemyred[j].x + enemyred[j].size_enemy) &&
                    shootQ.x > (enemyred[j].x - enemyred[j].size_enemy) &&
                    shootQ.y < (enemyred[j].y + enemyred[j].size_enemy) &&
                    shootQ.y > (enemyred[j].y - enemyred[j].size_enemy))
            {
                enemyred[j].alive = false;
                player.score += 2;
            }
        }
        if(shootQ.live == false)
        {
            enemyred[j].alive = true;
        }
    }
}

//funcao de colisao de tiro W com inimigo azul
void ShootWColisionEnemyBlue(struct Shoot &shootW, struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player)
{
    int j;
    for(j=0; j < *num_enemies; j++)
    {
        enemyblue[j].boundx = enemyblue[j].size_enemy;
        enemyblue[j].boundy = enemyblue[j].size_enemy;
        if(enemyblue[j].alive)
        {
            if(shootW.live &&
                    shootW.x < (enemyblue[j].x + enemyblue[j].size_enemy) &&
                    shootW.x > (enemyblue[j].x - enemyblue[j].size_enemy) &&
                    shootW.y < (enemyblue[j].y + enemyblue[j].size_enemy) &&
                    shootW.y > (enemyblue[j].y - enemyblue[j].size_enemy))
            {
                enemyblue[j].alive = false;
                player.score += 1;
            }
        }
        if(shootW.live == false)
        {
            enemyblue[j].alive = true;
        }
    }
}

//funcao para colisao de jogador com inimigo vermelho
void PlayerColisionEnemyRed(struct Player &player, struct Enemy_red enemyred[], int *num_enemies)
{
    int j;
    for(j=0; j< *num_enemies; j++)
    {
        enemyred[j].boundx = enemyred[j].size_enemy;
        enemyred[j].boundy = enemyred[j].size_enemy;
        if(enemyred[j].alive && player.alive)
        {
            if((enemyred[j].x + enemyred[j].size_enemy) > player.x &&
                    (enemyred[j].x - enemyred[j].size_enemy) < player.x + player.boundx &&
                    (enemyred[j].y + enemyred[j].size_enemy) > player.y - player.boundy &&
                    (enemyred[j].y - enemyred[j].size_enemy) < player.y)
            {
                enemyred[j].alive = false;
                player.jump = true;
                player.score += 2;
                if(!player.shield)
                    player.lives -= 1;
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
            enemyblue[j].boundx = enemyblue[j].size_enemy;
            enemyblue[j].boundy = enemyblue[j].size_enemy;
            if((enemyblue[j].x + enemyblue[j].size_enemy) > player.x &&
                    (enemyblue[j].x - enemyblue[j].size_enemy) < player.x + player.boundx &&
                    (enemyblue[j].y + enemyblue[j].size_enemy) > player.y - player.boundy &&
                    (enemyblue[j].y - enemyblue[j].size_enemy) < player.y)
            {
                enemyblue[j].alive = false;
                player.jump = true;
                player.score += 1;
                if(!player.shield)
                player.lives -= 1;
            }
        }
    }
}

//funcao para iniciar obstaculo
void InitObstacle(Obstacle &obstacle)
{
    obstacle.ID = OBSTACLE;
    obstacle.x = WIDTH/2;
    obstacle.y = HEIGHT/2;
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
            if(!player.inverted)
            {
                obstacle.vely += obstacle.speed;
                obstacle.y+=obstacle.vely;
                if(obstacle.y>(HEIGHT-50) &&
                        player.x>=obstacle.x &&
                        player.x+player.boundx <= obstacle.x+obstacle.size_obst)
                    al_draw_textf(medium_font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "JUMP!");
            }
            if(player.inverted)
            {
                obstacle.vely += obstacle.speed;
                obstacle.y-=obstacle.vely;
                if(obstacle.y<(50) &&
                        player.x>=obstacle.x &&
                        player.x+player.boundx <= obstacle.x+obstacle.size_obst)
                    al_draw_textf(medium_font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTER, "JUMP!");
            }
        }
    }
    if(obstacle.y>HEIGHT+20 || obstacle.y < -20)
    {
        obstacle.x = 200 + (rand() % 400);
        obstacle.y = HEIGHT/2;
        obstacle.speed = 0.1;
        obstacle.velx = 1;
        obstacle.vely = 0;
        obstacle.size_obst = 0;
        obstacle.real_size_obst = 250;
        obstacle.score = player.score + 10;
        obstacle.alive = false;
        player.score++;
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
            obstacle.x = 200 + (rand() % 400);
            obstacle.y = HEIGHT/2;
            obstacle.speed = 0.1;
            obstacle.velx = 1;
            obstacle.vely = 0;
            obstacle.size_obst = 0;
            obstacle.real_size_obst = 250;
            obstacle.alive = false;
            obstacle.score = player.score + 5;
            if(!player.shield)
            player.lives-=1;
            player.jump = true;
        }
    }
}

//funcao para escrever textos
void DrawText(ALLEGRO_FONT *title_font, ALLEGRO_FONT *medium_font, Player &player, Boss boss[], int *num_boss, int *text_color, int *text_boss)
{
    al_clear_to_color(al_map_rgb(*text_color*2, *text_color*2, *text_color*2));
    al_draw_text(title_font, al_map_rgb(*text_color, 0, 0), WIDTH/2, 150, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].alive)
        {
            al_clear_to_color(al_map_rgb(*text_boss * 2, *text_boss * 2, *text_boss * 2));
            al_draw_textf(medium_font, al_map_rgb(255, 255, 255), 50, 100, ALLEGRO_ALIGN_LEFT, "Boss: %d", boss[j].lives);
            al_draw_textf(medium_font, al_map_rgb(*text_boss, 0, 0), WIDTH/2, 100, ALLEGRO_ALIGN_CENTER, "Boss %d", *num_boss);
        }
    }
    al_draw_textf(medium_font, al_map_rgb(255, 255, 255), 50, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", player.score);
    al_draw_textf(medium_font, al_map_rgb(255, 255, 255), WIDTH - 50, 20, ALLEGRO_ALIGN_RIGHT, "Lives: %d", player.lives);
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
void InitBoss(struct Boss boss[], int *num_boss)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        boss[j].ID = BOSS;
        boss[j].x = WIDTH/2;
        boss[j].y = 400;
        boss[j].real_y = boss[j].y;
        boss[j].speed = 0.1;
        boss[j].size_boss = 0;
        boss[j].velx = 2;
        boss[j].vely = 15;
        boss[j].boundx = 0;
        boss[j].boundy = 0;
        boss[j].real_size_boss = 100;
        boss[j].lives = 20;
        boss[j].alive = false;
        boss[j].lived = false;
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
            al_draw_filled_circle(boss[j].x, boss[j].y, boss[j].size_boss, al_map_rgb(255,0,255));
            //al_draw_filled_rectangle(boss[j].x + 30, boss[j].y - 5, boss[j].x + boss[j].size_boss, boss[j].y - boss[j].size_boss, al_map_rgb(boss[j].size_boss + 100, boss[j].size_boss, 0));
            //al_draw_filled_rectangle(boss[j].x - 40, boss[j].y, boss[j].x - boss[j].size_boss, boss[j].y + boss[j].size_boss, al_map_rgb(boss[j].size_boss + 100, 50, boss[j].size_boss));
        }
    }
}

//funcao para atualizar boss
void UpdateBoss(struct Boss boss[], int *num_boss, int *text_boss, struct Player &player, struct Enemy_red enemyred[], int *num_enemyred, struct Enemy_blue enemyblue[], int *num_enemyblue)
{
    *num_boss = 0;
    if(player.score > 20 && boss[0].lived == false)
    {
        boss[0].alive = true;
        *num_boss = 1;
    }
    if(player.score > 80 && boss[1].lived == false)
    {
        boss[1].alive = true;
        *num_boss = 2;
    }
    int k;
    int j;
    for(j=0; j < *num_boss; j++)
    {
        if(boss[j].lives <= 0)
        {
            boss[j].alive = false;
            boss[j].lived = true;
            boss[j].x = WIDTH/2;
            boss[j].y = 400;
            boss[j].real_y = boss[j].y;
            boss[j].speed = 0.1;
            boss[j].size_boss = 0;
            boss[j].velx = 2;
            boss[j].vely = 15;
            boss[j].boundx = 0;
            boss[j].boundy = 0;
            boss[j].real_size_boss = 100;
            boss[j].lives = 20;
            *text_boss = 0;
        }
        if(boss[j].alive)
        {
            for(k=0; k < *num_enemyred; k++)
                enemyred[k].alive = false;
            for(k=0; k < *num_enemyblue; k++)
                enemyblue[k].alive = false;

            if(boss[j].size_boss<boss[j].real_size_boss)
                boss[j].size_boss+=boss[j].speed;

            if(boss[j].y < boss[j].real_y)
                boss[j].y += boss[j].vely;
            if(boss[j].y > boss[j].real_y)
                boss[j].y -= boss[j].vely;

            /*if(boss[j].x < boss[j].real_x)
                boss[j].x += boss[j].velx;
            if(boss[j].x > boss[j].real_x)
                boss[j].x -= boss[j].velx;*/

            if(boss[j].x < player.x)
                boss[j].x+=boss[j].velx;
            if(boss[j].x > player.x)
                boss[j].x-=boss[j].velx;
        }
    }
}

//funcao para colisao de player com boss
void PlayerColisionBoss(struct Player &player, struct Boss boss[], int *num_boss)
{
    int j;
    for(j=0; j < *num_boss; j++)
    {
        boss[j].boundx = boss[j].size_boss;
        boss[j].boundy = boss[j].size_boss;
        if(boss[j].alive && player.alive)
        {
            if((boss[j].x + boss[j].size_boss) > player.x &&
                    (boss[j].x - boss[j].size_boss) < player.x + player.boundx &&
                    (boss[j].y + boss[j].size_boss) > player.y - player.boundy &&
                    (boss[j].y - boss[j].size_boss) < player.y)
            {
                player.lives--;
                boss[j].lives--;
                boss[j].real_y = HEIGHT/2;
                boss[j].x = 150 + (rand() % 900);
                player.jump = true;
                player.score += 1;
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
        boss[j].boundx = boss[j].size_boss;
        boss[j].boundy = boss[j].size_boss;
        if(boss[j].alive)
        {
            if(shootW.live &&
                    shootW.x < (boss[j].x + boss[j].boundx) &&
                    shootW.x > (boss[j].x - boss[j].boundx) &&
                    shootW.y < (boss[j].y + boss[j].boundy) &&
                    shootW.y > (boss[j].y - boss[j].boundy))
            {
                boss[j].lives--;
                boss[j].x = 150 + (rand() % 900);
                player.score += 1;
            }
            if(shootQ.live &&
                    shootQ.x < (boss[j].x + boss[j].boundx) &&
                    shootQ.x > (boss[j].x - boss[j].boundx) &&
                    shootQ.y < (boss[j].y + boss[j].boundy) &&
                    shootQ.y > (boss[j].y - boss[j].boundy))
            {
                boss[j].lives--;
                boss[j].x = 150 + (rand() % 900);
                player.score += 1;
            }
        }
    }
}

//funcao para slow motion
/*void SlowMo(ALLEGRO_TIMER *slowmo, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue)
{
    if(keys[R] == true)
    {
        slowmo = al_create_timer(1.0 / 60);
        al_register_event_source(event_queue, al_get_timer_event_source(slowmo));
        al_start_timer(slowmo);
    }
    else if(keys[R] == false)
    {
        al_destroy_timer(slowmo);
    }
}*/

#endif // FUNCTIONS_H_INCLUDED
