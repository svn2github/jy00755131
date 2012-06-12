#include "StdAfx.h"
#include "data.base.h"

namespace matrix_data{

bool operator==(const device_limit& left,	const device_limit& right)
{
	bool is_equal = false;
	if(left.m_fLimit == right.m_fLimit &&
		left.m_uiNb == right.m_uiNb &&
		left.m_uiTestAim == right.m_uiTestAim &&
		left.m_uiTestType == right.m_uiTestType &&
		StrCmp(left.m_pcUnit, right.m_pcUnit) == 0 &&
		StrCmp(left.m_pcDescr, right.m_pcDescr) == 0) is_equal = true;

	return is_equal;
}

bool operator==(const sensor_info& left,	const sensor_info& right)
{
	bool is_equal = false;
	if(left.type_ == right.type_) is_equal = true;
	return is_equal;
}

 bool operator==(const sensor_type& left,	const sensor_type& right)
 {
	 bool is_equal = false;
	 if(left.nb_ == right.nb_)	is_equal = true;
	 return is_equal;
 }

bool operator==(const device_info& left,	const device_info& right)
{
	bool is_equal = false;
	if(left.sn_ == right.sn_)	is_equal = true;
	return is_equal;
}

data_base::data_base(void)
{
	
}

data_base::~data_base(void)
{
}
}