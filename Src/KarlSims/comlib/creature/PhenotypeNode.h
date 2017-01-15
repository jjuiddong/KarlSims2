//
//2013-12-10, jjuiddong
//		- Phenotype Node
//
// 2016-12-19
//		- Refactoring
//
#pragma once

#include "Genotype.h"
#include "NeuralNet.h"


namespace evc { namespace phenotype {

	class cJoint;
	class cSensor;
	class cEffector;

	class cNode
	{
	public:
		cNode();
		virtual ~cNode();
		bool Init(cNode *parent, const genotype::sBody &body, const Vector3 &pos=Vector3(0,0,0));
		void InitNeuralNet(const vector<double> &weights = vector<double>());
		bool Update(const float dtime);
		void Clear();

		// NeuralNet
		void GetOutputNerves(OUT vector<double> &out) const;
		int GetNeuronCount() const;
		void GetAllSensor(OUT vector<cSensor*> &out) const;
		void GetAllEffector(OUT vector<cEffector*> &out) const;


	public:
		// Physical Attributes
		genotype::sBody m_body;
		PxRigidDynamic *m_rigidBody;
		vector<cJoint*> m_joints;
		vector<cSensor*> m_sensors;
		vector<cEffector*> m_effectors;

		// Rendering
		RenderComposition *m_pShapeRenderer;
		RenderComposition *m_pOriginalShapeRenderer;
		int m_PaletteIndex; // tm palette index

		// NeuralNet
		cNeuralNet *m_neuralNet;
		vector<cSensor*> m_allConnectSensor;
		vector<cEffector*> m_allConnectEffector;
		vector<double> m_nerves;
	};

	} // phenotype
} // evc
