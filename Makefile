# Builds Assignment 2

all:
	make labyrinth
	make vase	

labyrinth:
	g++ labyrinth.cpp -pthread -o labyrinth

vase:
	g++ vase.cpp -pthread -o vase	

clean:
	rm labyrinth vase
