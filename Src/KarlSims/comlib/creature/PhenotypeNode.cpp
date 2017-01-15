
#include "stdafx.h"
#include "PhenotypeNode.h"
#include "Joint.h"
#include "Sensor.h"
#include "Effector.h"

using namespace evc;
using namespace evc::phenotype;


cNode::cNode()
	: m_rigidBody(NULL)
{
}

cNode::~cNode()
{
	Clear();
}


bool cNode::Init(cNode *parent, const genotype::sBody &body, const Vector3 &pos)
// pos = Vector3(0,0,0)
{
	m_body = body;

	m_rigidBody = cPhysxMgr::Get()->CreateBox(
		PxTransform(Vec3toPxVec3(pos)), Vec3toPxVec3(body.dim)
		,&PxVec3(0,0,0)
		,cPhysxMgr::Get()->GetMaterial(body.mtrl)
		,body.density);

	return true;
}


bool cNode::Update(const float dtime)
{
	for (auto j : m_joints)
		j->Update(dtime);

	return true;
}


void cNode::Clear() 
{
	m_rigidBody = NULL;

	SAFE_DELETE(m_neuralNet);

	for (auto j : m_joints)
		SAFE_DELETE(j);
	m_joints.clear();

	for (auto p : m_sensors)
		SAFE_DELETE(p);
	m_sensors.clear();

	for (auto p : m_effectors)
		SAFE_DELETE(p);
	m_effectors.clear();
}


// NeuralNet is only one in Nodes
void cNode::InitNeuralNet(const vector<double> &weights) // weights = vector<double>()
{
	int count = GetNeuronCount();
	RET(count <= 0);

	//if (m_pParentJointSensor)
	//	++count;

	m_nerves.resize(count);
	m_allConnectSensor.clear();
	m_allConnectEffector.clear();
	GetAllSensor(m_allConnectSensor);
	GetAllEffector(m_allConnectEffector);

	int inputCount = 0;
	for each (auto &sensor in m_allConnectSensor)
		inputCount += sensor->GetOutputCount();

	const int outputCount = m_allConnectEffector.size();

	SAFE_DELETE(m_neuralNet);
	m_neuralNet = new cNeuralNet(inputCount, outputCount, 1, inputCount);

	if (!weights.empty())
		m_neuralNet->PutWeights(weights);
}


int cNode::GetNeuronCount() const
{
	int count = 0;
	for each(auto &joint in m_joints)
		count += joint->GetNeuronCount();
	for each (auto &sensor in m_sensors)
		count += sensor->GetOutputCount();

	return count;
}


void cNode::GetOutputNerves(OUT vector<double> &out) const
{
	for each(auto &joint in m_joints)
		joint->GetOutputNerves(out);

	for each (auto &sensor in m_sensors)
	{
		const vector<double> &output = sensor->GetOutput();
		for each (auto &val in output)
			out.push_back(val);
	}
}


void cNode::GetAllSensor(OUT vector<cSensor*> &out) const
{
	for each (auto &sensor in m_sensors)
		out.push_back(sensor);
	for each (auto &joint in m_joints)
		joint->m_actor1->GetAllSensor(out);
}


void cNode::GetAllEffector(OUT vector<cEffector*> &out) const
{
	for each (auto &effector in m_effectors)
		out.push_back(effector);
	for each (auto &joint in m_joints)
		joint->m_actor1->GetAllEffector(out);
}
