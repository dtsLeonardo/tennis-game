ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: multitenis.exe 

multitenis.exe: multitenis.o
	gcc -o multitenis.exe multitenis.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

multitenis.o: multitenis.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c multitenis.c	
	
clean:
	del multitenis.o
	del multitenis.exe

