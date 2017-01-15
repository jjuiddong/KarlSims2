//
// 2016-12-18, jjuiddong
//
#pragma once

namespace genotype
{

	struct sBody;

	//--------------------------------------------------------------------------
	struct eJointType {
		enum Enum { NONE, FIXED, SPHERICAL, REVOLUTE, };
	};
	struct eShapeType {
		enum Enum { BOX, SPHERE };
	};
	struct eBodyType {
		enum Enum { DYNAMIC, KINEMATIC};
	};


	//--------------------------------------------------------------------------
	// Joint attributes
	struct sJoint
	{
		eJointType::Enum type;
		Vector4 rot; // angle,x,y,z (quaternion)
		Vector4 rotAxis; // angle,x,y,z (quaternion), revolute rotation axis
		Vector3 pos;
		Vector3 limit;
		float period; // switch reverse velocity period (milli seconds unit)
		float velocity;
		bool terminalOnly; // only create terminal node
		string linkName;
		sBody *link;

		sJoint();
		sJoint(const sJoint &rhs);
		sJoint& operator=(const sJoint &rhs);
	};


	//--------------------------------------------------------------------------
	// Body attributes
	struct sBody
	{
		string name;
		eBodyType::Enum type;
		eShapeType::Enum shape;
		Vector3 dim; // Box Shape Parameter
		float radius; // Shpere Shape Parameter
		Vector3 mtrl;
		float density;
		int depth; // generation depth, only use root body
		sBody *parent;
		vector<sJoint*> joints;

		sBody();
		sBody(const sBody &rhs);
		sBody& operator=(const sBody &rhs);
	};

}
