#include <stdio.h>  // Biblioteca de entrada e saída padrão.
#include <stdlib.h> // Biblioteca para alocação de memória.
#include <string.h> // Biblioteca para manipulação de strings.
#include <locale.h> // Biblioteca que permite a acentuação no código.

// Definindo a estrutura da mensagem de boas-vindas da Biblioteca
char MENSAGEM[] = "Bem Vindo(a) á Biblioteca Online!";
int largura; // Definindo a variável largura
int altura = 7; // Altura do quadrado (inclui a linha de cima e de baixo); (um número ímpar facilita a centralização vertical)

// Definindo a estrutura do usuário
typedef struct {
    char username[50];
    char password[50];
} Usuario;

// Definindo a estrutura do livro
typedef struct {
    char titulo[100];
    char autor[100];
    int ano;
} Livro;

// Função que limpa o buffer do teclado
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Funções para o sistema de biblioteca
void adicionarLivro(Livro livros[], int *quantidade);
void visualizarLivros(Livro livros[], int quantidade);
void procurarLivro(Livro livros[], int quantidade, char *titulo);
void cadastrarUsuario(Usuario usuarios[], int *qtdUsuarios);
int verificarLogin(Usuario usuarios[], int qtdUsuarios, char *username, char *password);

// Função para cadastrar novos usuários
void cadastrarUsuario(Usuario usuarios[], int *qtdUsuarios) {
    if (*qtdUsuarios >= 100) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    printf("Cadastro de novo usuário\n");
    printf("Nome do usuário: ");
    fgets(usuarios[*qtdUsuarios].username, sizeof(usuarios[*qtdUsuarios].username), stdin);
    usuarios[*qtdUsuarios].username[strcspn(usuarios[*qtdUsuarios].username, "\n")] = '\0';  // Remover a nova linha

    printf("Senha: ");
    fgets(usuarios[*qtdUsuarios].password, sizeof(usuarios[*qtdUsuarios].password), stdin);
    usuarios[*qtdUsuarios].password[strcspn(usuarios[*qtdUsuarios].password, "\n")] = '\0';  // Remover a nova linha

    (*qtdUsuarios)++;
    printf("Usuário cadastrado com sucesso!\n");
}

// Função para verificar o login
int verificarLogin(Usuario usuarios[], int qtdUsuarios, char *username, char *password) {
    for (int i = 0; i < qtdUsuarios; i++) {
        if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) {
            return 1;  // Login bem-sucedido
        }
    }
    return 0;  // Login falhou
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Permite acentuação no código

    // 2 Espaços de cada lado da mensagem
    largura = strlen(MENSAGEM) + 4;

    Usuario usuarios[100];  // Array para armazenar até 100 usuários
    int qtdUsuarios = 0;    // Contador de usuários cadastrados

    // Desenha a parte superior do quadrado
    for (int i = 0; i < largura; i++) {
        printf("*");
    }
    printf("\n");

    // Desenha o meio do quadrado com a mensagem
    for (int i = 0; i < altura - 2; i++) {
        printf("*"); // Lado esquerdo do quadrado

        if (i == (altura - 2) / 2) {
            int espacos_esquerda = (largura - strlen(MENSAGEM) - 2) / 2; // Espaços antes da mensagem
            int espacos_direita = largura - strlen(MENSAGEM) - espacos_esquerda - 2; // Espaços depois da mensagem
            for (int j = 0; j < espacos_esquerda; j++) {
                printf(" ");
            }
            printf("%s", MENSAGEM); // Mostra a mensagem centralizada
            for (int j = 0; j < espacos_direita; j++) {
                printf(" ");
            }
        } else {
            for (int j = 0; j < largura - 2; j++) {
                printf(" "); // Espaços vazios
            }
        }
        printf("*\n"); // Lado direito do quadrado
    }

    // Desenha a parte inferior do quadrado
    for (int i = 0; i < largura; i++) {
        printf("*");
    }
    printf("\n");

    // Continua com o resto do código da aplicação de biblioteca
    char usernameInput[50];
    char passwordInput[50];
    int escolha;

    printf("\nSistema de login para bibliotecários\n");
    printf("----------------\n");

    printf("Você já possui cadastro no sistema?\n");
    printf("1. Sim\n");
    printf("2. Não, efetuar cadastro\n");
    printf("Escolha uma opção, de 1 a 2: ");
    scanf("%d", &escolha);
    clearInputBuffer();  // Limpar o buffer do teclado

    if (escolha == 2) {
        cadastrarUsuario(usuarios, &qtdUsuarios);
    }

    // Tentativa de login
    printf("Nome do usuário: ");
    fgets(usernameInput, sizeof(usernameInput), stdin);
    usernameInput[strcspn(usernameInput, "\n")] = '\0';  // Remover a nova linha

    printf("Senha: ");
    fgets(passwordInput, sizeof(passwordInput), stdin);
    passwordInput[strcspn(passwordInput, "\n")] = '\0';  // Remover a nova linha

    if (verificarLogin(usuarios, qtdUsuarios, usernameInput, passwordInput)) {
        printf("Login bem-sucedido!\n");
    } else {
        printf("Nome de usuário ou senha incorretos.\n");
        return 1;
    }

    // Para limpar a tela
    system("cls");

    int quantidade = 0;
    Livro livros[100];  // Array para armazenar até 100 livros

    // Looping de escolhas da biblioteca
    while (1) {
        printf("\nSistema de Gerenciamento de Biblioteca\n");
        printf("----------------\n");

        printf("1. Adicionar Livro\n");
        printf("2. Visualizar Todos os Livros\n");
        printf("3. Procurar Livro pelo Título\n");
        printf("4. Sair\n");
        printf("Escolha uma opção, de 1 a 4: ");
        scanf("%d", &escolha);
        clearInputBuffer();  // Limpar o buffer do teclado

        switch (escolha) {
            case 1:
                adicionarLivro(livros, &quantidade);
                break;
            case 2:
                visualizarLivros(livros, quantidade);
                break;
            case 3: {
                char titulo[100];
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';  // Remover a nova linha
                procurarLivro(livros, quantidade, titulo);
                break;
            }
            case 4:
                exit(0);
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}

// Função para adicionar livros ao sistema
void adicionarLivro(Livro livros[], int *quantidade) {
    if (*quantidade >= 100) {
        printf("Limite de livros atingido!\n");
        return;
    }

    printf("Digite o título do livro: ");
    fgets(livros[*quantidade].titulo, sizeof(livros[*quantidade].titulo), stdin);
    livros[*quantidade].titulo[strcspn(livros[*quantidade].titulo, "\n")] = '\0';  // Remover a nova linha

    printf("Digite o autor do livro: ");
    fgets(livros[*quantidade].autor, sizeof(livros[*quantidade].autor), stdin);
    livros[*quantidade].autor[strcspn(livros[*quantidade].autor, "\n")] = '\0';  // Remover a nova linha

    printf("Digite o ano de publicação: ");
    scanf("%d", &livros[*quantidade].ano);
    clearInputBuffer();  // Limpar o buffer do teclado

    (*quantidade)++;
    printf("Livro adicionado com sucesso!\n");
}

// Função para visualizar todos os livros
void visualizarLivros(Livro livros[], int quantidade) {
    if (quantidade == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    printf("\nLista de Livros:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Título: %s\n", livros[i].titulo);
        printf("Autor: %s\n", livros[i].autor);
        printf("Ano de Publicação: %d\n", livros[i].ano);
        printf("-------------------------\n");
    }
}

// Função para procurar um livro pelo título
void procurarLivro(Livro livros[], int quantidade, char *titulo) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(livros[i].titulo, titulo) == 0) {
            printf("Livro encontrado!\n");
            printf("Título: %s\n", livros[i].titulo);
            printf("Autor: %s\n", livros[i].autor);
            printf("Ano de Publicação: %d\n", livros[i].ano);
            return;
        }
    }
    printf("Livro não encontrado.\n");
}
