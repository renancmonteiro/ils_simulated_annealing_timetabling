/*
 * Individuo.h
 *
 *  Created on: May 17, 2015
 *      Author: erika
 */

#ifndef INDIVIDUO_H_
#define INDIVIDUO_H_

#include <list>
#include <stdio.h>

#include "Alocacao.h"
class Problema;

using namespace std;

class Individuo {
public:
	list<Alocacao*> aulasAlocadas;
	list<Alocacao*> horariosVazios;
	list<Alocacao*> TodosHorarios;
	int fitness;
	int hard;
	int soft1, soft2, soft3, soft4;

	Problema* p;
	int** Alocacao_salas_utilizadas;
	int** Alocacao_dias_utilizados;
	int** Alocacao_horarios_utilizados_por_curriculo;
	Alocacao* ***matrizAlocacaoCurriculoDiasPeriodos;
	Alocacao* **matrizProfessorHorario;
	int **matrizProfessorHorarioQntd;

	Individuo(Problema* p);
	Individuo(Individuo* i);
	void LeDoArquivo(char* nomeArquivo);
	void ImprimeIndividuo();
	void ImprimeInstancia(char* fileName);
	~Individuo();

	Alocacao* ObtemAlocacao(int horario, int numSala);

	void ImprimeMatrizHorarioPorDisciplinas();
	void ImprimeMatrizProfessorHorario();
	void ImprimeMatrizProfessorHorarioQnt();
	void ImprimeAlocacao_horarios_utilizados_por_curriculo();
	void ImprimeMatrizAlocacaoCurriculoDiasPeriodos();
	void ImprimeMatrizAlocacaoSalasUtilizadas();
};

#endif /* INDIVIDUO_H_ */
