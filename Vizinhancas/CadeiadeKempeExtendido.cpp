/*
 * CadeiadeKempeExtendido.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: erika
 */

#include "CadeiadeKempeExtendido.h"

#include "../Model/Alocacao.h"
#include "../Model/Aula.h"
#include "../Model/Disciplina.h"
#include "../Model/Individuo.h"
#include "../Model/Problema.h"
#include "../Model/Professor.h"
#include "../Model/Curriculo.h"

#include "Swap.h"
#include "Move.h"

#include <stdlib.h>
#include <algorithm>
#include <list>
using namespace std;


class opAloc{
public:
	Alocacao* a;
	int disponivelParaTroca;//int > 0 representa que posso mover a alocacao para o horario de t2
	int sequencial;

	opAloc(Alocacao* piA, int seq){
		if( piA == NULL ) {
			fprintf(stderr, "Erro: alocacao nula ao criar opAloc (Cadeia de Kempe)\n\n");
			exit(0);
		}
		a = piA;
		disponivelParaTroca = 1;
		sequencial = seq;
	};
};

int CadeiadeKempeExtendido::KMP_temRestricao(Alocacao* p1, Alocacao* p2) {
	list<opAloc*>::iterator it;
	list<Curriculo*>::iterator itC1, itC2;
	Aula *aula1, *aula2;

	aula1 = p1->aula;
	aula2 = p2->aula;

	if( aula1!=NULL && aula2!=NULL ){
		if( aula1->disciplina->numeroSequencial==aula2->disciplina->numeroSequencial ){
			return 1;
		}
		if( aula1->disciplina->professor->numeroSequencial==aula2->disciplina->professor->numeroSequencial ){
			return 1;
		}
		for( itC1=aula1->disciplina->curriculos.begin(); itC1!=aula1->disciplina->curriculos.end(); itC1++ ){
			for( itC2=aula2->disciplina->curriculos.begin(); itC2!=aula2->disciplina->curriculos.end(); itC2++ ){
				if( (*itC1)->numeroSequencial==(*itC2)->numeroSequencial ){
					return 1;
				}
			}
		}
	}

	return 0;
}

int CadeiadeKempeExtendido::ObtemMelhorPosicaoA(int* matrizConflitos[]){
	int posInicial, numeroConflitosPosInicial, numeroConflitosPosAtual;
	list<opAloc*>::iterator it1, it2;

	posInicial = 0;
	numeroConflitosPosInicial = -9999;
	for( it1=t1.begin(); it1!=t1.end(); it1++ ){
		numeroConflitosPosAtual = 0;
		for( it2=t2.begin(); it2!=t2.end(); it2++ ){
			numeroConflitosPosAtual += matrizConflitos[(*it1)->sequencial][(*it2)->sequencial];
		}
		if( (*it1)->disponivelParaTroca > 0 ){
			if( (numeroConflitosPosAtual > numeroConflitosPosInicial) ){
				numeroConflitosPosInicial = numeroConflitosPosAtual;
				posInicial = (*it1)->sequencial;
			}
			if( (numeroConflitosPosAtual = numeroConflitosPosInicial) && ((rand()%RAND_MAX)<0.7) ){
				numeroConflitosPosInicial = numeroConflitosPosAtual;
				posInicial = (*it1)->sequencial;
			}
		}
	}
	if( numeroConflitosPosInicial <= 0 ) return -1;
	return posInicial;
}

opAloc* CadeiadeKempeExtendido::ObtemInicioProximaCadeiaKempe(int* matrizConflitos[], int tamConflitos, int nPosComConflitoA, int nposComConflitoB){
	int numeroConflitosPosInicial, numeroConflitosPosAtual;
	list<opAloc*>::iterator it1, it2;

	opAloc *posInicial = NULL;
	numeroConflitosPosInicial = 9999;
	if( nPosComConflitoA < nposComConflitoB ) {
		//Escolhe posição em t1
		for( it1=t1.begin(); it1!=t1.end(); it1++ ){
			numeroConflitosPosAtual = 0;
			for( it2=t2.begin(); it2!=t2.end(); it2++ ){
				numeroConflitosPosAtual += matrizConflitos[(*it1)->sequencial][(*it2)->sequencial];
			}
			//printf("Avalia sala %d : conflitos = %d\n", (*it1)->a->sala->numeroSequencial, numeroConflitosPosAtual);
			if( (*it1)->disponivelParaTroca > 0 ){
				if( (numeroConflitosPosAtual < numeroConflitosPosInicial) ){
					numeroConflitosPosInicial = numeroConflitosPosAtual;
					posInicial = (*it1);
				}
				if( (numeroConflitosPosAtual = numeroConflitosPosInicial) && ((rand()%RAND_MAX)<0.7) ){
					numeroConflitosPosInicial = numeroConflitosPosAtual;
					posInicial = (*it1);
				}
			}
		}
	} else {
		//Escolhe posição em t2
		for( it2=t2.begin(); it2!=t2.end(); it2++ ){
			numeroConflitosPosAtual = 0;
			for( it1=t1.begin(); it1!=t1.end(); it1++ ){
				numeroConflitosPosAtual += matrizConflitos[(*it1)->sequencial][(*it2)->sequencial];
			}
			//printf("Avalia sala %d : conflitos = %d\n", (*it2)->a->sala->numeroSequencial, numeroConflitosPosAtual);
			if( (*it2)->disponivelParaTroca > 0 ){
				if( (numeroConflitosPosAtual < numeroConflitosPosInicial) ){
					numeroConflitosPosInicial = numeroConflitosPosAtual;
					posInicial = (*it2);
				}
				if( (numeroConflitosPosAtual = numeroConflitosPosInicial) && ((rand()%RAND_MAX)<0.7) ){
					numeroConflitosPosInicial = numeroConflitosPosAtual;
					posInicial = (*it2);
				}
			}
		}
	}
	return posInicial;
}

int CadeiadeKempeExtendido::PreencheVetorCadeiaKempe(int* Conflitos[], int tamConflitos, int posInicial){
//Retorna -1 caso tenha dado errado. retorna > 0 caso tenha dado certo
	int nPosComConflitoA = 0, nPosComConflitoB = 0;
	int continua, qntdA, qntdB, inicializouB = 0;
	opAloc* op;

	list<opAloc*>::iterator it1, it2;

	//Escolhe o primeiro timeslot de t1
	op = NULL;
	for( it1=t1.begin(); it1!=t1.end(); it1++ ){
		if( (*it1)->sequencial==posInicial ){
			op = *it1;
			break;
		}
	}
	if( op == NULL ){
		fprintf(stderr, "A posicao Inicial '%d' da Cadeia de Kempe nao foi encontrada no vetor t1.\n", posInicial);
		exit(0);
	}
	trocast1.push_back(op);
	t1.remove(op);
	//~ printf("Constrói a acadeia de Kempe:\n");
	//~ if ( op->a->aula == NULL ) printf("horario, sala, aula = %3d, %3d, ---\n", op->a->horario->horario, op->a->sala->numeroSequencial);
	//~ else printf("horario, sala, aula = %3d, %3d, %3d\n", op->a->horario->horario, op->a->sala->numeroSequencial, op->a->aula->numero);
	nPosComConflitoA++;

	//Inicia busca por aulas conflitantes com a aula que se encontra na posInicial.
	qntdA = 1;
	continua = 1;
	while( continua ){
		continua = 0;
		qntdB = 0;
		//Acha elemento em t2 que conflita com alguem em trocast1
		//printf("Procura aula em t2 que conflite com alguma aula da cadeia de kempe atual\n");
		for( it1=trocast1.begin(); it1!=trocast1.end(); it1++ ){
			for( it2=t2.begin(); it2!=t2.end(); ){
				//printf("\t\tAula do (horario, sala) (%3d, %3d) conflita?\n", (*it2)->a->horario->horario, (*it2)->a->sala->numeroSequencial);
				if( (Conflitos[(*it1)->sequencial][(*it2)->sequencial] == 1) ){
					if( (*it2)->disponivelParaTroca == 0 )
						return -1;
					op = *it2;
					//printf("t2: adiciona sala %d\n", op->a->sala->numeroSequencial);
					it2++;
					trocast2.push_back(op);
					t2.remove(op);
					nPosComConflitoB++;
					qntdB++;
				}
				else it2++;
			}
		}
		if( inicializouB ) {qntdB++; inicializouB = 0;}
		qntdA = 0;
		//Acha elemento em t1 que conflita com alguem em trocast2
		//printf("Procura aula em t1 que conflite com alguma aula da cadeia de kempe atual\n");
		for( it2=trocast2.begin(); it2!=trocast2.end(); it2++ ){
			for( it1=t1.begin(); it1!=t1.end(); ){
				//printf("\t\tAula do (horario, sala) (%3d, %3d) conflita?\n", (*it1)->a->horario->horario, (*it1)->a->sala->numeroSequencial);
				if( (Conflitos[(*it1)->sequencial][(*it2)->sequencial] == 1) ){
					if( (*it1)->disponivelParaTroca == 0 )
						return -1;
					op = *it1;
					//printf("t1: adiciona sala %d\n", op->a->sala->numeroSequencial);
					it1++;
					trocast1.push_back(op);
					t1.remove(op);
					nPosComConflitoA++;
					qntdA++;
					continua = 1;
				}
				else it1++;
			}
		}

		//Se acabou a cadeia de kempe, mas o número de timeslots em trocast1 e trocas t2 é diferente
		if( continua == 0 && nPosComConflitoA != nPosComConflitoB ){
			//printf("Continua cadeia de Kempe\n");
			//escolhe mais uma cadeia de Kempe
			if( nPosComConflitoA < nPosComConflitoB ){
				op = ObtemInicioProximaCadeiaKempe(Conflitos, tamConflitos, nPosComConflitoA, nPosComConflitoB);
				if( op != NULL ){
					//printf("posInicial = %d\n", op->a->sala->numeroSequencial);
					trocast1.push_back(op);
					t1.remove(op);
					nPosComConflitoA++;
					qntdA++;
					continua = 1;
				} //else printf("posInicial = NULL\n");
			}
			else{//nPosComConflitoA > nPosComConflitoB
				op = ObtemInicioProximaCadeiaKempe(Conflitos, tamConflitos, nPosComConflitoA, nPosComConflitoB);
				if( op != NULL ){
					//printf("posInicial = %d\n", op->a->sala->numeroSequencial);
					trocast2.push_back(op);
					t2.remove(op);
					nPosComConflitoB++;
					inicializouB = 1;
					continua = 1;
				} //else printf("posInicial = NULL\n");
			}
		}

	}//Fim whlie(continua)
	//printf("Fim obtem trocas\n");

	return 1;
}



int CadeiadeKempeExtendido::KMP_preencheListaMovimentosTroca() {
	int i;
	int j;
	Problema* p = ind->p;
	list<opAloc*>::iterator it1, it2;

	int* Conflitos[p->nSalas];
	int posInicial;

	//Preenche Matriz(Grafo) de conflitos
	//~ printf("4.1. Preenche Matriz(Grafo) de conflitos\n");
	for (i = 0; i < p->nSalas; i++) {
		Conflitos[i] = (int*) malloc ( (p->nSalas) * sizeof(int));
		for (j = 0; j < p->nSalas; j++) Conflitos[i][j] = 0;
	}

	//~ printf("4.2. Preenche Matriz(Grafo) de conflitos\n");
	for( it1=t1.begin(); it1!=t1.end(); it1++ ){
		for( it2=t2.begin(); it2!=t2.end(); it2++ ){
			fflush(stdout);
			if ( (*it1)->a->aula!=NULL && (*it2)->a->aula!=NULL ) {
				Conflitos[(*it1)->sequencial][(*it2)->sequencial] = KMP_temRestricao((*it1)->a, (*it2)->a);
			}
			else {
				Conflitos[(*it1)->sequencial][(*it2)->sequencial] = 0;
			}
		}
	}
	/*
	printf("4.3. Matriz(Grafo) de conflitos:\n");
	Alocacao *a1, *a2;
	for( it1=t1.begin(); it1!=t1.end(); it1++ ){
		for( it2=t2.begin(); it2!=t2.end(); it2++ )
			if( Conflitos[(*it1)->sequencial][(*it2)->sequencial] == 1 ){
				a1 = (*it1)->a;
				a2 = (*it2)->a;
				printf("conflito entre ");
				if( a1->aula == NULL && a2->aula == NULL ){
					printf("--- (%3d)(%2d) x --- (%3d)(%2d)\n", a1->horario->horario, a1->sala->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
				}
				else if( a1->aula == NULL ){
					printf("--- (%3d)(%2d) x %3d (%3d)(%2d)\n", a1->horario->horario, a1->sala->numeroSequencial, a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
				}
				else if (a2->aula == NULL){
					printf("%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
				}
				else {
					printf("%3d (%3d)(%2d) x %3d (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial,
																a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
				}
			}
	}//*/

	//Obtem a posicao inicial (onde comeca a cadeia de kempe)
	//~ printf("4.4. Obtem a posicao inicial\n");
	posInicial = ObtemMelhorPosicaoA(Conflitos);
	if( posInicial < 0 ) {
		for (i = 0; i < p->nSalas; i++) {
			free(Conflitos[i]);
		}
		return 0;
	}

	//Obtem vetor contendo as posicoes da matriz que representam uma cadeia de Kempe
	//~ printf("4.5. PreencheVetorCadeiaKempe\n");
	PreencheVetorCadeiaKempe(Conflitos, p->nSalas, posInicial);

	//~ printf("4.6. free\n");
	for (i = 0; i < p->nSalas; i++) {
		free(Conflitos[i]);
	}

	return 1;
}

void CadeiadeKempeExtendido::AtualizaListaDisponiveis(){
    list<opAloc*>::iterator it1, it2;
    int continua = 1;
    opAloc* op;

    while(continua){
		continua = 0;
		//~ printf("Procura em t1, aulas que possuam restrição com aulas indisponíveis\n");
		for(it1=t1.begin(); it1!=t1.end(); it1++ ){
			for( it2=t2Ind.begin(); it2!=t2Ind.end(); it2++ ){
				//~ op = *it1;
				//~ printf("Avalia aula (%3d, %3d)\n", op->a->horario->horario, op->a->sala->numeroSequencial);
				if ( (*it1)->a->aula!=NULL && (*it2)->a->aula!=NULL && KMP_temRestricao((*it1)->a, (*it2)->a) ){
					op = *it1;
					//~ printf("Aula %s (%3d, %3d) indisponível\n", op->a->aula->disciplina->nome, op->a->horario->horario, op->a->sala->numeroSequencial);
					it1++;
					t1.remove(op);
					t1Ind.push_back(op);
					continua = 1;
					if( it1==t1.end() ) break;
				} 
			}			
		}
		//~ printf("Procura em t2, aulas que possuam restrição com aulas indisponíveis\n");
		for( it1=t1Ind.begin(); it1!=t1Ind.end(); it1++ ){
			for( it2=t2.begin(); it2!=t2.end(); it2++ ){
				//~ op = *it2;
				//~ printf("Avalia aula (%3d, %3d)\n", op->a->horario->horario, op->a->sala->numeroSequencial);
				if ( (*it1)->a->aula!=NULL && (*it2)->a->aula!=NULL && KMP_temRestricao((*it1)->a, (*it2)->a) ){
					op = *it2;
					//~ printf("Aula %s (%3d, %3d) indisponível\n", op->a->aula->disciplina->nome, op->a->horario->horario, op->a->sala->numeroSequencial);
					it2++;
					t2.remove(op);
					t2Ind.push_back(op);
					continua = 1;
					if( it2==t2.end() ) break;
				}
			}
		}
		//~ printf("continua = %d\n", continua);
	}
}

CadeiadeKempeExtendido::CadeiadeKempeExtendido(Problema* p, Individuo* piInd) {
    list<Alocacao*>::iterator it1, it2;
    list<opAloc*>::iterator it3, it4;
	int pos1 = rand() % piInd->aulasAlocadas.size();
	int pos2 = rand() % piInd->aulasAlocadas.size();
	int i, j;
	Movimento* m;

	//~ printf("1. Inicia construção Kempe\n");
	tipoMovimento = 3;

	ind = piInd;

	//escolher dois timeslots
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

	i = j = 0;
	//~ printf("2. Obtem todos os timeslots do horario %d e %d\n", a1->horario->horario, a2->horario->horario);
	for( it1=ind->TodosHorarios.begin(); it1!=ind->TodosHorarios.end(); it1++ ){
		if( (*it1)->horario->horario == a1->horario->horario ){
			if(	(*it1)->aula == NULL ){
				t1.push_back(new opAloc(*it1, i));
				i++;
			} else {
				if( p->HorarioIndisponivelDisciplina((*it1)->aula->disciplina, a2->horario->horario) ){
					t1Ind.push_back(new opAloc(*it1, 0));
				}else{
					t1.push_back(new opAloc(*it1, i));
					i++;
				}
			}
		}
		if( (*it1)->horario->horario == a2->horario->horario ){
			if(	(*it1)->aula == NULL ){
				t2.push_back(new opAloc(*it1, j));
				j++;
			} else {
				if( p->HorarioIndisponivelDisciplina((*it1)->aula->disciplina, a1->horario->horario) ){
					t2Ind.push_back(new opAloc(*it1, 0));
				}else{
					t2.push_back(new opAloc(*it1, j));
					j++;
				}
			}
		}
	}
	
	//Remove na lista de disponíveis, as alocações que tem restrição com os indisponíveis
	//~ printf("3. Remove na lista de disponíveis, as alocações que tem restrição com os indisponíveis\n");
	//~ printf("t1 = %3d, t1Ind = %3d\n", (int) t1.size(), (int) t1Ind.size());
	//~ printf("t2 = %3d, t2Ind = %3d\n", (int) t2.size(), (int) t2Ind.size());
	AtualizaListaDisponiveis();

	//obtem lista de movimentos a realizar e os realiza
	//~ printf("4. Obtem lista de movimentos a realizar e os realiza\n");
	KMP_preencheListaMovimentosTroca();

	if( (int)trocast1.size() == 0 || (int)trocast2.size() == 0 || (int)trocast1.size()!=(int)trocast2.size() ) {
		a1 = a2 = NULL;
		deltaFit = deltaHard = 99999;
		//printf("kempe invalido\n");
		return;
	}

	/*
	printf("\nCadeia de kempe: (tamanho = %d = %d)\n", (int) trocast1.size(), (int) trocast2.size());
	i = 0;
	for( it3=trocast1.begin(), it4=trocast2.begin(); it3!=trocast1.end(), it4!=trocast2.end(); it3++, it4++ ){
		printf("%2d: ", i);
		fflush(stdout);
		i++;
		a1 = (*it3)->a;
		a2 = (*it4)->a;

		if( a1->aula == NULL && a2->aula == NULL ){
			printf("\t--- (%3d)(%2d) x --- (%3d)(%2d)\n", a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
		}
		else if( a1->aula == NULL ){
			printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
		}
		else if (a2->aula == NULL){
			printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
		}
		else {
			printf("\t%3d (%3d)(%2d) x %3d (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial,
														a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
		}
	}
	printf("\n");
	*/

	//Cria movimentos move e Swap e os insere na lista de movimentos
	//~ printf("5. Cria movimentos move e Swap e os insere na lista de movimentos\n");
	for( it3=trocast1.begin(), it4=trocast2.begin(); it3!=trocast1.end(), it4!=trocast2.end(); it3++, it4++ ){
		a1 = (*it3)->a;
		a2 = (*it4)->a;

		if( a1->aula == NULL && a2->aula == NULL ) m = NULL;
		else if( a1->aula == NULL )	m = new Move(ind, a2, a1);
		else if (a2->aula == NULL) 	m = new Move(ind, a1, a2);
		else 						m = new Swap(ind, a1, a2);
		if(  m!= NULL ) movimentos.push_back(m);
	}

	//Calcula deltaFit
	//printf("Calcula deltaFit\n");
	deltaFit = calculaDeltaFit(p);
	if( deltaFit > 90000 ) {
		printf("Erro cadeia de Kempe inválida\n");
		printf("deltaFit = %5d (%2d %2d %2d %2d)\n", deltaFit, deltaSoft1, deltaSoft2, deltaSoft3, deltaSoft4);
		printf("\nCadeia de kempe: (tamanho = %d = %d)\n", (int) trocast1.size(), (int) trocast2.size());
		i = 0;
		for( it3=trocast1.begin(), it4=trocast2.begin(); it3!=trocast1.end(), it4!=trocast2.end(); it3++, it4++ ){
			printf("%2d: ", i);
			fflush(stdout);
			i++;
			a1 = (*it3)->a;
			a2 = (*it4)->a;

			if( a1->aula == NULL && a2->aula == NULL ){
				printf("\t--- (%3d)(%2d) x --- (%3d)(%2d)\n", a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
			}
			else if( a1->aula == NULL ){
				printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
			}
			else if (a2->aula == NULL){
				printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
			}
			else {
				printf("\t%3d (%3d)(%2d) x %3d (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial,
															a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
			}
		}
		printf("-------------------\n\n");
		printf("Disponiveis para a troca: \n");				
		for( it3=t1.begin(); it3!=t1.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}		
		printf("\n");
		for( it3=t2.begin(); it3!=t2.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\nIndisponiveis para a troca: \n");			
		for( it3=t1Ind.begin(); it3!=t1Ind.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\n");
		for( it3=t2Ind.begin(); it3!=t2Ind.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\n");
		printf("-------------------\n\n");		
		deltaFit = calculaDeltaFitDebug(p);
		exit(0);
	}
}


CadeiadeKempeExtendido::CadeiadeKempeExtendido(Problema* p, Individuo* piInd, int piHorario1, int piHorario2) {
    list<Alocacao*>::iterator it1, it2;
    list<opAloc*>::iterator it3, it4;
	int i, j;
	Movimento* m;

	tipoMovimento = 3;
	ind = piInd;

	//printf("Faz cadeia de kempe entre horarios %d e %d\n", piHorario1, piHorario2);
	//Obtem todos os timeslots dos horarios a1 e a2
	i = j = 0;
	for( it1=ind->TodosHorarios.begin(); it1!=ind->TodosHorarios.end(); it1++ ){
		if( (*it1)->horario->horario == piHorario1 ){
			if(	(*it1)->aula == NULL ){
				t1.push_back(new opAloc(*it1, i));
				i++;
			} else {
				if( p->HorarioIndisponivelDisciplina((*it1)->aula->disciplina, piHorario2) ){
					t1Ind.push_back(new opAloc(*it1, 0));
				}else{
					t1.push_back(new opAloc(*it1, i));
					i++;
				}
			}
		}
		if( (*it1)->horario->horario == piHorario2 ){
			if(	(*it1)->aula == NULL ){
				t2.push_back(new opAloc(*it1, j));
				j++;
			} else {
				if( p->HorarioIndisponivelDisciplina((*it1)->aula->disciplina, piHorario1) ){
					t2Ind.push_back(new opAloc(*it1, 0));
				}else{
					t2.push_back(new opAloc(*it1, j));
					j++;
				}
			}
		}
	}
	//escolher dois timeslots
	a1 = t1.back()->a;
	a2 = t2.back()->a;

	//Remove na lista de disponíveis, as alocações que tem restrição com os indisponíveis
	AtualizaListaDisponiveis();

	//obtem lista de movimentos a realizar e os realiza
	KMP_preencheListaMovimentosTroca();

	if( (int)trocast1.size() == 0 || (int)trocast2.size() == 0 || (int)trocast1.size()!=(int)trocast2.size() ) {
		a1 = a2 = NULL;
		deltaFit = deltaHard = 99999;
//		printf("Kempe invalido\n");
//		for( it3=trocast1.begin(); it3!=trocast1.end(); it3++ ){
//			a1 = (*it3)->a;
//			if( a1->aula == NULL )	printf("Move(%2d, --)\n", a1->sala->numeroSequencial);
//			else 					printf("Swap(%2d, --)\n", a1->sala->numeroSequencial);
//		}
//		for( it4=trocast2.begin(); it4!=trocast2.end(); it4++ ){
//			a2 = (*it4)->a;
//			if( a2->aula == NULL )	printf("Move(--, %2d)\n", a2->sala->numeroSequencial);
//			else 					printf("Swap(--, %2d)\n", a2->sala->numeroSequencial);
//		}
		return;
	}

	//Cria movimentos move e Swap e os insere na lista de movimentos
	for( it3=trocast1.begin(), it4=trocast2.begin(); it3!=trocast1.end(), it4!=trocast2.end(); it3++, it4++ ){
		a1 = (*it3)->a;
		a2 = (*it4)->a;

		if( a1->aula == NULL && a2->aula == NULL ) m = NULL;
		else if( a1->aula == NULL )	m = new Move(ind, a2, a1);
		else if (a2->aula == NULL) 	m = new Move(ind, a1, a2);
		else 						m = new Swap(ind, a1, a2);
		if(  m!= NULL ) movimentos.push_back(m);
	}

	//Calcula deltaFit
	deltaFit = calculaDeltaFit(p);
	if( deltaFit > 90000 ) {
		printf("Erro cadeia de Kempe inválida\n");
		printf("deltaFit = %5d (%2d %2d %2d %2d)\n", deltaFit, deltaSoft1, deltaSoft2, deltaSoft3, deltaSoft4);
		printf("\nCadeia de kempe: (tamanho = %d = %d)\n", (int) trocast1.size(), (int) trocast2.size());
		i = 0;
		for( it3=trocast1.begin(), it4=trocast2.begin(); it3!=trocast1.end(), it4!=trocast2.end(); it3++, it4++ ){
			printf("%2d: ", i);
			fflush(stdout);
			i++;
			a1 = (*it3)->a;
			a2 = (*it4)->a;

			if( a1->aula == NULL && a2->aula == NULL ){
				printf("\t--- (%3d)(%2d) x --- (%3d)(%2d)\n", a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
			}
			else if( a1->aula == NULL ){
				printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial);
			}
			else if (a2->aula == NULL){
				printf("\t%3d (%3d)(%2d) x --- (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
			}
			else {
				printf("\t%3d (%3d)(%2d) x %3d (%3d)(%2d)\n", a1->aula->disciplina->numeroSequencial, a1->horario->horario, a1->sala->numeroSequencial,
															a2->aula->disciplina->numeroSequencial, a2->horario->horario, a2->sala->numeroSequencial);
			}
		}
		printf("-------------------\n\n");
		printf("Disponiveis para a troca: \n");
		for( it3=t1.begin(); it3!=t1.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\n");
		for( it3=t2.begin(); it3!=t2.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\nIndisponiveis para a troca: \n");
		for( it3=t1Ind.begin(); it3!=t1Ind.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\n");
		for( it3=t2Ind.begin(); it3!=t2Ind.end(); it3++ ){
			a1 = (*it3)->a;
			if( a1->aula == NULL ) printf("(%3d, %3d, -----) ", a1->horario->horario, a1->sala->numeroSequencial);
			else printf("(%3d, %3d, %s) ", a1->horario->horario, a1->sala->numeroSequencial, a1->aula->disciplina->nome);
		}
		printf("\n");
		printf("-------------------\n\n");
		deltaFit = calculaDeltaFitDebug(p);
		exit(0);
	}
}

CadeiadeKempeExtendido::~CadeiadeKempeExtendido(){
	Movimento* m;
	opAloc* op;
	while( movimentos.size() > 0 ) {
		m = movimentos.back();
		movimentos.pop_back();
		delete(m);
	}
	while( trocast1.size() > 0 ) {
		op = trocast1.back();
		trocast1.pop_back();
		delete(op);
	}
	while( trocast2.size() > 0 ) {
		op = trocast2.back();
		trocast2.pop_back();
		delete(op);
	}
	while( t1.size() > 0 ) {
		op = t1.back();
		t1.pop_back();
		delete(op);
	}
	while( t2.size() > 0 ) {
		op = t2.back();
		t2.pop_back();
		delete(op);
	}
	while( t1Ind.size() > 0 ) {
		op = t1Ind.back();
		t1Ind.pop_back();
		delete(op);
	}
	while( t2Ind.size() > 0 ) {
		op = t2Ind.back();
		t2Ind.pop_back();
		delete(op);
	}
}

void CadeiadeKempeExtendido::aplicaMovimento(){
	aplicaMoveSemRecalculoFuncaoObjetivo();

	ind->fitness += deltaFit;
	ind->hard  += deltaHard;
	ind->soft1 += deltaSoft1;
	ind->soft2 += deltaSoft2;
	ind->soft3 += deltaSoft3;
	ind->soft4 += deltaSoft4;
}

int CadeiadeKempeExtendido::calculaDeltaFit(Problema* p){
	list< Movimento* >::iterator it;
	int deltaFitness;

	Movimento* m;
	list< Alocacao* > listaAloc;
	int violaRestricaoHard = 0;


	for( it=movimentos.begin(); it!=movimentos.end(); it++ ){
		m = (*it);
		listaAloc.push_back(m->a1);
		if( m->tipoMovimento == 2 ){
			listaAloc.push_back(m->a2);
		}
	}
	p->CalculaCustoAulasAlocadas(ind, listaAloc, this);
	deltaSoft1   = -deltaSoft1;
	deltaSoft2   = -deltaSoft2;
	deltaSoft3   = -deltaSoft3;
	deltaSoft4   = -deltaSoft4;
	deltaFitness = deltaSoft1 + deltaSoft2 + deltaSoft3 + deltaSoft4;

	aplicaMoveSemRecalculoFuncaoObjetivo();
	for( it=movimentos.begin(); it!=movimentos.end(); it++ ){
		m = (*it);
		violaRestricaoHard += p->violaRestricaoGrave(ind, m);
	}
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulasAlocadas(ind, listaAloc, this); //Já atualiza os deltas(Soft e Fitness)
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}

	desaplicaMoveSemRecalculoFuncaoObjetivo();
	return deltaFitness;
}


int CadeiadeKempeExtendido::calculaDeltaFitDebug(Problema* p){
	list< Movimento* >::iterator it;
	int deltaFitness;

	Movimento* m;
	list< Alocacao* > listaAloc;
	int violaRestricaoHard = 0;

	for( it=movimentos.begin(); it!=movimentos.end(); it++ ){
		m = (*it);
		listaAloc.push_back(m->a1);
		if( m->tipoMovimento == 2 ){
			listaAloc.push_back(m->a2);
		}
	}
	p->CalculaCustoAulasAlocadas(ind, listaAloc, this);
	deltaSoft1   = -deltaSoft1;
	deltaSoft2   = -deltaSoft2;
	deltaSoft3   = -deltaSoft3;
	deltaSoft4   = -deltaSoft4;
	deltaFitness = deltaSoft1 + deltaSoft2 + deltaSoft3 + deltaSoft4;

	//~ printf("ind antes:  %5d\n", p->CalculaFuncaoObjetivo(ind));
	aplicaMoveSemRecalculoFuncaoObjetivo();
	for( it=movimentos.begin(); it!=movimentos.end(); it++ ){
		m = (*it);
		violaRestricaoHard += p->violaRestricaoGrave(ind, m);
		if( m->a2->aula == NULL ) {
			//~ printf("\tm: (%3d,%3d: %s) x (%3d,%3d: -----) violaHard: %5d\n", m->a1->horario->horario, m->a1->sala->numeroSequencial, m->a1->aula->disciplina->nome,
																		   //~ m->a2->horario->horario, m->a2->sala->numeroSequencial, violaRestricaoHard);
			printf("m: (%3d,%3d:  ", m->a1->horario->horario, m->a1->sala->numeroSequencial);
			m->a1->aula->disciplina->imprime();
		} else {
			printf("m: (%3d,%3d: ", m->a1->horario->horario, m->a1->sala->numeroSequencial);
			printf("m: (%3d,%3d: ", m->a2->horario->horario, m->a2->sala->numeroSequencial);
			m->a1->aula->disciplina->imprime();
			m->a2->aula->disciplina->imprime();
		}
	}
	//~ printf("ind depois: %5d\n\n", p->CalculaFuncaoObjetivo(ind));
	if( ! violaRestricaoHard ){
		deltaFitness += p->CalculaCustoAulasAlocadas(ind, listaAloc, this); //Já atualiza os deltas(Soft e Fitness)
	}
	else {deltaFitness = 99999; deltaHard  = 99999;}

	desaplicaMoveSemRecalculoFuncaoObjetivo();
	return deltaFitness;
}

void CadeiadeKempeExtendido::aplicaMoveSemRecalculoFuncaoObjetivo(){
	list< Movimento* >::iterator it;
	Movimento* m;

	for( it=movimentos.begin(); it!=movimentos.end(); it++ ){
		m = (*it);
		if (m->tipoMovimento == 1){
			((Move*) m)->aplicaMoveSemRecalculoFuncaoObjetivo();
		}else{
			((Swap*) m)->aplicaMoveSemRecalculoFuncaoObjetivo();
		}
	}

}

void CadeiadeKempeExtendido::desaplicaMoveSemRecalculoFuncaoObjetivo(){
	list< Movimento* >::iterator it;
	Movimento* m;

	it=movimentos.end();
	do{
		it--;

		m = (*it);
		if (m->tipoMovimento == 1){
			((Move*) m)->aplicaMoveSemRecalculoFuncaoObjetivo();
		}else{
			((Swap*) m)->aplicaMoveSemRecalculoFuncaoObjetivo();
		}

	}while( it!=movimentos.begin() );
}


void CadeiadeKempeExtendido::imprimeVetorTrocas(list< opAloc* > vet){
    list<opAloc*>::iterator it;

	for( it=vet.begin(); it!=vet.end(); it++ ){
		if( (*it)->a->aula != NULL )
			printf("Disciplina %d/%s Horario %d\n", (*it)->a->aula->disciplina->numeroSequencial, (*it)->a->aula->disciplina->nome, (*it)->a->horario->horario);
		else
			printf("Disciplina --/------ Horario %d\n", (*it)->a->horario->horario);
	}
}

