/*
 * TimeMove.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: renan
 */

#include "TimeMove.h"
#include "Move.h"
#include "RoomMove.h"
#include "../Model/Alocacao.h"

TimeMove::TimeMove(Individuo* piInd, Alocacao* al1, Alocacao* al2){
	tipoMovimento = 3;
	list<Alocacao*>::iterator it;
	ind = piInd;
	m = NULL;
	a1 = al2;

	bool escolheu = false;
	/* Seleciona o primeiro TimeSlot disponível ascendente */
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++)
	{
		if((*(it))->sala->numeroSequencial == al1->sala->numeroSequencial && (*(it))->horario->horario > a1->horario->horario)
		{
			a2 = *it;
			escolheu = true;
			break;
		}
	}

	if(!escolheu)
	{
		/* Seleciona o primeiro TimeSlot disponível descendente */
		list<Alocacao*>::reverse_iterator itr;
		for(itr = ind->horariosVazios.rbegin(); itr != ind->horariosVazios.rend(); itr++)
		{
			if((*(itr))->sala->numeroSequencial == al1->sala->numeroSequencial && (*(itr))->horario->horario < a1->horario->horario)
			{
				a2 = *itr;
				escolheu = true;
				break;
			}
		}
	}

	if(a2 != NULL && escolheu){
		a1 = al1;
		deltaFit = calculaDeltaFitTimeMove(piInd->p);
	} else {
		m = new RoomMove(ind, al1, al2);
		deltaFit = m->deltaFit;
	}
}

TimeMove::TimeMove(Problema* p, Individuo* piInd)
{
	tipoMovimento = 3;
	ind = piInd;
	m = NULL;
	/* Escolhe randomicamente uma aula alocada */
	int posAulaAlocada = rand() % ind->aulasAlocadas.size();

	/* Cria o iterator pra lista de aulas alocadas */
	list<Alocacao*>::iterator it = ind->aulasAlocadas.begin();
	/* Avança até a posição escolhida */
	advance(it, posAulaAlocada);
	a1 = *(it);

	bool escolheu = false;

	/* Seleciona o primeiro TimeSlot disponível ascendente */
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++)
	{
		if((*(it))->sala->numeroSequencial == a1->sala->numeroSequencial && (*(it))->horario->horario > a1->horario->horario)
		{
			a2 = *it;
			escolheu = true;
			break;
		}
	}

	if(!escolheu)
	{
		/* Seleciona o primeiro TimeSlot disponível descendente */
		list<Alocacao*>::reverse_iterator itr;
		for(itr = ind->horariosVazios.rbegin(); itr != ind->horariosVazios.rend(); itr++)
		{
			if((*(itr))->sala->numeroSequencial == a1->sala->numeroSequencial && (*(itr))->horario->horario < a1->horario->horario)
			{
				a2 = *itr;
				escolheu = true;
				break;
			}
		}
	}

	if(a2 != NULL && escolheu){
		deltaFit = calculaDeltaFitTimeMove(p);
	} else {
		deltaFit = 99999; deltaHard  = 99999;
	}
}

void TimeMove::aplicaMovimento()
{

	if(m == NULL){
	/* Remove A1 da lista de aulas alocadas */
	ind->aulasAlocadas.remove(a1);

	/* Remove a2 da lista de alocações vazias */
	ind->horariosVazios.remove(a2);

	aplicaTimeMoveSemRecalculoFuncaoObjetivo();

	ind->fitness += deltaFit;
	ind->hard  += deltaHard;
	ind->soft1 += deltaSoft1;
	ind->soft2 += deltaSoft2;
	ind->soft3 += deltaSoft3;
	ind->soft4 += deltaSoft4;

	/* Insere A1 na lista de aulas alocadas */
	ind->aulasAlocadas.push_back(a1);

	/* Insere A2 na lista de alocações vazias, de modo ordenado */
	list<Alocacao*>::iterator it;
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end() && (*it)->sala->numeroSequencial <= a2->sala->numeroSequencial; it++) {
		if ( (*it)->sala->numeroSequencial == a2->sala->numeroSequencial && (*it)->horario->horario > a2->horario->horario ) {
			break;
		}
	}
	ind->horariosVazios.insert (it, a2);
	} else {
		m->aplicaMovimento();
	}
}

int TimeMove::calculaDeltaFitTimeMove(Problema* p)
{
	list<Restricao*>::iterator it;
	int deltaFitness;
	int violaRestricaoHard;

	deltaFitness  = -p->CalculaCustoAulaAlocada(ind, a1, this);

	deltaHard  = 0;
	deltaSoft1 = -deltaSoft1;
	deltaSoft2 = -deltaSoft2;
	deltaSoft3 = -deltaSoft3;
	deltaSoft4 = -deltaSoft4;
	deltaFitness = deltaSoft1 + deltaSoft2 + deltaSoft3 + deltaSoft4;

	aplicaTimeMoveSemRecalculoFuncaoObjetivo();
	violaRestricaoHard = p->violaRestricaoGrave(ind, this);
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulaAlocada(ind, a1, this);
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}
	aplicaTimeMoveSemRecalculoFuncaoObjetivo();

	return deltaFitness;
}


void TimeMove::aplicaTimeMoveSemRecalculoFuncaoObjetivo(){
	list<Curriculo*>::iterator itCurr;
	Timeslot* temp_hora;
	int aula1;

	aula1 = a1->aula->disciplina->numeroSequencial;
	ind->Alocacao_dias_utilizados[aula1][a1->horario->dia]--;
	ind->Alocacao_salas_utilizadas[aula1][a1->sala->numeroSequencial]--;
	for( itCurr = a1->aula->disciplina->curriculos.begin(); itCurr!=a1->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario]--;
		if( ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario] == 0)
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] = NULL;
	}
	ind->matrizProfessorHorarioQntd[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario]--;
	if( ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] == a1)
		ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] = NULL;
	//		a1->aula->disciplina->professor->restricaoHorario[a1->horario->horario] = NULL;

	temp_hora = a1->horario;
	a1->horario = a2->horario;
	a2->horario = temp_hora;

	ind->matrizProfessorHorarioQntd[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario]++;
	if( ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] == NULL)
		ind->matrizProfessorHorario[a1->aula->disciplina->professor->numeroSequencial][a1->horario->horario] = a1;
	//		a1->aula->disciplina->professor->restricaoHorario[a1->horario->horario] = a1;

	ind->Alocacao_dias_utilizados[aula1][a1->horario->dia]++;
	ind->Alocacao_salas_utilizadas[aula1][a1->sala->numeroSequencial]++;
	for( itCurr = a1->aula->disciplina->curriculos.begin(); itCurr!=a1->aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][a1->horario->horario]++;
		if( ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] == NULL )
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][a1->horario->dia][a1->horario->periodo] = a1;
	}
}
