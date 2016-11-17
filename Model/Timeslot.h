/*
 * Timeslot.h
 *
 *  Created on: May 8, 2015
 *      Author: erika
 */

#ifndef TIMESLOT_H_
#define TIMESLOT_H_

class Timeslot {
public:
	int dia;
	int horario;
	int periodo; //dia + horario

	Timeslot(int d, int h, int nDias);
	void imprime();
};

#endif /* TIMESLOT_H_ */
