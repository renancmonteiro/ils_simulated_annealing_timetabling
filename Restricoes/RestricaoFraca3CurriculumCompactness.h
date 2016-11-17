/*
 * RestricaoFraca3CurriculumCompactness.h
 *
 *  Created on: May 23, 2015
 *      Author: erika
 */

/*
 * CurriculumCompactness:
 * 	Lectures belonging to a curriculum should be adjacent to each other (i.e., in consecutive periods).
 * 	For a given curriculum we account for a violation every time there is one lecture not adjacent to any other lecture within the same day.
 * 	Each isolated lecture in a curriculum counts as 2 points of penalty.
 */

#ifndef RESTRICAOFRACA3CURRICULUMCOMPACTNESS_H_
#define RESTRICAOFRACA3CURRICULUMCOMPACTNESS_H_

#include <list>
#include <stdlib.h>

#include "Restricao.h"
#include "../Model/Curriculo.h"

class RestricaoFraca3CurriculumCompactness: public Restricao {
public:
	RestricaoFraca3CurriculumCompactness(){ peso = 2; };
	~RestricaoFraca3CurriculumCompactness(){};

	double contaViolacaoRestricao(Individuo* ind, Problema* p);
	double contaViolacaoRestricao(Individuo* ind, Curriculo* c, Problema* p, Alocacao* piA);
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m);
	double contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m);
};

#endif /* RESTRICAOFRACA3CURRICULUMCOMPACTNESS_H_ */
