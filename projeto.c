#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------FUNÇÃO QUE RETORNA O TAMANHO DO REGISTRO A SER LIDO----------*/
int pegar_tamanho_reg(FILE *fd, char *registro)
{
    char byte;

    if (!fread(&byte, sizeof(char), 1, fd))
    {
        return 0;
    }
    else
    {
        fread(registro, byte, 1, fd);
        registro[byte] = '\0';
        return byte;
    }
}

int main()
{
    /*----------ESTRUTURA DE CADA CAMPO NO ARQUIVO A SER LIDO----------*/
    struct historico
    {
        char id_aluno[4];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    } hist;

    char registro[120];
    int tam_reg;
    int cabecalho;

    /*--------CRIAÇÃO DOS PONTEIROS DOS ARQUIVOS---------*/

    // Ponteiros para obter informações dos arquivos insere.bin e remove.bin

    FILE *in;
    if (!(in = fopen("insere.bin", "r+b")))
    {
        printf("\nNao foi possivel abrir o arquivo de insersao");
        return 0;
    }

    FILE *out = fopen("out.bin", "r+b"); // Estamos usando r+b pois ela não permite truncamento (não zera o tamanho)
    if (out == NULL)
    {
        // Arquivo ainda não existe, tem que ser criado com w+b
        out = fopen("out.bin", "w+b");
        if (out == NULL)
        {
            printf("\nNao foi possivel criar o arquivo auxiliar de insersao");
            return 0;
        }
        cabecalho = -1;
        fwrite(&cabecalho, sizeof(int), 1, out);
    }

    /*----------CRIAÇÃO DO ARQUIVO QUE ARMAZENA O BYTE A SER LIDO NO ARQUIVO INSERE.BIN----------*/
    FILE *in_aux;
    int byte_in_aux;

    in_aux = fopen("in_aux.bin", "r+b");

    // Se o arquivo não existir, cria-o com "w+b"
    if (in_aux == NULL)
    {
        // Arquivo ainda não existe, tem que ser criado com w+b
        in_aux = fopen("in_aux.bin", "w+b");
        if (in_aux == NULL)
        {
            printf("\nNao foi possivel criar o arquivo auxiliar de insersao");
            return 0;
        }
        // Como o arquivo é novo, consideramos que é a primeira inserção
        byte_in_aux = 0;
        fwrite(&byte_in_aux, sizeof(int), 1, in_aux);
    }
    else
    {
        // Verifica se o arquivo está vazio
        fseek(in_aux, 0, SEEK_END);
        long tam_in_aux = ftell(in_aux);

        if (tam_in_aux == 0)
        {
            printf("\nPrimeira insercao no arquivo in_aux.bin");
            byte_in_aux = 0;
            fwrite(&byte_in_aux, sizeof(int), 1, in_aux);
        }
    }
    rewind(in_aux);

    /*-----------CRIAÇÃO DE UM ARQUIVO QUE GUARDA O ÚLTIMO BYTE DE OUT.BIN------------*/
    FILE *out_aux;
    int byte_out_aux;

    out_aux = fopen("out_aux.bin", "r+b");

    // Se o arquivo não existir, cria-o com "w+b"
    if (out_aux == NULL)
    {
        // Arquivo ainda não existe, tem que ser criado com w+b
        out_aux = fopen("out_aux.bin", "w+b");
        if (out_aux == NULL)
        {
            printf("\nNao foi possivel criar o arquivo auxiliar de insersao");
            return 0;
        }
        // Como o arquivo é novo, consideramos que é a primeira inserção
        byte_out_aux = 4; // Precisa pular os bytes destinados ao cabeçalho
        fwrite(&byte_out_aux, sizeof(int), 1, out_aux);
    }
    else
    {
        // Verifica se o arquivo está vazio
        fseek(out, 0, 2);
        long tam_out = ftell(out);

        rewind(out_aux);
        fwrite(&tam_out, sizeof(int), 1, out_aux);
    }
    rewind(out_aux);
    rewind(out);

    /*----------MENU PARA INTERAÇÃO COM O PROGRAMA----------*/
    printf("\n----------MENU----------");
    printf("\n1. Inserir");
    printf("\n2. Remover");
    printf("\n3. Compactar");
    printf("\n4. Sair do Programa");
    int opcao = -1;

    while (opcao != 4)
    {
        printf("\nINFORME SUA OPCAO: ");
        scanf("%d", &opcao);
        while (opcao < 1 || opcao > 4)
        {
            printf("\nOPCAO INFORMADA NAO EXISTE, INFORME SUA OPCAO NOVAMENTE: ");
            scanf("%d", &opcao);
        }

        /*----------OPERAÇÃO DE INSERIR NO ARQUIVO OUT.BIN----------*/
        if (opcao == 1)
        {
            // Obtem o byte a ser lido no arquivo insere.bin
            fread(&byte_in_aux, sizeof(int), 1, in_aux);
            printf("\nByte que sera lido no arquivo insere.bin: %d", byte_in_aux);

            // Faz a leitura do arquivo insere.bin e formata o conteúdo para o arquivo out.bin
            fseek(in, byte_in_aux, 0);
            fread(&hist, sizeof(hist), 1, in);
            sprintf(registro, "%s#%s#%s#%s#%.2f#%.2f", hist.id_aluno, hist.sigla_disc, hist.nome_aluno, hist.nome_disc, hist.media, hist.freq);
            printf("\n%s", registro);
            tam_reg = strlen(registro);
            tam_reg++;
            registro[tam_reg] = '\0';
            printf("\n%d", tam_reg);

            rewind(out);
            fread(&cabecalho, sizeof(int), 1, out);
            //Verifica se o cabeçalho é igual a menos 1
            if (cabecalho == -1)
            {
                fread(&byte_out_aux, sizeof(int), 1, out_aux);
                fseek(out, byte_out_aux, 0);
                printf("\nByte que vai inserir no out.bin : %d", byte_out_aux);
                fwrite(&tam_reg, sizeof(int), 1, out);
                fwrite(registro, sizeof(char), tam_reg, out);
            }

            // Atualiza o arquivo in_aux
            rewind(in_aux);
            byte_in_aux += 116;
            fwrite(&byte_in_aux, sizeof(int), 1, in_aux);
            rewind(in_aux);

            //Atualiza o out_aux
            rewind(out_aux);
            fseek(out, 0, 2);
            long tam_out = ftell(out);
            rewind(out_aux);
            fwrite(&tam_out, sizeof(int), 1, out_aux);
            printf("\nByte que vai inserir no out.bin : %d", tam_out);
            rewind(out);
            rewind(out_aux);
        }
        if (opcao == 4)
        {
            break;
        }

        opcao = -1;
    }
    printf("\n----------PROGRAMA FINALIZADO----------");
    fclose(in);
    fclose(out);
    fclose(in_aux);
    return 0;
}
