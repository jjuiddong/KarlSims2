//
// 2013 - 12 - 19, jjuiddong
//		detect creature, obstacle using sensor direction
//
// 2016-12-21
//		Refactoring
//
#pragma once

#include "Sensor.h"


class RenderBaseActor;
namespace evc {
	namespace phenotype {

		class cNode;
		class cVisionSensor : public cSensor
		{
		public:
			cVisionSensor();
			virtual ~cVisionSensor();
			virtual const vector<double>& GetOutput() override;
			void SetSensorInfo(cNode *sensorBody, const float length);


		private:
			cNode *m_pSensorBody;
			//RenderBaseActor *m_pSensorRenderActor;
			float m_DetectLength;
		};

	}
}