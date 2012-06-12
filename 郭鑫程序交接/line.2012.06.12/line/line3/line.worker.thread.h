#pragma once

#include "line.socket.impl.h"

// line_worker_thread
class line_worker_thread : public CWinThread
{
	DECLARE_DYNCREATE(line_worker_thread)

protected:
	line_worker_thread();           // protected constructor used by dynamic creation
	virtual ~line_worker_thread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void login_server_resp( WPARAM w_param, LPARAM l_param);

private:
	CEvent*  complete_token_event_;

public:
	virtual int Run();
};


