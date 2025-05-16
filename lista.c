// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

// Cria uma lista vazia.
struct lista_t *lista_cria()
{
	struct lista_t *lst = malloc(sizeof(struct lista_t));
	if (!lst)
		return NULL;

	lst->tamanho = 0;
	lst->prim = NULL;
	lst->ult = NULL;

	return lst;
}

// Remove todos os itens da lista e libera a memória.
struct lista_t *lista_destroi(struct lista_t *lst)
{
	int *item;

	//caso não tenha elementos
	if (!lst->prim) {
		free(lst);
		return NULL;
	}

	if (!lst)
		return NULL;

	while (lst->tamanho > 0) {
		item = &lst->prim->valor;
		lista_retira(lst, item, 0);
	}

	free(lst);

	return NULL;
}

// Insere o item na lista na posição indicada;
int lista_insere(struct lista_t *lst, int item, int pos)
{
	struct item_t *novoItem, *aux;

	if (!lst)
		return -1;

	novoItem = malloc(sizeof(struct item_t));
	if (!novoItem)
		return -1;

	novoItem->valor = item;
	novoItem->ant = NULL;
	novoItem->prox = NULL;

	//se a lista estiver vazia
	if (!lst->prim) {
		lst->prim = novoItem;
		lst->ult = novoItem;

		lst->tamanho++;
		return lst->tamanho;
	}
	//insere item no final da lista
	if (pos == -1 || pos > lst->tamanho) {
		lst->ult->prox = novoItem;
		novoItem->ant = lst->ult;
		lst->ult = novoItem;
		novoItem->prox = NULL;
	}
	//insere item no começo da lista
	else if (pos == 0) {
		lst->prim->ant = novoItem;
		novoItem->prox = lst->prim;
		lst->prim = novoItem;
	}
	//insere item no meio da lista
	else {
		aux = lst->prim->prox;
		for (int i = 1; i <= pos; i++) {
			if (i == pos) {
				novoItem->prox = aux;
				novoItem->ant = aux->ant;
				aux->ant->prox = novoItem;
				aux->ant = novoItem;
			}
			aux = aux->prox;
		}
	}

	lst->tamanho++;
	return lst->tamanho;
}

// Retira o item da lista da posição indicada.
int lista_retira(struct lista_t *lst, int *item, int pos)
{
	struct item_t *aux;

	if (!lst)
		return -1;

	if (!item)
		return -1;

	//se a lista estiver vazia
	if (lst->tamanho < 1)
		return -1;

	//se a posição exeder o tamanho da lista
	if (pos >= lst->tamanho)
		return -1;

	if (lst->tamanho == 1) {
		aux = lst->ult;
		lst->prim = NULL;
		lst->ult = NULL;
	}
	//retira último elemento
	else if ((pos == lst->tamanho - 1) || (pos == -1)) {
		aux = lst->ult;
		lst->ult = lst->ult->ant;
		lst->ult->prox = NULL;
	}
	//retira primeiro elemento
	else if (pos == 0) {
		aux = lst->prim;
		lst->prim = lst->prim->prox;
		lst->prim->ant = NULL;
	}
	//retira elementos intermediários
	else {
		aux = lst->prim->prox;
		for (int i = 1; i < pos; i++)
			aux = aux->prox;

		aux->ant->prox = aux->prox;
		aux->prox->ant = aux->ant;
	}

	lst->tamanho--;
	*item = aux->valor;
	free(aux);

	return lst->tamanho;
}

// Informa o valor do item na posição indicada, sem retirá-lo.
int lista_consulta(struct lista_t *lst, int *item, int pos)
{
	struct item_t *aux;

	if (!lst)
		return -1;

	if (!item)
		return -1;

	if (!lst->prim)
		return -1;

	if (pos >= lst->tamanho)
		return -1;

	//informa o valor do último item
	*item = 0;
	if (pos == -1) {
		aux = lst->ult;
		*item = aux->valor;
	}
	//informa primeiro valor ao antepenultimo 
	else {
		aux = lst->prim;
		for (int i = 0; i <= pos && i < lst->tamanho; i++) {
			if (i == pos)
				*item = aux->valor;

			aux = aux->prox;
		}
	}

	return lst->tamanho;
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
int lista_procura(struct lista_t *lst, int valor)
{
	struct item_t *aux;

	if (!lst)
		return -1;

	aux = lst->prim;
	for (int i = 0; i < lst->tamanho; i++) {
		if (valor == aux->valor)
			return i;

		aux = aux->prox;
	}

	return -1;
}

// Informa o tamanho da lista (o número de itens presentes nela).
int lista_tamanho(struct lista_t *lst)
{
	if (!lst)
		return -1;

	return lst->tamanho;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ..."
void lista_imprime(struct lista_t *lst)
{
	struct item_t *aux;

	if (!lst)
		return;

	if (!lst->prim)
		return;

	printf("%d", lst->prim->valor);
	aux = lst->prim->prox;
	while (aux) {
		printf(" %d", aux->valor);
		aux = aux->prox;
	}

	return;
}
