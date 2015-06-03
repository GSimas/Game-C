//obj IDS (para indentificar o objeto)
enum IDS{PLAYER, ENEMY, BULLET};

//estrutura do player
struct Player {
    int ID;
    int x; //posição x
    int y; //posição y
    int lives;
    int speed;
    int bundx; //(?) descobrir ainda
    int boundy; //(?) descobrir ainda
    int score;
};
