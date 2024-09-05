#include<stdio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct hist {
        char id_aluno[4];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    } vet[12] = {{"001", "AAA", "Alindo Pereira", "AAAAA", 7.5, 72.1},
                {"002", "BBB", "Brendo Mota Gomes", "BBBBB", 8.2, 80.2},
                {"003", "CCC", "Caio Ribeiro", "CCCCC", 5.4, 79.3},
                
                {"004", "DDD", "Danilo Santos Alves", "DDDDD", 6.8, 91.4},
                {"005", "EEE", "Eraldo Joaquim", "EEEEE", 7.3, 82.5},
                
                {"006", "FFF", "Fernando Montinegro", "FFFFF", 9.5, 92.6},
                {"007", "GGG", "Gustavo Henrique Sant", "GGGGG", 9.5, 92.7},
                {"008", "HHH", "Hugo Guimaraes", "HHHHH", 9.5, 92.8},
                {"009", "III", "Ivanildo Suarez Junior", "IIIII", 9.5, 92.9},
                {"010", "JJJ", "Joao Ribeiro Pedro", "JJJJJ", 9.5, 91.0},
                {"011", "KKK", "Karina Silva", "KKKKK", 9.5, 91.1},
                {"012", "LLL", "Leonardo Milanez", "LLLLL", 9.5, 91.2}};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct remove {
        char id_aluno[4];
        char sigla_disc[4];
    } vet_r[5] = {{"003","CCC"},
                  {"005","EEE"},
                  {"010","JJJ"},
                  {"001","AAA"},
                  {"012","LLL"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

