/*
 * RoomMove2.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: renan
 */

#include <iostream>
#include "RoomMove2.h"
#include "../Model/Alocacao.h"
#include <string.h>

using namespace std;

void RoomMove2::imprimeListaAlocada(){
	list<Alocacao*>::iterator itAulasAlocadas;

	printf("Imprimindo a Lista de Aulas Alocadas \n");
	printf("Tamaho da lista: %d\n", int(ind->aulasAlocadas.size()));
	for(itAulasAlocadas = ind->aulasAlocadas.begin(); itAulasAlocadas != ind->aulasAlocadas.end(); itAulasAlocadas++)
	{
		a2 = *itAulasAlocadas;
		a2->imprime();
	}
	printf("\n");
}

void RoomMove2::imprimeListaVazia(){
	list<Alocacao*>::iterator itAulasVazias;

	printf("Imprimindo a Lista de Aloc. Vazias \n");
	printf("Tamaho da lista: %d\n", int(ind->horariosVazios.size()));
	for(itAulasVazias = ind->horariosVazios.begin(); itAulasVazias != ind->horariosVazios.end(); itAulasVazias++)
	{
		a2 = *itAulasVazias;
		a2->imprime();
	}
	printf("\n");
}


RoomMove2::RoomMove2(Problema* p, Individuo* piInd)
{
	ind = piInd;

	/* Escolhe randomicamente uma aula alocada */
	int posAulaAlocada = rand() % ind->aulasAlocadas.size();

	/* Cria o iterator pra lista de aulas alocadas */
	list<Alocacao*>::iterator it = ind->aulasAlocadas.begin();

	/* Avança até a posição escolhida */
	advance(it, posAulaAlocada);
	a1 = *(it);

	list<Alocacao*> possiveisTimeslots;

	/* Escolhe uma possível sala ascendente */
	for(it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++)
	{
		if((*(it))->horario->horario == a1->horario->horario && (*(it))->sala->numeroSequencial > a1->sala->numeroSequencial)
		{
			possiveisTimeslots.push_back(*it);
		}
	}

	if(possiveisTimeslots.size() == 0)
	{
		/* Seleciona a primeira sala disponível descendente */
		list<Alocacao*>::reverse_iterator itr;
		for(itr = ind->horariosVazios.rbegin(); itr != ind->horariosVazios.rend(); itr++)
		{
			if((*(itr))->horario->horario == a1->horario->horario && (*(itr))->sala->numeroSequencial < a1->sala->numeroSequencial)
			{
				possiveisTimeslots.push_back(*itr);
			}
		}
	}

	if(possiveisTimeslots.size() > 0){
		/* Escolhe randomicamente um timeslot possivel */
		int posAulaPossivel = rand() % possiveisTimeslots.size();
		/* iterator pra lista de possiveis timeslots */
		it = possiveisTimeslots.begin();
		/* Avança até a posição escolhida */
		advance(it, posAulaPossivel);
		a2 = *(it);
		deltaFit = calculaDeltaFitRoomMove2(p);
	} else {
		deltaFit = 99999; deltaHard  = 99999;
	}
}

void RoomMove2::aplicaMovimento()
{
	/* Remove A1 da lista de aulas alocadas */
	ind->aulasAlocadas.remove(a1);

	/* Remove a2 da lista de alocações vazias */
	ind->horariosVazios.remove(a2);

	aplicaRoomMove2SemRecalculoFuncaoObjetivo();

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


int RoomMove2::calculaDeltaFitRoomMove2(Problema* p)
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

	aplicaRoomMove2SemRecalculoFuncaoObjetivo();
	violaRestricaoHard = p->violaRestricaoGrave(ind, this);
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulaAlocada(ind, a1, this);
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}
	aplicaRoomMove2SemRecalculoFuncaoObjetivo();

	return deltaFitness;
}


void RoomMove2::aplicaRoomMove2SemRecalculoFuncaoObjetivo(){
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
