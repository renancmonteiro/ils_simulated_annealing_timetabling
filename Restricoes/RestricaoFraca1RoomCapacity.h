/*
 * RestricaoFraca1RoomCapacity.h
 *
 *  Created on: May 23, 2015
 *      Author: erika
 */

/*RoomCapacity:
	For each lecture, the number of students that attend the course must be less or equal than the number of
	seats of all the rooms that host its lectures. Each student above the capacity counts as 1 point of penalty.
*/

#ifndef RESTRICAOFRACA1ROOMCAPACITY_H_
#define RESTRICAOFRACA1ROOMCAPACITY_H_

#include <list>
using namespace std;

#include "Restricao.h"
#include "../Model/Disciplina.h"

class RestricaoFraca1RoomCapacity: public Restricao{
public:
	RestricaoFraca1RoomCapacity(){};
	~RestricaoFraca1RoomCapacity(){};
	double contaViolacaoRestricao(Individuo* ind, Problema* p){
		int count = 0;
		list<Alocacao*>::iterator it;

		for(it = ind->aulasAlocadas.begin(); it != ind->aulasAlocadas.end(); it++){
			if( (*it)->aula->disciplina->nAlunos > (*it)->sala->capacidade )
				count += (*it)->aula->disciplina->nAlunos - (*it)->sala->capacidade;
		}
		ind->soft1 = count;
//		printf("Soft1: %d\n", ind->soft1);
		return count;
	}
	double contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m){
		int count = 0;

		if( a->aula->disciplina->nAlunos > a->sala->capacidade )
			count = a->aula->disciplina->nAlunos - a->sala->capacidade;

		if( m != NULL) m->deltaSoft1+=count;
//		printf("Soft1: %d\t", count);
		return count;
	}
	double contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m){
		int count = 0;
		list<Alocacao*>::iterator it;
		Alocacao* a;

		for(it = listaAloc.begin(); it != listaAloc.end(); it++){
			a = (*it);
			if( a->aula->disciplina->nAlunos > a->sala->capacidade )
				count += a->aula->disciplina->nAlunos - a->sala->capacidade;
		}

		if( m != NULL) m->deltaSoft1+=count;
//		printf("Soft1: %d\t", count);
		return count;
	};
};

#endif /* RESTRICAOFRACA1ROOMCAPACITY_H_ */
