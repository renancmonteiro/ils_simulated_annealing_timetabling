/*
 * GeraSolucacoInicial.h
 *
 *  Created on: May 18, 2015
 *      Author: erika
 */

#ifndef GERASOLUCACOINICIALGULOSA_H_
#define GERASOLUCACOINICIALGULOSA_H_

#include <stdio.h>
#include <stdlib.h>
#include <list>
using namespace std;

#include "Model/Problema.h"

class GeraSolucaoInicialGulosa {
public:
	GeraSolucaoInicialGulosa(Problema* p, float piThreshold);
	void CriaIndividuoInicial(Problema* p, Individuo* ind);
	~GeraSolucaoInicialGulosa();

private:
    class Candidato{
    public:
    	Disciplina* disc;
    	int* horariosViaveis;
    	int nTamVetorHorariosViaveis;
    	int nHorariosViaveis;
    	int nHorariosSalaViaveis;
    	int nAulasParaAlocar;
    	int nDisciplinasComMesmoCurriculoOuProfessor;
    	Candidato(Disciplina* a, Problema* p){
    		disc = a;
    		horariosViaveis = (int*) malloc ( p->nHorarios * sizeof(int) );
    		nHorariosViaveis = 0;
    		nHorariosSalaViaveis = 0;
    		nAulasParaAlocar = 0;
    		nDisciplinasComMesmoCurriculoOuProfessor = 0;
    		nTamVetorHorariosViaveis = p->nHorarios;
    	};
    	~Candidato(){ free(horariosViaveis); };
    	void imprime(){disc->imprime();};
    };
    class Possibilidade{
    public:
    	Aula* aula;
    	Alocacao* horario;
    	int custoAlocacao;
    	int nAulasParaIndisponibilizar;
    	double cEscolha;
    	Possibilidade(Aula* au, Alocacao* aloc, Problema* p, Individuo* ind){
    		aula = au; horario = aloc;
    		aloc->aula = aula;
    		custoAlocacao = p->calculaCustoNovaAlocacao(ind, aloc);
    		aloc->aula = NULL;
    		nAulasParaIndisponibilizar = 0;
    		cEscolha = 0.0;
    	};
    };

	Candidato** candidatos;
	int nCandidatos;
	float threshold;
	double alpha, betha;

	int comparaAulas(Problema* p, Individuo* ind, Candidato* c1, Candidato* c2);
	void ordenaAulasPorDificuldade(Problema* p, Individuo* ind);
	int contaTotalHorariosViaveis(Problema* p, Candidato* cand, Individuo* ind);
	void alocaAula(Problema* p, Individuo* ind, Candidato* cand);

};

#endif /* GERASOLUCACOINICIALGULOSA_H_ */
