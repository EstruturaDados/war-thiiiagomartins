// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <time.h>
#include "war.h"

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    criarMissoes();
    int num_territorios = alocarMapa();
    limparBufferEntrada();
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    struct Territorio *territorios = calloc(num_territorios, sizeof(struct Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    preencherTerritorios(territorios, num_territorios);
    // - Define a cor do jogador e sorteia sua missão secreta.
    char* missao_jogador = malloc(TAM_STRING * sizeof(char));
    if (missao_jogador == NULL) {
        printf("Erro ao alocar memoria para a missao.\n");
        liberarMemoria(territorios);
        return 1;
    }
    atribuirMissao(missao_jogador, missoes, TOTAL_MISSOES);
    TipoMissao id_missao_jogador = (TipoMissao)getEnumID(missao_jogador, missoes, TOTAL_MISSOES);

    char* cor_jogador = malloc(TAM_STRING * sizeof(char));
    if (cor_jogador == NULL) {
        printf("Erro ao alocar memoria para a cor.\n");
        liberarMemoria(territorios);
        return 1;
    }
    atribuirCor(cor_jogador, cores, TOTAL_CORES);
    Cor id_cor_jogador = (Cor)getEnumID(cor_jogador, cores, TOTAL_CORES);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    int opcao_menu;
    int segue_o_jogo;
    do {
        // exibe a missão do jogador
        printf("\n#### SUA MISSÃO (Exército %s) ####", cor_jogador);
        printf("\n>> %s\n", missao_jogador);

        // retorna o resultado
        exibirMapa(territorios, num_territorios);

        // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        exibirMenuPrincipal();

        if (scanf("%d", &opcao_menu) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();
        // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        //   - Opção 1: Inicia a fase de ataque.
        //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
        //   - Opção 0: Encerra o jogo.
        segue_o_jogo = opcaoMenu(opcao_menu, territorios, num_territorios, id_missao_jogador, id_cor_jogador);
        // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        printf("Pressione Enter para continuar...");
        getchar();
    } while (segue_o_jogo);
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorios);
    liberarMissoes();
    return 0;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

