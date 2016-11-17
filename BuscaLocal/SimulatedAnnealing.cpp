/*
 * SimulatedAnnealing.cpp
 *
 *  Created on: May 26, 2015
 *      Author: erika
 */

#include <stdlib.h>
#include <math.h>

#include "SimulatedAnnealing.h"
#include "../Model/Problema.h"
#include "../Vizinhancas/Movimento.h"
#include "../Vizinhancas/Move.h"
#include "../Vizinhancas/Swap.h"
#include "../Vizinhancas/TimeMove.h"
#include "../Vizinhancas/TimeMove2.h"
#include "../Vizinhancas/RoomMove.h"
#include "../Vizinhancas/RoomMove2.h"

 const int _MOVE = 1;
 const int _SWAP = 2;
 const int _KEMPE = 3;
 const int _TIMEMOVE = 4;
 const int _ROOMMOVE = 5;

 const int _MOVE_SWAP = 6;
 const int _TIME_ROOM = 7;

SimulatedAnnealing::SimulatedAnnealing(Problema* p, int pinMaxIterSemMelhora, double tempInicial, double tempFinal, double taxaDecaimentoTemp) {
	nMaxIterSemMelhora = pinMaxIterSemMelhora;
	problema = p;
	temperaturaInicial = tempInicial;
	temperaturaFinal   = tempFinal;
	taxaDecaimentoTemperatura = taxaDecaimentoTemp;
	aceitaPioraSA = 1;
}

SimulatedAnnealing::~SimulatedAnnealing() { }


Movimento* SimulatedAnnealing::obtemMovimento(Individuo* ind, int movimento){
	Movimento* mov;
	int movARealizar;

	float sorteio = (float) rand() / RAND_MAX;

	switch( movimento ){
		case _MOVE_SWAP:
			if(sorteio < 0.5){ 
				movARealizar = _MOVE;
			} else {
				movARealizar = _SWAP;
			}
			break;
		case _TIME_ROOM:
			if(sorteio < 0.5){
			 	movARealizar = _TIMEMOVE;
			} else {
			 	movARealizar = _ROOMMOVE;
			}
			break;
		default: 
			movARealizar = movimento;
	}

	switch(movARealizar){
	case _MOVE:  mov = new Move(problema, ind);
			     nMoves++;
			     if( mov->deltaFit < 0 ){
			    	 nMovesMelhora++;
			     }
			     else if( (mov->deltaHard <= 0) || (mov->deltaFit < 10000) ){
			 	 	nMovesValidos++;
			 	 }
			     else nMovesInvalido++;
		         break;
	case _SWAP:  mov = new Swap(problema, ind);
			     nSwaps++;
			     if( mov->deltaFit < 0 ){
			    	 nSwapsMelhora++;
			     }
			     else if( (mov->deltaHard <= 0) || (mov->deltaFit < 10000) ){
			 	 	nSwapsValidos++;
			 	 }
			     else nSwapsInvalido++;
		         break;
	case _TIMEMOVE: mov = new TimeMove2(problema, ind);
				 nTimeMoves++;
			     if( mov->deltaFit < 0 ){
			    	 nTimeMovesMelhora++;
			     }
			     else if( (mov->deltaHard <= 0) || (mov->deltaFit < 10000) ){
			 	 	nTimeMovesValidos++;
			 	 }
			     else nTimeMovesInvalido++;
		         break;
	case _ROOMMOVE: mov = new RoomMove2(problema, ind);
					nRoomMoves++;
					if( mov->deltaFit < 0 ){
						nRoomMovesMelhora++;
					}
					else if( (mov->deltaHard <= 0) || (mov->deltaFit < 10000) ){
						nRoomMovesValidos++;
					}
					else nRoomMovesInvalido++;
					break;
	}
	nMovimentosRealizados++;
	//printf(";%4d; %3d; %3d; %3d\n", mov->deltaFit, (mov->a1->sala->numeroSequencial*problema->nHorarios)+mov->a1->horario->horario, (mov->a2->sala->numeroSequencial*problema->nHorarios)+mov->a2->horario->horario);

	return mov;
}

Individuo* SimulatedAnnealing::executa(Individuo* bestInd, int movimento, int imprimeDetalhes){
	Movimento *vizinho;
	Individuo* melhorInd, *solucaoAtual;
	double prob;

	long N, i;
	double tAtual = temperaturaInicial;

	solucaoAtual = bestInd;
	melhorInd = new Individuo(solucaoAtual);
	i = 0;
	do {
		N = 500;
//		printf("i = %ld\n", i);
		i = 0;
		do {
			vizinho = obtemMovimento(solucaoAtual, movimento);
			if( vizinho->deltaFit <= 0 ){// função objetivo decresceu
				vizinho->aplicaMovimento();
				i++;

				if (solucaoAtual->fitness < melhorInd->fitness) {
					delete(melhorInd);
					melhorInd = new Individuo(solucaoAtual);
				}
			} else if( vizinho->deltaFit < 3000 ){
				// calcula probabilidade de aceitação
				prob = pow(M_E, -vizinho->deltaFit / tAtual);
				if( aceitaPioraSA && (((float) rand() / RAND_MAX) <= prob) ){
					vizinho->aplicaMovimento();
					i++;
				}
			}
			delete(vizinho);
			N--;

			if( i >= nMaxIterSemMelhora ) break;
		} while( N > 0 );

//		if( i >= nMaxIterSemMelhora ) break;
		tAtual *= taxaDecaimentoTemperatura;
	} while (tAtual > temperaturaFinal);

	delete(solucaoAtual);
	return melhorInd;
}

void SimulatedAnnealing::imprimeExecucao(){
	printf("Maximo de Iteracoes sem Melhora: %d\n", nMaxIterSemMelhora);
	printf("Variacao de temperatura:  %lf a %lf, a uma taxa de %lf\n", temperaturaInicial, temperaturaFinal, taxaDecaimentoTemperatura);
	printf("\nNumero de Movimentos Realizados: %d\n", nMovimentosRealizados);
  	printf("Total de Movimentos   :  nMoves: %7d nSwaps: %7d nTimeMoves: %7d nRoomMoves: %7d\n", nMoves, nSwaps, nTimeMoves, nRoomMoves);
	printf("Movimentos Invalidos  :  nMoves: %7d nSwaps: %7d nTimeMoves: %7d nRoomMoves: %7d\n", nMovesInvalido, nSwapsInvalido, nTimeMovesInvalido, nRoomMovesInvalido);
	printf("Movimentos Validos    :  nMoves: %7d nSwaps: %7d nTimeMoves: %7d nRoomMoves: %7d\n", nMovesValidos, nSwapsValidos, nTimeMovesValidos, nRoomMovesValidos);
	printf("Movimentos com Melhora:  nMoves: %7d nSwaps: %7d nTimeMoves: %7d nRoomMoves: %7d\n", nMovesMelhora, nSwapsMelhora, nTimeMovesMelhora, nRoomMovesMelhora);
}
