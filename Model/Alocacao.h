/*
 * Alocacao.h
 *
 *  Created on: May 12, 2015
 *      Author: erika
 */

#ifndef ALOCACAO_H_
#define ALOCACAO_H_

#include "Aula.h"
#include "Timeslot.h"
#include "Sala.h"

class Alocacao {
public:
	Aula* aula;
	Timeslot* horario;
	Sala* sala;
	int id;

	Alocacao(int piId);
	Alocacao(Alocacao* a);
	void imprime();
	~Alocacao();
};

#endif /* ALOCACAO_H_ */
