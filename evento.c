#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "evento.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"

float calcula_distancia(int x1, int x2, int y1, int y2)
{
	float d;

	d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return d;
}

struct evento_t *cria_evento(int tipo, int tempo, int dado1, int dado2)
{
	struct evento_t *ev = malloc(sizeof(struct evento_t));
	if (!ev)
		return (NULL);

	ev->tipo = tipo;
	ev->tempo = tempo;
	ev->dado1 = dado1;
	ev->dado2 = dado2;

	return ev;
}

int destroi_evento(struct evento_t *ev)
{
	if (!ev)
		return (-1);

	free(ev);

	return (0);
}

int trata_chega(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	int espera;
	struct evento_t *novoEv;
	int h = ev->dado1;
	int b = ev->dado2;

	if (h > md->nHerois)
		printf("heroi invalido\n");

	if (b > md->nBases)
		printf("base invalida\n");

	// Imprime mensagens do evento chega
	printf("%6d: CHEGA ", md->clock);
	// ID do heroi
	printf("HEROI %2d ", h);
	// ID da base destino
	printf("BASE %d ", b);
	// Número de heróis presente / lotação da base
	printf("( %2d/ %2d) ", cjto_card(md->bases[b]->herois),
	       md->bases[b]->lota);

	// Se há espaço na base e fila de espera está vazia
	if ((cjto_card(md->bases[b]->herois) < md->bases[b]->lota)
	    && (lista_tamanho(md->bases[b]->espera) == 0))
		espera = 1;
	else			// Calcula se herói vai esperar
		espera =
		    (md->herois[h]->pac) >
		    (10 * lista_tamanho(md->bases[b]->espera));

	// Determina próximo evento com cálculo da espera
	if (espera) {
		// Cria e adiciona ev espera na LEF
		printf("ESPERA\n");
		novoEv = cria_evento(EV_ESPERA, md->clock, h, b);
		fprio_insere(lef, novoEv, EV_ESPERA, md->clock);
	} else {
		// Cria e adiciona ev desiste na LEF
		printf("DESISTE\n");
		novoEv = cria_evento(EV_DESISTE, md->clock, h, b);
		fprio_insere(lef, novoEv, EV_DESISTE, md->clock);
	}

	return ev->tempo;
}

int trata_espera(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *novoEv;
	int h = ev->dado1;
	int b = ev->dado2;
	int tamEspera = lista_tamanho(md->bases[b]->espera);

	// Imprime mensagens do evento espera
	printf("%6d: ESPERA ", md->clock);
	// ID do heroi
	printf("HEROI %2d ", h);
	// ID da base destino
	printf("BASE %d ", b);
	// Número de heróis na fila
	printf("( %2d)\n", lista_tamanho(md->bases[b]->espera));

	// Atualiza número máximo da fila de espera
	if (md->bases[b]->filaMax < tamEspera)
		md->bases[b]->filaMax = tamEspera;

	// Adiciona ao fim da fila de espera o id do herói
	lista_insere(md->bases[b]->espera, h, -1);

	// Cria e adiciona ev avisa na LEF
	novoEv = cria_evento(EV_AVISA, md->clock, h, b);
	fprio_insere(lef, novoEv, EV_AVISA, md->clock);

	return ev->tempo;
}

int trata_desiste(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!ev || !md || !lef)
		return (-1);

	struct evento_t *novoEv;
	int h = ev->dado1;
	int b = aleat(0, N_BASES - 1);

	// Imprime mensagens do evento desiste
	printf("%6d: DESISTE ", md->clock);
	// ID do heroi
	printf("HEROI %2d ", ev->dado1);
	// ID da base destino
	printf("BASE %d\n", ev->dado2);

	// Cria e adiciona ev viaja na LEF
	novoEv = cria_evento(EV_VIAJA, md->clock, h, b);
	fprio_insere(lef, novoEv, EV_VIAJA, md->clock);

	return ev->tempo;
}

int trata_avisa(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *novoEv;
	int h = ev->dado1;
	int b = ev->dado2;

	// Imprime mensagens do evento avisa    
	printf("%6d: AVISA ", md->clock);
	// Base do porteiro que é avisado
	printf("PORTEIRO BASE %d ", b);
	// Número de heróis presente / lotação da base
	printf("( %2d/ %2d) ", cjto_card(md->bases[b]->herois),
	       md->bases[b]->lota);
	// Fila de espera da base
	printf("LISTA ");
	lista_imprime(md->bases[b]->espera);
	printf("\n");

	// Cria eventos de entrada na base dependendo da fila de espera
	// e lotação da base
	while (cjto_card(md->bases[b]->herois) < md->bases[b]->lota
	       && lista_tamanho(md->bases[b]->espera) > 0) {
		lista_retira(md->bases[b]->espera, &h, 0);
		cjto_insere(md->bases[b]->herois, h);
		novoEv = cria_evento(EV_ENTRA, md->clock, h, b);
		fprio_insere(lef, novoEv, EV_ENTRA, md->clock);

		// Imprime mensagem de admissão do herói na base
		printf("%6d: AVISA ", md->clock);
		// Base do porteiro que é avisado
		printf("PORTEIRO BASE %d ", b);
		// Herói que foi admitido dentro da base
		printf("ADMITE %2d\n", h);
	}

	return ev->tempo;
}

int trata_entra(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *novoEv;
	int TPB;
	int h = ev->dado1;
	int b = ev->dado2;

	// Imprime mensagens do evento entra
	printf("%6d: ENTRA ", md->clock);
	// Herói que entra na base
	printf("HEROI %2d ", h);
	// Base que o herói entra
	printf("BASE %d ", b);
	// Número de heróis presente / lotação da base
	printf("( %2d/ %2d) ", cjto_card(md->bases[b]->herois),
	       md->bases[b]->lota);

	// Calcula o tempo de permanência do herói na base 
	// Cria um evento para quando ele sair  
	TPB = 15 + md->herois[h]->pac * aleat(1, 20);
	novoEv = cria_evento(EV_SAI, md->clock + TPB, h, b);
	fprio_insere(lef, novoEv, EV_SAI, md->clock + TPB);

	// Imprime tempo que o herói sairá da base
	printf("SAI %d\n", md->clock + TPB);

	return ev->tempo;
}

int trata_sai(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *evViaja, *evAvisa;
	int d = aleat(0, N_BASES - 1);
	int h = ev->dado1;
	int b = ev->dado2;

	// Imprime mensagens do evento sai
	printf("%6d: SAI ", md->clock);
	// Herói que sai da base
	printf("HEROI %2d ", h);
	// Base que o herói sai
	printf("BASE %d ", b);
	// Número de heróis presente / lotação da base
	printf("( %2d/ %2d)\n", cjto_card(md->bases[b]->herois),
	       md->bases[b]->lota);

	// Retira herói do conjunto de heróis da base
	cjto_retira(md->bases[b]->herois, h);

	// Cria e insere ev viaja na LEF
	evViaja = cria_evento(EV_VIAJA, md->clock, h, d);
	fprio_insere(lef, evViaja, EV_VIAJA, md->clock);

	// Cria e insere ev avisa na LEF
	evAvisa = cria_evento(EV_AVISA, md->clock, h, b);
	fprio_insere(lef, evAvisa, EV_AVISA, md->clock);

	return evViaja->tempo;
}

int trata_viaja(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *novoEv;
	int duracao, distancia;
	int h = ev->dado1;
	int d = ev->dado2;
	int b = md->herois[h]->base;

	// Imprime mensagens do evento viaja
	printf("%6d: VIAJA ", md->clock);
	// Herói que viaja
	printf("HEROI %2d ", h);
	// Base que o herói sai
	printf("BASE %d ", md->bases[b]->id);
	// Base que o herói chega
	printf("BASE %d ", d);

	// Calcula distância entre base atual e base destino 
	distancia = calcula_distancia(md->bases[b]->local, d,
				      md->bases[b]->local, d);

	// Calcula duração da viagem
	duracao = distancia / md->herois[h]->velo;

	// Cria e insere ev chega na LEF
	novoEv = cria_evento(EV_CHEGA, md->clock + duracao, h, d);
	fprio_insere(lef, novoEv, EV_CHEGA, md->clock + duracao);

	// Imprime distância entre as bases
	printf("DIST %d ", distancia);
	// Imprime Velocidade do heroi 
	printf("VEL %d ", md->herois[h]->velo);
	// Imprime tempo de chegada na outra base
	printf("CHEGA %d\n", md->clock + duracao);
	// Imprime fila de espera da base atual
	lista_imprime(md->bases[b]->espera);

	return ev->tempo;
}

int trata_morre(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!md || !ev || !lef)
		return (-1);

	struct evento_t *novoEv;
	int h = ev->dado1;
	int m = ev->dado2;
	int b = md->herois[h]->base;

	// Imprime mensagens do evento morre
	printf("%6d: MORRE ", md->clock);
	// ID do herói
	printf("HEROI %2d ", h);
	// ID da missão
	printf("MISSAO %d\n", m);

	// Retira herói do conjunto de heróis da base
	cjto_retira(md->bases[b]->herois, h);

	// Muda status do herói para morto
	md->herois[h]->vivo = 0;

	// Cria e insere ev avisa na LEF
	novoEv = cria_evento(EV_AVISA, md->clock, h, b);
	fprio_insere(lef, novoEv, EV_AVISA, md->clock);

	return ev->tempo;
}

// Insertion Sort para ordenar vetor de distâncias entre missão e bases
void insertionSort(struct distancia_t dist[], int a, int b)
{
	if (!dist)
		return;

	int j;
	struct distancia_t key;

	for (int i = a + 1; i < b; i++) {
		key = dist[i];	// Elemento a ser inserido
		j = i - 1;	// Índice da parte do vetor ordenada

		// Descobre a posição correta do elemento a ser inserido
		while (j >= a && dist[j].d > key.d) {
			dist[j + 1] = dist[j];
			j--;
		}

		dist[j + 1] = key;
	}

	return;
}

int trata_missao(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{
	if (!ev || !md || !lef)
		return (-1);

	struct evento_t *novoEv;
	int BMP, risco;
	int m = ev->dado1;
	struct cjto_t *habsBase;
	int i;

	(md->missoes[m]->tenta)++;

	// Imprime mensagens do evento missão
	printf("%6d: MISSAO %d ", md->clock, m);
	// Número de tentaivas para concluir a missão
	printf("TENT %d ", md->missoes[m]->tenta);
	// Conjunto de habilidades para cumprir a missão
	printf("HAB REQ: ");
	cjto_imprime(md->missoes[m]->hab);
	printf("\n");

	// Somente na primeira tentativa da missão
	if (md->missoes[m]->tenta == 1) {
		// Cria vetor de distâncias entre missão e bases
		for (int i = 0; i < md->nBases; i++) {
			md->missoes[m]->distancias[i].d =
			    calcula_distancia(md->missoes[m]->local,
					      md->bases[i]->local,
					      md->missoes[m]->local,
					      md->bases[i]->local);
			md->missoes[m]->distancias[i].idBase = i;
		}
		// Ordena vetor de distâncias
		insertionSort(md->missoes[m]->distancias, 0, md->nBases);
	}

	i = 0;
	while (i < md->nBases) {

		habsBase = cjto_cria(N_HABILIDADES);

		// Base mais próxima
		BMP = md->missoes[m]->distancias[i].idBase;

		// Faz união de todas habilidades dos heróis da base mais próxima
		for (int j = 0; j < md->nHerois; j++)
			if (cjto_pertence(md->bases[BMP]->herois, j)) {
				struct cjto_t *novaHab;
				novaHab =
				    cjto_uniao(habsBase, md->herois[j]->hab);
				cjto_destroi(habsBase);
				habsBase = cjto_copia(novaHab);
				cjto_destroi(novaHab);
			}

		// Se habilidades da base contém ou é igual a habilidades da missão
		if (cjto_contem(habsBase, md->missoes[m]->hab)
		    || cjto_iguais(habsBase, md->missoes[m]->hab)) {

			// Imprime mensagens de missão cumprida
			printf("%6d: MISSAO %d CUMPRIDA ", md->clock, m);
			printf("BASE %d ", BMP);
			printf("HABs: ");
			cjto_imprime(md->missoes[m]->hab);
			printf("\n");

			// Calcula se herói sobrevive ou morre 
			for (int h = 0; h < md->nHerois; h++) {
				if (cjto_pertence(md->bases[BMP]->herois, h)) {

					risco = md->missoes[m]->perigo /
					    (md->herois[h]->pac +
					     md->herois[h]->exp + 1.0);
					if (risco > aleat(0, 30)) {
						// Cria e insere ev morre na LEF se morre
						novoEv =
						    cria_evento(EV_MORRE,
								md->clock, h,
								m);
						fprio_insere(lef, novoEv,
							     EV_MORRE,
							     md->clock);
					} else
						// Atualiza a experiência do herói se sobrevive
						(md->herois[h]->exp)++;
				}
			}

			md->missoes[m]->cumprida = 1;
			(md->nMissoesCumpridas)++;
			cjto_destroi(habsBase);
			break;
		}

		cjto_destroi(habsBase);
		i++;
	}

	// Se nenhuma base conseguir cumprir a missão
	if (!md->missoes[m]->cumprida) {
		// Imprime mensagens de missão impossível
		printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, m);
		novoEv = cria_evento(EV_MISSAO, md->clock + 24 * 60, m, -1);
		fprio_insere(lef, novoEv, EV_MISSAO, md->clock + 24 * 60);
	}

	return ev->tempo;
}

int trata_fim(struct mundo_t *md, struct evento_t *ev, struct fprio_t *lef)
{

	if (!md || !lef)
		return (-1);

	struct evento_t *novoEv;
	int maxTent, minTent, tenta, somaTent, mortes;
	float misCumpridas, mediaTent;

	// Encerra a simulação
	printf("%d: FIM\n", md->clock);
	// Cria e insere ev fim na LEF
	novoEv = cria_evento(EV_FIM, md->clock, -1, -1);
	fprio_insere(lef, novoEv, EV_FIM, md->clock);

	// Imprime informações sobre os heróis do mundo
	for (int i = 0; i < md->nHerois; i++) {
		printf("\nHEROI %2d ", md->herois[i]->id);
		if (md->herois[i]->vivo)
			printf("VIVO ");
		else
			printf("MORT ");
		printf("PAC %3d VEL %4d EXP %4d HABS ", md->herois[i]->pac,
		       md->herois[i]->velo, md->herois[i]->exp);
		cjto_imprime(md->herois[i]->hab);
	}

	printf("\n");
	// Imprime informações das bases do mundo
	for (int i = 0; i < md->nBases; i++) {
		printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n",
		       md->bases[i]->id, md->bases[i]->lota,
		       md->bases[i]->filaMax, md->bases[i]->missoes);
	}

	// Calcula missões cumpridas
	misCumpridas = (float)md->nMissoesCumpridas / md->nMissoes * 100;
	printf("EVENTOS TRATADOS: %d\n", md->nEventos);
	printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", md->nMissoesCumpridas,
	       md->nMissoes, misCumpridas);

	// Calcula Tentativas/Missões
	maxTent = md->missoes[0]->tenta;
	minTent = maxTent;
	somaTent = maxTent;
	for (int i = 1; i < md->nMissoes; i++) {
		tenta = md->missoes[i]->tenta;
		if (tenta > maxTent)	// Descobre maior n de tentativas
			maxTent = tenta;
		else		// Descobre menor n de tentativas
			minTent = tenta;
		somaTent += tenta;
	}

	// Faz a média do número de tentativas
	mediaTent = (float)somaTent / md->nMissoes;
	printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n",
	       minTent, maxTent, mediaTent);

	mortes = 0;
	// Calcula a taxa de mortalidade do mundo
	for (int i = 0; i < md->nHerois; i++) {
		if (!md->herois[i]->vivo)
			mortes++;
	}

	float taxaMorte = (float)mortes / md->nHerois * 100;
	printf("Mortes: %d ", mortes);
	printf("total: %d ", md->nHerois);

	printf("TAXA MORTALIDADE: %.1f%%\n", taxaMorte);

	return ev->tempo;
}
