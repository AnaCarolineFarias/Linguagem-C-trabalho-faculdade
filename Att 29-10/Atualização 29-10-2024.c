#include <stdio.h>  // Biblioteca de entrada e saída padrão.
#include <stdlib.h> // Biblioteca para alocação de memória.
#include <string.h> // Biblioteca para manipulação de strings.
#include <locale.h> // Biblioteca que permite a acentuação no código.
#include <time.h>   // Biblioteca para manipulação de datas.

// Definindo a estrutura da mensagem de boas-vindas da Biblioteca
char MENSAGEM[] = "Bem Vindo(a) á Biblioteca Virtual!";
int largura; // Definindo a variável largura
int altura = 7; // Altura do quadrado (inclui a linha de cima e de baixo); (um número ímpar facilita a centralização vertical)

// Definindo a estrutura do usuário
typedef struct {
    char usuario[50];
    char senha[50];
    int Admin; // 1 para admin, 0 para usuário comum
} Usuario;

// Definindo a estrutura do livro
typedef struct {
    char titulo[100];
    char autor[100];
    int ano;
} Livro;

// Definindo a estrutura para armazenar informações de empréstimos
typedef struct {
    char titulo[100];
    char usuario[50];
    time_t dataEmprestimo;
} Emprestimo;

// Variáveis e constantes globais
#define PRAZO_DEVOLUCAO 7          // Prazo para devolução do livro em dias
#define MULTA_POR_DIA_ATRASO 1.50  // Multa em reais por dia de atraso

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
int verificarLogin(Usuario usuarios[], int qtdUsuarios, char *usuario, char *senha, int *Admin);
void excluirUsuario(Usuario usuarios[], int *qtdUsuarios);
void excluirLivro(Livro livros[], int *quantidade, char *titulo);
void solicitarEmprestimo(Emprestimo emprestimos[], Livro livros[], int *qtdEmprestimos, int quantidadeLivros, char *usuario);
void verificarMultas(Emprestimo emprestimos[], int qtdEmprestimos);

// Função para cadastrar novos usuários
void cadastrarUsuario(Usuario usuarios[], int *qtdUsuarios) {
    if (*qtdUsuarios >= 100) {

        system("cls"); //Limpar o terminal

        printf("Limite de usuários atingido!\n");
        return;
    }

    system("cls"); //Limpar o terminal

    printf("\nCadastro de novo usuário\n");
    printf("Nome do usuário: ");
    fgets(usuarios[*qtdUsuarios].usuario, sizeof(usuarios[*qtdUsuarios].usuario), stdin);
    usuarios[*qtdUsuarios].usuario[strcspn(usuarios[*qtdUsuarios].usuario, "\n")] = '\0';  // Remover a nova linha

    printf("Senha: ");
    fgets(usuarios[*qtdUsuarios].senha, sizeof(usuarios[*qtdUsuarios].senha), stdin);
    usuarios[*qtdUsuarios].senha[strcspn(usuarios[*qtdUsuarios].senha, "\n")] = '\0';  // Remover a nova linha

    printf("Você é um administrador? (1 para sim, 0 para não): ");
    scanf("%d", &usuarios[*qtdUsuarios].Admin);
    clearInputBuffer();  // Limpar o buffer do teclado

    system("cls"); //Limpar o terminal

    (*qtdUsuarios)++;
    printf("Usuário cadastrado com sucesso!\n");
}

// Função para verificar o login
int verificarLogin(Usuario usuarios[], int qtdUsuarios, char *Usuario, char *senha, int *Admin) {
    for (int i = 0; i < qtdUsuarios; i++) {
        if (strcmp(usuarios[i].usuario, Usuario) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            *Admin = usuarios[i].Admin;  // Verifica se o usuário é admin
            return 1;  // Login bem-sucedido
        }
    }
    return 0;  // Login falhou
}
void excluirUsuario(Usuario usuarios[], int *qtdUsuarios) {
    char usuarioExcluir[50];
    system("cls"); //Limpar o terminal

    printf("Digite o nome do usuário a ser excluído: ");
    fgets(usuarioExcluir, sizeof(usuarioExcluir), stdin);
    usuarioExcluir[strcspn(usuarioExcluir, "\n")] = '\0';

    for (int i = 0; i < *qtdUsuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuarioExcluir) == 0) {
            for (int j = i; j < *qtdUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }
            (*qtdUsuarios)--;
            system("cls"); //Limpar o terminal

            printf("Usuário %s excluído com sucesso!\n", usuarioExcluir);
            return;
        }
    }
    system("cls");

    printf("Usuário %s não encontrado.\n", usuarioExcluir);
}
// Função para excluir livro
void excluirLivro(Livro livros[], int *quantidade, char *titulo) {
    for (int i = 0; i < *quantidade; i++) {
        if (strcmp(livros[i].titulo, titulo) == 0) {
            for (int j = i; j < *quantidade - 1; j++) {
                livros[j] = livros[j + 1]; // Desloca os livros para a esquerda
            }

             system("cls"); // Limpar o terminal

            (*quantidade)--; // Decrementa a quantidade de livros
            printf("Livro '%s' excluído com sucesso!\n", titulo);
            return;
        }
    }
     system("cls"); // Limpar o terminal

    printf("Livro '%s' não encontrado.\n", titulo);
}

// Função para solicitar empréstimo de um livro
void solicitarEmprestimo(Emprestimo emprestimos[], Livro livros[], int *qtdEmprestimos, int quantidadeLivros, char *usuario) {
    if (*qtdEmprestimos >= 100) {
        system("cls"); //Limpar o terminal

        printf("Limite de empréstimos atingido!\n");
        return;
    }

    system("cls"); //Limpar o terminal

    printf("\nSolicitação de Empréstimo de Livro\n");
    printf("Digite o título do livro que deseja emprestar: ");
    char titulo[100];
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    // Verifica se o livro está disponível
    int livroEncontrado = 0;
    for (int i = 0; i < quantidadeLivros; i++) {
        if (strcmp(livros[i].titulo, titulo) == 0) {
            livroEncontrado = 1;
            break;
        }
    }

    if (!livroEncontrado) {
        system("cls");//Limpar o terminal

        printf("Livro não encontrado.\n");
        return;
    }

    // ARMAZENA O EMPRÉSTIMO
    strcpy(emprestimos[*qtdEmprestimos].titulo, titulo);
    strcpy(emprestimos[*qtdEmprestimos].usuario, usuario);
    emprestimos[*qtdEmprestimos].dataEmprestimo = time(NULL);
    (*qtdEmprestimos)++;

    system("cls"); //Limpar o terminal

    printf("Empréstimo realizado com sucesso!\n");
}

// FUNÇÂO PARA VERIFICAR MULTAS
void verificarMultas(Emprestimo emprestimos[], int qtdEmprestimos) {
    system("cls"); //Limpar o terminal

    printf("\nVerificação de Multas\n");

    time_t agora = time(NULL);
    for (int i = 0; i < qtdEmprestimos; i++) {
        float diferencaDias = difftime(agora, emprestimos[i].dataEmprestimo) / (60 * 60 * 24);
        if (diferencaDias > PRAZO_DEVOLUCAO) {
            float diasAtraso = diferencaDias - PRAZO_DEVOLUCAO;
            float multa = diasAtraso * MULTA_POR_DIA_ATRASO;
            system("cls"); //Limpar o terminal

            printf("Usuário: %s - Livro: %s - Dias de Atraso: %.0f - Multa: R$%.2f\n",
                   emprestimos[i].usuario, emprestimos[i].titulo, diasAtraso, multa);
        } else {
            system("cls"); //Limpar o terminal

            printf("Não há multas, seus livros ainda estão dentro do prazo de devolução.\n");
        }
    }
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

    // Variáveis para verificar se o Login e Senha correspondem com os cadastrados
    char usuarioInput[50];
    char senhaInput[50];
    int escolha, Admin;
    Emprestimo emprestimos[100];
    int qtdEmprestimos = 0;

    printf("\n         SISTEMA DE LOGIN            \n");
    printf("---------------------------------------\n");
    printf("| 1. Já possui cadastro.              |\n");
    printf("| 2. Novo cadastro                    |\n");
    printf("| Escolha uma opção, de 1 a 2:        |\n");
    printf("---------------------------------------\n");
    scanf("%d", &escolha);
    clearInputBuffer();  // Limpar o buffer do teclado

    if (escolha == 2) {
        cadastrarUsuario(usuarios, &qtdUsuarios);
    }

    // Tentativa de login
    system("cls"); // Limpar o terminal

    printf("Nome do usuário: ");
    fgets(usuarioInput, sizeof(usuarioInput), stdin); // Corrigido
    usuarioInput[strcspn(usuarioInput, "\n")] = '\0';  // Remover a nova linha

    printf("Senha: ");
    fgets(senhaInput, sizeof(senhaInput), stdin);
    senhaInput[strcspn(senhaInput, "\n")] = '\0';  // Remover a nova linha

    if (verificarLogin(usuarios, qtdUsuarios, usuarioInput, senhaInput, &Admin)) {
        system("cls"); // Limpar o terminal

        printf("Login bem-sucedido!\n");
    } else {
        system("cls"); //Limpar o terminal

        printf("Nome de usuário ou senha incorretos.\n");
    return 1;
}

    int quantidade = 0;
    Livro livros[100];  // Array para armazenar até 100 livros

    // Looping de escolhas da biblioteca
    while (1) {
        printf("\nSISTEMA DE GERENCIAMENTO DE BIBLIOTECA\n");
        printf("---------------------------------------\n");
        printf("| 1. Adicionar Livro                  |\n");
        printf("| 2. Visualizar Todos os Livros       |\n");
        printf("| 3. Procurar Livro pelo Título       |\n");
        printf("| 4. Excluir Usuário                  |\n");
        printf("| 5. Excluir Livros                   |\n");
        printf("| 6. Solicitar Empréstimo de Livro    |\n");
        printf("| 7. Verificar Multas                 |\n");
        printf("| 8. Sair da aplicação                |\n");
        printf("| Escolha uma opção, de 1 a 8:        |\n");
        printf("---------------------------------------\n");
        scanf("%d", &escolha);
        clearInputBuffer();  // Limpar o buffer do teclado

        switch (escolha) {
            case 1:
                if (Admin) {
                    adicionarLivro(livros, &quantidade);
                } else {
                    system("cls"); //Limpar o terminal

                    printf("[ERRO]Acesso negado! Apenas administradores podem adicionar livros.\n");
                }
                break;
            case 2:
                visualizarLivros(livros, quantidade);
                break;
            case 3: {
                system("cls"); //Limpar o terminal

                char titulo[100];
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';  // Remover a nova linha
                procurarLivro(livros, quantidade, titulo);
                break;
            }
            case 4:
                if (Admin) {
                    excluirUsuario(usuarios, &qtdUsuarios);
                } else {
                    system("cls"); //Limpar o terminal

                    printf("[ERRO]Acesso negado! Apenas administradores podem excluir usuários.\n");
                }
                break;
            case 5: {
                char titulo[100];
                printf("\nDigite o título do livro que deseja excluir: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';  // Remover a nova linha
                excluirLivro(livros, &quantidade, titulo);
            }
                break;
            case 6:
                solicitarEmprestimo(emprestimos, livros, &qtdEmprestimos, quantidade, usuarioInput);
                break;
            case 7:
                verificarMultas(emprestimos, qtdEmprestimos);
                break;
            case 8:
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

        system("cls"); //Limpar o terminal

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
    system("cls"); //Limpar o terminal

    printf("Livro adicionado com sucesso!\n");
}

// Função para visualizar todos os livros
void visualizarLivros(Livro livros[], int quantidade) {
    if (quantidade == 0) {

        system("cls"); //Limpar o terminal

        printf("Nenhum livro cadastrado no sistema.\n");
        return;
    }

    system("cls"); //Limpar o terminal

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
    system("cls"); //Limpar o terminal

    printf("Livro não encontrado.\n");
}
