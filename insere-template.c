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
    } vet[6] = {{"001", "AAA", "Joao da Silva", "AAAA", 7.5, 72.3},
                {"001", "BBB", "Joao da Silva", "BBB", 8.2, 80.4},
                {"001", "CCC", "Joao da Silva", "CCCCC", 5.4, 79.7},
                
                {"001", "CCC", "Paulo Souza", "CCCCC", 6.8, 91.3},
                {"001", "HHH", "Paulo Souza", "HHH", 7.3, 82.7},
                
                {"002", "HHH", "Pedro Silva Silva", "HHH", 9.5, 92.7}};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct remove {
        char id_aluno[4];
        char sigla_disc[4];
    } vet_r[4] = {{"001","CCC"},
                  {"001","HHH"},
                  {"002","HHH"},
                  {"001","AAA"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

