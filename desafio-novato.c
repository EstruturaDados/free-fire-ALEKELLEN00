#include <stdio.h>
#include <string.h>

// 1. Estrutura do Item (Struct)
typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
} Item;

// 2. Inventário e Variáveis Globais
#define CAPACIDADE_MAXIMA 10
Item mochila[CAPACIDADE_MAXIMA];
int num_itens = 0;

// 4. Implementação da funcionalidade de Adicionar Item
void adicionar_item() {
    if (num_itens >= CAPACIDADE_MAXIMA) {
        printf("\n🚫 Mochila Cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    printf("\n--- Adicionar Novo Item ---\n");
    while (getchar() != '\n'); // Limpa o buffer

    printf("Nome do Item: ");
    fgets(mochila[num_itens].nome, sizeof(mochila[num_itens].nome), stdin);
    mochila[num_itens].nome[strcspn(mochila[num_itens].nome, "\n")] = 0;

    printf("Tipo do Item (ex:Arma, Cura, Munição, etc): ");
    fgets(mochila[num_itens].tipo, sizeof(mochila[num_itens].tipo), stdin);
    mochila[num_itens].tipo[strcspn(mochila[num_itens].tipo, "\n")] = 0;

    printf("Quantidade: ");
    //  Usamos scanf aqui porque esperamos apenas um inteiro
    if (scanf("%d", &mochila[num_itens].quantidade) != 1 || mochila[num_itens].quantidade < 0) {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        while (getchar() != '\n'); // Limpa o buffer
        return; 
    }

    num_itens++;
    printf("✅ Item '%s' adicionado com sucesso!\n", mochila[num_itens - 1].nome);
}

// 5. Implementação da funcionalidade de Listar Itens
void listar_itens() {
    if (num_itens == 0) {
        printf("\n🎒 Mochila Vazia. Nenhum item cadastrado.\n");
        return;
    }

    printf("\n====================================================================\n");
    printf("|%3s|%-25s|%-15s|%10s|\n", 
           "#", "Nome do Item", "Tipo", "Quantidade");
    printf("====================================================================\n");

    for (int i = 0; i < num_itens; i++) {
        printf("|%3d|%-25s|%-15s|%10d|\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("====================================================================\n");
}

// 6. Implementação da funcionalidade de Remover Item
void remover_item() {
    if (num_itens == 0) {
        printf("\n🎒 Mochila Vazia. Nada para remover.\n");
        return;
    }

    char nome_remover[50];
    printf("\n--- Remover Item ---\n");
    while (getchar() != '\n'); // Limpa o buffer

    printf("Digite o NOME do item a ser removido: ");
    fgets(nome_remover, sizeof(nome_remover), stdin);
    nome_remover[strcspn(nome_remover, "\n")] = 0;

    int indice_encontrado = -1;

    for (int i = 0; i < num_itens; i++) {
        if (strcmp(mochila[i].nome, nome_remover) == 0) {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado != -1) {
        // Deslocamento dos itens
        for (int i = indice_encontrado; i < num_itens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }

        num_itens--;
        printf("🗑️ Item '%s' removido com sucesso!\n", nome_remover);
    } else {
        printf("❌ Item '%s' não encontrado na mochila.\n", nome_remover);
    }
}

// 3. Função Principal e Menu Interativo
int main() {
    int opcao;

    do {
        // Exibe o Menu
        printf("===========================================\n");
        printf(" MOCHILA DE SOBREVIVENVIA - CÓDIGO DA ILHA ");
        printf("\n===========================================\n");
        printf("🎒 Itens na Mochila: %d/%d\n", num_itens, CAPACIDADE_MAXIMA);

        printf("\n1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        // Trata a entrada da opção
        if (scanf("%d", &opcao) != 1) {
            // Se o usuário digitar algo que não é um número
            printf("\nOpção inválida. Digite um número.\n");
            while (getchar() != '\n'); // Limpa o buffer para evitar loop
            opcao = -1; // Garante que o loop continue
            continue;
        }

        switch (opcao) {
            case 1:
                adicionar_item();
                break;
            case 2:
                remover_item();
                break;
            case 3:
                listar_itens();
                break;
            case 0:
                printf("\nSobrevivente pronto. Saindo do sistema...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}