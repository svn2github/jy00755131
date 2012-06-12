#pragma once


// range_static
#include <list>


class range_static : public CStatic
{
	DECLARE_DYNAMIC(range_static)

public:
	range_static();
	virtual ~range_static();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	std::list<CStatic*> static_list;
	std::list<CEdit*> edit_list;

};


