#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct regveiculocab{
    char status;
    long long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[19];
    char descreveData[36];
    char descreveLugares[43];
    char descreveLinha[27];
    char descreveModelo[18];
    char descreveCategoria[21];
} RegVeiculoCab;

typedef struct regveiculodados{
    char removido;
    int tamanhoRegistro;
    char prefixo[6];
    char data[11];
    int quantidadeLugares;
    int codLinha;    
    int tamanhoModelo;
    char modelo[100];
    int tamanhoCategoria;    
    char categoria[100];
} RegVeiculoDados;

typedef struct reglinhacab{
    char status;
    long long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[16];
    char descreveCartao[14];
    char descreveNome[14];
    char descreveCor[25];
} RegLinhaCab;

typedef struct reglinhadados{
    //Declaro codLinha como char para poder ler o asterisco
    char codLinha[4];
    char aceitaCartao[2];
    char nomeLinha[100];
    char corLinha[100];
    char removido;
    int tamanhoRegistro;
    int tamanhoNome;
    int tamanhoCor;
} RegLinhaDados;

//////////////////////////////////////////////////////////

void ler_linha(FILE* arquivo, char* linha){
    fgets(linha, 1000, arquivo);
    return;
}

//////////////////////////////////////////////////////////

void escrever_dados_veiculo(FILE* binario, char* linha, RegVeiculoDados* dados){

    //removido
    printf("%d ", fwrite(&dados->removido, sizeof(char), 1, binario));
    //tamanhoRegistro
    printf("%d ", fwrite(&dados->tamanhoRegistro, sizeof(int), 1, binario));
    //prefixo
    printf("%d ", fwrite(dados->prefixo, sizeof(char), 5, binario));
    //data
    printf("%d ", fwrite(dados->data, sizeof(char), 10, binario));
    //quantidadeLugares
    printf("%d ", fwrite(&dados->quantidadeLugares, sizeof(int), 1, binario));
    //codLinha
    printf("%d ", fwrite(&dados->codLinha, sizeof(int), 1, binario));
    //tamanhoModelo
    printf("%d ", fwrite(&dados->tamanhoModelo, sizeof(int), 1, binario));
    //modelo
    printf("%d ", fwrite(dados->modelo, sizeof(char), dados->tamanhoModelo, binario));
    //tamanhoCategoria
    printf("%d ", fwrite(&dados->tamanhoCategoria, sizeof(int), 1, binario));
    //categoria
    printf("%d\n", fwrite(dados->categoria, sizeof(char), dados->tamanhoCategoria, binario));


    printf("Prefixo do veiculo: %s\n", dados->prefixo);
    printf("Modelo do veiculo: %s\n", dados->modelo);
    printf("Categoria do veiculo: %s\n", dados->categoria);
    printf("Data de entrada do veiculo na frota: %s\n", dados->data);
    printf("Quantidade de lugares sentados disponiveis: %d\n", dados->quantidadeLugares);
    printf("\n");

    return;
}

void atribuir_dados_veiculo(RegVeiculoDados* dados, char* linha){

    char *tok;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(dados->prefixo, tok);

    //Confiro se o registro esta logicamente removido
    if(dados->prefixo[0] == '*'){
        dados->removido = '0';
    } else{
        dados->removido = '1';
    }

    tok = strtok(NULL, ",");

    strcpy(dados->data, tok);

    tok = strtok(NULL, ",");
    dados->quantidadeLugares = atoi(tok);

    tok = strtok(NULL, ",");
    dados->codLinha = atoi(tok);

    tok = strtok(NULL, ",");
    dados->tamanhoModelo = strlen(tok);    
    
    strcpy(dados->modelo, tok);
    tok = strtok(NULL, ",");

    //O tamanho está acrescentado em 1 por conta do \n
    dados->tamanhoCategoria = strlen(tok) - 1;
    strcpy(dados->categoria, tok);
    //Por conta de o fgets estar lendo o \n, eu
    //substituo ele por um \0
    dados->categoria[strlen(tok) - 1] = '\0';

    dados->tamanhoRegistro = dados->tamanhoModelo + dados->tamanhoCategoria + 31;

    return;
}

void escrever_cabecalho_veiculo(FILE* binario, char* linha, RegVeiculoCab* cabecalho){

    //status
    fwrite("0", sizeof(char), 1, binario);
    //byteProxReg
    fwrite("0", sizeof(long long), 1, binario);
    //nroRegistros
    fwrite("0", sizeof(int), 1, binario);
    //nroRegRemovidos
    fwrite("0", sizeof(int), 1, binario);
    //descrevePrefixo
    fwrite(cabecalho->descrevePrefixo, sizeof(char), 18, binario);
    //descreveData
    fwrite(cabecalho->descreveData, sizeof(char), 35, binario);
    //descreveLugares
    fwrite(cabecalho->descreveLugares, sizeof(char), 42, binario);
    //descreveLinha
    fwrite(cabecalho->descreveLinha, sizeof(char), 26, binario);
    //descreveModelo
    fwrite(cabecalho->descreveModelo, sizeof(char), 17, binario);
    //descreveCategoria
    fwrite(cabecalho->descreveCategoria, sizeof(char), 20, binario);

    return;
}

void atribuir_cabecalho_veiculo(RegVeiculoCab* cabecalho, char* linha){

    char *tok;

    cabecalho->status = 0;
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(cabecalho->descrevePrefixo, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveData, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveLugares, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveLinha, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveModelo, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveCategoria, tok);
    //Por conta de o fgets estar lendo o \n, eu
    //substituo ele por um \0
    cabecalho->descreveCategoria[strlen(tok) - 1] = '\0';
    return;
}

void primeiro_caso(){

    RegVeiculoCab* cabecalho = (RegVeiculoCab*) malloc(sizeof(RegVeiculoCab));
    RegVeiculoDados* dados = (RegVeiculoDados*) malloc(sizeof(RegVeiculoDados));

    char nome_arquivo[20];
    char linha[1000];

    scanf("%[^\n]s", nome_arquivo);

    FILE* csv = fopen(nome_arquivo, "r");
    FILE* binario = fopen("veiculo.bin", "wb");

    ler_linha(csv, linha);

    atribuir_cabecalho_veiculo(cabecalho, linha);

    escrever_cabecalho_veiculo(binario, linha, cabecalho);

    while(1){
        ler_linha(csv, linha);
        if(linha == NULL){
            break;
        }
        atribuir_dados_veiculo(dados, linha);
        escrever_dados_veiculo(binario, linha, dados);
    }

    fclose(csv);
    fclose(binario);

    return;
}

//////////////////////////////////////////////////////////

void escrever_dados_linha(FILE* binario, char* linha, RegLinhaDados* dados){

    fwrite(&dados->removido, sizeof(char), 1, binario);
    fwrite(&dados->tamanhoRegistro, sizeof(int), 1, binario);
    fwrite(&dados->codLinha, sizeof(int), 1, binario);
    fwrite(&dados->aceitaCartao, sizeof(char), 1, binario);
    fwrite(&dados->tamanhoNome, sizeof(int), 1, binario);
    fwrite(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
    fwrite(&dados->tamanhoCor, sizeof(int), 1, binario);
    fwrite(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);

    return;
}

void atribuir_dados_linha(RegLinhaDados* dados, char* linha){

    char *tok;

    dados->removido = '1';
    dados->tamanhoRegistro = 0;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(dados->codLinha, tok);

    //Confiro se o registro esta logicamente removido
    if(dados->codLinha[0] == '*'){
        dados->removido = 0;
    } else{
        dados->removido = 1;
    }


    tok = strtok(NULL, ",");
    strcpy(dados->aceitaCartao, tok);

    tok = strtok(NULL, ",");
    dados->tamanhoNome = strlen(tok);
    strcpy(dados->nomeLinha, tok);

    tok = strtok(NULL, ",");
    //O tamanho está acrescentado em 1 por conta do \n
    dados->tamanhoCor = strlen(tok) - 1;
    strcpy(dados->corLinha, tok);
    //Por conta de o fgets estar lendo o \n, eu
    //substituo ele por um \0
    dados->corLinha[strlen(tok) - 1] = '\0';

    dados->tamanhoRegistro = dados->tamanhoCor + dados->tamanhoNome + 13;

    return;
}

void escrever_cabecalho_linha(FILE* binario, char* linha, RegLinhaCab* cabecalho){

    fwrite("0", sizeof(char), 1, binario);
    fwrite("0", sizeof(long long), 1, binario);
    fwrite("0", sizeof(int), 1, binario);
    fwrite("0", sizeof(int), 1, binario);
    fwrite(cabecalho->descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalho->descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalho->descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalho->descreveCor, sizeof(char), 24, binario);

    return;
}

void atribuir_cabecalho_linha(RegLinhaCab* cabecalho, char* linha){

    char *tok;

    cabecalho->status = 0;
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(cabecalho->descreveCodigo, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveCartao, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveNome, tok);
    tok = strtok(NULL, ",");
    strcpy(cabecalho->descreveCor, tok);
    //Por conta de o fgets estar lendo o \n, eu
    //substituo ele por um \0
    cabecalho->descreveCor[strlen(tok) - 1] = '\0';
    return;
}

void segundo_caso(){

    RegLinhaCab* cabecalho = (RegLinhaCab*) malloc(sizeof(RegLinhaCab));
    RegLinhaDados* dados = (RegLinhaDados*) malloc(sizeof(RegLinhaDados));

    char nome_arquivo[20];
    char linha[1000];

    scanf("%[^\n]s", nome_arquivo);

    FILE* csv = fopen(nome_arquivo, "r");
    FILE* binario = fopen("linha.bin", "wb");

    ler_linha(csv, linha);

    atribuir_cabecalho_linha(cabecalho, linha);

    escrever_cabecalho_linha(binario, linha, cabecalho);

    while(1){
        ler_linha(csv, linha);
        if(linha == NULL){
            break;
        }
        atribuir_dados_linha(dados, linha);
        escrever_dados_linha(binario, linha, dados);
    }

    fclose(csv);
    fclose(binario);

    return;
}

//////////////////////////////////////////////////////////

void terceiro_caso(){

    RegVeiculoDados* dados = (RegVeiculoDados*) malloc(sizeof(RegVeiculoDados));

    char nome_arquivo[20];

    dados->tamanhoRegistro = 175;

    scanf("%[^\n]s", nome_arquivo);

    FILE* binario = fopen(nome_arquivo, "rb");  

    if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
        return;
    }  

    while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
        fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
        if(dados->removido = '1'){
            fread(dados->prefixo, sizeof(char), 5, binario);
            fread(dados->data, sizeof(char), 10, binario);
            fread(&dados->quantidadeLugares, sizeof(int), 1, binario);
            fseek(binario, sizeof(int), SEEK_CUR);
            fread(&dados->tamanhoModelo, sizeof(int), 1, binario);
            fread(dados->modelo, sizeof(char), dados->tamanhoModelo, binario);
            dados->modelo[dados->tamanhoModelo] = '\0';
            fread(&dados->tamanhoCategoria, sizeof(int), 1, binario);
            fread(dados->categoria, sizeof(char), dados->tamanhoCategoria, binario);
            dados->categoria[dados->tamanhoCategoria] = '\0';
            printf("Prefixo do veiculo: %s\n", dados->prefixo);
            printf("Modelo do veiculo: %s\n", dados->modelo);
            printf("Categoria do veiculo: %s\n", dados->categoria);
            printf("Data de entrada do veiculo na frota: %s\n", dados->data);
            printf("Quantidade de lugares sentados disponiveis: %d\n", dados->quantidadeLugares);
            printf("\n");
        } else{
            if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                return;
            }
        }
    }

    fclose(binario);

    return;
}

int main(){

    int funcao;

    scanf("%d ", &funcao);

    switch(funcao){
        case 1:
            primeiro_caso();
            break;
        case 2:
            segundo_caso();
            break;
        case 3:
            terceiro_caso();
            break;
        
    }

    return 0;
}