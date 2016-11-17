/*
 * RestricaoFraca4RoomStability.cpp
 *
 *  Created on: May 23, 2015
 *      Author: erika
 */

#include "RestricaoFraca4RoomStability.h"

#include <list>
#include "../Model/Problema.h"


double RestricaoFraca4RoomStability::contaViolacaoRestricao(Individuo* ind, Problema* p){
	int count = 0;
	int i, j;
	for( i=0; i<p->nDisciplinas; i++ ){
		for( j=0; j<p->nSalas; j++ ){
			if( ind->Alocacao_salas_utilizadas[i][j] > 0 ) count++;
		}
		count--;
	}
	ind->soft4 = peso * count;
//	printf("Soft4: %d\n", ind->soft4);
	return peso * count;
}

double RestricaoFraca4RoomStability::contaViolacaoRestricao(Individuo* ind, Disciplina* d, int nSalas){
	int count = -1;
	int j;
//	printf("d%2d: ", d->numeroSequencial);
	for( j=0; j<nSalas; j++ ){
//		printf("%d ", ind->Alocacao_salas_utilizadas[d->numeroSequencial][j]);
		if( ind->Alocacao_salas_utilizadas[d->numeroSequencial][j] > 0 ) count++;
	}
//	printf("\tcount = %d\n", count);
	return count;
}

double RestricaoFraca4RoomStability::contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m){
	int count = peso*contaViolacaoRestricao(ind, a->aula->disciplina, p->nSalas);
	if( m != NULL) m->deltaSoft4+=count;
//	printf("Soft4: %d\n", count);
	return count;
}

double RestricaoFraca4RoomStability::contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m){
	list<Alocacao*>::iterator itAloc;
	list<Disciplina*> listaDisc;
	list<Disciplina*>::iterator itDisc;
	int count = 0;


	for( itAloc=listaAloc.begin(); itAloc!=listaAloc.end(); itAloc++ ){
		if( (*itAloc)->aula != NULL ){
			listaDisc.push_back((*itAloc)->aula->disciplina);
		}
	}
	listaDisc.sort();
	listaDisc.unique();

	for( itDisc=listaDisc.begin(); itDisc!=listaDisc.end(); itDisc++ ){
		count += peso*contaViolacaoRestricao(ind, (*itDisc), p->nSalas);
	}

	if( m != NULL) m->deltaSoft4+=count;
//	printf("Soft4: %d\n", count);
	return count;
}
