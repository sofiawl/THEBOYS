#ifndef MUNDO
#define MUNDO

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS	N_HABILIDADES * 5
#define N_BASES	N_HEROIS / 5
#define N_MISSOES	T_FIM_DO_MUNDO / 100

struct heroi_t {
	int id;			// Identificação do herói
	int vivo;		// 0 para morto e 1 para vivo
	struct cjto_t *hab;	// Habilidades do herói
	int pac;		// Paciência do herói
	int velo;		// Velocidade do herói
	int exp;		// Experiência do herói
	int base;		// ID da base em que o herói se encontra

};

struct base_t {
	int id;			// Identificação da base
	int lota;		// Número máximo de heróis na base
	int missoes;		// Número de missões que participou
	int filaMax;		// Número máximo de heróis na fila
	struct cjto_t *herois;	// ID dos heróis que estão na base
	struct lista_t *espera;	// Fila de ID de herois esperam para entrar
	int local;		// Localização da base (x = y)
};

struct distancia_t {
	int d;			// Distâncias entre da missão e bases
	int idBase;		// Identificação das bases
};

struct missao_t {
	int id;			// Identificação da missão
	int cumprida;		// 1 para missão cumprida 0 para não cumprida
	int tenta;		// Número de tentativas para cumprir a missão
	struct cjto_t *hab;	// Habilidades necessárias para missão
	int perigo;		// Nível de perigo
	int local;		// Localização da base (x = y)
	struct distancia_t distancias[N_BASES];	// Vetor de distâncias entre
	//missão e bases 
};

struct mundo_t {
	int nHerois;		// Número total de heróis no mundo
	int nBases;		// Número total de bases no mundo
	int nMissoes;		// Número total de missões no mundo
	int nMissoesCumpridas;	// Missões que foram cumpridas
	int nHab;		// Número de habilidades do mundo
	int nEventos;		// Quanridade de eventos tratados no mundo
	int tamMundo;		// Coordenadas máximas do plano cartesiano (x = y)
	int clock;		// Tempo atual do mundo
	int tempoFinal;		// Tempo fim do mundo
	struct missao_t *missoes[N_MISSOES];	// Vetor de missões do mundo
	struct heroi_t *herois[N_HEROIS];	// Vetor de heróis do mundo
	struct base_t *bases[N_BASES];	// Vetor de bases do mundo
};

// Retorna um número aleatório entre min e max inclusive
int aleat(int min, int max);

// Inicializa mundo, retorna mundo
struct mundo_t *mundo_cria();

// Libera as memórias alocadas para mundo
void mundo_destroi(struct mundo_t *md);

// Inicializa herói, retorna herói
struct heroi_t *heroi_cria(int i);

// Libera as memórias alocadas para herói
void heroi_destroi(struct heroi_t *hr);

// Insere herói do índice i do vetor de heróis
// retorna -1 para erro ou id do herói
int heroi_insere(struct heroi_t *hr, struct mundo_t *md);

// Retira o id do herói do vetor de heróis do mundo
// retorna -1 para erro ou id do herói
int heroi_retira(int index, struct heroi_t *hr, struct mundo_t *md);

// Inicializa a base
struct base_t *base_cria(int i);

// Libera memórias alocadas pela base
void base_destroi(struct base_t *bs);

// Insere base no índice i do vetor de bases
// retorna -1 para erro ou id da base
int base_insere(struct base_t *bs, struct mundo_t *md);

// Retira id da base do vetor de bases do mundo
// retorna -1 para erro ou id da base
int base_retira(int index, struct base_t *bs, struct mundo_t *md);

// Inicializa missão
struct missao_t *missao_cria(int i);

// Libera memórias alocadas para missão
void missao_destroi(struct missao_t *mis);

// Insere id da missão no vetor de missões do mundo
// retorna -1 para erro ou id da missão
int missao_insere(struct missao_t *mis, struct mundo_t *md);

// Retira id da missão do vetor missões do mundo
// retorna -1 para erro ou id da missão
int missao_retira(int index, struct missao_t *mis, struct mundo_t *md);

#endif
