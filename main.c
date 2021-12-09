#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define quant_processos 12

typedef struct {
	int criacao;
	int duracao;
	int prioridade;
	int decremento;
	int termino;
	int turn_around;
	int temp_espera;
	bool status;
} processo;

int verifica_menor_processo(int tempo_atual, processo* processos); // verifica e retorna qual é o processo com menor tempo de duração

int main (void) {
    printf("Projeto de codificacao - ECOS01 - STRN (Shortest Remaining-Time Next) \n\n");
    FILE *entrada; 
    entrada = fopen ("entrada.txt", "r"); // abre o arquivo de entrada como leitura
    
    int i = 0;
    int count = 1;
    processo processos[quant_processos]; // vetor com todos os processos
    
    while (i < quant_processos){ // armazena os dados de entrada nos vetores de criação, duração e prioridade
        int dado_entrada;
        fscanf (entrada, "%d", &dado_entrada); 
        if(count == 1){ 
            processos[i].criacao = dado_entrada; // armazena o tempo de chegada dos processos
            count++;
        }
        else if(count == 2){
            processos[i].duracao = dado_entrada; // armazena o tempo de duração dos processos
            processos[i].decremento = dado_entrada; // armazena o tempo de duração dos processos para decrementar futuramente em outras funções
            count++;
        }
        else if (count == 3){
            processos[i].prioridade = dado_entrada; // armazena a prioridade dos processos
            count++;
            count = 1;
            i++;
        }
        // inicialização dos atributos restantes 
        processos[i].termino = 0; 
        processos[i].turn_around = 0;
        processos[i].temp_espera = 0;
        processos[i].status = false;
    }
    
    fclose (entrada); // fecha o arquivo de entrada
    
    // imprime os dados de criação, duração e prioridade dos processos
    printf("Dados de entrada: \n\n");
    printf("DC DS PE\n");
    for( i = 0; i < quant_processos; i++){ 
    	if(processos[i].criacao < 10){
    		printf("0%d ", processos[i].criacao);
		}
		else {
			printf("%d ", processos[i].criacao);
		}
		if(processos[i].decremento < 10){
    		printf("0%d ", processos[i].decremento);
		}
		else {
			printf("%d ", processos[i].decremento);
		}
		if(processos[i].prioridade < 10){
    		printf("0%d", processos[i].prioridade);
		}
		else {
			printf("%d", processos[i].prioridade);
		}
        printf("\n");
    }
    
    // imprime o diagrama de tempo da execução
    printf("\nDiagrama de tempo da execucao: ");
    printf("\n\ntempo");
    for(i = 0; i < quant_processos; i++){ // imprime os indices dos processos
    	if(i < 9){
    		printf(" P0%d", i + 1);
		}
    	else {
    		printf(" P%d", i + 1);
		}	
	}
    printf("\n");
    
    int tempo_atual = 0;
    int tempo_total = 0;
    
    for(i = 0; i < quant_processos; i++){ // soma o tempo total de duração de todos os processos
        tempo_total = tempo_total + processos[i].decremento;
    }
    
    int escalonamento[tempo_total];
    int menor_processo;
    
    for(i = 0; i < tempo_total; i++){ // escalona os processos de acordo com o algoritmo STRN
    	tempo_atual = i;
    	menor_processo = verifica_menor_processo(tempo_atual, processos);
		if(menor_processo != -1){
			escalonamento[tempo_atual] = menor_processo;
    		processos[menor_processo].decremento = processos[menor_processo].decremento - 1;
    		if(processos[menor_processo].decremento == 0){
    			processos[menor_processo].status = true;
    			processos[menor_processo].termino = i + 1;
			}
		}
	}
	
	int j;
    for (j = 0; j < tempo_total; j++){ // imprime a situação dos processos conforme o tempo
    	if((j < 10) && (j + 1 < 10)){
    		printf ("0%d-0%d ", j, j + 1);
		}
		else if((j < 10) && (j + 1 >= 10)){
    		printf ("0%d-%d ", j, j + 1);
		}
		else{
    		printf ("%d-%d ", j, j + 1);
		}
    	for (i = 0; i < quant_processos; i++){
        	if(escalonamento[j] == i){
            	printf("### ");
        	}
        	else{
            	printf("--- ");
        	}
    	}
    	printf("\n");
	}
	
	int soma_turn_around;
	for(i = 0; i < quant_processos; i++){ // soma os turn arounds dos processos 
		processos[i].turn_around = processos[i].termino - processos[i].criacao;
		soma_turn_around = soma_turn_around + processos[i].turn_around;
	}
	
	// calcula e imprime o tempo medio de vida dos processos
	float temp_vida_medio_resto;
	temp_vida_medio_resto = soma_turn_around % quant_processos;
	float temp_vida_medio_dec;
	temp_vida_medio_dec = temp_vida_medio_resto / quant_processos;
	float temp_vida_medio_int;
	temp_vida_medio_int = soma_turn_around / quant_processos;
	printf("\nTempo medio de vida: ");
	float temp_vida_medio;
	temp_vida_medio = temp_vida_medio_dec + temp_vida_medio_int;		
	printf("%0.2f ", temp_vida_medio);
	
	int temp_espera_total = 0;
	for(i = 0; i < quant_processos; i++){ // soma o tempo de espera total dos processos
		processos[i].temp_espera = processos[i].turn_around - processos[i].duracao;
		temp_espera_total = temp_espera_total + processos[i].temp_espera;
	}
	
	//calcula e imprime o tempo medio de espera dos processos
	float temp_espera_medio_resto;
	temp_espera_medio_resto = temp_espera_total % quant_processos;
	float temp_espera_medio_dec;
	temp_espera_medio_dec = temp_espera_medio_resto / quant_processos;
	float temp_espera_medio_int;
	temp_espera_medio_int = temp_espera_total / quant_processos;
	printf("\n\nTempo medio de espera: ");
	float temp_espera_medio;
	temp_espera_medio = temp_espera_medio_dec + temp_espera_medio_int;	
	printf("%0.2f ", temp_espera_medio);
	
	// calcula e imprime o numero de troca de contextos dos processos
	int troca_contextos = 0;
	for(i = 0; i < tempo_total; i++){
        if((i + 1 < tempo_total) && (escalonamento[i] != escalonamento[i + 1])){
            troca_contextos++;
        }
    }
    printf("\n\nNumero de troca de contextos: %d", troca_contextos);
	
    return 0;
}

int verifica_menor_processo(int tempo_atual, processo* processos){
	int i;
	int menor_processo_atual = -1;
	for(i = 0; i < quant_processos; i++){
		if((processos[i].criacao <= tempo_atual) && (processos[i].status == false)){
			if(menor_processo_atual == -1){
				menor_processo_atual = i;
			}
			if(processos[i].decremento < processos[menor_processo_atual].decremento){
				menor_processo_atual = i;
			}
		}
	}
	return menor_processo_atual;
}   
