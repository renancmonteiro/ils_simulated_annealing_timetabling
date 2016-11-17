/*
 * Professor.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: erika
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Professor.h"
#include "Alocacao.h"
#include "Disciplina.h"

Professor::Professor(int piNHorarios, int piNSalas, int i, char* piNome) {
	numeroSequencial = i;
	nHorarios = piNHorarios;
	nSalas = piNSalas;
	strcpy(nome, piNome);
}

Professor::~Professor() {}
