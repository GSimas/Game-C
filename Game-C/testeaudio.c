//gcc -Wall testeaudio.c -o testeaudio -lallegro -lallegro_audio -lallegro_acodec
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  if (argc!=2) {
   printf("Uso: testeaudio nomearquivo\n");
   exit(1);
  }
  al_init();
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(1); // to create the default mixer/voice

  ALLEGRO_AUDIO_STREAM* stream = al_load_audio_stream(argv[1], 3, 1024);
  if (!stream) {
    printf("Erro ao carregar %s", argv[1]);
    exit(1);
  }
  al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());

  while(al_get_audio_stream_playing(stream))
  {
    al_rest(0.1);
  }
  return 0;
}

