/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V3.7.2
*Titulo: "SHOCK EFFECT"
*********************************/

//inclusao de bibliotecas//////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "objects.h" //header de objetos
#include "functions.h" //header de funcoes

////////////////////////////////////////////////////////////////////////////////
//Variáveis Globais/////////////////////////////////////////////////////////////
int WIDTH; //largura display
int HEIGHT; //altura display
int back_x; //ponto x do fundo do background
int back_y; //ponto y do fundo do background
const int GRAVITY = 1;
const int FPS = 60;
enum KEYS {ENTER, UP, DOWN, LEFT, RIGHT, Q, W, E};
bool keys[8] = {false, false, false, false, false, false, false, false};
const int TELA_INICIO = 0;
const int TELA_INSTRU = 1;
const int TELA_JOGO = 2;
const int TELA_FINAL = 3;
const int TELA_PAUSE = 4;
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//MAIN////////////////////////////////////////////////////////////////////////
int main()
{
    //primitive variables////////////////////////////////////////////////////
    int tela = TELA_INICIO; //definição da tela inicial
    int text_color = 255; //variavel para cor (animacao inicial de jogo - efeito relampago)
    int text_boss = 255; //variavel para cor de texto boss
    bool done = false;
    bool redraw = true;
    int letra; //variavel que grava a escolha do background
    int i; //variavel "curinga"
    //Quantidade máxima de inimigos
    int NUM_ENEMYRED = 10; //quantidade máxima de inimigos vermelhos
    int NUM_ENEMYBLUE = 10; //quantidade máxima de inimigos azuis
    int NUM_BOSS = 5; //quantidade máxima de boss

    //object variables
    struct Player player;
    struct Enemy_red enemyred[NUM_ENEMYRED];
    struct Enemy_blue enemyblue[NUM_ENEMYBLUE];
    struct Boss boss[NUM_BOSS];
    struct Shoot shootQ;
    struct Shoot shootW;
    struct Shoot shootE;
    struct Obstacle obstacle;
    struct SpriteScientist scientist;
    struct Sprite background0;
    struct Sprite background1;
    struct Sprite background2;
    struct Sprite background3;
    struct Sprite background4;
    struct Sprite background5;
    struct Sprite background6;
    struct Sprite enemyred_sprite;

    //allegro variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    ALLEGRO_SAMPLE *musica0 = NULL;
    ALLEGRO_SAMPLE *musica1 = NULL;
    ALLEGRO_SAMPLE *musica2 = NULL;
    ALLEGRO_SAMPLE *musica3 = NULL;
    ALLEGRO_SAMPLE *musica4 = NULL;
    ALLEGRO_SAMPLE *musica5 = NULL;
    ALLEGRO_SAMPLE *musica6 = NULL;
    ALLEGRO_SAMPLE *musica666 = NULL;

    ALLEGRO_SAMPLE_ID musica1id;
    ALLEGRO_SAMPLE_ID musica3id;
    ALLEGRO_SAMPLE_ID musica666id;

    ALLEGRO_FONT *title_font = NULL;
    ALLEGRO_FONT *medium_font = NULL;

    /////////////////////////////////////////////////
    //função para escolha de background/////////////
    OpcaoBackground(letra);
    ////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    //verificacoes de erro e inicializações///////////////////

    //Inicialização do allegro
    if(!al_init())
        return -1;
    //Inicialização display
    display = al_create_display(WIDTH,HEIGHT); //criar display
    if(!display)
        return -1; //se der merda
    //Allegro Module Init
    al_init_primitives_addon();
    //addons relacionados a fonte
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        printf("Falha ao inicializar addon allegro_ttf.\n");
        return -1;
    }
    medium_font = al_load_font("fonts/EHSMB.TTF", WIDTH/20, 0);
    if (!medium_font) {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }
    title_font = al_load_font("fonts/French Electric Techno.ttf", WIDTH/8, 0);
    if (!title_font) {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    //desenhar "carregando na tela"//////////////////////////////////////////////////
    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.1, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.3, ALLEGRO_ALIGN_CENTRE, "LOADING...");
    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.5, ALLEGRO_ALIGN_CENTRE, "99%");
    al_flip_display();

    //inicialização teclado
    al_install_keyboard();
    //inicialização addon de imagem
    if(!al_init_image_addon()) {
        printf("Falha ao inicializar image addon");
        return -1;
    }
    //inicialização audio
    if(!al_install_audio()) {
        printf("Falha ao inicializar audio");
        return -1;
    }
    //inicialização addon acodec (relacionado a audio)
    if(!al_init_acodec_addon()) {
        printf("Falha ao inicializar acodec addon");
        return -1;
    }
    //reserva amostras de som para serem tocadas
    if(!al_reserve_samples(6)) {
        printf("Falha ao reservar samples");
        return -1;
    }

    //ALLEGRO QUEUE (eventos)
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    //////////////////////////////////////////////////////////////////////////////
    //Inicializacao de objetos///////////////////////////////////////////////////

    //Inicializar tiro Q
    InitShootQ(shootQ, letra);
    if(!shootQ.bitmap){
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootQ.\n");
        return -1;
    }
    //inicializar tiro W
    InitShootW(shootW, letra);
    if(!shootW.bitmap) {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootW.\n");
        return -1;
    }
    //inicializar tiro E
    InitShootE(shootE);
    if(!shootE.bitmap) {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shield.\n");
        return -1;
    }
    //Inicializar sprite do jogador
    InitScientist(scientist);
    scientist.bitmap = al_load_bitmap("images/scientist.png");
    if (!scientist.bitmap) {
        al_destroy_display(display);
        printf("Falha ao carregar sprite scientist.\n");
        return -1;
    }
    //outras inicializações
    InitPlayer(player, &text_color); //funcao que "inicia" player
    InitEnemyRed(enemyred, &NUM_ENEMYRED, letra); //funcao que inicia enemyred
    InitEnemyredSprite(enemyred_sprite); // funcao que inicializa sprite de inimigo vermelho
    InitEnemyBlue(enemyblue, &NUM_ENEMYBLUE, letra); //funcao que inicia enemyblue
    InitObstacle(obstacle); //funcao que inicializa obstaculos
    InitBoss(boss, &NUM_BOSS, letra); //funcao que inicializa chefes (bosses)
    switch (letra)
    {
    case 0:
        InitBackground0(background0, musica0, musica666, &musica666id, letra);//funcao que inicializa sprite de background0
        break;
    case 1:
        InitBackground1(background1, musica1, &musica1id); //funcao que inicializa sprite de background1 alternativo
        break;
    case 2:
        InitBackground2(background2, musica2); //funcao que inicializa sprite de background2 alternativo
        break;
    case 3:
        InitBackground3(background3, musica3, &musica3id); //funcao que inicializa sprite de background3 alternativo
        break;
    case 4:
        InitBackground4(background4, musica4); //funcao que inicializa sprite de background4 alternativo
        break;
    case 5:
        InitBackground5(background5, musica5); //funcao que inicializa sprite de background5 alternativo
        break;
    case 6:
        InitBackground6(background6, musica6); //funcao que inicializa sprite de background6 alternativo
        break;
    case 666:
        InitBackground0(background0, musica0, musica666, &musica666id, letra);
    }

    printf("tudo certo");

    //ALLEGRO REGISTER///////////////////////////////////////////////////////
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    /////////////////////////////////////////////////////////////////////////////
    //Escrever na tela DEU//////////////////////////////////////////////////////
    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.9, ALLEGRO_ALIGN_CENTRE, "Deu!");
    al_flip_display();

    /////////////////////////////////////////////////////////////////////////////
    //Loop do jogo///////////////////////////////////////////////////////////////
    while (!done)
    {
        //definir evento do allegro
        ALLEGRO_EVENT ev;
        //switch para opções de tela (inicio, intrução, jogo e final)
        switch(tela)
        {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case TELA_INICIO: //tela inicial com titulo do jogo
            //esperar evento
            al_wait_for_event(event_queue, &ev);
            //se o evento for fechar o display fecha o display
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;
            //se o evento for enter define a variavel tela para próxima tela
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    tela = TELA_INSTRU;
            }
            //desenhar tela de inicio
            switch(letra)
            {
            case 0:
                al_draw_bitmap(background0.image[12], 0, 0, 0);
                break;
            case 1:
                al_draw_bitmap(background1.image[11], 0, 0, 0);
                break;
            case 2:
                al_draw_bitmap(background2.image[12], 0, 0, 0);
                break;
            case 3:
                al_draw_bitmap(background3.image[11], 0, 0, 0);
                break;
            case 4:
                al_draw_bitmap(background4.image[11], 0, 0, 0);
                break;
            case 5:
                al_draw_bitmap(background5.image[20], 0, 0, 0);
                break;
            case 6:
                al_draw_bitmap(background6.image[21], 0, 0, 0);
                break;
            case 666:
                al_draw_bitmap(background0.image[12], 0, 0, 0);
                break;
            }

            al_flip_display();
            break;
        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        case TELA_INSTRU: //tela com instruções sobre como jogar
        //esperar evento
            al_wait_for_event(event_queue, &ev);
            //se o evento for fechar o display fecha o display
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;
            //se o evento for enter define a variavel tela como próxima tela
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    tela = TELA_JOGO;
            }
            //desenha tela de instrução
            if(letra == 0 || letra == 666)
                al_draw_bitmap(background0.image[13], 0, 0, 0);
            else
                al_clear_to_color(al_map_rgb(0, 255, 0));
            al_flip_display();
            break;
        ///////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////
        case TELA_JOGO: //Tela do jogo, inicia o jogo em si
            //esperar evento
            al_wait_for_event(event_queue, &ev);

            //se o evento for fechar o display fecha o display
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

            //evento do timer (vai entrar nesse else if sempre, a nao ser que feche a janela)
            else if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                //define a variavel redraw como true para habilitar o desenho do jogo
                redraw = true;

                //funções de movimentação e variação da musica ou cor do texto
                ChangeColor(&text_color, player, boss, &NUM_BOSS, &text_boss);
                PlayerJump(player, &keys[UP]);
                PlayerRight(player, &keys[RIGHT], scientist);
                PlayerLeft(player, &keys[LEFT]);
                PlayerSample(player, letra, &musica3id, musica3);
                BossSample(boss, &NUM_BOSS, letra, &musica1id, musica1, &musica666id, musica666);

                //updates: atualizar objetos
                UpdateShootQ(shootQ, player);
                UpdateShootW(shootW, player);
                UpdateShootE(shootE, player);
                UpdateEnemyRed(enemyred, &NUM_ENEMYRED, player, shootQ);
                UpdateEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player, shootW);
                UpdateObstacle(obstacle, medium_font, player);
                UpdateBoss(boss, &NUM_BOSS, &text_boss, player, enemyred, &NUM_ENEMYRED, enemyblue,
                           &NUM_ENEMYBLUE, letra);

                //colisoes: testar colisões
                ShootQColisionEnemyRed(shootQ,enemyred, &NUM_ENEMYRED, player);
                ShootWColisionEnemyBlue(shootW, enemyblue, &NUM_ENEMYBLUE, player);
                ShootColisionBoss(shootW, shootQ, boss, &NUM_BOSS, player);
                PlayerColisionEnemyBlue(player, enemyblue, &NUM_ENEMYBLUE);
                PlayerColisionEnemyRed(player, enemyred, &NUM_ENEMYRED);
                PlayerColisionObstacle(player,obstacle);
                PlayerColisionBoss(player, boss, &NUM_BOSS);

                //Reset Player, verifica se o player está morto
                //se estiver morto reseta todos os objetos
                //e define a variavel tela como tela final
                ResetPlayer(&tela, player, enemyred, &NUM_ENEMYRED,
                            enemyblue, &NUM_ENEMYBLUE, obstacle,
                            boss, &NUM_BOSS, &text_color,
                            musica3, &musica3id,
                            musica666, &musica666id, letra);
            }
             ///////////////////////////////////////////////////
            //se o evento for um apertar de teclas/////////////
            //define tecla como abaixada e inicializa função para ela se houver
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
                    FireShootQ(shootQ, player);
                    break;
                case ALLEGRO_KEY_W:
                    keys[W] = true;
                    FireShootW(shootW, player);
                    break;
                case ALLEGRO_KEY_E:
                    keys[E] = true;
                    FireShootE(shootE, player);
                    break;
                case ALLEGRO_KEY_ENTER:
                    tela = TELA_FINAL;
                    break;
                }
            }
            //se o evento for o não apertar de teclas define elas como false
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    keys[UP] = false;
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

            //se não tiver mais eventos para acontecer desenha os objetos no display
            if(redraw && al_is_event_queue_empty(event_queue))
            {
                //defini variavle que habilita o desenho como falsa
                redraw = false;

                ///desenha o background escolhido
                switch (letra)
                {
                case 0:
                    DrawBackground0(background0);
                    break;
                case 1:
                    DrawBackground1(background1);
                    break;
                case 2:
                    DrawBackground2(background2);
                    break;
                case 3:
                    DrawBackground3(background3);
                    break;
                case 4:
                    DrawBackground4(background4);
                    break;
                case 5:
                    DrawBackground5(background5);
                    break;
                case 6:
                    DrawBackground6(background6);
                    break;
                case 666:
                    DrawBackground0(background0);
                    break;
                }
                //desenha os outros objetos
                DrawText(title_font, medium_font, player, boss, &NUM_BOSS, &text_color, &text_boss, obstacle);
                DrawShootQ(shootQ, letra, boss);
                DrawShootW(shootW, letra, boss);
                DrawShootE(shootE, player);
                DrawEnemyRed(enemyred, &NUM_ENEMYRED, player, enemyred_sprite);
                DrawEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player);
                DrawBoss(boss, &NUM_BOSS, player);
                DrawObstacle(obstacle);
                DrawScientist(player, scientist, &keys[LEFT], &keys[RIGHT]);

                al_flip_display();
            }
            break;
        //////////////////////////////////////////////////////////////////////////////
        //se tela for a tela final
        case TELA_FINAL:
            //espera por evento
            al_wait_for_event(event_queue, &ev);
            //se o evento for fechar o display fecha o display
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;
            //se o evento for enter deifine tela como próxima tela
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    tela = TELA_INICIO;
            }
            //desenha a tela final
            switch(letra)
            {

            case 0:
                al_draw_bitmap(background0.image[14], 0, 0, 0);
                break;
            case 1:
                al_draw_bitmap(background1.image[13], 0, 0, 0);
                break;
            case 2:
                al_draw_bitmap(background2.image[14], 0, 0, 0);
                break;
            case 3:
                al_draw_bitmap(background3.image[13], 0, 0, 0);
                break;
            case 4:
                al_draw_bitmap(background4.image[13], 0, 0, 0);
                break;
            case 5:
                al_draw_bitmap(background5.image[22], 0, 0, 0);
                break;
            case 6:
                al_draw_bitmap(background6.image[23], 0, 0, 0);
                break;
            case 666:
                al_draw_bitmap(background0.image[15], 0, 0, 0);
                break;
            }

            al_flip_display();

            break;
        } //final do switch
    } //final do while/////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    //destroi objetos////////////////////////////////////////////////
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(title_font);
    al_destroy_font(medium_font);
    al_destroy_display(display);
    al_destroy_sample(shootQ.sample);
    al_destroy_sample(shootW.sample);
    for(i=0; i<5; i++)
        al_destroy_sample(player.sample[i]);
    al_destroy_bitmap(scientist.bitmap);
    al_destroy_bitmap(shootE.bitmap[0]);
    for(i=0; i<3; i++)
        al_destroy_bitmap(shootQ.bitmap[i]);
    al_destroy_bitmap(shootW.bitmap[0]);
    for(i=0; i<NUM_ENEMYRED; i++)
        al_destroy_bitmap(enemyred[i].image);
    for(i=0; i<NUM_ENEMYBLUE; i++)
        al_destroy_bitmap(enemyblue[i].image);

    switch(letra)
    {
    case 0:
        al_destroy_sample(musica0);
        for(i=0; i<background0.frame_max + 3; i++)
            al_destroy_bitmap(background0.image[i]);
        break;
    case 1:
        al_destroy_sample(musica1);
        for(i=0; i<background1.frame_max + 2; i++)
            al_destroy_bitmap(background1.image[i]);
        break;
    case 2:
        al_destroy_sample(musica2);
        for(i=0; i<background2.frame_max + 2; i++)
            al_destroy_bitmap(background2.image[i]);
        break;
    case 3:
        al_destroy_sample(musica3);
        for(i=0; i<background3.frame_max + 2; i++)
            al_destroy_bitmap(background3.image[i]);
        break;
    case 4:
        al_destroy_sample(musica4);
        for(i=0; i<background4.frame_max + 2; i++)
            al_destroy_bitmap(background4.image[i]);
        break;
    case 5:
        al_destroy_sample(musica5);
        for(i=0; i<background5.frame_max + 2; i++)
            al_destroy_bitmap(background5.image[i]);
        break;
    case 6:
        al_destroy_sample(musica6);
        for(i=0; i<background6.frame_max + 2; i++)
            al_destroy_bitmap(background6.image[i]);
        break;
    case 666:
        al_destroy_sample(musica666);
        for(i=0; i<background0.frame_max + 2; i++)
            al_destroy_bitmap(background0.image[i]);
    }

    //retorna 0
    return 0;

}//final da MAIN!!
