/*
 * TimeMove.h
 *
 *  Created on: Aug 16, 2015
 *      Author: renan
 */


#ifndef TIMEMOVE_H_
#define TIMEMOVE_H_

#include "Movimento.h"

#include "../Model/Individuo.h"
#include "../Model/Problema.h"


class TimeMove : public Movimento{
public:
	TimeMove(Problema* p, Individuo* piInd);
	TimeMove(Individuo* piInd, Alocacao* al1, Alocacao* al2);
	void aplicaMovimento();

private:
	Movimento *m;
	int calculaDeltaFitTimeMove(Problema* p);
	void aplicaTimeMoveSemRecalculoFuncaoObjetivo();
	list<Alocacao*>::iterator itSubListaAulasVaziaInicio;
	list<Alocacao*>::iterator itSubListaAulasVaziaFinal;
};

#endif /* TIMEMOVE_H_ */
