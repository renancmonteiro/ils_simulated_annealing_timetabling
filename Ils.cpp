#include "Ils.h"

 ILS::ILS() {
	nMaxIter = 999;

	//SA
	temperaturaInicial = 1.5;
	temperaturaFinal = 0.005;
	taxaDecaimentoTemperatura = 0.999;
	aceitaPioraSA = 1;
	nMaxIterSemMelhora = 500;

	mediaSolucoes = 0;
	soft1 = soft2 = soft3 = soft4 = 0;
	f1 = f2 = 0;
	info = 0;
	infoSD = 0;
	imprimeGrafico = 0;
	iteracoesExecutadas = 0;
	tempoExecucao = 0;
	tempoLimite = 192;
	seed = time(NULL);
	nMovimentosRealizados = 0;

	nMoves = nSwaps = nTimeMoves = nRoomMoves = 0;
	nMovesValidos = nSwapsValidos = nTimeMovesValidos = nRoomMovesValidos = 0;
	nMovesMelhora = nSwapsMelhora = nTimeMovesMelhora = nRoomMovesMelhora = 0;
	nMovesInvalido = nSwapsInvalido = nTimeMovesInvalido = nRoomMovesInvalido = 0;
}

Individuo* ILS::executa(Problema* p) {

	Individuo *ind, *bestInd = NULL, *indConstrucao = NULL;
	GeraSolucaoInicialGulosa* geraSolucaoG;
	Pertubacao *pert = NULL;
	
	float tempo_inicial, tempo_construcao, tempo_busca_local, tempo_pertubacao;
	int fitness_inicial, fitness_final;

	srand(seed);
	tempo_inicial = clock() / CLOCKS_PER_SEC;

	// CONSTRUÇÃO
	indConstrucao = new Individuo(p);
	geraSolucaoG = new GeraSolucaoInicialGulosa(p, 0.0);
	geraSolucaoG->CriaIndividuoInicial(p, indConstrucao);
	delete (geraSolucaoG);

	p->CalculaFuncaoObjetivo(indConstrucao);
	f1 += indConstrucao->fitness;

	if (indConstrucao->fitness >= 5000) {
		fprintf(stderr,
			"ERRO Construcao ILS: Restricao Forte Conflitante\n");
		return indConstrucao;
	}

	// if (info) {
		tempo_construcao = clock() / CLOCKS_PER_SEC;
		printf("\tCONSTRUCAO:  ;%d;;%f;;\n", indConstrucao->fitness,
			tempo_construcao - tempo_inicial);
	// }

	if (infoSD) {
		printf("%d\n", indConstrucao->fitness);
	}

	// busca_local
	ind = executaSimulatedAnnealing(p, indConstrucao, 6);

	f2 += ind->fitness;
	if (ind->fitness >= 10000) {
		fprintf(stderr, "ERRO Busca local: Restricao Forte Conflitantes: %d\n", ind->hard);
		exit(0);
	}

	if (info) {
		tempo_busca_local = (clock() / CLOCKS_PER_SEC);
		printf("\tBUSCA LOCAL: ;;%d;;%f;\n", ind->fitness,
			tempo_busca_local - tempo_inicial);
	}

	// busca_local 2
	ind = executaSimulatedAnnealing(p, ind, 7);
	f2 += ind->fitness;

	if (ind->fitness >= 10000) {
			fprintf(stderr, "ERRO Busca local: Restricao Forte Conflitantes: %d\n", ind->hard);
			exit(0);
	}

	if (info) {
		tempo_busca_local = (clock() / CLOCKS_PER_SEC);
		printf("\tBUSCA LOCAL: ;;%d;;%f;\n", ind->fitness,
			tempo_busca_local - tempo_inicial);
	}

	if (infoSD){
		printf("%d\n", ind->fitness);
	}

	if (info) {
		printf("\n");
	}

	while(tempoExecucao < tempoLimite){

		// pertubação
		pert = new Pertubacao(p, ind);
		ind = pert->executa();

		if(info){
			tempo_pertubacao =  clock() / CLOCKS_PER_SEC;
			printf("\tPERTUBACAO:  ;;%d;;%f;\n", ind->fitness, tempo_pertubacao - tempo_inicial);
		}

		fitness_inicial = ind->fitness;
		fitness_final = 0;

		while(fitness_inicial > fitness_final){

			// busca_local
			ind = executaSimulatedAnnealing(p, ind, 6);
			f2 += ind->fitness;
			fitness_final = ind->fitness;

			if (ind->fitness >= 10000) {
					fprintf(stderr, "ERRO Busca local: Restricao Forte Conflitantes: %d\n", ind->hard);
					exit(0);
			}

			if (info) {
					tempo_busca_local = (clock() / CLOCKS_PER_SEC);
					printf("\tBUSCA LOCAL: ;;%d;;%f;\n", ind->fitness,
						tempo_busca_local - tempo_inicial);
			}

			if (infoSD){
				printf("%d\n", ind->fitness);
			}

			if(fitness_inicial > fitness_final){
				// busca_local 2 
				ind = executaSimulatedAnnealing(p, ind, 7);
				f2 += ind->fitness;

				fitness_inicial = fitness_final;
				fitness_final = ind->fitness;

				if (ind->fitness >= 10000) {
						fprintf(stderr, "ERRO Busca local: Restricao Forte Conflitantes: %d\n", ind->hard);
						exit(0);
				}

				if (info) {
						tempo_busca_local = (clock() / CLOCKS_PER_SEC);
						printf("\tBUSCA LOCAL: ;;%d;;%f;\n", ind->fitness,
							tempo_busca_local - tempo_inicial);
				}
			}
		}

		if (infoSD)
			printf("%d\n", ind->fitness);

		soft1 += ind->soft1;
		soft2 += ind->soft2;
		soft3 += ind->soft3;
		soft4 += ind->soft4;

		mediaSolucoes += ind->fitness;

		// criterio_aceitação
		if ((bestInd == NULL) || (ind->fitness < bestInd->fitness)) {
			if (bestInd != NULL){
				delete (bestInd);
			}
			bestInd = new Individuo(ind);
		} else {
			delete(ind);
			ind = new Individuo(bestInd);
		}

		// fim_iteração
		iteracoesExecutadas++;
		
		tempoExecucao = (clock() / CLOCKS_PER_SEC) - tempo_inicial;

		if (info) {
			printf("\n");
		}
	}

	if (info) {
		printf("Executou %d iteracoes em %f s\n", iteracoesExecutadas,
			tempoExecucao);
		printf("Num Movimentos Realizados: %d\n", nMovimentosRealizados);
	}
	
	if (infoSD) {
		printf("%f\n", tempoExecucao);
		printf("%d\n", nMovimentosRealizados);
	}

	mediaSolucoes /= iteracoesExecutadas;

	soft1 /= iteracoesExecutadas;
	soft2 /= iteracoesExecutadas;
	soft3 /= iteracoesExecutadas;
	soft4 /= iteracoesExecutadas;

	f1 /= iteracoesExecutadas;
	f2 /= iteracoesExecutadas;

	return bestInd;
}

Individuo* ILS::executaSimulatedAnnealing(Problema* p, Individuo* indInicial, int vizinhanca) {
	SimulatedAnnealing* simulatedAnnealing;
	Individuo* ind;

	simulatedAnnealing = new SimulatedAnnealing(p, nMaxIterSemMelhora,
		temperaturaInicial, temperaturaFinal, taxaDecaimentoTemperatura);

	ind = simulatedAnnealing->executa(indInicial, vizinhanca, info);

	nMovimentosRealizados += simulatedAnnealing->nMovimentosRealizados;

	nMoves += simulatedAnnealing->nMoves;
	nSwaps += simulatedAnnealing->nSwaps;
	nTimeMoves += simulatedAnnealing->nTimeMoves;
	nRoomMoves += simulatedAnnealing->nRoomMoves;
	
	nMovesMelhora += simulatedAnnealing->nMovesMelhora;
	nSwapsMelhora += simulatedAnnealing->nSwapsMelhora;
	nTimeMovesMelhora += simulatedAnnealing->nTimeMovesMelhora;
	nRoomMovesMelhora += simulatedAnnealing->nRoomMovesMelhora;
	
	nMovesValidos += simulatedAnnealing->nMovesValidos;
	nSwapsValidos += simulatedAnnealing->nSwapsValidos;
	nTimeMovesValidos += simulatedAnnealing->nTimeMovesValidos;
	nRoomMovesValidos += simulatedAnnealing->nRoomMovesValidos;
	
	nMovesInvalido += simulatedAnnealing->nMovesInvalido;
	nSwapsInvalido += simulatedAnnealing->nSwapsInvalido;
	nTimeMovesInvalido += simulatedAnnealing->nTimeMovesInvalido;
	nRoomMovesInvalido += simulatedAnnealing->nRoomMovesInvalido;
	
	delete (simulatedAnnealing);
	return ind;
}

void ILS::ImprimeExecucao() {
	printf("\nSeed:  %d\n", seed);
	printf("Numero maximo de iteracoes:  %d\n", nMaxIter);

	printf("Busca Local: SA\n");
	printf("Variacao de temperatura:  %lf a %lf, a uma taxa de %lf\n", temperaturaInicial, temperaturaFinal, taxaDecaimentoTemperatura);
	printf("N:  500\n");
	printf("Na: %3d\n", nMaxIterSemMelhora);

	printf("\nNumero de Movimentos Realizados: %d\n", nMovimentosRealizados);

	if( nMovimentosRealizados > 100 ){
		printf(
			"Total de Movimentos   :  nMv: %5d nSw: %5d nTimeMv: %5d nRoomMv: %5d\n",
			nMoves, nSwaps, nTimeMoves, nRoomMoves);
		printf(
			"Movimentos Invalidos  :  nMv: %5d nSw: %5d nTimeMv: %5d nRoomMv: %5d\n",
			nMovesInvalido, nSwapsInvalido, nTimeMovesInvalido, nRoomMovesInvalido);
		printf(
			"Movimentos Validos    :  nMv: %5d nSw: %5d nTimeMv: %5d nRoomMv: %5d\n",
			nMovesValidos, nSwapsValidos, nTimeMovesValidos, nRoomMovesValidos);
		printf(
			"Movimentos com Melhora:  nMv: %5d nSw: %5d nTimeMv: %5d nRoomMv: %5d",
			nMovesMelhora, nSwapsMelhora, nTimeMovesMelhora, nRoomMovesMelhora);
	}

	printf("\nIteracoes do ILS executadas: %d em %lf s\n",
		iteracoesExecutadas, tempoExecucao);
	printf("Media das solucoes:  %d\n", mediaSolucoes);
}

void ILS::ImprimeExecucaoResumida() {
	if (nMoves > 0) {
		printf("Move\n");
		printf("%6d\n", nMoves);
		printf("%6d\n", nMovesInvalido);
		printf("%6d\n", nMovesValidos);
		printf("%6d\n", nMovesMelhora);
	}
	if (nSwaps > 0) {
		printf("Swap\n");
		printf("%6d\n", nSwaps);
		printf("%6d\n", nSwapsInvalido);
		printf("%6d\n", nSwapsValidos);
		printf("%6d\n", nSwapsMelhora);
	}
}