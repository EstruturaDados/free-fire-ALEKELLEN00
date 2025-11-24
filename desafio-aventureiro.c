#include <stdio.h>
#include <string.h>

// 1. Estrutura do Item
typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
} Item;

// 2. Inventário e Variáveis Globais
#define CAPACIDADE_MAXIMA 10
Item mochila[CAPACIDADE_MAXIMA];
int num_itens = 0;

// Funções Auxiliares (Mesmas do Nível Novato)
void adicionar_item() {
    if (num_itens >= CAPACIDADE_MAXIMA) {
        printf("\n🚫 Mochila Cheia! Não é possível adicionar mais itens.\n");
        return;
    }
    printf("\n--- Adicionar Novo Item ---\n");
    while (getchar() != '\n'); 

    printf("Nome do Item: ");
    fgets(mochila[num_itens].nome, sizeof(mochila[num_itens].nome), stdin);
    mochila[num_itens].nome[strcspn(mochila[num_itens].nome, "\n")] = 0;

    printf("Tipo do Item (ex: Cura, Munição): ");
    fgets(mochila[num_itens].tipo, sizeof(mochila[num_itens].tipo), stdin);
    mochila[num_itens].tipo[strcspn(mochila[num_itens].tipo, "\n")] = 0;

    printf("Quantidade: ");
    if (scanf("%d", &mochila[num_itens].quantidade) != 1 || mochila[num_itens].quantidade < 0) {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        while (getchar() != '\n'); 
        return; 
    }

    num_itens++;
    printf("✅ Item '%s' adicionado com sucesso!\n", mochila[num_itens - 1].nome);
}

void listar_itens() {
    if (num_itens == 0) {
        printf("\n🎒 Mochila Vazia. Nenhum item cadastrado.\n");
        return;
    }

    printf("\n=======================================================\n");
    printf("| # | Nome do Item | Tipo | Quantidade |\n");
    printf("=======================================================\n");

    for (int i = 0; i < num_itens; i++) {
        printf("| %-1d | %-20s | %-15s | %-10d |\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("=======================================================\n");
}

void remover_item() {
    if (num_itens == 0) {
        printf("\n🎒 Mochila Vazia. Nada para remover.\n");
        return;
    }

    char nome_remover[50];
    printf("\n--- Remover Item ---\n");
    while (getchar() != '\n'); 

    printf("Digite o NOME do item a ser removido: ");
    fgets(nome_remover, sizeof(nome_remover), stdin);
    nome_remover[strcspn(nome_remover, "\n")] = 0;

    int indice_encontrado = -1;

    // Busca Sequencial para remoção
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(mochila[i].nome, nome_remover) == 0) {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado != -1) {
        // Deslocamento dos itens (Lógica de remoção)
        for (int i = indice_encontrado; i < num_itens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }

        num_itens--;
        printf("🗑️ Item '%s' removido com sucesso!\n", nome_remover);
    } else {
        printf("❌ Item '%s' não encontrado na mochila.\n", nome_remover);
    }
}

// 4. Implementação da nova funcionalidade: Buscar Item
void buscar_item() {
    if (num_itens == 0) {
        printf("\n🎒 Mochila Vazia. Nenhum item para buscar.\n");
        return;
    }

    char nome_busca[50];
    int item_encontrado = 0; // <<-- A FLAG
    
    printf("\n--- Buscar Item ---\n");
    while (getchar() != '\n'); 

    printf("Digite o NOME do item que deseja buscar: ");
    fgets(nome_busca, sizeof(nome_busca), stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    // Busca Sequencial
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            printf("\n✅ Item encontrado:\n");
            printf("------------------------------------------\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("------------------------------------------\n");
            
            item_encontrado = 1; 
            break; 
        }
    }

    if (item_encontrado == 0) {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nome_busca);
    }
}


// 3. Função Principal e Menu Interativo Atualizado
int main() {
    int opcao;

    do {
        printf("\n\n=== Desafio Código da Ilha: Nível Aventureiro ===\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item por Nome\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpção inválida. Digite um número.\n");
            while (getchar() != '\n'); 
            opcao = -1; 
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
            case 4:
                buscar_item();
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