// ============================================================================
// TERRITORIO.C - FUNÇÕES RELACIONADAS AOS TERRITÓRIOS
// ============================================================================
#include "war.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Definição dos nomes das cores
const char* nomes_cores[] = {
    "Vermelho",
    "Azul",
    "Verde",
    "Amarelo",
    "Branco",
    "Lilás"
};

// Limpa o buffer de entrada do teclado.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Retorna o número total de cores disponíveis.
int getNumCores() {
    return sizeof(nomes_cores) / sizeof(nomes_cores[0]);
}

// Preenche os dados de um vetor de territórios.
void preencherTerritorios(struct Territorio *territorios, int num_territorios) {
    int num_cores = getNumCores();
    printf("### CADASTRO DE TERRITÓRIOS ###\n");
    // Executa um loop até preencher os dados de acordo com a quantidade de territórios
    for (int i = 0; i < num_territorios; i++) {
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
        // Verifica se a cor escolhida é válida
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
void exibirMapa(const struct Territorio *territorios, int num_territorios) {
    printf("\n#### MAPA DO MUNDO - ESTADO ATUAL ####\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("\n> %d. %s (Exército %s, Tropas: %d)",
            i + 1,
            territorios[i].nome,
            nomes_cores[territorios[i].cor_exercito],
            territorios[i].quantidade_tropas
        );
    }
}

// Permite escolher a quantidade de territórios que serão utilizados no jogo
int alocarMapa() {
    int num_territorios;
    int entrada_valida = 0;
    do {
        printf("Quantos territorios voce deseja cadastrar? ");
        // Verifica se o numero é positivo
        if (scanf("%d", &num_territorios) == 1 && num_territorios > 0) {
            // Marca a entrada como válida para sair do loop
            entrada_valida = 1;
        } else {
            printf("Entrada invalida. Por favor, digite um numero inteiro positivo.\n");
            limparBufferEntrada();
        }
    } while (!entrada_valida);

    return num_territorios;
}

// Verifica o resultado da batalha
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor) {
    srand(time(NULL));
    int dado_atacante, dado_defensor;

    // Rola o dado do atacante (1 a 6)
    dado_atacante = (rand() % 6) + 1;
    printf("O atacante %s rolou um dado e tirou: %d.\n", atacante->nome, dado_atacante);

    // Rola o dado do defensor (1 a 6)
    dado_defensor = (rand() % 6) + 1;
    printf("O defensor %s rolou um dado e tirou: %d.\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        // Atacante vence
        printf("VITÓRIA DO ATAQUE! o defensor perdeu 1 tropa.\n");
        defensor->quantidade_tropas--;
        if (defensor->quantidade_tropas == 0) {
            printf("CONQUISTA! O território %s foi dominado pelo exercito %s!\n",
                defensor->nome,
                nomes_cores[atacante->cor_exercito]
            );
            defensor->cor_exercito = atacante->cor_exercito;
            // transfere metade das tropas do atacante para o defensor
            int tropas_transferidas = atacante->quantidade_tropas / 2;
            defensor->quantidade_tropas = tropas_transferidas;
            atacante->quantidade_tropas -= tropas_transferidas;

        }
    } else {
        // Defensor vence ou há empate
        printf("O defensor venceu a batalha!\n");
        atacante->quantidade_tropas--;
    }
}

// Função de configuração para a fase de ataque
void faseDeAtaque(struct Territorio *territorios, int num_territorios) {
    int num_atacante, num_defensor;
    do {
        printf("\n#### FASE DE ATAQUE ####\n");

        // Em caso de -1, não verifica se o atacante está atacando a si mesmo
        num_atacante = escolherTerritorio("Escolha o território ATACANTE (digite 0 para sair): ", num_territorios, -1);
        // Se escolher 0 interrompe o loop
        if (num_atacante == 0) {
            printf("\nFase de ataque encerrada.\n");
            break;
        }
        do {
            num_defensor = escolherTerritorio("Escolha o território DEFENSOR (digite 0 para sair): ", num_territorios, num_atacante);
            if (territorios[num_atacante - 1].cor_exercito == territorios[num_defensor - 1].cor_exercito) {
                printf("Você não pode atacar um território aliado. Tente novamente.\n");
            } else {
                break;
            }
        } while (1);
        // Chama a função que executa a batalha
        simularAtaque(&territorios[num_atacante - 1], &territorios[num_defensor - 1]);

        // retorna o resultado
        exibirMapa(territorios, num_territorios);
    } while (1);
}

// Escolhe qual território vai atacar ou defender
// Se ignorar_territorio for -1, não verifica se o atacante está atacando a si mesmo
int escolherTerritorio(const char* prompt, int num_territorios, int ignorar_territorio) {
    int escolha;
    do {
        printf("%s", prompt);
        // Verifica se a entrada é um número válido
        if (scanf("%d", &escolha) != 1 || (escolha < 1 || escolha > num_territorios) && escolha != 0) {
            printf("Território inválido. Tente novamente!!!\n");
            limparBufferEntrada();
        // Verifica se a escolha é o território que deve ser ignorado
        } else if (escolha == ignorar_territorio) {
            printf("Um territorio não pode atacar a si mesmo. Escolha outro defensor! >:-( \n");
            limparBufferEntrada();
        } else {
            limparBufferEntrada();
            return escolha;
        }
    } while (1);
}

// libera a memória armazenada
void liberarMemoria(struct Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}
