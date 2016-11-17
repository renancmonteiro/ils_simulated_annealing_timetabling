/*
 * BuscaLocal.h
 *
 *  Created on: May 26, 2015
 *      Author: erika
 */

#ifndef BUSCALOCAL_H_
#define BUSCALOCAL_H_

#include "../Model/Individuo.h"

class BuscaLocal {
public:
	int nMovimentosRealizados;
	int nMoves, nSwaps, nTimeMoves, nRoomMoves;
	int nMovesValidos, nSwapsValidos, nTimeMovesValidos, nRoomMovesValidos;
	int nMovesMelhora, nSwapsMelhora, nTimeMovesMelhora, nRoomMovesMelhora;
	int nMovesInvalido, nSwapsInvalido, nTimeMovesInvalido, nRoomMovesInvalido;

	BuscaLocal(){
		nMovimentosRealizados = 0;
		nMoves = nSwaps = nTimeMoves = nRoomMoves = 0;
		nMovesValidos = nSwapsValidos = nTimeMovesValidos = nRoomMovesValidos = 0;
		nMovesMelhora = nSwapsMelhora = nTimeMovesMelhora = nRoomMovesMelhora = 0;
		nMovesInvalido = nSwapsInvalido = nTimeMovesInvalido = nRoomMovesInvalido = 0;
	};
	virtual ~BuscaLocal(){};

	virtual Individuo* executa(Individuo* ind){ return ind; };
	virtual void imprimeExecucao(){};
};

#endif /* BUSCALOCAL_H_ */
