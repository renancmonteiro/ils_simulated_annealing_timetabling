/*
 * Individuo.cpp
 *
 *  Created on: May 17, 2015
 *      Author: erika
 */

#include <stdlib.h>
#include <string.h>

#include "Problema.h"
#include "Individuo.h"
#include "Disciplina.h"
#include "Alocacao.h"

Individuo::Individuo(Problema* p) {
	list<Timeslot*>::iterator itHora;
	list<Sala*>::iterator itSala;
	Alocacao* aloc;
	int i, j, k;
	i = 0;

	for( itSala=p->salas.begin(); itSala!=p->salas.end(); itSala++ ) {
		for( itHora=p->horarios.begin(); itHora!=p->horarios.end(); itHora++ ) {
			aloc = new Alocacao(i);
			//printf("Cria alocacao %2d (Sala = %2d, Horario = %2d)\n", i, (*itHora)->horario, (*itSala)->numeroSequencial);
			aloc->aula 	  = NULL;
			aloc->horario = *itHora;
			aloc->sala	  = *itSala;
			horariosVazios.push_back(aloc);
			TodosHorarios.push_back(aloc);
			i++;
		}
	}
	
	this->p = p;
	hard = soft1 = soft2 = soft3 = soft4 = 0;
	fitness = 99999;

	Alocacao_salas_utilizadas = (int**) malloc( p->nDisciplinas * sizeof(int*) );
	Alocacao_dias_utilizados  = (int**) malloc( p->nDisciplinas * sizeof(int*) );
	for( j=0; j<p->nDisciplinas; j++ ) {
		Alocacao_salas_utilizadas[j] = (int*) malloc( p->nSalas * sizeof(int) );
		Alocacao_dias_utilizados[j]  = (int*) malloc( p->nDias * sizeof(int) );
		for( i=0; i<p->nSalas; i++ ) Alocacao_salas_utilizadas[j][i] = 0;
		for( i=0; i<p->nDias; i++ ) Alocacao_dias_utilizados[j][i] = 0;
	}

	Alocacao_horarios_utilizados_por_curriculo = (int**) malloc( p->nCurriculos * sizeof(int*) );
	for( j=0; j<p->nCurriculos; j++ ) {
		Alocacao_horarios_utilizados_por_curriculo[j] = (int*) malloc( p->nHorarios * sizeof(int) );
		for( i=0; i<p->nHorarios; i++ )
			Alocacao_horarios_utilizados_por_curriculo[j][i] = 0;
	}

	matrizAlocacaoCurriculoDiasPeriodos = (Alocacao****) malloc( p->nCurriculos * sizeof(Alocacao***) );
	for( j=0; j<p->nCurriculos; j++ ) {
		matrizAlocacaoCurriculoDiasPeriodos[j] = (Alocacao***) malloc( p->nDias * sizeof(Alocacao**) );
		for( i=0; i<p->nDias; i++ ){
			matrizAlocacaoCurriculoDiasPeriodos[j][i] = (Alocacao**) malloc( p->nPerDias * sizeof(Alocacao*) );
			for( k=0; k<p->nPerDias; k++ ){
				matrizAlocacaoCurriculoDiasPeriodos[j][i][k] = NULL;
			}
		}
	}

	matrizProfessorHorario = (Alocacao***) malloc( ((int) p->professores.size()) * sizeof(Alocacao**) );
	matrizProfessorHorarioQntd = (int**) malloc( ((int) p->professores.size()) * sizeof(int*) );
	for( j=0; j<(int) p->professores.size(); j++ ) {
		matrizProfessorHorario[j] = (Alocacao**) malloc( p->nHorarios * sizeof(Alocacao*) );
		matrizProfessorHorarioQntd[j] = (int*) malloc( p->nHorarios * sizeof(int) );
		for( i=0; i<p->nHorarios; i++ ){
			matrizProfessorHorario[j][i] = NULL;
			matrizProfessorHorarioQntd[j][i] = 0;
		}
	}
}

Individuo::Individuo(Individuo* i) {
	list<Alocacao*>::iterator it;
	Alocacao* copiaAloc;
	int k, j, s;
	for( it=i->TodosHorarios.begin(); it!=i->TodosHorarios.end(); it++ ) {
		copiaAloc = new Alocacao(*it);
		TodosHorarios.push_back( copiaAloc );
	}
	for( it=i->horariosVazios.begin(); it!=i->horariosVazios.end(); it++ ) {
		copiaAloc = this->ObtemAlocacao((*it)->horario->horario, (*it)->sala->numeroSequencial);
		horariosVazios.push_back( copiaAloc );
	}
	for( it=i->aulasAlocadas.begin(); it!=i->aulasAlocadas.end(); it++ ) {
		copiaAloc = this->ObtemAlocacao((*it)->horario->horario, (*it)->sala->numeroSequencial);
		aulasAlocadas.push_back( copiaAloc );
	}
	hard  = i->hard;
	soft1 = i->soft1;
	soft2 = i->soft2;
	soft3 = i->soft3;
	soft4 = i->soft4;
	fitness = i->fitness;
	p = i->p;
	Alocacao_salas_utilizadas = (int**) malloc( p->nDisciplinas * sizeof(int*) );
	Alocacao_dias_utilizados  = (int**) malloc( p->nDisciplinas * sizeof(int*) );
	Alocacao_horarios_utilizados_por_curriculo = (int**) malloc( p->nCurriculos * sizeof(int*) );
	for( j=0; j<p->nDisciplinas; j++ ) {
		Alocacao_salas_utilizadas[j] = (int*) malloc( p->nSalas * sizeof(int) );
		Alocacao_dias_utilizados[j]  = (int*) malloc( p->nDias * sizeof(int) );
		for( k=0; k<p->nSalas; k++ ) Alocacao_salas_utilizadas[j][k] = i->Alocacao_salas_utilizadas[j][k];
		for( k=0; k<p->nDias ; k++ ) Alocacao_dias_utilizados[j][k] = i->Alocacao_dias_utilizados[j][k];
	}
	for( j=0; j<p->nCurriculos; j++ ) {
		Alocacao_horarios_utilizados_por_curriculo[j] = (int*) malloc( p->nHorarios * sizeof(int) );
		for( k=0; k<p->nHorarios; k++ )
			Alocacao_horarios_utilizados_por_curriculo[j][k] = i->Alocacao_horarios_utilizados_por_curriculo[j][k];
	}
	matrizAlocacaoCurriculoDiasPeriodos = (Alocacao****) malloc( p->nCurriculos * sizeof(Alocacao***) );
	for( j=0; j<p->nCurriculos; j++ ) {
		matrizAlocacaoCurriculoDiasPeriodos[j] = (Alocacao***) malloc( p->nDias * sizeof(Alocacao**) );
		for( s=0; s<p->nDias; s++ ){
			matrizAlocacaoCurriculoDiasPeriodos[j][s] = (Alocacao**) malloc( p->nPerDias * sizeof(Alocacao*) );
			for( k=0; k<p->nPerDias; k++ ){
				matrizAlocacaoCurriculoDiasPeriodos[j][s][k] = i->matrizAlocacaoCurriculoDiasPeriodos[j][s][k];
			}
		}
	}
	matrizProfessorHorario = (Alocacao***) malloc( ((int) p->professores.size()) * sizeof(Alocacao**) );
	matrizProfessorHorarioQntd = (int**) malloc( ((int) p->professores.size()) * sizeof(int*) );
		for( j=0; j<(int) p->professores.size(); j++ ) {
			matrizProfessorHorario[j] = (Alocacao**) malloc( p->nHorarios * sizeof(Alocacao*) );
			matrizProfessorHorarioQntd[j] = (int*) malloc( p->nHorarios * sizeof(int) );
			for( s=0; s<p->nHorarios; s++ ){
				if( i->matrizProfessorHorario[j][s] != NULL ){
					copiaAloc = this->ObtemAlocacao(i->matrizProfessorHorario[j][s]->horario->horario, i->matrizProfessorHorario[j][s]->sala->numeroSequencial);
					if( copiaAloc==NULL ) {
						fprintf(stderr, "Individuo.cpp: Erro ao obter a alocacao %d %d\n\n", i->matrizProfessorHorario[j][s]->horario->horario, i->matrizProfessorHorario[j][s]->sala->numeroSequencial);
						exit(0);
					}
				}
				else copiaAloc = NULL;
				matrizProfessorHorario[j][s] = copiaAloc;
				matrizProfessorHorarioQntd[j][s] = i->matrizProfessorHorarioQntd[j][s];
			}
		}
}


void Individuo::LeDoArquivo(char* nomeArquivo){
	int dia, periodo;
	char nomeDisciplina[30];
	char nomeSala[30];
	FILE* arq = fopen(nomeArquivo, "r");

	list<Alocacao*>::iterator itAloc;
	list<Curriculo*>::iterator itCurr;
	list<Aula*>::iterator itAula;
	Alocacao* aloc;
	Disciplina* d = NULL, *dAnterior = NULL;
	Aula* aula;

	while( fscanf(arq, "%s %s %d %d\n", nomeDisciplina, nomeSala, &dia, &periodo) ){
//		printf("%s %s %d %d\n", nomeDisciplina, nomeSala, dia, periodo);

		if( d == NULL || strcmp(d->nome, nomeDisciplina)!=0 ){
			d = p->ObtemDisciplinaPorNome(nomeDisciplina);
			if( d==dAnterior ) break;
			itAula = d->aulas.begin();
			aula = *itAula;
			itAula++;
		}else{
			aula = *itAula;
			itAula++;
			if( itAula==d->aulas.end() ) { dAnterior = d; d = NULL; }
		}

		for( itAloc=horariosVazios.begin(); itAloc!=horariosVazios.end(); itAloc++ ) {
			if( strcmp((*itAloc)->sala->idSala,nomeSala)==0 && (*itAloc)->horario->dia==dia && (*itAloc)->horario->periodo==periodo ){
				aloc = *itAloc;
				aloc->aula = aula;
				aulasAlocadas.push_back(aloc);
				horariosVazios.remove(aloc);
				Alocacao_dias_utilizados[aloc->aula->disciplina->numeroSequencial][aloc->horario->dia]++;
				Alocacao_salas_utilizadas[aloc->aula->disciplina->numeroSequencial][aloc->sala->numeroSequencial]++;
				matrizProfessorHorarioQntd[aloc->aula->disciplina->professor->numeroSequencial][aloc->horario->horario]++;
				matrizProfessorHorario[aloc->aula->disciplina->professor->numeroSequencial][aloc->horario->horario] = aloc;
//				aula->disciplina->professor->restricaoHorario[aloc->horario->horario] = aloc;
				for( itCurr = aula->disciplina->curriculos.begin(); itCurr!=aula->disciplina->curriculos.end(); itCurr++){
					Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][aloc->horario->horario]++;
					matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] = aloc;
				}
				break;
			}
		}
	}

	p->CalculaFuncaoObjetivo(this);
	fclose(arq);
}

void Individuo::ImprimeMatrizProfessorHorario(){
	int j, i;
	for( j=0; j<(int) p->professores.size(); j++ ) {
		for( i=0; i<p->nHorarios; i++ ){
			if( matrizProfessorHorario[j][i] != NULL ) fprintf(stderr, "%s ", matrizProfessorHorario[j][i]->aula->disciplina->nome);
			else fprintf(stderr, "----- ");
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

void Individuo::ImprimeMatrizAlocacaoSalasUtilizadas(){
	int j, i;
	for( j=0; j<p->nDisciplinas; j++ ) {
		for( i=0; i<p->nSalas; i++ ) {
			fprintf(stderr, "%d ", Alocacao_salas_utilizadas[j][i]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

void Individuo::ImprimeMatrizProfessorHorarioQnt(){
	int j, i;
	for( j=0; j<(int) p->professores.size(); j++ ) {
		for( i=0; i<p->nHorarios; i++ ){
			fprintf(stderr, "%d ", matrizProfessorHorarioQntd[j][i]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

void Individuo::ImprimeMatrizHorarioPorDisciplinas(){
	int j, i, k;
	for( j=0; j<p->nCurriculos; j++ ) {
		for( i=0; i<p->nDias; i++ ) {
			for( k=0; k<p->nPerDias; k++ ) {
				if( matrizAlocacaoCurriculoDiasPeriodos[j][i][k] != NULL ) fprintf(stderr, "%3d ", matrizAlocacaoCurriculoDiasPeriodos[j][i][k]->aula->disciplina->numeroSequencial);
				else fprintf(stderr, "--- ");
			}
			fprintf(stderr, " | ");
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}


void Individuo::ImprimeAlocacao_horarios_utilizados_por_curriculo(){
	int j, k;
	for( j=0; j<p->nCurriculos; j++ ) {
		for( k=0; k<p->nHorarios; k++ ) fprintf(stderr, "%3d ", Alocacao_horarios_utilizados_por_curriculo[j][k]);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}
void Individuo::ImprimeMatrizAlocacaoCurriculoDiasPeriodos(){
	int j, i, k;
	for( j=0; j<p->nCurriculos; j++ ) {
		for( i=0; i<p->nDias; i++ ) {
			for( k=0; k<p->nPerDias; k++ ) {
				if( matrizAlocacaoCurriculoDiasPeriodos[j][i][k] != NULL ) fprintf(stderr, "%s ", matrizAlocacaoCurriculoDiasPeriodos[j][i][k]->aula->disciplina->nome);
				else fprintf(stderr, "----- ");
			}
			fprintf(stderr, " | ");
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

void Individuo::ImprimeIndividuo(){
	list<Alocacao*>::iterator it;

	printf("\nIndividuo: ");
	printf("fitness: %d\n", fitness);
	printf("hard: %3d\tsoft1: %3d\tsoft2: %3d\tsoft3: %3d\tsoft4: %3d\n", hard, soft1, soft2, soft3, soft4);

	//checar possiveis erros
	for(it = aulasAlocadas.begin(); it != aulasAlocadas.end(); it++){
		if( (*it)->aula == NULL  ){
			printf("Aula alocada eh nula\n");
			exit(0);
		}
	}
	for(it = horariosVazios.begin(); it != horariosVazios.end(); it++){
		if( (*it)->aula != NULL  ){
			printf("Horario vazio nao eh nulo\n");
			exit(0);
		}
	}

	printf("\n\n");
}

void Individuo::ImprimeInstancia(char* nomeArquivo){
	list<Alocacao*>::iterator it;
    Timeslot* ts;
    FILE *f;

	// abre arquivo
    f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        printf("Erro de abertura do arquivo %s\n", nomeArquivo);
        exit(1);
    }

	for( it=aulasAlocadas.begin(); it!=aulasAlocadas.end(); it++ ) {
		ts = (*it)->horario;
		fprintf(f, "%s %s %d %d\n", (*it)->aula->disciplina->nome, (*it)->sala->idSala, ts->dia, ts->periodo);
    }
	fclose(f);
}


Alocacao* Individuo::ObtemAlocacao(int horario, int numSala){
	list<Alocacao*>::iterator it;
	for( it=TodosHorarios.begin(); it!=TodosHorarios.end(); it++ ) {
		if( ((*it)->horario->horario == horario) &&
			((*it)->sala->numeroSequencial == numSala) )
			return (*it);
	}
	fprintf(stderr, "ERRO em Individuo.cpp: horario %d e sala %d nao existente no individuo.\n\n", horario, numSala);
	exit(-1);
}


Individuo::~Individuo(){
	Alocacao* aloc;
	int i, s;

	while( TodosHorarios.size() > 0 ) {
		aloc = TodosHorarios.back();
		TodosHorarios.pop_back();
		delete(aloc);
	}

	for( i=0; i!=p->nDisciplinas; i++ ) {
		free(Alocacao_dias_utilizados[i]);
		free(Alocacao_salas_utilizadas[i]);
	}
	free(Alocacao_salas_utilizadas);
	free(Alocacao_dias_utilizados);

	for( i=0; i!=p->nCurriculos; i++ ) {
		free(Alocacao_horarios_utilizados_por_curriculo[i]);
		for( s=0; s<p->nDias; s++ ){
			free(matrizAlocacaoCurriculoDiasPeriodos[i][s]);
		}
		free(matrizAlocacaoCurriculoDiasPeriodos[i]);
	}
	free(Alocacao_horarios_utilizados_por_curriculo);
	free(matrizAlocacaoCurriculoDiasPeriodos);
	for( i=0; i!=(int) p->professores.size(); i++ ) {
		free(matrizProfessorHorario[i]);
		free(matrizProfessorHorarioQntd[i]);
	}
	free(matrizProfessorHorario);
	free(matrizProfessorHorarioQntd);
}
