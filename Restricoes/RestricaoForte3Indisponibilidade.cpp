/*
 * RestricaoIndisponibilidade.cpp
 *
 *  Created on: May 12, 2015
 *      Author: erika
 */

#include "RestricaoForte3Indisponibilidade.h"

#include <stdio.h>
#include <stdlib.h>
#include<list>
using namespace std;

#include "../Model/Alocacao.h"
#include "../Model/Disciplina.h"


RestricaoForte3Indisponibilidade::RestricaoForte3Indisponibilidade(int piNAulas, int piNHorarios) {
	int i, j;
	Hard = 1;
	peso = 10000;
	nLinhas = piNAulas;
	nColunas = piNHorarios;
	MatrizIndisponibilidade = (int**) malloc (nLinhas*sizeof(int*));
	for( i=0; i<nLinhas; i++ ){
		MatrizIndisponibilidade[i] = (int*) malloc (nColunas*sizeof(int));
		for( j=0; j<nColunas; j++ ) MatrizIndisponibilidade[i][j] = 0;
	}
}

double RestricaoForte3Indisponibilidade::contaViolacaoRestricao(Individuo* ind, Problema* p){
	list<Alocacao*>::iterator it;
	int numeroAula, horario;

	for(it = ind->aulasAlocadas.begin(); it != ind->aulasAlocadas.end(); it++){
		numeroAula = (*it)->aula->disciplina->numeroSequencial;
		horario = (*it)->horario->horario;
		if( MatrizIndisponibilidade[numeroAula][horario] ) {
			printf("Aula Indisponivel: %d.%s - horario %d\n", numeroAula, (*it)->aula->disciplina->nome, horario);
			return peso;
		}
	}
	return 0;
}

double RestricaoForte3Indisponibilidade::contaViolacaoRestricaoAlocacao(Individuo* ind, Problema* p, Alocacao* a, Movimento* m){
	if( a->aula != NULL ){
		int numeroAula = a->aula->disciplina->numeroSequencial;
		if( MatrizIndisponibilidade[numeroAula][a->horario->horario] ) { if( m != NULL) m->deltaHard+=peso; return peso; }
	}
	return 0;
}

void RestricaoForte3Indisponibilidade::AdicionaIndisponibilidade(int iAula, int iHorarios){
	if( iAula >= nLinhas ) {
		printf("Erro ao adicionar a indisponibilidade para a aula %d (no horario %d)", iAula, iHorarios);
		exit(0);
	}
	if( iHorarios >= nColunas ) {
		printf("Erro ao adicionar a indisponibilidade para o horario %d (aula %d)", iHorarios, iAula);
		exit(0);
	}
	MatrizIndisponibilidade[iAula][iHorarios] = 1;
};
int RestricaoForte3Indisponibilidade::EhIndisponivel(int iAula, int iHorarios){
	return MatrizIndisponibilidade[iAula][iHorarios];
}

RestricaoForte3Indisponibilidade::~RestricaoForte3Indisponibilidade() {
	int i;
	for( i=0; i<nLinhas; i++ )
		free(MatrizIndisponibilidade[i]);
	free(MatrizIndisponibilidade);
}

void RestricaoForte3Indisponibilidade::ImprimeIndisponibilidades(){
	int i, j;
	for (i = 0; i < nLinhas; i++) {
	        for (j = 0; j <nColunas; j++) {
	        	printf("%d ", MatrizIndisponibilidade[i][j]);
	        }
	        printf("\n");
	    }
}

double RestricaoForte3Indisponibilidade::restricaoViolada(Individuo* ind, Problema* p, Movimento* m){
	list<Alocacao*>::iterator it;
	int numeroAula, horario;

	if( m->a1->aula != NULL ){
		numeroAula = m->a1->aula->disciplina->numeroSequencial;
		horario    = m->a1->horario->horario;
		if( MatrizIndisponibilidade[numeroAula][horario] ){
			if( m != NULL) m->deltaHard+=peso;
			return peso;
		}
	}
	if( m->a2->aula != NULL ){
		numeroAula = m->a2->aula->disciplina->numeroSequencial;
		horario    = m->a2->horario->horario;
		if( MatrizIndisponibilidade[numeroAula][horario] ){
			if( m != NULL) m->deltaHard+=peso;
			return peso;
		}
	}

	return 0;
}
