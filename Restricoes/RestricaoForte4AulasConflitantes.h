/*
 * RestricaoForteAulasConflitantes.h
 *
 *  Created on: May 22, 2015
 *      Author: erika
 */

#ifndef RESTRICAOFORTEAULASCONFLITANTES_H_
#define RESTRICAOFORTEAULASCONFLITANTES_H_

#include "Restricao.h"
#include "../Model/Curriculo.h"

class RestricaoForte4AulasConflitantes: public Restricao {
public:
	RestricaoForte4AulasConflitantes(){ Hard = 1; peso = 10000;};
	~RestricaoForte4AulasConflitantes(){};
	double contaViolacaoRestricao(Individuo* ind, Problema* p);
	double contaViolacaoRestricaoCurriculo(Individuo* ind, Curriculo* d, Problema* p);
	double contaViolacaoRestricaoProfessor(Individuo* ind, Problema* p);
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m);

	double restricaoViolada(Individuo* ind, Problema* p, Movimento* m);
	int horarioEhDisponivel(Individuo* ind, Disciplina* d, int iHorarios);
};

#endif /* RESTRICAOFORTEAULASCONFLITANTES_H_ */
