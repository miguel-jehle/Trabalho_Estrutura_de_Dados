#include "TABM.h"
#include "Tabelas.c"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÃO DE BUSCA
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* TABM_busca(char* arq, int id){
    if(id == -1) return "NULL";
    FILE* fp = fopen(arq,"rb");
    if(!fp) return "NULL";
    //if(!fp) exit(1);
    TABM a;
    fread(&a,sizeof(TABM),1,fp);
    fclose(fp);
    int i = 0;
    while ((i < a.nchaves) && (id > a.chaves[i].id)) i++;
    if ((i < a.nchaves) && (a.folha) && (id == a.chaves[i].id)) return arq;
    if (a.folha) return "NULL";
    if (a.chaves[i].id == id) i++;
    if(i > a.nchaves) i = a.nchaves;
    return TABM_busca(a.filhos[i], id);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          LIBERA NÓ NA MEMÓRIA PRINCIPAL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TABM_libera_no(TABM * no){
    free(no);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÃO DE IMPRESSÃO AUXILIAR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void imp(char* raiz,int andar, int t){
    FILE* fp = fopen(raiz,"rb");
    if(fp){
        TABM * a = TABM_cria_no(t);
        fread(a,sizeof(TABM),1,fp);
        int i,j;
        for(i = 0; i < a->nchaves; i++){
            imp(a->filhos[i],andar+1,t);
            for(j = 0; j<= andar; j++) printf("\t");
            printf("%d\n", a->chaves[i].id);
        }
        imp(a->filhos[i],andar+1,t);
        fclose(fp);
        TABM_libera_no(a);
    }
    return;
}

void TABM_imprime(char** raiz,int t){
    imp(*raiz,0,t);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÕES DE COPIAR STRUCTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void copia(TABM * T, TABM * aux, int t){
    aux->nchaves = T->nchaves;
    aux->folha = T->folha;
    strcpy(aux->prox,T->prox);

    int i;
    for (int i=0; i<T->nchaves; i++){
        aux->chaves[i].id = T->chaves[i].id;
        aux->chaves[i].num_camisa = T->chaves[i].num_camisa;
        strcpy(aux->chaves[i].posicao, T->chaves[i].posicao);
        strcpy(aux->chaves[i].nome,T->chaves[i].nome);
        aux->chaves[i].dia = T->chaves[i].dia;
        strcpy(aux->chaves[i].mes,T->chaves[i].mes);
        aux->chaves[i].ano = T->chaves[i].ano;
        aux->chaves[i].idade = T->chaves[i].idade;
        aux->chaves[i].part_sel = T->chaves[i].part_sel;
        aux->chaves[i].gol_sel = T->chaves[i].gol_sel;
        strcpy(aux->chaves[i].pais_time,T->chaves[i].pais_time);
        strcpy(aux->chaves[i].time,T->chaves[i].time);
        aux->chaves[i].capitao = T->chaves[i].capitao;
        strcpy(aux->chaves[i].sele,T->chaves[i].sele);
    }
    for(i = 0; i<= 2 * t;i++){
        strcpy(aux->filhos[i],T->filhos[i]);
    }
    return;
}

TJ copia_chaves (TJ aux, TJ T){
    aux.id = T.id;
    aux.num_camisa = T.num_camisa;
    strcpy(aux.posicao, T.posicao);
    strcpy(aux.nome,T.nome);
    aux.dia = T.dia;
    strcpy(aux.mes,T.mes);
    aux.ano = T.ano;
    aux.idade = T.idade;
    aux.part_sel = T.part_sel;
    aux.gol_sel = T.gol_sel;
    strcpy(aux.pais_time,T.pais_time);
    strcpy(aux.time,T.time);
    aux.capitao = T.capitao;
    strcpy(aux.sele,T.sele);
    return aux;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÃO DE CRIAR O NÓ NA MEMÓRIA PRINCIPAL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TABM *TABM_cria_no(int t){
    TABM* novo = (TABM*)malloc(sizeof(TABM));
    novo->nchaves = 0;
    novo->folha = 1;
    strcpy(novo->prox, "Prox");
    return novo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÃO DE DIVIDIR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TABM *divisao(TABM *S, int i, TABM* T, int t, int * cont){
    TABM *z = TABM_cria_no(t);
    char *z_arq = (char*)malloc(sizeof(char)*30);
    TABM_cria(t, cont, &z_arq);
    z->folha = T->folha;
    int j;
    if(!T->folha){
        z->nchaves = t-1;
        for(j=0;j<t-1;j++) z->chaves[j] = T->chaves[j+t];
        for(j=0;j<t;j++){
            strcpy(z->filhos[j],T->filhos[j+t]);
            strcpy(T->filhos[j+t],"NULL");
        }
    }
    else {
        z->nchaves = t; //z possuirá uma chave a mais que T se for folha
        for(j=0;j < t;j++) z->chaves[j] = T->chaves[j+t-1];//Caso em que T é folha, temos q passar a info para o nó da direita
        strcpy(z->prox,T->prox);
        strcpy(T->prox, z_arq);
    }
    T->nchaves = t-1;
    for(j=S->nchaves; j>=i; j--) strcpy(S->filhos[j+1],S->filhos[j]);
    strcpy(S->filhos[i],z_arq);
    for(j=S->nchaves; j>=i; j--) S->chaves[j] = S->chaves[j-1];
    S->chaves[i-1] = T->chaves[t-1];
    S->nchaves++;
    FILE* fz = fopen(z_arq,"wb+");
    TABM aux;
    copia(z,&aux,t);
    fwrite(&aux,sizeof(TABM),1,fz);
    fclose(fz);
    free(z_arq);
    TABM_libera_no(z);
    return S;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          INSERE NO NÓ NÃO COMPLETO
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TABM *insere_nao_completo(TABM *S, TJ * jogador, int t, int * cont){
    int i = S->nchaves-1;
    
    if(S->folha){
        while((i>=0) && (jogador->id < S->chaves[i].id)){
            S->chaves[i+1] = S->chaves[i];
            i--;
        }
        S->chaves[i+1] = *jogador;
        S->nchaves++;
        return S;
    }
    while((i>=0) && (jogador->id < S->chaves[i].id)) i--;
    i++;

    FILE* fp = fopen(S->filhos[i],"rb+");
    TABM* S_filho = TABM_cria_no(t);
    fread(S_filho,sizeof(TABM),1,fp);
    fclose(fp);

    if(S_filho->nchaves == ((2*t)-1)){
        fp = fopen(S->filhos[i],"wb");
        S = divisao(S, (i+1), S_filho, t, cont);
        if(jogador->id > S->chaves[i].id) i++;
        TABM aux;
        copia(S_filho,&aux,t);
        fwrite(&aux,sizeof(TABM),1,fp);
        fclose(fp);
    }
    

    fp = fopen(S->filhos[i],"rb+");  
    //S_filho = TABM_cria_no(t);
    fread(S_filho,sizeof(TABM),1,fp);
    fclose(fp);
    
    S_filho = insere_nao_completo(S_filho, jogador, t, cont);

    TABM aux;
    copia(S_filho,&aux,t);
    fp = fopen(S->filhos[i],"wb");
    fwrite(&aux,sizeof(TABM),1,fp);
    fclose(fp);
    TABM_libera_no(S_filho);
    return S;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          INSERIR NÓ NA ÁRVORE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* TABM_insere(TJ *jogador, int t, char ** raiz, int * cont){
    FILE * fp = fopen(*raiz, "rb+");
    //se não tem arvore ainda
    if(!fp){
        TABM_cria(t, cont, raiz);// Esse cara é o nosso "retorno", visto que temos uma variável raiz para controlar qual o nó do topo
        FILE * fraiz = fopen(*raiz, "rb+");
        if(!fraiz) exit(1);
        TABM *T = TABM_cria_no(t);
        fread(T,sizeof(TABM),1,fraiz);
        T->chaves[0] = *jogador;
        T->nchaves = 1;
        rewind(fraiz);
        TABM aux;
        copia(T,&aux,t);
        fwrite(&aux, sizeof(TABM), 1, fraiz);
        TABM_libera_no(T);
        fclose(fraiz);
        return *raiz;
    }
    
    //Verificando se o id já está na árvore e alterar os dados baseado no id.
    char ver[20];
    strcpy(ver,TABM_busca(*raiz,jogador->id));
    if(strcmp(ver,"NULL") != 0){
        FILE* fd = fopen(ver,"rb");
        TABM a;
        fread(&a,sizeof(TABM),1,fd);
        fclose(fd);

        int i;
        for(i = 0; a.chaves[i].id != jogador->id; i++);
        a.chaves[i] = copia_chaves(a.chaves[i],*jogador);

        fd = fopen(ver,"wb");
        fwrite(&a,sizeof(TABM),1,fd);
        fclose(fd);
        return *raiz;
    }
    
    TABM *T = TABM_cria_no(t);
    fread(T,sizeof(TABM),1,fp);
    fclose(fp);

    if(T->nchaves == (2*t)-1){ //tá lotado
        char * S_arq = (char*)malloc(sizeof(char)*30);
        TABM_cria(t, cont, &S_arq);
        FILE *fs = fopen(S_arq, "rb+");
        if(!fs) exit(1);
        TABM* S = (TABM*)malloc(sizeof(TABM));
        fread(S, sizeof(TABM),1,fs);
        fclose(fs);
        S->nchaves = 0;
        S->folha = 0;
        strcpy(S->filhos[0],(*raiz));
        S = divisao(S,1,T,t, cont);
        fp = fopen(*raiz, "wb");
        TABM aux_3;
        copia(T, &aux_3, t);
        fwrite(&aux_3, sizeof(TABM), 1, fp);
        fclose(fp);
        S = insere_nao_completo(S, jogador, t, cont);
        TABM aux;
        copia(S,&aux,t);
        fs = fopen(S_arq,"wb");
        fwrite(&aux, sizeof(TABM),1,fs); //Escreve S
        fclose(fs);

        TABM_libera_no(T);
        TABM_libera_no(S);
        strcpy(*raiz, S_arq);
        free(S_arq);
        return *raiz;
    }

    T = insere_nao_completo(T, jogador, t,cont);
    TABM aux;
    copia(T,&aux,t);
    fp = fopen(*raiz, "wb");
    fwrite(&aux,sizeof(TABM),1,fp);
    fclose(fp);
    TABM_libera_no(T);
    return *raiz;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          FUNÇÃO PARA REMOVER CHAVES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* TABM_remover(char* no, int ch, int t){
    char aux[20];
    strcpy(aux,TABM_busca(no,ch));
    if(strcmp(aux,"NULL") == 0) return no;
    FILE* fp = fopen(no,"rb+");
    if(!fp) return no;
    TABM arv;
    fread(&arv,sizeof(TABM),1,fp);
    fclose(fp);

    int i;
    for(i = 0; i < arv.nchaves && arv.chaves[i].id < ch; i++);

    if((i < arv.nchaves) && (ch == arv.chaves[i].id) && (arv.folha)){ //Caso 1
        //printf("\nCASO 1\n");
        int j;
        for(j = i; j < arv.nchaves-1;j++) arv.chaves[j] = copia_chaves(arv.chaves[j],arv.chaves[j+1]);
        arv.nchaves--;
        if(!arv.nchaves){
            remove(no);
        }
        else {
            fp = fopen(no,"wb");
            fwrite(&arv,sizeof(TABM),1,fp);
            fclose(fp);
        }
        return no;
    }

    if((i < arv.nchaves) && (ch == arv.chaves[i].id)) i++; //DAndo merda
    FILE* fy = fopen(arv.filhos[i],"rb"); 
    if(!fy) exit(1);
    TABM y,z; z.nchaves = 0; //Verificação para substituir o NULL;
    fread(&y,sizeof(TABM),1,fy);
    fclose(fy);

    if(y.nchaves == t-1){//Caso 3A e 3B
        if(i < arv.nchaves){
            FILE* fz = fopen(arv.filhos[i+1],"rb"); //Evitar segmentation fault
            TABM i_mais_1;
            fread(&i_mais_1,sizeof(TABM),1,fz);
            fclose(fz);
            if((i < arv.nchaves) && (i_mais_1.nchaves >= t)){ //Caso 3A direita
                //printf("\nCASO 3A: i menor que nchaves\n");
                copia(&i_mais_1,&z,t); //CHANCE DE MERDA
                if(!y.folha){
                    y.chaves[t-1] = copia_chaves(y.chaves[t-1],arv.chaves[i]);
                    arv.chaves[i] = copia_chaves(arv.chaves[i],z.chaves[0]);
                }
                else{
                    arv.chaves[i].id = z.chaves[0].id + 1;
                    y.chaves[t-1] = copia_chaves(y.chaves[t-1],z.chaves[0]);
                }
                y.nchaves++;

                int j;
                for(j = 0; j < z.nchaves-1; j++) z.chaves[j] = copia_chaves(z.chaves[j],z.chaves[j+1]);
                strcpy(y.filhos[y.nchaves],z.filhos[0]);
                for(j = 0; j < z.nchaves; j++) strcpy(z.filhos[j],z.filhos[j+1]);
                z.nchaves--;
                
                fp = fopen(no,"wb");
                fwrite(&arv,sizeof(TABM),1,fp);
                fclose(fp);

                fy = fopen(arv.filhos[i],"wb");
                fwrite(&y,sizeof(TABM),1,fy);
                fclose(fy);

                fz = fopen(arv.filhos[i+1],"wb");
                fwrite(&z,sizeof(TABM),1,fz);
                fclose(fz);

                strcpy(arv.filhos[i],TABM_remover(arv.filhos[i],ch,t));

                FILE* f_aux = fopen(arv.filhos[i],"rb");
                if(!f_aux){//Equivalente ao return NULL
                    strcpy(arv.filhos[i],"Filh"); 
                    fp = fopen(no,"wb");
                    fwrite(&arv,sizeof(TABM),1,fp);
                    fclose(fp);
                }
                else fclose(f_aux);

                return no;
            }
        }
        if(i > 0){//Evitar segmentation fault
            FILE* fz = fopen(arv.filhos[i-1],"rb");
            TABM i_menos_1;
            fread(&i_menos_1,sizeof(TABM),1,fz);
            fclose(fz);
            if((i > 0) && (z.nchaves == 0) && (i_menos_1.nchaves >= t)){//CASO 3A esquerda
                //printf("\nCASO 3A: i igual a nchaves\n");
                copia(&i_menos_1,&z,t); //z recebe i_menos_1

                int j;
                for(j = y.nchaves; j > 0; j--) y.chaves[j] = copia_chaves(y.chaves[j],y.chaves[j-1]);
                for(j = y.nchaves+1; j > 0; j--) strcpy(y.filhos[j],y.filhos[j-1]);

                if(!y.folha){
                    y.chaves[0] = copia_chaves(y.chaves[0],arv.chaves[i-1]);
                    arv.chaves[i-1] = copia_chaves(arv.chaves[i-1],z.chaves[z.nchaves-1]);
                }
                else{
                    arv.chaves[i-1] = copia_chaves(arv.chaves[i-1],z.chaves[z.nchaves-1]);
                    y.chaves[0] = copia_chaves(y.chaves[0],z.chaves[z.nchaves-1]);
                }
                y.nchaves++;

                strcpy(y.filhos[0],z.filhos[z.nchaves]);
                z.nchaves--;
                
                fp = fopen(no,"wb");
                fwrite(&arv,sizeof(TABM),1,fp);
                fclose(fp);

                fy = fopen(arv.filhos[i],"wb");
                fwrite(&y,sizeof(TABM),1,fy);
                fclose(fy);

                fz = fopen(arv.filhos[i-1],"wb");
                fwrite(&z,sizeof(TABM),1,fz);
                fclose(fz);

                strcpy(arv.filhos[i],TABM_remover(arv.filhos[i],ch,t)); //CHANCE DE MERDA, original: arv->filho[i] = remover(y, ch, t);
                FILE* f_aux = fopen(arv.filhos[i],"rb");
                if(!f_aux){//Equivalente ao return NULL
                    strcpy(arv.filhos[i],"Filh"); 
                    fp = fopen(no,"wb");
                    fwrite(&arv,sizeof(TABM),1,fp);
                    fclose(fp);
                }
                else fclose(f_aux);
                return no;
            }
        } 
        if(z.nchaves == 0){ //Caso 3B
            if(i < arv.nchaves){
                FILE* fz = fopen(arv.filhos[i+1],"rb"); //Evitar segmentation fault
                TABM i_mais_1;
                fread(&i_mais_1,sizeof(TABM),1,fz);
                fclose(fz);
                if(i < arv.nchaves && i_mais_1.nchaves == t-1){
                    //printf("\nCASO 3B: i menor que nchaves\n");
                    copia(&i_mais_1,&z,t); // z = arv.filho[i+1]
                    if(!y.folha){
                        y.chaves[t-1] = copia_chaves(y.chaves[t-1],arv.chaves[i]);
                        y.nchaves++;
                    }
                    int j = 0;
                    while(j < t-1){
                        if(!y.folha) y.chaves[t+j] = copia_chaves(y.chaves[t+j],z.chaves[j]);
                        else y.chaves[t+j-1] = copia_chaves(y.chaves[t+j-1],z.chaves[j]);
                        y.nchaves++;
                        j++;
                    }
                    strcpy(y.prox,z.prox);
                    if(!y.folha){
                        for(j = 0; j < t; j++){
                            strcpy(y.filhos[t+j],z.filhos[j]);
                            strcpy(z.filhos[j],"Filh");
                        }
                    }
                    remove(arv.filhos[i+1]); //libera(z)
                    for(j=i; j < arv.nchaves-1; j++){
                        arv.chaves[j] = copia_chaves(arv.chaves[j],arv.chaves[j+1]);
                        strcpy(arv.filhos[j+1],arv.filhos[j+2]);
                    }
                    strcpy(arv.filhos[arv.nchaves],"Filh");
                    arv.nchaves--;
                    if(!arv.nchaves){ //ALTISSIMAS CHANCES DE MERDA
                        remove(no);
                        strcpy(no,arv.filhos[0]);
                    }
                    fp = fopen(no,"wb");
                    fwrite(&arv,sizeof(TABM),1,fp);
                    fclose(fp);

                    fy = fopen(arv.filhos[i],"wb");
                    fwrite(&y,sizeof(TABM),1,fy);
                    fclose(fy);

                    strcpy(no,TABM_remover(no,ch,t));

                    FILE* f_aux = fopen(arv.filhos[i],"rb");
                    if(!f_aux){//Equivalente ao return NULL
                        strcpy(arv.filhos[i],"Filh"); 
                        fp = fopen(no,"wb");
                        fwrite(&arv,sizeof(TABM),1,fp);
                        fclose(fp);
                    }
                    else fclose(f_aux);
                    return no;
                }
            }
            if(i > 0 && z.nchaves == 0){
                FILE* fz = fopen(arv.filhos[i-1],"rb"); //Evitar segmentation fault
                TABM i_menos_1;
                fread(&i_menos_1,sizeof(TABM),1,fz);
                fclose(fz);
                if((i > 0) && (i_menos_1.nchaves == t-1)){
                    //printf("\nCASO 3B: i igual a nchaves\n");
                    copia(&i_menos_1,&z,t); //z = arv.filho[i-1]
                    if(!y.folha){
                        if(i == arv.nchaves){
                            z.chaves[t-1] = copia_chaves(z.chaves[t-1],arv.chaves[i-1]);
                        }else{
                            z.chaves[t-1] = copia_chaves(z.chaves[t-1],arv.chaves[i]);
                        }
                        z.nchaves++;
                    }
                    int j = 0;
                    while(j < t-1){
                        if(!y.folha) z.chaves[t+j] = copia_chaves(z.chaves[t+j],y.chaves[j]);
                        else z.chaves[t+j-1] = copia_chaves(z.chaves[t+j-1],y.chaves[j]);
                        z.nchaves++;
                        j++;
                    }
                    strcpy(z.prox,y.prox);
                    if(!z.folha){
                        for(j = 0; j < t; j++){
                            strcpy(z.filhos[t+j],y.filhos[j]);
                            strcpy(y.filhos[j],"Filh");
                        }
                    }
                    remove(arv.filhos[i]);
                    strcpy(arv.filhos[arv.nchaves],"Filh");
                    arv.nchaves--;
                    if(!arv.nchaves){//CHANCE ALTÌSSIMA DE MERDA
                        remove(no);
                        strcpy(no,arv.filhos[0]);
                    }
                    else{
                        fp = fopen(no,"wb");
                        fwrite(&arv,sizeof(TABM),1,fp);
                        fclose(fp);

                        // fy = fopen(arv.filhos[i],"wb");
                        // fwrite(&y,sizeof(TABM),1,fy);
                        // fclose(fy);

                        if(strcmp(arv.filhos[i-1],"Filh") != 0){
                            fz = fopen(arv.filhos[i-1],"wb");
                            fwrite(&z,sizeof(TABM),1,fz);
                            fclose(fz);
                        

                        i--;

                        strcpy(arv.filhos[i],TABM_remover(arv.filhos[i],ch,t));
                        FILE* f_aux = fopen(arv.filhos[i],"rb");
                        if(!f_aux){//Equivalente ao return NULL
                            strcpy(arv.filhos[i],"Filh"); 
                            fp = fopen(no,"wb");
                            fwrite(&arv,sizeof(TABM),1,fp);
                            fclose(fp);
                        }
                        else fclose(f_aux);
                        }
                    }
                    return no;
                }   
            }
        }
    }
    strcpy(arv.filhos[i],TABM_remover(arv.filhos[i],ch,t));
    FILE* f_aux = fopen(arv.filhos[i],"rb");
    if(!f_aux){//Equivalente ao return NULL
        strcpy(arv.filhos[i],"Filh"); 
        fp = fopen(no,"wb");
        fwrite(&arv,sizeof(TABM),1,fp);
        fclose(fp);
    }
    else fclose(f_aux);
    return no;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          CRIA O ARQUIVO "VAZIO"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TABM_cria(int t, int *cont, char ** arq){
    sprintf(*arq, "Arquivos/%04d.bin", (*cont));
    FILE * fp = fopen(*arq, "wb");
    if(!fp) exit(1);
    TABM novo;
    TJ jogador;
    novo.folha = 1;
    novo.nchaves = 0;
    novo.prox;
    strcpy(novo.prox, "Arquivos/Prox");
    for(int i = 0; i < (t*2)-1; i++) strcpy(novo.chaves[i].nome,"Chave");
    for(int i = 0; i < (t*2); i++) strcpy(novo.filhos[i],"Filh");
    fwrite(&novo, sizeof(TABM), 1, fp);
    fclose(fp);
    (*cont)++;
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          PRINTAR CONTEÚDO DO ARQUIVO BINÁRIO
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printa_arqb(char* entrada, int t){
    FILE* fp = fopen(entrada, "rb");
    TABM novo;
    fread(&novo, sizeof(TABM),1,fp);
    printf("\n\tArquivo atual: %s\n",entrada);
    printf("\n\tFolha: %d\n", novo.folha);
    printf("\n\tN Chaves: %d\n", novo.nchaves);
    printf("\n\tProximo: %s\n", novo.prox);
    for (int i = 0; i < novo.nchaves; i++) printf("\n\t%d/%d/%s/%s/%d %s %d (aged %d)/%d/%d/%s/%s\n", 
            novo.chaves[i].id, novo.chaves[i].num_camisa, novo.chaves[i].posicao, novo.chaves[i].nome, 
            novo.chaves[i].dia, novo.chaves[i].mes, novo.chaves[i].ano, novo.chaves[i].idade, novo.chaves[i].part_sel, 
            novo.chaves[i].gol_sel, novo.chaves[i].pais_time, novo.chaves[i].time);
    for (int i = 0; i < 2*t; i++) printf("\n\t%s\n",novo.filhos[i]);
    printf("\n\n\n");
    fclose(fp);
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          LÊ DADOS PARA ARQUIVO
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void le_dados(char * arquivo, char ** raiz, int t, int* cont){
    FILE * fp = fopen(arquivo, "r");
    if(!fp) exit(1);
    TJ *jogador = (TJ*)malloc(sizeof(TJ));
    int contar = 284;
    char tmp[30];
    while(fscanf(fp, "%s", tmp) == 1){ //lendo por seleção
        while(contar && (fscanf(fp, "%d/%d/%4[^/]/%30[^/]/%d %10s %d (aged %d)/%d/%d/%20[^/]/%30[^\n]", &jogador->id, &jogador->num_camisa, jogador->posicao, jogador->nome, &jogador->dia, jogador->mes, &jogador->ano, &jogador->idade, &jogador->part_sel, &jogador->gol_sel, jogador->pais_time, jogador->time) == 12)){ //lendo por jogador
            strcpy(jogador->sele,tmp);
            if(strstr(jogador->nome, "(captain)")) jogador->capitao = 1;
            else jogador->capitao = 0;
            for(int i=0; i<strlen(jogador->nome); i++){
                if(jogador->nome[i] == '(') jogador->nome[i-1] = '\0'; //tirando captain do nome
            }
            strcpy((*raiz),TABM_insere(jogador, t, raiz, cont));
            contar--;
        }
    }
    fclose(fp);
    free(jogador);
    return;
}
