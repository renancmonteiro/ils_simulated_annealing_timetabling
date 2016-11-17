/*
 * Swap.h
 *
 *  Created on: Jun 8, 2015
 *      Author: erika
 */

#ifndef SWAP_H_
#define SWAP_H_

#include "Movimento.h"

#include "../Model/Individuo.h"
#include "../Model/Problema.h"

class Swap : public Movimento{
public:
	Swap(Problema* p, Individuo* piInd);
	Swap(Individuo* piInd, Alocacao* piA1, Alocacao *piA2);
	virtual ~Swap();
	void aplicaMovimento();
	void desfazMovimento();
	void aplicaMoveSemRecalculoFuncaoObjetivo();

private:
	int calculaDeltaFitSwap(Problema* p);
};

#endif /* SWAP_H_ */
