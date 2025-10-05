#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PASSAGEIROS 100
#define MAX_TRIPULACAO 50
#define MAX_VOOS 100
#define MAX_ASSENTOS 200
#define MAX_RESERVAS 300
// Sugestão de Melhoria: mover constantes como essas para um header separado, como config.h. Isso melhora organização e manutenção.

// Estruturas
typedef struct {
    int codigo;
    char nome[50];
    char endereco[100];
    char telefone[20];
    int fidelidade; 
    int pontosFidelidade;
} Passageiro;
// Sugestão de Melhoria: documentar a struct, como (// representa um passageiro do sistema).
// Sugestão de Melhoria: usar bool para fidelidade em vez de int.

typedef struct {
    int codigo;
    char nome[50];
    char telefone[20];
    char cargo[20]; 
} Tripulacao;
// Sugestão de Melhoria: cargo poderia ser um enum (PILOTO, COPILOTO, COMISSARIO). Enum é mais seguro que string.

typedef struct {
    int codigoVoo;
    char data[11]; // formato: DD/MM/AAAA
    char hora[6];  // formato: HH:MM
    char origem[50];
    char destino[50];
    int codigoAviao;
    int codigoPiloto;
    int codigoCopiloto;
    int codigoComissario;
    int status; // 1 para ativo, 0 para inativo
    float tarifa;
} Voo;
// Sugestão de Melhoria: substituir status por enum { ATIVO, INATIVO } para melhor legibilidade.

typedef struct {
    int numeroAssento;
    int codigoVoo;
    int status; // 1 para ocupado, 0 para livre
} Assento;
// Sugestão de Melhoria: status poderia usar enum também { LIVRE, OCUPADO }.

typedef struct {
    int codigoVoo;
    int numeroAssento;
    int codigoPassageiro;
} Reserva;
// Sugestão de Melhoria: criar função utilitária para criar reserva em vez de manipular direto struct.


int cadastrarPassageiro(Passageiro passageiros[], int *numPassageiros);
int cadastrarTripulacao(Tripulacao tripulacao[], int *numTripulacao);
int cadastrarVoo(Voo voos[], int *numVoos, Tripulacao tripulacao[], int numTripulacao);
void cadastrarAssentos(Assento assentos[], int *numAssentos, int codigoVoo);
void listarAssentosDisponiveis(Assento assentos[], int numAssentos, int codigoVoo);
int reservarAssento(Assento assentos[], int numAssentos, Reserva reservas[], int *numReservas, Passageiro passageiros[], int numPassageiros);
int liberarAssento(Assento assentos[], int numAssentos, Reserva reservas[], int *numReservas);
void pesquisarPassageiro(Passageiro passageiros[], int numPassageiros);
void pesquisarTripulacao(Tripulacao tripulacao[], int numTripulacao);
void listarVoosPorPassageiro(Reserva reservas[], int numReservas, Voo voos[], int numVoos, int codigoPassageiro);
void acumularPontosFidelidade(Passageiro passageiros[], int numPassageiros, Reserva reservas[], int numReservas);

// Funções principais
int cadastrarPassageiro(Passageiro passageiros[], int *numPassageiros) {
    if (*numPassageiros >= MAX_PASSAGEIROS) {
        printf("\nLimite de passageiros atingido!\n");
        return 0;
    }

    Passageiro p;
    printf("\nCodigo do Passageiro: ");
    scanf("%d", &p.codigo);
    // Sugestão de Melhoria: validar entrada. O scanf pode receber valor inválido.
    // Sugestão de Melhoria: criar função auxiliar existePassageiro(codigo). Isso evita a duplicação de lógica.

    for (int i = 0; i < *numPassageiros; i++) {
        if (passageiros[i].codigo == p.codigo) {
            printf("\nErro: Já existe um passageiro com o codigo %d!\n", p.codigo);
            return 0;
        }
    }

    printf("Nome: ");
    scanf(" %[^\n]", p.nome);
    // Sugestão de Melhoria: arriscado o scanf ficar sem limite. Isso pode causar buffer overflow, a melhor opção é usar fgets.
    printf("Endereco: ");
    scanf(" %[^\n]", p.endereco);
    printf("Telefone: ");
    scanf(" %[^\n]", p.telefone);
    p.fidelidade = 1;  
    p.pontosFidelidade = 0;

    passageiros[*numPassageiros] = p;
    (*numPassageiros)++;
    printf("\nPassageiro cadastrado com sucesso!\n");
    return 1;
}
// Sugestão de Melhoria: separar lógica de entrada e saída (scanf/printf) da lógica de negócio. Poderia aplicar MVC.


int cadastrarTripulacao(Tripulacao tripulacao[], int *numTripulacao) {
    if (*numTripulacao >= MAX_TRIPULACAO) {
        printf("\nLimite de tripulação atingido!\n");
        return 0;
    }

    Tripulacao t;
    printf("\nCodigo do Tripulante: ");
    scanf("%d", &t.codigo);

    // Verifica se o código já existe
    for (int i = 0; i < *numTripulacao; i++) {
        if (tripulacao[i].codigo == t.codigo) {
            printf("\nErro: Já existe um tripulante com o codigo %d!\n", t.codigo);
            return 0;
        }
    }

    printf("Nome: ");
    scanf(" %[^\n]", t.nome);
    // Sugestão de Melhoria: arriscado o scanf ficar sem limite. Isso pode causar buffer overflow, a melhor opção é usar fgets.
    printf("Telefone: ");
    scanf(" %[^\n]", t.telefone);

    // Validando o cargo
    int cargoValido = 0;
    while (!cargoValido) {
        printf("Cargo (piloto/copiloto/comissario): ");
        scanf(" %[^\n]", t.cargo);
        if (strcmp(t.cargo, "piloto") == 0 || strcmp(t.cargo, "copiloto") == 0 || strcmp(t.cargo, "comissario") == 0) {
            cargoValido = 1;
        } else {
            printf("Cargo invalido. Por favor, digite novamente.\n");
        }
    }

    tripulacao[*numTripulacao] = t;
    (*numTripulacao)++;
    printf("\nTripulante cadastrado com sucesso!\n");
    return 1;
}
// Sugestão de Melhoria: separar lógica de entrada e saída (scanf/printf) da lógica de negócio. Poderia aplicar MVC.

int cadastrarVoo(Voo voos[], int *numVoos, Tripulacao tripulacao[], int numTripulacao) {
    if (*numVoos >= MAX_VOOS) {
        printf("\nLimite de voos atingido!\n");
        return 0;
    }

    if (numTripulacao == 0) {
        printf("\nNao há tripulação cadastrada. Nao foi possivel criar voos.\n");
        return 0;
    }

    int temPiloto = 0, temCopiloto = 0;
    // Sugestão de Melhoria: usar bool em vez de int para flags.
    for (int i = 0; i < numTripulacao; i++) {
        if (strcmp(tripulacao[i].cargo, "piloto") == 0) {
            temPiloto = 1;
        }
        if (strcmp(tripulacao[i].cargo, "copiloto") == 0) {
            temCopiloto = 1;
        }
        if (temPiloto && temCopiloto) break;
    }
    // Sugestão de Melhoria: lógica repetida. Poderia virar função validarTripulacao().

    if (!temPiloto || !temCopiloto) {
        printf("\nNao tem piloto e/ou copiloto cadastrados. Nao foi possivel ativar o voo.\n");
        return 0;
    }

    Voo v;
    v.codigoVoo = *numVoos + 1;
    // Sugestão de Melhoria: poderia considerar usar geração automática de ID em função separada.
    printf("\nData (DD/MM/AAAA): ");
    scanf(" %[^\n]", v.data);
    printf("Hora (HH:MM): ");
    scanf(" %[^\n]", v.hora);
    // Sugestão de Melhoria: validar formato de data e hora antes de salvar.
    printf("Origem: ");
    scanf(" %[^\n]", v.origem);
    printf("Destino: ");
    scanf(" %[^\n]", v.destino);
    printf("Tarifa: ");
    scanf("%f", &v.tarifa);

    
    printf("\nCodigo do Piloto: ");
    scanf("%d", &v.codigoPiloto);
    printf("Codigo do Copiloto: ");
    scanf("%d", &v.codigoCopiloto);
    printf("Codigo do Comissário (ou 0 para nenhum): ");
    scanf("%d", &v.codigoComissario);

   // Sugestão de Melhoria: poderia extrair essa validação para função validarCodigoTripulacao.
    int pilotoEncontrado = 0, copilotoEncontrado = 0;
    for (int i = 0; i < numTripulacao; i++) {
        if (tripulacao[i].codigo == v.codigoPiloto && strcmp(tripulacao[i].cargo, "piloto") == 0) {
            pilotoEncontrado = 1;
        }
        if (tripulacao[i].codigo == v.codigoCopiloto && strcmp(tripulacao[i].cargo, "copiloto") == 0) {
            copilotoEncontrado = 1;
        }
    }

    if (!pilotoEncontrado || !copilotoEncontrado) {
        printf("\nCodigo de piloto e/ou copiloto invalido(s). Nao é possível ativar o voo.\n");
        return 0;
    }

    v.status = 1; 
    voos[*numVoos] = v;
    (*numVoos)++;
    printf("\nVoo cadastrado e marcado como ativo com sucesso!\n");
    return 1;
}
// Sugestão de Melhoria: função muito longa. Poderia refatorar em sub-funções.


void cadastrarAssentos(Assento assentos[], int *numAssentos, int codigoVoo) {
    int quantidade;
    printf("\nQuantos assentos deseja cadastrar para o voo %d? ", codigoVoo);
    scanf("%d", &quantidade);

    for (int i = 0; i < quantidade; i++) {
        Assento a;
        a.numeroAssento = *numAssentos + 1;
        a.codigoVoo = codigoVoo;
        a.status = 0; // Livre
        assentos[*numAssentos] = a;
        (*numAssentos)++;
    }
    printf("\n%d assentos cadastrados com sucesso para o voo %d!\n", quantidade, codigoVoo);
}

void listarAssentosDisponiveis(Assento assentos[], int numAssentos, int codigoVoo) {
    printf("\nAssentos disponíveis para o voo %d:\n", codigoVoo);
    for (int i = 0; i < numAssentos; i++) {
        if (assentos[i].codigoVoo == codigoVoo && assentos[i].status == 0) {
            printf("Assento %d\n", assentos[i].numeroAssento);
        }
    }
}

int reservarAssento(Assento assentos[], int numAssentos, Reserva reservas[], int *numReservas, Passageiro passageiros[], int numPassageiros) {
    int codigoVoo, numeroAssento, codigoPassageiro;
    printf("\nCodigo do Voo: ");
    scanf("%d", &codigoVoo);
    listarAssentosDisponiveis(assentos, numAssentos, codigoVoo);

    printf("\nEscolha o numero do assento: ");
    scanf("%d", &numeroAssento);
    printf("Codigo do Passageiro: ");
    scanf("%d", &codigoPassageiro);

    // Validações e reserva
    for (int i = 0; i < numAssentos; i++) {
        if (assentos[i].codigoVoo == codigoVoo && assentos[i].numeroAssento == numeroAssento && assentos[i].status == 0) {
            assentos[i].status = 1;
            Reserva r = {codigoVoo, numeroAssento, codigoPassageiro};
            reservas[*numReservas] = r;
            (*numReservas)++;
            printf("\nAssento %d reservado com sucesso!\n", numeroAssento);
            return 1;
        }
    }
    printf("\nAssento não disponível ou inexistente.\n");
    return 0;
}

int liberarAssento(Assento assentos[], int numAssentos, Reserva reservas[], int *numReservas) {
    int codigoVoo, numeroAssento;
    printf("\nCodigo do Voo: ");
    scanf("%d", &codigoVoo);
    printf("Numero do Assento: ");
    scanf("%d", &numeroAssento);

    // Libera o assento
    for (int i = 0; i < numAssentos; i++) {
        if (assentos[i].codigoVoo == codigoVoo && assentos[i].numeroAssento == numeroAssento && assentos[i].status == 1) {
            assentos[i].status = 0;
            // Problema: aqui o assento é liberado, mas a reserva correspondente não é removida.
            // Sugestão de Melhoria: criar função removerReserva(codigoVoo, numeroAssento).
            printf("\nAssento %d liberado com sucesso!\n", numeroAssento);
            return 1;
        }
    }
    printf("\nAssento nao encontrado ou já está livre.\n");
    return 0;
}

void pesquisarPassageiro(Passageiro passageiros[], int numPassageiros) {
    char nome[50];
    int codigo;
    printf("\nDigite o codigo ou nome do passageiro: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < numPassageiros; i++) {
        if (passageiros[i].codigo == atoi(nome) || strstr(passageiros[i].nome, nome) != NULL) {
            printf("\nPassageiro encontrado: %s\n", passageiros[i].nome);
            return;
        }
    }
    printf("\nPassageiro nao encontrado.\n");
}

void pesquisarTripulacao(Tripulacao tripulacao[], int numTripulacao) {
    char nome[50];
    int codigo;
    printf("\nDigite o codigo ou nome do tripulante: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < numTripulacao; i++) {
        if (tripulacao[i].codigo == atoi(nome) || strstr(tripulacao[i].nome, nome) != NULL) {
            printf("\nTripulante encontrado: %s\n", tripulacao[i].nome);
            return;
        }
    }
    printf("\nTripulante nao encontrado.\n");
}

void listarVoosPorPassageiro(Reserva reservas[], int numReservas, Voo voos[], int numVoos, int codigoPassageiro) {
    printf("\nVoos do Passageiro:\n");
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].codigoPassageiro == codigoPassageiro) {
            for (int j = 0; j < numVoos; j++) {
                if (voos[j].codigoVoo == reservas[i].codigoVoo) {
                    printf("Voo %d - %s para %s\n", voos[j].codigoVoo, voos[j].origem, voos[j].destino);
                }
            }
        }
    }
}

void acumularPontosFidelidade(Passageiro passageiros[], int numPassageiros, Reserva reservas[], int numReservas) {
    for (int i = 0; i < numReservas; i++) {
        for (int j = 0; j < numPassageiros; j++) {
            if (passageiros[j].codigo == reservas[i].codigoPassageiro) {
                passageiros[j].pontosFidelidade += 10;  // A cada voo, o passageiro ganha 10 pontos
            }
        }
    }
    printf("\nPontos de fidelidade acumulados!\n");
}

int main() {
    Passageiro passageiros[MAX_PASSAGEIROS];
    Tripulacao tripulacao[MAX_TRIPULACAO];
    Voo voos[MAX_VOOS];
    Assento assentos[MAX_ASSENTOS];
    Reserva reservas[MAX_RESERVAS];

    int numPassageiros = 0, numTripulacao = 0, numVoos = 0, numAssentos = 0, numReservas = 0;

    int opcao;
    do {
        printf("\nEscolha uma opcaoo:\n");
        // Sugestão de Melhoria: extrair para função menuPrincipal() para reduzir responsabilidade da main.
        printf("1 - Cadastrar Passageiro\n");
        printf("2 - Cadastrar Tripulante\n");
        printf("3 - Cadastrar Voo\n");
        printf("4 - Cadastrar Assentos\n");
        printf("5 - Reservar Assento\n");
        printf("6 - Liberar Assento\n");
        printf("7 - Pesquisar Passageiro\n");
        printf("8 - Pesquisar Tripulante\n");
        printf("9 - Listar Voos por Passageiro\n");
        printf("10 - Acumular Pontos Fidelidade\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarPassageiro(passageiros, &numPassageiros);
                break;
            // Sugestão de Melhoria: as funções poderiam receber struct Sistema em vez de múltiplos parâmetros.
            case 2:
                cadastrarTripulacao(tripulacao, &numTripulacao);
                break;
            case 3:
                cadastrarVoo(voos, &numVoos, tripulacao, numTripulacao);
                break;
            case 4:
                if (numVoos > 0) {
                    int codigoVoo;
                    printf("\nDigite o codigo do voo para cadastrar assentos: ");
                    scanf("%d", &codigoVoo);
                    cadastrarAssentos(assentos, &numAssentos, codigoVoo);
                } else {
                    printf("\nNenhum voo cadastrado!\n");
                }
                break;
            case 5:
                reservarAssento(assentos, numAssentos, reservas, &numReservas, passageiros, numPassageiros);
                break;
            case 6:
                liberarAssento(assentos, numAssentos, reservas, &numReservas);
                break;
            case 7:
                pesquisarPassageiro(passageiros, numPassageiros);
                break;
            case 8:
                pesquisarTripulacao(tripulacao, numTripulacao);
                break;
            case 9:
                if (numPassageiros > 0) {
                    int codigoPassageiro;
                    printf("\nDigite o codigo do passageiro: ");
                    scanf("%d", &codigoPassageiro);
                    listarVoosPorPassageiro(reservas, numReservas, voos, numVoos, codigoPassageiro);
                } else {
                    printf("\nNenhum passageiro cadastrado!\n");
                }
                break;
            case 10:
                acumularPontosFidelidade(passageiros, numPassageiros, reservas, numReservas);
                break;
            case 0:
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;

}
// Sugestão de Melhora: a main está bem sobrecarregada. O ideal seria ter apenas inicialização e chamada do menu.
