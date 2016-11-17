/*
 * Curriculo.h
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#ifndef CURRICULO_H_
#define CURRICULO_H_

#include <list>
#include "Disciplina.h"

using namespace std;

class Curriculo {
public:
	list<Disciplina*> disciplinas;
	char nome[30];
	int numeroSequencial;
	int numeroDisciplinas;

	Curriculo(char* piNome, int i, int pNumDisc);
	virtual ~Curriculo();
};

#endif /* CURRICULO_H_ */
