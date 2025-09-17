// ============================================================================
// TERRITORIO.H
// Contém definições de constantes, estruturas e protótipos de funções
// relacionadas aos territórios.
// ============================================================================

#ifndef PROJETO_WAR_H
#define PROJETO_WAR_H

// --- Inclusão de Bibliotecas ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globais ---
#define TAM_STRING 100

// --- Estrutura de Dados ---
typedef enum {
    VERMELHO,
    AZUL,
    VERDE,
    AMARELO,
    BRANCO,
    LILÁS
} Cor;

struct Territorio {
    char nome[TAM_STRING];
    Cor cor_exercito;
    int quantidade_tropas;
};

// Funções de interface com o usuário e de sistema
void limparBufferEntrada();
void liberarMemoria(struct Territorio* mapa);
int getNumCores();
void preencherTerritorios(struct Territorio *territorios, int num_territorios);
void exibirMapa(const struct Territorio *territorios, int num_territorios);
int alocarMapa();
void faseDeAtaque(struct Territorio *territorios, int num_territorios);
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);

// Função genérica
int escolherTerritorio(const char* prompt, int num_territorios, int ignorar_territorio);

#endif