/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. Almeida
 *
 * Trabalho 02 - Árvore-B
 *
 * ========================================================================== *
 *   <<< IMPLEMENTE AQUI!!! COPIE E COLE O CONTEÚDO DESTE ARQUIVO NO AVA >>>
 * ========================================================================== */

/* Bibliotecas */
#include "ORI_T02_HEADER.h"

/* ===========================================================================
 * ================================= FUNÇÕES ================================= */
/* <<< COLOQUE AQUI OS DEMAIS PROTÓTIPOS DE FUNÇÕES, SE NECESSÁRIO >>> */
bool verificar_kit_jogador(Jogador *jogador, char *idKitJogadorAtual); 
//FALTA LISTA INVERTIDA E RECOMPENSAR VENCEDOR


/* Compara a chave (key) com cada elemento do índice (elem).
 * Funções auxiliares para o buscar e inserir chaves em Árvores-B.
 * Note que, desta vez, as funções comparam chaves no formato string, e não struct.
 * "key" é a chave do tipo string que está sendo buscada ou inserida. 
 * "elem" é uma chave do tipo string da struct btree_node.
 *
 * Dica: consulte sobre as funções strncmp() e strnlen(). Muito provavelmente, você vai querer utilizá-las no código.
 * */

/* Função de comparação entre chaves do índice jogadores_idx */
int order_jogadores_idx(const void *key, const void *elem) {
	return strncmp(key, elem, TAM_ID_JOGADOR-1);
}

/* Função de comparação entre chaves do índice kits_idx */
int order_kits_idx(const void *key, const void *elem) {
	return strncmp(key, elem, TAM_ID_KIT-1);
}

/* Função de comparação entre chaves do índice partidas_idx */
int order_partidas_idx(const void *key, const void *elem) {	
	return strncmp(key, elem, TAM_ID_PARTIDA-1);
}

/* Função de comparação entre chaves do índice resultados_idx */
int order_resultados_idx(const void *key, const void *elem) {	
	if(strncmp(key, elem, TAM_ID_JOGADOR-1) == 0){
		return strncmp(key + TAM_ID_JOGADOR-1, elem + TAM_ID_JOGADOR-1, TAM_ID_PARTIDA-1);
	}
	else{
		return strncmp(key, elem, TAM_ID_JOGADOR-1);;
	}
}

/* Função de comparação entre chaves do índice preco_kit_idx */
int order_preco_kit_idx(const void *key, const void *elem){
	if(strncmp(key, elem, TAM_FLOAT_NUMBER-1) == 0){
		return strncmp(key + TAM_FLOAT_NUMBER-1, elem + TAM_FLOAT_NUMBER-1, TAM_ID_KIT-1);
	}
	else{
		return strncmp(key, elem, TAM_FLOAT_NUMBER-1);
	}	
}

/* Função de comparação entre chaves do índice data_partida_idx */
int order_data_partida_idx(const void *key, const void *elem) {
	if(strncmp(key, elem, TAM_DATETIME-1) == 0){
		return strncmp(key + TAM_DATETIME-1, elem + TAM_DATETIME-1, TAM_ID_PARTIDA-1);
	}
	else{
		return strncmp(key, elem, TAM_DATETIME-1);
	}
}

/* Função de comparação entre chaves do índice jogador_kits_idx */
int order_jogador_kit_idx(const void *key, const void *elem) {
	return strncmp(key, elem, TAM_CHAVE_JOGADOR_KIT_SECUNDARIO_IDX);
}

/* Função de comparação entre vitórias, eliminacoes e tempo de sobrevivencia dos jogadores
 * Usada na função recompensar_vencedores_periodo; */
int qsort_info_jogador(const void *a, const void *b) {    
    Info_Jogador *j1 = (Info_Jogador *)a;
    Info_Jogador *j2 = (Info_Jogador *)b;

    if (j1->vitorias != j2->vitorias) {
        return j1->vitorias - j2->vitorias;
    }

    if (j1->eliminacoes != j2->eliminacoes) {
        return j1->eliminacoes - j2->eliminacoes;
    }
    
    if (strcmp(j1->sobrevivencia, j2->sobrevivencia) != 0) {
        if (strcmp(j1->sobrevivencia, j2->sobrevivencia) < 0) {
            return 1;
        } else {
            return -1;
        }
    }
    
    return strcmp(j1->id_jogador, j2->id_jogador);
}




/* Cria o índice respectivo */
void criar_jogadores_idx() {
	char str[TAM_CHAVE_JOGADORES_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_jogadores; ++i) {
		Jogador z = recuperar_registro_jogador(i);

		sprintf(str, "%s%04d", z.id_jogador, i);
		btree_insert(str, &jogadores_idx);
	}
	printf(INDICE_CRIADO, "jogadores_idx");
}


void criar_kits_idx() {
	char str[TAM_CHAVE_KITS_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_kits; ++i) {
		Kit k = recuperar_registro_kit(i);

		sprintf(str, "%s%04d", k.id_kit, i);
		btree_insert(str, &kits_idx);
	}
	printf(INDICE_CRIADO, "kits_idx");
}


void criar_partidas_idx() {
	char str[TAM_CHAVE_PARTIDAS_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_partidas; ++i) {
		Partida p = recuperar_registro_partida(i);

		sprintf(str, "%s%04d", p.id_partida, i);
		btree_insert(str, &partidas_idx);
	}
	printf(INDICE_CRIADO, "partidas_idx");
}


void criar_resultados_idx(){
	char str[TAM_CHAVE_RESULTADOS_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_resultados; ++i) {
		Resultado r = recuperar_registro_resultado(i);

		sprintf(str, "%s%s%04d", r.id_jogador, r.id_partida, i);
		btree_insert(str, &resultados_idx);
	}
	printf(INDICE_CRIADO, "resultados_idx");
}


void criar_preco_kit_idx() {
	char str[TAM_CHAVE_PRECO_KIT_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_kits; ++i) {
		Kit k = recuperar_registro_kit(i);

		sprintf(str, "%013.2lf%s", k.preco, k.id_kit);
		btree_insert(str, &preco_kit_idx);
	}
	printf(INDICE_CRIADO, "preco_kit_idx");
}


void criar_data_partida_idx() {
	char str[TAM_CHAVE_DATA_PARTIDA_IDX + 1];
	for (unsigned i = 0; i < qtd_registros_partidas; ++i) {
		Partida p = recuperar_registro_partida(i);

		sprintf(str, "%s%s", p.inicio, p.id_partida);
		btree_insert(str, &data_partida_idx);
	}
	printf(INDICE_CRIADO, "data_partida_idx");
}

void criar_jogador_kits_idx() { 
	for (unsigned i = 0; i < qtd_registros_jogadores; ++i)
	{
		Jogador jogador = recuperar_registro_jogador(i);

		for (unsigned i = 0; i < QTD_MAX_KITS; i++)
		{
			if (jogador.kits[i][0] != '\0')
			{				
				jogador.kits[i][TAM_ID_KIT-1] = '\0';				
				inverted_list_insert(jogador.kits[i], jogador.id_jogador, &jogador_kits_idx);
			}
		}
	}

	printf(INDICE_CRIADO, "jogador_kits_idx");
}



/* Recupera do arquivo o registro com o RRN informado
 * e retorna os dados nas structs */
Jogador recuperar_registro_jogador(int rrn) {
	Jogador j;
	char data[TAM_REGISTRO_JOGADOR + 1], *p;

	strncpy(data, ARQUIVO_JOGADORES + (rrn * TAM_REGISTRO_JOGADOR), TAM_REGISTRO_JOGADOR);
	data[TAM_REGISTRO_JOGADOR] = '\0';

	p = strtok(data, ";");
	strcpy(j.id_jogador, p);

	p = strtok(NULL, ";");
	strcpy(j.apelido, p);

	p = strtok(NULL, ";");
	strcpy(j.cadastro, p);

	p = strtok(NULL, ";");
	strcpy(j.premio, p);

	p = strtok(NULL, ";");
	j.saldo = atof(p);

	p = strtok(NULL, ";");

	for (int i = 0; i < QTD_MAX_KITS; ++i) {
		memset (j.kits[i], 0, TAM_ID_KIT);
	}

	if (p[0] != '#') {
		p = strtok(p, "|");
		for (int i = 0; p; p = strtok(NULL, "|"), ++i) {
			strncpy(j.kits[i], p, TAM_ID_KIT-1);
		}
	}
	
	return j;
}


Kit recuperar_registro_kit(int rrn) {
    Kit k;
    char data[TAM_REGISTRO_KIT + 1], *p;

    strncpy(data, ARQUIVO_KITS + (rrn * TAM_REGISTRO_KIT), TAM_REGISTRO_KIT);
    data[TAM_REGISTRO_KIT] = '\0';
    
    p = strtok(data, ";");
    strcpy(k.id_kit, p);

    p = strtok(NULL, ";");
    strcpy(k.nome, p);

    p = strtok(NULL, ";");
    strcpy(k.poder, p);

    p = strtok(NULL, ";");
    k.preco = atof(p);

    return k;
}


Partida recuperar_registro_partida(int rrn) {
	Partida p;
	char *data = ARQUIVO_PARTIDAS + (rrn * TAM_REGISTRO_PARTIDA);
	
	strncpy(p.id_partida, data, TAM_ID_PARTIDA - 1);
	data += (TAM_ID_PARTIDA - 1);
	p.id_partida[TAM_ID_PARTIDA - 1] = '\0';

	strncpy(p.inicio, data, TAM_DATETIME - 1);
	data += (TAM_DATETIME - 1);
	p.inicio[TAM_DATETIME - 1] = '\0';

	strncpy(p.duracao, data, TAM_TIME - 1);
	data += (TAM_TIME - 1);
	p.duracao[TAM_TIME - 1] = '\0';

	strncpy(p.cenario, data, TAM_CENARIO - 1);
	data += (TAM_CENARIO - 1);
	p.cenario[TAM_CENARIO - 1] = '\0';

	strncpy(p.id_jogadores, data, TAM_ID_JOGADORES - 1);
	p.id_jogadores[TAM_ID_JOGADORES - 1] = '\0';

	return p;
}


Resultado recuperar_registro_resultado(int rrn) {
	Resultado r;
	char *data = ARQUIVO_RESULTADOS + (rrn * TAM_REGISTRO_RESULTADO);
	char temp[5];

	strncpy(r.id_jogador, data, TAM_ID_JOGADOR - 1);
	data += (TAM_ID_JOGADOR - 1);
	r.id_jogador[TAM_ID_JOGADOR - 1] = '\0';

	strncpy(r.id_partida, data, TAM_ID_PARTIDA - 1);
	data += (TAM_ID_PARTIDA - 1);
	r.id_partida[TAM_ID_PARTIDA - 1] = '\0';

	strncpy(r.id_kit, data, TAM_ID_KIT - 1);
	data += (TAM_ID_KIT - 1);
	r.id_kit[TAM_ID_KIT - 1] = '\0';

	strncpy(temp, data, 4);
	r.colocacao = atoi(temp);
	data += 4;

	strncpy(r.sobrevivencia, data, TAM_TIME - 1);
	data += (TAM_TIME - 1);
	r.sobrevivencia[TAM_TIME - 1] = '\0';

	strncpy(temp, data, 4);
	r.eliminacoes = atoi(temp);

	return r;
	
}

/* Escreve em seu respectivo arquivo na posição informada (RRN) */
void escrever_registro_jogador(Jogador j, int rrn) {
	char data[TAM_REGISTRO_JOGADOR + 1], number[100];
	data[0] = '\0'; number[0] = '\0';

	strcat(data, j.id_jogador);
	strcat(data, ";");
	strcat(data, j.apelido);
	strcat(data, ";");
	strcat(data, j.cadastro);
	strcat(data, ";");
	strcat(data, j.premio);
	strcat(data, ";");
	sprintf(number, "%013.2lf", j.saldo);
	strcat (data, number);
	strcat(data, ";");

	for (int i = 0; i < QTD_MAX_KITS; ++i) {
		if (strlen(j.kits[i]) > 0) {
			if (i != 0)
				strcat (data, "|");
			strcat(data, j.kits[i]);
		}
	}
	strcat(data, ";");


	strpadright(data, '#', TAM_REGISTRO_JOGADOR);

	strncpy(ARQUIVO_JOGADORES + rrn*TAM_REGISTRO_JOGADOR, data, TAM_REGISTRO_JOGADOR);
}

void escrever_registro_kit(Kit k, int rrn) {
    char data[TAM_REGISTRO_KIT + 1], number[100];
    data[0] = '\0'; 

    
    strcat(data, k.id_kit);
    strcat(data, ";");
    strcat(data, k.nome);
    strcat(data, ";");
    strcat(data, k.poder);
    strcat(data, ";");
    
    sprintf(number, "%013.2lf", k.preco);
    strcat(data, number);
    strcat(data, ";");
    
    strpadright(data, '#', TAM_REGISTRO_KIT);
    
    strncpy(ARQUIVO_KITS + rrn * TAM_REGISTRO_KIT, data, TAM_REGISTRO_KIT);
}


void escrever_registro_partida(Partida p, int rrn) {
    char data[TAM_REGISTRO_PARTIDA + 1]; 
    data[0] = '\0';   
   
    strncat(data, p.id_partida, TAM_ID_PARTIDA - 1);    
    strncat(data, p.inicio, TAM_DATETIME - 1);       
    strncat(data, p.duracao, TAM_TIME - 1);       
    strncat(data, p.cenario, TAM_CENARIO - 1);
	strncat(data, p.id_jogadores, TAM_ID_JOGADORES - 1);    
    

    strncpy(ARQUIVO_PARTIDAS + rrn * TAM_REGISTRO_PARTIDA, data, TAM_REGISTRO_PARTIDA);
}


void escrever_registro_resultado(Resultado jp, int rrn) {
	char data[TAM_REGISTRO_RESULTADO + 1]; 
	data[0] = '\0';
	char aux[5];	

	strncat(data, jp.id_jogador, 11); 	
	strncat(data, jp.id_partida, 8);
	strncat(data, jp.id_kit, 3);

	snprintf(aux, sizeof(aux), "%04d", jp.colocacao);
	strncat(data, aux, 4); 

   strncat(data, jp.sobrevivencia, 6);

   snprintf(aux, sizeof(aux), "%04d", jp.eliminacoes);
   strncat(data, aux, 4);

   strncpy(ARQUIVO_RESULTADOS + rrn * TAM_REGISTRO_RESULTADO, data, TAM_REGISTRO_RESULTADO);

}
    

/* Exibe um registro com base no RRN */
bool exibir_jogador(int rrn) {
	if (rrn < 0)
		return false;

	Jogador j = recuperar_registro_jogador(rrn);

	printf ("%s, %s, %s, %s, %.2lf\n", j.id_jogador, j.apelido, j.cadastro, j.premio, j.saldo);

	return true;
}

bool exibir_kit(int rrn) {
	if (rrn < 0)
		return false;

	Kit kit = recuperar_registro_kit(rrn);

	printf("%s, %s, %s, %.2lf\n", kit.id_kit, kit.nome, kit.poder, kit.preco);

	return true;
}

bool exibir_partida(int rrn) {
	if (rrn < 0)
		return false;

	Partida partida = recuperar_registro_partida(rrn);

	printf("%s, %s, %s, %s, %s\n", partida.id_partida, partida.inicio, partida.duracao, partida.cenario, partida.id_jogadores);

	return true;
}


/* Exibe um registro com base na chave de um btree_node */
bool exibir_btree_jogadores(char *chave) {
	char num [5];
	memset (num, 0, 5);
	memcpy (num, chave + TAM_ID_JOGADOR-1, 4);

	int rrn = strtol(num, NULL, 10);
    return exibir_jogador(rrn);
}

bool exibir_btree_kits(char *chave) {
	char num [5];
	memset (num, 0, 5);
	memcpy (num, chave + TAM_ID_KIT-1, 4);

	int rrn = strtol(num, NULL, 10);
    return exibir_kit(rrn);
}

bool exibir_btree_partidas(char *chave) {
	char num [5];
	memset (num, 0, 5);
	memcpy (num, chave + TAM_ID_PARTIDA-1, 4);

	int rrn = strtol(num, NULL, 10);
    return exibir_partida(rrn);
}

bool exibir_btree_preco_kit(char *chave) {
	int rrn = atoi(chave + TAM_FLOAT_NUMBER - 1);
	return exibir_kit(rrn);
}

bool exibir_btree_data_partida(char *chave) {
    // Extrair o ID da partida da chave fornecida
    char id_partida[TAM_ID_PARTIDA] = {'\0'};
    strncpy(id_partida, chave + TAM_DATETIME - 1, TAM_ID_PARTIDA - 1);
    id_partida[TAM_ID_PARTIDA - 1] = '\0';

    // Buffer para armazenar a chave encontrada
    char partida_str[TAM_CHAVE_PARTIDAS_IDX + 1] = {'\0'};

    // Buscar a chave na árvore B
    if (!btree_search(partida_str, false, id_partida, partidas_idx.rrn_raiz, &partidas_idx)) {
        // Se a chave não for encontrada, retorna falso
        return false;
    }

    // Obter o RRN a partir da chave encontrada
    int rrn = atoi(partida_str + TAM_ID_PARTIDA - 1);

    // Exibir a partida correspondente ao RRN
    return exibir_partida(rrn);
}


/* Funções principais */
void cadastrar_jogador_menu(char *id_jogador, char *apelido) {
	bool found = btree_search(NULL, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);

	if(found){
		printf(ERRO_PK_REPETIDA, id_jogador);
		return;
	}
	
	Jogador newPlayer;
	char data[TAM_DATETIME];
	current_datetime(data);

	strcpy(newPlayer.id_jogador, id_jogador);
    strcpy(newPlayer.apelido, apelido);
    strcpy(newPlayer.cadastro, data);
    strcpy(newPlayer.premio, "000000000000");
	newPlayer.saldo = 0.00;

	memset(newPlayer.kits, '\0', sizeof(newPlayer.kits));

	escrever_registro_jogador(newPlayer, qtd_registros_jogadores);

	char jogador_str[TAM_CHAVE_JOGADORES_IDX + 1];

	sprintf(jogador_str, "%s%04d", id_jogador, qtd_registros_jogadores);
	jogador_str[TAM_CHAVE_JOGADORES_IDX] = '\0';

	btree_insert(jogador_str, &jogadores_idx);

	qtd_registros_jogadores++;
	printf(SUCESSO);

}


void remover_jogador_menu(char *id_jogador) {
	
	char jogador_str[TAM_CHAVE_JOGADORES_IDX + 1];
	bool foundJogador = btree_search(jogador_str, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);

	if(!foundJogador){
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}
	
	int rrnJogador = atoi(jogador_str + TAM_ID_JOGADOR - 1);
	Jogador player = recuperar_registro_jogador(rrnJogador);
	char aux[2] = "*|";
	strncpy(player.id_jogador, aux, 2);

	escrever_registro_jogador(player, rrnJogador);

	btree_delete(jogador_str, &jogadores_idx);

	printf(SUCESSO);

}


void adicionar_saldo_menu(char *id_jogador, double valor) {
	adicionar_saldo(id_jogador, valor, true);
}


void adicionar_saldo(char *id_jogador, double valor, bool flag){
	if (valor <= 0){
		printf(ERRO_VALOR_INVALIDO);
		return;
	}

	char jogador_str[TAM_CHAVE_JOGADORES_IDX + 1];
	jogador_str[TAM_CHAVE_JOGADORES_IDX] = '\0';

	bool found = btree_search(jogador_str, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);

	if(!found){
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}

	char stringJogador[TAM_RRN_REGISTRO + 1];
	stringJogador[0] = '\0';

	strncpy(stringJogador, jogador_str + TAM_ID_JOGADOR - 1, 4);
	stringJogador[4] = '\0';
	
	int rrnJogador = atoi(stringJogador);
	Jogador player = recuperar_registro_jogador(rrnJogador);
	player.saldo += valor;
	escrever_registro_jogador(player, rrnJogador);

	printf(SUCESSO);


}


void cadastrar_kit_menu(char *nome, char *poder, double preco) {

	char kitID[4];
	sprintf(kitID, "%03d", qtd_registros_kits);	

	Kit novoKit;
	strcpy(novoKit.id_kit, kitID);
    strcpy(novoKit.nome, nome);
    strcpy(novoKit.poder, poder);
    novoKit.preco = preco;

	escrever_registro_kit(novoKit, qtd_registros_kits);

	char kit_str[TAM_CHAVE_KITS_IDX + 1];
	kit_str[0] = '\0';

	sprintf(kit_str, "%s%04d", kitID, qtd_registros_kits);
	kit_str[TAM_CHAVE_KITS_IDX] = '\0';

	char preco_str[TAM_CHAVE_PRECO_KIT_IDX + 1];
	preco_str[0] = '\0';

	sprintf(preco_str, "%013.2lf%s", preco, kitID);
	preco_str[TAM_CHAVE_PRECO_KIT_IDX] = '\0';

	btree_insert(kit_str, &kits_idx);
	btree_insert(preco_str, &preco_kit_idx);

	qtd_registros_kits++;

	printf(SUCESSO);

	
}


void comprar_kit_menu(char *id_jogador, char *id_kit) {

	char player_str[TAM_CHAVE_JOGADORES_IDX + 1];
	char kit_str[TAM_CHAVE_KITS_IDX + 1];

	int rrnJogador;
	int rrnKit;

	bool foundJogador = btree_search(player_str, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);	
	bool foundKit = btree_search(kit_str, false, id_kit, kits_idx.rrn_raiz, &kits_idx);	

	if(!foundJogador || !foundKit){
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}
	else{
		rrnJogador = atoi(player_str + TAM_ID_JOGADOR - 1);
		rrnKit = atoi(kit_str + TAM_ID_KIT - 1);

		Jogador player = recuperar_registro_jogador(rrnJogador);
		Kit kit = recuperar_registro_kit(rrnKit);

		if(player.saldo < kit.preco){
			printf(ERRO_SALDO_NAO_SUFICIENTE);
			return;
		}
		else{
			for(int i = 0; i < QTD_MAX_KITS; i++){
				if(!strcmp(player.kits[i], kit.id_kit)){
					printf(ERRO_KIT_REPETIDO, player.id_jogador, kit.id_kit);
				    return;
				}
				if(player.kits[i][0] == '\0')
					break;	
			}

			for(int i = 0; i < QTD_MAX_KITS; i++){
				if(player.kits[i][0] == '\0'){
					strcpy(player.kits[i], kit.id_kit);
					break;
				}
			}
			player.saldo -= kit.preco;
			//inverted_list_insert(id_kit, player.id_jogador, &jogador_kits_idx);
			escrever_registro_jogador(player, rrnJogador);
		}
	}
	printf(SUCESSO);
}


void executar_partida_menu(char *inicio, char *duracao, char *cenario, char *id_jogadores, char *kits_jogadores, char *duracoes_jogadores, char *eliminacoes_jogadores) {
    //adaptada do t01 com algumas modificações
    Partida partida;
    char idPartidaFormatado[9];
    sprintf(idPartidaFormatado, "%08d", qtd_registros_partidas);
    strcpy(partida.id_partida, idPartidaFormatado);

    for(int i = 0; i < QTD_MAX_JOGADORES; i++){

        char jogador_str[TAM_ID_JOGADOR];
        strncpy(jogador_str, id_jogadores + i * 11, TAM_ID_JOGADOR - 1);
        jogador_str[TAM_ID_JOGADOR - 1] = '\0';

        char player[TAM_CHAVE_JOGADORES_IDX + 1] = {'\0'};
        bool foundJogador = btree_search(player, false, jogador_str, jogadores_idx.rrn_raiz, &jogadores_idx);
        player[TAM_CHAVE_JOGADORES_IDX] = '\0';

        int rrnJogador = atoi(player + TAM_ID_JOGADOR - 1);

        if(!foundJogador || rrnJogador == -1){
            printf(ERRO_REGISTRO_NAO_ENCONTRADO);
            return;
        }

        Jogador recJogador = recuperar_registro_jogador(rrnJogador);
        char kit_str[TAM_ID_KIT];
        strncpy(kit_str, kits_jogadores + i * 3, TAM_ID_KIT - 1);
        kit_str[TAM_ID_KIT - 1] = '\0';

        if (!verificar_kit_jogador(&recJogador, kit_str)) {
            char chaveKit[TAM_CHAVE_KITS_IDX + 1] = {'\0'};
            btree_search(chaveKit, false, kit_str, kits_idx.rrn_raiz, &kits_idx);
            int rrnKit = atoi(chaveKit + TAM_ID_KIT - 1);
            Kit kitRecuperado = recuperar_registro_kit(rrnKit);

            printf(ERRO_JOGADOR_KIT, recJogador.id_jogador, kitRecuperado.nome);
            return;
        }
    }

    char chavePartida[TAM_CHAVE_PARTIDAS_IDX + 1] = {'\0'};
    sprintf(chavePartida, "%s%04d", idPartidaFormatado, qtd_registros_partidas);
    btree_insert(chavePartida, &partidas_idx);

    strcpy(partida.inicio, inicio);
    strcpy(partida.duracao, duracao);
    strcpy(partida.cenario, cenario);
    strcpy(partida.id_jogadores, id_jogadores);

    escrever_registro_partida(partida, qtd_registros_partidas);

    char chaveData[TAM_CHAVE_DATA_PARTIDA_IDX + 1] = {'\0'};
    sprintf(chaveData, "%s%s", inicio, idPartidaFormatado);
    btree_insert(chaveData, &data_partida_idx);

    qtd_registros_partidas++;

    printf(SUCESSO);

    for(int i = 0; i < QTD_MAX_JOGADORES; i++){
        char idJogador[TAM_ID_JOGADOR];

        strncpy(idJogador, id_jogadores + (i * 11), 11);
        idJogador[TAM_ID_JOGADOR - 1] = '\0';

        Resultado resultado;
        char colocacao[5];
        sprintf(colocacao, "%04d", i + 1);
        resultado.colocacao = atoi(colocacao);

        strcpy(resultado.id_partida, partida.id_partida);
        strncpy(resultado.id_jogador, id_jogadores + (i * 11), 11);
        resultado.id_jogador[11] = '\0';

        strncpy(resultado.id_kit, kits_jogadores + (i * 3), 3);
        resultado.id_kit[3] = '\0';

        strncpy(resultado.sobrevivencia, duracoes_jogadores + (i * 6), 6);
        resultado.sobrevivencia[6] = '\0';

        char eliminacoesStr[5] = {'\0'};
        strncpy(eliminacoesStr, eliminacoes_jogadores + (i * 4), 4);
        resultado.eliminacoes = atoi(eliminacoesStr);

        char chaveResultado[TAM_ID_JOGADOR + TAM_ID_PARTIDA];
        sprintf(chaveResultado, "%s%s", idJogador, partida.id_partida);

        char chaveResultadoExistente[TAM_CHAVE_RESULTADOS_IDX + 1] = {'\0'};
        if (btree_search(chaveResultadoExistente, false, chaveResultado, resultados_idx.rrn_raiz, &resultados_idx)) {
            continue;
        }

        escrever_registro_resultado(resultado, qtd_registros_resultados);

        // Corrigido: aumentando o tamanho do buffer para evitar overflow
        char chaveResultadoInserir[TAM_CHAVE_RESULTADOS_IDX + 5]; // Aumenta o tamanho para garantir que caiba a chave
        sprintf(chaveResultadoInserir, "%s%04d", chaveResultado, qtd_registros_resultados);
        btree_insert(chaveResultadoInserir, &resultados_idx);

        qtd_registros_resultados++;
    }   
}


void recompensar_vencedor_menu (char *data_inicio, char *data_fim, double premio) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "recompensar_vencedor_menu()");
}


/* Busca */
void buscar_jogador_id_menu(char *id_jogador) {
	char str[TAM_CHAVE_JOGADORES_IDX];
	memset (str, 0, TAM_CHAVE_JOGADORES_IDX);
	
	printf (REGS_PERCORRIDOS);
    bool found = btree_search(str, true, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);
	printf ("\n");
	
    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else {
		char num[5];
		memset (num, 0, 5);
		memcpy (num, str+TAM_ID_JOGADOR-1, 4);
		
        exibir_jogador(strtol (num, NULL, 10));
	}
}


void buscar_kit_id_menu(char *id_kit) {
	char str[TAM_CHAVE_KITS_IDX];
	memset (str, 0, TAM_CHAVE_KITS_IDX);
	
	printf(REGS_PERCORRIDOS);
    bool found = btree_search(str, true, id_kit, kits_idx.rrn_raiz, &kits_idx);
	printf("\n");

    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else {
		char num[5];
		memset (num, 0, 5);
		memcpy (num, str+TAM_ID_KIT-1, 4);
		
        exibir_kit(strtol (num, NULL, 10));
	}
}


void buscar_partida_id_menu(char *id_partida) {
	char str[TAM_CHAVE_PARTIDAS_IDX];
	memset (str, 0, TAM_CHAVE_PARTIDAS_IDX);
	
	printf(REGS_PERCORRIDOS);
    bool found = btree_search(str, true, id_partida, partidas_idx.rrn_raiz, &partidas_idx);
	printf("\n");
	
    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else {
		char num[5];
		memset (num, 0, 5);
		memcpy (num, str+TAM_ID_PARTIDA-1, 4);
		
        exibir_partida(strtol (num, NULL, 10));
	}
}

/* Listagem */
void listar_jogadores_id_menu() {
	if (qtd_registros_jogadores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	else
		btree_print_in_order(NULL, NULL, exibir_btree_jogadores, jogadores_idx.rrn_raiz, &jogadores_idx);
}


void listar_jogadores_kits_menu(char *kit){	
}


void listar_kits_compra_menu(char *id_jogador) {
	char jogador_str[TAM_ID_JOGADOR + 1];
	char saldo[TAM_FLOAT_NUMBER + 1];
	char aux[TAM_FLOAT_NUMBER + 1];

	btree_search(jogador_str, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);
	int rrnJogador = atoi(jogador_str + TAM_ID_JOGADOR - 1);
	Jogador player = recuperar_registro_jogador(rrnJogador);
	sprintf(saldo, "%013.2lf", player.saldo);
	sprintf(aux, "0000000000.00");

	if (!btree_print_in_order(aux, saldo, exibir_btree_preco_kit, preco_kit_idx.rrn_raiz, &preco_kit_idx))
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
}


void listar_partidas_periodo_menu(char *data_inicio, char *data_fim) {
	if(!btree_print_in_order(data_inicio, data_fim, exibir_btree_data_partida, data_partida_idx.rrn_raiz, &data_partida_idx)){
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	}	
}


void imprimir_arquivo_jogadores_menu() {
	if (qtd_registros_jogadores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_JOGADORES);
}


void imprimir_arquivo_kits_menu() {
	if (qtd_registros_kits == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_KITS);
}


void imprimir_arquivo_partidas_menu() {
	if (qtd_registros_partidas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PARTIDAS);
}


void imprimir_arquivo_resultados_menu() {
	if (qtd_registros_resultados == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_RESULTADOS);
}


/* Imprimir índices primários */
void imprimir_jogadores_idx_menu() {
	if (jogadores_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_JOGADORES_IDX);
}


void imprimir_kits_idx_menu() {
	if (kits_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_KITS_IDX);
}


void imprimir_partidas_idx_menu() {
	if (partidas_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PARTIDAS_IDX);
}


void imprimir_resultados_idx_menu() {
	if (resultados_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_RESULTADOS_IDX);
}


/* Imprimir índices secundários */
void imprimir_preco_kit_idx_menu() {
	if (preco_kit_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PRECO_KIT_IDX);
}


void imprimir_data_partida_idx_menu() {
	if (data_partida_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_DATA_PARTIDA_IDX);
}


void imprimir_jogador_kits_secundario_idx_menu() {
	if (jogador_kits_idx.qtd_registros_secundario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", jogador_kits_idx.arquivo_secundario);
}


void imprimir_jogador_kits_primario_idx_menu() {
	if (jogador_kits_idx.qtd_registros_primario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", jogador_kits_idx.arquivo_primario);
}


/* Liberar espaço */
void liberar_espaco_menu() {
	int indice_atual = 0;

for (int i = 0; i < qtd_registros_jogadores; i++) {
    // verifica se o registro não está excluído
    if (strncmp(ARQUIVO_JOGADORES + (i * TAM_REGISTRO_JOGADOR), "*|", 2) != 0) {
        // copia o registro não excluído
        strncpy(ARQUIVO_JOGADORES + (indice_atual * TAM_REGISTRO_JOGADOR), ARQUIVO_JOGADORES + (i * TAM_REGISTRO_JOGADOR), TAM_REGISTRO_JOGADOR);

        // recupera o id do jogador
        char jogador_id[TAM_ID_JOGADOR];
        jogador_id[0] = '\0';
        strncpy(jogador_id, ARQUIVO_JOGADORES + (indice_atual * TAM_REGISTRO_JOGADOR), TAM_ID_JOGADOR);

        // atualiza o índice
        indice_atual++;
    }
}
// marca o fim dos registros válidos
(ARQUIVO_JOGADORES + (indice_atual * TAM_REGISTRO_JOGADOR))[0] = '\0';

qtd_registros_jogadores = indice_atual;
// reseta o índice e a quantidade de nós
jogadores_idx.rrn_raiz = -1;
jogadores_idx.qtd_nos = 0;
// limpa o arquivo de índices
memset(jogadores_idx.arquivo, '\0', TAM_ARQUIVO_JOGADORES_IDX);

// prepara a chave para inserção
char chave_jogador[TAM_CHAVE_JOGADORES_IDX + 1];
chave_jogador[0] = '\0';

// insere as chaves no índice
for (unsigned i = 0; i < qtd_registros_jogadores; ++i) {
    Jogador jogador_atual = recuperar_registro_jogador(i);

    // formata a chave do jogador
    snprintf(chave_jogador, TAM_CHAVE_JOGADORES_IDX + 1, "%s%04d", jogador_atual.id_jogador, i);

    // insere no índice
    btree_insert(chave_jogador, &jogadores_idx);
}

printf(SUCESSO);
    
}


/* Funções de busca binária */

/**
 * Função Genérica de busca binária, que aceita parâmetros genéricos (assinatura baseada na função bsearch da biblioteca C).
 *
 * @param key Chave de busca genérica.
 * @param base0 Base onde ocorrerá a busca, por exemplo, um ponteiro para um vetor.
 * @param nmemb Número de elementos na base.
 * @param size Tamanho do tipo do elemento na base, dica: utilize a função sizeof().
 * @param compar Ponteiro para a função que será utilizada nas comparações.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso. Se true, imprime as posições avaliadas durante a busca, até que todas sejam visitadas ou o elemento seja encontrado (pela 1a vez).
 *                       Caso o número de elementos seja par (p.ex, 10 elementos), então há 2 (duas) possibilidades para a posição da mediana dos elementos (p.ex., 5a ou 6a posição se o total fosse 10).
 *                       Neste caso, SEMPRE escolha a posição mais à direita (p.ex., a posição 6 caso o total for 10).
 * @param posicao_caso_repetido Caso o elemento seja encontrado, se houver mais do que 1 ocorrência, indica qual deve ser retornada. As opções são:
 *                     -1 = primeira : retorna a PRIMEIRA ocorrência (posição mais à esquerda).
 *                      0 = meio : retorna a ocorrência do MEIO (posição central). [modo padrão]
 *                     +1 = ultima : retorna a ÚLTIMA ocorrência (posição mais à direita).
 * @param retorno_se_nao_encontrado Caso o elemento NÃO seja encontrado, indica qual valor deve ser retornado. As opções são:
 *                     -1 = predecessor : retorna o elemento PREDECESSOR (o que apareceria imediatamente antes do elemento procurado, caso fosse encontrado).
 *                      0 = nulo : retorna NULL. [modo padrão]
 *                     +1 = sucessor : retorna o elemento SUCESSOR (o que apareceria imediatamente depois do elemento procurado, caso fosse encontrado).
 * @return Retorna o elemento encontrado ou NULL se não encontrou.
 */
int busca_binaria_com_reps(const void *key, const void *base0, size_t nmemb, size_t size, 
                           int (*compar)(const void *, const void *), bool exibir_caminho, 
                           int posicao_caso_repetido, int retorno_se_nao_encontrado) 
{
    int inicio = 0, meio, fim = nmemb - 1;
    bool auxprintf = true;

    while (inicio <= fim) 
    {
        // Ajuste para garantir que o meio seja escolhido de forma eficiente
        meio = (inicio + fim) / 2;
        if ((fim - inicio + 1) % 2 == 0) {
            meio += 1; // Se o intervalo for par, escolhe o meio para a direita
        }

        // Exibindo a trajetória da busca, se necessário
        if (exibir_caminho) 
        {
            if (auxprintf) {
                printf(" (");
                auxprintf = false;
            } else {
                printf(" ");
            }
            printf("%d", meio);
        }        

        // Ajuste da busca conforme a comparação
        if (compar(key, base0 + (meio * size)) > 0) 
        {
            inicio = meio + 1;
        } 
        else if (compar(key, base0 + (meio * size)) < 0) 
        {
            fim = meio - 1;
        } 
        else 
        {
            // Caso a chave seja encontrada, decidimos qual posição retornar
            if (exibir_caminho) 
            {
                printf(")");
            }

            if (posicao_caso_repetido == -1) 
            {
                // Encontrando a primeira ocorrência
                for (int i = meio - 1; i >= 0 && compar(key, base0 + (i * size)) == 0; i--) 
                {
                    meio = i;  // Avançamos para a primeira ocorrência
                }
                return meio;
            } 
            else if (posicao_caso_repetido == 1) 
            {
                // Encontrando a última ocorrência
                for (int i = meio + 1; i < nmemb && compar(key, base0 + (i * size)) == 0; i++) 
                {
                    meio = i;  // Avançamos para a última ocorrência
                }
                return meio;
            } 
            else 
            {
                return meio;  // Retorna o índice da ocorrência atual
            }
        }
    }

    // Caso não tenha encontrado a chave
    if (exibir_caminho) 
    {
        printf(")");
    }

    // Caso a chave não tenha sido encontrada, dependendo da lógica definida
    if (retorno_se_nao_encontrado == -1) 
    {
        // Retorna o índice do maior elemento menor que a chave (predecessor)
        if (fim >= 0) {
            return fim;
        } else {
            return -1;
        }
    } 
    else if (retorno_se_nao_encontrado == 1) 
    {
        // Retorna o índice do menor elemento maior que a chave (sucessor)
        return inicio;
    } 
    else 
    {
        return -1;  // Não encontrado
    }
}


int busca_binaria(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int retorno_se_nao_encontrado) {
	return busca_binaria_com_reps(key, base0, nmemb, size, compar, exibir_caminho, 0, retorno_se_nao_encontrado);
}

/* Funções de manipulação de Lista Invertida */

/* Funções de manipulação de Lista Invertida */
/**
 * Responsável por inserir duas chaves (chave_secundaria e chave_primaria) em uma Lista Invertida (t).<br />
 * Atualiza os parâmetros dos índices primário e secundário conforme necessário.<br />
 * As chaves a serem inseridas devem estar no formato correto e com tamanho t->tam_chave_primario e t->tam_chave_secundario.<br />
 * O funcionamento deve ser genérico para qualquer Lista Invertida, adaptando-se para os diferentes parâmetros presentes em seus structs.<br />
 *
 * @param chave_secundaria Chave a ser buscada (caso exista) ou inserida (caso não exista) no registro secundário da Lista Invertida.
 * @param chave_primaria Chave a ser inserida no registro primário da Lista Invertida.
 * @param t Ponteiro para a Lista Invertida na qual serão inseridas as chaves.
 */
void inverted_list_insert(char *chave_secundaria, char *chave_primaria, inverted_list *t) {
    
}




/**
 * Responsável por buscar uma chave no índice secundário de uma Lista invertida (T). O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e conterá o índice inicial das chaves no registro primário.<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. A chave encontrada deverá ser retornada e o caminho não deve ser informado.<br />
 * ...<br />
 * int result;<br />
 * bool found = inverted_list_secondary_search(&result, false, nome, &jogador_kits_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse na chave encontrada, apenas se ela existe, e o caminho não deve ser informado.<br />
 * ...<br />
 * bool found = inverted_list_secondary_search(NULL, false, nome, &jogador_kits_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 3. Há interesse no caminho feito para encontrar a chave.<br />
 * ...<br />
 * int result;<br />
 * bool found = inverted_list_secondary_search(&result, true, nome, &jogador_kits_idx);<br />
 * </code>
 *
 * @param result Ponteiro para ser escrito o índice inicial (primeira ocorrência) das chaves do registro primário. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave_secundaria Chave a ser buscada.
 * @param t Ponteiro para o índice do tipo Lista invertida no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool inverted_list_secondary_search(int *result, bool exibir_caminho, char *chave_secundaria, inverted_list *t)
{	
	int idx = 0;
	char rrn[TAM_RRN_REGISTRO + 1];
	inverted_list_binary_search(&idx, exibir_caminho, chave_secundaria, t);
	if (!inverted_list_binary_search(&idx, exibir_caminho, chave_secundaria, t))
		return false;
	else
	{
		strncpy(rrn, t->arquivo_secundario + (idx * (t->tam_chave_secundaria + TAM_RRN_REGISTRO)) + t->tam_chave_secundaria, TAM_RRN_REGISTRO);
		rrn[TAM_RRN_REGISTRO] = '\0';
		if (result)
			*result = atoi(rrn);
		return true;
	}
}



/**
 * Responsável por percorrer o índice primário de uma Lista invertida (T). O valor de retorno indica a quantidade de chaves encontradas.
 * O ponteiro para o vetor de strings result pode ser fornecido opcionalmente, e será populado com a lista de todas as chaves encontradas.
 * O ponteiro para o inteiro indice_final também pode ser fornecido opcionalmente, e deve conter o índice do último campo da lista encadeada
 * da chave primaria fornecida (isso é útil na inserção de um novo registro).<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. As chaves encontradas deverão ser retornadas e tanto o caminho quanto o indice_final não devem ser informados.<br />
 * ...<br />
 * char chaves[TAM_CHAVE_JOGADOR_KIT_PRIMARIO_IDX][MAX_REGISTROS];<br />
 * int qtd = inverted_list_primary_search(chaves, false, indice, NULL, &jogador_kits_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse nas chaves encontradas, apenas no indice_final, e o caminho não deve ser informado.<br />
 * ...<br />
 * int indice_final;
 * int qtd = inverted_list_primary_search(NULL, false, indice, &indice_final, &jogador_kits_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 3. Há interesse nas chaves encontradas e no caminho feito.<br />
 * ...<br />
 * char chaves[TAM_CHAVE_JOGADOR_KIT_PRIMARIO_IDX][MAX_REGISTROS];<br />
 * int qtd = inverted_list_primary_search(chaves, true, indice, NULL, &jogador_kits_idx);<br />
 * ...<br />
 * <br />
 * </code>
 *
 * @param result Ponteiro para serem escritas as chaves encontradas. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param indice Índice do primeiro registro da lista encadeada a ser procurado.
 * @param indice_final Ponteiro para ser escrito o índice do último registro encontrado (cujo campo indice é -1). É ignorado caso NULL.
 * @param t Ponteiro para o índice do tipo Lista invertida no qual será buscada a chave.
 * @return Indica a quantidade de chaves encontradas.
 */
int inverted_list_primary_search(char result[][TAM_CHAVE_JOGADOR_KIT_PRIMARIO_IDX], bool exibir_caminho, int indice, int *indice_final, inverted_list *t){
	
	int idx = indice;
    int chave = 0;

	char rrn[TAM_RRN_REGISTRO + 1];

	if (exibir_caminho)
		printf(REGS_PERCORRIDOS);
	while (idx != -1)
	{
		if (exibir_caminho)
			printf(" %d", idx);
		if (result)
			strncpy(result[chave], t->arquivo_primario + (idx * (t->tam_chave_primaria + TAM_RRN_REGISTRO)), t->tam_chave_primaria);
		if (idx != -1)
			if (indice_final)
				*indice_final = idx;
		strncpy(rrn, t->arquivo_primario + (idx * (t->tam_chave_primaria + TAM_RRN_REGISTRO)) + t->tam_chave_primaria, TAM_RRN_REGISTRO);
		idx = atoi(rrn);
		chave++;
	}

	if (exibir_caminho)
		printf("\n");
	return chave;
}



/**
 * Responsável por buscar uma chave (k) dentre os registros secundários de uma Lista Invertida de forma eficiente.<br />
 * O valor de retorno deve indicar se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e conterá o índice no registro secundário da chave encontrada.<br />
 *
 * @param result Ponteiro para ser escrito o índice nos registros secundários da chave encontrada. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Lista Invertida.
 * @param t Ponteiro para o índice da Lista Invertida no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool inverted_list_binary_search(int* result, bool exibir_caminho, char *chave, inverted_list *t) {
	int inicio = 0;
    int fim = t->qtd_registros_secundario - 1;
    int meio;
    char *p;
    int n = t->qtd_registros_secundario;

    while(inicio <= fim){
        meio = inicio + n / 2;
        p = (t->arquivo_secundario + (TAM_CHAVE_JOGADOR_KIT_SECUNDARIO_IDX + 4) * meio);        

        if(exibir_caminho == true)
            printf(" %d", meio);

        if(t->compar(chave, p) == 0){
            *result = meio;
            return true;
        }
        else{
            if(t->compar(chave, p) > 0)
                inicio = meio + 1;
            else
                fim = meio - 1;
        }
        n = (fim - inicio) + 1;
    }

    return false;
}



/* Funções de manipulação de Árvores-B */

/**
 * Responsável por inserir uma chave (k) em uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * A chave a ser inserida deve estar no formato correto e com tamanho t->tam_chave.<br />
 * O funcionamento deve ser genérico para qualquer Árvore-B, considerando que os únicos parâmetros que se alteram entre
 * as árvores é o t->tam_chave.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * ...<br />
 * char jogador_str[TAM_CHAVE_JOGADORES_IDX + 1];<br />
 * sprintf(jogador_str, "%s%04d", id_jogador, rrn_jogador);<br />
 * btree_insert(jogador_str, &jogadores_idx);<br />
 * ...<br />
 * </code>
 *
 * @param chave Chave a ser inserida na Árvore-B.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 */
void btree_insert(char *chave, btree *t) {
    
	if(t->rrn_raiz == -1){
		btree_node no = btree_node_malloc(t);

		no.this_rrn = t->qtd_nos;
		no.qtd_chaves = 1;

		strcpy(no.chaves[0], chave);
		no.folha = true;

		t->rrn_raiz = t->qtd_nos;
		t->qtd_nos = 1;
		
		btree_write(no, t);
		
	} 
	else {
		promovido_aux p = btree_insert_aux(chave, t->rrn_raiz, t);
		if(p.chave_promovida[0] != '\0'){

			btree_node no = btree_node_malloc(t);
			no.this_rrn = t->qtd_nos;
			no.qtd_chaves = 1;

			strncpy(no.chaves[0], p.chave_promovida, t->tam_chave);
			no.folha = false;

			no.filhos[0] = t->rrn_raiz;
			no.filhos[1] = p.filho_direito;

			t->qtd_nos++;
			t->rrn_raiz = no.this_rrn;

			btree_write(no, t);
			btree_node_free(no);
			
		}
	}
}


/**
 * Função auxiliar de inserção de uma chave (k) em uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * Esta é uma função recursiva. Ela recebe o RRN do nó a ser trabalhado sobre.<br />
 *
 * @param chave Chave a ser inserida na Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Retorna uma struct do tipo promovido_aux que contém a chave promovida e o RRN do filho direito.
 */
promovido_aux btree_insert_aux(char *chave, int rrn, btree *t) {
    promovido_aux p;

    // inicializa promovido_aux com a chave fornecida e filho_direito como -1
    strcpy(p.chave_promovida, chave);
    p.filho_direito = -1;

    // se o rrn for válido, processa o nó
    if (rrn >= 0) {
        // lê o nó correspondente ao rrn
        btree_node no = btree_read(rrn, t);

        // busca a posição onde a chave deve ser inserida
        int posicao;
        if (!btree_binary_search(&posicao, false, chave, &no, t)) {
            // chamada recursiva para inserir a chave no filho correspondente
            p = btree_insert_aux(chave, no.filhos[posicao], t);

            // verifica se houve promoção de uma chave
            if (p.chave_promovida[0] != '\0') {
                // verifica se há espaço no nó atual para a chave promovida
                if (no.qtd_chaves < btree_order - 1) {
                    // desloca as chaves e filhos para abrir espaço
                    for (int i = no.qtd_chaves - 1; i >= posicao; i--) {
                        strncpy(no.chaves[i + 1], no.chaves[i], t->tam_chave);
                        no.filhos[i + 2] = no.filhos[i + 1];
                    }

                    // insere a chave promovida e atualiza o ponteiro para o filho direito
                    strncpy(no.chaves[posicao], p.chave_promovida, t->tam_chave);
                    no.filhos[posicao + 1] = p.filho_direito;

                    // incrementa o contador de chaves no nó
                    no.qtd_chaves++;

                    // escreve o nó atualizado no disco
                    btree_write(no, t);

                    // indica que não houve overflow
                    p.chave_promovida[0] = '\0';
                } else {
                    // caso contrário, divide o nó porque está cheio
                    p = btree_divide(p, &no, posicao, t);
                }
            }
        }
    }

    return p;
}



/**
 * Função auxiliar para dividir um nó de uma Árvore-B (T). Atualiza os parâmetros conforme necessário.<br />
 *
 * @param promo Uma struct do tipo promovido_aux que contém a chave a ser inserida e o RRN do seu filho direito.
 * @param node Ponteiro para nó que deve ser dividido. 
 * @param i O índice da posição onde a chave a ser inserida deve estar.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Retorna uma struct do tipo promovido_aux que contém a chave promovida e o RRN do filho direito.
 */
promovido_aux btree_divide(promovido_aux promo, btree_node *node, int i, btree *t) {
    promovido_aux p;
    
    strncpy(p.chave_promovida, promo.chave_promovida, t->tam_chave);
    p.filho_direito = promo.filho_direito;
    
    btree_node no = btree_node_malloc(t);

    // inicializa o novo no com valores padrao
    no.this_rrn = t->qtd_nos;
    no.qtd_chaves = 0;
    no.folha = node->folha;

    // incrementa o contador de nos na arvore
    t->qtd_nos++;

    // calcula o ponto de divisao (minimo)
    int min = (btree_order - 1) / 2;

    // caso a chave inserida esteja exatamente no meio do no
    if (i == btree_order / 2) {
        // copia a ultima chave do no atual para o novo no
        strcpy(no.chaves[0], node->chaves[node->qtd_chaves - 1]);

        // ajusta os filhos do novo no e do no atual
        no.filhos[0] = p.filho_direito;
        no.filhos[1] = node->filhos[node->qtd_chaves];

        // reduz o numero de filhos no no atual
        node->filhos[node->qtd_chaves]--;
        p.filho_direito = -1;

        // incrementa as chaves no novo no e reduz no no atual
        no.qtd_chaves++;
        node->qtd_chaves--;

        // desloca as chaves e filhos no no atual para inserir a nova chave
        for (int j = node->qtd_chaves - 1; j >= i; j--) {
            strncpy(node->chaves[j + 1], node->chaves[j], t->tam_chave);
            node->filhos[j + 2] = node->filhos[j + 1];
        }

        // insere a nova chave e ajusta os filhos no no atual
        strncpy(node->chaves[i], p.chave_promovida, t->tam_chave);
        node->filhos[i + 1] = p.filho_direito;
        p.filho_direito = no.this_rrn;

        // grava os nos atualizados no arquivo
        btree_write(*node, t);
        btree_write(no, t);
        btree_node_free(no);

        return p;
    }
    // caso a chave inserida esteja depois do meio
    else if (i > btree_order / 2) {
        // insere a nova chave no novo no
        strncpy(no.chaves[0], p.chave_promovida, t->tam_chave);

        // ajusta os filhos do novo no
        no.filhos[0] = node->filhos[node->qtd_chaves];
        no.filhos[1] = p.filho_direito;

        // incrementa o numero de chaves no novo no
        no.qtd_chaves++;

        // ajusta os filhos no no atual
        node->filhos[node->qtd_chaves] = -1;
    }
    // caso a chave inserida esteja antes do meio
    else {
        // copia a ultima chave do no atual para o novo no
        strncpy(no.chaves[0], node->chaves[node->qtd_chaves - 1], t->tam_chave);
        no.filhos[0] = node->filhos[node->qtd_chaves - 1];
        no.filhos[1] = node->filhos[node->qtd_chaves];

        // ajusta os filhos no no atual
        node->filhos[node->qtd_chaves - 1] = -1;
        node->filhos[node->qtd_chaves] = -1;

        // incrementa as chaves no novo no e reduz no no atual
        no.qtd_chaves++;
        node->qtd_chaves--;

        // desloca as chaves e filhos no no atual para inserir a nova chave
        for (int j = node->qtd_chaves - 1; j >= i; j--) {
            strncpy(node->chaves[j + 1], node->chaves[j], t->tam_chave);
            node->filhos[j + 2] = node->filhos[j + 1];
        }

        // insere a nova chave e ajusta os filhos no no atual
        strncpy(node->chaves[i], p.chave_promovida, t->tam_chave);
        node->filhos[i + 1] = p.filho_direito;

        // define a chave promovida e ajusta o filho direito
        strncpy(p.chave_promovida, node->chaves[node->qtd_chaves], t->tam_chave);
        p.filho_direito = no.this_rrn;

        // grava os nos atualizados no arquivo
        btree_write(*node, t);
        btree_write(no, t);

        return p;
    }

    // redistribui chaves entre o no atual e o novo no
    for (int k = no.qtd_chaves, j = min; k < min; k++, j++) {
        strncpy(no.chaves[k], node->chaves[j], t->tam_chave);
        no.filhos[k + 1] = node->filhos[j + 1];
        node->filhos[j + 1] = -1;
        node->qtd_chaves--;
        no.qtd_chaves++;

	}

        strncpy(p.chave_promovida, node->chaves[node->qtd_chaves - 1], t->tam_chave);

        node->qtd_chaves--;
        p.filho_direito = no.this_rrn;

        btree_write(*node, t);
        btree_write(no, t);

        return p;
    
}



/**
 * Responsável por remover uma chave (k) de uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * A chave a ser removida deve estar no formato correto e com tamanho t->tam_chave.<br />
 * O funcionamento deve ser genérico para qualquer Árvore-B, considerando que os únicos parâmetros que se alteram entre
 * as árvores é o t->tam_chave.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * ...<br />
 * char jogador_str[TAM_CHAVE_JOGADORES_IDX + 1];<br />
 * sprintf(jogador_str, "%s%04d", id_jogador, rrn_jogador);<br />
 * btree_delete(jogador_str, &jogadores_idx);<br />
 * ...<br />
 * </code>
 *
 * @param chave Chave a ser removida da Árvore-B.
 * @param t Ponteiro para o índice do tipo Árvore-B do qual será removida a chave.
 */
void btree_delete(char *chave, btree *t) {
	if (btree_delete_aux(chave, t->rrn_raiz, t))
        t->rrn_raiz = 0;
}



/**
 * Função auxiliar de remoção de uma chave (k) de uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * Esta é uma função recursiva. Ela recebe o RRN do nó a ser trabalhado sobre.<br />
 *
 * @param chave Chave a ser removida da Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B do qual será removida a chave.
 * @return Indica se a remoção deixou o nó que foi processado com menos chaves que o mínimo necessário.
 */
bool btree_delete_aux(char *key, int node_rrn, btree *tree) {
    int min = (btree_order - 1) / 2;
    int idx = 0;

    if (node_rrn >= 0) { 
        btree_node node = btree_read(node_rrn, tree);

        if (btree_binary_search(&idx, false, key, &node, tree)) {
            
            if (!node.folha) {
                
                btree_node child_node = btree_read(node.filhos[idx], tree);
                strncpy(node.chaves[idx], child_node.chaves[child_node.qtd_chaves - 1], tree->tam_chave);
                btree_write(node, tree);

                
                if (btree_delete_aux(node.chaves[idx], node.filhos[idx], tree)) {
                    
                    return btree_borrow_or_merge(&node, idx, tree);
                }
            } else { 
               
                if (idx < node.qtd_chaves - 1) {
                    for (int i = idx; i < node.qtd_chaves - 1; i++) {
                        strcpy(node.chaves[i], node.chaves[i + 1]);
                    }
                }
                node.qtd_chaves--;
                btree_write(node, tree);

                
                return node.qtd_chaves < min;
            }
        } else {
            
            if (btree_delete_aux(key, node.filhos[idx], tree)) {
                return btree_borrow_or_merge(&node, idx, tree);
            }
        }
    }
    return false;
}



/**
 * Função auxiliar para redistribuir ou concatenar nós irmãos adjacentes à esquerda e à direita de um nó pai em uma Árvore-B (T). 
 * Atualiza os parâmetros conforme necessário.<br />
 *
 * @param node Ponteiro para nó pai dos nós irmãos adjacentes que deve ser redistribuidos ou concatenados. 
 * @param i O índice da posição no nó pai onde se encontra a chave separadora dos nós irmãos adjacentes.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual serão redistribuídos ou concatenados os nós irmãos adjacentes.
 * @return Indica se a redistribuição ou concatenação deixou o nó pai com menos chaves que o mínimo necessário.
 */
bool btree_borrow_or_merge(btree_node *node, int i, btree *t) {
    int min = (btree_order - 1) / 2;
    btree_node currentNode = btree_read(node->filhos[i], t);

    // Empresta do irmão à direita
    if (i + 1 < btree_order) {
        if (node->filhos[i + 1] != -1) {
            btree_node node_direita = btree_read(node->filhos[i + 1], t);
            if (node_direita.qtd_chaves > min) {
                strcpy(currentNode.chaves[currentNode.qtd_chaves], node->chaves[i]);
                currentNode.qtd_chaves++;
                btree_write(currentNode, t);

                strcpy(node->chaves[i], node_direita.chaves[0]);
                btree_write(*node, t);

                for (int j = 0; j < node_direita.qtd_chaves - 1; j++) {
                    strcpy(node_direita.chaves[j], node_direita.chaves[j + 1]);
                }
                node_direita.qtd_chaves--;
                btree_write(node_direita, t);

                return false;
            }
        }
    }

    // Empresta do irmão à esquerda
    if (i - 1 >= 0) {
        if (node->filhos[i - 1] != -1) {
            btree_node node_esquerda = btree_read(node->filhos[i - 1], t);
            if (node_esquerda.qtd_chaves > min) {
                for (int j = currentNode.qtd_chaves; j > 0; j--) {
                    strcpy(currentNode.chaves[j], currentNode.chaves[j - 1]);
                }
                strcpy(currentNode.chaves[0], node->chaves[i - 1]);
                currentNode.qtd_chaves++;
                btree_write(currentNode, t);

                strcpy(node->chaves[i - 1], node_esquerda.chaves[node_esquerda.qtd_chaves - 1]);
                btree_write(*node, t);

                node_esquerda.qtd_chaves--;
                btree_write(node_esquerda, t);

                return false;
            }
        }
    }

    // Fusão com o irmão à direita
    if (i + 1 < btree_order) {
        if (node->filhos[i + 1] != -1) {
            btree_node node_direita = btree_read(node->filhos[i + 1], t);
            if (node_direita.qtd_chaves <= min) {
                strcpy(currentNode.chaves[currentNode.qtd_chaves], node->chaves[i]);
                currentNode.qtd_chaves++;

                for (int j = 0; j < node_direita.qtd_chaves; j++) {
                    strcpy(currentNode.chaves[currentNode.qtd_chaves], node_direita.chaves[j]);
                    currentNode.filhos[currentNode.qtd_chaves] = node_direita.filhos[j];
                    currentNode.qtd_chaves++;
                }
                currentNode.filhos[currentNode.qtd_chaves] = node_direita.filhos[node_direita.qtd_chaves];
                btree_write(currentNode, t);

                for (int j = i; j < node->qtd_chaves - 1; j++) {
                    strcpy(node->chaves[j], node->chaves[j + 1]);
                }
                node->qtd_chaves--;
                if (node->qtd_chaves > 0) {
                    for (int j = i; j < node->qtd_chaves; j++) {
                        node->filhos[j] = node->filhos[j + 1];
                    }
                    node->filhos[node->qtd_chaves + 1] = -1;
                } else {
                    for (int j = 0; j <= btree_order; j++) {
                        node->filhos[j] = -1;
                    }
                }
                btree_write(*node, t);

                node_direita.qtd_chaves = 0;
                btree_write(node_direita, t);

                return node->qtd_chaves < min;
            }
        }
    }

    // Fusão com o irmão à esquerda
    if (i - 1 >= 0) {
        if (node->filhos[i - 1] != -1) {
            btree_node node_esquerda = btree_read(node->filhos[i - 1], t);
            if (node_esquerda.qtd_chaves <= min) {
                strcpy(node_esquerda.chaves[node_esquerda.qtd_chaves], node->chaves[i - 1]);
                node_esquerda.qtd_chaves++;

                for (int j = 0; j < currentNode.qtd_chaves; j++) {
                    strcpy(node_esquerda.chaves[node_esquerda.qtd_chaves], currentNode.chaves[j]);
                    node_esquerda.filhos[node_esquerda.qtd_chaves] = currentNode.filhos[j];
                    node_esquerda.qtd_chaves++;
                }
                node_esquerda.filhos[node_esquerda.qtd_chaves] = currentNode.filhos[currentNode.qtd_chaves];
                btree_write(node_esquerda, t);

                for (int j = i; j < node->qtd_chaves - 1; j++) {
                    strcpy(node->chaves[j], node->chaves[j + 1]);
                }
                node->qtd_chaves--;
                if (node->qtd_chaves > 0) {
                    for (int j = i; j < node->qtd_chaves; j++) {
                        node->filhos[j] = node->filhos[j + 1];
                    }
                } else {
                    for (int j = 0; j <= btree_order; j++) {
                        node->filhos[j] = -1;
                    }
                }
                btree_write(*node, t);

                currentNode.qtd_chaves = 0;
                btree_write(currentNode, t);

                return node->qtd_chaves < min;
            }
        }
    }

    return false;
}

/**
 * Responsável por buscar uma chave (k) em uma Árvore-B (T). O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para a string result pode ser fornecido opcionalmente, e conterá o resultado encontrado.<br />
 * Esta é uma função recursiva. O parâmetro rrn recebe a raíz da Árvore-B na primeira chamada, e nas chamadas
 * subsequentes é o RRN do filho de acordo com o algoritmo fornecido.<br />
 * Comportamento de acordo com as especificações do PDF sobre Árvores-B e suas operações.<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. A chave encontrada deverá ser retornada e o caminho não deve ser informado.<br />
 * ...<br />
 * char result[TAM_CHAVE_JOGADORES_IDX + 1];<br />
 * bool found = btree_search(result, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse na chave encontrada, apenas se ela existe, e o caminho não deve ser informado.<br />
 * ...<br />
 * bool found = btree_search(NULL, false, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);<br />
 * ...<br />
 * <br />
* // Exemplo 3. Há interesse no caminho feito para encontrar a chave.<br />
  * ...<br />
 * char result[TAM_CHAVE_JOGADORES_IDX + 1];<br />
 * bool found = btree_search(result, true, id_jogador, jogadores_idx.rrn_raiz, &jogadores_idx);<br />
 * printf("\n");<br />
 * </code>
 *
 * @param result Ponteiro para ser escrita a chave encontrada. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado. É o RRN da raíz da Árvore-B caso seja a primeira chamada.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool btree_search(char *result, bool exibir_caminho, char *chave, int rrn, btree *t) { //estava recursiva porem dando mt erro de memoria, pedi ao chat para deixar iterativa
    while (rrn != -1) {
        btree_node node = btree_read(rrn, t);

        // Limpar filhos além da quantidade de chaves
        for (int i = node.qtd_chaves + 1; i < btree_order; i++) {
            node.filhos[i] = -1;
        }

        // Exibir caminho, se solicitado
        if (exibir_caminho) {
            printf(" %d", rrn);
        }

        int i;
        bool encontrou = btree_binary_search(&i, exibir_caminho, chave, &node, t);

        if (encontrou) {
            if (result) {
                strncpy(result, node.chaves[i], t->tam_chave);
            }
            btree_node_free(node);
            return true;
        } else {
            rrn = node.filhos[i];
            btree_node_free(node);
        }
    }

    return false;
}


/**
 * Responsável por buscar uma chave (k) dentro do nó de uma Árvore-B (T) de forma eficiente. O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e indica o índice da chave encontrada (caso tenha sido encontrada) 
 * ou o índice do filho onde esta chave deve estar (caso não tenha sido encontrada).<br />
 *
 * @param result Ponteiro para ser escrito o índice da chave encontrada ou do filho onde ela deve estar. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Árvore-B.
 * @param node Ponteiro para o nó onde a busca deve ser feita.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool btree_binary_search(int *result, bool exibir_caminho, char* chave, btree_node* node, btree* t) {
	int i = busca_binaria(chave, node->chaves[0], node->qtd_chaves, t->tam_chave, t->compar, exibir_caminho, 1);

	if (result)
		*result = i;

	return (i >= 0 && i < node->qtd_chaves && t->compar(chave, node->chaves[i]) == 0);

}


/**
 * Função para percorrer uma Árvore-B (T) em ordem e armazenar as chaves, em result, que estiverem dentro do intervalo fornecido.<br />
 * Os parâmetros chave_inicio e chave_fim podem ser fornecidos opcionalmente, e contém o intervalo do percurso.
 * Caso chave_inicio e chave_fim sejam NULL, o índice inteiro é percorrido.
 * Esta é uma função recursiva. O parâmetro rrn recebe a raíz da Árvore-B na primeira chamada, e nas chamadas
 * subsequentes é o RRN do filho de acordo com o algoritmo de percursão em ordem.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * // Exemplo 1. Intervalo não especificado.
 * ...<br />
 * bool imprimiu = btree_search_in_order(result, NULL, NULL, 0, jogadores_idx.rrn_raiz, &jogadores_idx);
 * ...<br />
 * <br />
 * // Exemplo 2. Imprime as transações contidas no intervalo especificado.
 * ...<br />
 * bool imprimiu = btree_search_in_order(result, data_inicio, data_fim, 0, data_partida_idx.rrn_raiz, &data_partida_idx);
 * ...<br />
 * </code>
 *
 * @param result Vetor de strings onde serão armazenadas as chaves dos registros válidos.
 * @param chave_inicio Começo do intervalo. É ignorado caso NULL.
 * @param chave_fim Fim do intervalo. É ignorado caso NULL.
 * @param qtd Recebe a quantidade atual de chaves válidas (valor usado para retornar a quantidade com a recursão).
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Indica a quantidade de chaves válidas.
 */
int btree_search_in_order(char **result, char *chave_inicio, char *chave_fim, int qtd, int rrn, btree *t) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "btree_search_in_order()");
	return 0;
}


/**
 * Função para percorrer uma Árvore-B (T) em ordem.<br />
 * Os parâmetros chave_inicio e chave_fim podem ser fornecidos opcionalmente, e contém o intervalo do percurso.
 * Caso chave_inicio e chave_fim sejam NULL, o índice inteiro é percorrido.
 * Esta é uma função recursiva. O parâmetro rrn recebe a raíz da Árvore-B na primeira chamada, e nas chamadas
 * subsequentes é o RRN do filho de acordo com o algoritmo de percursão em ordem.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * // Exemplo 1. Intervalo não especificado.
 * ...<br />
 * bool imprimiu = btree_print_in_order(NULL, NULL, exibir_btree_jogadores, jogadores_idx.rrn_raiz, &jogadores_idx);
 * ...<br />
 * <br />
 * // Exemplo 2. Imprime as transações contidas no intervalo especificado.
 * ...<br />
 * bool imprimiu = btree_print_in_order(data_inicio, data_fim, exibir_btree_data_partida, data_partida_idx.rrn_raiz, &data_partida_idx);
 * ...<br />
 * </code>
 *
 * @param chave_inicio Começo do intervalo. É ignorado caso NULL.
 * @param chave_fim Fim do intervalo. É ignorado caso NULL.
 * @param exibir Função utilizada para imprimir uma chave no índice. É informada a chave para a função.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Indica se alguma chave foi impressa.
 */
bool btree_print_in_order(char *chave_inicio, char *chave_fim, bool (*exibir)(char *chave), int rrn, btree *t) { // a anterior estava errada, pedi para o chat corrigir
    
	if (rrn == -1) {
        return false;
    }

    btree_node node = btree_read(rrn, t);
    bool chave_exibida = false; 
   
    for (int i = 0; i < node.qtd_chaves; i++) {        
        if (!chave_inicio || t->compar(chave_inicio, node.chaves[i]) < 0) {
            if (btree_print_in_order(chave_inicio, chave_fim, exibir, node.filhos[i], t)) {
                chave_exibida = true; 
            }
        }

       
        if ((!chave_inicio || t->compar(node.chaves[i], chave_inicio) >= 0) &&
            (!chave_fim || t->compar(node.chaves[i], chave_fim) <= 0)) {
            exibir(node.chaves[i]);
            chave_exibida = true; 
        }
        
        if (i == node.qtd_chaves - 1 && (!chave_fim || t->compar(chave_fim, node.chaves[i]) > 0)) {
            if (btree_print_in_order(chave_inicio, chave_fim, exibir, node.filhos[i + 1], t)) {
                chave_exibida = true;
            }
        }
    }

    return chave_exibida;
}




/**
 * Função interna para ler um nó em uma Árvore-B (T).<br />
 *
 * @param no No a ser lido da Árvore-B.
 * @param t Árvore-B na qual será feita a leitura do nó.
 */
btree_node btree_read(int rrn, btree *t) {
  btree_node no;    
    no = btree_node_malloc(t);
    no.this_rrn = rrn;
    int offset = btree_register_size(t) * rrn;
    char buffer[t->tam_chave + 1];
    
    strncpy(buffer, "\0", 1);  // Inicializando como vazio
    strncpy(buffer, t->arquivo + offset, t->tam_chave);
    strncpy(buffer + 3, "\0", 1);  // Para garantir que o valor será convertido corretamente
    no.qtd_chaves = atoi(buffer);
    
    offset += 3;

    for (int i = 0; i < btree_order - 1; i++, offset += t->tam_chave) {
        strncpy(buffer, t->arquivo + offset, t->tam_chave);
        strncpy(buffer + t->tam_chave, "\0", 1);

        if (strncmp(buffer, "#", 1) != 0)
            strcpy(no.chaves[i], buffer);
    }

    strncpy(buffer, t->arquivo + offset, 1);
    if (strncmp(buffer, "F", 1) == 0)
        no.folha = false;
    else
        no.folha = true;

    offset++;

    for (int i = 0; i < btree_order; i++, offset += 3) {
        strncpy(buffer, t->arquivo + offset, 3);
        strncpy(buffer + 3, "\0", 1);

        if (strncmp(buffer, "*", 1) != 0)
            no.filhos[i] = atoi(buffer);
        else
            no.filhos[i] = -1;
    }

    return no;
}



/**
 * Função interna para escrever um nó em uma Árvore-B (T).<br />
 *
 * @param no No a ser escrito na Árvore-B.
 * @param t Árvore-B na qual será feita a escrita do nó.
 */
void btree_write(btree_node no, btree *t) {

	int espacoNaoUtilizado = btree_order - no.qtd_chaves - 1;
    char no_principal[btree_register_size(t) + 1];
    char no_temporal[t->tam_chave + 1];

    strncpy(no_principal, "\0", 1);  // Inicializando como vazio
    sprintf(no_temporal, "%03d", no.qtd_chaves);
    strcpy(no_principal, no_temporal);

    for (int i = 0; i < no.qtd_chaves; i++) {
        strncat(no_principal, no.chaves[i], t->tam_chave);
    }

    if (espacoNaoUtilizado != 0)  // Verificando se há espaço sobrando no nó
        strpadright(no_principal, '#', strlen(no_principal) + espacoNaoUtilizado * t->tam_chave);  // Preenchendo com '#'

    if (no.folha == true)  // Verificando se o nó é folha
        strcat(no_principal, "T");
    else
        strcat(no_principal, "F");

    for (int i = 0; i < btree_order; i++) {
        if (no.filhos[i] != -1) {  // Verificando se existe filho
            sprintf(no_temporal, "%03d", no.filhos[i]);
            strncat(no_principal, no_temporal, 3);
        } else {
            strcat(no_principal, "***");
        }
    }

    strncpy(t->arquivo + no.this_rrn * btree_register_size(t), no_principal, btree_register_size(t));
}



/**
 * Função interna para alocar o espaço necessário dos campos chaves (vetor de strings) e filhos (vetor de inteiros) da struct btree_node.<br />
 *
 * @param t Árvore-B base para o qual será alocado um struct btree_node.
 */
btree_node btree_node_malloc(btree *t) {
	btree_node no;

	no.this_rrn = t->qtd_nos;

	no.qtd_chaves = 0;

	char *aux = malloc (sizeof (char) * (btree_order-1) * (t->tam_chave)); //vetor contíguo para permitir uso de algumas funções da stdlib
	memset (aux, 0, (btree_order-1) * (t->tam_chave));
	no.chaves = malloc (sizeof (void*) * (btree_order-1));
	for (int i = 0; i < btree_order-1; i++) {
		no.chaves[i] = (aux + i * (t->tam_chave));
	}

	no.folha = true;
	no.filhos = malloc (sizeof(int) * btree_order);
	memset (no.filhos, -1, sizeof(int) * btree_order);

	return no;
}


/**
 * Função interna para liberar o espaço alocado pelo btree_node_malloc.<br />
 *
 * @param no Nó para o qual o espaço alocado deverá ser librerado;
 */
void btree_node_free (btree_node no) {
	//if (*no.chaves)
		//free (*no.chaves);
	//if (no.chaves)
	//free (no.chaves);
	//if (no.filhos)
		//free (no.filhos);
}


/**
 * Função interna para calcular o tamanho em bytes de uma Árvore-B.<br />
 *
 * @param t Árvore-B base para o qual será calculado o tamanho.
 */
int btree_register_size(btree *t) {
	int chaves_ordenadas = (btree_order-1)*t->tam_chave;
	return 3 + chaves_ordenadas + 1 + (btree_order*3);
}


/* Funções Auxiliares */

char *strpadright(char *str, char pad, unsigned size){
	for (unsigned i = strlen(str); i < size; ++i)
		str[i] = pad;
	str[size] = '\0';
	return str;
}

char *strupr(char *str){
	for (char *p = str; *p; ++p)
		*p = toupper(*p);
	return str;
}

char *strlower(char *str){
	for (char *p = str; *p; ++p)
		*p = tolower(*p);
	return str;
}

bool verificar_kit_jogador(Jogador *jogador, char *idKitJogadorAtual) {
    for (int i = 0; i < QTD_MAX_KITS; i++) {
        if (jogador->kits[i][0] != '\0' && strcmp(idKitJogadorAtual, jogador->kits[i]) == 0) {
            return true;
        }
    }
    return false;
}

