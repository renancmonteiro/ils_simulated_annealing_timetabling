/*
 * RoomMove.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: renan
 */

#include <iostream>
#include "RoomMove.h"
#include "Move.h"
#include "../Model/Alocacao.h"
#include <string.h>

RoomMove::RoomMove(Individuo* piInd, Alocacao* al1, Alocacao* al2){
	list<Alocacao*>::iterator it;

	ind = piInd;
	a1 = al2;

	bool escolheu = false;

	/* Escolhe uma possível sala ascendente */
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++)
	{
		if((*(it))->horario->horario == al1->horario->horario && (*(it))->sala->numeroSequencial > a1->sala->numeroSequencial)
		{
			a2 = *(it);
			escolheu = true;
			break;
		}
	}

	if(!escolheu)
	{
		/* Seleciona a primeira sala disponível descendente */
		list<Alocacao*>::reverse_iterator itr;
		for(itr = ind->horariosVazios.rbegin(); itr != ind->horariosVazios.rend(); itr++)
		{
			if((*(itr))->horario->horario == al1->horario->horario && (*(itr))->sala->numeroSequencial < a1->sala->numeroSequencial)
			{
				a2 = *itr;
				escolheu = true;
				break;
			}
		}
	}

	if(a2 != NULL && escolheu)
	{
		a1 = al1;
		deltaFit = calculaDeltaFitRoomMove(piInd->p);
	} else {
		deltaFit = 99999; deltaHard  = 99999;
	}
}

RoomMove::RoomMove(Problema* p, Individuo* piInd)
{
	ind = piInd;

	/* Escolhe randomicamente uma aula alocada */
	int posAulaAlocada = rand() % ind->aulasAlocadas.size();

	/* Cria o iterator pra lista de aulas alocadas */
	list<Alocacao*>::iterator it = ind->aulasAlocadas.begin();

	/* Avança até a posição escolhida */
	advance(it, posAulaAlocada);
	a1 = *(it);

	bool escolheu = false;
	a2 = NULL;

	/* Escolhe uma possível sala ascendente */
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++)
	{
		if((*(it))->horario->horario == a1->horario->horario && (*(it))->sala->numeroSequencial > a1->sala->numeroSequencial)
		{
			a2 = *(it);
			escolheu = true;
			break;
		}
	}

	if(!escolheu)
	{
		/* Seleciona a primeira sala disponível descendente */
		list<Alocacao*>::reverse_iterator itr;
		for(itr = ind->horariosVazios.rbegin(); itr != ind->horariosVazios.rend(); itr++)
		{
			if((*(itr))->horario->horario == a1->horario->horario && (*(itr))->sala->numeroSequencial < a1->sala->numeroSequencial)
			{
				a2 = *itr;
				escolheu = true;
				break;
			}
		}
	}

	if(a2 != NULL && escolheu)
	{
		deltaFit = calculaDeltaFitRoomMove(p);
	} else {
		deltaFit = 99999; deltaHard  = 99999;
	}
}

void RoomMove::aplicaMovimento()
{
	/* Remove A1 da lista de aulas alocadas */
	ind->aulasAlocadas.remove(a1);

	/* Remove a2 da lista de alocações vazias */
	ind->horariosVazios.remove(a2);

	aplicaRoomMoveSemRecalculoFuncaoObjetivo();

	ind->fitness += deltaFit;
	ind->hard  += deltaHard;
	ind->soft1 += deltaSoft1;
	ind->soft2 += deltaSoft2;
	ind->soft3 += deltaSoft3;
	ind->soft4 += deltaSoft4;

	/* Insere A2 na lista de alocações vazias, de modo ordenado */
	list<Alocacao*>::iterator it;
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end() && (*it)->sala->numeroSequencial <= a2->sala->numeroSequencial; it++) {
		if ( (*it)->sala->numeroSequencial == a2->sala->numeroSequencial && (*it)->horario->horario > a2->horario->horario ) {
			break;
		}
	}
	ind->horariosVazios.insert (it, a2);

	/* Insere A1 na lista de aulas alocadas */
	ind->aulasAlocadas.push_back(a1);
}


int RoomMove::calculaDeltaFitRoomMove(Problema* p)
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

	aplicaRoomMoveSemRecalculoFuncaoObjetivo();
	violaRestricaoHard = p->violaRestricaoGrave(ind, this);
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulaAlocada(ind, a1, this);
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}
	aplicaRoomMoveSemRecalculoFuncaoObjetivo();

	return deltaFitness;
}


void RoomMove::aplicaRoomMoveSemRecalculoFuncaoObjetivo(){
	list<Curriculo*>::iterator itCurr;
	Sala* temp_sala;
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

	temp_sala = a1->sala;
	a1->sala = a2->sala;
	a2->sala = temp_sala;

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
