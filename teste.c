#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // obs --- usar biblioteca conio.h no windows
#include <unistd.h>  // possibilita as funções para criar getch

#define MAX_NAME_LENGTH 49
// apenas para linux/macOs

int getch() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

typedef struct node {
    char name[50];
    struct node* left;
    struct node* right;
} Node;

void insert(Node** root, char name[]) {
    if (*root == NULL) {
        *root = (Node*) malloc(sizeof(Node));
        strcpy((*root)->name, name);
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        if (strcmp(name, (*root)->name) < 0) {
            insert(&((*root)->left), name);
        } else {
            insert(&((*root)->right), name);
        }
    }
}


void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%s ", root->name);
        inorderTraversal(root->right);
    }
}

Node* search(Node* root, char name[]) {
    if (root == NULL) {
        return NULL;
    } else if (strcmp(name, root->name) == 0) {
        return root;
    } else if (strcmp(name, root->name) < 0) {
        return search(root->left, name);
    } else {
        return search(root->right, name);
    }
}

void removeNode(Node** root, char name[]) {
    if (*root == NULL) {
        return;
    }
    if (strcmp(name, (*root)->name) < 0) {
        removeNode(&((*root)->left), name);
    } else if (strcmp(name, (*root)->name) > 0) {
        removeNode(&((*root)->right), name);
    } else {
        if ((*root)->left == NULL && (*root)->right == NULL) {
            free(*root);
            *root = NULL;
        } else if ((*root)->left == NULL) {
            Node* temp = *root;
            *root = (*root)->right;
            free(temp);
        } else if ((*root)->right == NULL) {
            Node* temp = *root;
            *root = (*root)->left;
            free(temp);
        } else {
            Node* temp = (*root)->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy((*root)->name, temp->name);
            removeNode(&((*root)->right), temp->name);
        }
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Node* root = NULL;
    char name[50];
     //nomes aleatórios
    insert(&root, "Raul");
    insert(&root, "Leandro");
    insert(&root, "Marinho");
    insert(&root, "Mozer");
    insert(&root, "Júnior");
    insert(&root, "Júnior");
    insert(&root, "Andrade");
    insert(&root, "Adílio");
    insert(&root, "Zico");
    insert(&root, "Lico");
    insert(&root, "Tita");
    insert(&root, "Nunes");

    int opcao;
do {
    printf("\nOpções:\n");
    printf("1 - Buscar nome\n");
    printf("2 - Remover nome\n");
    printf("3 - Inserir nome\n");
    printf("4 - Imprimir todos os nomes\n");
    printf("0 - Sair\n");
    printf("Digite uma opção: ");
    scanf("%d", &opcao);
    clearInputBuffer();
    switch (opcao) {
        case 1:
    printf("Digite o nome a ser buscado: ");
    fflush(stdin);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove o caractere de nova linha lido pelo fgets
    Node* result = search(root, name);
    if (result == NULL) {
        printf("Nome não encontrado.\n");
    } else {
        printf("Nome encontrado: %s\n", result->name);
    }
    break;
case 2:
    printf("Digite o nome a ser removido: ");
    fflush(stdin);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove o caractere de nova linha lido pelo fgets
    removeNode(&root, name);
    printf("Nome removido.\n");
    break;
case 3:
    printf("Digite o nome a ser adicionado: ");
    fflush(stdin);
    fgets(name, sizeof(name), stdin);
    int len = strlen(name);
    if (len > 0 && name[len-1] == '\n') {
        name[len-1] = '\0'; // Remove o caractere de nova linha lido pelo fgets
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {} // Descarta o restante da linha que não foi lido pelo fgets
    }
    if (strlen(name) >= MAX_NAME_LENGTH) {
        printf("Nome muito grande, tente novamente.\n");
    } else {
        insert(&root, name);
        printf("Nome adicionado.\n");
    }
    break;
case 4:
    printf("Todos os nomes: ");
    inorderTraversal(root);
    printf("\n");
    break;
case 0:
    printf("Pressione qualquer tecla para sair.\n");
    getch();
    break;

        default:
            printf("Opção inválida.\n");
            while (getchar() != '\n'); // limpar o buffer do teclado
            break;
    }
} while (opcao != 0);

return 0;
}