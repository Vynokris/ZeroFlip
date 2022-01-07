LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c "src/main.cpp" -o Build/main.o
	g++ -c "src/Math.cpp" -o Build/Math.o
	g++ -c "src/card.cpp" -o Build/card.o
	g++ -c "src/game_board.cpp" -o Build/game_board.o
	g++ -c "src/indicator.cpp" -o Build/indicator.o
	g++ -c "src/ui.cpp" -o Build/ui.o
	g++ -o zero_flip Build/main.o Build/Math.o Build/card.o Build/game_board.o Build/indicator.o Build/ui.o $(LIBS)

clean:
	rm -rf Build/*
	rm -f zero_flip