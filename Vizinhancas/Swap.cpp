/*
 * Swap.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: erika
 */

#include <string.h>

#include "Swap.h"

#include "../Model/Alocacao.h"
#include "../Model/Aula.h"
#include "../Model/Disciplina.h"
using namespace std;


Swap::Swap(Problema* p, Individuo* piInd) {
  tipoMovimento = 2;

  list<Alocacao*>::iterator it1, it2;
	int pos1 = rand() % piInd->aulasAlocadas.size();
	int pos2 = rand() % piInd->aulasAlocadas.size();

	ind = piInd;
	tipoMovimento = 2;

	it1 = piInd->aulasAlocadas.begin();
	it2 = piInd->aulasAlocadas.begin();
	advance(it1, pos1);
	advance(it2, pos2);

	a1 = *(it1);
	while( (*it2)->aula->disciplina->numeroSequencial == a1->aula->disciplina->numeroSequencial ){
		it2++;
		if( it2 == ind->aulasAlocadas.end() ) it2 = ind->aulasAlocadas.begin();
	}
	a2 = (*it2);

	deltaFit = calculaDeltaFitSwap(p);
}

Swap::Swap(Individuo* piInd, Alocacao* piA1, Alocacao *piA2){
  tipoMovimento = 2;

  ind = piInd;
  a1 = piA1;
  a2 = piA2;

  deltaFit = calculaDeltaFitSwap(piInd->p);
}

Swap::~Swap(){}


void Swap::aplicaMovimento(){
	list<Curriculo*>::iterator itCurr;

	aplicaMoveSemRecalculoFuncaoObjetivo();

	ind->fitness += deltaFit;
	ind->hard  += deltaHard;
	ind->soft1 += deltaSoft1;
	ind->soft2 += deltaSoft2;
	ind->soft3 += deltaSoft3;
	ind->soft4 += deltaSoft4;
}

void Swap::desfazMovimento(){
	list<Curriculo*>::iterator itCurr;

	aplicaMoveSemRecalculoFuncaoObjetivo();

	ind->fitness -= deltaFit;
	ind->hard  -= deltaHard;
	ind->soft1 -= deltaSoft1;
	ind->soft2 -= deltaSoft2;
	ind->soft3 -= deltaSoft3;
	ind->soft4 -= deltaSoft4;
}

int Swap::calculaDeltaFitSwap(Problema* p){
	list<Restricao*>::iterator it;
	int deltaFitness;
	int violaRestricaoHard;

	deltaFitness  = -p->CalculaCustoAulaAlocada(ind, a1, this);//alimenta os deltaSofts
	deltaFitness -=  p->CalculaCustoAulaAlocada(ind, a2, this);//alimenta os deltaSofts

	deltaHard  = 0;
	deltaSoft1 = -deltaSoft1;
	deltaSoft2 = -deltaSoft2;
	deltaSoft3 = -deltaSoft3;
	deltaSoft4 = -deltaSoft4;
	deltaFitness = deltaSoft1 + deltaSoft2 + deltaSoft3 + deltaSoft4;

	aplicaMoveSemRecalculoFuncaoObjetivo();
	violaRestricaoHard = p->violaRestricaoGrave(ind, this);
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulaAlocada(ind, a1, this);
		deltaFitness += p->CalculaCustoAulaAlocada(ind, a2, this);
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}
	aplicaMoveSemRecalculoFuncaoObjetivo();

	return deltaFitness;
}


void Swap::aplicaMoveSemRecalculoFuncaoObjetivo(){
  list<Curriculo*>::iterator itCurr;
	Aula* temp;
	int aula1 = a1->aula->disciplina->numeroSequencial;
	int aula2 = a2->aula->disciplina->numeroSequencial;

	ind->Alocacao_dias_utilizados[aula1][a1->horario->dia]--;
	ind->Alocacao_salas_utilizadas[aula1][a1->sala->numeroSequencial]--;
	ind->Alocacao_dias_utilizados[aula2][a2->horario->dia]--;
	ind->Alocacao_salas_utilizadas[aula2][a2->sala->numeroSequencial]--;
	for( itCurr = a1->aula->disciplina->curriculos.begin(); itCurr!=a1->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario]--;
		if( ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario] == 0)
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] = NULL;
	}
	for( itCurr = a2->aula->disciplina->curriculos.begin(); itCurr!=a2->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a2->horario->horario]--;
		if( ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a2->horario->horario] == 0)
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a2->horario->dia][a2->horario->periodo] = NULL;
	}

	ind->matrizProfessorHorarioQntd[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario]--;
	if( ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] == a1)
		ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] = NULL;
//		a1->aula->disciplina->professor->restricaoHorario[a1->horario->horario] = NULL;
	ind->matrizProfessorHorarioQntd[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario]--;
	if( ind->matrizProfessorHorario[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario] == a2)
		ind->matrizProfessorHorario[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario] = NULL;
//		a2->aula->disciplina->professor->restricaoHorario[a2->horario->horario] = NULL;

	temp = a1->aula;
	a1->aula = a2->aula;
	a2->aula = temp;

	ind->matrizProfessorHorarioQntd[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario]++;
	if( ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] == NULL)
		ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] = a1;
//		a1->aula->disciplina->professor->restricaoHorario[a1->horario->horario] = a1;
	ind->matrizProfessorHorarioQntd[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario]++;
	if( ind->matrizProfessorHorario[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario] == NULL)
		ind->matrizProfessorHorario[a2->aula->disciplina->professor->numeroSequencial][a2->horario->horario] = a2;
//		a2->aula->disciplina->professor->restricaoHorario[a2->horario->horario] = a2;

	ind->Alocacao_dias_utilizados[aula1][a2->horario->dia]++;
	ind->Alocacao_salas_utilizadas[aula1][a2->sala->numeroSequencial]++;
	ind->Alocacao_dias_utilizados[aula2][a1->horario->dia]++;
	ind->Alocacao_salas_utilizadas[aula2][a1->sala->numeroSequencial]++;
	for( itCurr = a1->aula->disciplina->curriculos.begin(); itCurr!=a1->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario]++;
		if( ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] == NULL )
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] = a1;
	}
	for( itCurr = a2->aula->disciplina->curriculos.begin(); itCurr!=a2->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a2->horario->horario]++;
		if( ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a2->horario->dia][a2->horario->periodo] == NULL )
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a2->horario->dia][a2->horario->periodo] = a2;
	}
}
