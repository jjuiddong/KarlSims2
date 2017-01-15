//
// 2013-12-19, jjuiddong
//	  detect  light
//
// 2016-12-21
//		- Refactoring
//
#pragma once

#include "PhenotypeNode.h"
#include "Sensor.h"

class RenderBaseActor;

namespace evc {
	namespace phenotype {

		class cPhotoSensor : public cSensor
		{
		public:
			cPhotoSensor();
			virtual ~cPhotoSensor();
			virtual const vector<double>& GetOutput() override;
			void SetSensorInfo(cNode *sensorBody, const PxVec3 &dir, const float length);


		private:
			cNode *m_pSensorBody;
			//RenderBaseActor *m_pSensorRenderActor;
			float m_DetectLength;
		};

	}
}
