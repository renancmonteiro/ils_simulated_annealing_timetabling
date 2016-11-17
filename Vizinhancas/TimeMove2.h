/*
 * TimeMove.h
 *
 *  Created on: Aug 16, 2015
 *      Author: renan
 */


#ifndef TIMEMOVE2_H_
#define TIMEMOVE2_H_

#include "Movimento.h"

#include "../Model/Individuo.h"
#include "../Model/Problema.h"


class TimeMove2 : public Movimento{
public:
	TimeMove2(Problema* p, Individuo* piInd);
	void aplicaMovimento();

private:
	Problema* problema;
	int calculaDeltaFitTimeMove2(Problema* p);
	void aplicaTimeMove2SemRecalculoFuncaoObjetivo();
	list<Alocacao*>::iterator itSubListaAulasVaziaInicio;
	list<Alocacao*>::iterator itSubListaAulasVaziaFinal;
};

#endif /* TIMEMOVE_H_ */
