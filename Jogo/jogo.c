#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"
#include "ler.h"
#include "escrever.h"

/**
@file jogo.c
Ficheiro principal
*/

/**
brief Macro para o tamanho máximo para o estado e etc
*/
#define MAX_BUFFER		10240
/**
brief Macro para o tamanho do tabuleiro
*/
#define TAM				10
/**
brief Macro para a escala para a impressao dos objetos
*/
#define ESCALA			40

/**
\brief Função que testa se a posição é valida
@param x Coordenada x
@param y Coordenada y
@returns 1 se a posição for válida, 0 caso contrário
*/
int posicao_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < TAM && y < TAM);
}

/**
\brief Função que testa se duas posições são iguais
@param p Posição a comparar com as coordenadas
@param x Coordenada x
@param y Coordenada y
@returns 1 se as posições são iguas, 0 caso contrário
*/
int posicao_igual(POSICAO p, int x, int y) {
    return p.x == x && p.y == y;
}

/**
\brief Função que testa se uma posição tem um item
@param e Estado de onde extraimos os items
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem item, 0 caso contrário
*/
int tem_item (ESTADO e, int x, int y) {
	int i;
	for(i = 0;i < e.num_items;i++)
			if (posicao_igual(e.items[i], x, y))
					return 1;
	return 0;
}

/**
\brief Função que testa se uma posição tem a porta de saída
@param e Estado de onde extraimos a posicao da porta
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem porta, 0 caso contrário
*/
int tem_porta(ESTADO e, int x, int y) {
	return (posicao_igual(e.porta_saida, x, y));
}

/**
\brief Função que testa se uma posição tem o jogador
@param e Estado de onde extraimos a posicao do jogador
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem jogador, 0 caso contrário
*/
int tem_jogador(ESTADO e, int x, int y) {
    return posicao_igual(e.jog, x, y);
}

/**
\brief Função que testa se uma posição tem um inimigo
@param e Estado de onde extraimos os inimigos
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem inimigo, 0 caso contrário
*/
int tem_inimigo(ESTADO e, int x, int y) {
    int i;
    for(i = 0;i < e.num_inimigos;i++)
        if (posicao_igual(e.inimigo[i], x, y))
            return 1;
    return 0;
}

/**
\brief Função que testa se uma posição tem um inimigo que ataca ao longe
@param e Estado de onde extraimos os inimigos que atacam ao longe
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem inimigo que ataca ao longe, 0 caso contrário
*/
int tem_inim_longe (ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_inimigos_longe;i++)
			if (posicao_igual(e.inimigo[i], x, y))
					return 1;
	return 0;
}

/**
\brief Função que testa se uma posição tem um obstaculo
@param e Estado de onde extraimos os obstaculos
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem obstaculos, 0 caso contrário
*/
int tem_obstaculo(ESTADO e, int x, int y) {
    int i;
    for(i = 0;i < e.num_obstaculos;i++)
        if (posicao_igual(e.obstaculo[i], x, y))
            return 1;
    return 0;
}

/**
\brief Função que testa se uma posição tem a espada caída
@param e Estado de onde extraimos a posicao da espada caída
@param x Coordenada x
@param y Coordenada y
@returns 1 se tem a espada caída, 0 caso contrário
*/
int tem_espada(ESTADO e, int x, int y) {
	return posicao_igual(e.espada, x, y);
}

/**
\brief Função que testa se uma posição é uma casa inacessível
@param e Estado de onde extraimos as casas inacessíveis
@param x Coordenada x
@param y Coordenada y
@returns 1 se a casa está ocupada, 0 caso contrário
*/
int posicao_ocupada(ESTADO e, int x, int y) {
    return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_porta(e, x, y) || tem_espada(e, x, y) || tem_item(e, x, y);
}

/**
\brief Função que imprime um quadrado invisivel para servir de link
@param x Coordenada x
@param y Coordenada y
@param e Estado que tornaremos link
*/
void imprime_quadr_link (int x, int y, ESTADO e) {
	char nome[7] = {0};
	strncpy(nome, e.letra, 6);

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s,%d,%d", nome, x, y);

	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

/**
\brief Função que imprime as casas possíveis de movimento para os inimigos
@param e Estado de onde extraimos o que contem cada casa
@param x Coordenada x
@param y Coordenada y
*/
void cores_inimigo (ESTADO e, int x, int y) {
	if (tem_obstaculo(e, x, y) || tem_porta(e, x, y) || tem_inimigo (e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_BW.png");
		else IMAGEM(x, y, ESCALA, "Ground2_BW.png");
	else if (tem_jogador(e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_R.png");
		else IMAGEM(x, y, ESCALA, "Ground2_R.png");
	else
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
		else IMAGEM(x, y, ESCALA, "Ground2_L.png");
}

/**
\brief Função que imprime as casas possíveis de movimento para o jogador
@param e Estado de onde extraimos o que contem cada casa
@param x Coordenada x
@param y Coordenada y
*/
void cores_jogador (ESTADO e, int x, int y) {
	if (tem_inimigo(e, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_R.png");
		else IMAGEM(x, y, ESCALA, "Ground2_R.png");
	else if (posicao_igual(e.porta_saida, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_G.png");
		else IMAGEM(x, y, ESCALA, "Ground2_G.png");
	else if (tem_obstaculo(e, x,y ) || posicao_igual(e.porta_entrada, x, y))
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_BW.png");
		else IMAGEM(x, y, ESCALA, "Ground2_BW.png");
	else
		if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
		else IMAGEM(x, y, ESCALA, "Ground2_L.png");
}

/**
\brief Função que inicia o processo de iluminar as casas possiveis de movimento
@param e Estado de onde extraimos o que contem cada casa
@param x Coordenada x
@param y Coordenada y
@param jog_inimi Int decisor se o objeto é um inimigo(1) ou um jogador(0)
*/
void imprime_casa_iluminada (ESTADO e, int x, int y, int jog_inim) {
	if (!posicao_valida(x,y)) return;

	if (jog_inim) cores_inimigo (e, x, y);
	else cores_jogador(e, x, y);

	}

/**
\brief Função que imprime uma casa no tabuleiro
@param x Coordenada x
@param y Coordenada y
*/
void imprime_casa(int x, int y) {
	int idx = (x + y) % 2;
	if (idx == 0) IMAGEM(x, y, ESCALA, "Ground1_Y.png");
	else IMAGEM(x, y, ESCALA, "Ground2_B.png");
}

/**
\brief Função que introduz um objeto numa casa aleatória
@param e Estado que usamos para ver se podemos colocar o obstaculo em cada casa
@param tipo_item Int decisor se o objeto é um inimigo(0), um obstaculo(1) ou um item(2)
@returns Estado atualizado com o objeto introduzido em campo
*/
ESTADO inicializar_posicao(ESTADO e, int tipo_item) {
    int x, y;
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while(posicao_ocupada(e, x, y));

		if (tipo_item == 0) {
			e.inimigo[(int)e.num_inimigos].x = x;
	    e.inimigo[(int)e.num_inimigos].y = y;
	    e.num_inimigos++;
		}
		else if (tipo_item == 1) {
    	e.obstaculo[(int)e.num_obstaculos].x = x;
    	e.obstaculo[(int)e.num_obstaculos].y = y;
    	e.num_obstaculos++;
		}
		else {
			e.items[(int)e.num_items].x = x;
    	e.items[(int)e.num_items].y = y;
			e.num_items++;
		}

    return e;
	}

/**
\brief Função que inicia o processo de introduzir um certo numero de objetos em casas
@param e Estado que usamos para ver se podemos colocar o obstaculo em cada casa
@param num Numero de objetos a introduzir
@param tipo_item Int decisor se o objeto é um inimigo(0), um obstaculo(1) ou um item(2)
@returs Estado com os objetos introduzidos em campo
*/
ESTADO inicializar_posicoes (ESTADO e, int num, int tipo) {
	int i;
	for(i = 0; i < num; i++)
		e = inicializar_posicao(e, tipo);
	return e;
}

/**
\brief Função que decide que tipos de items são introduzidos e quantos são aleatoriamente
@param e Estado que é modificado
@returs Estado com os tipos de items definidos
*/
ESTADO inicializar_tipo_items (ESTADO e) {
	int i;
	for (i=0; i < (int)e.num_items; i++) {
		int r = random() % 11;
		if (r < 4) e.tipo_item[i] = 0;
		else if (r < 8) e.tipo_item[i] = 1;
		else if (r < 10) e.tipo_item[i] = 2;
		else e.tipo_item[i] = 3;
	}

	return e;
}

/**
\brief Função que cria um novo nivel e inicializa tudo
@returs Estado com os tipos de items definidos
*/
ESTADO inicializar() {
	ESTADO e = {0};
	sprintf(e.letra, "%s", "AAAAAA");
	e.porta_entrada.x = 9;
	e.porta_entrada.y = 9;
	e.porta_saida.x = 0;
	e.porta_saida.y = 0;
	e.jog.x = 9;
	e.jog.y = 9;
	e = inicializar_posicoes(e, 12, 0);
	e = inicializar_posicoes(e, 20, 1);
	e.num_inimigos_longe = 3;
	e.ilumina.x = TAM; e.ilumina.y = TAM;
	e.nivel = 1;
	e.espada.x = TAM; e.espada.y = TAM;
	e.Lava_on = 1;
	e.num_vidas = 3;
	e.num_mana = 3;
	e.num_stamina = 5;
	int num_items = random() % 3 + 1;
	e = inicializar_posicoes(e, num_items, 2);
	e = inicializar_tipo_items(e);
	return e;
}

/**
\brief Função que trata da recolha de um scroll e seus benificios
@param e Estado a ser modificado com a recolha
@param x Coordenada x do jogador (futura) e do scroll
@param y Coordenada y do jogador (futura) e do scroll
*/
void recolhe_scroll (ESTADO * e, int x, int y) {
	int tipo = random() % 10;

	if (tipo < 5) (*e).PU_Scroll = 1;
	else if (tipo < 8) (*e).PU_Scroll = 2;
	else (*e).PU_Scroll = 5;
}

/**
\brief Função que trata da recolha de um item e seus benificios
@param e Estado a ser modificado com a recolha
@param x Coordenada x do jogador (futura) e do item
@param y Coordenada y do jogador (futura) e do item
*/
void recolhe_item(ESTADO *e, int x, int y) {
	int i;
	for (i=0; e->items[i].x != x || e->items[i].y != y; i++);

	if (e->tipo_item[i] == 0 && e->num_vidas < 5) e->num_vidas++;
	else if (e->tipo_item[i] == 1 && e->num_stamina < 5) e->num_stamina++;
	else if (e->tipo_item[i] == 2 && e->num_mana < 5 ) e->num_mana++;
	else if (e->tipo_item[i] == 3) recolhe_scroll(e, x, y);

	int j = i;
	for (; i < e->num_items; i++) {e->items[i].x = e->items[i+1].x; e->items[i].y = e->items[i+1].y;}
	for (i = j; i < e->num_items; i++) e->tipo_item[i] = e->tipo_item[i+1];

	e->num_items--;

	e->score++;
}

/**
\brief Função que trata de eliminar um inimigo da face do tabuleiro
@param e Estado a ser modificado com a matança do inimigo
@param x Coordenada x do inimigo a ser aniquilado
@param y Coordenada y do inimigo a ser aniquilado
*/
void apaga_inimigo (ESTADO *e, int x, int y) {
	int i;
	for (i=0; i < (int)e->num_inimigos; i++)
		if (e->inimigo[i].x == x && e->inimigo[i].y == y) {
				if (tem_inim_longe(*e, x, y)) {e->num_inimigos_longe--; e->score++;}
				for (; i < (int)e->num_inimigos; i ++) e->inimigo[i] = e->inimigo[i+1];
				e->inimigo[i].x = 0; e->inimigo[i].y = 0;
				e->num_inimigos--;
				e->score++;
		}
}

/**
\brief Função que trata de eliminar os inimigos numa linha reta ao atirar a espada
@param e Estado a ser modificado com a matança dos inimigos
@param coord_fix Coordenada fixa decisora se a matança é vertical ou horizontal
@param d Coordenada que decide a partir de onde começa a matar
*/
void kill_line (ESTADO *e, int coord_fix, int d) {
	int dx = 0, dy = 0, inc;

	if (d > 0) inc = 1;
	else inc = -1;

	if (coord_fix == 0) {e->espada.x = e->jog.x; dy = d; apaga_inimigo(e, e->jog.x, (e->jog.y + dy - inc));}
	else {e->espada.y = e->jog.y; dx = d; apaga_inimigo(e, (e->jog.x + dx - inc), e->jog.y);}

	while (tem_inimigo(*e, (e->jog.x + dx), (e->jog.y + dy))) {
		apaga_inimigo(e, (e->jog.x + dx), (e->jog.y + dy));
		if (coord_fix == 0) {e->espada.y = e->jog.y + dy; dy += inc;}
		else {e->espada.x = e->jog.x + dx; dx += inc;}
	}
}

/**
\brief Função que trata de mudar a posição da espada quando lançada e invocar a matança
@param e Estado a ser modificado com a matança dos inimigos
@param dx Posicao relativa da espada ao jogador horizontalmente
@param dy Posicao relativa da espada ao jogador verticalmente
*/
void kill_streak (ESTADO *e, int dx, int dy) {
	POSICAO n_espada;
	n_espada.x = e->jog.x + dx; n_espada.y = e->jog.y + dy;
	if (!tem_inimigo(*e, (e->jog.x + dx), (e->jog.y + dy))) e->espada = n_espada;
	else {
		if (dx != 0) kill_line (e, 1, dx);
		else kill_line (e, 0, dy);
	}
}

/**
\brief Função que tira o que está na posicao a ser ocupada pelo jogador (by killing or replacing)
@param e Estado a ser modificado com a matança dos inimigos
@param dx Posicao relativa da posicao futura ao jogador horizontalmente
@param dy Posicao relativa da posicao futura ao jogador verticalmente
*/
void kill_replace (ESTADO *e, int dx, int dy) {
	int x = e->jog.x + dx;
	int y = e->jog.y + dy;

	if (e->PU_Sword == 2) {
		e->PU_Sword = 1;
		kill_streak(e, dx, dy);
	}
	else {
		if (tem_inimigo(*e, x, y)) apaga_inimigo(e, x, y);
		else if (tem_espada(*e, x, y)) {e->espada.x = TAM; e->espada.y = TAM; e->PU_Sword=0;}
		else if (tem_item(*e, x, y)) recolhe_item(e, x, y);
		e->jog.x = x; e->jog.y = y;
	}
}

/**
\brief Função que trata de mover o jogador para a posicao nova
@param e Estado a ser modificado com a matança dos inimigos
@param dx Posicao relativa da posicao futura ao jogador horizontalmente
@param dy Posicao relativa da posicao futura ao jogador verticalmente
*/
void move(ESTADO *e, int dx, int dy) {
	kill_replace(e, dx, dy);

	e->fase = 1;
	e->ilumina.x = TAM; e->ilumina.y = TAM;
	e->teleport_on = 0;

	if (e->PU_Shield > 0) e->PU_Shield--;
}

/**
\brief Função que diz se podemos introduzir um link na posicao
@param e Estado de onde retiramos as condicoes de colocação do link
@param x Coordenada x da posicao a por um link
@param y Coordenada y da posicao a por um link
*/
int in_range(ESTADO e, int x, int y) {
	if (e.jog.x == x && e.jog.y == y) return 0;
	else if (e.PU_Sword == 1 && tem_inimigo(e, x, y)) return 0;

	if (e.teleport_on) return 1;
	else if (e.PU_Sword == 2) return ((abs(e.jog.x - x) <= 2 && e.jog.y == y) || (abs(e.jog.y - y) <= 2 && e.jog.x == x));
	else return (abs(e.jog.x - x) <= 1 && abs(e.jog.y - y) <= 1);
}

/**
\brief Função que trata de modificar o estado para que a casa seja iluminada
@param e Estado a modificar para sabermos que as casas são para ser iluminadas
@param x Coordenada x da posicao a iluminar
@param y Coordenada y da posicao a iluminar
*/
void light(ESTADO *e, int x, int y) {
	if (!posicao_valida(e->ilumina.x, e->ilumina.y)){
		e->ilumina.x = x;
		e->ilumina.y = y;
	} else {
		e->ilumina.x = TAM;
		e->ilumina.y = TAM;
	}
	e->fase = 0;
}

/**
\brief Função que trata de imprimir os movimentos possíveis que o jogador pode fazer
@param e Estado que usa para saber os movimentos possiveis
*/
void imprime_movimentos(ESTADO e) {
	int dx, dy;

	if (e.teleport_on) {
		for (dx = 0; dx < TAM; dx++)
			for (dy = 0; dy < TAM; dy++)
				if (dx != 0 || dy != 0) imprime_quadr_link (dx, dy, e);
	}
	else if (e.PU_Sword == 2) {
		for(dx = -2; dx <= 2; dx++) if (dx!=0) imprime_quadr_link((e.jog.x + dx), e.jog.y, e);
		for(dy = -2; dy <= 2; dy++) if (dy!=0) imprime_quadr_link(e.jog.x, (e.jog.y + dy), e);
	}
	else {
		for(dx = -1;dx <= 1;dx++)
			for(dy = -1;dy <= 1;dy++) {
				int x = e.jog.x + dx;
				int y = e.jog.y + dy;
				if(!posicao_valida(x, y)) continue;
				else if(posicao_ocupada(e, x, y)) {
							if (!tem_inimigo(e, x, y) && !tem_espada(e, x, y) && !tem_item(e, x, y) && !(tem_obstaculo(e, x, y) && !e.Lava_on)) continue;
							else if (tem_inimigo(e, x, y) && posicao_valida(e.espada.x, e.espada.y)) continue;
				}
				imprime_quadr_link(x, y, e);
			}
	}
}

/**
\brief Função que trata de imprimir os movimentos possíveis que o jogador pode fazer
@param e Estado que usa para saber a posição do jogador
*/
void imprime_jogador(ESTADO e) {
	if (!posicao_valida(e.jog.x, e.jog.y)) return;
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "Goblin.png");
	imprime_quadr_link(e.jog.x, e.jog.y, e);
	imprime_movimentos(e);
}

/**
\brief Função que trata de imprimir os inimigios
@param e Estado que usa para saber a posição dos inimigos
*/
void imprime_inimigos(ESTADO e) {
	int i;
	for (i=0; i < (int)e.num_inimigos_longe; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Necromancer.png");
		imprime_quadr_link((int)e.inimigo[i].x, (int)e.inimigo[i].y, e);
	}
	for(; i < (int)e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Goon.png");
		imprime_quadr_link((int)e.inimigo[i].x, (int)e.inimigo[i].y, e);
	}
}

/**
\brief Função que trata de imprimir os obstáculos
@param e Estado que usa para saber a posição dos obstáculos
*/
void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if (e.Lava_on) IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "Lava.png");
		else IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "Lava_Obsidian_2.png");

}

/**
\brief Função que trata de criar um novo nível para quando o jogador chega à porta
@param e Estado que altera para um novo nível
*/
void novo_nivel(ESTADO *e) {
		ESTADO novo = inicializar();
		novo.score = e->score + 5;
		novo.nivel = e->nivel + 1;
		novo.num_vidas = e->num_vidas;
		novo.num_stamina = e->num_stamina;
		novo.num_mana = e->num_mana;
		novo.PU_Scroll = e->PU_Scroll;

		int i;
		for (i=0; i<6; i++) novo.letra[i] = e->letra[i];

		*e = novo;
}

/**
\brief Função que trata de imprimir no tabuleiro a porta
@param e Estado de onde retira a posição da porta
*/
void imprime_porta(ESTADO e) {

		IMAGEM(e.porta_entrada.x, e.porta_entrada.y, ESCALA, "Bottom_hole.png");
		IMAGEM(e.porta_saida.x, e.porta_saida.y, ESCALA, "Hole.png");

		if ((e.jog.x - e.porta_saida.x) <= 1 && (e.jog.y - e.porta_saida.y <= 1) && !posicao_valida(e.espada.x, e.espada.y))
			imprime_quadr_link (e.porta_saida.x, e.porta_saida.y, e);
}
/**
\brief Função que testa se uma posição dada é válida e livre no tabuleiro
@param e Estado de onde retira as coordenadas da posição
@param x Coordenada x da posição a ser testada
@param y Coordenada y da posição a ser testada
*/
int valor_casas (ESTADO e, int x, int y) {
	if (posicao_valida(x, y)) {
			if (posicao_ocupada(e, x, y))
				return 0;
	} else if (!posicao_valida(x, y))
				return 0;
	return 1;
}

/**
\brief Função que calcula a distância entre dois pontos
@param x Coordenada x do primeiro ponto
@param y Coordenada y do primeiro ponto
@param ix Coordenada x do segundo ponto
@param iy Coordenada y do segundo ponto
*/
int dist(int x, int y, int ix, int iy) {
	int r = abs(x - ix) + abs(y - iy);
	return r;
}

/**
\brief Função que testa se o jogador está numa posição onde é atingido pelos arqueiros
@param e Estado atual do jogo
@param i Número do inimigo
*/
int range_inimigo_longe(ESTADO e, int i) {
	int dist;
	if (i < e.num_inimigos_longe) {
		if ((abs (e.jog.x - e.inimigo[i].x) == 2 && abs(e.jog.y - e.inimigo[i].y) == 0) && !tem_obstaculo(e, e.jog.x, e.jog.y)) {
			dist = e.jog.x - e.inimigo[i].x;
			if (!posicao_ocupada(e, e.jog.x - dist/2, e.jog.y)) return 1;
		}
		else if ((abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 2) && !tem_obstaculo(e, e.jog.x, e.jog.y)) {
			dist = e.jog.y - e.inimigo[i].y;
			if (!posicao_ocupada(e, e.jog.x, e.jog.y - dist/2)) return 1;
		}
	}
	return 0;
}

/**
\brief Função que testa se o jogador está numa posição onde é atingido pelos inimigos
@param e Estado atual do jogo
@param i Número do inimigo
*/
int range_inimigo (ESTADO e, int i) {
	if (i >= e.num_inimigos_longe)
		if (((abs (e.jog.x - e.inimigo[i].x) == 1 && abs(e.jog.y - e.inimigo[i].y) == 0) || (abs (e.jog.x - e.inimigo[i].x) == 0 && abs(e.jog.y - e.inimigo[i].y) == 1)) && !tem_obstaculo(e, e.jog.x, e.jog.y))
			return 1;
	return 0;
}

/**
\brief Função que avalia qual a melhor posição para o inimigo se deslocar
@param e Estado atual do jogo
@param i Número do inimigo
*/
ESTADO bot (ESTADO e, int i) {
	int dx, dy;
	int tam = dist(e.jog.x, e.jog.y, e.inimigo[i].x, e.inimigo[i].y);
		for(dx = -1; dx <= 1; dx++) {
			for(dy = -1; dy <= 1; dy++)
				if(valor_casas(e, e.inimigo[i].x + dx, e.inimigo[i].y + dy) && dist(e.jog.x, e.jog.y, e.inimigo[i].x + dx, e.inimigo[i].y + dy) < tam) {
					e.inimigo[i].x = e.inimigo[i].x + dx;
					e.inimigo[i].y = e.inimigo[i].y + dy;
					break;
				}
			if (dist(e.jog.x, e.jog.y, e.inimigo[i].x, e.inimigo[i].y) < tam) break;
		}
	return e;
}

/**
\brief Função que avalia qual a melhor posição para o arqueiro se deslocar
@param e Estado atual do jogo
@param i Número do inimigo
*/
ESTADO bot_longe (ESTADO e, int i) {
	int dx, dy;
	dx = 0;
	for(dy = -2; dy <= 2; dy += 4)
		if (dist(e.jog.x, e.jog.y + dy, e.inimigo[i].x, e.inimigo[i].y) == 1 && !posicao_ocupada(e, e.jog.x, e.jog.y + dy) && posicao_valida(e.jog.x, e.jog.y + dy)) {
			e.inimigo[i].y = e.jog.y + dy;
			return e;
	}
	dy = 0;
	for(dx = -2; dx <= 2; dx += 4)
		if (dist(e.jog.x + dx, e.jog.y, e.inimigo[i].x, e.inimigo[i].y) == 1 && !posicao_ocupada(e, e.jog.x + dx, e.jog.y) && posicao_valida(e.jog.x + dx, e.jog.y)) {
			e.inimigo[i].x = e.jog.x + dx;
			return e;
		}
	return e;
}

/**
\brief Função que trata de mover os inimigos
@param e Estado de onde retira as posições dos inimigos entre outras coisas
@returns Estado modificado com os inimigos movidos
*/
ESTADO mover_inimigos (ESTADO e) {
	int i;
	for (i=0; i<(int)e.num_inimigos; i++)
		if ((range_inimigo(e, i) || range_inimigo_longe(e, i)) && e.num_vidas == 1) {
			e.inimigo[i].x = e.jog.x; e.inimigo[i].y = e.jog.y;
			e.jog.x = TAM; e.jog.y = TAM;
			e.num_vidas--;
			e.num_mana = 0;
			e.num_stamina = 0;
			e.game_over = 1;
		}
		else if (range_inimigo(e, i) || range_inimigo_longe(e, i)) {
			e.num_vidas--;
			}
		else if (i < e.num_inimigos_longe && (abs(e.jog.x - e.inimigo[i].x) <= 2 && abs(e.jog.y - e.inimigo[i].y) <= 2))
		 	e = bot_longe(e, i);
		else e = bot(e, i);
	return e;
}

/**
\brief Função que trata de iluminar os inimigos
@param e Estado de onde retira a posição do inimigo
@param x Coordenada x da posição do inimigo
@param y Coordenada y da posição do inimigo
*/
void iluminar_inimigo (ESTADO e, int x, int y) {
	int dx, dy;
	for (dx=-1; dx<= 1; dx++)
		for (dy=-1; dy<= 1; dy++)
			if (dx == 0 || dy == 0) imprime_casa_iluminada(e, x + dx, y + dy, 1);
}

/**
\brief Função que trata de iluminar o jogador
@param e Estado de onde retira a posição do jogador
@param x Coordenada x da posição do jogador
@param y Coordenada y da posição do jogador
*/
void iluminar_jogador (ESTADO e, int x, int y) {
	int dx, dy;

	if (e.PU_Sword == 2) {
		dy = 0;
		for(dx = -2; dx <= 2; dx++) if (dx!=0) imprime_casa_iluminada(e, x + dx, y + dy, 0);
		dx = 0;
		for(dy = -2; dy <= 2; dy++) if (dy!=0) imprime_casa_iluminada(e, x + dx, y + dy, 0);
	}
	else {
		for (dx=-1; dx<= 1; dx++)
			for (dy=-1; dy<= 1; dy++) imprime_casa_iluminada(e, x + dx, y + dy ,0);
	}
}

/**
\brief Função que trata de iluminar os inimigos que atacam ao longe
@param e Estado de onde retira a posição do inimigo
@param x Coordenada x da posição do inimigo
@param y Coordenada y da posição do inimigo
*/
void iluminar_inimigo_longe (ESTADO e, int x, int y) {
	int dx, dy;

	if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_L.png");
	else IMAGEM(x, y, ESCALA, "Ground2_L.png");

	for (dx=-1; dx >= -4; dx--) {
	 		if (tem_inimigo(e, x + dx, y) || tem_obstaculo(e, x + dx, y) || tem_jogador(e, x + dx, y)) {imprime_casa_iluminada(e, x + dx, y, 1); break;}
			else imprime_casa_iluminada(e, x + dx, y, 1);
	}

	for (dx=1; dx <= 4; dx++) {
	 		if (tem_inimigo(e, x + dx, y) || tem_obstaculo(e, x + dx, y) || tem_jogador(e, x + dx, y)) {imprime_casa_iluminada(e, x + dx, y, 1); break;}
			else imprime_casa_iluminada(e, x + dx, y, 1);
	}

	for (dy=-1; dy >= -4; dy--) {
	 		if (tem_inimigo(e, x, y + dy) || tem_obstaculo(e, x, y + dy) || tem_jogador(e, x, y + dy)) {imprime_casa_iluminada(e, x, y + dy, 1); break;}
			else imprime_casa_iluminada(e, x, y + dy, 1);
	}

	for (dy=1; dy <= 4; dy++) {
	 		if (tem_inimigo(e, x, y + dy) || tem_obstaculo(e, x, y + dy) || tem_jogador(e, x, y + dy)) {imprime_casa_iluminada(e, x, y + dy, 1); break;}
			else imprime_casa_iluminada(e, x, y + dy, 1);
	}

}

/**
\brief Função que trata de escolher o que se tem de iluminar
@param e Estado de onde retira a posição a iluminar
*/
void imprime_ilumi (ESTADO e) {
	int x = e.ilumina.x;
	int y = e.ilumina.y;

	if (tem_jogador(e, x, y)) iluminar_jogador (e, x, y);
	else if (tem_inim_longe(e, x, y)) iluminar_inimigo_longe (e, x, y);
	else if (tem_inimigo(e, x, y)) iluminar_inimigo (e, x, y);
}

/**
\brief Função que trata de personificar os efeitos do poder escudo
@param e Estado que altera para causar os efeitos necessários
*/
void shield(ESTADO * e) {
		if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}
		else {e->PU_Shield = 2; e->num_stamina -= 2;}

		if (e->PU_Sword == 2) {e->PU_Sword = 0; e->num_stamina ++;}
}

/**
\brief Função que imprime o escudo no tabuleiro
@param e Estado onde vai buscar se o poder está ativado
*/
void imprime_shield (ESTADO e) {

	if ((e.num_stamina < 1 || (e.num_stamina == 1 && e.PU_Sword !=2)) && e.PU_Shield < 2 && !e.teleport_on) {IMAGEM(0, (TAM+1), ESCALA, "X.png"); return;}
	else if (e.PU_Shield == 2) IMAGEM(0, (TAM+1), ESCALA, "X.png");
	else IMAGEM(0, (TAM + 1), ESCALA, "Shield_2.png");

	imprime_quadr_link(0, (TAM+1), e);
}

/**
\brief Função que trata de personificar os efeitos do poder espada
@param e Estado que altera para causar os efeitos necessários
*/
void sword (ESTADO *e) {
		if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}

		if (e->PU_Sword == 0 && e->espada.x == TAM && e->espada.y == TAM && (e->num_stamina > 0 || (e->num_stamina == 0 && e->PU_Shield == 2)) ) {
			e->PU_Sword = 2;
			e->PU_Shield = 1;
			e->num_stamina --;
		}
		else if (e->PU_Sword == 2) {
			e->PU_Sword = 0;
			e->PU_Shield = 1;
			e->num_stamina ++;
		}
}

/**
\brief Função que imprime a espada no tabuleiro
@param e Estado onde vai buscar a posição da espada e se o poder está ativado
*/
void imprime_sword (ESTADO e) {
	if (e.PU_Sword == 0 && e.espada.x == TAM && e.espada.y == TAM && (e.num_stamina > 0 || (e.num_stamina == 0 && e.PU_Shield == 2)) && !e.teleport_on) IMAGEM(0, TAM, ESCALA, "Mace.png");
	else if (e.espada.x != TAM && e.espada.y != TAM) {IMAGEM (e.espada.x, e.espada.y, ESCALA, "Mace.png"); IMAGEM(0, TAM, ESCALA, "X.png"); return;}
	else if (e.PU_Sword == 2) IMAGEM(0, TAM, ESCALA, "X.png");

	imprime_quadr_link (0, TAM, e);
}

/**
\brief Função que trata de personificar os efeitos do poder teletransporte
@param e Estado que altera para causar os efeitos necessários
*/
void teleport(ESTADO *e) {
	(*e).teleport_on = 1;
}

/**
\brief Função que trata de personificar os efeitos do poder terramoto
@param e Estado que altera para causar os efeitos necessários
@param x Coordenada x do jogador
@param y Coordenada y do jogador
*/
void earthquake(ESTADO *e, int x, int y) {
	int dx, dy;
	for (dx = -2; dx <= 2; dx++)
		for (dy = -2; dy <= 2; dy++) apaga_inimigo(e, x + dx, y+ dy);
}

/**
\brief Função que trata de personificar os efeitos do poder petrificar
@param e Estado que altera para causar os efeitos necessários
*/
void obsidian (ESTADO *e) {
	(*e).Lava_on = 0;
}

/**
\brief Função que trata de decidir que efeitos causar consoante o tipo de scroll
@param e Estado que altera para causar os efeitos necessários
*/
void scroll(ESTADO *e) {
	if (e->PU_Shield == 2) {e->PU_Shield = 0; e->num_stamina += 2;}
	else if (e->PU_Sword == 2) {e->PU_Sword = 0; e->num_stamina++;}

	e->num_mana -=  e->PU_Scroll;

	if (e->PU_Scroll == 1) obsidian(e);
	else if (e->PU_Scroll == 2) earthquake(e, e->jog.x, e->jog.y);
	else teleport(e);

	e->PU_Scroll = 0;

}

/**
\brief Função que imprime no ecrã de jogo a imagem do scroll ou da impossibilidade de usar o mesmo
@param e Estado que usa para saber o que tem de imprimir
*/
void imprime_scroll (ESTADO e) {
	if (e.num_mana >= e.PU_Scroll && e.PU_Scroll != 0) {
		IMAGEM(0, 12, ESCALA, "Scroll.png");
		imprime_quadr_link(0, 12, e);
	}
	else IMAGEM(0, 12, ESCALA, "X.png");
}

/**
\brief Função que imprime no ecrã de jogo a imagem dos corações (vida, stamina e mana)
@param e Estado que usa para saber quantos corações tem de imprimir
*/
void imprime_coracoes (ESTADO e) {
	int i, c;
	for (c=0; c<=2; c++) {
		if (c == 0)
			for (i=1; i<=e.num_vidas; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Red.png");
		else if (c == 1)
			for (i=1; i<=e.num_stamina; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Green.png");
		else
			for (i=1; i<=e.num_mana; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Blue.png");

		for (; i<=5; i++) IMAGEM ((4+i), (TAM + c), ESCALA, "Heart_Blocked.png");
	}
}

/**
\brief Função que imprime no ecrã de jogo as poções que o jogador é capaz de apanhar
@param e Estado que usa para saber onde imprimir as poções
*/
void imprime_items (ESTADO e) {
	int i;
	for (i = 0; i < (int)e.num_items; i++) {
		if (e.tipo_item[i] == 0) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Red.png");
		else if (e.tipo_item[i] == 1) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Green.png");
		else if (e.tipo_item[i] == 2) IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Potion_Blue.png");
		else IMAGEM (e.items[i].x, e.items[i].y, ESCALA, "Scroll.png");
	}
}

/**
\brief Função que imprime no ecrã de jogo o texto que o jogo possui
@param e Estado que usa para saber onde escrever o texto
*/
void imprime_texto(ESTADO e) {
	char nome[7] = {0};
	strncpy(nome, e.letra, 6);
	TEXTO(1, -1, ESCALA, 35, "#583a25", nome);

	char score[MAX_BUFFER];
	sprintf(score, "Sc:%d", e.score);

	char level[MAX_BUFFER];
	sprintf(level, "L:%d", (int)e.nivel);

	TEXTO_END(10, -1, ESCALA, 35, "#583a25", score);
	TEXTO_END(7, -1, ESCALA, 35, "#583a25", level);

	TEXTO(1, 10, ESCALA, 35, "#583a25", "Club");
	TEXTO(1, 11, ESCALA, 35, "#583a25", "Shield");

	if (e.PU_Scroll == 1) TEXTO(1, 12, ESCALA, 35, "#583a25", "Petrify");
	else if (e.PU_Scroll == 2) TEXTO(1, 12, ESCALA, 35, "#583a25", "Quake");
	else if (e.PU_Scroll == 5) TEXTO(1, 12, ESCALA, 35, "#583a25", "Portal");
	else TEXTO(1, 12, ESCALA, 35, "#583a25", "Scroll");

	TEXTO(4, 10, ESCALA, 35, "#583a25", "L:");
	TEXTO(4, 11, ESCALA, 35, "#583a25", "S:");
	TEXTO(4, 12, ESCALA, 35, "#583a25", "M:");
}

/**
\brief Função que imprime a interface de jogo
*/
void imprime_canvas() {
	int x, y;

	for (y = -1; y < 0; y++)
		for (x=0; x < TAM; x++) {
			if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
	}

	for(; y < TAM; y++)
		for(x = 0; x < TAM; x++)
			imprime_casa(x, y);

	for (; y < TAM + 3; y++)
		for (x=0; x < TAM; x++) {
			if (((x+y)%2) == 0) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");
		}
}

/**
\brief Função que imprime o mapa de jogo
@param e Estado contendo o conteudo do jogo
*/
void imprime_mapa(ESTADO e) {
	IMAGEM(0, -1, ESCALA, "Map.png");
	e.PU_Shield = 1;
	imprime_quadr_link(0, -1, e);
}

/**
\brief Função que imprime um botão de interface
@param * link Hiperligação do botão
@param y Coordenada y onde ficará o botão
@param * texto String com o texto do botão
*/
void imprime_botao(char* link, int y, char* texto) {

	ABRIR_LINK(link);

	int c;
	for (c=1; c<9; c++) {
		if ((c+y)%2) IMAGEM(c, y, ESCALA, "Ground1_B.png");
		else IMAGEM(c, y, ESCALA, "Ground2_B.png");
	}

	TEXTO_MID(5, y, ESCALA, 40, "#e4a91f", texto);

	FECHAR_LINK;
}

/**
\brief Função que imprime o link da letra para escolher o nome do save
@param x Coordenada x onde ficará a letra
@param y Coordenada y onde ficará a letra
@param * nome String com o nome completo
*/
void imprime_link_letra(int x, int y, char * nome) {
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?Start_%s", nome);

	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

/**
\brief Função que imprime no ecrã start as letras do nome do save
@param * nome String com o nome completo do save
*/
void imprime_letras(char * nome) {
	int i;

	for (i=0; i<6; i++) {

			char letra[2] = {0};
			letra[0] = nome[i];
			TEXTO((2+i), 7, ESCALA, 40, "#583a25", letra);

			IMAGEM((2+i), 5, ESCALA, "Up2.png");
			IMAGEM((2+i), 6, ESCALA, "Up.png");
			IMAGEM((2+i), 8, ESCALA, "Down.png");
			IMAGEM((2+i), 9, ESCALA, "Down2.png");

			if (nome[i] != 'Z') nome[i]++;
			else nome[i] = 'A';
			imprime_link_letra((2+i), 6, nome);

			nome[i] = letra[0];

			if (nome[i] != 'A') nome[i]--;
			else nome[i] = 'Z';
			imprime_link_letra((2+i), 8, nome);

			nome[i] = letra[0];

			if (nome[i] < 'V') nome[i]+=5;
			else nome[i] = ('A' + (nome[i] - 'V'));
			imprime_link_letra((2+i), 5, nome);

			nome[i] = letra[0];

			if (nome[i] > 'E') nome[i]-=5;
			else nome[i] = ('Z' - ('E' - nome[i]));
			imprime_link_letra((2+i), 9, nome);

			nome[i] = letra[0];
		}
}

/**
\brief Função que, recebendo um texto, muda de linha sempre que aparece ";"
@param * texto String com o texto a ser formatado
*/
void avanca_linha(char * texto) {
	int i, j;
	for (i=0; texto[i]!=0 && texto[i]!=';'; i++);
	if (texto[i] != 0) {
		for(j = 0; texto[i + j + 1]!= 0; j++)
		texto[j] = texto[j+i+1];
		texto[j] = 0;
	}
}

/**
\brief Função que trata de introduzir um novo score nos leaderboards
@param * leader String principal com todos os scores
@param * nome Nome do jogador com o score a introduzir
@param * lvl Nível do jogador com o score a introduzir
@param * scr Score a introduzir
@param i Posição onde introduzir
@param * String modificada com os novos scores
*/
void muda_linha(char * leader, char * nome, char *  lvl, char * scr, int i, char * new_leader) {
	int j, n;
	char antes[MAX_BUFFER], depois[MAX_BUFFER];
	for(j=0; n<i-1; j++) {if (leader[j] == ';') n++; antes[j] = leader[j];}
	antes[j] = 0;
	n++;
	int pos = j;
	for(; n < 5; j++) {if (leader[j] == ';') n++; depois[j-pos] = leader[j];}
	depois[j-pos] = 0;
	sprintf(new_leader, "%s%s,%s,%s;%s",antes, nome, lvl, scr, depois);
}

/**
\brief Função que guarda o resultado obtido por um jogador no leaderboard
@param * nome String com o nome do save em que o jogador jogou
@param nivel Nível em que o jogador ficou
@param ponto Pontuação que o jogador obteve
*/
void guarda_score(char * nome, int nivel, int ponto) {
	char leader[MAX_BUFFER], save_leader[MAX_BUFFER];
	ler(leader, "LEADERS.dat");
	strcpy(save_leader, leader);

	char nomes[5][7] = {{0}}, level[5][4], score[6][6];
	int i;
	int pontos[5];

	for (i=0; i<5; i++) {
		if (sscanf(leader, "%6[^,],%4[^,],%6[^;]", nomes[i], level[i], score[i]) != 3) break;
		sscanf(leader, "%*6[^,],%*4[^,],%d", &pontos[i]);
		avanca_linha(leader);
	}

	strcpy(leader, save_leader);
	//TEXTO_MID(5, 4, ESCALA, 10, "#583a25", leader);

	char lvl[4];
	sprintf(lvl, "%d", nivel);
	char scr[6];
	sprintf(scr, "%d", ponto);

	char new_leader[MAX_BUFFER];
	strcpy(new_leader, leader);

	for(i=0; i<5 && pontos[i] > ponto; i++);
	if (i<5) muda_linha(leader, nome, lvl, scr, (i+1), new_leader);
	//TEXTO_MID(5, 6, ESCALA, 10, "#583a25", new_leader);

	escrever(new_leader, "LEADERS.dat");
}

/**
\brief Função que termina o jogo porque o jogador ficou sem vidas
@param e Estado de jogo onde o mesmo termina
*/
void game_over(ESTADO e) {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	TEXTO(1, 1, ESCALA, 110, "#583a25", "GAME");
	TEXTO_END(9, 3, ESCALA, 110, "#583a25", "OVER");

	char score[MAX_BUFFER];
	sprintf(score, "Score: %d", e.score);

	char level[MAX_BUFFER];
	sprintf(level, "Level: %d", e.nivel);

	TEXTO_MID(5, 5, ESCALA, 60, "#583a25", score);
	TEXTO_MID(5, 7, ESCALA, 60, "#583a25", level);

	imprime_botao("http://localhost/cgi-bin/jogo?", 9, "Main Menu");

	char nome[6];
	strncpy(nome, e.letra, 6);
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", nome);

	guarda_score(nome, e.nivel, e.score);

	imprime_botao(link, 11, "Retry");
}

/**
\brief Função que imprime no ecrã fundo do jogo
@param * texto Botão escolhido pelo utilizador
*/
void imprime_background(char* texto) {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	for (x=1; x<9; x++) {
		if ((x+1)%2) IMAGEM(x, 0, ESCALA, "Ground1_B.png");
		else IMAGEM(x, 0, ESCALA, "Ground2_B.png");
	}

	TEXTO_MID(5, 0, ESCALA, 40, "#e4a91f", texto);
}

/**
\brief Função que, antes de iniciar um jogo, apresenta um menu ao utilizador para colocar o nome no save
@param * nome String com o nome do save
*/
void introduzir_nome(char * nome) {

	imprime_background("New Game");

	TEXTO_MID(5, 2, ESCALA, 40, "#583a25", "Introduza um nome");
	TEXTO_MID(5, 3, ESCALA, 40, "#583a25", "e inicie o jogo:");

	imprime_letras(nome);

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/jogo?%s", nome);

	imprime_botao(link, 11, "Start");
}

/**
\brief Função que recarrega um jogo anteriormente guardado
@param * args Argumentos necessários para reiniciar o jogo
*/
void load(char * args) {
	ESTADO e;

	char estado[MAX_BUFFER];
	ler(estado, "SAVE.dat");
	e = str2estado(estado);

	char nome[6];
	strncpy(nome, e.letra, 6);

	char fich[11];
	sprintf(fich, "%s.dat", nome);

	escrever(estado2str(e), fich);
	sprintf(args, "%s,10,10", nome);
}

/**
\brief Função que guarda um jogo num ficheiro
@param *e Estado atual do jogo que será guardado
*/
void save(ESTADO *e) {
	e->fase = 0;
	escrever(estado2str(*e), "SAVE.dat");
}

/**
\brief Função que inicia um jogo
@param e Estado a ser criado
*/
void jogo(ESTADO e) {

	if (e.game_over) {
		game_over(e);

		char nome[6];
		strncpy(nome, e.letra, 6);

		char fich[11];
		sprintf(fich, "%s.dat", nome);
		remove(fich);

		return;
	}

	imprime_canvas();
	imprime_texto(e);

	if (posicao_valida (e.ilumina.x, e.ilumina.y)) imprime_ilumi(e);

	imprime_obstaculos(e);

	imprime_mapa(e);
	imprime_inimigos(e);
	imprime_porta(e);
	imprime_sword(e);
	imprime_items(e);

	imprime_jogador(e);
	imprime_shield(e);
	imprime_scroll(e);
	imprime_coracoes(e);

}

/**
\brief Função que cria a interface inicial com as várias opções de jogo
*/
void interface() {
	int y,x;
	for (y=-1; y<TAM + 3; y++)
		for (x=0; x<TAM; x++)
			if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_LY.png");
			else IMAGEM(x, y, ESCALA, "Ground2_LY.png");

	TEXTO_MID(5, 1, ESCALA, 110, "#583a25", "GOBLIN");
	TEXTO_MID(5, 3, ESCALA, 110, "#583a25", "N'BONES");

	imprime_botao("http://localhost/cgi-bin/jogo?Start_AAAAAA", 5, "New Game");
	imprime_botao("http://localhost/cgi-bin/jogo?Continue", 7, "Continue Game");
	imprime_botao("http://localhost/cgi-bin/jogo?Leaderboards", 9, "Leaderboards");
	imprime_botao("http://localhost/cgi-bin/jogo?Help", 11, "Help");
}

/**
\brief Função que cria um botão para cada item de jogo em "Help". Cada item possui uma breve descrição
*/
void imprime_botao_ajuda(int x, int y) {
	char link[MAX_BUFFER];

	if (y==4) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Goblin");
		else if (x==5) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Goon");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Necromancer");
	}
	else if (y==6) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Hearts");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Potions");
	}
	else if (y==8) {
		if (x==3) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Club");
		else if (x==5) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Shield");
		else if (x==7) sprintf(link, "http://localhost/cgi-bin/jogo?Help_%s", "Scroll");
	}

	ABRIR_LINK(link);
	QUADRADO_LINK(x, y, ESCALA);
	FECHAR_LINK;
}

/**
\brief Função que gera o menu "Help"
*/
void help() {

	imprime_background("Help");

	int x,y;
	for (y = 4; y <= 8; y+= 2)
		for (x = 3; x <= 7; x += 2)
			if (x != 5 || y != 6) {
				if ((x+y)%2) IMAGEM(x, y, ESCALA, "Ground1_B.png");
				else IMAGEM(x, y, ESCALA, "Ground2_B.png");
			}

	TEXTO(1, 2, ESCALA, 35, "#583a25", "Click the items below:");
	TEXTO(1, 4, ESCALA, 35, "#583a25", "Ch:");
	TEXTO(1, 6, ESCALA, 35, "#583a25", "H:");
	TEXTO(5, 6, ESCALA, 35, "#583a25", "P:");
	TEXTO(1, 8, ESCALA, 35, "#583a25", "A:");

	IMAGEM(3, 4, ESCALA, "Goblin.png");
	IMAGEM(5, 4, ESCALA, "Goon.png");
	IMAGEM(7, 4, ESCALA, "Necromancer.png");
	IMAGEM(3, 6, ESCALA, "Heart_Red.png");
	IMAGEM(7, 6, ESCALA, "Potion_Red.png");
	IMAGEM(3, 8, ESCALA, "Mace.png");
	IMAGEM(5, 8, ESCALA, "Shield_2.png");
	IMAGEM(7, 8, ESCALA, "Scroll.png");

	for (y = 4; y <= 8; y+= 2)
		for (x = 3; x <= 7; x += 2)
			if (x != 5 || y != 6) imprime_botao_ajuda(x, y);

	imprime_botao("http://localhost/cgi-bin/jogo?", 11, "Main Menu");
}

/**
\brief Função que gera o menu "Leaderboards"
*/
void leaderboards() {
	imprime_background("Leaderboards");

	TEXTO(1, 2, ESCALA, 35, "#583a25", "Pl:");
	TEXTO_MID(6, 2, ESCALA, 35, "#583a25", "L:");
	TEXTO_END(9, 2, ESCALA, 35, "#583a25", "Sc:");

	char leader[MAX_BUFFER];
	ler(leader, "LEADERS.dat");

	char nomes[5][7] = {{0}}, level[5][4], score[6][6];
	int i, num_scans = 0;

	for (i=0; i<5; i++) {
		sscanf(leader, "%6[^,],%4[^,],%6[^;]", nomes[i], level[i], score[i]);
		num_scans++;
		avanca_linha(leader);
	}

	for (i = 4; i < 4 + num_scans; i++) {
		TEXTO(1, i, ESCALA, 35, "#583a25", nomes[i-4]);
		TEXTO_MID(6, i, ESCALA, 35, "#583a25", level[i-4]);
		TEXTO_END(9, i, ESCALA, 35, "#583a25", score[i-4]);
	}

	imprime_botao("http://localhost/cgi-bin/jogo?", 11, "Main Menu");
}

/**
\brief Função que escreve uma breve descrição em cada item de jogo
@param *tipo Tipo de item
*/
void imprime_texto_ajuda(char *tipo) {
	if (strcmp(tipo, "Goblin") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Main character");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "- Kills skeletons");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "- Attacks and moves");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "in all directions");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Has 3 powers: ");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "Club, Shield & Scroll");
	}
	else if (strcmp(tipo, "Goon") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Easy enemy");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "- Attacks vertically");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "& horizontally");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "- Has range of one");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Moves in all");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "directions");
	}
	else if (strcmp(tipo, "Necromancer") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Hard enemy");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "- Attacks as a Goon");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "but only if you're");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "at a range of 2");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Moves in all");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "directions");
	}
	else if (strcmp(tipo, "Hearts") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Red hearts mean");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "life poins left ");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "- Green hearts mean");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "stamina points left");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Blue hearts mean");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "mana points left");
	}
	else if (strcmp(tipo, "Potions") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Red potions give");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "1 life point");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "- Green potions give");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "1 stamina point");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Blue potions give");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "1 mana point");
	}
	else if (strcmp(tipo, "Club") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Hits horizontally");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "and vertically");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "- Has range of 1 or 2");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "and kill anything in");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "a row ");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "- Uses 1 stamina point");
	}
	else if (strcmp(tipo, "Shield") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Activates a ");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "protection that makes");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "you invincible for");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "a turn to every");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "hit");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "- Uses 2 stamina points");
	}
	else if (strcmp(tipo, "Scroll") == 0) {
		TEXTO(1, 2, ESCALA, 35, "#583a25", "- Petrify uses 1 mana");
		TEXTO(1, 3, ESCALA, 35, "#583a25", "& makes lava walkable");
		TEXTO(1, 4, ESCALA, 35, "#583a25", "- Quake uses 3 mana");
		TEXTO(1, 5, ESCALA, 35, "#583a25", "& kills anything nearby");
		TEXTO(1, 6, ESCALA, 35, "#583a25", "- Teleport uses 5 mana");
		TEXTO(1, 7, ESCALA, 35, "#583a25", "& enables teleport");
	}
}

/**
\brief Função que imprime as páginas de ajuda
@param *args Tipo de ajuda necessária
*/
void ajudas(char* args) {

	imprime_background(args);

	imprime_texto_ajuda(args);

	imprime_botao("http://localhost/cgi-bin/jogo?Help", 9, "Help");
	imprime_botao("http://localhost/cgi-bin/jogo?", 11, "Main Menu");
}

/**
\brief Função que executa todos os movimentos e seleções feitas no jogo
@param *args Argumentos necessários para o jogo
*/
ESTADO agir (char* args) {
	ESTADO e;
	char nome[6];
	char fich[11];
	int x, y;

	if (sscanf(args, "%6c,%d,%d", nome, &x, &y) == 1) {
		sprintf(fich, "%s.dat", nome);

		e = inicializar();
		int i;
		for (i=0; i<6; i++) e.letra[i] = nome[i];
	}
	else {
		sprintf(fich, "%s.dat", nome);

		char estado[MAX_BUFFER];
		ler(estado, fich);
		e = str2estado(estado);

		int dx = x - e.jog.x;
		int dy = y - e.jog.y;

		if (posicao_igual(e.porta_saida, x, y)) novo_nivel(&e);
		else if (posicao_valida(x, y) && in_range(e, x, y)) move(&e, dx, dy);
		else if (posicao_valida(x, y)) light(&e, x, y);
		else if (x == 0 && y == 10) sword(&e);
		else if (x == 0 && y == 11) shield(&e);
		else if (x == 0 && y == 12)	scroll(&e);
		else save(&e);

	}

	return e;
}

/**
\brief Função que lê o estado de jogo e decide o que fazer de acordo com o que o utilizador selecionar
@param *args Argumentos necessários para o jogo
*/
void ler_estado(char *args) {
	char nome[6];

	if(strlen(args) == 0) interface();
	else if(sscanf(args, "Start_%s", nome) == 1) introduzir_nome(nome);
	else if(strcmp(args, "Leaderboards") == 0) leaderboards();
	else if (strcmp(args, "Help") == 0) help();
	else if (sscanf(args, "Help_%s", nome) == 1) ajudas(nome);
	else {
		if (strcmp(args, "Continue") == 0) load(args);

		ESTADO e = agir(args);

		if (e.fase != 0 && e.PU_Shield < 1) e = mover_inimigos(e);
		jogo(e);

		char nome[6];
		sscanf(args, "%6c", nome);
		char fich[6];
		sprintf(fich, "%s.dat", nome);
		escrever(estado2str(e), fich);
	}
}

/**
\brief Função principal que gera todo o jogo
*/
int main() {
    srandom(time(NULL));

	COMECAR_HTML;

	FONTE;

	ABRIR_SVG(600, 600);

	ler_estado(getenv("QUERY_STRING"));

	FECHAR_SVG;

	return 0;
}
