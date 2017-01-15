//
// 2016-12-21, jjuiddong
// creature class
//
#pragma once

#include "GenotypeParser.h"

namespace evc
{
	namespace phenotype {
		class cNode;
		class cJoint;
	}

	class cCreature
	{
	public:
		cCreature();
		virtual ~cCreature();

		bool Init(const string &genotypeFileName, const Vector3 &pos = Vector3(0, 0, 0));
		bool Update(const float dtime);
		void Clear();


	protected:
		bool CreatePhenotype(const Vector3 &pos);

		phenotype::cNode* CreatePhenotypeNode(phenotype::cNode *parent,
			const genotype::sBody &body, const genotype::sJoint *joint = NULL,
			const Vector3 &pos = Vector3(0, 0, 0), const int depth = 0);

		phenotype::cNode* CreateTerminalNode(phenotype::cNode *parent,
			const genotype::sBody &body);

		PxRigidDynamic* CreateBody(phenotype::cNode *parent, const genotype::sBody &body
			, const genotype::sJoint * joint = NULL, const Vector3 &pos = Vector3(0, 0, 0)
			, const int depth = 0);

		void CreateSkinningMesh();
		void CreateRenderComposition(phenotype::cNode *node);


	public:
		genotype::cParser m_parser;
		phenotype::cNode *m_phRoot;
		vector<phenotype::cNode*> m_nodes;
		int m_maxGenerationDepth;

		// rendering
		vector<PxTransform> m_tmPalette;
	};

}
