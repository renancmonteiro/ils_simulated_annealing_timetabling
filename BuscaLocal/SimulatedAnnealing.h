/*
 * SimulatedAnnealing.h
 *
 *  Created on: May 26, 2015
 *      Author: erika
 */

#ifndef SIMULATEDANNEALING_H_
#define SIMULATEDANNEALING_H_

#include "BuscaLocal.h"

class Problema;
class Individuo;
class Movimento;
class SimulatedAnnealing: public BuscaLocal {
public:
	Problema* problema;
	double temperaturaInicial, temperaturaFinal;
	double taxaDecaimentoTemperatura;
	int aceitaPioraSA;
	int nMaxIterSemMelhora;

	SimulatedAnnealing(Problema* p, int pinMaxIterSemMelhora, double tempInicial, double tempFinal, double taxaDecaimentoTemp);
	~SimulatedAnnealing();

	Individuo* executa(Individuo* bestInd, int movimento, int imprimeDetalhes);
	void imprimeExecucao();
private:

	Movimento* obtemMovimento(Individuo* ind, int movimento);
};

#endif /* SIMULATEDANNEALING_H_ */
