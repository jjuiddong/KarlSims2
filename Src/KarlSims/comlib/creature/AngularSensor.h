//
// 2013-12-10, jjuiddong
//	joint angle detect
//
// 2016-12-21
//	update
//
#pragma once

#include "Sensor.h"


namespace evc {
	namespace phenotype {

		class cAngularSensor : public cSensor
		{
		public:
			cAngularSensor() : cSensor(), m_pJoint(NULL) { }
			virtual ~cAngularSensor() {}
			void SetJoint(PxJoint *joint);
			virtual const vector<double>& GetOutput() override;


		private:
			PxJoint *m_pJoint;
		};


		inline void cAngularSensor::SetJoint(PxJoint *joint) { m_pJoint = joint; }

		inline const vector<double>& cAngularSensor::GetOutput()
		{
			RETV(!m_pJoint, m_Output);
			const PxQuat q = m_pJoint->getRelativeTransform().q;
			double angle = q.getAngle();

			// scale 0 ~ 1.f
			angle = angle / (PxPi / 2.f) * 2.f;
			if (angle >= 1.f)
				angle = 1.f;

			if (q.x < 0)
				angle = -angle;

			m_Output[0] = angle;
			return m_Output;
		}

	}
}
