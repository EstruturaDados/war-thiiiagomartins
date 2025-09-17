// ============================================================================
// UTILS.C - FUNÇÕES RELACIONADAS AO JOGO
// ============================================================================
#include "war.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


// --- Constantes Globais ---
// Definição dos nomes das cores
const char* cores[] = {
    "Vermelho",
    "Azul",
    "Verde",
    "Amarelo",
    "Branco",
    "Lilás"
};
const int TOTAL_CORES = sizeof(cores) / sizeof(cores[0]);
// Definição das missões do jogo
static const char* missoes_fixas[] = {
    "Conquistar 2 territórios",
    "Conquistar o mundo"
};

char** missoes = NULL;
int TOTAL_MISSOES = 0;

// --- Funções de setup e gerenciamento de memória ---
// Limpa o buffer de entrada do teclado.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// libera a memória armazenada
void liberarMemoria(struct Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

// Libera o espaço das missões
void liberarMissoes() {
    for (int i = 0; i < TOTAL_MISSOES; i++) {
        free(missoes[i]);
    }
    free(missoes);
    missoes = NULL;
    TOTAL_MISSOES = 0;
    printf("\nMemória de missões liberada com sucesso.\n");
}


// --- Funções de interface com o usuário ---
// Permite escolher a quantidade de territórios que serão utilizados no jogo
int alocarMapa() {
    int num_territorios;
    int entrada_valida = 0;
    do {
        printf("Quantos territórios você deseja cadastrar? ");
        // Verifica se o numero é positivo
        if (scanf("%d", &num_territorios) == 1 && num_territorios > 0) {
            // Marca a entrada como válida para sair do loop
            entrada_valida = 1;
        } else {
            printf("Entrada inválida. Por favor, digite um número inteiro positivo.\n");
            limparBufferEntrada();
        }
    } while (!entrada_valida);

    return num_territorios;
}

// Preenche os dados de um vetor de territórios.
void preencherTerritorios(struct Territorio *territorios, int num_territorios) {
    int num_cores = TOTAL_CORES;
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
            printf("  %d = %s\n", j, cores[j]);
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

// exibe o menu principal do jogo
void exibirMenuPrincipal() {
    printf("\n#### MENU DE AÇÕES ####\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do jogo\n");
    printf("Escolha uma opção: ");
}

// Função de configuração para a fase de ataque
int faseDeAtaque(struct Territorio *territorios, int num_territorios, TipoMissao missao_jogador, Cor cor_jogador) {
    int num_atacante, num_defensor;
    printf("\n#### FASE DE ATAQUE ####\n");

    // Em caso de -1, não verifica se o atacante está atacando a si mesmo
    num_atacante = escolherTerritorio("Escolha o território ATACANTE (digite 0 para sair): ", num_territorios, -1);
    // Se escolher 0 interrompe o loop
    if (num_atacante == 0) {
        printf("\nFase de ataque encerrada.\n");
        return 0;
    }
    do {
        num_defensor = escolherTerritorio("Escolha o território DEFENSOR (digite 0 para sair): ", num_territorios, num_atacante);
        if (territorios[num_atacante - 1].cor_exercito == territorios[num_defensor - 1].cor_exercito) {
            printf("Você não pode atacar um território aliado. Tente novamente. >:-( \n");
        } else {
            break;
        }
    } while (1);
    // Chama a função que executa a batalha
    simularAtaque(&territorios[num_atacante - 1], &territorios[num_defensor - 1]);
    // Verifica se o jogador executou a missão
    if (verificarVitoria(missao_jogador, territorios, num_territorios, cor_jogador)) {
        return 1;
    }
    return 0;
}


// --- Funções de lógica principal do jogo ---
// Escolhe uma das missões e atribui ao jogador
void atribuirMissao(char* missao_escolhida, const char* missoes[], int totalMissoes) {
    // Sorteia uma missão aleatória
    // int indice_sorteado = rand() % totalMissoes;
    int indice_sorteado = 1;

    // Armazena a missão sorteada
    strcpy(missao_escolhida, missoes[indice_sorteado]);
}

// Escolhe uma das cores e atribui ao jogador
void atribuirCor(char* cor_escolhida, const char* cores[], int totalCores) {
    // Sorteia uma cor aleatória
    // int indice_sorteado = rand() % totalCores;
    int indice_sorteado = 1;
    // Armazena a cor sorteada
    strcpy(cor_escolhida, cores[indice_sorteado]);
}

// Verifica o resultado da batalha
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor) {
    int dado_atacante, dado_defensor;

    // Rola o dado do atacante (1 a 6)
    dado_atacante = (rand() % 6) + 1;
    printf("O ATACANTE %s rolou um dado e tirou: %d.\n", atacante->nome, dado_atacante);

    // Rola o dado do defensor (1 a 6)
    dado_defensor = (rand() % 6) + 1;
    printf("O DEFENSOR %s rolou um dado e tirou: %d.\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        // Atacante vence
        printf("VITÓRIA DO ATAQUE! o defensor perdeu 1 tropa.\n");
        defensor->quantidade_tropas--;
        if (defensor->quantidade_tropas == 0) {
            printf("CONQUISTA! O território %s foi dominado pelo exercito %s!\n",
                defensor->nome,
                cores[atacante->cor_exercito]
            );
            defensor->cor_exercito = atacante->cor_exercito;
            // transfere metade das tropas do atacante para o defensor
            int tropas_transferidas = atacante->quantidade_tropas / 2;
            defensor->quantidade_tropas = tropas_transferidas;
            atacante->quantidade_tropas -= tropas_transferidas;

        }
    } else {
        // Defensor vence ou há empate
        printf("O DEFENSOR venceu a batalha!\n");
        atacante->quantidade_tropas--;
    }
}

// Verifica se o jogador cumpriu a missão
// Retorna 1 se a missão foi cumprida, e 0 caso contrário.
int verificarVitoria(const TipoMissao missao, const struct Territorio* territorios, int num_territorios, const Cor cor_jogador) {
    switch (missao) {
        case CONQUISTAR_2_TERRITORIOS: {
            // Conta quantos territorios o jogador domina
            int territorios_conquistados = 0;
            for (int i = 0; i < num_territorios; i++) {
                if (territorios[i].cor_exercito == cor_jogador) {
                    territorios_conquistados++;
                }
            }
            return territorios_conquistados >= 2 ? 1 : 0;
        }
        case ELIMINAR_VERMELHO:
        case ELIMINAR_AZUL:
        case ELIMINAR_VERDE:
        case ELIMINAR_AMARELO:
        case ELIMINAR_BRANCO:
        case ELIMINAR_LILAS: {
            const char *missao_string = missoes[missao];
            const char *cor_eliminar = strrchr(missao_string, ' ');
            if (cor_eliminar != NULL) {
                cor_eliminar++;
            }
            int id_cor_eliminar = getEnumID(cor_eliminar, cores, TOTAL_CORES);

            for (int i = 0; i < num_territorios; i++) {
                if (territorios[i].cor_exercito == id_cor_eliminar) {
                    return 0;
                }
            }
            return 1;
        }
        case CONQUISTAR_MUNDO: {
            int territorios_conquistados = 0;
            for (int i = 0; i < num_territorios; i++) {
                //Verifica quantos territórios conquistou
                if (territorios[i].cor_exercito == cor_jogador) {
                    territorios_conquistados++;
                }
            }
            return territorios_conquistados == num_territorios ? 1 : 0;
        }
        default:
            return 0;
    }
}


// --- Função utilitária ---
// Exibe os dados dos territórios. Usa 'const' para garantir que não os modificará.
void exibirMapa(const struct Territorio *territorios, int num_territorios) {
    printf("\n#### MAPA DO MUNDO - ESTADO ATUAL ####");
    for (int i = 0; i < num_territorios; i++) {
        printf("\n> %d. %s (Exército %s, Tropas: %d)",
            i + 1,
            territorios[i].nome,
            cores[territorios[i].cor_exercito],
            territorios[i].quantidade_tropas
        );
    }
    printf(" \n");
}

// Verifica qual a opção escolhida no menu e redireciona para a função correta
int opcaoMenu(int opcao_menu, struct Territorio *territorios, int num_territorios, const TipoMissao missao_jogador, const Cor cor_jogador) {
    switch (opcao_menu) {
        case 1:
            int result = faseDeAtaque(territorios, num_territorios, missao_jogador, cor_jogador);
            if (result == 1) {
                printf("\nParabéns! Missão dada é missão cumprida! Você venceu o jogo!\n");
                return 0;
            }
            return 1;
        case 2:
            if (verificarVitoria(missao_jogador, territorios, num_territorios, cor_jogador)) {
                printf("\nParabéns! Missão dada é missão cumprida! Você cumpriu sua missão e venceu o jogo!\n");
                return 0;
            }
            printf("Missão não cumprida ainda. Continue jogando!\n");
            return 1;
        case 0:
            printf("\nEncerrando o jogo. Até mais!\n");
            return 0;
        default:
            printf("\nOpção inválida. Por favor, escolha 1, 2 ou 0. >:-(\n");
            return 1;
    }
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

// Função que converte a string da missão para o seu ID (enum)
int getEnumID(const char* string, const char* array_strings[], int total_elements) {
    for (int i = 0; i < total_elements; i++) {
        if (strcmp(string, array_strings[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// cria o array com as missões que serão utilizadas no jogo, de acordo com as cores
void criarMissoes() {
    int totalFixas = sizeof(missoes_fixas) / sizeof(missoes_fixas[0]);
    TOTAL_MISSOES = totalFixas + TOTAL_CORES; // fixas + cores

    // Aloca memória para o array de ponteiros
    missoes = malloc(TOTAL_MISSOES * sizeof(char*));
    if (!missoes) {
        perror("malloc missoes");
        exit(EXIT_FAILURE);
    }

    // Copia as missões fixas
    for (int i = 0; i < totalFixas; i++) {
        missoes[i] = strdup(missoes_fixas[i]);
        if (!missoes[i]) { perror("strdup"); exit(EXIT_FAILURE); }
    }

    // Gera as missões de eliminação por cor
    for (int i = 0; i < TOTAL_CORES; i++) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Eliminar o exército %s", cores[i]);
        missoes[totalFixas + i] = strdup(buffer);
        if (!missoes[totalFixas + i]) { perror("strdup"); exit(EXIT_FAILURE); }
    }
}
