/*
 * CadeiadeKempeExtendido.h
 *
 *  Created on: Apr 17, 2016
 *      Author: erika
 */

#ifndef CADEIADEKEMPEEXTENDIDO_H_
#define CADEIADEKEMPEEXTENDIDO_H_

#include "Movimento.h"

#include <list>
using namespace std;

class Individuo;
class Problema;
class Alocacao;
class opAloc;
class CadeiadeKempeExtendido: public Movimento {
public:
	//Lista com todos os timeslots disponíveis para a troca do horário t1 e do horário t2
	list< opAloc* > t1;
	list< opAloc* > t2;
	
	//Lista com todos os timeslots Indisponíveis para a troca do horário t1 e do horário t2
	list< opAloc* > t1Ind;
	list< opAloc* > t2Ind;

	//Lista com todos os timeslots escolhidos para a troca do horário t1 e do horário t2
	list< opAloc* > trocast1;
	list< opAloc* > trocast2;

	//Lista com todos os movimentos montada a partir de trocast1 e trocast2
	list< Movimento* > movimentos;

	CadeiadeKempeExtendido(Problema* p, Individuo* piInd);
	CadeiadeKempeExtendido(Problema* p, Individuo* piInd, int piHorario1, int piHorario2);
	virtual ~CadeiadeKempeExtendido();
	void aplicaMovimento();

private:
	int calculaDeltaFit(Problema* p);
	int calculaDeltaFitDebug(Problema* p);
	void aplicaMoveSemRecalculoFuncaoObjetivo();
	void desaplicaMoveSemRecalculoFuncaoObjetivo();
	int KMP_preencheListaMovimentosTroca();
	int KMP_temRestricao(Alocacao* p1, Alocacao* p2);
	int ObtemMelhorPosicaoA(int* matrizConflitos[]);
	void AtualizaListaDisponiveis();

	opAloc* ObtemInicioProximaCadeiaKempe(int* matrizConflitos[], int tamConflitos, int nPosComConflitoA, int nposComConflitoB);
	int PreencheVetorCadeiaKempe(int* Conflitos[], int tamConflitos, int posInicial);

	void imprimeVetorTrocas(list< opAloc* > vet);
};

#endif /* CADEIADEKEMPEEXTENDIDO_H_ */
