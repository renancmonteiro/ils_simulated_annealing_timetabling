/*
 * Timeslot.cpp
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#include "Timeslot.h"

#include <stdio.h>

Timeslot::Timeslot(int d, int h, int nDias) {
	dia = d;
	periodo = h;
	horario = d*nDias + h;
}

void Timeslot::imprime(){
	printf("%d %d ( %d )\n", dia, periodo, horario);
}
