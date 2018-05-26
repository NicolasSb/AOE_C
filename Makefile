CC=gcc
CFLAGS=-Wall -Wextra -pedantic-errors `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image -lcmocka -lm -lSDL_ttf
VPATH=model:view:controller:tests/model

OBJS=main.o files.o tile.o map.o buildings.o perso.o game_utils.o iso_utils.o application.o view.o controller_player.o list.o map_chunk.o qtree.o game.o animals.o ressource.o interaction_model.o interaction_view.o interaction_controler.o menu_model.o menu_view.o

TEST_OBJS=tests.o test_animals.o test_buildings.o testFiles.o test_list.o testMap.o testMapChunk.o test_perso.o test_gameUtils.o testQTree.o test_ressource.o testTile.o

all: aoe4

aoe4: $(OBJS) $(TEST_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
	@mkdir ./obj
	@mv *.o ./obj

main.o : view_constants.h application.h view.h map.h controller_player.h game.h tests.h interaction.h menu.h
controller_player.o: view.h game_utils.h interaction.h
tile.o : files.h
map.o : files.h tile.h
buildings.o : files.h
view.o : map.h tile.h view_constants.h game_utils.h
player.o : perso.h buildings.h ressource.h list.h animals.h 
list.o : perso.h buildings.h
perso.o : files.h
iso_utils.o: view_constants.h application.h files.h
interaction_controler.o: interaction.h
interaction_model.o: interaction.h application.h files.h
interaction_view.o: interaction.h

test_animals.o: animals.h tests.h
test_batiments.o: buildings.h files.h tests.h
testFiles.o: files.h tests.h
test_list.o: list.h tests.h perso.h
testMap.o: map.h tests.h
testMapChunk.o: map_chunk.h tests.h
test_perso.o: perso.h tests.h
test_player.o: game_utils.h tests.h
testQTree.o: qtree.h tests.h
test_ressource.o: ressource.h tests.h
testTile.o: tile.h tests.h


%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f aoe4
	@rm -f *.o *.gcno *.gcda *.save
	@rm -r ./obj
	
