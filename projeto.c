#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------FUNÇÃO QUE RETORNA O TAMANHO DO REGISTRO A SER LIDO----------*/
int pegar_tamanho_reg(FILE *fd, char *registro)
{
    char byte;

    if (!fread(&byte, sizeof(int), 1, fd))
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

    struct remover
    {
        char id_aluno[4];
        char sigla_disc[4];
    } remove;

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

    FILE *re;
    if (!(re = fopen("remove.bin", "r+b")))
    {
        printf("\nNao foi possivel  abrir o arquivo remove.bin");
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
            byte_in_aux = 0;
            fwrite(&byte_in_aux, sizeof(int), 1, in_aux);
        }
    }
    rewind(in_aux);

    /*----------CRIAÇÃO DO ARQUIVO QUE ARMAZENA O BYTE A SER LIDO NO ARQUIVO REMOVE.BIN----------*/
    FILE *re_aux;
    int byte_re_aux;

    re_aux = fopen("re_aux.bin", "r+b");

    // Se o arquivo não existir, cria-o com "w+b"
    if (re_aux == NULL)
    {
        // Arquivo ainda não existe, tem que ser criado com w+b
        re_aux = fopen("re_aux.bin", "w+b");
        if (re_aux == NULL)
        {
            printf("\nNao foi possivel criar o arquivo auxiliar de remocao");
            return 0;
        }
        // Como o arquivo é novo, consideramos que é a primeira inserção
        byte_re_aux = 0;
        fwrite(&byte_re_aux, sizeof(int), 1, re_aux);
    }
    else
    {
        // Verifica se o arquivo está vazio
        fseek(re_aux, 0, SEEK_END);
        long tam_re_aux = ftell(re_aux);

        if (tam_re_aux == 0)
        {
            printf("\nPrimeira remocao no arquivo re_aux.bin");
            byte_re_aux = 0;
            fwrite(&byte_re_aux, sizeof(int), 1, re_aux);
        }
    }
    rewind(re_aux);

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
            // Verifica se o cabeçalho é igual a -1, para adicionar no último byte do arquivo out.bin
            if (cabecalho == -1)
            {
                fseek(out, 0, 2);
                fwrite(&tam_reg, sizeof(int), 1, out);
                fwrite(registro, sizeof(char), tam_reg, out);
            }
            else
            {
            }

            // Atualiza o arquivo in_aux
            rewind(in_aux);
            byte_in_aux += 116;
            fwrite(&byte_in_aux, sizeof(int), 1, in_aux);
            rewind(in_aux);
        }
        if (opcao == 2)
        {
            rewind(out);
            fread(&cabecalho, sizeof(int), 1, out);

            char pegar_chave[20];
            fread(&byte_re_aux, sizeof(int), 1, re_aux);
            fseek(re, byte_re_aux, 0);
            fread(&remove, sizeof(remove), 1, re);

            sprintf(pegar_chave, "%s%s", remove.id_aluno, remove.sigla_disc);

            tam_reg = pegar_tamanho_reg(out, registro);
            char *ptrchar;
            int offset_byte = ftell(out);

            int chave_encontrada = 0; //Flag para ver se a chave foi encontrada ou não

            while (tam_reg > 0)
            {
                char reg_aux[120];
                char registro_copy[120];         // Cria uma cópia para o strtok
                strcpy(registro_copy, registro); // Copia o conteúdo de registro

                reg_aux[0] = '\0';
                ptrchar = strtok(registro_copy, "#");

                while (ptrchar != NULL)
                {
                    strcat(reg_aux, ptrchar);
                    ptrchar = strtok(NULL, "#");
                }

                if (strstr(reg_aux, pegar_chave) != NULL)
                {
                    printf("\nRegistro que sera removido: %s", reg_aux);
                    int tamanho_bytes_registro = tam_reg;
                    char *estrela = "*";
                    int offset_proximo_registro = cabecalho;

                    fseek(out, offset_byte, 0);
                    cabecalho = offset_byte;

                    fwrite(&tamanho_bytes_registro, sizeof(int), 1, out);
                    fwrite(estrela, sizeof(char), 1, out);
                    fwrite(&offset_proximo_registro, sizeof(int), 1, out);

                    // Volta para o início para escrever o cabeçalho
                    fseek(out, 0, SEEK_SET);
                    fwrite(&offset_byte, sizeof(int), 1, out);
                    chave_encontrada = 1;
                    break;
                }

                // Avança para o próximo registro
                
                tam_reg = pegar_tamanho_reg(out, registro);
                offset_byte = ftell(out) - tam_reg - sizeof(int);
            }

            if(chave_encontrada == 0)
            {
                printf("\nA chave nao foi encontrada e a proxima remocao acontecera com a chave seguinte a esta.");
            }

            // Atualiza o arquivo re_aux
            rewind(re_aux);
            byte_re_aux += 8;
            fwrite(&byte_re_aux, sizeof(int), 1, re_aux);
            rewind(re_aux);
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
