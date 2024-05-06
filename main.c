#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

// Pesssoa
struct Person {
    char name[100];
    short int age;
    float balance;  
    int id;
};

// Gera um id para uma pessoa baseado nos id's existentes já
int GenID(struct Person* array, int arraySize) {
    int maxID = 0;

    if (arraySize == 0){
        return 1;
    }
    else{
        for (int i = 0; i < arraySize; i++) {
            if (array[i].id > maxID) {
                maxID = array[i].id;
            }
        }
        return maxID + 1;
    }
}

// Adiciona uma pessoa a lista de pessoas [data]
void addPerson(struct Person* array, int* arraySize, char* name, short int age, float balance) {
    struct Person temp;
    strcpy(temp.name, name);
    temp.age = age;
    temp.balance = balance;
    temp.id = GenID(array, *arraySize);
    array[*arraySize] = temp;
    *arraySize = *arraySize + 1;
}

// Remove uma pessoa da lista de pessoas [data] através do id
void removePersonById(struct Person* array, int* arraySize, int id){
    int i;
    for (i = 0; i < *arraySize; i++) {
        if (array[i].id == id)
        break;
    }

    for (int j = i; j < *arraySize - 1; j++) {
        array[j] = array[j + 1];
    }

    (*arraySize)--;
}

// Retorna o struct de uma pessoas através do id dela
struct Person* findPersonById(struct Person* array, int arraySize, int id){
    for (int i = 0; i < arraySize; i++) {
        if (array[i].id == id) return &array[i];
    }

    return NULL;
}

void transfer(struct Person* source, struct Person* destination, float amount) {
    source->balance -= amount;
    destination->balance += amount;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Define a localização para Português do Brasil UTF-8

    struct Person* data;
    int dataCapacity = 10;
    int dataSize = 0;

    int option;

    char name[100];
    char oldName[100];
    short int age;
    float balance;
    int consultID;
    int sourceID, destinationID;
    float amount;
    int removeID;
    int userNumber;

    // Aloca memória para o data (array de Person)
    data = (struct Person*)malloc(dataCapacity * sizeof(struct Person));

    printf("--------------Bem vind@ ao UaiBank!--------------\n");
    printf("1 - Inserir novo usuário.\n");
    printf("2 - Inserir múltiplos usuários.\n");
    printf("3 - Consultar saldo de usuário por ID.\n");
    printf("4 - Fazer uma transferência entre usuário.\n");
    printf("5 - Remover usuário por ID.\n");
    printf("6 - Listar todos usuários.\n");
    printf("Caso deseje sair, digite um número diferente dos anteriores.\n");
    printf("--------------------------------------------------\n");
    printf("Opção: ");
    scanf("%d", &option);

    while (option > 0 && option < 7) {
        switch (option){
            case 1:
                printf("Informe: <nome> <idade> <saldo atual> ");
                scanf(" %99[^0-9] %hd %f", name, &age, &balance);
                if (strlen(name) > 0) {
                    name[strlen(name) - 1] = '\0';
                }

                if (dataSize >= dataCapacity) {
                    dataCapacity++;
                    data = (struct Person*)realloc(data, dataCapacity * sizeof(struct Person));
                    if (data == NULL) {
                        printf("Erro ao realocar memória.\n");
                        exit(1);
                    }
                }
                addPerson(data, &dataSize, name, age, balance);
                printf("%s foi adicionado com o ID: %d\n", data[dataSize -1].name, data[dataSize -1].id);
                break;

            case 2:
                printf("Quantos usuários serão adicionados?: ");
                scanf("%d", &userNumber);

                for (int i = 0; i < userNumber; i++) {
                    printf("Informe: <nome> <idade> <saldo atual> ");
                    scanf(" %99[^0-9] %hd %f", name, &age, &balance);
                    if (strlen(name) > 0) {
                        name[strlen(name) - 1] = '\0';
                    }
                    
                    if (dataSize >= dataCapacity) {
                        dataCapacity++;
                        data = (struct Person*)realloc(data, dataCapacity * sizeof(struct Person));
                        if (data == NULL) {
                            printf("Erro ao realocar memória.\n");
                            exit(1);
                        }
                    }
                    addPerson(data, &dataSize, name, age, balance);
                    printf("\n%s foi adicionado com o ID: %d\n", data[dataSize -1].name, data[dataSize -1].id);
                }
                break;

            case 3:

                printf("Informe o ID que deseja consultar: ");
                scanf("%d", &consultID);
                struct Person* consult = findPersonById(data, dataSize, consultID);
                if (consult == NULL) printf("Usuário não encontrado.\n");
                else printf("\nSaldo de %s: %.2f\n", consult->name, consult->balance);
                break;

            case 4:
                printf("Informe o ID de origem e destino da transferência e a quantia a ser transferida: ");
                scanf("%d %d %f", &sourceID, &destinationID, &amount);

                if (amount < 0) {
                    while (amount <= 0) {
                        printf("\nTranferências com valor negativo não são permitidas. Informe um novo valor: \n");
                        scanf("%f", &amount);
                    }
                }

                struct Person* source = findPersonById(data, dataSize, sourceID);
                struct Person* destination = findPersonById(data, dataSize, destinationID);

                if (source->balance < amount){
                    printf("\nO usuário %s não possui saldo suficiente para realizar a transferência.\n", source->name);
                    break;
                }

                transfer(source, destination, amount);
                printf("\nA transferência foi realizada com sucesso.\n");
                break;

            case 5:
                printf("Informe o ID do usuário a ser removido: ");
                scanf("%d", &removeID);
                removePersonById(data, &dataSize, removeID);
                printf("\nO usuário de ID = %d foi removido.\n", removeID);
                break;

            case 6:
                if (dataSize > 0){
                    for (int i = 0; i < dataSize; i++){
                        printf("Nome: %s | Idade: %hd | Saldo: %.2f | ID: %d\n", data[i].name, data[i].age, data[i].balance, data[i].id);
                    }
                }
                else printf("\nAinda não há usuários cadastrados.\n");
                break;
        }

        printf("\n--------------------------------------------------\n");
        printf("1 - Inserir novo usuário.\n");
        printf("2 - Inserir múltiplos usuários.\n");
        printf("3 - Consultar saldo de usuário por ID.\n");
        printf("4 - Fazer uma transferência entre usuário. <id origem> <id destino> <quantia>\n");
        printf("5 - Remover usuário por ID.\n");
        printf("6 - Listar todos usuários.\n");
        printf("Caso deseje sair, digite um número diferente dos anteriores.\n");
        printf("--------------------------------------------------\n");
        printf("Opção: ");
        scanf("%d", &option);
    }

    free(data);
    return 0;
}