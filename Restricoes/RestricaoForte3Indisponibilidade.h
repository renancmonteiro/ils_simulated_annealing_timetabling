/*
 * RestricaoIndisponibilidade.h
 *
 *  Created on: May 12, 2015
 *      Author: erika
 */

#ifndef RESTRICAOINDISPONIBILIDADE_H_
#define RESTRICAOINDISPONIBILIDADE_H_

#include "Restricao.h"

class RestricaoForte3Indisponibilidade: public Restricao {
public:
	int** MatrizIndisponibilidade;
	int nLinhas, nColunas;

	RestricaoForte3Indisponibilidade(int, int);
	double contaViolacaoRestricao(Individuo* ind, Problema* p);
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m);
	void AdicionaIndisponibilidade(int iAula, int iHorarios);
	int EhIndisponivel(int iAula, int iHorarios);
	void ImprimeIndisponibilidades();
	virtual ~RestricaoForte3Indisponibilidade();

	double restricaoViolada(Individuo* ind, Problema* p, Movimento* m);
};

#endif /* RESTRICAOINDISPONIBILIDADE_H_ */
