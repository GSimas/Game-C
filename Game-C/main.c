/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V1.0
*Titulo: "NÃO DEFINIDO"
*********************************/

//inclusão de bibliotecas
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "objects.h"

//prototypes
void InitPlayer(Player &player);

//main
int main()
{
    //primitive variables
    bool done = false;

    //object variables
    Player player;

    //allegro variables
    ALLEGRO_DISPLAY *display;

    //initializate functions
    if(!al_init())
            return -1; //erro caso inicializar

        display = al_create_display(800,600); //criar display

        if(!display)
            return -1; //se der merda

        al_init_primitives_addon();
        InitPlayer(player); //chamar função que "inicia" player

        while (!done) {
            DrawPlayer(player); //desenha player
            al_flip_display(); //atualiza a tela
            al_clear_to_color(al_map_rgb(0,0,0)); //tela preta
        }

        al_rest(5.0f);

        al_destroy_display(display);

    return 0;
}
