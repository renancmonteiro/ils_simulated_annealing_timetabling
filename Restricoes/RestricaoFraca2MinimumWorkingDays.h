/*
 * RestricaoFraca2MinimumWorkingDays.h
 *
 *  Created on: May 23, 2015
 *      Author: erika
 */

/*
 * MinimumWorkingDays:
 * 	The lectures of each course must be spread into a minimum number of days.
 * 	Each day below the minimum counts as 5 points of penalty.
 */


#ifndef RESTRICAOFRACA2MINIMUMWORKINGDAYS_H_
#define RESTRICAOFRACA2MINIMUMWORKINGDAYS_H_

#include <list>

#include "Restricao.h"

class RestricaoFraca2MinimumWorkingDays: public Restricao {
public:
	RestricaoFraca2MinimumWorkingDays(){ peso = 5;};
	~RestricaoFraca2MinimumWorkingDays(){};

	double contaViolacaoRestricao(Individuo* ind, Problema* p);
	double contaViolacaoRestricao(Individuo* ind, Disciplina* d, int nDiasSemana);
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m);
	double contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m);

};

#endif /* RESTRICAOFRACA2MINIMUMWORKINGDAYS_H_ */
