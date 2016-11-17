/*
 * RoomMove.h
 *
 *  Created on: Aug 16, 2015
 *      Author: renan
 */


#ifndef RoomMove_H_
#define RoomMove_H_

#include "Movimento.h"
#include "../Model/Individuo.h"
#include "../Model/Problema.h"


class RoomMove : public Movimento{
public:
	RoomMove(Problema* p, Individuo* piInd);
	RoomMove(Individuo* piInd, Alocacao* al1, Alocacao* al2);
	void aplicaMovimento();

private:
	list<Alocacao*> salasPossiveis;
	void aplicaRoomMoveSemRecalculoFuncaoObjetivo();
	int calculaDeltaFitRoomMove(Problema* p);
	void imprimeListaVazia();
	void imprimeListaAlocada();
};

#endif /* RoomMove_H_ */
