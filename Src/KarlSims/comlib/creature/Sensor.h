/**
 @filename Sensor.h
 
 sensor interface 
*/
#pragma once


namespace evc {
	namespace phenotype {

		class cSensor
		{
		public:
			cSensor(const int outputCount = 1)
				: m_Output(0, 0), m_Dir(0, 0, 0), m_OutputCount(outputCount)
			{
				m_Output.resize(outputCount);
			}

			virtual ~cSensor() {}
			virtual const vector<double>& GetOutput() = 0;
			const int GetOutputCount() const;


		protected:
			vector<double> m_Output;
			int m_OutputCount;
			PxVec3 m_Dir;
		};


		inline const int cSensor::GetOutputCount() const { return m_OutputCount; }
	}
}