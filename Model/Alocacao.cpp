/*
 * Alocacao.cpp
 *
 *  Created on: May 12, 2015
 *      Author: erika
 */

#include <stdlib.h>
#include <stdio.h>

#include "Alocacao.h"

#include "Disciplina.h"
#include "Curriculo.h"

Alocacao::Alocacao(int piId){
	id = piId;
	aula = NULL;
	sala = NULL;
	horario = NULL;

//	custoAlocacaoHard  = 0;
//	custoAlocacaoSoft1 = 0;
//	custoAlocacaoSoft2 = 0;
//	custoAlocacaoSoft3 = 0;
//	custoAlocacaoSoft4 = 0;
}

Alocacao::Alocacao(Alocacao* a){
	id = a->id;
	aula = a->aula;
	sala = a->sala;
	horario = a->horario;

//	custoAlocacaoHard  = a->custoAlocacaoHard;
//	custoAlocacaoSoft1 = a->custoAlocacaoSoft1;
//	custoAlocacaoSoft2 = a->custoAlocacaoSoft2;
//	custoAlocacaoSoft3 = a->custoAlocacaoSoft3;
//	custoAlocacaoSoft4 = a->custoAlocacaoSoft4;
}

void Alocacao::imprime(){
    list<Curriculo*>::iterator itCurr;

    if(aula != NULL){
			printf("Aula %d/%s ( ", aula->disciplina->numeroSequencial, aula->disciplina->nome);
			for( itCurr = aula->disciplina->curriculos.begin(); itCurr!=aula->disciplina->curriculos.end(); itCurr++){
				printf("%d ", (*itCurr)->numeroSequencial);
		}
		printf("): ");
    }
	printf("sala %d no horario %d/%d (%d)\n", sala->numeroSequencial, horario->dia, horario->periodo, horario->horario);
}

Alocacao::~Alocacao(){
}
