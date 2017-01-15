//
// 2013-12-10, jjuiddong
//		Effector
//
// 2016-12-21
//		Update
// 
#pragma once


namespace evc {
	namespace phenotype {

	class cEffector
	{
	public:
		cEffector(const double period) : m_Period(period), m_ElapseT(0) {}
		virtual ~cEffector() {}
		virtual void Signal(const double dtime, const double signal) = 0;
		void SetPeriod(const double period);


	protected:
		double m_Period;
		double m_ElapseT;
		double m_Signal;
	};


	inline void cEffector::SetPeriod(const double period) { m_Period = period; }
	}
}

