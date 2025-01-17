#include "Funcoes.c"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void){
    int t, x,opcao=1,id,ano,mes;
    int cont = 1;
    char * raiz = (char*)malloc(sizeof(char)*30);
    mkdir("Arquivos", 0777);
    mkdir("Tabelas",0777);
    printf("Insira o valor de t para a construcao da arvore: ");
    scanf("%d", &t);
    if(t < 2) t = 2;
    le_dados("EURO.txt", &raiz, t, &cont);
    tabela_nacionalidade("EURO.txt");
    tabela_posicoes("EURO.txt");
    do{
        printf("\n\n\t0 - Sair\n\n\t1 ao 20 - Questoes\n\n\t21 - Menu");
        printf("\n");
        printf("\n\n\tDigite uma das opcoes: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            int m;
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\t1 - Geral\n\n\t2 - Nacionalidade\n\n\t3 - Posição");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            printf("\n\n\tDigite uma das opcoes: ");
            scanf("%d",&m);
            if(m == 1){
                printf("\n\n\t=====================================================================================================");
                printf("\n\tJOGADORES MAIS VELHO E MAIS NOVO DA COMPETIÇÂO: \n");
                Q1_geral(raiz);
                printf("\n\n\t=====================================================================================================");
                printf("\n");
            }
            if(m == 2){
                printf("\n\n\t=====================================================================================================");
                printf("\n\tJOGADORES MAIS VELHO E MAIS NOVO DE CADA SELEÇÃO: \n");
                Q1_nacionalidade("Tabelas/Nacionalidades.bin",raiz);
                printf("\n\n\t=====================================================================================================");
                printf("\n");
            }
            if(m ==3){
                printf("\n\n\t=====================================================================================================");
                printf("\n\tJOGADORES MAIS VELHO E MAIS NOVO DE CADA POSIÇÃO: \n");
                Q1_posicao("Tabelas/Posições.bin",raiz);
                printf("\n\n\t=====================================================================================================");
                printf("\n");
            }
            break;
        case 2:
            printf("\n\n\t=====================================================================================================");
            printf("\n\tJOGADORES COM MAIS OU MENOS JOGOS EM CADA SELECAO: \n");
            Q2("Tabelas/Nacionalidades.bin",raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 3:
            printf("\n\n\t=====================================================================================================");
            printf("\n\tJOGADORES COM MAIS OU MENOS JOGOS NO GERAL: \n");
            Q3(raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 4:
            printf("\n\n\t=====================================================================================================");
            printf("\n\tSELECAO COM MAIS OU MENOS JOGADORES: \n");
            Q4("Tabelas/Nacionalidades.bin");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 5:
            printf("\n\n\t=====================================================================================================");
            printf("\n\tJOGADORES QUE ATUAM FORA DO SEU PAIS DE ORIGEM: \n");
            Q5("Tabelas/Nacionalidades.bin",raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 6:
            printf("\n\n\t=====================================================================================================");
            printf("\n\tJOGADORES QUE ATUAM NO SEU PAIS DE ORIGEM: \n");
            Q6("Tabelas/Nacionalidades.bin",raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 7:
            printf("\n\tDigite o ano: ");
            scanf("%d",&ano);
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJogadores nascidos em %d:", ano);
            Q7(raiz,ano);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 8:
            printf("\n\tDigite o mes (numerico): ");
            scanf("%d",&mes);
            char Mes[21];
            if(mes == 1) strcpy(Mes,"January");
            if(mes == 2) strcpy(Mes,"February");
            if(mes == 3) strcpy(Mes,"March");
            if(mes == 4) strcpy(Mes,"April");
            if(mes == 5) strcpy(Mes,"May");
            if(mes == 6) strcpy(Mes,"June");
            if(mes == 7) strcpy(Mes,"July");
            if(mes == 8) strcpy(Mes,"August");
            if(mes == 9) strcpy(Mes,"September");
            if(mes == 10) strcpy(Mes,"October");
            if(mes == 11) strcpy(Mes,"November");
            if(mes == 12) strcpy(Mes,"December");
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES NASCIDOS EM %s:", Mes);
            Q8(raiz,Mes);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            printf("\n");
            break;
        case 9:
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tSELEÇÃO COM JOGADORES QUE MAIS ATUAM FORA DO PAÍS DE ORIGEM: \n");
            Q9("Tabelas/Nacionalidades.bin",raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 10:
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tSELEÇÃO COM JOGADORES QUE MAIS ATUAM NO PAÍS DE ORIGEM: \n");
            Q10("Tabelas/Nacionalidades.bin",raiz);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 11:
            printf("\tDigite o id para obter as infos subordinadas a ele: ");
            scanf("%d",&id);
            printf("\n\n\t=====================================================================================================");
            Q11(raiz,id);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 12:
            int resp = 1;
            do{
                printf("\n\n\t=====================================================================================================");
                printf("\n\n\t0 - Sair\n\n\t1 - Numero da camisa\n\n\t2 - Posicao\n\n\t3 - Idade\n\n\t4 - Partidas pela selecao\n\n\t5 - Gols pela selecao\n\n\t6 - Time\n\n\t7 - Capitao");
                printf("\n\n\t=====================================================================================================");
                printf("\n");
                printf("\n\n\tDigite uma das opcoes: ");
                scanf("%d", &resp);
                switch (resp){
                case 1:
                    int novo_num;
                    printf("\tDigite o id do jogador e o novo numero da camisa: ");
                    scanf("%d %d",&id,&novo_num);
                    printf("\n\n\t=====================================================================================================");
                    Q12_1(raiz,id,novo_num);
                    break;
                case 2:
                    char nova_pos[20];
                    printf("\tDigite o id do jogador e a nova posicao: ");
                    scanf("%d %s",&id,nova_pos);
                    printf("\n\n\t=====================================================================================================");
                    Q12_2(raiz,id,nova_pos);
                    break;
                case 3:
                   int nova_idade;
                    printf("\tDigite o id do jogador e a nova idade: ");
                    scanf("%d %d",&id,&nova_idade);
                    printf("\n\n\t=====================================================================================================");
                    Q12_3(raiz,id,nova_idade);
                    break;
                case 4:
                    int nova_qtd;
                    printf("\tDigite o id do jogador e a nova quantidade de partidas: ");
                    scanf("%d %d",&id,&nova_qtd);
                    printf("\n\n\t=====================================================================================================");
                    Q12_4(raiz,id,nova_qtd);
                    break;
                case 5:
                    int nova_qtd_gols;
                    printf("\tDigite o id do jogador e a nova quantidade de partidas: ");
                    scanf("%d %d",&id,&nova_qtd_gols);
                    printf("\n\n\t=====================================================================================================");
                    Q12_5(raiz,id,nova_qtd_gols);
                    break;
                case 6:
                    char novo_time[20];
                    printf("\tDigite o id do jogador e o novo time: ");
                    scanf("%d %s",&id,novo_time);
                    printf("\n\n\t=====================================================================================================");
                    Q12_6(raiz,id,novo_time);
                    break;
                case 7:
                    int op;
                    printf("\tDigite o id e 1 para tornar e 0 para retirar: ");
                    scanf("%d %d", &id, &op);
                    printf("\n\n\t=====================================================================================================");
                    Q12_7(raiz, id, op, t);
                    break;  
                default:
                    if(resp != 0) printf("\tOpcao invalida!!!\n");
                    break;      
                }

            }while(resp);
            break;
        case 13:
            char pais[20];
            int p;
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\t1 - Germany\n\n\t2 - Scotland\n\n\t3 - Croatia\n\n\t4 - Albania\n\n\t5 - Slovenia\n\n\t6 - Denmark\n\n\t7 - Netherlands\n\n\t8 - France\n\n\t9 - Ukraine\n\n\t10 - Georgia\n\n\t11 - Portugal");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            printf("\n\n\tDigite uma das opcoes: ");
            scanf("%d",&p);
            if(p == 1) strcpy(pais,"Germany");
            if(p == 2) strcpy(pais,"Scotland");
            if(p == 3) strcpy(pais,"Croatia");
            if(p == 4) strcpy(pais,"Albania");
            if(p == 5) strcpy(pais,"Slovenia");
            if(p == 6) strcpy(pais,"Denmark");
            if(p == 7) strcpy(pais,"Netherlands");
            if(p == 8) strcpy(pais,"France");
            if(p == 9) strcpy(pais,"Ukraine");
            if(p == 10) strcpy(pais,"Georgia");
            if(p == 11) strcpy(pais,"Portugal");
            printf("\n\n\t=====================================================================================================");
            Q13(raiz,pais,"Tabelas/Nacionalidades.bin");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 14:
            int e;
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\t1 - Busca dos capitães\n\n\t2 - Remoção");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            printf("\n\n\tDigite uma das opcoes: ");
            scanf("%d",&e);
            printf("\n\n\t=====================================================================================================");
            if(e == 2) {
                printf("\n\n\tREMOVENDO TODOS OS ATUAIS CAPITÃES");
                strcpy(raiz,Q14_R(raiz,t));
                printf("\n\n\tTABELAS ATUALIZADAS COM SUCESSO!");
            }
            else {
                printf("\n\n\tCAPITÃES E SUAS RESPECTIVAS SELEÇÕES: ");
                Q14_B(raiz);
            }
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 15:
            int idade;
            printf("\tDigite a idade: ");
            scanf("%d",&idade);
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES COM MAIS QUE %d ANOS EXCLUÍDOS COM SUCESSO!\n",idade);
            Q15(raiz,idade,t);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 16:
            char paiis[20], sele[20];
            printf("\tDigite o seleção: ");
            scanf("%s",sele);
            printf("\tDigite o pais: ");
            scanf("%s",paiis);
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES DO(A) %s QUE JOGAM NO(A) %s EXCLUÍDOS COM SUCESSO!\n",sele,paiis);
            Q16(raiz,t,sele,paiis);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 17:
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES NÃO ATUAM NO PAÍS DE ORIGEM EXCLUÍDOS COM SUCESSO!\n");
            Q17(raiz,t);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 18:
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES ATUAM NO PAÍS DE ORIGEM EXCLUÍDOS COM SUCESSO!\n");
            Q18(raiz,t);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 19:
            char selee[20];
            printf("\tDigite o seleção: ");
            scanf("%s",selee);
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES DO(A) %s EXCLUÍDOS COM SUCESSO!\n",selee);
            Q19(raiz,t,selee);
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 20:
            Q20(raiz,t);
            printf("\n\n\t=====================================================================================================");
            printf("\n\n\tJOGADORES DO CONJUNTO EXCLUÍDOS COM SUCESSO!");
            printf("\n\n\t=====================================================================================================");
            printf("\n");
            break;
        case 21:
            printf("\n");
            printf("\t=====================================================================================================");
            printf("\n\n\t1 - Os jogador mais novo e mais velho\n\n\t2 - OS jogadores que mais e menos atuaram por suas equipes\n\n\t3 - Jogadores que mais e menos atuaram no geral\n\n\t4 - A maior e menor seleção\n\n\t5 - Busca de jogadores que atuam fora do seu país de origem\n\n\t6 - Busca de jogadores que atuam no seu país de origem\n\n\t7 - Busca de jogadores que nasceram no mesmo ano\n\n\t8 - Busca de jogadores que nasceram no mesmo mês\n\n\t9 - Busca das seleções com mais jogadores que atuam fora do seu páis de origem\n\n\t10 - Busca das seleções com mais jogadores que atuam no seu país de origem\n\n\t11 - Dados de um jogador pelo ID\n\n\t12 - Alteração de informações\n\n\t13 - Busca de todos os jogadores de uma seleção\n\n\t14 - Busca e remoção de todos os capitães\n\n\t15 - Remoção de jogadores a partir de uma determinada idade\n\n\t16 - Retirada de todos os jogadores de uma seleção que atuam num determinado país\n\n\t17 - Retirada de todos os os jogadores de uma seleção que não atuam no país de origem\n\n\t18 - Retirada de todos os os jogadores de uma seleção atuam no país de origem\n\n\t19 - Retirada de todos os os jogadores de uma seleção\n\n\t20 - Remoção de jogadores, dado um conjunto contendo suas chaves primárias.");
            printf("\n\n");
            printf("\t=====================================================================================================");
            break;
        case 22:
            int arq;
            char nome[20];
            printf("Digite o numero do arquivo: ");
            scanf("%d",&arq);
            sprintf(nome, "Arquivos/%04d.bin", arq);
            printa_arqb(nome,t);
            break;
        case 23:
            printf("\n");
            printf("\t=====================================================================================================");
            printa_nacionalidades("Tabelas/Nacionalidades.bin");
            printf("\n\n");
            printf("\t=====================================================================================================");
            break;
        case 24:
            printf("\n");
            printf("\t=====================================================================================================");
            printa_posicoes("Tabelas/Posições.bin");
            printf("\n\n");
            printf("\t=====================================================================================================");
            break;
        case 25:
            TABM_imprime(&raiz,t);
            break;
        default:
            if(opcao != 0) printf("Opcao invalida!!!\n");
            break;
        }
    } while (opcao);


    free(raiz);

    system("rm -fr Arquivos");
    system("rm -rf Tabelas");
    return 0;
}