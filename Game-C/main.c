#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

int main()
{
    ALLEGRO_DISPLAY *display;
        if(!al_init())
            return -1; //erro caso inicializar

        display = al_create_display(800,600);

        if(!display)
            return -1;

        al_rest(5.0f);

        al_destroy_display(display);

    return 0;
}
