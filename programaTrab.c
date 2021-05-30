#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/

//////////////////////////////////////////////////////////

void ler_linha(FILE* arquivo, char* linha){
    fgets(linha, 1000, arquivo);
    return;
}

//////////////////////////////////////////////////////////

void escrever_dados_veiculo(FILE* binario, char* linha, RegVeiculoDados* dados){

    fwrite(&dados->removido, sizeof(char), 1, binario);
    fwrite(&dados->tamanhoRegistro, sizeof(int), 1, binario);
    fwrite(dados->prefixo, sizeof(char), 5, binario);
    fwrite(dados->data, sizeof(char), 10, binario);
    fwrite(&dados->quantidadeLugares, sizeof(int), 1, binario);
    fwrite(&dados->codLinha, sizeof(int), 1, binario);
    fwrite(&dados->tamanhoModelo, sizeof(int), 1, binario);
    fwrite(dados->modelo, sizeof(char), dados->tamanhoModelo, binario);
    fwrite(&dados->tamanhoCategoria, sizeof(int), 1, binario);
    fwrite(dados->categoria, sizeof(char), dados->tamanhoCategoria, binario);

    return;
}

void atribuir_dados_veiculo(RegVeiculoDados* dados, RegVeiculoCab* cabecalho, char* linha){

    char *tok;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(dados->prefixo, tok);

    cabecalho->nroRegistros++;

    //Confiro se o registro esta logicamente removido
    if(dados->prefixo[0] == '*'){
        cabecalho->nroRegRemovidos++;
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

    char nome_csv[20];
    char nome_bin[20];
    char linha[1000];

    scanf("%s ", nome_csv);
    scanf("%[^\n]s", nome_bin);

    FILE* csv = fopen(nome_csv, "r");
    FILE* binario = fopen(nome_bin, "wb");

    ler_linha(csv, linha);

    atribuir_cabecalho_veiculo(cabecalho, linha);

    escrever_cabecalho_veiculo(binario, linha, cabecalho);

    while(1){
        linha[0] = '\0';
        ler_linha(csv, linha);

        if(strcmp(linha, "\0") == 0){
            break;
        }
        atribuir_dados_veiculo(dados, cabecalho, linha);
        escrever_dados_veiculo(binario, linha, dados);
    }

    cabecalho->status = '1';
    cabecalho->byteProxReg = ftell(binario);

    fseek(binario, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->byteProxReg, sizeof(long long), 1, binario);
    fwrite(&cabecalho->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(int), 1, binario);

    binarioNaTela(nome_bin);

    fclose(csv);
    fclose(binario);

    return;
}

//////////////////////////////////////////////////////////

void escrever_dados_linha(FILE* binario, char* linha, RegLinhaDados* dados){

    fwrite(&dados->removido, sizeof(char), 1, binario);
    fwrite(&dados->tamanhoRegistro, sizeof(int), 1, binario);
    fwrite(&dados->codLinha, sizeof(int), 1, binario);
    fwrite(dados->aceitaCartao, sizeof(char), 1, binario);
    fwrite(&dados->tamanhoNome, sizeof(int), 1, binario);
    fwrite(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
    fwrite(&dados->tamanhoCor, sizeof(int), 1, binario);
    fwrite(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);

    return;
}

void atribuir_dados_linha(RegLinhaDados* dados, RegLinhaCab* cabecalho,char* linha){

    char *tok;

    dados->tamanhoRegistro = 0;

    //Lendo os campos
    tok = strtok(linha, ",");
    strcpy(dados->codLinha, tok);

    cabecalho->nroRegistros++;

    //Confiro se o registro esta logicamente removido
    if(dados->codLinha[0] == '*'){
        cabecalho->nroRegRemovidos++;
        dados->removido = '0';
    } else{
        dados->removido = '1';
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

    char nome_csv[20];
    char nome_bin[20];
    char linha[1000];

    scanf("%s ", nome_csv);
    scanf("%[^\n]s", nome_bin);

    FILE* csv = fopen(nome_csv, "r");
    FILE* binario = fopen(nome_bin, "wb");

    ler_linha(csv, linha);

    atribuir_cabecalho_linha(cabecalho, linha);

    escrever_cabecalho_linha(binario, linha, cabecalho);

    while(1){
        linha[0] = '\0';
        ler_linha(csv, linha);

        if(strcmp(linha, "\0") == 0){
            break;
        }
        atribuir_dados_linha(dados, cabecalho,linha);
        escrever_dados_linha(binario, linha, dados);
    }

    cabecalho->status = '1';
    cabecalho->byteProxReg = ftell(binario);

    fseek(binario, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->byteProxReg, sizeof(long long), 1, binario);
    fwrite(&cabecalho->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(int), 1, binario);

    binarioNaTela(nome_bin);

    fclose(csv);
    fclose(binario);

    return;
}

//////////////////////////////////////////////////////////

void terceiro_caso(){

    RegVeiculoDados* dados = (RegVeiculoDados*) malloc(sizeof(RegVeiculoDados));

    char nome_bin[20];

    dados->tamanhoRegistro = 174;

    scanf("%[^\n]s", nome_bin);

    FILE* binario = fopen(nome_bin, "rb");

    char status;
    fread(&status, sizeof(char), 1, binario);
    if(status == '0'){
        printf("Falha no processamento do arquivo\n");
        return;
    }

    if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
        return;
    }  

    while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
        fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
        if(dados->removido == '1'){
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

void quarto_caso(){

    RegLinhaDados* dados = (RegLinhaDados*) malloc(sizeof(RegLinhaDados));

    char nome_bin[20];

    dados->tamanhoRegistro = 81;

    scanf("%[^\n]s", nome_bin);

    FILE* binario = fopen(nome_bin, "rb");

    char status;
    fread(&status, sizeof(char), 1, binario);
    if(status == '0'){
        printf("Falha no processamento do arquivo\n");
        return;
    }

    if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
        return;
    }  

    while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
        fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
        if(dados->removido == '1'){
            fread(&dados->codLinha, sizeof(int), 1, binario);

            fread(dados->aceitaCartao, sizeof(char), 1, binario);
            fread(&dados->tamanhoNome, sizeof(int), 1, binario);
            fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
            dados->nomeLinha[dados->tamanhoNome] = '\0';
            fread(&dados->tamanhoCor, sizeof(int), 1, binario);
            fread(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);
            dados->corLinha[dados->tamanhoCor] = '\0';
            printf("Codigo da linha: %s\n", dados->codLinha);
            printf("Nome da linha: %s\n", dados->nomeLinha);
            printf("Cor que descreve a linha: %s\n", dados->corLinha);
            printf("Aceita cartao: ");
            if(strcmp(dados->aceitaCartao, "S") == 0){
                printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
            }
            if(strcmp(dados->aceitaCartao, "N") == 0){
                printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
            }
            if(strcmp(dados->aceitaCartao, "F") == 0){
                printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
            }
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

int quinto_caso(){

    RegVeiculoDados* dados = (RegVeiculoDados*) malloc(sizeof(RegVeiculoDados));

    dados->tamanhoRegistro = 175;

    char nome_bin[20];
    char nomeDoCampo[20];
    char chave[100];

    int contagem = 0;

    scanf("%s ", nome_bin);
    scanf("%s ", nomeDoCampo);
    scanf("%[^\n]s", chave);

    FILE* binario = fopen(nome_bin, "rb");

    char status;
    fread(&status, sizeof(char), 1, binario);
    if(status == '0'){
        printf("Falha no processamento do arquivo\n");
        return 1;
    }

    if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
        return contagem;
    } 

    if(strcmp(nomeDoCampo, "prefixo") == 0){

        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(dados->prefixo, sizeof(char), 5, binario);
                if(strcmp(dados->prefixo, chave) == 0){
                    contagem = 1;
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
                    if(fseek(binario, dados->tamanhoRegistro - 5, SEEK_CUR) != 0){
                        return contagem;
                    }
                }            
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            } 
        }
    }

    if(strcmp(nomeDoCampo, "data") == 0){

        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(dados->prefixo, sizeof(char), 5, binario);
                fread(dados->data, sizeof(char), 10, binario);
                if(strcmp(dados->data, chave) == 0){
                    contagem = 1;
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
                    if(fseek(binario, dados->tamanhoRegistro - 5 - 10, SEEK_CUR) != 0){
                        return contagem;
                    }
                }            
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            } 
        }
    }

    if(strcmp(nomeDoCampo, "quantidadeLugares") == 0){

        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(dados->prefixo, sizeof(char), 5, binario);
                fread(dados->data, sizeof(char), 10, binario);
                fread(&dados->quantidadeLugares, sizeof(int), 1, binario);
                if(dados->quantidadeLugares == atoi(chave)){
                    contagem = 1;
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
                    if(fseek(binario, dados->tamanhoRegistro - 5 - 10 - 4, SEEK_CUR) != 0){
                        return contagem;
                    }
                }            
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            } 
        }
    }

    if(strcmp(nomeDoCampo, "modelo") == 0){

        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(dados->prefixo, sizeof(char), 5, binario);
                fread(dados->data, sizeof(char), 10, binario);
                fread(&dados->quantidadeLugares, sizeof(int), 1, binario);
                fseek(binario, sizeof(int), SEEK_CUR);
                fread(&dados->tamanhoModelo, sizeof(int), 1, binario);
                fread(dados->modelo, sizeof(char), dados->tamanhoModelo, binario);
                dados->modelo[dados->tamanhoModelo] = '\0';
                if(strcmp(dados->modelo, chave) == 0){
                    contagem = 1;
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
                    if(fseek(binario, dados->tamanhoRegistro - 5 - 10 - 4 - 4 - 4 - dados->tamanhoModelo, SEEK_CUR) != 0){
                        return contagem;
                    }
                }            
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            } 
        }
    }
    
    if(strcmp(nomeDoCampo, "categoria") == 0){

        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
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
                if(strcmp(dados->categoria, chave) == 0){
                    contagem = 1;
                    printf("Prefixo do veiculo: %s\n", dados->prefixo);
                    printf("Modelo do veiculo: %s\n", dados->modelo);
                    printf("Categoria do veiculo: %s\n", dados->categoria);
                    printf("Data de entrada do veiculo na frota: %s\n", dados->data);
                    printf("Quantidade de lugares sentados disponiveis: %d\n", dados->quantidadeLugares);
                    printf("\n");
                } else{
                    if(fseek(binario, dados->tamanhoRegistro - 5 - 10 - 4 - 4 - 4 - dados->tamanhoModelo - 4 - dados->tamanhoCategoria, SEEK_CUR) != 0){
                        return contagem;
                    }
                }            
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            } 
        }
    }

    fclose(binario);

    return contagem;
}

int sexto_caso(){

    RegLinhaDados* dados = (RegLinhaDados*) malloc(sizeof(RegLinhaDados));

    dados->tamanhoRegistro = 82;

    char nome_bin[20];
    char nomeDoCampo[20];
    char chave[100];
    
    int contagem = 0;

    scanf("%s ", nome_bin);
    scanf("%s ", nomeDoCampo);
    scanf("%[^\n]s", chave);

    FILE* binario = fopen(nome_bin, "rb");

    char status;
    fread(&status, sizeof(char), 1, binario);
    if(status == '0'){
        printf("Falha no processamento do arquivo\n");
        return 1;
    }

    if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
        return contagem;
    }

    if(strcmp(nomeDoCampo, "codLinha") == 0){
        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(&dados->codLinha, sizeof(int), 1, binario);
                if(strcmp(dados->codLinha, chave) == 0){
                    contagem = 1;
                    fread(dados->aceitaCartao, sizeof(char), 1, binario);
                    fread(&dados->tamanhoNome, sizeof(int), 1, binario);
                    fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
                    dados->nomeLinha[dados->tamanhoNome] = '\0';
                    fread(&dados->tamanhoCor, sizeof(int), 1, binario);
                    fread(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);
                    dados->corLinha[dados->tamanhoCor] = '\0';
                    printf("Codigo da linha: %s\n", dados->codLinha);
                    printf("Nome da linha: %s\n", dados->nomeLinha);
                    printf("Cor que descreve a linha: %s\n", dados->corLinha);
                    printf("Aceita cartao: ");
                    if(strcmp(dados->aceitaCartao, "S") == 0){
                        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
                    }
                    if(strcmp(dados->aceitaCartao, "N") == 0){
                        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
                    }
                    if(strcmp(dados->aceitaCartao, "F") == 0){
                        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
                    }
                    printf("\n");
                } else{
                    if(fseek(binario, dados->tamanhoRegistro - 4, SEEK_CUR) != 0){
                        return contagem;
                    }
                }
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            }
        }
    }

    if(strcmp(nomeDoCampo, "aceitaCartao") == 0){
        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(&dados->codLinha, sizeof(int), 1, binario);
                fread(dados->aceitaCartao, sizeof(char), 1, binario);
                if(strcmp(dados->aceitaCartao, chave) == 0){
                    contagem = 1;
                    fread(&dados->tamanhoNome, sizeof(int), 1, binario);
                    fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
                    dados->nomeLinha[dados->tamanhoNome] = '\0';
                    fread(&dados->tamanhoCor, sizeof(int), 1, binario);
                    fread(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);
                    dados->corLinha[dados->tamanhoCor] = '\0';
                    printf("Codigo da linha: %s\n", dados->codLinha);
                    printf("Nome da linha: %s\n", dados->nomeLinha);
                    printf("Cor que descreve a linha: %s\n", dados->corLinha);
                    printf("Aceita cartao: ");
                    if(strcmp(dados->aceitaCartao, "S") == 0){
                        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
                    }
                    if(strcmp(dados->aceitaCartao, "N") == 0){
                        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
                    }
                    if(strcmp(dados->aceitaCartao, "F") == 0){
                        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
                    }
                    printf("\n");
                } else{
                    if(fseek(binario, dados->tamanhoRegistro - 4 - 1, SEEK_CUR) != 0){
                        return contagem;
                    }
                }
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            }
        }
    }
    
    if(strcmp(nomeDoCampo, "nomeLinha") == 0){
        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(&dados->codLinha, sizeof(int), 1, binario);
                fread(dados->aceitaCartao, sizeof(char), 1, binario);
                fread(&dados->tamanhoNome, sizeof(int), 1, binario);
                fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
                dados->nomeLinha[dados->tamanhoNome] = '\0';
                if(strcmp(dados->nomeLinha, chave) == 0){
                    contagem = 1;
                    fread(&dados->tamanhoCor, sizeof(int), 1, binario);
                    fread(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);
                    dados->corLinha[dados->tamanhoCor] = '\0';
                    printf("Codigo da linha: %s\n", dados->codLinha);
                    printf("Nome da linha: %s\n", dados->nomeLinha);
                    printf("Cor que descreve a linha: %s\n", dados->corLinha);
                    printf("Aceita cartao: ");
                    if(strcmp(dados->aceitaCartao, "S") == 0){
                        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
                    }
                    if(strcmp(dados->aceitaCartao, "N") == 0){
                        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
                    }
                    if(strcmp(dados->aceitaCartao, "F") == 0){
                        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
                    }
                    printf("\n");
                } else{
                    if(fseek(binario, dados->tamanhoRegistro - 4 - 1 - 4 - dados->tamanhoNome, SEEK_CUR) != 0){
                        return contagem;
                    }
                }
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            }
        }
    }

    if(strcmp(nomeDoCampo, "corLinha") == 0){
        while(fread(&dados->removido, sizeof(char), 1, binario) == 1){
            fread(&dados->tamanhoRegistro, sizeof(int), 1, binario);
            if(dados->removido == '1'){
                fread(&dados->codLinha, sizeof(int), 1, binario);
                fread(dados->aceitaCartao, sizeof(char), 1, binario);
                fread(&dados->tamanhoNome, sizeof(int), 1, binario);
                fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, binario);
                dados->nomeLinha[dados->tamanhoNome] = '\0';
                fread(&dados->tamanhoCor, sizeof(int), 1, binario);
                fread(dados->corLinha, sizeof(char), dados->tamanhoCor, binario);
                dados->corLinha[dados->tamanhoCor] = '\0';
                if(strcmp(dados->corLinha, chave) == 0){
                    contagem = 1;
                    printf("Codigo da linha: %s\n", dados->codLinha);
                    printf("Nome da linha: %s\n", dados->nomeLinha);
                    printf("Cor que descreve a linha: %s\n", dados->corLinha);
                    printf("Aceita cartao: ");
                    if(strcmp(dados->aceitaCartao, "S") == 0){
                        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
                    }
                    if(strcmp(dados->aceitaCartao, "N") == 0){
                        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
                    }
                    if(strcmp(dados->aceitaCartao, "F") == 0){
                        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
                    }
                    printf("\n");
                } else{
                    if(fseek(binario, dados->tamanhoRegistro - 4 - 1 - 4 - dados->tamanhoNome - 4 - dados->tamanhoCor, SEEK_CUR) != 0){
                        return contagem;
                    }
                }
            } else{
                if(fseek(binario, dados->tamanhoRegistro, SEEK_CUR) != 0){
                    return contagem;
                }
            }
        }
    }

    fclose(binario);

    return contagem;
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
        case 4:
            quarto_caso();
            break;
        case 5:
            if(quinto_caso() == 0){
                printf("Registro inexistente\n");
            }
            break;
        case 6:
            if(sexto_caso() == 0){
                printf("Registro Inexistente\n");
            }
            break;
    }

    return 0;
}