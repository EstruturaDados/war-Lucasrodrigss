#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20

// =========================================================
// ESTRUTURA DE DADOS
// =========================================================
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// =========================================================
// PROTÓTIPOS DAS FUNÇÕES
// =========================================================

// Setup e memória
Territorio* alocarMapa(int n);
void inicializarTerritorios(Territorio *mapa, int n);
void liberarMemoria(Territorio *mapa);

// Interface com o usuário
void exibirMapa(const Territorio *mapa, int n);
void exibirMenu();
void exibirMissao(int idMissao);
void limparBufferEntrada();

// Lógica principal
void faseDeAtaque(Territorio *mapa, int n);
void simularAtaque(Territorio *atacante, Territorio *defensor);
int sortearMissao();
int verificarVitoria(int idMissao, const Territorio *mapa, int n, const char *corJogador);

// =========================================================
// FUNÇÃO PRINCIPAL
// =========================================================
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // Configuração inicial
    Territorio *mapa = alocarMapa(MAX_TERRITORIOS);
    inicializarTerritorios(mapa, MAX_TERRITORIOS);

    char corJogador[MAX_COR];
    printf("Digite a cor do seu exército: ");
    fgets(corJogador, MAX_COR, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';

    int missao = sortearMissao();
    int opcao, venceu = 0;

    // Loop principal
    do {
        printf("\n====================\n");
        exibirMapa(mapa, MAX_TERRITORIOS);
        printf("====================\n");
        exibirMissao(missao);
        exibirMenu();
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, MAX_TERRITORIOS);
                break;
            case 2:
                venceu = verificarVitoria(missao, mapa, MAX_TERRITORIOS, corJogador);
                if (venceu)
                    printf("\n🎉 PARABÉNS! Você completou sua missão!\n");
                else
                    printf("\nAinda não completou sua missão...\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

// =========================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// =========================================================

// Aloca o mapa dinamicamente
Territorio* alocarMapa(int n) {
    Territorio *mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    return mapa;
}

// Inicializa os territórios
void inicializarTerritorios(Territorio *mapa, int n) {
    const char *nomes[] = {"Brasil", "Argentina", "Chile", "Peru", "México"};
    const char *cores[] = {"Vermelho", "Verde", "Azul", "Amarelo", "Preto"};

    for (int i = 0; i < n; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = rand() % 5 + 1; // de 1 a 5 tropas
    }
}

// Libera a memória do mapa
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// Exibe o mapa formatado
void exibirMapa(const Territorio *mapa, int n) {
    printf("\n=== MAPA ATUAL ===\n");
    printf("%-3s | %-15s | %-10s | %s\n", "ID", "TERRITÓRIO", "COR", "TROPAS");
    printf("--------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-15s | %-10s | %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe o menu principal
void exibirMenu() {
    printf("\n=== MENU ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

// Exibe a missão atual
void exibirMissao(int idMissao) {
    printf("\n=== MISSÃO ===\n");
    if (idMissao == 1)
        printf("Destruir o exército Verde.\n");
    else if (idMissao == 2)
        printf("Conquistar 3 territórios.\n");
}

// Fase de ataque
void faseDeAtaque(Territorio *mapa, int n) {
    int a, d;
    exibirMapa(mapa, n);
    printf("\nEscolha o território atacante (1-%d): ", n);
    scanf("%d", &a);
    printf("Escolha o território defensor (1-%d): ", n);
    scanf("%d", &d);
    limparBufferEntrada();

    if (a < 1 || a > n || d < 1 || d > n || a == d) {
        printf("Escolha inválida!\n");
        return;
    }

    simularAtaque(&mapa[a-1], &mapa[d-1]);
}

// Simula uma batalha entre territórios
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter mais de 1 tropa!\n");
        return;
    }

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\n%s (A) rolou %d\n", atacante->nome, dadoA);
    printf("%s (D) rolou %d\n", defensor->nome, dadoD);

    if (dadoA >= dadoD) {
        defensor->tropas--;
        printf("Atacante venceu! %s perde 1 tropa.\n", defensor->nome);
        if (defensor->tropas <= 0) {
            printf("%s foi conquistado!\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1;
            atacante->tropas--;
        }
    } else {
        atacante->tropas--;
        printf("Defensor resistiu! %s perde 1 tropa.\n", atacante->nome);
    }
}

// Sorteia uma missão aleatória
int sortearMissao() {
    return rand() % 2 + 1; // 1 ou 2
}

// Verifica se o jogador cumpriu a missão
int verificarVitoria(int idMissao, const Territorio *mapa, int n, const char *corJogador) {
    int contConquistados = 0;

    if (idMissao == 1) { // Destruir exército verde
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0)
                return 0; // Ainda existe exército verde
        }
        return 1;
    }

    if (idMissao == 2) { // Conquistar 3 territórios
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                contConquistados++;
        }
        return (contConquistados >= 3);
    }

    return 0;
}

// Limpa o buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
