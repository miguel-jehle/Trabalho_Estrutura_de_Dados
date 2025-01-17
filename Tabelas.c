#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          CRIAR TABELA DE NACIONALIDADES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tabela_nacionalidade(char* entrada){
    FILE* fp = fopen(entrada, "r");
    FILE* fs = fopen("Tabelas/Nacionalidades.bin", "wb");
    if(!fp || !fs) exit(1);
    char seleção[20], tmp[200];
    int cont = 0, id;

    while(fscanf(fp,"%s",seleção) == 1){
        long pos = ftell(fp);
        int cont = 0, id;
        while(fscanf(fp, "%d/%199[^\n]",&id,tmp) == 2){
            cont++;
        }
        fwrite(seleção, sizeof(char)*20, 1, fs);
        fwrite(&cont, sizeof(int), 1, fs);
        fseek(fp, pos, SEEK_SET);
        while(fscanf(fp, "%d/%199[^\n]",&id,tmp) == 2){
            fwrite(&id, sizeof(int), 1, fs);
        }
    }
    fclose(fp);
    fclose(fs);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          PRINTAR TABELA DE NACIONALIDADES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printa_nacionalidades(char * entrada){
    FILE* fp = fopen(entrada, "rb");
    if(!fp) exit(1);
    char selecao[20];
    printf("\n\tNacionalidades:\n");

    while(fread(selecao, sizeof(char)*20, 1, fp) == 1){
        int cont;
        fread(&cont,sizeof(int), 1,fp);
        printf("\n\t%s %d ",selecao,cont);
        int id;
        for(int i =0; i<cont; i++){
            fread(&id,sizeof(int), 1,fp);
            printf("%d ",id);
        }
        printf("\n\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          CRIAR TABELA DE POSIÇÕES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tabela_posicoes(char* entrada){
    FILE* fp = fopen(entrada, "r");
    FILE* fs = fopen("Tabelas/Posições.bin", "wb");
    if(!fp || !fs) exit(1);
    char posicao[5], tmp[200], selecao[20];
    int cont_GK = 0, cont_MF = 0, cont_DF = 0, cont_FW = 0, tmp1, tmp2;
    // GK MF DF FW
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(strcmp("GK", posicao) == 0) cont_GK++;
            if(strcmp("MF", posicao) == 0) cont_MF++;
            if(strcmp("DF", posicao) == 0) cont_DF++;
            if(strcmp("FW", posicao) == 0) cont_FW++;
        }
    }
    fwrite("GK", sizeof(char)*5, 1, fs);
    fwrite(&cont_GK, sizeof(int), 1, fs);

    fclose(fp);
    fp = fopen(entrada, "r");
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(strcmp("GK", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }

    }
    fwrite("MF", sizeof(char)*5, 1, fs);
    fwrite(&cont_MF, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(strcmp("MF", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fwrite("DF", sizeof(char)*5, 1, fs);
    fwrite(&cont_DF, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");    
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(strcmp("DF", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fwrite("FW", sizeof(char)*5, 1, fs);
    fwrite(&cont_FW, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");    
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(strcmp("FW", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fclose(fp);
    fclose(fs);
}

void tabela_posicoes_alterar(char* entrada, int id, char* nova_pos){
    FILE* fp = fopen(entrada, "r");
    FILE* fs = fopen("Tabelas/Posições.bin", "wb");
    if(!fp || !fs) exit(1);
    char posicao[5], tmp[200], selecao[20];
    int cont_GK = 0, cont_MF = 0, cont_DF = 0, cont_FW = 0, tmp1, tmp2;
    // GK MF DF FW
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(tmp1 == id) strcpy(posicao,nova_pos);
            if(strcmp("GK", posicao) == 0) cont_GK++;
            if(strcmp("MF", posicao) == 0) cont_MF++;
            if(strcmp("DF", posicao) == 0) cont_DF++;
            if(strcmp("FW", posicao) == 0) cont_FW++;
        }
    }
    fwrite("GK", sizeof(char)*5, 1, fs);
    fwrite(&cont_GK, sizeof(int), 1, fs);

    fclose(fp);
    fp = fopen(entrada, "r");
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(tmp1 == id) strcpy(posicao,nova_pos);
            if(strcmp("GK", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }

    }
    fwrite("MF", sizeof(char)*5, 1, fs);
    fwrite(&cont_MF, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(tmp1 == id) strcpy(posicao,nova_pos);
            if(strcmp("MF", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fwrite("DF", sizeof(char)*5, 1, fs);
    fwrite(&cont_DF, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");    
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(tmp1 == id) strcpy(posicao,nova_pos);
            if(strcmp("DF", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fwrite("FW", sizeof(char)*5, 1, fs);
    fwrite(&cont_FW, sizeof(int), 1, fs);
    fclose(fp);
    fp = fopen(entrada, "r");    
    while((fscanf(fp,"%s", selecao) == 1)){
        while(fscanf(fp, "%d/%d/%2s/%199[^\n]",&tmp1,&tmp2,posicao,tmp) == 4){
            if(tmp1 == id) strcpy(posicao,nova_pos);
            if(strcmp("FW", posicao) == 0){
                fwrite(&tmp1, sizeof(int), 1, fs);
            }
        }
    }
    fclose(fp);
    fclose(fs);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          PRINTAR TABELA DE POSIÇÕES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printa_posicoes(char * entrada){
    FILE* fp = fopen(entrada, "rb");
    if(!fp) exit(1);
    char posicao[5];
    printf("Posições:\n");
    while(fread(posicao, sizeof(char)*5, 1, fp) == 1){
        int cont;
        fread(&cont,sizeof(int), 1,fp);
        printf("\n\t%s %d ",posicao,cont);
        int id;
        for(int i =0; i<cont; i++){
            fread(&id,sizeof(int), 1,fp);
            printf("%d ",id);
        }
        printf("\n\n");
    }
}