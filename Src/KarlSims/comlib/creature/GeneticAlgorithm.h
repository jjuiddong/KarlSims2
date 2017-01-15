// 2013-12-13
//		- Mat Buckland 2002 Idea
//
// 2016-12-22, jjuiddong
//		- Refactoring
//
#pragma once

#include "genome.h"


namespace evc
{
	class cGeneticAlgorithm : public common::cSingleton<cGeneticAlgorithm>
	{
	public:
		cGeneticAlgorithm();
		virtual ~cGeneticAlgorithm();
		void InitGenome();
		void AddGenome(const sGenome &genome);
		const vector<sGenome>& GetGenomes() const;
		void Epoch();


	protected:
		sGenome GetChromoRoulette();
		void GrabNBest(int NBest, const int NumCopies, vector<sGenome> &Pop);


	private:
		vector<sGenome> m_Genomes;
	};


	inline const vector<sGenome>& cGeneticAlgorithm::GetGenomes() const { return m_Genomes; }
}
