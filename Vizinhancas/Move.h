/*
 * Move.h
 *
 *  Created on: May 30, 2015
 *      Author: erika
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "Movimento.h"

#include "../Model/Individuo.h"
#include "../Model/Problema.h"

class Alocacao;
class Move : public Movimento{
public:
	Move(Problema* p, Individuo* piInd);
	Move(Individuo* piInd, Alocacao* piA1, Alocacao* piA2);
	virtual ~Move(){};
	void aplicaMovimento();
	void desfazMovimento();
	void aplicaMoveSemRecalculoFuncaoObjetivo();

private:
	int calculaDeltaFitMove(Problema* p);
};

#endif /* MOVE_H_ */
