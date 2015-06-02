//obj IDS (para indentificar o objeto)
enum IDS{PLAYER, ENEMY, BULLET};

//estrutura do player
struct Player {
    int ID; //identidade
    int x; //posição x
    int y; //posição y
    int lives; //vidas
    int speed; //velocidade
    int bundx; //(?) descobrir ainda
    int boundy; //(?) descobrir ainda
    int score; //pontuação do jogador
}
