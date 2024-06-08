#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <time.h>

const float FPS = 60;  
const int SCREEN_W = 500;
const int SCREEN_H = 600;

//Jogador
int DIST_FUNDO = 50;
int ALTURA_JOGADOR = 30;
int LARGURA_JOGADOR = 100;
float VEL_JOGADOR = 3.5;

//Bola
float TAM_MIN = 8;
float TAM_MAX = 12.5;
float VEL_MAX = 3.5;
float VEL_MIN = 2;

typedef struct Jogador {
	float x, y, h, w, vel, reb;
	int dir, esq, cima, baixo;
	ALLEGRO_COLOR cor;
} Jogador;

typedef struct Canhao {
	float x, y;
} Canhao;

typedef struct Bola {
	float x, dx, y, dy, r, vel;
	int dir, esq, cima, baixo;
	ALLEGRO_COLOR cor;
} Bola;

struct tm *data_hora_atual;

//Função para desenhar a quadra
void desenhaQuadra() {
	al_clear_to_color(al_map_rgb(0, 155, 0));
	al_draw_line(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, al_map_rgb(255,255,255), 3);
}

//Função para iniciar, desenhar e atualizar os valores dos jogadores
void initJogador1(Jogador *p1) {
	p1->h = ALTURA_JOGADOR;
	p1->w = LARGURA_JOGADOR;
	p1->x = SCREEN_W/2 - p1->w/2;
	p1->y = DIST_FUNDO/2;
	p1->cor = al_map_rgb(0, 0, 155);
	p1->dir = 0;
	p1->esq = 0;
	p1->cima = 0;
	p1->baixo = 0;
	p1->vel = VEL_JOGADOR;
}
void initJogador2(Jogador *p2) {
	p2->h = ALTURA_JOGADOR;
	p2->w = LARGURA_JOGADOR;
	p2->x = SCREEN_W/2 - p2->w/2;
	p2->y = SCREEN_H - DIST_FUNDO;
	p2->cor = al_map_rgb(155, 0, 0);
	p2->dir = 0;
	p2->esq = 0;
	p2->cima = 0;
	p2->baixo = 0;
	p2->vel = VEL_JOGADOR;
}
void desenhaJogador(Jogador p) {
	al_draw_filled_rectangle(p.x, p.y, p.x + p.w, p.y + p.h, p.cor );
}
void atualizaJogador(Jogador *p1, Jogador *p2) {
	
	//Limita as laterais dos dois jogadores
	
	if(p1->x + p1->vel <= SCREEN_W - LARGURA_JOGADOR + 4 && p1->x + p1->vel >= 4)
	     p1->x = p1->x + p1->dir*p1->vel - p1->esq*p1->vel;
	 
	if(p1->x + p1->vel > SCREEN_W - LARGURA_JOGADOR + 4) 
	     p1->x = p1->x - p1->esq*p1->vel;
	 
	if(p1->x + p1->vel < 4)
		 p1->x = p1->x + p1->dir*p1->vel;
	 
	if(p2->x + p1->vel <= SCREEN_W - LARGURA_JOGADOR + 4 && p2->x + p2->vel >= 4)
	     p2->x = p2->x + p2->dir*p2->vel - p2->esq*p2->vel;
	 
	if(p2->x + p2->vel > SCREEN_W - LARGURA_JOGADOR + 4) 
	     p2->x = p2->x - p2->esq*p2->vel;
	 
	if(p2->x + p2->vel < 4)
		 p2->x = p2->x + p2->dir*p2->vel;
	 
	//Limita a linha superior e inferior dos dois jogadores
	
	if(p1->y + p1->vel <= SCREEN_H/2 - ALTURA_JOGADOR - 1 && p1->y + p1->vel >= 6)
	     p1->y = p1->y + p1->baixo*p1->vel - p1->cima*p1->vel;
	 
	if(p1->y + p1->vel > SCREEN_H/2 - ALTURA_JOGADOR - 1) 
	     p1->y = p1->y - p1->cima*p1->vel;
	 
	if(p1->y + p1->vel < 6)
		 p1->y = p1->y + p1->baixo*p1->vel;
	 
	if(p2->y + p2->vel >= SCREEN_H/2 + 6 && p2->y + p2->vel <= SCREEN_H - ALTURA_JOGADOR + 6)
	     p2->y = p2->y + p2->baixo*p2->vel - p2->cima*p2->vel;
	 
	if(p2->y + p2->vel > SCREEN_H - ALTURA_JOGADOR + 6) 
	     p2->y = p2->y - p2->baixo*p2->vel;
	 
	if(p2->y + p2->vel < SCREEN_H/2 + 6)
		 p2->y = p2->y + p2->baixo*p2->vel;
}

//Função para iniciar o canhão
void initCanhao(Canhao *c) {
     int k;
	 for(k=0; k<4; k++) {	
		//Os valores "13" é pra descontar o raio quando atinge valor máximo de 12.5
		 switch(k) {
		     case 0:
			 c[0].x = SCREEN_W - 13; 
			 c[0].y = 13;
			 break;

			 case 1:			
			 c[1].x = 13;
			 c[1].y = 13;
			 break;

			 case 2:
			 c[2].x = 13;
			 c[2].y = SCREEN_H - 13;
			 break;
			
			 case 3:
			 c[3].x = SCREEN_W - 13;
			 c[3].y = SCREEN_H - 13;
			 break;	
		}			
	}
}	

//Funções para gerar números aleatórios para a bola
float velAleatoria(float n, float m) 
{
   return n + (float)rand()/RAND_MAX * (m-n); 
}

float tamAleatorio(float min, float max) 
{
   return min + (float)rand()/RAND_MAX * (max-min); 
}

//Função para iniciar, desenhar e atualizar os valores das bolas
void initBola(Canhao *c, Bola *b) {
	 
	 int l;	
	 for(l = 0; l<30; l++) {	
		//0   1   2   3  
		//4   5   6   7
		//8   9  10  11  
		//12  13  14  15
		//16  17  18  19
		//20  21  22  23
		//24  25  26  27
		//28  29  
		
		if(l == 0 || l == 4 || l == 8
		|| l == 12 || l == 16 || l == 20
		|| l == 24 || l == 28) {
			
			 b[l].x = c[0].x;
			 b[l].y = c[0].y;
			 b[l].dx = velAleatoria(VEL_MIN, VEL_MAX) * (-1);
	         b[l].dy = velAleatoria(VEL_MIN, VEL_MAX);
			 b[l].r = tamAleatorio(TAM_MIN, TAM_MAX);
			 b[l].cor = al_map_rgb(0, 0, 0);
		}

		else if(l == 1 || l == 5 || l == 9
		|| l == 13 || l == 17 || l == 21
		|| l == 25 || l == 29) {
			
			 b[l].x = c[1].x;
			 b[l].y = c[1].y;
			 b[l].dx = velAleatoria(VEL_MIN, VEL_MAX);
	         b[l].dy = velAleatoria(VEL_MIN, VEL_MAX);
			 b[l].r = tamAleatorio(TAM_MIN, TAM_MAX);
			 b[l].cor = al_map_rgb(0, 0, 0);
		}
		
		else if(l == 2 || l == 6 || l == 10
		|| l == 14 || l == 18 || l == 22
		|| l == 26) {
			
			 b[l].x = c[2].x;
			 b[l].y = c[2].y;
			 b[l].dx = velAleatoria(VEL_MIN, VEL_MAX);
	         b[l].dy = velAleatoria(VEL_MIN, VEL_MAX) * (-1);
			 b[l].r = tamAleatorio(TAM_MIN, TAM_MAX);
			 b[l].cor = al_map_rgb(0, 0, 0);
		}
			
		else if(l == 3 || l == 7 || l == 11
		|| l == 15 || l == 19 || l == 23 
		|| l == 27) {
			
			 b[l].x = c[3].x;
			 b[l].y = c[3].y;
			 b[l].dx = velAleatoria(VEL_MIN, VEL_MAX) * (-1);
	         b[l].dy = velAleatoria(VEL_MIN, VEL_MAX) * (-1);
			 b[l].r = tamAleatorio(TAM_MIN, TAM_MAX);
			 b[l].cor = al_map_rgb(0, 0, 0);
		}			
	}
}
void desenhaBola(Bola *b, int lancar) {
	int l;
	for(l=0; l<lancar; l++)
		al_draw_filled_circle(b[l].x, b[l].y, b[l].r, b[l].cor);
}
void atualizaBola(Bola *b, int lancar, Jogador *p1, Jogador *p2) {
     int l;
		 for (l = 0; l < lancar; l++) {
			
			if(b[l].x + b[l].r > SCREEN_W  || b[l].x - b[l].r < 0) 
				b[l].dx = -b[l].dx;
			
			if(p1->reb == 1)
				if(b[l].x + b[l].r >= p1->x && b[l].x - b[l].r <= p1->x + p1->w && b[l].y + b[l].r >= p1->y && b[l].y - b[l].r <= p1->y + p1->h && b[l].dy<0)
						b[l].dy = -b[l].dy;
			
			if(p2->reb == 1)
				if(b[l].x + b[l].r >= p2->x && b[l].x - b[l].r <= p2->x + p2->w && b[l].y + b[l].r >= p2->y && b[l].y - b[l].r <= p2->y + p2->h && b[l].dy>0)
						b[l].dy = -b[l].dy;
					
			b[l].x = b[l].x + b[l].dx;
			b[l].y = b[l].y + b[l].dy;		 
		 } 
}
	 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
//----------------------- rotinas de inicializacao ---------------------------------------
        
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   
	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//inicia o temporizador
	al_start_timer(timer);

    
	Jogador p1, p2;
	Canhao c[4];
	Bola b[30];    
	int playing = 1, tempo = 0, lancar = 0, i = 0; 
	int pontosP1 = 0, pontosP2 = 0, winner = 0, partida = 0;
	int vezesP1 = 0, vezesP2 = 0;
	char won[50], pontuation[50], nvezes[50], buf[50], placarGeral[20] = "Placar Geral";
	time_t segundos;

	initJogador1(&p1);
	initJogador2(&p2);
	
	initCanhao(c);
	initBola(c, b);	

	time(&segundos);
	data_hora_atual = localtime(&segundos);

	FILE *arq = fopen("npartida.txt", "r");
	while(!feof(arq)) {
		fgets(buf, 50, arq);
		vezesP1 = atoi(strtok(buf, " "));
		vezesP2 = atoi(strtok(NULL, " "));
	}
	fclose(arq);

	while(playing) {
		
		ALLEGRO_EVENT ev;
		
		desenhaQuadra();
		atualizaJogador(&p1, &p2);
		desenhaJogador(p1);
		desenhaJogador(p2);
		
		atualizaBola(b, lancar, &p1, &p2);
		desenhaBola(b, lancar);
		
		for(i=0; i<lancar; i++) {
			if(b[i].y < -50) {
				pontosP2++;
				b[i].x = 200;
				b[i].y = -30;
				b[i].dx = 0;
				b[i].dy = 0;
				if(pontosP2 == 10) {
					winner = 2;
					
					playing = 0;
				} 
			    if(pontosP2 < 10 && tempo > 100) 
					lancar++; 
				
			}
			if(b[i].y > SCREEN_H + 50) {
				b[i].x = 200;
				b[i].y = SCREEN_H + 30;
				b[i].dx = 0;
				b[i].dy = 0;
				pontosP1++;
				if(pontosP1 == 10) {
					winner = 1;
					
					playing = 0;
				}
			    if(pontosP1 < 10 && tempo > 100)   
					lancar++;
			}
		}
		
		al_wait_for_event(event_queue, &ev);
        
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			al_flip_display();
		
			if(al_get_timer_count(timer)%(int)FPS == 0) {
				 tempo = (al_get_timer_count(timer)/FPS);
				 printf("\n%d segundos se passaram.", tempo);
				 if(tempo%5 == 0.0 && lancar < 20) {
					 lancar++;
					 printf(" Bola %d", lancar); 	
				 }
			}
		}
		
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			 
			 switch(ev.keyboard.keycode) {
			 //se a tecla for o W
				 case ALLEGRO_KEY_W:
					 p1.cima = 1;						 
				 break;
			
				 case ALLEGRO_KEY_S:
					 p1.baixo = 1;						 
				 break;
			
				 case ALLEGRO_KEY_A:	 
					 p1.esq = 1;
				 break;
			
				 case ALLEGRO_KEY_D:			 
					 p1.dir = 1;
				 break;
				 
				 case ALLEGRO_KEY_SPACE:			 
					 p1.reb = 1;
			   	 break;
				 
				 case ALLEGRO_KEY_UP:
					 p2.cima = 1;						 
				 break;
			
				 case ALLEGRO_KEY_DOWN:
					 p2.baixo = 1;						 
				 break;
			
				 case ALLEGRO_KEY_LEFT:	 
					 p2.esq = 1;
				 break;
			
				 case ALLEGRO_KEY_RIGHT:			 
					 p2.dir = 1;
				 break;
				 
				 case ALLEGRO_KEY_PAD_ENTER:			 
					 p2.reb = 1;
				 break;
				}
		}
	    
        else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {

				case ALLEGRO_KEY_W:
					p1.cima = 0;
				break;
		
				case ALLEGRO_KEY_S:
					p1.baixo = 0;
				break;
		
				case ALLEGRO_KEY_A:
					p1.esq = 0;
				break;
		
				case ALLEGRO_KEY_D:
					p1.dir = 0;
				break;
				
				case ALLEGRO_KEY_SPACE:			 
					 p1.reb = 0;
				break;
				
				case ALLEGRO_KEY_UP:
					p2.cima = 0;
				break;
		
				case ALLEGRO_KEY_DOWN:
					p2.baixo = 0;
				break;
		
				case ALLEGRO_KEY_LEFT:
					p2.esq = 0;
				break;
		
				case ALLEGRO_KEY_RIGHT:
					p2.dir = 0;
				break;
				
				case ALLEGRO_KEY_PAD_ENTER:			 
					 p2.reb = 0;
				break;

			}	
		}  

	    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		     playing = 0;
	    }
	}
		
	FILE *temp = fopen("historico.txt", "a");
	fprintf(temp, "Data: %d/%d/%d\nTime: %d:%d:%d\nPontos do primeiro jogador: %d\nPontos do segundo jogador: %d\n\n", data_hora_atual->tm_mday,
    data_hora_atual->tm_mon+1, data_hora_atual->tm_year+1900, data_hora_atual->tm_hour, data_hora_atual->tm_min, data_hora_atual->tm_sec, pontosP1, pontosP2);
	fclose(temp);
		
	//inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();
	
	//inicializa o modulo allegro que entende arquivos tff de fontes
    al_init_ttf_addon();
	
	//ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);
	ALLEGRO_FONT *size_24 = al_load_font("arial.ttf", 20, 1);
	
	if(winner == 1 || winner == 2) {
		al_clear_to_color(al_map_rgb(0,0,0));
		sprintf(won, "Jogador %d ganhou!", winner);	
		al_draw_text(size_32, al_map_rgb(20, 200, 30), 110, SCREEN_H/2, 0, won);
		al_flip_display();
		al_rest(3);
	}
	if(winner == 1 || winner == 2) {
	al_clear_to_color(al_map_rgb(0,0,0));
	sprintf(pontuation, "Jogador 1 (%d pontos) e Jogador 2 (%d pontos)", pontosP1, pontosP2);
	al_draw_text(size_24, al_map_rgb(0, 200, 30), 50, SCREEN_H/2, 0, pontuation);
	al_flip_display();
	al_rest(3);
	}
	
	al_clear_to_color(al_map_rgb(0,0,0));
	if(winner == 1) {
		sprintf(nvezes, "Jogador 1 (%d pontos) e Jogador 2 (%d pontos)", vezesP1+1, vezesP2);
		al_draw_text(size_24, al_map_rgb(15, 200, 30), 180, SCREEN_H/2 - 60, 0, placarGeral);
		al_draw_text(size_24, al_map_rgb(15, 200, 30), 50, SCREEN_H/2 + 10, 0, nvezes);
		al_flip_display();
		al_rest(3);
	}
	else if(winner == 2) {
		sprintf(nvezes, "Jogador 1 (%d pontos) e Jogador 2 (%d pontos)", vezesP1, vezesP2+1);
		al_draw_text(size_24, al_map_rgb(15, 200, 30), 180, SCREEN_H/2 - 60, 0, placarGeral);
		al_draw_text(size_24, al_map_rgb(15, 200, 30), 50, SCREEN_H/2 + 10, 0, nvezes);
		al_flip_display();
		al_rest(3);
	}
	
	FILE *arqw = fopen("npartida.txt", "w");
	if(winner == 1)
		fprintf(arqw, "%d %d", vezesP1+1, vezesP2);
	else if(winner == 2)
		fprintf(arqw, "%d %d", vezesP1, vezesP2+1);
	fclose(arqw);
	
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	
	return 0;
}