#Estrutura geral do código

##main.c
No arquivo main.c são encontrados:
- As inicializações de objetos, e eventos de ALLEGRO (como o timer, queue, entre outros)
- O loop principal do jogo (while(!done))
- A execução de funções
  - Quanto a determinadas condições de teclado
  - Relacionadas a atualização de objetos (UpdateEnemyred, UpdateBoss, etc.)
  - Funções de desenhos (DrawText, DrawBoss e outros...)

##objects.h
No arquivo header objects.h são encontrados:
- Estruturas de objetos e suas instâncias:
  - Posição x e y
  - Dimensões (width, height, size)
  - Elementos específicos (velocidade geral - speed, velocidade de movimentação no eixo x e no eixo y - velx e vely, etc...)
  - Elementos ALLEGRO (ALLEGRO_BITMAP, ALLEGRO_SAMPLE e ALLEGRO_SAMPLE_INSTANCE)
  - Entre outros elementos individuais.
  
##functions.c
No arquivo C functions.c encontram-se:
- Todas as funções executadas no jogo (criadas por nós desenvolvedores - para funções Allegro consultar biblioteca):
- As funções estão organizadas de acordo com:
  - Elementos do jogo (Player, Enemy, Boss...)
  - Ações (Init, Shoot, Update)
  - Dentre outras funções específicas

##functions.h
O header functions.h contém:
- As chamadas das funções presentes no arquivo functions.c, com seus respectivos membros

##Outros diretórios
Há também outros diretórios integrantes ao repositório do game, tais são:
- images: no qual apresentam-se todas as imagens utilizadas no jogo (backgrounds, sprite sheet e outras figuras)
- sounds: contém todos os sons que são tocados no jogo (dependendo da opção escolhida e das ações realizadas). Há arquivos em formatos suportados: .wav, .flac e .ogg
- fonts: diretório onde há os arquivos .ttf das fontes usadas no jogo

##Bibliotecas adicionas
Observa-se que para identificar mais detalhadamente as funções ALLEGRO executadas no jogo é possível consultar as bibliotecas integrantes.
Também pode-se obter mais informações pela wiki ALLEGRO, em: <https://wiki.allegro.cc/>.

##Observações
Makefile incluído!

Código aberto para modificações!

Softwares utilizados:
- ~~Microsoft Visual Studio Professional 2013~~
- Code::Blocks IDE (programação em Windows)
- Atom (programação em Linux)
- Photoshop (edição de imagens)
- Aseprite (edição de sprites)
- Audacity (edição de áudio)

Jogue à vontade!
