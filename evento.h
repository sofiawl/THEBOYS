#ifndef EVENTO
#define EVENTO
#include "mundo.h"
#include "fprio.h"

#define EV_CHEGA 1
#define EV_ESPERA 2
#define EV_DESISTE 3
#define EV_AVISA 4
#define EV_ENTRA 5
#define EV_SAI 6
#define EV_VIAJA 7
#define EV_MORRE 8
#define EV_MISSAO 9
#define	EV_FIM 10

struct evento_t {
	int tipo;
	int tempo;
	int dado1;
	int dado2;
};

// Calcula a distância entre dois pontos cartesianos
float calcula_distancia(int x1, int x2, int y1, int y2);

// Inicializa e aloca memória do evento 
struct evento_t *cria_evento(int tipo, int tempo, int dado1, int dado2);

// Libera memória alocada pelo eventp
int destroi_evento(struct evento_t *ev);

// Trata evento chega 
// Retorna tempo do evento ou -1 para erro
int trata_chega(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_espera(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_desiste(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_avisa(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_entra(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_sai(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_viaja(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_morre(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_missao(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);

// Trata evento espera
// Retorna tempo do evento ou -1 para erro
int trata_fim(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef);
#endif
