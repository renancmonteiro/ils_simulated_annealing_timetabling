#ifndef PERTUBACAO_H_
#define PERTUBACAO_H_

#include "Model/Individuo.h"
#include "Model/Problema.h"
#include "Vizinhancas/Movimento.h"
#include "Vizinhancas/CadeiadeKempeExtendido.h"
#include <stdio.h>

class Pertubacao {
	public:
		Pertubacao(Problema *p, Individuo *ind);
		~Pertubacao();

		Individuo* executa();
		int deltaFit;
	private:
		Problema *p;
		Individuo *ind;
		Movimento *mov;
};

#endif