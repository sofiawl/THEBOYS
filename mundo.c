#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mundo.h"
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

int aleat(int min, int max)
{
	return min + rand() % (max - min + 1);
}

struct mundo_t *mundo_cria()
{
	struct mundo_t *md = malloc(sizeof(struct mundo_t));
	if (!md)
		return (NULL);

	md->nHab = N_HABILIDADES;
	md->tamMundo = N_TAMANHO_MUNDO;
	md->clock = T_INICIO;
	md->tempoFinal = T_FIM_DO_MUNDO;
	md->nHerois = N_HEROIS;
	md->nBases = N_BASES;
	md->nMissoes = N_MISSOES;
	md->nMissoesCumpridas = 0;
	md->nEventos = 0;

	return md;
}

void mundo_destroi(struct mundo_t *md)
{
	if (!md)
		return;

	for (int j = 0; j < N_HEROIS; j++) {
		heroi_destroi(md->herois[j]);
		//md->herois[j] = NULL;
	}

	for (int j = 0; j < N_BASES; j++) {
		base_destroi(md->bases[j]);
		//md->bases[j] = NULL;
	}

	for (int j = 0; j < N_MISSOES; j++) {
		missao_destroi(md->missoes[j]);
		//md->missoes[j] = NULL;        
	}

	free(md);
	return;
}

struct heroi_t *heroi_cria(int i)
{
	struct heroi_t *hr = malloc(sizeof(struct heroi_t));
	if (!hr)
		return (NULL);

	hr->id = i;
	hr->vivo = 1;
	hr->hab = cjto_aleat(aleat(1, 3), N_HABILIDADES);
	hr->pac = aleat(0, 100);
	hr->velo = aleat(50, 5000);
	hr->exp = 0;
	hr->base = 0;

	return hr;
}

void heroi_destroi(struct heroi_t *hr)
{
	if (!hr || !hr->hab)
		return;

	cjto_destroi(hr->hab);
	hr->hab = NULL;

	free(hr);
	return;
}

int heroi_insere(struct heroi_t *hr, struct mundo_t *md)
{
	if (!hr || !md)
		return (-1);

	if (hr->id == md->nHerois)
		return (-1);

	md->herois[hr->id] = hr;

	return hr->id;
}

int heroi_retira(int index, struct heroi_t *hr, struct mundo_t *md)
{
	if (!hr || !md)
		return (-1);

	if (md->nHerois == 0)
		return (-1);

	for (int i = index; i < md->nHerois; i++)
		md->herois[i] = md->herois[i + 1];

	heroi_destroi(hr);

	return hr->id;
}

struct base_t *base_cria(int i)
{
	struct base_t *bs = malloc(sizeof(struct base_t));
	if (!bs)
		return (NULL);

	bs->id = i;
	bs->missoes = 0;
	bs->filaMax = 0;
	bs->lota = aleat(3, 10);
	bs->herois = cjto_cria(N_HEROIS);
	bs->espera = lista_cria();
	bs->local = aleat(0, N_TAMANHO_MUNDO - 1);

	return bs;
}

void base_destroi(struct base_t *bs)
{
	if (!bs || !bs->espera || !bs->herois)
		return;

	lista_destroi(bs->espera);
	bs->espera = NULL;
	cjto_destroi(bs->herois);
	bs->herois = NULL;

	free(bs);
	return;
}

int base_insere(struct base_t *bs, struct mundo_t *md)
{
	if (!bs || !md)
		return (-1);

	if (bs->id == md->nBases)
		return (-1);

	md->bases[bs->id] = bs;

	return bs->id;
}

int base_retira(int index, struct base_t *bs, struct mundo_t *md)
{
	if (!bs || !md)
		return (-1);

	if (md->nBases == 0)
		return (-1);

	for (int i = index; i < md->nHerois; i++)
		md->herois[i] = md->herois[i + 1];

	base_destroi(bs);

	return bs->id;
}

struct missao_t *missao_cria(int i)
{
	struct missao_t *mis = malloc(sizeof(struct missao_t));
	if (!mis)
		return (NULL);

	mis->id = i;
	mis->cumprida = 0;
	mis->tenta = 0;
	mis->hab = cjto_aleat(aleat(6, 10), N_HABILIDADES);
	mis->perigo = aleat(0, 100);
	mis->local = aleat(0, N_TAMANHO_MUNDO - 1);

	return mis;
}

void missao_destroi(struct missao_t *mis)
{
	if (!mis || !mis->hab)
		return;

	cjto_destroi(mis->hab);
	mis->hab = NULL;

	free(mis);
	return;
}

int missao_insere(struct missao_t *mis, struct mundo_t *md)
{
	if (!mis || !md)
		return (-1);

	if (mis->id == md->nMissoes)
		return (-1);

	md->missoes[mis->id] = mis;

	return mis->id;
}

int missao_retira(int index, struct missao_t *mis, struct mundo_t *md)
{
	if (!mis || !md)
		return (-1);

	if (md->nMissoes == 0)
		return (-1);

	for (int i = index; i < md->nMissoes; i++)
		md->missoes[i] = md->missoes[i + 1];

	missao_destroi(mis);

	return mis->id;
}
