// 2013-12-13, jjuiddong
//		Genome class
//
//	2016-12-22, jjuiddong
//		Refactoring
//
#pragma once

#include "Genotype.h"

namespace evc
{
	class cCreature;

	struct sGenome
	{
		double fitness;
		vector<double> chromo;

		sGenome() : fitness(0) {}
		sGenome( const vector<double> &w, double f ): chromo(w), fitness(f) {}

		//overload '<' used for sorting
		friend bool operator<(const sGenome& lhs, const sGenome& rhs) {
			return (lhs.fitness < rhs.fitness);
		}

		const sGenome& operator=(const sGenome &rhs) {
			if (this != &rhs)
			{
				fitness = rhs.fitness;
				chromo = rhs.chromo;
			}
			return *this;
		}
	};


	bool GetChromo(const cCreature *creature, const genotype::sBody *body, OUT vector<double> &chromo);
	genotype::sBody* BuildBody(const vector<double> &chromo, OUT vector<double> &weights);
	void Crossover(const vector<double> &mum, const vector<double> &dad, OUT vector<double> &baby1, OUT vector<double> &baby2);
	void Mutate(INOUT vector<double> &chromo);
}
