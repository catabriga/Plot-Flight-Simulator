all:

	gcc -o plot *.c -lm -lGL -lGLU -lglut -lX11 -Wall

clean:
	rm plot
	rm *.o

install:
	cp plot /usr/games

uninstall:
	rm /usr/games/plot
