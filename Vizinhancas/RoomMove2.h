/*
 * RoomMove.h
 *
 *  Created on: Aug 16, 2015
 *      Author: renan
 */


#ifndef RoomMove2_H_
#define RoomMove2_H_

#include "Movimento.h"
#include "../Model/Individuo.h"
#include "../Model/Problema.h"


class RoomMove2 : public Movimento{
public:
	RoomMove2(Problema* p, Individuo* piInd);
	void aplicaMovimento();

private:
	list<Alocacao*> salasPossiveis;
	void aplicaRoomMove2SemRecalculoFuncaoObjetivo();
	int calculaDeltaFitRoomMove2(Problema* p);
	void imprimeListaVazia();
	void imprimeListaAlocada();
};

#endif /* RoomMove_H_ */
