// ============================================================================
// TERRITORIO.C - FUNÇÕES RELACIONADAS AOS TERRITÓRIOS
// ============================================================================
#include "war.h"
#include <stdio.h>
#include <string.h>

// --- Definição dos nomes das cores ---
const char* nomes_cores[] = {
    "Vermelho",
    "Azul",
    "Verde",
    "Amarelo",
    "Branco",
    "Lilás"
};

// --- Implementação da Função Utilitária ---
// Limpa o buffer de entrada do teclado.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Implementação da Função de Lógica ---
// Retorna o número total de cores disponíveis.
int getNumCores() {
    return sizeof(nomes_cores) / sizeof(nomes_cores[0]);
}

// --- Implementação das Funções de Interface com o Usuário ---
// Preenche os dados de um vetor de territórios.
void preencherTerritorios(struct Territorio territorios[]) {
    int num_cores = getNumCores();
    printf("### CADASTRO DE TERRITÓRIOS ###\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n#### Cadastro do Território %d ####\n", i + 1);

        printf("Nome do território: ");
        fgets(territorios[i].nome, TAM_STRING, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        int cor_int;
        printf("Cor do Exército. Escolha uma das opções abaixo:\n");
        for (int j = 0; j < num_cores; j++) {
            printf("  %d = %s\n", j, nomes_cores[j]);
        }

        printf("Sua escolha: ");
        scanf("%d", &cor_int);
        limparBufferEntrada();

        if (cor_int >= 0 && cor_int < num_cores) {
            territorios[i].cor_exercito = (Cor)cor_int;
        } else {
            printf("Cor inválida! Por favor, use uma das opções. Vamos tentar novamente para este território.\n");
            i--;
            continue;
        }

        printf("Número de tropas: ");
        scanf("%d", &territorios[i].quantidade_tropas);
        limparBufferEntrada();
    }
}

// Exibe os dados dos territórios. Usa 'const' para garantir que não os modificará.
void exibirTerritorios(const struct Territorio territorios[]) {
    printf("\n--- LISTAGEM DE TERRITORIOS CADASTRADOS ---\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exercito: %s\n", nomes_cores[territorios[i].cor_exercito]);
        printf("Quantidade de Tropas: %d\n", territorios[i].quantidade_tropas);
    }
}