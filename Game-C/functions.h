#ifndef __FUNCTIONS_H_INCLUDED
#define __FUNCTIONS_H_INCLUDED

//Funções Player
void InitPlayer(Player &player, int *text_color);
void DrawPlayer(Player &player);
void PlayerJump(Player &player, bool *UP);
void PlayerRight(Player &player, bool *RIGHT);
void PlayerLeft(Player &player, bool *LEFT);
void ResetPlayer(Player &player, Enemy_red enemyred[], int *num_enemyred, Enemy_blue enemyblue[], int *num_enemyblue, Obstacle &obstacle, Boss boss[], int *num_boss, int *text_color);
void TransportPlayer(Player &player);

//Funções shootQ
void InitShootQ(struct Shoot &shootQ);
void DrawShootQ(struct Shoot &shootQ);
void FireShootQ(struct Shoot &shootQ, struct Player &player);
void UpdateShootQ(struct Shoot &shootQ, struct Player &player);

//Funções shootW
void InitShootW(struct Shoot &shootW);
void DrawShootW(struct Shoot &shootW);
void FireShootW(struct Shoot &shootW, struct Player &player);
void UpdateShootW(struct Shoot &shootW, struct Player &player);

//Funções shootE
void InitShootE(Shoot &shootE);
void DrawShootE(Shoot &shootE, Player &player);
void FireShootE(Shoot &shootE, Player &player);
void UpdateShootE(Shoot &shootE, Player &player);

//Funções enemyred
void InitEnemyRed(struct Enemy_red enemyred[], int *num_enemies);
void DrawEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player);
void UpdateEnemyRed(struct Enemy_red enemyred[], int *num_enemies, struct Player &player);
void ShootQColisionEnemyRed(struct Shoot &shootQ, struct Enemy_red enemyred[], int *num_enemies, struct Player &player);
void PlayerColisionEnemyRed(struct Player &player, struct Enemy_red enemyred[], int *num_enemies);

//Funções enemyblue
void InitEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies);
void DrawEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player);
void UpdateEnemyBlue(struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player);
void ShootWColisionEnemyBlue(struct Shoot &shootW, struct Enemy_blue enemyblue[], int *num_enemies, struct Player &player);
void PlayerColisionEnemyBlue(struct Player &player, struct Enemy_blue enemyblue[], int *num_enemies);

//funções Boss
void InitBoss(struct Boss boss[], int *num_boss);
void DrawBoss(struct Boss boss[], int *num_boss, struct Player &player);
void UpdateBoss(struct Boss boss[], int *num_boss, int *text_boss, struct Player &player, struct Enemy_red enemyred[], int *num_red, struct Enemy_blue enemyblue[], int *num_blue);
void PlayerColisionBoss(struct Player &player, struct Boss boss[], int *num_boss);
void ShootColisionBoss(struct Shoot &shootW, struct Shoot &shootQ, struct Boss boss[], int *num_boss, struct Player &player);

//funções obstaculos
void InitObstacle(Obstacle &obstacle);
void DrawObstacle(Obstacle &obstacle);
void UpdateObstacle(Obstacle &obstacle, ALLEGRO_FONT *medium_font, Player &player);
void PlayerColisionObstacle(Player &player, Obstacle &obstacle);

//funções relacionadas a texto
void ChangeColor(int *text_color, struct Player &player, struct Boss boss[], int *num_boss, int *text_boss);
void DrawText(ALLEGRO_FONT *title_font, ALLEGRO_FONT *medium_font, Player &player, Boss boss[], int *num_boss, int *text_color, int *text_boss);

#endif // FUNCTIONS_H_INCLUDED
