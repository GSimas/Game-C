#ifndef __objects_h
#define __objects_h

//obj IDS (para indentificar o objeto)
enum IDS {PLAYER, ENEMY, SHOOT};

//estrutura do player
struct Player
{
    char ID;
    int x; //posição x
    int y; //posição y
    int lives;
    int speed;
    int jumpSpeed;
    bool jump;
    bool moving;
    bool colision;
    bool alive;
    bool inverted;
    bool shield;
    int velx;
    int vely;
    int boundx; //(?) descobrir ainda
    int boundy; //(?) descobrir ainda
    int score;
    int energy;
    int death_counter;

    ALLEGRO_SAMPLE *sample[7];
    ALLEGRO_SAMPLE_INSTANCE *instance[2];
};

struct Enemy_red
{
    char ID;
    float x;
    float y;
    float speed;
    float size_enemy;
    float velx;
    float speedx;
    float vely;
    float width;
    float height;
    int boundx;
    int boundy;
    int real_size_enemy;
    bool moving;
    bool alive;

    ALLEGRO_BITMAP *image;
};

struct Enemy_blue
{
    char ID;
    float x;
    float y;
    float speed;
    float speedx;
    float size_enemy;
    float velx;
    float vely;
    float width;
    float height;
    int boundx;
    int boundy;
    float real_size_enemy;
    bool moving;
    bool alive;

    ALLEGRO_BITMAP *image;
};

struct Boss
{
    char ID;
    float x;
    float real_x;
    float y;
    float real_y;
    float speed;
    float size_boss;
    int velx;
    int vely;
    int boundx;
    int boundy;
    int real_size_boss;
    int lives;
    bool alive;
    bool lived;
    bool instance_played;

    ALLEGRO_SAMPLE *sample[2];
    ALLEGRO_SAMPLE_INSTANCE *instance[2];
};

struct Shoot
{
    char ID;
    float x;
    float y;
    int speed;
    int temp;
    int s;
    float width;
    float height;
    float boundx;
    float boundy;
    bool live;

    ALLEGRO_BITMAP *bitmap;
    struct ALLEGRO_SAMPLE *sample;
};

struct Obstacle
{
    char ID;
    float x;
    float y;
    float speed;
    float velx;
    float vely;
    int size_obst;
    int real_size_obst;
    int score;
    bool alive;
};

struct SpriteScientist
{
    ALLEGRO_BITMAP *bitmap;
    int frameDelay;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int maxFrame;
    int curFrameA;
    int curFrameB;
    int curFrameC;
};

struct Sprite
{
    int x;
    int y;
    int frame_max;
    int frame_atual;
    int frame_count;
    int frame_delay;

    ALLEGRO_BITMAP *image[40];
};

#endif
