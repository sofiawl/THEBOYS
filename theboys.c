// programa principal do projeto "The Boys - 2024/2"
// Autor: Sofia Wamser Lima, GRR 20240495

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"
#include "evento.h"

// seus #defines vão aqui

int main()
{
	struct fprio_t *lef;
	struct mundo_t *md;
	struct heroi_t *hr;
	struct base_t *bs;
	struct missao_t *mis;
	struct evento_t *ev;
	int tempo, base, fim;
	int tipo, prio;

	lef = fprio_cria();

	// Estados e eventos iniciais 
	md = mundo_cria();

	// Inicializa bases do mundo
	for (int i = 0; i < N_BASES; i++) {
		bs = base_cria(i);
		base_insere(bs, md);
	}

	// Inicializa heróis do mundo
	for (int i = 0; i < N_HEROIS; i++) {
		hr = heroi_cria(i);
		heroi_insere(hr, md);
		//Simula a chegada dele à uma base
		base = aleat(0, 9);	//N_BASES-1
		tempo = aleat(0, 4320);	// 3 dias
		ev = cria_evento(EV_CHEGA, tempo, hr->id, base);
		fprio_insere(lef, ev, EV_CHEGA, tempo);
	}

	// Inicializa missões do mundo
	for (int i = 0; i < N_MISSOES; i++) {
		mis = missao_cria(i);
		missao_insere(mis, md);
		// Insere missão como evento na LEF
		tempo = aleat(0, md->tempoFinal);
		ev = cria_evento(EV_MISSAO, tempo, mis->id, -1);
		fprio_insere(lef, ev, EV_MISSAO, tempo);
	}

	// Cria evento final
	tempo = md->tempoFinal;
	ev = cria_evento(EV_FIM, tempo, -1, -1);
	fprio_insere(lef, ev, EV_FIM, tempo);


	fim = 1;
	while (fim) {
		ev = fprio_retira(lef, &tipo, &prio);
		if (!ev)
			return (-1);

		(md->nEventos)++;
		md->clock = +ev->tempo;

		switch (ev->tipo) {
		case EV_CHEGA:
			trata_chega(md, ev, lef);
			break;

		case EV_ESPERA:
			trata_espera(md, ev, lef);
			break;

		case EV_DESISTE:
			trata_desiste(md, ev, lef);
			break;

		case EV_AVISA:
			trata_avisa(md, ev, lef);
			break;

		case EV_ENTRA:
			trata_entra(md, ev, lef);
			break;

		case EV_SAI:
			trata_sai(md, ev, lef);
			break;

		case EV_VIAJA:
			trata_viaja(md, ev, lef);
			break;

		case EV_MORRE:
			trata_morre(md, ev, lef);
			break;

		case EV_MISSAO:
			trata_missao(md, ev, lef);
			break;

		case EV_FIM:
			trata_fim(md, ev, lef);
			fim = 0;
			break;
		}

		destroi_evento(ev);
	}

	mundo_destroi(md);
	fprio_destroi(lef);

	return (0);
}
