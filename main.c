#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Pessoa
struct Person {
  char name[100];
  short int age;
  float balance;
  long id;
};

// Gera um id para uma pessoa baseado nos id's existentes já
long GenID() {
  long ID;
  ID = time(NULL);
  return ID;
}

// Adiciona uma pessoa a lista de pessoas [data]
void addPerson(struct Person *array, int *arraySize, char *name, short int age,
               float balance, long ID) {
  struct Person temp;
  strcpy(temp.name, name);
  temp.age = age;
  temp.balance = balance;
  temp.id = ID;
  array[*arraySize] = temp;
  (*arraySize)++;
}

// Remove uma pessoa da lista de pessoas [data] através do id
void removePersonById(struct Person *array, int *arraySize, int id) {
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
struct Person *findPersonById(struct Person *array, int arraySize, int id) {
  for (int i = 0; i < arraySize; i++) {
    if (array[i].id == id)
      return &array[i];
  }

  return NULL;
}

void transfer(struct Person *source, struct Person *destination, float amount,
              FILE *file) {
  source->balance -= amount;
  destination->balance += amount;
  fprintf(file, "%s,%s,%f\n", source->name, destination->name, amount);
}

int countChar(char *str, char c) {
  int count = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == c) {
      count++;
    }
  }
  return count;
}

int parseName(char *name, char *input) {
  int i = 0;
  while (input[i] != '\0' && input[i] != ',') {
    name[i] = input[i];
    i++;
  }
  name[i] = '\0';
  i++;
  return i;
}

int parseInformation(char *name, short int *age, float *balance) {
  char input[300];
  fgets(input, 300, stdin);

  if (countChar(input, ',') != 2) {
    printf("Erro: Formato inválido.\n");
    return 1;
  }

  int stopIndex = parseName(name, input);

  if (strlen(name) > 100) {
    printf("Erro: Nome deve ter no máximo 100 caracteres.\n");
    return 1;
  }

  sscanf(input + stopIndex + 1, "%hd, %f", age, balance);
  return 0;
}

int main() {
  setlocale(
      LC_ALL,
      "pt_BR.UTF-8"); // Define a localização para Português do Brasil UTF-8

  struct Person *data;
  int dataCapacity = 10;
  int dataSize = 0;

  int option;

  char name[200];
  char oldName[100];
  char line[300];
  short int age;
  int userID;
  float balance;
  int consultID;
  int sourceID, destinationID;
  float amount;
  int removeID;
  int userNumber;

  FILE *transfersFile;
  transfersFile = fopen("transfers.txt", "a");
  if (transfersFile == NULL) {
    printf("Erro ao abrir o arquivo.");
    return 1;
  }

  // Aloca memória para o data (array de Person).
  data = (struct Person *)malloc(dataCapacity * sizeof(struct Person));

  // Inicializa o data com os dados dos usuarios cadastrados nas instancias
  // anteriores.
  FILE *file;
  file = fopen("data.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.");
    return 1;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    struct Person temp;
    int dataRead =
        sscanf(line, " %99[^0-9] %hd %f %d", name, &age, &balance, &userID);

    if (strlen(name) > 0) {
      name[strlen(name) - 1] = '\0';
    }

    if (dataRead == 4) {
      if (dataSize >= dataCapacity) {
        dataCapacity++;
        data = (struct Person *)realloc(data,
                                        dataCapacity * sizeof(struct Person));
        if (data == NULL) {
          printf("Erro ao realocar memória.\n");
          exit(1);
        }
      }
      addPerson(data, &dataSize, name, age, balance, userID);
    }
  }

  fclose(file);

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
  char foo;

  while (option > 0 && option < 7) {
    switch (option) {
    case 1:
      scanf("%c", &foo);
      printf("Informe: <nome>, <idade>, <saldo atual>\n");

      int callback = parseInformation(name, &age, &balance);
      if (callback == 1)
        break;

      if (dataSize >= dataCapacity) {
        dataCapacity++;
        data = (struct Person *)realloc(data,
                                        dataCapacity * sizeof(struct Person));
        if (data == NULL) {
          printf("Erro ao realocar memória.\n");
          exit(1);
        }
      }
      addPerson(data, &dataSize, name, age, balance, GenID());
      printf("%s foi adicionado com o ID: %ld\n", data[dataSize - 1].name,
             data[dataSize - 1].id);
      break;

    case 2:
      printf("Quantos usuários serão adicionados?: ");
      scanf("%d", &userNumber);
      scanf("%c", &foo);

      for (int i = 0; i < userNumber; i++) {
        printf("Informe: <nome>, <idade>, <saldo atual>\n");

        int callback = parseInformation(name, &age, &balance);

        if (callback != 1) {
          if (dataSize >= dataCapacity) {
            dataCapacity++;
            data = (struct Person *)realloc(data, dataCapacity *
                                                      sizeof(struct Person));
            if (data == NULL) {
              printf("Erro ao realocar memória.\n");
              exit(1);
            }
          }
          addPerson(data, &dataSize, name, age, balance, GenID());
          printf("%s foi adicionado com o ID: %ld\n\n", data[dataSize - 1].name,
                 data[dataSize - 1].id);
        }
      }
      break;

    case 3:

      printf("Informe o ID que deseja consultar: ");
      scanf("%d", &consultID);
      struct Person *consult = findPersonById(data, dataSize, consultID);
      if (consult == NULL)
        printf("Usuário não encontrado.\n");
      else
        printf("\nSaldo de %s: %.2f\n", consult->name, consult->balance);
      break;

    case 4:
      printf("Informe <id origem> <id destino> <quantia>: ");
      scanf("%d %d %f", &sourceID, &destinationID, &amount);

      // Verifica se o valor da transferência é positivo
      if (amount < 0) {
        while (amount <= 0) {
          printf("\nTranferências com valor negativo não são permitidas. "
                 "Informe um novo valor: \n");
          scanf("%f", &amount);
        }
      }

      // Pega os ponteiros dos usuários de ID: source e destination
      struct Person *source = findPersonById(data, dataSize, sourceID);
      struct Person *destination =
          findPersonById(data, dataSize, destinationID);

      // Verifica se os usuarios participantes de transferência existem
      if (source == NULL) {
        printf("\nO usuário com ID: %d não foi encontrado.\n", sourceID);
        break;
      } else if (destination == NULL) {
        printf("\nO usuário com ID: %d não foi encontrado.\n", destinationID);
        break;
      }

      // Verifica se o source tem saldo o suficiente para realizar a transação
      if (source->balance < amount) {
        printf("\nO usuário %s não possui saldo suficiente para realizar a "
               "transferência.\n",
               source->name);
        break;
      }

      // Realiza a transferência entre os usuários
      transfer(source, destination, amount, transfersFile);
      printf("\nA transferência foi realizada com sucesso.\n");
      break;

    case 5:
      printf("Informe o ID do usuário a ser removido: ");
      scanf("%d", &removeID);
      removePersonById(data, &dataSize, removeID);
      printf("\nO usuário de ID = %d foi removido.\n", removeID);
      break;

    case 6:
      if (dataSize > 0) {
        for (int i = 0; i < dataSize; i++) {
          printf("Nome: %s | Idade: %hd | Saldo: %.2f | ID: %ld\n",
                 data[i].name, data[i].age, data[i].balance, data[i].id);
        }
      } else
        printf("\nAinda não há usuários cadastrados.\n");
      break;
    }

    printf("\n--------------------------------------------------\n");
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
  }

  file = fopen("data.txt", "w");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.");
    return 1;
  }

  for (int i = 0; i < dataSize; i++) {
    fprintf(file, "%s %hd %f %ld\n", data[i].name, data[i].age, data[i].balance,
            data[i].id);
  }

  free(data);
  return 0;
}
