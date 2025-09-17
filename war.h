// ============================================================================
// TERRITORIO.H
// Contém definições de constantes, estruturas e protótipos de funções
// relacionadas ao jogo.
// ============================================================================

#ifndef PROJETO_WAR_H
#define PROJETO_WAR_H

// --- Inclusão de Bibliotecas ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globais ---
#define TAM_STRING 100
extern const char* cores[];
extern const int TOTAL_CORES;
extern char** missoes;
extern int TOTAL_MISSOES;

// --- Estrutura de Dados ---
typedef enum {
    VERMELHO,
    AZUL,
    VERDE,
    AMARELO,
    BRANCO,
    LILÁS
} Cor;

typedef enum {
    CONQUISTAR_2_TERRITORIOS,
    CONQUISTAR_MUNDO,
    ELIMINAR_VERMELHO,
    ELIMINAR_AZUL,
    ELIMINAR_VERDE,
    ELIMINAR_AMARELO,
    ELIMINAR_BRANCO,
    ELIMINAR_LILAS,
} TipoMissao;

struct Territorio {
    char nome[TAM_STRING];
    Cor cor_exercito;
    int quantidade_tropas;
};

// Funções de setup e gerenciamento de memória:
void limparBufferEntrada();
void liberarMemoria(struct Territorio* mapa);
void criarMissoes();
void liberarMissoes();
// Funções de interface com o usuário:
int alocarMapa();
void preencherTerritorios(struct Territorio *territorios, int num_territorios);
void exibirMenuPrincipal();
int faseDeAtaque(struct Territorio *territorios, int num_territorios, TipoMissao missao_jogador, Cor cor_jogador);
// Funções de lógica principal do jogo:
void atribuirMissao(char* missao_escolhida, const char* missoes[], int totalMissoes);
void atribuirCor(char* cor_escolhida, const char* cores[], int totalCores);
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);
int verificarVitoria(const TipoMissao missao, const struct Territorio* territorios, int num_territorios, Cor cor_jogador);
// Função utilitária:
void exibirMapa(const struct Territorio *territorios, int num_territorios);
int opcaoMenu(int opcao_menu, struct Territorio *territorios, int num_territorios, TipoMissao missao_jogador, Cor cor_jogador);
int escolherTerritorio(const char* prompt, int num_territorios, int ignorar_territorio);
int getEnumID(const char* string, const char* array_strings[], int total_elements);

#endif