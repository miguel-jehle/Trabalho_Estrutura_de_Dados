#include "TABM.c"
#include <limits.h>

void remove_tabela(char* tabela, int id){
    FILE* fp = fopen(tabela,"rb+");
    if(!fp) exit(1);
    int id_atual, qtd, tmp = 0;
    char sele[20];

    while(fread(&sele,sizeof(char)*20,1,fp)){
        fread(&qtd,sizeof(int),1,fp);
        tmp = 0;
        while(qtd != tmp && fread(&id_atual,sizeof(int),1,fp)){
            if(id == id_atual){
                fseek(fp,-sizeof(int),SEEK_CUR);
                int e = -1;
                fwrite(&e,sizeof(int),1,fp);
                fclose(fp);
                return;
            }
            tmp++;
        }
    }
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (1.1) Retorno do jogador mais novo e mais velho da competição geral;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int month2num(char* mes){
    if(!strcmp(mes,"January")) return 1;
    if(!strcmp(mes,"February")) return 2;
    if(!strcmp(mes,"March")) return 3;
    if(!strcmp(mes,"April")) return 4;
    if(!strcmp(mes,"May")) return 5;
    if(!strcmp(mes,"June")) return 6;
    if(!strcmp(mes,"July")) return 7;
    if(!strcmp(mes,"August")) return 8;
    if(!strcmp(mes,"September")) return 9;
    if(!strcmp(mes,"October")) return 10;
    if(!strcmp(mes,"November")) return 11;
    if(!strcmp(mes,"December")) return 12;
    if(!strcmp(mes,"MIN")) return INT_MIN;
    if(!strcmp(mes,"MAX")) return INT_MAX;
}

int compara_TJ(const void* a, const void* b){
    TJ* pa = (TJ*)a;
    TJ* pb = (TJ*)b;

    return strcmp((pa)->nome,(pb)->nome);
}

void Q1_geral_percorre(char*arv,TJ* jogadorV, TJ* jogadorN,int* contV,int* contN){
    FILE* fp = fopen(arv,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q1_geral_percorre(no.filhos[0],jogadorV,jogadorN,contV,contN);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if((no.chaves[i].ano == jogadorV->ano) && (no.chaves[i].dia == jogadorV->dia) && strcmp(no.chaves[i].mes,jogadorV->mes) == 0) (*contV)++;
                else{
                    if(no.chaves[i].ano < jogadorV->ano) {
                        *jogadorV = copia_chaves(*jogadorV,no.chaves[i]);
                        (*contV) = 1;
                    } 
                    if(no.chaves[i].ano == jogadorV->ano){
                        if(month2num(no.chaves[i].mes) < month2num(jogadorV->mes)) {
                            *jogadorV = copia_chaves(*jogadorV,no.chaves[i]); 
                            (*contV) = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorV->mes)){
                            if(no.chaves[i].dia < jogadorV->dia) {
                                *jogadorV = copia_chaves(*jogadorV,no.chaves[i]); 
                                (*contV) = 1;
                            } 
                        }
                    }
                }
                if((no.chaves[i].ano == jogadorN->ano) && (no.chaves[i].dia == jogadorN->dia) && strcmp(no.chaves[i].mes,jogadorN->mes) == 0) (*contN)++;
                else{
                    if(no.chaves[i].ano > jogadorN->ano) {
                        *jogadorN = copia_chaves(*jogadorN,no.chaves[i]);
                        (*contN) = 1;
                        } 
                    if(no.chaves[i].ano == jogadorN->ano){
                        if(month2num(no.chaves[i].mes) > month2num(jogadorN->mes)) {
                            *jogadorN = copia_chaves(*jogadorN,no.chaves[i]);
                            (*contN) = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorN->mes)){
                            if(no.chaves[i].dia > jogadorN->dia) {
                                *jogadorN = copia_chaves(*jogadorN,no.chaves[i]);
                                (*contN) = 1;
                            } 
                        }
                    }
                }
            }
            Q1_geral_percorre(no.prox,jogadorV,jogadorN,contV,contN);
        }
    }
    return;
}

void Q1_geral_preenche(char * arv, TJ jogadorV, TJ jogadorN, int * indiceV, int * indiceN, TJ * vetV, TJ * vetN,int contV, int contN){
    FILE* fp = fopen(arv,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q1_geral_preenche(no.filhos[0], jogadorV, jogadorN, indiceV, indiceN, vetV, vetN,contV,contN);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if((contV > 1) && (no.chaves[i].ano == jogadorV.ano) && (no.chaves[i].dia == jogadorV.dia) && strcmp(no.chaves[i].mes,jogadorV.mes) == 0){
                    vetV[(*indiceV)] = copia_chaves(vetV[(*indiceV)],no.chaves[i]);
                    (*indiceV)++;
                }
                if((contN > 1) && (no.chaves[i].ano == jogadorN.ano) && (strcmp(no.chaves[i].mes,jogadorN.mes) == 0) && (no.chaves[i].dia == jogadorN.dia)){
                    vetN[(*indiceN)] = copia_chaves(vetN[(*indiceN)],no.chaves[i]);
                    (*indiceN)++;
                }
            }
            Q1_geral_preenche(no.prox, jogadorV, jogadorN, indiceV, indiceN, vetV, vetN,contV,contN);
        }
    }
    return;
}

void Q1_geral(char* arv){
    FILE* fp = fopen(arv,"rb");
    if(!fp){
        printf("\n\n\tPREZADO USUARIO: Todos os jogadores foram excluidos!");
        return;
    }
    TJ velho, novo, *vetV, *vetN; int contV = 0, contN = 0;
    velho.dia = INT_MAX;
    strcpy(velho.mes, "MAX");
    velho.ano = INT_MAX;
    novo.dia = INT_MIN;
    strcpy(novo.mes, "MIN");
    novo.ano = INT_MIN;
    Q1_geral_percorre(arv,&velho, &novo, &contV, &contN);

    if(contV == 1) printf("\n\tMAIS VELHO DA COMPETIÇÃO: %s (%s) NASCIDO EM %d/%d/%d: \n",velho.nome,velho.sele,velho.dia,month2num(velho.mes),velho.ano);
    if(contN == 1) printf("\n\tMAIS NOVO DA COMPETIÇÃO: %s (%s): NASCIDO EM %d/%d/%d: \n",novo.nome,novo.sele,novo.dia,month2num(novo.mes),novo.ano);

    int indiceV = 0;
    int indiceN = 0;
    if(contV > 1 || contN > 1){
        if(contV > 1) vetV = (TJ*)malloc(sizeof(TJ)*contV);
        if(contN > 1) vetN = (TJ*)malloc(sizeof(TJ)*contN);

        Q1_geral_preenche(arv, velho, novo, &indiceV, &indiceN, vetV, vetN, contV, contN);
        
    }

    if(contV > 1)qsort(vetV, contV, sizeof(TJ), compara_TJ);
    if(contN > 1)qsort(vetN, contN, sizeof(TJ), compara_TJ);

    if(contV > 1){
        printf("\n\tJOGADORES MAIS VELHOS EMPATADOS NA DATA %d/%d/%d: ", velho.dia,month2num(velho.mes),velho.ano);
        for(int i=0; i<contV; i++) printf("\n\t%s (%s)",vetV[i].nome,vetV[i].sele);
        free(vetV);
    }
    
    if(contN > 1){
        printf("\n\tJOGADORES MAIS NOVOS EMPATADOS NA DATA %d/%d/%d: ", novo.dia,month2num(novo.mes),novo.ano);
        for(int i=0; i<contN; i++) printf("\n\t%s (%s)",vetN[i].nome, vetN[i].sele);
        free(vetN);
    }
    printf("\n");
    
    return;


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (1.2) Retorno do jogador mais novo e mais velho de cada seleção;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q1_nacionalidade(char* tabela, char * arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) return ;
    int qtd, num, contV, contN, tmp, indiceV, indiceN;
    long pos;
    char sele[21], aux[25];
    TJ jogadorV, jogadorN, *vetV, *vetN;
    TABM no;
    jogadorV.dia = INT_MAX;
    strcpy(jogadorV.mes, "MAX");
    jogadorV.ano = INT_MAX;
    jogadorN.dia = INT_MIN;
    strcpy(jogadorN.mes, "MIN");
    jogadorN.ano = INT_MIN;

    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        contV = 0;
        contN = 0;
        pos = ftell(fp); //posição antes de ler a qtd da seleção
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        printf("\n\t%s: ",sele);
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&no,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; no.chaves[i].id != num; i++);//achei o jogador
                if((no.chaves[i].ano == jogadorV.ano) && (no.chaves[i].dia == jogadorV.dia) && strcmp(no.chaves[i].mes,jogadorV.mes) == 0) contV++;
                else{
                    if(no.chaves[i].ano < jogadorV.ano) {
                        jogadorV = copia_chaves(jogadorV,no.chaves[i]);
                        contV = 1;
                    } 
                    if(no.chaves[i].ano == jogadorV.ano){
                        if(month2num(no.chaves[i].mes) < month2num(jogadorV.mes)) {
                            jogadorV = copia_chaves(jogadorV,no.chaves[i]); 
                            contV = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorV.mes)){
                            if(no.chaves[i].dia < jogadorV.dia) {
                                jogadorV = copia_chaves(jogadorV,no.chaves[i]); 
                                contV = 1;
                            } 
                        }
                    }
                }
                if((no.chaves[i].ano == jogadorN.ano) && (no.chaves[i].dia == jogadorN.dia) && strcmp(no.chaves[i].mes,jogadorN.mes) == 0) contN++;
                else{
                    if(no.chaves[i].ano > jogadorN.ano) {
                        jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                        contN = 1;
                        } 
                    if(no.chaves[i].ano == jogadorN.ano){
                        if(month2num(no.chaves[i].mes) > month2num(jogadorN.mes)) {
                            jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                            contN = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorN.mes)){
                            if(no.chaves[i].dia > jogadorN.dia) {
                                jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                                contN = 1;
                            } 
                        }
                    }
                }
            }
            tmp++;
        }
        //acabei uma seleção
        if(contV == 1) printf("\n\tMAIS VELHO: %s  NASCIDO EM %d/%d/%d: \n",jogadorV.nome,jogadorV.dia,month2num(jogadorV.mes),jogadorV.ano);
        if(contN == 1) printf("\n\tMAIS NOVO: %s  NASCIDO EM %d/%d/%d: \n",jogadorN.nome,jogadorN.dia,month2num(jogadorN.mes),jogadorN.ano);
        
        indiceV = 0;
        indiceN = 0;
        if(contV > 1 || contN > 1){
            if(contV > 1) vetV = (TJ*)malloc(sizeof(TJ)*contV);
            if(contN > 1) vetN = (TJ*)malloc(sizeof(TJ)*contN);
            fseek(fp, pos, SEEK_SET);
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&no,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; no.chaves[i].id != num; i++);//achei o jogador
                    if((contV > 1) && (no.chaves[i].ano == jogadorV.ano) && (no.chaves[i].dia == jogadorV.dia) && strcmp(no.chaves[i].mes,jogadorV.mes) == 0){
                        vetV[indiceV] = copia_chaves(vetV[indiceV],no.chaves[i]);
                        indiceV++;
                    }
                    if((contN > 1) && (no.chaves[i].ano == jogadorN.ano) && (strcmp(no.chaves[i].mes,jogadorN.mes) == 0) && (no.chaves[i].dia == jogadorN.dia)){
                        vetN[indiceN] = copia_chaves(vetN[indiceN],no.chaves[i]);
                        indiceN++;
                    }
                }
                tmp++;
            }
        }

        if(contV > 1)qsort(vetV, contV, sizeof(TJ), compara_TJ);
        if(contN > 1)qsort(vetN, contN, sizeof(TJ), compara_TJ);

        if(contV > 1){
            printf("\n\tJOGADORES MAIS VELHOS EMPATADOS NA DATA %d/%d/%d: ", jogadorV.dia, month2num(jogadorV.mes),jogadorV.ano);
            for(int i=0; i<contV; i++) printf("\n\t%s",vetV[i].nome);
            printf("\n");
            free(vetV);
        }

        if(contN > 1){
            printf("\n\tJOGADORES MAIS NOVOS EMPATADOS NA DATA %d/%d/%d: ", jogadorN.dia, month2num(jogadorN.mes),jogadorN.ano);
            for(int i=0; i<contN; i++) printf("\n\t%s",vetN[i].nome);
            printf("\n");
            free(vetN);
        }
        printf("\n");
    
        jogadorV.dia = INT_MAX;
        strcpy(jogadorV.mes, "MAX");
        jogadorV.ano = INT_MAX;
        jogadorN.dia = INT_MIN;
        strcpy(jogadorN.mes, "MIN");
        jogadorN.ano = INT_MIN;
        printf("\n");
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (1.3) Retorno do jogador mais novo e mais velho de cada posição;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q1_posicao(char* tabela, char * arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) return ;
    int qtd, num, contV, contN, tmp, indiceV, indiceN;
    long pos;
    char posicao[5], aux[25];
    TJ jogadorV, jogadorN, *vetV, *vetN;
    TABM no;
    jogadorV.dia = INT_MAX;
    strcpy(jogadorV.mes, "MAX");
    jogadorV.ano = INT_MAX;
    jogadorN.dia = INT_MIN;
    strcpy(jogadorN.mes, "MIN");
    jogadorN.ano = INT_MIN;

    while(fread(&posicao, sizeof(char)*5, 1, fp) == 1){
        contV = 0;
        contN = 0;
        pos = ftell(fp); //posição antes de ler a qtd da seleção
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        printf("\n\t%s: ",posicao);
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&no,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; no.chaves[i].id != num; i++);//achei o jogador
                if((no.chaves[i].ano == jogadorV.ano) && (no.chaves[i].dia == jogadorV.dia) && strcmp(no.chaves[i].mes,jogadorV.mes) == 0) contV++;
                else{
                    if(no.chaves[i].ano < jogadorV.ano) {
                        jogadorV = copia_chaves(jogadorV,no.chaves[i]);
                        contV = 1;
                    } 
                    if(no.chaves[i].ano == jogadorV.ano){
                        if(month2num(no.chaves[i].mes) < month2num(jogadorV.mes)) {
                            jogadorV = copia_chaves(jogadorV,no.chaves[i]); 
                            contV = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorV.mes)){
                            if(no.chaves[i].dia < jogadorV.dia) {
                                jogadorV = copia_chaves(jogadorV,no.chaves[i]); 
                                contV = 1;
                            } 
                        }
                    }
                }
                if((no.chaves[i].ano == jogadorN.ano) && (no.chaves[i].dia == jogadorN.dia) && strcmp(no.chaves[i].mes,jogadorN.mes) == 0) contN++;
                else{
                    if(no.chaves[i].ano > jogadorN.ano) {
                        jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                        contN = 1;
                        } 
                    if(no.chaves[i].ano == jogadorN.ano){
                        if(month2num(no.chaves[i].mes) > month2num(jogadorN.mes)) {
                            jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                            contN = 1;
                        } 
                        if(month2num(no.chaves[i].mes) == month2num(jogadorN.mes)){
                            if(no.chaves[i].dia > jogadorN.dia) {
                                jogadorN = copia_chaves(jogadorN,no.chaves[i]);
                                contN = 1;
                            } 
                        }
                    }
                }
            }
            tmp++;
        }
        //acabei uma posição
        if(contV == 1) printf("\n\tMAIS VELHO: %s NASCIDO EM %d/%d/%d \n",jogadorV.nome,jogadorV.dia,month2num(jogadorV.mes),jogadorV.ano);
        if(contN == 1) printf("\n\tMAIS NOVO: %s NASCIDO EM %d/%d/%d \n",jogadorN.nome,jogadorN.dia,month2num(jogadorN.mes),jogadorN.ano);
        
        indiceV = 0;
        indiceN = 0;
        if(contV > 1 || contN > 1){
            if(contV > 1) vetV = (TJ*)malloc(sizeof(TJ)*contV);
            if(contN > 1) vetN = (TJ*)malloc(sizeof(TJ)*contN);
            fseek(fp, pos, SEEK_SET);
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&no,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; no.chaves[i].id != num; i++);//achei o jogador
                    if((contV > 1) && (no.chaves[i].ano == jogadorV.ano) && (no.chaves[i].dia == jogadorV.dia) && strcmp(no.chaves[i].mes,jogadorV.mes) == 0){
                        vetV[indiceV] = copia_chaves(vetV[indiceV],no.chaves[i]);
                        indiceV++;
                    }
                    if((contN > 1) && (no.chaves[i].ano == jogadorN.ano) && (strcmp(no.chaves[i].mes,jogadorN.mes) == 0) && (no.chaves[i].dia == jogadorN.dia)){
                        vetN[indiceN] = copia_chaves(vetN[indiceN],no.chaves[i]);
                        indiceN++;
                    }
                }
                tmp++;
            }
        }

        if(contV > 1)qsort(vetV, contV, sizeof(TJ), compara_TJ);
        if(contN > 1)qsort(vetN, contN, sizeof(TJ), compara_TJ);

        if(contV > 1){
            printf("\n\tJOGADORES MAIS VELHOS EMPATADOS NA DATA %d/%d/%d: ", jogadorV.dia, month2num(jogadorV.mes),jogadorV.ano);
            for(int i=0; i<contV; i++) printf("\n\t%s (%s)",vetV[i].nome,vetV[i].sele);
            printf("\n");
            free(vetV);
        }

        if(contN > 1){
            printf("\n\tJOGADORES MAIS NOVOS EMPATADOS NA DATA %d/%d/%d: ", jogadorN.dia, month2num(jogadorN.mes),jogadorN.ano);
            for(int i=0; i<contN; i++) printf("\n\t%s (%s)",vetN[i].nome, vetN[i].sele);
            printf("\n");
            free(vetN);
        }
        printf("\n");
    
        jogadorV.dia = INT_MAX;
        strcpy(jogadorV.mes, "MAX");
        jogadorV.ano = INT_MAX;
        jogadorN.dia = INT_MIN;
        strcpy(jogadorN.mes, "MIN");
        jogadorN.ano = INT_MIN;
        printf("\n");
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (2) Os jogadores que mais e menos atuaram em suas equipes; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q2(char* tabela, char * arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) return ;
    int qtd, num, contM, contm, tmp, indiceM, indicem;
    long pos;
    char sele[21], aux[25];
    TJ jogadorM, jogadorm, *vetM, *vetm;
    TABM a;
    jogadorM.part_sel = INT_MIN;
    jogadorm.part_sel = INT_MAX;

    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        contm = 0;
        contM = 0;
        pos = ftell(fp); //posição antes de ler a qtd da seleção
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        printf("\n");
        printf("\n\t%s: ",sele);
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&a,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                if(a.chaves[i].part_sel == jogadorM.part_sel) contM++;
                if(a.chaves[i].part_sel > jogadorM.part_sel){
                    jogadorM = copia_chaves(jogadorM,a.chaves[i]);
                    contM = 1;
                }
                if(a.chaves[i].part_sel == jogadorm.part_sel) contm++;
                if(a.chaves[i].part_sel < jogadorm.part_sel){
                    jogadorm = copia_chaves(jogadorm,a.chaves[i]);
                    contm = 1;
                }
            }
            tmp++;
        }
        //acabei uma seleção
        if(contM == 1) printf("\n\t%s eh o jogador que mais atuou no(a) %s, com %d partidas.", jogadorM.nome, jogadorM.sele, jogadorM.part_sel);
        if(contm == 1) printf("\n\t%s eh o jogador que menos atuou no(a) %s, com %d partidas.", jogadorm.nome, jogadorm.sele, jogadorm.part_sel);
        
        indiceM = 0;
        indicem = 0;
        if(contM > 1 || contm > 1){
            if(contM > 1) vetM = (TJ*)malloc(sizeof(TJ)*contM);
            if(contm > 1) vetm = (TJ*)malloc(sizeof(TJ)*contm);
            fseek(fp, pos, SEEK_SET);
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&a,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                    if(contM > 1 && a.chaves[i].part_sel == jogadorM.part_sel){
                        vetM[indiceM] = copia_chaves(vetM[indiceM],a.chaves[i]);
                        indiceM++;                        
                    }
                    if(contm > 1 && a.chaves[i].part_sel == jogadorm.part_sel){
                        vetm[indicem] = copia_chaves(vetm[indicem],a.chaves[i]);
                        indicem++;                        
                    }
                }
                tmp++;
            }
        }

        if(contM > 1)qsort(vetM, contM, sizeof(TJ), compara_TJ);
        if(contm > 1)qsort(vetm, contm, sizeof(TJ), compara_TJ);

        if(contM > 1){
            printf("\n\tJogadores que mais atuaram no(a) %s em ordem alfabética com %d partidas: ", jogadorM.sele, jogadorM.part_sel);
            for(int i=0; i<contM; i++) printf("\n\t%s",vetM[i].nome);
            free(vetM);
        }
        if (contm > 1){
            printf("\n\tJogadores que menos atuaram no(a) %s em ordem alfabética com %d partidas: ", jogadorm.sele, jogadorm.part_sel);
            for(int i=0; i<contm; i++) printf("\n\t%s",vetm[i].nome);    
            free(vetm);  
        }

        jogadorM.part_sel = INT_MIN;
        jogadorm.part_sel = INT_MAX;
        printf("\n");
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (3) Os jogadores que mais e menos atuaram no total;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q3_percorre(char*arv,TJ* jogadorM, TJ* jogadorm,int* contM,int* contm){
    FILE* fp = fopen(arv,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q3_percorre(no.filhos[0],jogadorM,jogadorm,contM,contm);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if(no.chaves[i].part_sel == jogadorM->part_sel) (*contM)++;
                if(no.chaves[i].part_sel > jogadorM->part_sel){
                    *jogadorM = copia_chaves(*jogadorM,no.chaves[i]);
                    *contM = 1;
                }
                if(no.chaves[i].part_sel == jogadorm->part_sel) (*contm)++;
                if(no.chaves[i].part_sel < jogadorm->part_sel){
                    *jogadorm = copia_chaves(*jogadorm,no.chaves[i]);
                    *contm = 1;
                }
            }
            Q3_percorre(no.prox,jogadorM,jogadorm,contM,contm);
        }
    }
    return;
}

void Q3_preenche(char * arv, TJ jogadorM, TJ jogadorm, int * indiceM, int * indicem, TJ * vetM, TJ * vetm,int contM, int contm){
    FILE* fp = fopen(arv,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q3_preenche(no.filhos[0], jogadorM, jogadorm, indiceM, indicem, vetM, vetm,contM,contm);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if((contM > 1) && (no.chaves[i].part_sel == jogadorM.part_sel)){
                    vetM[(*indiceM)] = copia_chaves(vetM[(*indiceM)],no.chaves[i]);
                    (*indiceM)++;
                }
                if(contm > 1 && no.chaves[i].part_sel == jogadorm.part_sel){
                    vetm[(*indicem)] = copia_chaves(vetm[(*indicem)],no.chaves[i]);
                    (*indicem)++;
                }
            }
            Q3_preenche(no.prox, jogadorM, jogadorm, indiceM, indicem, vetM, vetm,contM,contm);
        }
    }
    return;
}

void Q3(char* arv){
    TJ jogadorM, jogadorm;
    int contM = 0, contm = 0;
    jogadorm.part_sel = INT_MAX;
    jogadorM.part_sel = INT_MIN;

    Q3_percorre(arv,&jogadorM,&jogadorm,&contM,&contm);


    if(contM == 1){
        printf("\n\t%s eh o jogador que mais atuou no total, com %d partidas.\n",jogadorM.nome,jogadorM.part_sel);
    }
    if (contm == 1){
        printf("\n\t%s eh o jogador que menos atuou no total, com %d partidas.\n",jogadorm.nome,jogadorm.part_sel);
    }

    if(contM > 1 || contm > 1){
        TJ * vetm;
        TJ * vetM;
        if(contm > 1){
            vetm = (TJ*)malloc((sizeof(TJ))*contm);
        }
        if(contM > 1) {
            vetM = (TJ*)malloc((sizeof(TJ))*contM);
        }

        int indicem = 0, indiceM = 0;
        Q3_preenche(arv, jogadorM, jogadorm, &indiceM, &indicem, vetM, vetm,contM, contm);


        if(contM > 1)qsort(vetM, contM, sizeof(TJ), compara_TJ);
        if(contm > 1)qsort(vetm, contm, sizeof(TJ), compara_TJ);

        if(contM > 1){
            printf("\n\tJogadores que mais atuaram no total em ordem alfabética com %d partidas:\n", jogadorM.part_sel);
            for(int i=0; i<contM; i++) printf("\n\t%d. %s (%s)",vetM[i].id,vetM[i].nome,vetM[i].sele);
        }
        if (contm > 1){
            printf("\n\tJogadores que menos atuaram no total em ordem alfabética com %d partidas:\n", jogadorm.part_sel);
            for(int i=0; i<contm; i++) printf("\n\t%d. %s (%s)",vetm[i].id,vetm[i].nome,vetm[i].sele);        
        }
        if(contM > 1) free(vetM);
        if(contm > 1) free(vetm);
    }
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (4) A menor e a maior seleção, isto é, com menos ou mais convocados
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int compara_str(const void *a, const void *b) {
    char **pa = (char**)a;
    char **pb = (char**)b;
    return strcmp(*pa, *pb);
}

void Q4(char* tabela){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) exit(1);
    int qtd, respM = INT_MIN, respm = INT_MAX, num, tmp=0, lim;
    char maior_sel[20], menor_sel[20], aux[20];
    while(fread(&aux, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        lim = qtd;
        while(fread(&num, sizeof(int), 1, fp) == 1){
            tmp++;
            if(num == -1) qtd--;
            if(tmp == lim) break;
        }
        if(qtd > respM){
            respM = qtd;
            strcpy(maior_sel, aux);
        }
        if(qtd < respm){
            respm = qtd;
            strcpy(menor_sel, aux);
        }
    }
    rewind(fp);
    int contm = 0, contM = 0;
    //vendo quantos tem essa quantidade
    while(fread(&aux, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        lim = qtd;
        while(fread(&num, sizeof(int), 1, fp) == 1){
            tmp++;
            if(num == -1) qtd--;
            if(tmp == lim) break;
        }
        if(qtd == respM) contM++;
        if(qtd == respm) contm++;
    }
    
    if(contM == 1){
        printf("\n\t%s eh a maior seleção com %d jogadores\n",maior_sel,respM);
    }
    if (contm == 1){
        printf("\n\t%s eh a menor seleção com %d jogadores\n",menor_sel,respm);
    }
    
    if(contM > 1 || contm > 1){
        char **vetm;
        char **vetM;
        if(contm > 1){
            vetm = (char**)malloc((sizeof(char*))*contm);
            for(int i = 0; i < contm; i++) vetm[i] = malloc(sizeof(char)*21);
        }
        if(contM > 1) {
            vetM = (char**)malloc((sizeof(char*))*contM);
            for(int i = 0; i < contM; i++) vetM[i] = malloc(sizeof(char)*21);
        }
        rewind(fp);
        int m=0, M=0;
        while(fread(&aux, sizeof(char)*20, 1, fp) == 1){
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            lim = qtd;
            while(fread(&num, sizeof(int), 1, fp) == 1){
                tmp++;
                if(num == -1) qtd--;
                if(tmp == lim) break;
            }
            if(contM > 1 && qtd == respM){
                strcpy(vetM[M], aux);
                M++;
            }
            if(contm > 1 && qtd == respm){
                strcpy(vetm[m], aux);
                m++;
            }
        }
        if(contM > 1)qsort(vetM, contM, sizeof(char*), compara_str);
        if(contm > 1)qsort(vetm, contm, sizeof(char*), compara_str);
        if(contM > 1){
            printf("\n\n\tMaiores seleções empatadas em ordem alfabética com %d jogadores: ",respM);
            for(int i=0; i<contM; i++) printf("\n\t%s",vetM[i]);
        }
        if (contm > 1){
            printf("\n\n\tMenores seleções empatadas em ordem alfabética com %d jogadores: ",respm);
            for(int i=0; i<contm; i++) printf("\n\t%s",vetm[i]);        }
        
        if(contM > 1){
            for(int i = 0; i < contM; i++) free(vetM[i]);
            free(vetM);
        }
        if(contm > 1){
            for(int i = 0; i < contm; i++) free(vetm[i]);
            free(vetm);
        }
    }
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (5) Busca de todos os jogadores que atuam fora do seu país de origem
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q5(char* tabela, char* arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) exit(1);

    int qtd, num, tmp = 0;
    char sele[20],aux[20];
    TABM a;

    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        printf("\n");
        printf("\n\t%s: ",sele);
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&a,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; a.chaves[i].id != num; i++);
                if(strcmp(a.chaves[i].pais_time,sele) != 0) printf("\n\t%d. %s (%s)",a.chaves[i].id, a.chaves[i].nome, a.chaves[i].pais_time);
            }
            tmp++;
        }
    }
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (6) Busca de todos os jogadores que atuam no seu país de origem
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q6(char* tabela,char* arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) exit(1);

    int qtd, num, tmp = 0;
    char sele[20],aux[20];
    TABM a;

    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        printf("\n");
        printf("\n\t%s: ",sele);
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&a,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; a.chaves[i].id != num; i++);
                if(strcmp(a.chaves[i].pais_time,sele) == 0) printf("\n\t%d. %s",a.chaves[i].id, a.chaves[i].nome);
            }
            tmp++;
        }
    }
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (7) Busca de todos os jogadores que nasceram no mesmo ano
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q7(char* raiz, int ano){
    FILE* fp = fopen(raiz,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q7(no.filhos[0],ano);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if(no.chaves[i].ano == ano) printf("\n\t%d. %s",no.chaves[i].id,no.chaves[i].nome);
            }
            Q7(no.prox,ano);
        }
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (8) Busca de todos os jogadores que nasceram no mesmo mês
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q8(char* raiz, char* Mes){
    FILE* fp = fopen(raiz,"rb");
    if(fp){
        TABM no;
        fread(&no,sizeof(TABM),1,fp);
        if(!no.folha){
            Q8(no.filhos[0],Mes);
        }
        if(no.folha){
            for(int i = 0; i < no.nchaves; i++){
                if(strcmp(no.chaves[i].mes,Mes) == 0) printf("\n\t%d. %s",no.chaves[i].id,no.chaves[i].nome);
            }
            Q8(no.prox,Mes);
        }
    }
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (9) Busca da(s) seleção(ções) com mais jogadores que atuam fora do seu país de origem
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q9(char* tabela, char * arv){
    FILE * fp = fopen(tabela, "rb");
    int qtd, num, cont, tmp, indice, qtd_maiores, maior = INT_MIN;
    char sele[21], aux[25], resp_sele[21];
    char * vet;
    TABM a;

    qtd_maiores = 0;
    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        cont = 0;
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&a,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                if(strcmp(a.chaves[i].pais_time, a.chaves[i].sele) != 0) cont++;
            }
            tmp++;
        }
        //acabei uma seleção
        if(cont == maior) qtd_maiores++;
        if(cont > maior){
            maior = cont; 
            strcpy(resp_sele, sele);
            qtd_maiores = 1; 
        }
    }

    if(qtd_maiores == 1) printf("\n\t%s eh a seleção com mais jogadores que atuam fora do seu país de origem com %d jogadores.\n", resp_sele, maior);

    if(qtd_maiores > 1){
        char **vet = (char**)malloc((sizeof(char*))*qtd_maiores);
        for(int i = 0; i < qtd_maiores; i++) vet[i] = malloc(sizeof(char)*21);
        int indice = 0;

        rewind(fp);
        while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
            cont = 0;
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&a,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                    if(strcmp(a.chaves[i].pais_time, a.chaves[i].sele) != 0) cont++;
                }
                tmp++;
            }
            if(cont == maior){
                strcpy(vet[indice], sele);
                indice++;
            }
        }

        qsort(vet, qtd_maiores, sizeof(char*), compara_str);

        printf("\n\tSeleções com mais jogadores que atuam fora do seu pais de origem em ordem alfabetica, com %d jogadores: ", maior);
        for(int i=0; i<qtd_maiores; i++) printf("\n\t- %s",vet[i]);
        for(int i=0; i<qtd_maiores; i++) free(vet[i]);
        free(vet);
    }

    fclose(fp);
    return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (10) Busca da(s) seleção(ções) com mais jogadores que atuam no seu país de origem
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q10(char* tabela, char * arv){
    FILE * fp = fopen(tabela, "rb");
    if(!fp){
        printf("\n\n\tPREZADO USUARIO: Todos os jogadores foram excluidos!");
        return;
    }
    int qtd, num, cont, tmp, indice, qtd_maiores, maior = INT_MIN;
    char sele[21], aux[25], resp_sele[21];
    char * vet;
    TABM a;

    qtd_maiores = 0;
    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        cont = 0;
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            strcpy(aux,TABM_busca(arv,num));
            if(strcmp(aux,"NULL") != 0){
                FILE* fj = fopen(aux,"rb");
                if(!fj) exit(1);
                fread(&a,sizeof(TABM),1,fj);
                fclose(fj);

                int i;
                for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                if(strcmp(a.chaves[i].pais_time, a.chaves[i].sele) == 0) cont++;
            }
            tmp++;
        }
        //acabei uma seleção
        if(cont == maior) qtd_maiores++;
        if(cont > maior){
            maior = cont; 
            strcpy(resp_sele, sele);
            qtd_maiores = 1; 
        }
    }

    if(qtd_maiores == 1) printf("\n\t%s eh a selecao com mais jogadores que atuam no seu país de origem com %d jogadores.\n", resp_sele, maior);

    if(qtd_maiores > 1){
        char **vet = (char**)malloc((sizeof(char*))*qtd_maiores);
        for(int i = 0; i < qtd_maiores; i++) vet[i] = malloc(sizeof(char)*21);
        int indice = 0;

        rewind(fp);
        while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
            cont = 0;
            fread(&qtd, sizeof(int), 1, fp);
            tmp = 0;
            while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&a,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; a.chaves[i].id != num; i++);//achei o jogador
                    if(strcmp(a.chaves[i].pais_time, a.chaves[i].sele) == 0) cont++;
                }
                tmp++;
            }
            if(cont == maior){
                strcpy(vet[indice], sele);
                indice++;
            }
        }

        qsort(vet, qtd_maiores, sizeof(char*), compara_str);

        printf("\n\tSelecoes com mais jogadores que atuam no seu pais de origem em ordem alfabetica, com %d jogadores: ", maior);
        for(int i=0; i<qtd_maiores; i++) printf("\n\t-%s",vet[i]);
        for(int i=0; i<qtd_maiores; i++) free(vet[i]);
        free(vet);
    }

    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (11) Busca das informações subordinadas, dadas a chave primária (identificador único do jogador)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q11(char* arv,int ch){
    FILE* fp = fopen(arv,"rb");
    if(!fp){
        printf("\n\n\tPREZADO USUARIO: Todos os jogadores foram excluidos!");
        return;
    }
    fclose(fp);
    
    char resp[21];
    strcpy(resp,TABM_busca(arv,ch));

    fp = fopen(resp,"rb");
    if(!fp){
        printf("\n\n\tPREZADO USUARIO: O jogador nao se encontra na arvore");
        return;
    }
    TABM no;
    fread(&no,sizeof(TABM),1,fp);
    fclose(fp);

    int i = 0;
    while(i < no.nchaves && no.chaves[i].id != ch) i++;

    printf("\n\n\tARQUIVO: %s",resp);
    printf("\n\n\tINFOS SUBORDINADAS");
    printf("\n\tNome: %s",no.chaves[i].nome);
    printf("\n\tSelecao: %s", no.chaves[i].sele);
    printf("\n\tNumero da camisa: %d", no.chaves[i].num_camisa);
    printf("\n\tPosicao: %s", no.chaves[i].posicao);
    printf("\n\tData de nascimento: %d %s %d", no.chaves[i].dia, no.chaves[i].mes, no.chaves[i].ano);
    printf("\n\tIdade: %d", no.chaves[i].idade);
    printf("\n\tPartidas pela selecao: %d", no.chaves[i].part_sel);
    printf("\n\tGols pela selecao: %d", no.chaves[i].gol_sel);
    printf("\n\tTime regular: %s", no.chaves[i].time);
    printf("\n\tPais do time regular: %s", no.chaves[i].pais_time);
    if(no.chaves[i].capitao) printf("\n\tCapitao: SIM");
    else(printf("\n\tCapitao: NAO"));
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.1) Alteração do número da camisa de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_1(char* arv, int id, int novo_num){
    char resp[20], sele_tmp[20], aux[20];
    int qtd, tmp, num, ver = 0;
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }    
        
    TABM a, b;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    char selecao[20];
    strcpy(selecao,a.chaves[i].sele);

    FILE* fp = fopen("Tabelas/Nacionalidades.bin", "rb");
    if(!fp) exit(1);

    while(fread(&sele_tmp, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        while((tmp != qtd) && (fread(&num, sizeof(int), 1, fp) == 1)){
            if(strcmp(sele_tmp,selecao) == 0){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&b,sizeof(TABM),1,fj);
                    fclose(fj);

                    int j;
                    for(j = 0; b.chaves[j].id != num; j++);
                    if(b.chaves[j].num_camisa == novo_num){
                        printf("\n");
                        printf("\n\tPREZADO USUARIO: Ja existe um jogador desta selecao com este numero!");
                        fclose(fp);
                        return;
                    }
                }
            }
            tmp++;
        }
    }
    a.chaves[i].num_camisa = novo_num;
    printf("\n");
    printf("\n\tAlterado com sucesso!");
    fj = fopen(resp, "wb");
    fwrite(&a,sizeof(TABM),1,fj);
    fclose(fj);
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.2) Alteração da posição do jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_2(char* arv, int id, char* pos){
    char resp[20];
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(strcmp(a.chaves[i].posicao,"GK") == 0){
        printf("\n\tPREZADO USUARIO: Nao eh possivel alterar a posicao de um goleiro!");
        return;
    }

    if(strcmp(pos, a.chaves[i].posicao)){ //times diferentes
        strcpy(a.chaves[i].posicao, pos);
        tabela_posicoes_alterar("EURO.txt", id, pos);
        printf("\n\n\tAlterado com sucesso e tabela de posicoes atualizada!");
        fj = fopen(resp, "wb");
        fwrite(&a, sizeof(TABM), 1, fj);
        fclose(fj);
    }else printf("\n\n\tPREZADO USUARIO: O jogador ja joga nesta posicao!");
    
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.3) Alteração da idade de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_3(char* arv, int id, int nova_idade){
    char resp[20];
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(nova_idade >= a.chaves[i].idade){
        a.chaves[i].idade = nova_idade;
        printf("\n");
        printf("\n\tAlterado com sucesso!");
        fj = fopen(resp,"wb");
        fwrite(&a,sizeof(TABM),1,fj);
        fclose(fj);
    }
    else{
        printf("\n");
        printf("\n\tPREZADO USUARIO: Nao eh possivel retrocedor uma idade!");
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.4) Alteração da quantidade de partidas de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_4(char* arv, int id, int novas_partidas){
    char resp[20], sele_tmp[20], aux[20];
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a, b;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(novas_partidas >= a.chaves[i].part_sel){
        a.chaves[i].part_sel = novas_partidas;
        printf("\n\n\tAlterado com sucesso!");
        fj = fopen(resp, "wb");
        fwrite(&a, sizeof(TABM), 1, fj);
        fclose(fj);
    }else printf("\n\n\tPREZADO USUARIO: Não eh possivel retroceder a quantidade de partidas!");
    
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.5) Alteração do número de gols de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_5(char* arv, int id, int novos_gols){
    char resp[20], sele_tmp[20], aux[20];
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a, b;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(novos_gols >= a.chaves[i].gol_sel){
        a.chaves[i].gol_sel = novos_gols;
        printf("\n\n\tAlterado com sucesso!");
        fj = fopen(resp, "wb");
        fwrite(&a, sizeof(TABM), 1, fj);
        fclose(fj);
    }else printf("\n\n\tPREZADO USUARIO: Não eh possivel retroceder a quantidade de gols!");
    
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.6) Alteração do time de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q12_6(char* arv, int id, char * novo_time){
    char resp[20];
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);
    
    //time e o pais
    char novo_pais[21];
    printf("\n\n\tQual o pais desse time? ");
    scanf("%s", novo_pais);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(strcmp(novo_time, a.chaves[i].time)){ //times diferentes
        strcpy(a.chaves[i].time, novo_time);
        strcpy(a.chaves[i].pais_time, novo_pais);
        printf("\n\n\tAlterado com sucesso!");
        fj = fopen(resp, "wb");
        fwrite(&a, sizeof(TABM), 1, fj);
        fclose(fj);
    }else printf("\n\n\tPREZADO USUARIO: O jogador ja joga neste time!");
    
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (12.7) Alteração da opção capitão de um jogador
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vira_capitao(char * arv, int id){
    char resp[20], sele_tmp[20], aux[20];
    int qtd, tmp, num, ver = 0;
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a, b;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);

    if(a.chaves[i].capitao){
        printf("\n\n\tPREZADO USUARIO: O %s ja eh capitao!",a.chaves[i].nome);
        return;
    }

    char selecao[20];
    strcpy(selecao,a.chaves[i].sele);

    FILE* fp = fopen("Tabelas/Nacionalidades.bin", "rb");
    if(!fp) exit(1);

    while(fread(&sele_tmp, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        while((tmp != qtd) && (fread(&num, sizeof(int), 1, fp) == 1)){
            if(strcmp(sele_tmp,selecao) == 0){ //Tornar alguem capitao
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&b,sizeof(TABM),1,fj);
                    fclose(fj);

                    int j;
                    for(j = 0; b.chaves[j].id != num; j++);
                    if((b.chaves[j].capitao) && (b.chaves[j].id != id)){
                        b.chaves[j].capitao = 0;
                        FILE* fb = fopen(aux,"wb");
                        fwrite(&b,sizeof(TABM),1,fb);
                        fclose(fb);
                    }
                }
            }
            tmp++;
        }
    }
    a.chaves[i].capitao = 1;
    printf("\n");
    printf("\n\tAlterado com sucesso!");
    printf("\n\t%s eh o novo capitao do(a) %s",a.chaves[i].nome, a.chaves[i].sele);
    fj = fopen(resp, "wb");
    fwrite(&a,sizeof(TABM),1,fj);
    fclose(fj);
    fclose(fp);
    return;
    
}

void deixa_capitao(char * arv, int id, int t,int ver){
    char resp[20], sele_tmp[20], aux[20], arq_maior[25] = "DEU_RUIM.bin";
    int qtd, tmp, num, partidas = INT_MIN, id_maior;
    strcpy(resp,TABM_busca(arv,id));
    FILE* fj = fopen(resp,"rb");
    if(!fj){ 
        printf("\n");
        printf("\n\tPREZADO USUARIO: O jogador não está na árvore!");
        return;
    }
    TABM a, b, jog_maior;
    fread(&a,sizeof(TABM),1,fj);
    fclose(fj);

    int i = 0;
    for(i = 0; a.chaves[i].id != id; i++);
    
    if(!a.chaves[i].capitao){
        if(ver) printf("\n\n\tPREZADO USUARIO: O %s nao eh capitao!",a.chaves[i].nome);
        return;
    }
    a.chaves[i].capitao = 0;
    fj = fopen(resp, "wb");
    fwrite(&a,sizeof(TABM),1,fj);
    fclose(fj);

    char selecao[20];
    strcpy(selecao,a.chaves[i].sele);

    FILE* fp = fopen("Tabelas/Nacionalidades.bin", "rb");
    if(!fp) exit(1);

    while(fread(&sele_tmp, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0; int lim = qtd;
        while((tmp != lim) && (fread(&num, sizeof(int), 1, fp) == 1)){
            if(num == -1) qtd--;
            if(!qtd) return;
            if(strcmp(sele_tmp,selecao) == 0){ //Tirar o capitão
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&b,sizeof(TABM),1,fj);
                    fclose(fj);

                    int j;
                    for(j = 0; b.chaves[j].id != num; j++);
                    if((b.chaves[j].part_sel > partidas) && (b.chaves[j].id != id)){

                        partidas = b.chaves[j].part_sel;
                        id_maior = b.chaves[j].id;
                        copia(&b,&jog_maior,t);
                        strcpy(arq_maior,aux);
                    }
                }
            }
            tmp++;
        }
    }
    int j;
    for(j = 0; jog_maior.chaves[j].id != id_maior; j++);
    jog_maior.chaves[j].capitao = 1;
    if(strcmp(arq_maior,"DEU_RUIM.bin") != 0){
        fj = fopen(arq_maior,"wb");
        fwrite(&jog_maior,sizeof(TABM),1,fj);
        fclose(fj);
        if(ver){
            printf("\n");
            printf("\n\tAlterado com sucesso!");
            printf("\n\t%s eh o novo capitao do(a) %s",jog_maior.chaves[j].nome, jog_maior.chaves[j].sele);
        }
    }
    return;
}


void Q12_7(char* arv, int id, int op, int t){

    if(op) vira_capitao(arv, id);
    else deixa_capitao(arv, id, t,1);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (13) Busca de todos os jogadores de uma seleção;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q13(char* arv, char*pais, char* tabela){
    FILE * fp = fopen(tabela, "rb");
    if(!fp) exit(1);

    int qtd, num, tmp = 0;
    char sele[20],aux[20];
    TABM a;

    printf("\n");
    printf("\n\t%s: ",pais);
    while(fread(&sele, sizeof(char)*20, 1, fp) == 1){
        fread(&qtd, sizeof(int), 1, fp);
        tmp = 0;
        while(tmp != qtd && fread(&num, sizeof(int), 1, fp) == 1){
            if(strcmp(sele,pais) == 0){
                strcpy(aux,TABM_busca(arv,num));
                if(strcmp(aux,"NULL") != 0){
                    FILE* fj = fopen(aux,"rb");
                    if(!fj) exit(1);
                    fread(&a,sizeof(TABM),1,fj);
                    fclose(fj);

                    int i;
                    for(i = 0; a.chaves[i].id != num; i++);
                    printf("\n\t%d. %s",a.chaves[i].id, a.chaves[i].nome);
                }
            }    
            tmp++;
        }
    }
    fclose(fp);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (14) Busca e remoção de todos os capitães;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q14_B(char* arv){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q14_B(a.filhos[0]);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(a.chaves[i].capitao) printf("\n\t%d. %s (%s)",a.chaves[i].id,a.chaves[i].nome, a.chaves[i].sele);
            }
            Q14_B(a.prox);
        }
    }
    return;
}

void Q_14_R_percorre(char* arv, char* lista){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q_14_R_percorre(a.filhos[0],lista);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(a.chaves[i].capitao){
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q_14_R_percorre(a.prox,lista);
        }
    }
    return;
}

char* Q14_R(char* arv, int t){
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/Capitães.bin","wb");
    fclose(fl);
    Q_14_R_percorre(arv,"Tabelas/Capitães.bin");

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/Capitães.bin","rb");
    int id_cap;
    while(fread(&id_cap,sizeof(int),1,fl)){
        deixa_capitao(arv,id_cap,t, 1);
        strcpy(arv,TABM_remover(arv,id_cap,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id_cap);
    }
    remove("Tabelas/Capitães.bin");
    fclose(fl);

    return arv;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (15) Remoção de jogadores a partir de uma determinada idade;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q15_percorre(char* arv, char* lista,int idade){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q15_percorre(a.filhos[0],lista,idade);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(a.chaves[i].idade >= idade){
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q15_percorre(a.prox,lista,idade);
        }
    }
    return;
}

char* Q15(char* arv,int idade ,int t){
    char no[20];
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/idade.bin","wb");
    fclose(fl);
    Q15_percorre(arv,"Tabelas/idade.bin",idade);

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/idade.bin","rb");
    int id_cap;
    while(fread(&id_cap,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id_cap));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id_cap; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id_cap,t, 0);
        strcpy(arv,TABM_remover(arv,id_cap,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id_cap);
    }
    remove("Tabelas/idade.bin");
    fclose(fl);

    return arv;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (16) Retirada de todos os jogadores de uma seleção que atuam num determinado país;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q16_percorre(char* arv, char* lista,char* sele, char* pais){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q16_percorre(a.filhos[0],lista,sele,pais);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if((strcmp(a.chaves[i].sele,sele) == 0) &&(strcmp(a.chaves[i].pais_time,pais) == 0)){
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q16_percorre(a.prox,lista,sele,pais);
        }
    }
    return;
}

char* Q16(char* arv,int t,char* sele, char* pais){
    char no[25];
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/a.bin","wb");
    fclose(fl);
    Q16_percorre(arv,"Tabelas/a.bin",sele,pais);

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/a.bin","rb");
    int id_cap;
    while(fread(&id_cap,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id_cap));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id_cap; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id_cap,t,0);
        strcpy(arv,TABM_remover(arv,id_cap,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id_cap);
    }
    remove("Tabelas/a.bin");
    fclose(fl);

    return arv;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (17) Retirada de todos os os jogadores de uma seleção que não atuam no país de origem;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q17_percorre(char* arv, char* lista){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q17_percorre(a.filhos[0],lista);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(strcmp(a.chaves[i].sele, a.chaves[i].pais_time)){ //forem diferentes
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q17_percorre(a.prox,lista);
        }
    }
    return;
}

char* Q17(char* arv,int t){
    char no[25];
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/aux.bin","wb");
    fclose(fl);
    Q17_percorre(arv,"Tabelas/aux.bin");

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/aux.bin","rb");
    int id;
    while(fread(&id,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id,t,0);
        strcpy(arv,TABM_remover(arv,id,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id);
    }
    remove("Tabelas/aux.bin");
    fclose(fl);

    return arv;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (18) Retirada de todos os os jogadores de uma seleção que atuam no país de origem;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q18_percorre(char* arv, char* lista){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q18_percorre(a.filhos[0],lista);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(strcmp(a.chaves[i].sele, a.chaves[i].pais_time) == 0){ //forem iguais
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q18_percorre(a.prox,lista);
        }
    }
    return;
}

char* Q18(char* arv,int t){
    char no[20];
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/b.bin","wb");
    fclose(fl);
    Q18_percorre(arv,"Tabelas/b.bin");

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/b.bin","rb");
    int id;
    while(fread(&id,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id,t,0);
        strcpy(arv,TABM_remover(arv,id,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id);
    }
    remove("Tabelas/b.bin");
    fclose(fl);

    return arv;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          (19) Retirada de todos os os jogadores de uma seleção;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Q19_percorre(char* arv, char* lista, char * selecao){
    FILE* fp  = fopen(arv,"rb");
    if(fp){
        TABM a;
        fread(&a,sizeof(TABM),1,fp);
        fclose(fp);
        if(!a.folha){
            Q19_percorre(a.filhos[0],lista, selecao);
        }
        if(a.folha){
            for(int i = 0; i < a.nchaves; i++){
                if(strcmp(a.chaves[i].sele, selecao) == 0){ //forem iguais
                    FILE* fs = fopen(lista,"ab");
                    fwrite(&a.chaves[i].id,sizeof(int),1,fs);
                    fclose(fs);
                }
            }
            Q19_percorre(a.prox,lista, selecao);
        }
    }
    return;
}

char* Q19(char* arv,int t, char * selecao){
    char no[25];
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/c.bin","wb");
    fclose(fl);
    Q19_percorre(arv,"Tabelas/c.bin", selecao);

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/c.bin","rb");
    int id;
    while(fread(&id,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id,t,0);
        strcpy(arv,TABM_remover(arv,id,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id);
    }
    remove("Tabelas/c.bin");
    fclose(fl);

    return arv;
}

char* Q20(char* arv,int t){
    //Criando lista de capitães
    FILE* fl = fopen("Tabelas/rem.bin","wb");
    fclose(fl);
    char no[25];
    int entrada;
    do{
        printf("\tDigite o conjunto de valor e -1 para parar: ");
        scanf("%d",&entrada);
        if(entrada < 0) break;
        fl = fopen("Tabelas/rem.bin","ab");
        if(!fl) exit(1);
        fwrite(&entrada,sizeof(int),1,fl);
        fclose(fl);
        
    }while(1);

    //Com a tabela criada, basta percorrer ir retirando, e buscar os id para colocar -1 na tabela de nacionalidade(não vi necessidade de mudar o de posições)
    fl = fopen("Tabelas/rem.bin","rb");
    int id;
    while(fread(&id,sizeof(int),1,fl)){
        strcpy(no,TABM_busca(arv,id));
        if(strcmp(no,"NULL") != 0){
            TABM a;
            FILE* fp = fopen(no,"rb");
            fread(&a,sizeof(TABM),1,fp);
            int i;
            for(i = 0; a.chaves[i].id != id; i++);
            printf("\n\t%s",a.chaves[i].nome);
            fclose(fp);
        }
        deixa_capitao(arv,id,t,0);
        strcpy(arv,TABM_remover(arv,id,t));
        remove_tabela("Tabelas/Nacionalidades.bin",id);
    }
    remove("Tabelas/rem.bin");
    fclose(fl);

    return arv;
}