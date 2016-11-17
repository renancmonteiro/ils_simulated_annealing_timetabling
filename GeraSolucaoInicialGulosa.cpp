/*
 * GeraSolucacoInicial.cpp
 *
 *  Created on: May 18, 2015
 *      Author: erika
 */

#include "GeraSolucaoInicialGulosa.h"

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <math.h>

GeraSolucaoInicialGulosa::~GeraSolucaoInicialGulosa(){
	int i;
	for (i = 0; i <nCandidatos; i++) delete(candidatos[i]);
	free(candidatos);
}

GeraSolucaoInicialGulosa::GeraSolucaoInicialGulosa(Problema* p, float piThreshold) {
    list<Disciplina*>::iterator it;
    list<Disciplina*>::iterator it2;
    list<Aula*>::iterator itAulas;
    int i;

    threshold = piThreshold;
	alpha     = 1;
	betha     = 0.5;
	//Inicializa lista de candidatos
    nCandidatos = p->nDisciplinas;
    candidatos = (Candidato**) malloc ( nCandidatos * sizeof(Candidato*) );
    i = 0;
	for (it = p->disciplinas.begin(); it != p->disciplinas.end(); it++) {
		candidatos[i] = new Candidato((*it), p);
		candidatos[i]->nAulasParaAlocar = (*it)->nAulas;
		for (it2 = p->disciplinas.begin(); it2 != p->disciplinas.end(); it2++) {
			if( p->SaoDoMesmoProfessor(*it2, *it) || p->SaoDoMesmoCurriculo(*it2, *it) )
				candidatos[i]->nDisciplinasComMesmoCurriculoOuProfessor++;
		}
		i++;
	}
}

void GeraSolucaoInicialGulosa::CriaIndividuoInicial(Problema* p, Individuo* ind){
	Candidato* c;
	list<Alocacao*>::iterator it;
	int i;

	while (nCandidatos > 0) {       // enquanto ha candidatos a alocar

		for( i=0; i<nCandidatos; i++ )
			contaTotalHorariosViaveis(p, candidatos[i], ind);
		ordenaAulasPorDificuldade(p, ind);

		// aula "mais dificil"
		c = candidatos[nCandidatos-1];
// 		printf("%3d: Total Horarios Viaveis = %3d\n", c->disc->numeroSequencial, c->nHorariosViaveis/c->nAulasParaAlocar);

		alocaAula(p, ind, c);
		c->nAulasParaAlocar--;
		if( c->nAulasParaAlocar == 0 ) {
			delete(c);
			nCandidatos--;
		}

	}

}

void GeraSolucaoInicialGulosa::alocaAula(Problema* p, Individuo* ind, Candidato* cand){
	int i;
	Aula* aula = cand->disc->aulas.back();
	list<Possibilidade*> possibilidades;
    list<Possibilidade*>::iterator itPos;
    list<Possibilidade*>::iterator itPos2;
    list<Possibilidade*>::iterator itPosProximo2;
    list<Alocacao*>::iterator it;
    list<Curriculo*>::iterator itCurr;
    list<Disciplina*>::iterator itDisc;

    Possibilidade* possibilidade;
    Alocacao* aloc;
    float cMin = 999999, cMax = 0;

	int nrPossibilidades = 0;
    float maxAulasParaInd = 0, minAulasParaInd = 99999;
    double maxcEscolha = 0, mincEscolha = 99999;
    //printf("\nAloca aula da disciplina %3d\n", cand->disc->numeroSequencial);

	// cria "Alocacao" marcando (horario,sala) que estao disponiveis
	for( it = ind->horariosVazios.begin(); it!=ind->horariosVazios.end(); it++ ){
		if( cand->horariosViaveis[(*it)->horario->horario] == 0 ){
			possibilidade = new Possibilidade(aula, (*it), p, ind);
			possibilidades.push_back( possibilidade );
			if( (nrPossibilidades == 0) || (possibilidade->custoAlocacao > cMax) ) cMax = possibilidade->custoAlocacao;
			if( (nrPossibilidades == 0) || (possibilidade->custoAlocacao < cMin) ) cMin = possibilidade->custoAlocacao;
			//Calcula o numero de aulas que se tornam indisponíveis ao alocar essa nesse horário
			for( i=0; i<nCandidatos; i++ ){
				if( candidatos[i]->horariosViaveis[possibilidade->horario->horario->horario] == 0 ){
					possibilidade->nAulasParaIndisponibilizar++;
				}
			}
			if( maxAulasParaInd < possibilidade->nAulasParaIndisponibilizar ) maxAulasParaInd = possibilidade->nAulasParaIndisponibilizar;
			if( minAulasParaInd > possibilidade->nAulasParaIndisponibilizar ) minAulasParaInd = possibilidade->nAulasParaIndisponibilizar;
			//Calcula o custo * nAulasParaIndisponibilizar
			if( cMax == cMin ) cMax++;
			if( maxAulasParaInd == minAulasParaInd ) maxAulasParaInd++;
			//cEscolha = g do artigo do Lu e Hao
			possibilidade->cEscolha =
					( alpha ) * ( (possibilidade->nAulasParaIndisponibilizar))+//-minAulasParaInd)/(maxAulasParaInd-minAulasParaInd) ) +
					( betha ) * ( (possibilidade->custoAlocacao));//-cMin)/(cMax-cMin) );
			if( maxcEscolha < possibilidade->cEscolha ) maxcEscolha = possibilidade->cEscolha;
			if( mincEscolha > possibilidade->cEscolha ) mincEscolha = possibilidade->cEscolha;
			/*
			printf("Opcao de Horario %3.2lf: %3.2f + %3.2f\n",
					possibilidade->cEscolha,
					(possibilidade->custoAlocacao-cMin)/(cMax-cMin),
					(possibilidade->nAulasParaIndisponibilizar-minAulasParaInd)/(maxAulasParaInd-minAulasParaInd) );
			//*/
			nrPossibilidades++;
		}
	}

	if( nrPossibilidades == 0 ){
		fprintf(stderr,"Nao ha possiveis horarios/salas para alocar a aula %s\n", cand->disc->nome);
		fprintf(stderr,"nHorariosViaveis     = %3d\n", cand->nHorariosViaveis);
		fprintf(stderr,"nHorariosSalaViaveis = %3d\n", cand->nHorariosSalaViaveis);
		fprintf(stderr,"nDiscMesmoCurrOuProf = %3d\n", cand->nDisciplinasComMesmoCurriculoOuProfessor);
		fprintf(stderr,"nAulasParaAlocar     = %3d\n", cand->nAulasParaAlocar);
		ind->ImprimeIndividuo();
		exit(0);
	}
//	printf("nPossiveisHorarios: %3d\n", nrPossibilidades);
//	printf("Custo (%4.0lf %4.0lf) Indis (%3.0lf %3.0lf) cEscolha (%3.0lf %3.0lf)\n", cMin, cMax, minAulasParaInd, maxAulasParaInd, mincEscolha, maxcEscolha);

	for( itPos=possibilidades.begin(); itPos!=possibilidades.end(); itPos++ ){
		if( (*itPos)->cEscolha == mincEscolha ) {
			aloc = (*itPos)->horario;
			break;
		}
	}

	aloc->aula = aula;
	ind->aulasAlocadas.push_back(aloc);
	ind->horariosVazios.remove(aloc);
	ind->Alocacao_dias_utilizados[aloc->aula->disciplina->numeroSequencial][aloc->horario->dia]++;
	ind->Alocacao_salas_utilizadas[aloc->aula->disciplina->numeroSequencial][aloc->sala->numeroSequencial]++;
	ind->matrizProfessorHorarioQntd[aloc->aula->disciplina->professor->numeroSequencial][aloc->horario->horario]++;
	ind->matrizProfessorHorario[aloc->aula->disciplina->professor->numeroSequencial][aloc->horario->horario] = aloc;
	for( itCurr = aula->disciplina->curriculos.begin(); itCurr!=aula->disciplina->curriculos.end(); itCurr++){
		ind->Alocacao_horarios_utilizados_por_curriculo[(*itCurr)->numeroSequencial][aloc->horario->horario]++;
		ind->matrizAlocacaoCurriculoDiasPeriodos[(*itCurr)->numeroSequencial][aloc->horario->dia][aloc->horario->periodo] = aloc;
	}

//	printf("Aloca aula %3d(%3d) no horario %3d\n", cand->disc->numeroSequencial, cand->disc->professor->numeroSequencial, aloc->horario->horario);
	while( possibilidades.size() > 0 ) {
		possibilidade = possibilidades.back();
		possibilidades.pop_back();
		delete(possibilidade);
	}

}

int GeraSolucaoInicialGulosa::contaTotalHorariosViaveis(Problema* p, Candidato* cand, Individuo* ind){
	int i;
	Disciplina *discPos;
	Timeslot* horarioPos;
	list<Alocacao*>::iterator it;
	Disciplina* discCand = cand->disc;
	int horarioVisitado[p->nHorarios];

	// Inicializa os horarios viaveis
	for (i = 0; i < p->nHorarios; i++) {
		horarioVisitado[i]       = 0;
		cand->horariosViaveis[i] = p->HorarioIndisponivelDisciplina(discCand, i);
	}

	// verifica os conflitos curriculo/professor
	for( it = ind->aulasAlocadas.begin(); it != ind->aulasAlocadas.end(); it++ ){
		discPos    = (*it)->aula->disciplina;
		horarioPos = (*it)->horario;
		//Conflito de Professor e de Currículo
		if ( p->SaoDoMesmoProfessor(discCand, discPos) || p->SaoDoMesmoCurriculo(discCand, discPos) ) {
			cand->horariosViaveis[horarioPos->horario] = 1;
		}
	}

	cand->nHorariosViaveis     = 0;
	cand->nHorariosSalaViaveis = 0;
	for( it = ind->horariosVazios.begin(); it != ind->horariosVazios.end(); it++ ){
		if(  cand->horariosViaveis[(*it)->horario->horario] == 0 ) {
			cand->nHorariosSalaViaveis++;
			if( horarioVisitado[(*it)->horario->horario] == 0 ){
				horarioVisitado[(*it)->horario->horario] = 1;
				cand->nHorariosViaveis++;
			}
		}
	}
	return cand->nHorariosViaveis;
}

void GeraSolucaoInicialGulosa::ordenaAulasPorDificuldade(Problema* p, Individuo* ind){
	int i, j;
	Candidato* chave;
	for (j = 1; j < nCandidatos; j++) {
		chave = candidatos[j];
		i = j - 1;
		while (i >= 0 && comparaAulas(p, ind, candidatos[i], chave) > 0) {
			candidatos[i + 1] = candidatos[i];
			i--;
		}
		candidatos[i + 1] = chave;
	}
	/*
	printf("Candidatos(%3d): ", nCandidatos);
	j = nCandidatos - 5;
	if( j < 0 ) j = 0;
	for (;j < nCandidatos; j++) {
		printf("%2d(%2d) ", candidatos[j]->disc->numeroSequencial, candidatos[j]->nHorariosViaveis/candidatos[j]->nAulasParaAlocar);
	}
	printf("\n");
	*/
}

int GeraSolucaoInicialGulosa::comparaAulas(Problema* p, Individuo* ind, Candidato* c1, Candidato* c2) {
	int horariosDisp1, horariosDisp2;
	int horariosSalaDisp1, horariosSalaDisp2;
	Disciplina* d1 = c1->disc;
	Disciplina* d2 = c2->disc;

	if ( d1 == d2 ) return 0; //Mesma disciplina

	// calcula os horarios disponiveis para cada aula
	horariosDisp1 = c1->nHorariosViaveis / sqrt(c1->nAulasParaAlocar);
	horariosDisp2 = c2->nHorariosViaveis / sqrt(c2->nAulasParaAlocar);
	horariosSalaDisp1 = c1->nHorariosSalaViaveis / sqrt(c1->nAulasParaAlocar);
	horariosSalaDisp2 = c2->nHorariosSalaViaveis / sqrt(c2->nAulasParaAlocar);

	if (horariosDisp1 > horariosDisp2) {
		return -1;
	} else if (horariosDisp1 < horariosDisp2) {
		return 1;
	} else if (horariosSalaDisp1 > horariosSalaDisp2) {
		return -1;
	} else if (horariosSalaDisp1 < horariosSalaDisp2) {
		return 1;
	} else if (c1->nDisciplinasComMesmoCurriculoOuProfessor < c2->nDisciplinasComMesmoCurriculoOuProfessor) {
		return -1;
	} else if (c1->nDisciplinasComMesmoCurriculoOuProfessor > c2->nDisciplinasComMesmoCurriculoOuProfessor) {
		return 1;
	} else {
		return 0;
	}
}

