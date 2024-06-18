#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 2000
#define NUM_CHECKINS 2000

struct Node {
    int key;
    struct Node* next;
};

// Tabela hash
struct Node* hash_table[TABLE_SIZE];

// Função de hash simples (método da divisão)
int hash_function(int key) {
    return key % TABLE_SIZE;
}

// Função para inserir uma chave na tabela hash
void insert_into_hash_table(int key) {
    int index = hash_function(key);
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    if (new_node == NULL) {
        printf("Erro ao alocar memória para novo nó.\n");
        exit(EXIT_FAILURE);
    }
    new_node->key = key;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

// Função para buscar uma chave na tabela hash
// Retorna o número de passos e colisões encontradas
void search_in_hash_table(int key, int* steps, int* collisions) {
    int index = hash_function(key);
    struct Node* current = hash_table[index];
    *steps = 0;
    *collisions = 0;
    
    while (current != NULL) {
        (*steps)++;
        if (current->key == key) {
            return;
        }
        (*collisions)++;
        current = current->next;
    }
}

// Função para liberar a memória alocada para a tabela hash
void free_hash_table() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        struct Node* current = hash_table[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
        hash_table[i] = NULL;
    }
}

// Função para gerar um número aleatório entre min e max
int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    // Inicializar a tabela hash com NULL
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hash_table[i] = NULL;
    }
    
    // Gerar dados aleatórios para o arquivo checkin.txt
    FILE *arquivo;
    arquivo = fopen("checkin.txt", "w");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    srand(time(NULL)); // Seed para a função rand() baseada no tempo atual

    for (int i = 0; i < NUM_CHECKINS; i++) {
        int numero = rand_int(1, 99999); // Gera número aleatório de 1 a 99999
        fprintf(arquivo, "%d\n", numero);
        insert_into_hash_table(numero); // Insere na tabela hash enquanto gera
    }

    fclose(arquivo);
    printf("Arquivo gerado com sucesso: checkin.txt\n\n");

    // Abrir arquivo checkin.txt para busca de chaves na tabela hash
    arquivo = fopen("checkin.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo checkin.txt.\n");
        return EXIT_FAILURE;
    }
    
    int total_steps = 0;
    int total_collisions = 0;
    int total_searches = 0;
    double total_search_time = 0.0;
    
    clock_t start, end;
    
    int key;
    while (fscanf(arquivo, "%d", &key) == 1) {
        int steps, collisions;
        
        start = clock();
        search_in_hash_table(key, &steps, &collisions);
        end = clock();
        
        total_steps += steps;
        total_collisions += collisions;
        total_searches++;
        total_search_time += ((double) (end - start)) / CLOCKS_PER_SEC;
        
        printf("Chave %d: Passos necessários: %d, Colisões: %d\n", key, steps, collisions);
    }
    
    fclose(arquivo);
    
    // Mostrar resultados totais
    printf("\nResultados totais:\n");
    printf("Quantidade de passos necessários (média): %.2f\n", (double) total_steps / total_searches);
    printf("Quantidade de colisões identificadas: %d\n", total_collisions);
    printf("Tempo necessário para encontrar (média): %.6f segundos\n", total_search_time / total_searches);
    
    // Liberar memória alocada para a tabela hash
    free_hash_table();
    
    return EXIT_SUCCESS;
}
