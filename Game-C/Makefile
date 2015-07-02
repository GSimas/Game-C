#Para escrever comentários ##
############################# Makefile ##########################
all: teste
teste: main.o  functions.o game.o
        # O compilador faz a ligação entre os dois objetos
        #         gcc -o teste teste.o main.o
        #         #-----> Distancia com o botão TAB ### e não com espaços
functions.o: functions.c objects.h
	g++ -g -Wall functions.c -c -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec
main.o: main.c objects.h functions.h
	g++ -g -Wall main.c -c -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec
game.o: main.o functions.o
	g++ -g functions.o main.o -o game -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec
clean:
	rm -rf *.o
mrproper: clean
	rm -rf teste
