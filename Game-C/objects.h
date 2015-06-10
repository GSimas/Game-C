#ifndef __objects_h
#define __objects_h

//obj IDS (para indentificar o objeto)
enum IDS{PLAYER, ENEMY, BULLET};

//estrutura do player
struct Player {
    char ID;
    int x; //posição x
    int y; //posição y
    int lives;
    int speed;
    int jumpSpeed;
    bool jump;
    bool moving;
    bool colision;
    int velx;
    int vely;
    int boundx; //(?) descobrir ainda
    int boundy; //(?) descobrir ainda
    int score;
};

struct Enemy_red {
    int x;
    int y;
    float speed;
    int velx;
    int vely;
    int boundx;
    int boundy;
    bool moving;
    bool jumping;
    bool alive;
};

struct Enemy_blue {
    int x;
    int y;
    float speed;
    int velx;
    int vely;
    int boundx;
    int boundy;
    bool moving;
    bool jumping;
    bool alive;
};

struct Shoot {
    char ID;
    int x;
    int y;
    int speed;
    bool live;
};

#endif
