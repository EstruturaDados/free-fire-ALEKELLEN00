#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// === ESTRUTURAS E VARIÁVEIS GLOBAIS ===

// 1. Estrutura do Item ATUALIZADA
typedef struct
{
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade; // 1 (baixa) a 5 (alta)
} Item;

// 2. Enumeração para Critérios de Ordenação
typedef enum
{
    CRITERIO_NOME = 1,
    CRITERIO_TIPO = 2,
    CRITERIO_PRIORIDADE = 3
} CriterioOrdenacao;

// 3. Inventário e Variáveis de Estado
#define CAPACIDADE_MAXIMA 10
Item mochila[CAPACIDADE_MAXIMA];
int num_itens = 0;
bool is_ordenado_por_nome = false; // Flag para pré-requisito da Busca Binária
int num_comparacoes = 0;           // Contador para análise de desempenho (usado na ordenação)

// === FUNÇÕES AUXILIARES ===

//4. Converte uma string inteira para minúsculas (para busca e ordenação case-insensitive)
void to_lower_case(char *string)
{
    for (int i = 0; string[i]; i++)
    {
        string[i] = tolower((unsigned char)string[i]);
    }
}

//5. Limpa o buffer de entrada (necessário após scanf de números)
void limpar_buffer()
{
    int c;
    // Otimização: A sua limpeza de buffer original estava chamando getchar duas vezes, o que pode pular input.
    // Esta versão é mais segura.
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// === FUNCIONALIDADES DO INVENTÁRIO ===

void adicionar_item()
{
    if (num_itens >= CAPACIDADE_MAXIMA)
    {
        printf("\n🚫 Mochila Cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    printf("\n--- Coletando Novo Componente ---\n");
    limpar_buffer();

    printf("Nome do Item: ");
    fgets(mochila[num_itens].nome, sizeof(mochila[num_itens].nome), stdin);
    mochila[num_itens].nome[strcspn(mochila[num_itens].nome, "\n")] = 0;

    printf("Tipo do Item (ex: Arma, Cura, Munição, etc): ");
    fgets(mochila[num_itens].tipo, sizeof(mochila[num_itens].tipo), stdin);
    mochila[num_itens].tipo[strcspn(mochila[num_itens].tipo, "\n")] = 0;

    printf("Quantidade: ");
    if (scanf("%d", &mochila[num_itens].quantidade) != 1 || mochila[num_itens].quantidade < 0)
    {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    printf("Prioridade (1 a 5, sendo 5 a maior): ");
    if (scanf("%d", &mochila[num_itens].prioridade) != 1 || mochila[num_itens].prioridade < 1 || mochila[num_itens].prioridade > 5)
    {
        printf("⚠️ Prioridade inválida. Item não adicionado.\n");
        limpar_buffer();
        return;
    }

    num_itens++;
    //6. Resetar o estado de ordenação após a inserção
    is_ordenado_por_nome = false;
    printf("✅ Item '%s' adicionado. Estado de ordenação resetado.\n", mochila[num_itens - 1].nome);
}

// =======================================================================
// === FUNÇÃO LISTAR ITENS  (Formato Compacto) ===
// =======================================================================
void listar_itens()
{
    if (num_itens == 0)
    {
        printf("\n🎒 Mochila Vazia. Nenhum item cadastrado.\n");
        return;
    }

    //7. A linha separadora tem exatamente 71 caracteres
    printf("\n=======================================================================\n");

    // CABEÇALHO : Formato compacto |%...|
    printf("|%3s|%-20s|%-15s|%10s|%10s|\n",
           "#", "Nome do Item", "Tipo", "Quantidade", "Prioridade");

    printf("=======================================================================\n");

    for (int i = 0; i < num_itens; i++)
    {
        // DADOS : Formato compacto |%...|
        printf("|%3d|%-20s|%-15s|%10d|%10d|\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    // LINHA SEPARADORA FINAL (71 caracteres)
    printf("=======================================================================\n");
}
// =======================================================================

void remover_item()
{
    if (num_itens == 0)
    {
        printf("\n🎒 Mochila Vazia. Nada para remover.\n");
        return;
    }

    char nome_remover[50];
    printf("\n--- Remover Item ---\n");
    limpar_buffer();

    printf("Digite o NOME do item a ser removido: ");
    fgets(nome_remover, sizeof(nome_remover), stdin);
    nome_remover[strcspn(nome_remover, "\n")] = 0;
    to_lower_case(nome_remover);

    int indice_encontrado = -1;
    char temp_nome[50];

    //8. Busca Sequencial (simples e robusta para remoção)
    for (int i = 0; i < num_itens; i++)
    {
        strcpy(temp_nome, mochila[i].nome);
        to_lower_case(temp_nome);

        if (strcmp(temp_nome, nome_remover) == 0)
        {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado != -1)
    {
        // Deslocamento dos itens
        for (int i = indice_encontrado; i < num_itens - 1; i++)
        {
            mochila[i] = mochila[i + 1];
        }

        num_itens--;
        // Resetar o estado de ordenação após a remoção
        is_ordenado_por_nome = false;
        printf("🗑️ Item removido com sucesso! Estado de ordenação resetado.\n");
    }
    else
    {
        printf("❌ Item não encontrado na mochila.\n");
    }
}

// === FUNCIONALIDADE MESTRE: ORDENAÇÃO E BUSCA BINÁRIA ===

void ordenar_mochila(CriterioOrdenacao criterio)
{
    if (num_itens <= 1)
    {
        printf("\n✅ Mochila já ordenada (1 item ou vazia).\n");
        is_ordenado_por_nome = (criterio == CRITERIO_NOME);
        return;
    }

    num_comparacoes = 0; // Resetar o contador
    Item chave;
    int j;

    // Insertion Sort
    for (int i = 1; i < num_itens; i++)
    {
        chave = mochila[i];
        j = i - 1;

        while (j >= 0)
        {
            num_comparacoes++;

            int condicao_troca = 0;

            if (criterio == CRITERIO_NOME)
            {
                // Compara nomes (case-insensitive)
                char nome_j[50], nome_chave[50];
                strcpy(nome_j, mochila[j].nome);
                strcpy(nome_chave, chave.nome);
                to_lower_case(nome_j);
                to_lower_case(nome_chave);
                // strcmp > 0 significa que nome_j vem depois de nome_chave, precisa trocar
                condicao_troca = (strcmp(nome_j, nome_chave) > 0);
            }
            else if (criterio == CRITERIO_TIPO)
            {
                // Compara tipos
                condicao_troca = (strcmp(mochila[j].tipo, chave.tipo) > 0);
            }
            else if (criterio == CRITERIO_PRIORIDADE)
            {
                // Prioridade descendente (maior prioridade vem primeiro)
                condicao_troca = (mochila[j].prioridade < chave.prioridade);
            }

            if (condicao_troca)
            {
                mochila[j + 1] = mochila[j]; // Desloca
                j = j - 1;
            }
            else
            {
                break;
            }
        }
        mochila[j + 1] = chave; // Insere
    }

    //9. Atualiza o estado de ordenação
    is_ordenado_por_nome = (criterio == CRITERIO_NOME);

    printf("\n✅ Ordenação concluída com sucesso.\n");
    printf("📊 Critério: %d. Total de Comparações: %d\n", criterio, num_comparacoes);
}

void busca_binaria_por_nome()
{
    if (num_itens == 0)
    {
        printf("\n🎒 Mochila Vazia.\n");
        return;
    }

    // Validação de pré-requisito
    if (!is_ordenado_por_nome)
    {
        printf("\n🛑 ERRO MESTRE: A Busca Binária requer que a lista esteja **ordenada por NOME**.\n");
        printf("Por favor, ordene a mochila pelo critério Nome (opção 4 -> 1) primeiro.\n"); // ATUALIZADO
        return;
    }

    char nome_busca[50];
    printf("\n--- Busca Binária por Nome ---\n");
    limpar_buffer();

    printf("Digite o NOME do item para busca: ");
    fgets(nome_busca, sizeof(nome_busca), stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;
    to_lower_case(nome_busca);

    int esquerda = 0;
    int direita = num_itens - 1;
    int encontrado = 0;

    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;

        char nome_meio[50];
        strcpy(nome_meio, mochila[meio].nome);
        to_lower_case(nome_meio);

        int comparacao = strcmp(nome_meio, nome_busca);

        if (comparacao == 0)
        {
            // Item encontrado
            printf("\n✅ Item encontrado (Busca Binária):\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            encontrado = 1;
            break;
        }
        else if (comparacao < 0)
        {
            // Nome buscado é 'maior', procurar na metade direita
            esquerda = meio + 1;
        }
        else
        {
            // Nome buscado é 'menor', procurar na metade esquerda
            direita = meio - 1;
        }
    }

    if (!encontrado)
    {
        printf("\n❌ Item '%s' não encontrado.\n", nome_busca);
    }
}

// === MENUS ===

void menu_ordenacao()
{
    int escolha;
    printf("\n--- Menu de Ordenação ---\n");
    printf("1. Por Nome\n");
    printf("2. Por Tipo\n");
    printf("3. Por Prioridade (Maior para Menor)\n");
    printf("Escolha o critério: ");

    if (scanf("%d", &escolha) != 1)
    {
        printf("\nOpção inválida. Digite um número.\n");
        limpar_buffer();
        return;
    }

    if (escolha >= 1 && escolha <= 3)
    {
        ordenar_mochila((CriterioOrdenacao)escolha);
    }
    else
    {
        printf("\nOpção de critério inválida.\n");
    }
}

// === FUNÇÃO PRINCIPAL ===

int main()
{
    int opcao;

    do
    {
        printf("\n==============================================");
        printf("\n PLANO DE FUGA - CÓDIGO DA ILHA (NÍVEL MESTRE) \n");
        printf("================================================\n");

        printf("🎒 Itens na Mochila: %d/%d\n", num_itens, CAPACIDADE_MAXIMA);

        printf("Status de Ordenação por Nome: %s\n", is_ordenado_por_nome ? "✅ ORDENADO" : "❌ NÃO ORDENADO\n");

        printf("1. Adicionar Componente \n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario) \n");
        printf("4. Organizar Mochila (Ordenar Componentes) \n");
        printf("5. Busca Binária por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("------------------------------------------------\n");

        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf("\nOpção inválida. Digite um número.\n");
            limpar_buffer();
            opcao = -1;
            continue;
        }

        switch (opcao)
        {
        case 1:
            adicionar_item();
            break;
        case 2:
            remover_item();
            break;
        case 3:
            listar_itens();
            break;
        case 4: // NOVO CASE 4 (Antigo 5)
            menu_ordenacao();
            break;
        case 5: // NOVO CASE 5 (Antigo 6)
            busca_binaria_por_nome();
            break;
        case 0:
            printf("\nSobrevivente Mestre pronto. Sistema Finalizado.\n");
            break;
        default:
            printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}