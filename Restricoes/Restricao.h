/*
 * Restricao.h
 *
 *  Created on: May 12, 2015
 *      Author: erika
 */

#ifndef RESTRICAO_H_
#define RESTRICAO_H_

#include <list>
using namespace std;

#include "../Model/Individuo.h"
#include "../Vizinhancas/Movimento.h"
class Problema;

class Restricao {
public:
	int Hard;
	int peso;

	Restricao(){ Hard = 0; peso = 1;};
	virtual ~Restricao(){};

	virtual double contaViolacaoRestricao(Individuo* ind, Problema* p){return 0;};
	virtual double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m){return 0;};
	virtual double contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m){return 0;};

	virtual double restricaoViolada(Individuo* ind, Problema* p, Movimento* m){return 0;};
};

#endif /* RESTRICAO_H_ */
