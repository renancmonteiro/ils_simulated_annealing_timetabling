#ifndef ILS_H_
#define ILS_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "BuscaLocal/SimulatedAnnealing.h"
#include "Model/Individuo.h"
#include "Model/Problema.h"
#include "GeraSolucaoInicialGulosa.h"
#include "Pertubacao.h"

class Problema;

class ILS {
public:
	int nMaxIter;
	int nMaxIterSemMelhora;

	int seed;
    int mediaSolucoes;
    int soft1, soft2, soft3, soft4;
    int f1, f2;

    int info, infoSD;
    int imprimeGrafico;
    int imprimeFO;

    //Parametros da Busca Local
    int nMaxIterBuscaLocal;

    //Parametros do SA
	double temperaturaInicial, temperaturaFinal;
	double taxaDecaimentoTemperatura;
	int aceitaPioraSA;

    double tempoLimite;
    double tempoExecucao;
	int iteracoesExecutadas;
	int nMovimentosRealizados;
	int nMoves, nSwaps, nTimeMoves, nRoomMoves;
	int nMovesValidos, nSwapsValidos, nTimeMovesValidos, nRoomMovesValidos;
	int nMovesMelhora, nSwapsMelhora, nTimeMovesMelhora, nRoomMovesMelhora;
	int nMovesInvalido, nSwapsInvalido, nTimeMovesInvalido, nRoomMovesInvalido;

	ILS();

	Individuo* executa(Problema* p);
	void ImprimeExecucao();
	void ImprimeExecucaoResumida();

private:
	Individuo* executaSimulatedAnnealing(Problema* p, Individuo* indInicial, int vizinhanca);
	Individuo* indC;
};

#endif
