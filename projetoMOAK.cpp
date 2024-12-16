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

// Dados em memória
Cliente clientes[maxClientes];
Quarto quartos[maxQuartos];
Reserva reservas[maxReservas];
int totalClientes = 0, totalQuartos = 0, totalReservas = 0;

// Função para verificar conflito de datas
int conflitoDeDatas(char *checkin1, char *checkout1, char *checkin2, char *checkout2) {
    return !(strcmp(checkout1, checkin2) < 0 || strcmp(checkout2, checkin1) < 0);
}

// Função para verificar se o quarto está disponível em um intervalo de datas
int verificarDisponibilidade(int idQuarto, char *dataCheckin, char *dataCheckout) {
    for (int i = 0; i < totalReservas; i++) {
        if (reservas[i].idQuarto == idQuarto) {
            if (conflitoDeDatas(reservas[i].dataCheckin, reservas[i].dataCheckout, dataCheckin, dataCheckout)) {
                return 0; // Conflito encontrado, quarto indisponível
            }
        }
    }
    return 1; // Quarto disponível
}

// Funções para Clientes
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
        printf(RED "Cliente não encontrado.\n" RESET);
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

// Funções para Quartos
void cadastrarQuarto() {
    if (totalQuartos >= maxQuartos) {
        printf(RED "Limite de quartos atingido.\n" RESET);
        return;
    }
    Quarto novo;
    novo.id = totalQuartos + 1;
    
    do {
        fflush(stdin);
        printf("Digite o tipo do quarto (standart/master/deluxe): ");
        gets(novo.tipo);
    } while (strcmp(novo.tipo, "standart") != 0 && strcmp(novo.tipo, "master") != 0 && strcmp(novo.tipo, "deluxe") != 0);

    printf("Digite o preço do quarto: ");
    scanf("%f", &novo.preco);
    
    do {
		printf("Digite a capacidade do quarto (MAX 6): ");
	    scanf("%d", &novo.capacidade);
    } while (novo.capacidade < 1 || novo.capacidade > 6);
    
    quartos[totalQuartos++] = novo;
    printf(GREEN "Quarto cadastrado com sucesso! ID: %d\n" RESET, novo.id);
}

void listarQuartos() {
    printf(BLUE "\n--- Lista de Quartos ---\n" RESET);
    for (int i = 0; i < totalQuartos; i++) {
        printf("ID: %d | Tipo: %s | Preço: %.2f | Capacidade: %d\n",
               quartos[i].id, quartos[i].tipo, quartos[i].preco, quartos[i].capacidade);
    }
}

void listarQuartosDisponiveis() {
    char dataCheckin[11], dataCheckout[11];

    do {
        printf("Digite a data de check-in (DD/MM/AAAA): ");
        scanf("%s", dataCheckin);
    } while (strlen(dataCheckin) != 10);

    do {
        printf("Digite a data de check-out (DD/MM/AAAA): ");
        scanf("%s", dataCheckout);
    } while (strlen(dataCheckout) != 10);

    printf(CYAN "\n--- Lista de Quartos Disponíveis ---\n" RESET);
    int disponiveis = 0;
    for (int i = 0; i < totalQuartos; i++) {
        if (verificarDisponibilidade(quartos[i].id, dataCheckin, dataCheckout)) {
            printf("ID: %d | Tipo: %s | Preço: %.2f | Capacidade: %d\n",
                   quartos[i].id, quartos[i].tipo, quartos[i].preco, quartos[i].capacidade);
            disponiveis++;
        }
    }

    if (disponiveis == 0) {
        printf(RED "Nenhum quarto disponível para o intervalo de datas fornecido.\n" RESET);
    }
}

void editarQuarto() {
    int id;
    printf("Digite o ID do quarto que deseja editar: ");
    scanf("%d", &id);
    if (id <= 0 || id > totalQuartos) {
        printf(RED "Quarto não encontrado.\n" RESET);
        return;
    }
    Quarto *quarto = &quartos[id - 1];

    fflush(stdin);
    do {
        printf("Digite o tipo do quarto (standart/master/deluxe): ");
        gets(quarto->tipo);
    } while (strcmp(quarto->tipo, "single") != 0 && strcmp(quarto->tipo, "double") != 0 && strcmp(quarto->tipo, "suite") != 0);

    printf("Digite o novo preço do quarto: ");
    scanf("%f", &quarto->preco);
    printf("Digite a nova capacidade do quarto: ");
    scanf("%d", &quarto->capacidade);
    printf(GREEN "Quarto atualizado com sucesso!\n" RESET);
}

// Funções para Reservas
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
        printf(RED "Cliente ou quarto inválido.\n" RESET);
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
        printf(RED "O quarto selecionado está indisponível para as datas escolhidas.\n" RESET);
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
        printf(RED "Reserva não encontrada.\n" RESET);
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
    	fflush(stdin);
    	
    	printf(CYAN " ______________________________ ");
        printf(CYAN "\n|                              |\n");
        printf("|     SISTEMA DE RESERVAS      |\n");
        printf("|______________________________|\n|                              |\n" RESET);
        printf(CYAN "|" RESET " Seja bem-vindo(a),           " CYAN "|" RESET "\n" CYAN "|" RESET " como podemos ajudar hoje?    " CYAN "|" RESET "\n" CYAN "|                              |" RESET "\n");
        printf(CYAN "|" RESET "1.  Cadastrar Cliente         " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "2.  Listar Clientes           " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "3.  Editar Cliente            " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "4.  Cadastrar Quarto          " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "5.  Listar Quartos            " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "6.  Listar Quartos Disponíveis" CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "7.  Editar Quarto             " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "8.  Realizar Reserva          " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "9.  Listar Reservas           " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "10. Cancelar Reserva          " CYAN "|" RESET "\n");
        printf(CYAN "|" RESET "11. " RED "Sair                      " CYAN "|" RESET "\n" RESET);
        printf(CYAN "|______________________________|\n" RESET);
        printf("\nEscolha uma opção: ");
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
		    listarQuartosDisponiveis();
		    system("pause");
		    system("cls");
			break;
        case 7:
            editarQuarto();
            system("pause");
            system("cls");
            break;
        case 8:
            realizarReserva();
        	system("pause");
            system("cls");
            break;
        case 9:
            listarReservas();
            system("pause");
            system("cls");
            break;
        case 10:
            cancelarReserva();
            system("pause");
            system("cls");
            break;
        case 11:
            printf(RED "Saindo...\n" RESET);
            break;
        default:
            printf(RED "Opção inválida! Tente novamente.\n" RESET);
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