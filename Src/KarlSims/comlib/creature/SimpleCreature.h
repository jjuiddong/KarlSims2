//
// 2016-12-19, jjuiddong
// simple generate creature with genotype
//
#pragma once

#include "GenotypeParser.h"

namespace evc
{
	namespace phenotype { 
		class cNode; 
		class cJoint;
	}

	class cSimpleCreature
	{
	public:
		cSimpleCreature();
		virtual ~cSimpleCreature();

		bool Init(const string &genotypeFileName, const Vector3 &pos=Vector3(0,0,0));
		bool Update(const float dtime);
		void Clear();


	protected:
		bool CreatePhenotype(const Vector3 &pos);

		phenotype::cNode* CreatePhenotypeNode(phenotype::cNode *parent, 
			const genotype::sBody &body, const genotype::sJoint *joint=NULL,
			const Vector3 &pos = Vector3(0, 0, 0), const int depth=0);

		PxRigidDynamic* CreateBody(phenotype::cNode *parent, const genotype::sBody &body
			, const genotype::sJoint * joint = NULL, const Vector3 &pos = Vector3(0, 0, 0)
			, const int depth = 0);


	protected:
		genotype::cParser m_parser;
		phenotype::cNode *m_phRoot;
		int m_maxGenerationDepth;
	};

}
