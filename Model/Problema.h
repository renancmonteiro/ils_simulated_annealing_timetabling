/*
 * Problema.h
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#ifndef PROBLEMA_H_
#define PROBLEMA_H_

#include <list>

#include "Sala.h"
#include "Aula.h"
#include "Disciplina.h"
#include "Timeslot.h"
#include "Curriculo.h"
#include "Professor.h"
#include "../Restricoes/RestricaoForte3Indisponibilidade.h"
#include "../Restricoes/RestricaoForte4AulasConflitantes.h"
#include "../Restricoes/RestricaoFraca1RoomCapacity.h"
#include "../Restricoes/RestricaoFraca2MinimumWorkingDays.h"
#include "../Restricoes/RestricaoFraca3CurriculumCompactness.h"
#include "../Restricoes/RestricaoFraca4RoomStability.h"
#include "Individuo.h"
class Movimento;

class Problema {
public:
	std::list<Disciplina*> disciplinas;
	std::list<Curriculo*> curriculos;
	std::list<Timeslot*> horarios;
	std::list<Sala*> salas;
	RestricaoForte3Indisponibilidade* indisponibilidades;
	std::list<Restricao*> restricoes;
	std::list<Professor*> professores;

	int** MatrizDisciplinaCurriculo;

	char nome[30];
	int nDisciplinas;
	int nCurriculos;
	int nDias, nPerDias, nHorarios;
	int nSalas;
	int nAulasTotal;

//	int pesoHard;
	int nRestricoes;

	Problema(char* nomeArquivo);

	Disciplina* ObtemDisciplinaPorNome(char* nome);
	int SaoDoMesmoCurriculo(Disciplina* d1, Disciplina* d2);
	int SaoDoMesmoProfessor(Disciplina* d1, Disciplina* d2);
	bool PertenceAoCurriculo(Disciplina* d, Curriculo* c);
	int CalculaFuncaoObjetivoSoft(Individuo* ind);
	int CalculaFuncaoObjetivo(Individuo* ind);
	int CalculaFuncaoObjetivo(Individuo* ind, bool gravaIndividuo);
	int CalculaCustoAulaAlocada(Individuo* ind, Alocacao* a, Movimento* m);
	int CalculaCustoAulaAlocada(Individuo* ind, Alocacao* a);
	int CalculaCustoAulasAlocadas(Individuo* ind, list<Alocacao*> a, Movimento* m);
	int calculaCustoNovaAlocacao(Individuo* ind, Alocacao* aloc);
	void ImprimeProblema();
	void ImprimeMatrizDisciplinaDisciplina();
	bool HorarioIndisponivelDisciplina(Disciplina* d, int horario);
	Professor* obtemProfessorPorNome(char* nome);
	int violaRestricaoGrave(Individuo* ind, Movimento* m);

	virtual ~Problema();
};

#endif /* PROBLEMA_H_ */
