#include "Pertubacao.h"

Pertubacao::Pertubacao(Problema *p, Individuo *ind){
	this->p = p;
	this->ind = ind;

	this->mov = new CadeiadeKempeExtendido(this->p, this->ind);

	while(this->mov->deltaHard >= 0 && this->mov->deltaFit >= 10000){
		delete(this->mov);
		this->mov = new CadeiadeKempeExtendido(this->p, this->ind);
	}

	this->deltaFit = this->mov->deltaFit;
}

Pertubacao::~Pertubacao(){
	delete(p);
	delete(ind);
	delete(mov);
}

Individuo* Pertubacao::executa(){
	this->mov->aplicaMovimento();

	return this->mov->ind;
}