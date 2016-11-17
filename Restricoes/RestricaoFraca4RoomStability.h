/*
 * RestricaoFraca4RoomStability.h
 *
 *  Created on: May 23, 2015
 *      Author: erika
 */

#ifndef RESTRICAOFRACA4ROOMSTABILITY_H_
#define RESTRICAOFRACA4ROOMSTABILITY_H_

#include "Restricao.h"
#include "../Model/Disciplina.h"

class RestricaoFraca4RoomStability: public Restricao {
public:
	RestricaoFraca4RoomStability(){ peso = 1;};
	virtual ~RestricaoFraca4RoomStability(){ };
	double contaViolacaoRestricao(Individuo* ind, Problema* p);
	double contaViolacaoRestricao(Individuo* ind, Disciplina* d, int nSalas);
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m);
	double contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m);
};

#endif /* RESTRICAOFRACA4ROOMSTABILITY_H_ */
