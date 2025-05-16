#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
struct fprio_t *fprio_cria()
{
	struct fprio_t *f = malloc(sizeof(struct fprio_t));
	if (!f)
		return (NULL);

	f->prim = NULL;
	f->num = 0;

	return (f);
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
struct fprio_t *fprio_destroi(struct fprio_t *f)
{
	int tipo, prio;
	void *item;

	if (!f)
		return (NULL);

	// Retira todos itens da fila.
	while (fprio_tamanho(f) > 0) {
		item = fprio_retira(f, &tipo, &prio);
		if (item)
			free(item);
	}

	free(f);

	return (NULL);
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes (FIFO).
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
	struct fpnodo_t *novoNodo, *aux, *prev;

	if (!f || !item)
		return (-1);

	// Cria e inicia novo nodo.
	novoNodo = malloc(sizeof(struct fpnodo_t));
	if (!novoNodo)
		return (-1);

	novoNodo->item = item;
	novoNodo->tipo = tipo;
	novoNodo->prio = prio;
	novoNodo->prox = NULL;

	if (f->prim && f->prim->item == item) {
		// Verifica se já existe item no começo da fila.
		free(novoNodo);
		return (-1);
	}
	// Insere nodo na fila.
	if (!f->prim || prio < f->prim->prio) {
		// Fila vazia ou novo nodo tem prioridade maior que o primeiro.
		novoNodo->prox = f->prim;
		f->prim = novoNodo;
	} else {
		// Fila não vazia ou prioridade menor que o primeiro. 
		prev = f->prim;
		aux = f->prim->prox;

		while (aux && prio >= aux->prio) {
			// Verifica se já existe item no meio da fila.
			if (aux->item == item) {
				free(novoNodo);
				return (-1);
			}

			prev = aux;
			aux = aux->prox;
		}

		prev->prox = novoNodo;
		novoNodo->prox = aux;
	}

	f->num++;

	return (f->num);
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade.
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
	void *item, *nodoRemovido;

	if (!f || !tipo || !prio)
		return (NULL);

	// Guarda informações que precisam ser devolvidas.
	*tipo = f->prim->tipo;
	*prio = f->prim->prio;
	item = f->prim->item;

	// Remove primeiro nodo.
	nodoRemovido = f->prim;
	f->prim = f->prim->prox;
	f->num--;

	free(nodoRemovido);

	return (item);
}

// Informa o número de itens na fila.
int fprio_tamanho(struct fprio_t *f)
{
	if (!f)
		return (-1);

	return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
void fprio_imprime(struct fprio_t *f)
{
	struct fpnodo_t *aux;

	if (!f || !f->prim)
		return;

	printf("(%d %d)", f->prim->tipo, f->prim->prio);
	aux = f->prim->prox;
	while (aux) {
		printf(" (%d %d)", aux->tipo, aux->prio);
		aux = aux->prox;
	}

	return;
}
