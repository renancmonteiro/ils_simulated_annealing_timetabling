/*
 * RestricaoFraca3CurriculumCompactness.cpp
 *
 *  Created on: May 22, 2015
 *      Author: erika
 */

#include "RestricaoFraca3CurriculumCompactness.h"
#include "../Model/Problema.h"

/*
 * Lectures belonging to a curriculum should be adjacent to each other (i.e., in consecutive periods).
 * For a given curriculum we account for a violation every time there is one
 * lecture not adjacent to any other lecture within the same day.
 * Each isolated lecture in a curriculum counts as 2 points of penalty
 */

double RestricaoFraca3CurriculumCompactness::contaViolacaoRestricao(Individuo* ind, Problema* p){
	int count = 0;
	list<Curriculo*>::iterator itCurr;

	for(itCurr = p->curriculos.begin(); itCurr != p->curriculos.end(); itCurr++){
		count += contaViolacaoRestricao(ind, (*itCurr), p, NULL);
	}

	ind->soft3 = peso * count;
//	printf("Soft3: %d\n", ind->soft3);
	return peso * count;
}


double RestricaoFraca3CurriculumCompactness::contaViolacaoRestricao(Individuo* ind, Curriculo* c, Problema* p, Alocacao* piA){
	int contaViolacao = 0;

//	printf("Curriculo %d %s\n", c->numeroSequencial, c->nome);
//	printf("--Avalia Curriculum Compactness\n");
    for (int d=0;d<p->nDias;d++) {
        for (int s=0;s<p->nPerDias;s++) {
            Alocacao* a = ind->matrizAlocacaoCurriculoDiasPeriodos[c->numeroSequencial][d][s];
			if (a==NULL) continue;
//            if( piA == NULL || piA == a ){
				Alocacao* prev = (s==0?NULL:ind->matrizAlocacaoCurriculoDiasPeriodos[c->numeroSequencial][d][s-1]);
				Alocacao* next = (s+1==p->nPerDias?NULL:ind->matrizAlocacaoCurriculoDiasPeriodos[c->numeroSequencial][d][s+1]);
				if (next==NULL && prev==NULL) {
//	            	printf("aula %d (%d/%d) solitaria - curriculo %d\n", a->aula->numero, a->horario->dia, a->horario->periodo, c->numeroSequencial);
					contaViolacao++;
				}
//            }
        }
    }
//    printf("--\n");
	return contaViolacao;
}
//
//Alocacao* anterior(Individuo* ind, Alocacao* placement, Alocacao* eqCheck, int numCurriculo){
//    if (placement==NULL) return NULL;
//    Alocacao* prev = (placement->horario->horario==0?NULL:ind->matrizAlocacaoCurriculoHorario[numCurriculo][placement->horario->horario-1]);
//    if (eqCheck!=NULL && prev!=NULL && eqCheck->aula->disciplina == prev->aula->disciplina) return NULL;
//    return prev;
//
//}
//Alocacao* proximo(Individuo* ind, Alocacao* placement, Alocacao* eqCheck, int numCurriculo){
//    if (placement==NULL) return NULL;
//    Alocacao* next = (placement->horario->horario+1==ind->p->nHorarios?NULL:ind->matrizAlocacaoCurriculoHorario[numCurriculo][placement->horario->horario+1]);
//    if (eqCheck!=NULL && next!=NULL && eqCheck->aula->disciplina == next->aula->disciplina) return NULL;
//    return next;
//}

double RestricaoFraca3CurriculumCompactness::contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m){
	int count = 0;
	list<Curriculo*>::iterator itCurr;
	Disciplina* d = a->aula->disciplina;

	for(itCurr = d->curriculos.begin(); itCurr != d->curriculos.end(); itCurr++){
		count += contaViolacaoRestricao(ind, (*itCurr), p, a);
	}

	if( m != NULL) {
		m->deltaSoft3+=peso*count;
	}

	return peso*count;
}

double RestricaoFraca3CurriculumCompactness::contaViolacaoRestricaoListaAlocacao(Individuo* ind, Problema* p, list<Alocacao*> listaAloc, Movimento* m){
	list<Alocacao*>::iterator itAloc;
	int count = 0;

	list<Curriculo*>::iterator itCurr;
	Disciplina* d;

	for( itAloc=listaAloc.begin(); itAloc!=listaAloc.end(); itAloc++ ){
		if( (*itAloc)->aula != NULL ){
			d = (*itAloc)->aula->disciplina;
			for(itCurr = d->curriculos.begin(); itCurr != d->curriculos.end(); itCurr++){
				count += contaViolacaoRestricao(ind, (*itCurr), p, NULL);
			}
		}
	}

	if( m != NULL) m->deltaSoft3+=peso*count;
	return peso*count;
}
