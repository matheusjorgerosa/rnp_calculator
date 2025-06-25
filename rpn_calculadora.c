#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// --- Definição do TAD Pilha ---
#define MAX 100 // Tamanho máximo da pilha

typedef struct {
    double items[MAX];
    int top;
} Pilha;

// Inicializa a pilha
void inicializaPilha(Pilha *p) {
    p->top = -1; // Indica pilha vazia
}

// Verifica se a pilha está vazia
int estaVazia(Pilha *p) {
    return p->top == -1;
}

// Verifica se a pilha está cheia
int estaCheia(Pilha *p) {
    return p->top == MAX - 1;
}

// Empilha um valor
void push(Pilha *p, double valor) {
    if (estaCheia(p)) {
        fprintf(stderr, "Pilha cheia!\n");
        exit(-1);
    }
    p->items[++(p->top)] = valor;
}

// Desempilha um valor
double pop(Pilha *p) {
    if (estaVazia(p)) {
        fprintf(stderr, "Pilha vazia! Expressão inválida ou insuficiência de operandos.\n");
        exit(-1);
    }
    return p->items[(p->top)--];
}

// --- Funções da Calculadora RPN ---

// Verifica se um token é um operador
int ehOperador(const char *token) {
    return (strcmp(token, "+") == 0 ||
            strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0);
}

// Função principal de avaliação RPN
double calculaRPN(const char *expressao) {
    Pilha pilha;
    inicializaPilha(&pilha);

    char *expressao_copia = strdup(expressao); // Cria uma cópia mutável da string
    if (expressao_copia == NULL) {
        fprintf(stderr, "Falha na alocação de memória.\n");
        exit(-1);
    }

    char *token = strtok(expressao_copia, " "); // Divide a string por espaços

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // Se for um número (incluindo números negativos)
            push(&pilha, atof(token)); // Converte string para double
        } else if (ehOperador(token)) {
            // Se for um operador
            if (estaVazia(&pilha)) {
                fprintf(stderr, "Operador '%s' sem operandos suficientes. Expressão inválida.\n", token);
                free(expressao_copia);
                exit(-1);
            }
            double b = pop(&pilha);
            if (estaVazia(&pilha)) {
                fprintf(stderr, "Operador '%s' sem operandos suficientes. Expressão inválida.\n", token);
                free(expressao_copia);
                exit(-1);
            }
            double a = pop(&pilha);
            double resultado;

            if (strcmp(token, "+") == 0) {
                resultado = a + b;
            } else if (strcmp(token, "-") == 0) {
                resultado = a - b;
            } else if (strcmp(token, "*") == 0) {
                resultado = a * b;
            } else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    fprintf(stderr, "Divisão por zero!\n");
                    free(expressao_copia);
                    exit(-1);
                }
                resultado = a / b;
            }
            push(&pilha, resultado);
        } else {
            fprintf(stderr, "Token inválido encontrado: '%s'\n", token);
            free(expressao_copia);
            exit(-1);
        }
        token = strtok(NULL, " "); // Próximo token
    }

    free(expressao_copia); // Libera a memória alocada para a cópia

    if (pilha.top != 0) { // Deve haver apenas um item na pilha no final
        fprintf(stderr, "Expressão mal formada. Sobraram operandos na pilha.\n");
        exit(-1);
    }

    return pop(&pilha); // O resultado final está no topo da pilha
}

// --- Função Principal (main) ---
int main() {
    char expressao[256]; // Buffer para a expressão RPN

    printf("Calculadora RPN\n");
    printf("Operacoes: +, -, *, /\n");
    printf("Digite a expressao RPN separada por espacos: ");

    // Lê a linha completa da entrada padrão
    if (fgets(expressao, sizeof(expressao), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler a expressao.\n");
        return -1;
    }

    // Remove o caractere de nova linha, se existir
    expressao[strcspn(expressao, "\n")] = 0;

    double resultado = calculaRPN(expressao);
    printf("Resultado: %.2f\n", resultado);

    return 0;
}