#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define maxClientes 10
#define maxQuartos 10
#define maxReservas 10

// Estruturas
typedef struct {
    int id;
    char nome[100];
    char email[100];
} Cliente;

typedef struct {
    int id;
    char tipo[50];
    float preco;
    int capacidade;
} Quarto;

typedef struct {
    int id;
    int idCliente;
    int idQuarto;
    char dataCheckin[11];
    char dataCheckout[11];
} Reserva;

// Dados em mem�ria
Cliente clientes[maxClientes];
Quarto quartos[maxQuartos];
Reserva reservas[maxReservas];
int totalClientes = 0, totalQuartos = 0, totalReservas = 0;

// Fun��o para verificar conflito de datas
int conflitoDeDatas(char *checkin1, char *checkout1, char *checkin2, char *checkout2) {
    return !(strcmp(checkout1, checkin2) < 0 || strcmp(checkout2, checkin1) < 0);
}

// Fun��o para verificar se o quarto est� dispon�vel em um intervalo de datas
int verificarDisponibilidade(int idQuarto, char *dataCheckin, char *dataCheckout) {
    for (int i = 0; i < totalReservas; i++) {
        if (reservas[i].idQuarto == idQuarto) {
            if (conflitoDeDatas(reservas[i].dataCheckin, reservas[i].dataCheckout, dataCheckin, dataCheckout)) {
                return 0; // Conflito encontrado, quarto indispon�vel
            }
        }
    }
    return 1; // Quarto dispon�vel
}

// Fun��es para Clientes
void cadastrarCliente() {
    if (totalClientes >= maxClientes) {
        printf("Limite de clientes atingido.\n");
        return;
    }
    Cliente novo;
    novo.id = totalClientes + 1;
    fflush(stdin);
    printf("Digite o nome do cliente: ");
    gets(novo.nome);
    fflush(stdin);
    printf("Digite o email do cliente: ");
    gets(novo.email);
    clientes[totalClientes++] = novo;
    printf(GREEN "Cliente cadastrado com sucesso! ID: %d\n" RESET, novo.id);
}

void listarClientes() {
    printf(BLUE "\n--- Lista de Clientes ---\n" RESET);
    for (int i = 0; i < totalClientes; i++) {
        printf("ID: %d | Nome: %s | Email: %s\n", clientes[i].id, clientes[i].nome, clientes[i].email);
    }
}

void editarCliente() {
    int id;
    printf("Digite o ID do cliente que deseja editar: ");
    scanf("%d", &id);
    if (id <= 0 || id > totalClientes) {
        printf(RED "Cliente n�o encontrado.\n" RESET);
        return;
    }
    fflush(stdin);
    printf("Digite o novo nome do cliente: ");
    gets(clientes[id - 1].nome);
    fflush(stdin);
    printf("Digite o novo email do cliente: ");
    gets(clientes[id - 1].email);
    printf("Cliente atualizado com sucesso!\n");
}

// Fun��es para Quartos
void cadastrarQuarto() {
    if (totalQuartos >= maxQuartos) {
        printf(RED "Limite de quartos atingido.\n" RESET);
        return;
    }
    Quarto novo;
    novo.id = totalQuartos + 1;
    do {
        fflush(stdin);
        printf("Digite o tipo do quarto (single/double/suite): ");
        gets(novo.tipo);
    } while (strcmp(novo.tipo, "single") != 0 && strcmp(novo.tipo, "double") != 0 && strcmp(novo.tipo, "suite") != 0);

    printf("Digite o pre�o do quarto: ");
    scanf("%f", &novo.preco);
    printf("Digite a capacidade do quarto: ");
    scanf("%d", &novo.capacidade);
    quartos[totalQuartos++] = novo;
    printf(GREEN "Quarto cadastrado com sucesso! ID: %d\n" RESET, novo.id);
}

void listarQuartos() {
    printf(BLUE "\n--- Lista de Quartos ---\n" RESET);
    for (int i = 0; i < totalQuartos; i++) {
        printf("ID: %d | Tipo: %s | Pre�o: %.2f | Capacidade: %d\n",
               quartos[i].id, quartos[i].tipo, quartos[i].preco, quartos[i].capacidade);
    }
}

void editarQuarto() {
    int id;
    printf("Digite o ID do quarto que deseja editar: ");
    scanf("%d", &id);
    if (id <= 0 || id > totalQuartos) {
        printf(RED "Quarto n�o encontrado.\n" RESET);
        return;
    }
    Quarto *quarto = &quartos[id - 1];

    fflush(stdin);
    do {
        printf("Digite o tipo do quarto (single/double/suite): ");
        gets(quarto->tipo);
    } while (strcmp(quarto->tipo, "single") != 0 && strcmp(quarto->tipo, "double") != 0 && strcmp(quarto->tipo, "suite") != 0);

    printf("Digite o novo pre�o do quarto: ");
    scanf("%f", &quarto->preco);
    printf("Digite a nova capacidade do quarto: ");
    scanf("%d", &quarto->capacidade);
    printf(GREEN "Quarto atualizado com sucesso!\n" RESET);
}

// Fun��es para Reservas
void realizarReserva() {
    if (totalReservas >= maxReservas) {
        printf(RED "Limite de reservas atingido.\n" RESET);
        return;
    }
    int idCliente, idQuarto;
    char dataCheckin[11], dataCheckout[11];

    printf("Digite o ID do cliente: ");
    scanf("%d", &idCliente);
    printf("Digite o ID do quarto: ");
    scanf("%d", &idQuarto);

    if (idCliente <= 0 || idCliente > totalClientes || idQuarto <= 0 || idQuarto > totalQuartos) {
        printf(RED "Cliente ou quarto inv�lido.\n" RESET);
        return;
    }
	
	do {
		printf("Digite a data de check-in (DD/MM/AAAA): ");
		scanf("%s", dataCheckin);
	} while (strlen(dataCheckin) != 10);
    
    do {
    	printf("Digite a data de check-out (DD/MM/AAAA): ");
    	scanf("%s", dataCheckout);
	} while (strlen(dataCheckout) != 10);
	
    if (!verificarDisponibilidade(idQuarto, dataCheckin, dataCheckout)) {
        printf(RED "O quarto selecionado est� indispon�vel para as datas escolhidas.\n" RESET);
        return;
    }

    // Criar nova reserva
    Reserva nova;
    nova.id = totalReservas + 1;
    nova.idCliente = idCliente;
    nova.idQuarto = idQuarto;
    strcpy(nova.dataCheckin, dataCheckin);
    strcpy(nova.dataCheckout, dataCheckout);

    reservas[totalReservas++] = nova;
    printf(GREEN "Reserva realizada com sucesso! ID: %d\n" RESET, nova.id);
}

void listarReservas() {
    printf(BLUE "\n--- Lista de Reservas ---\n" RESET);
    for (int i = 0; i < totalReservas; i++) {
        printf("ID: %d | Cliente ID: %d | Quarto ID: %d | Check-in: %s | Check-out: %s\n",
               reservas[i].id, reservas[i].idCliente, reservas[i].idQuarto,
               reservas[i].dataCheckin, reservas[i].dataCheckout);
    }
}

void cancelarReserva() {
    int id;
    printf("Digite o ID da reserva que deseja cancelar: ");
    scanf("%d", &id);
    if (id <= 0 || id > totalReservas) {
        printf(RED "Reserva n�o encontrada.\n" RESET);
        return;
    }

    for (int i = id - 1; i < totalReservas - 1; i++) {
        reservas[i] = reservas[i + 1];
    }
    totalReservas--;
    printf(GREEN "Reserva cancelada com sucesso!\n" RESET);
}

// Menu principal
void menu() {
    int opcao;
    do {
        printf(BLUE "\n==============================\n");
        printf("       SISTEMA DE RESERVAS     \n");
        printf("==============================\n\n" RESET);
        printf("Seja bem-vindo(a), como podemos ajud�-lo hoje?\n\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Listar Clientes\n");
        printf("3. Editar Cliente\n");
        printf("4. Cadastrar Quarto\n");
        printf("5. Listar Quartos\n");
        printf("6. Editar Quarto\n");
        printf("7. Realizar Reserva\n");
        printf("8. Listar Reservas\n");
        printf("9. Cancelar Reserva\n");
        printf("10. Sair\n");
        printf(BLUE "==============================\n" RESET);
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
	        case 1:
	            cadastrarCliente();
	            system("pause");
	            system("cls");
	            break;
	        case 2:
	            listarClientes();
	            system("pause");
	            system("cls");
	            break;
	        case 3:
	            editarCliente();
	            system("pause");
	            system("cls");
	            break;
	        case 4:
	            cadastrarQuarto();
	            system("pause");
	            system("cls");
	            break;
	        case 5:
	            listarQuartos();
	            system("pause");
	            system("cls");
	            break;
	        case 6:
	            editarQuarto();
	            system("pause");
	            system("cls");
	            break;
	        case 7:
	            realizarReserva();
	             system("pause");
	            system("cls");
	            break;
	        case 8:
	            listarReservas();
	            system("pause");
	            system("cls");
	            break;
	        case 9:
	            cancelarReserva();
	            system("pause");
	            system("cls");
	            break;
	        case 10:
	            printf("Saindo...\n");
	            break;
	        default:
	            printf(RED "Op��o inv�lida! Tente novamente.\n" RESET);
	            system("pause");
	            system("cls");
        }
    } while (opcao != 10);
}

int main() {
    setlocale(LC_ALL, "portuguese");
    menu();
    return 0;
}