/*
 * Disciplina.h
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#ifndef DISCIPLINA_H_
#define DISCIPLINA_H_

#include <list>
#include <string>

class Aula;
class Curriculo;
class Professor;

using namespace std;

class Disciplina {
public:
	list<Aula*> aulas;
	int nAulas;
	list<Curriculo*> curriculos;
	int nCurriculos;//Usado na ordenacao por dificuldade
//	list<Curriculo*> outrosCurriculos; //Curriculos que nao pertencem a disciplina

	char nome[30];
	char nomeProfessor[30];
	Professor* professor;
	int minDiasSemana;
	int nAlunos;
	int numeroSequencial;

	Disciplina(char* piNome, Professor* p, int n, int piNAulas, int piMinDias, int piNAlunos, int piNCurriculos, int piNumAulaSequencial);
	void imprime();

	virtual ~Disciplina();
};

#endif /* DISCIPLINA_H_ */
