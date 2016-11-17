#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Model/Problema.h"
#include "Model/Individuo.h"
#include "Ils.h"

using namespace std;

void configuraParametros(ILS *ils, int piNParam, char **piParams) {
	int i;

	for (i = 2; i < piNParam; i++) {
		if (strncmp("seed", piParams[i], 4) == 0) {
			ils->seed = atoi(piParams[i] + 5);
		}
		else if (strncmp("n", piParams[i], 1) == 0) {
			ils->nMaxIterSemMelhora = atoi(piParams[i] + 2);
//			ils->nMaxIterBuscaLocal = 120000;
		}
		else if (strncmp("info", piParams[i], 4) == 0) {
			ils->info = atoi(piParams[i] + 5);
		}
		else if (strncmp("t0", piParams[i], 2) == 0) {
			ils->temperaturaInicial = atof(piParams[i] + 3);
		}
		else if (strncmp("tFinal", piParams[i], 6) == 0) {
			ils->temperaturaFinal = atof(piParams[i] + 7);
		}
		else if (strncmp("beta", piParams[i], 4) == 0) {
			ils->taxaDecaimentoTemperatura = atof(piParams[i] + 5);
		}
		else if (strncmp("grafico", piParams[i], 7) == 0) {
			ils->imprimeGrafico = atoi(piParams[i] + 8);
		} else if (strncmp("timeout", piParams[i], 7) == 0){
			ils->tempoLimite = atoi(piParams[i] + 8);
		}

	}
	srand(ils->seed);
}

int main(int argc, char** argv) {
	Problema* p;
	Individuo* best;
	ILS* ils;

	if( argc < 2 ){
		fprintf(stderr, "Modo de executar:\n\n");
		fprintf(stderr, "\t./ils arquivo_problema_entrada arquivo_saida [parametros]\n\n\tonde os parametros podem ser:\n");
		fprintf(stderr, "\t\tseed=VALOR_SEED (recebe time(NULL) caso nao informado)\n");
		fprintf(stderr, "\t\tn=NUMERO_MAXIMO_ITERACOES_SEM_MELHORA\n");
		fprintf(stderr, "\t\tinfo=1 para imprimir debug\n");
		fprintf(stderr, "\t\tgrafico=1 para imprimir informacoes para geracao de grafico\n\n");

		exit(0);
	}

	p = new Problema(argv[1]);
	ils = new ILS();
	configuraParametros(ils, argc, argv);

	best = ils->executa(p);
	ils->ImprimeExecucao();
	best->ImprimeIndividuo();

	delete(ils);
	delete(best);
	delete(p);

	return 0;
}
