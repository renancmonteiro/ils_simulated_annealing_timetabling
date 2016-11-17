/*
 * Problema.cpp
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#include "Problema.h"

//#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Restricoes/RestricaoFraca2MinimumWorkingDays.h"
#include "../Restricoes/RestricaoFraca4RoomStability.h"

//#include <iostream>
using namespace std;

void Problema::ImprimeMatrizDisciplinaDisciplina(){
	int i, j;
	for (i = 0; i < nDisciplinas; i++) {
		for (j = 0; j < nDisciplinas; j++) {
//			printf("%d ", MatrizDisciplinaDisciplina[i][j]);
		}
		printf("\n");
	}
}

int Problema::SaoDoMesmoCurriculo(Disciplina* d1, Disciplina* d2){
	int i;
	for( i=0; i<nCurriculos; i++ ){
		if( MatrizDisciplinaCurriculo[d1->numeroSequencial][i] && MatrizDisciplinaCurriculo[d2->numeroSequencial][i]){
			return 1;
		}
	}
	return 0;
}
int Problema::SaoDoMesmoProfessor(Disciplina* d1, Disciplina* d2){
	if( d1->professor->numeroSequencial == d2->professor->numeroSequencial ) return 1;
	return 0;
}
bool Problema::PertenceAoCurriculo(Disciplina* d, Curriculo* c){
	return MatrizDisciplinaCurriculo[d->numeroSequencial][c->numeroSequencial];
}


int Problema::CalculaFuncaoObjetivoSoft(Individuo* ind){
	double violacoesSoft = 0;
	list<Restricao*>::iterator it;

	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if ( (*it)->Hard == 0 )
			violacoesSoft += (*it)->contaViolacaoRestricao(ind, this);
	}
	return violacoesSoft;
}
int Problema::CalculaFuncaoObjetivo(Individuo* ind){
	double violacoesHard = indisponibilidades->contaViolacaoRestricao(ind, this);
	double violacoesSoft = 0;
	list<Restricao*>::iterator it;

	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if ( (*it)->Hard == 0 )
			violacoesSoft += (*it)->contaViolacaoRestricao(ind, this);
		else
			violacoesHard += (*it)->contaViolacaoRestricao(ind, this);
	}

	ind->hard = violacoesHard;
	ind->fitness = violacoesHard + violacoesSoft;
	return violacoesHard + violacoesSoft;
}
int Problema::CalculaFuncaoObjetivo(Individuo* ind, bool gravaIndividuo){
	int violacoesHard = indisponibilidades->contaViolacaoRestricao(ind, this);
	int violacoesSoft = 0;
	list<Restricao*>::iterator it;

	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if ( (*it)->Hard == 0 )
			violacoesSoft += (*it)->contaViolacaoRestricao(ind, this);
		else
			violacoesHard += (*it)->contaViolacaoRestricao(ind, this);
	}

	if( gravaIndividuo ){
		ind->hard = violacoesHard;
		ind->fitness = violacoesHard + violacoesSoft;
	}
	return violacoesHard + violacoesSoft;
}

int Problema::CalculaCustoAulaAlocada(Individuo* ind, Alocacao* a){
//	int violacoes = indisponibilidades->contaViolacaoRestricaoAlocacao(ind, this, a, NULL);
	int violacoes = 0;
	list<Restricao*>::iterator it;
	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if( (*it)->Hard == 0 ) //Se for soft:
			violacoes += (*it)->contaViolacaoRestricaoAlocacao(ind, this, a, NULL);
	}
	return violacoes;
}
int Problema::CalculaCustoAulaAlocada(Individuo* ind, Alocacao* a,Movimento* m){
	int violacoes = 0;
	list<Restricao*>::iterator it;
	if( a->aula != NULL ){
//		violacoes = indisponibilidades->contaViolacaoRestricaoAlocacao(ind, this, a, m);
		for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
			if( (*it)->Hard == 0 ) //Se for soft:
				violacoes += (*it)->contaViolacaoRestricaoAlocacao(ind, this, a, m);
		}
	}
	return violacoes;
}

int Problema::CalculaCustoAulasAlocadas(Individuo* ind, list<Alocacao*> a, Movimento* m){
	int violacoes = 0;
	list<Restricao*>::iterator it;

	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if( (*it)->Hard == 0 ) //Se for soft:
			violacoes += (*it)->contaViolacaoRestricaoListaAlocacao(ind, this, a, m);
	}
	return violacoes;
}

int Problema::calculaCustoNovaAlocacao(Individuo* ind, Alocacao* aloc){
	list<Curriculo*>::iterator itCurr;
	Disciplina* d = aloc->aula->disciplina;
	int custoAlocacao;

	ind->Alocacao_dias_utilizados[d->numeroSequencial][aloc->horario->dia]++;
	ind->Alocacao_salas_utilizadas[d->numeroSequencial][aloc->sala->numeroSequencial]++;
	ind->matrizProfessorHorario[d->professor->numeroSequencial][aloc->horario->horario] = aloc;
	for( itCurr = d->curriculos.begin(); itCurr!=d->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][aloc->horario->horario]++;
		if( ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] == NULL )
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] = aloc;
	}

	custoAlocacao = CalculaCustoAulaAlocada(ind, aloc);

	ind->Alocacao_dias_utilizados[d->numeroSequencial][aloc->horario->dia]--;
	ind->Alocacao_salas_utilizadas[d->numeroSequencial][aloc->sala->numeroSequencial]--;
	ind->matrizProfessorHorario[d->professor->numeroSequencial][aloc->horario->horario] = NULL;
	for( itCurr = d->curriculos.begin(); itCurr!=d->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][aloc->horario->horario]--;
		if( ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] == aloc )
			ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] = NULL;
	}

	return custoAlocacao;
}

int Problema::violaRestricaoGrave(Individuo* ind, Movimento* m){
	int violacoes = 0;
	list<Restricao*>::iterator it;
	violacoes += indisponibilidades->restricaoViolada(ind, this, m);
	for( it=restricoes.begin(); it!=restricoes.end(); it++ ){
		if ( ((*it)->Hard)!=0 ){
			violacoes += (*it)->restricaoViolada(ind, this, m);
		}
	}
	return violacoes;
}

Disciplina* Problema::ObtemDisciplinaPorNome(char* nome){
	Disciplina* d;
	list<Disciplina*>::iterator it;
	for(it = disciplinas.begin(); it != disciplinas.end(); it++){
		d = (*it);
		if( strcmp(d->nome, nome) == 0 ) return d;
	}
	return NULL;
}
Professor* Problema::obtemProfessorPorNome(char* nome){
	Professor* p;
	list<Professor*>::iterator it;
	for(it = professores.begin(); it != professores.end(); it++){
		p = (*it);
		if( strcmp(p->nome, nome) == 0 ) return p;
	}
	return NULL;
}

bool Problema::HorarioIndisponivelDisciplina(Disciplina* d, int horario){
	return indisponibilidades->EhIndisponivel(d->numeroSequencial, horario);
}

Problema::Problema(char *nomeArquivo) {
	    FILE *f;
	    int i, j;
	    Disciplina* d;
	    char nome[30], professor[30];
	    int nAulas, minDiasAula, nAlunos;
	    Sala* s;
	    Curriculo* c;
	    Professor* p;
	    int nDiscCurr;
	    int dia, periodo;
		list<Aula*>::iterator iter;
		list<Disciplina*>::iterator itDisc1;
		list<Disciplina*>::iterator itDisc2;
		list<Curriculo*>::iterator itCurr1;

		// abre arquivo da instancia para leitura dos dados
	    f = fopen(nomeArquivo, "r");
	    if (f == NULL) {
	        printf("Erro de abertura do arquivo %s\n", nomeArquivo);
	        exit(1);
	    }

	    // dados básicos do cabeçalho
	    fscanf(f, "Name: %s\n", this->nome);
	    fscanf(f, "Courses: %d\n", &(nDisciplinas));
	    fscanf(f, "Rooms: %d\n", &(nSalas));
	    fscanf(f, "Days: %d\n", &(nDias));
	    fscanf(f, "Periods_per_day: %d\n", &(nPerDias));
	    fscanf(f, "Curricula: %d\n", &(nCurriculos));
	    fscanf(f, "Constraints: %d\n", &(nRestricoes));

    	MatrizDisciplinaCurriculo  = (int**) malloc ( nDisciplinas * sizeof(int*));
	    for (i = 0; i < nDisciplinas; i++) {
	    	MatrizDisciplinaCurriculo[i]  = (int*) malloc ( nCurriculos * sizeof(int));
		    for (j = 0; j < nCurriculos; j++) {
	        	MatrizDisciplinaCurriculo[i][j] = 0;
	        }
	    }

	    nHorarios = nDias * nPerDias;
	    //Cria Horarios
	    for (i = 0; i < nDias; i++) {
	    	for (j = 0; j < nPerDias; j++) {
	    		horarios.push_back(new Timeslot(i, j, nPerDias));
	    	}
	    }

	    fscanf(f, "\nCOURSES:\n");
		nAulasTotal = 0;
	    for (i = 0; i < nDisciplinas; i++) {
	        fscanf(f, "%s %s %d %d %d\n", nome, professor, &nAulas, &minDiasAula, &nAlunos);
	        p = obtemProfessorPorNome(professor);
	        if( p == NULL ){
	        	p = new Professor(nHorarios, nSalas,(int) professores.size(), professor);
	        	professores.push_back(p);
	        }
	        d = new Disciplina(nome, p, i, nAulas, minDiasAula, nAlunos, 0, nAulasTotal);
	        nAulasTotal+=nAulas;
	        strcpy(d->nome, nome);
	    	disciplinas.push_back(d);
	    }

	    fscanf(f, "\nROOMS:\n");
	    for (i = 0; i < nSalas; i++) {
	    	s = new Sala(i);
	        fscanf(f, "%s %d\n", s->idSala, &(s->capacidade));
	    	salas.push_back(s);
	    }

	    fscanf(f, "\nCURRICULA:\n");
	    for (i = 0; i < nCurriculos; i++) {
	        fscanf(f, "%s %d", nome, &nDiscCurr);
	        c = new Curriculo(nome, i, nDiscCurr);
	        curriculos.push_back(c);

	        for (j = 0; j < nDiscCurr; j++) {
	            fscanf(f, "%s ", nome);
	            d = this->ObtemDisciplinaPorNome(nome);
	            d->curriculos.push_back(c);
	            d->nCurriculos++;
				MatrizDisciplinaCurriculo[d->numeroSequencial][c->numeroSequencial] = true;
		        c->disciplinas.push_back(d);
	        }
	    }

	    fscanf(f, "\nUNAVAILABILITY_CONSTRAINTS:\n");
	    indisponibilidades = new RestricaoForte3Indisponibilidade(nDisciplinas, nHorarios);
	    for (i = 0; i < nRestricoes; i++) {
	        fscanf(f, "%s %d %d\n", nome,  &dia, &periodo);
            d = this->ObtemDisciplinaPorNome(nome);
            if( d == NULL ){
            	fprintf(stderr, "Erro ao obter a disciplina %s\n", nome);
            	exit(0);
            }
			for ( iter = d->aulas.begin( ) ; iter != d->aulas.end( ) ; iter++ ){
				indisponibilidades->AdicionaIndisponibilidade(d->numeroSequencial, dia*nPerDias+periodo);
			}
	    }

	    fclose(f);

	    restricoes.push_back(new RestricaoForte4AulasConflitantes);
	    restricoes.push_back(new RestricaoFraca1RoomCapacity);
	    restricoes.push_back(new RestricaoFraca2MinimumWorkingDays);
	    restricoes.push_back(new RestricaoFraca3CurriculumCompactness);
	    restricoes.push_back(new RestricaoFraca4RoomStability);
}

void Problema::ImprimeProblema(){
	list<Disciplina*>::iterator IterDisc;
	list<Curriculo*>::iterator IterCurr;
	list<Sala*>::iterator IterSala;
	list<Timeslot*>::iterator IterHor;

	printf("Problema: %s", nome);
	printf(" %d Dias * %d PerDias = %d horarios\n", nDias, nPerDias, nHorarios);
	for ( IterHor = horarios.begin( ) ; IterHor != horarios.end( ) ; IterHor++ )
		(*IterHor)->imprime();

	printf("Disciplinas (%d):\n", nDisciplinas);
	for ( IterDisc = disciplinas.begin( ) ; IterDisc != disciplinas.end( ) ; IterDisc++ )
		printf("%2d: %s - %s\t%d Alunos\t%d Dias minimos\t%d Aulas\n", (*IterDisc)->numeroSequencial, (*IterDisc)->nome, (*IterDisc)->professor->nome, (*IterDisc)->nAlunos, (*IterDisc)->minDiasSemana, (*IterDisc)->nAulas);

	printf("Curriculos (%d):\n", nCurriculos);
	for ( IterCurr = curriculos.begin( ) ; IterCurr != curriculos.end( ) ; IterCurr++ )
		printf("%s\n", (*IterCurr)->nome);
	printf("Restricoes (%d):\n", nRestricoes);
    indisponibilidades->ImprimeIndisponibilidades();
    printf("\n");
    printf("Salas (%d):\n", nSalas);
	for ( IterSala = salas.begin() ; IterSala != salas.end( ) ; IterSala++ )
		printf("%s(%d)\t", (*IterSala)->idSala, (*IterSala)->capacidade);
	printf("\n");

}


Problema::~Problema() {
	Disciplina* d;
	Sala* s;
	Curriculo* c;
	Timeslot* t;
	Restricao* r;
	Professor* p;
	int i;

	while( disciplinas.size() > 0 ) {
		d = disciplinas.back();
		disciplinas.pop_back();
		delete(d);
	}
	while( salas.size() > 0 ) {
		s = salas.back();
		salas.pop_back();
		delete(s);
	}
	while( curriculos.size() > 0 ) {
		c = curriculos.back();
		curriculos.pop_back();
		delete(c);
	}
	while( horarios.size() > 0 ) {
		t = horarios.back();
		horarios.pop_back();
		delete(t);
	}
	while( restricoes.size() > 0 ) {
		r = restricoes.back();
		restricoes.pop_back();
		delete(r);
	}
	while( professores.size() > 0 ) {
		p = professores.back();
		professores.pop_back();
		delete(p);
	}
	delete(indisponibilidades);

    for (i = 0; i < nDisciplinas; i++) free(MatrizDisciplinaCurriculo[i]);
    free(MatrizDisciplinaCurriculo);
}
