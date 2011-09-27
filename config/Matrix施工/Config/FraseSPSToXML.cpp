#include "StdAfx.h"
#include "FraseSPSToXML.h"
// 主交叉站标识
#define LAUX_LOGO				'X'
// 交叉站标识
#define LAU_LOGO				'A'
// 采集站标识
#define FDU_LOGO				'C'
// 电源站标识
#define LAUL_LOGO				'B'
// 爆炸机标识
#define BLAST_LOGO			'C'
// 辅助道标识
#define AUX_LOGO				'C'
// 交叉站连接线标识
#define LAUXLINE_LOGO		'|'
// 大线方向连接线标识
#define LINE_LOGO				'-'
CFraseSPSToXML::CFraseSPSToXML(void)
: m_uiIntervalLAUL(25)
, m_uiLAUXNum(1)
, m_uiBlastMachineNum(4)
, m_uiAuxNum(4)
{
}

CFraseSPSToXML::~CFraseSPSToXML(void)
{
	m_vecLine.clear();
}
BOOL CFraseSPSToXML::read_file_rec(CString strPath)
{
	CString str1,str2;
	CString str_header_type;
	CString str_line ;
	CString str_point;
	CString str_type;
	int m_list = 0;
	int flag = 0;
	m_icnt_rec = 0;
	double intvl = 0;
 	m_vecLine.clear();
	CStdioFile file;
	memset(&rec_file[0], 0, ARRAY_SIZE*sizeof(rec_file_struct));
	if (FALSE == file.Open(strPath, CFile::modeRead))
	{
		return FALSE;
	}
	while (file.GetPosition()<file.GetLength())
	{
		file.ReadString(str1);
		str2 = str1.GetAt(0);
		if (str2 == FLAG_R)
		{
			str_line = _T("");
			str_point = _T("");
			str_type = _T("");
			str_line = readbyte(1, 17, str1);
			if (_tstoi(str_line) != rec_file[m_icnt_rec].lineName)     //如果测线号有变，则数组位置递增（起始位置为1），测点号为下限
			{
				m_vecLine.push_back(0);
				m_icnt_rec++;
				rec_file[m_icnt_rec].lineName = _tstoi(str_line);
				rec_file[m_icnt_rec].type = _tstoi(readbyte(27, 28, str1));
				str_point = readbyte(17, 25, str1);
				rec_file[m_icnt_rec].point_lower = _tstof(str_point); 	            
			}    
			else                                                //否则测点号上限不断更新                                                       
			{
				str_point = readbyte(17, 25, str1);
				if (rec_file[m_icnt_rec].point_upper == 0)              //该测线上第二个点，计算间隔
				{
					rec_file[m_icnt_rec].interval = _tstof(str_point) - rec_file[m_icnt_rec].point_lower;
					rec_file[m_icnt_rec].point_upper = _tstof(str_point); 
				}
				else
				{
					intvl = _tstof(str_point) - rec_file[m_icnt_rec].point_upper;   //同测线上，间隔不等
					if (intvl != rec_file[m_icnt_rec].interval)
					{
						m_icnt_rec++;
						rec_file[m_icnt_rec].lineName = _tstoi(str_line);
						rec_file[m_icnt_rec].type = _tstoi(readbyte(27, 28, str1));
						rec_file[m_icnt_rec].point_lower = _tstof(str_point);						
					}
					else rec_file[m_icnt_rec].point_upper = _tstof(str_point); 
				}				
			}
			rec_file[m_icnt_rec].count++;
			m_vecLine[m_icnt_rec - 1] = rec_file[m_icnt_rec].count;
		}
		else if (str2 == _T('H'))
		{
			str_header_type = _T("");
			str_header_type = readbyte(1, 4, str1);
			if (str_header_type == HEADER_INSTRUMENT_TYPE)
			{
				m_str_instr_type = _T("");
				m_str_instr_type = readbyte(32, 33, str1);		
			}
			else if (str_header_type == HEADER_REC_TYPE)
			{
				m_str_rec_type = _T("");
				m_str_rec_type = readbyte(32, 34, str1);
			}
			if (str_header_type == HEADER_SHOT_TYPE)
			{
				m_str_shot_type = _T("");
				m_str_shot_type = readbyte(32, 34, str1);
			}
		}
		else break;
	}
	return TRUE;
}

BOOL CFraseSPSToXML::read_file_comm(CString strPath)
{
	CStdioFile file;
	CString str1, str2, str_header_type;
	double shot_line;
	double shot_point;
	m_icnt_comm = 0;
	memset(&comm_file[0], 0, ARRAY_SIZE*sizeof(comm_file_struct));
	if (FALSE == file.Open(strPath, CFile::modeRead))
	{
		return FALSE;
	}

	while (file.GetPosition()<file.GetLength())
	{
		file.ReadString(str1);
		str2 = str1.GetAt(0);
		if (str2 == FLAG_X)
		{
			shot_line = _tstoi(readbyte(13, 29, str1));
			shot_point = _tstof(readbyte(29, 37, str1));
			if (comm_file[m_icnt_comm].shot_lineName!=0 && comm_file[m_icnt_comm].shot_point!=0)
			{
				if (comm_file[m_icnt_comm].shot_lineName != shot_line || comm_file[m_icnt_comm].shot_point != shot_point)
				{
					m_icnt_comm++;
					comm_file[m_icnt_comm].shot_lineName = shot_line;
					comm_file[m_icnt_comm].shot_point = shot_point;

				}
			}
			else 
			{
				m_icnt_comm++;
				comm_file[m_icnt_comm].shot_lineName = shot_line;
				comm_file[m_icnt_comm].shot_point = shot_point;
			}
			comm_file[m_icnt_comm].rec_line_num ++;

			comm_file[m_icnt_comm].rec_point_index[comm_file[m_icnt_comm].rec_line_num-1]= _tstoi(readbyte(37, 38, str1));
			comm_file[m_icnt_comm].record_channel_lower[comm_file[m_icnt_comm].rec_line_num-1] = _tstoi(readbyte(38, 42, str1));
			comm_file[m_icnt_comm].record_channel_upper[comm_file[m_icnt_comm].rec_line_num-1] = _tstoi(readbyte(42, 46, str1));
			comm_file[m_icnt_comm].record_channel_interval[comm_file[m_icnt_comm].rec_line_num-1] = _tstoi(readbyte(46, 47, str1));
			comm_file[m_icnt_comm].rec_lineName[comm_file[m_icnt_comm].rec_line_num-1] = _tstoi(readbyte(47, 63, str1));
			comm_file[m_icnt_comm].rec_point_lower[comm_file[m_icnt_comm].rec_line_num-1] = _tstof(readbyte(63, 71, str1));
			comm_file[m_icnt_comm].rec_point_upper[comm_file[m_icnt_comm].rec_line_num-1] = _tstof(readbyte(71, 79, str1));	

			comm_file[m_icnt_comm].channel_lower[comm_file[m_icnt_comm].rec_line_num-1] = (int)((comm_file[m_icnt_comm].rec_point_lower[comm_file[m_icnt_comm].rec_line_num-1] - rec_file[comm_file[m_icnt_comm].rec_line_num].point_lower)/rec_file[comm_file[m_icnt_comm].rec_line_num].interval + 1);
			comm_file[m_icnt_comm].channel_upper[comm_file[m_icnt_comm].rec_line_num-1] = (int)((comm_file[m_icnt_comm].rec_point_upper[comm_file[m_icnt_comm].rec_line_num-1] - rec_file[comm_file[m_icnt_comm].rec_line_num].point_lower)/rec_file[comm_file[m_icnt_comm].rec_line_num].interval + 1);

		}
		else if (str2 == _T('H'))
		{
			str_header_type = _T("");
			str_header_type = readbyte(1, 4, str1);
			if (str_header_type == HEADER_INSTRUMENT_TYPE)
			{
				m_str_instr_type = _T("");
				m_str_instr_type = readbyte(32, 33, str1);		
			}
			else if (str_header_type == HEADER_REC_TYPE)
			{
				m_str_rec_type = _T("");
				m_str_rec_type = readbyte(32, 34, str1);
			}
			if (str_header_type == HEADER_SHOT_TYPE)
			{
				m_str_shot_type = _T("");
				m_str_shot_type = readbyte(32, 34, str1);
			}
		}
		else break;
	}
	return TRUE;   
}


//解析sps文件，返回string
CString CFraseSPSToXML::readbyte(int ifrm_byte, int ito_byte, CString str)
{
	CString str1,str2;
	str1 = _T("");
	for (int i = ifrm_byte; i<ito_byte; i++)
	{
		str2 = str.GetAt(i);
		if(str2 != _T(" "))
		{
			str1 += str2;
		}
	}
	return str1;
}


//打开xml文件
BOOL CFraseSPSToXML::OpenMatrixIniXMLFile(CString strPath)
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = _T("msxml2.domdocument");
	if (FALSE == m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError))
	{
		AfxMessageBox(_T("Create XML dispatch error!"));
		return FALSE;
	}
	oVariant = strPath;
	if (FALSE == m_oXMLDOMDocument.load(oVariant))
	{
		AfxMessageBox(_T("Load Matrix XML file error!"));
		return FALSE;
	}
	return TRUE;
}

//关闭xml文件
void CFraseSPSToXML::CloseMatrixIniXMLFile(void)
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

//保存xml文件
void CFraseSPSToXML::SaveMatrixIniXMLFile(CString strPath)
{
	COleVariant oVariant;
	oVariant = strPath;
	m_oXMLDOMDocument.save(oVariant);
}

//保存xml文件各项，先全部删除，再增加节点
void CFraseSPSToXML::OnSave_rec(void)
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = _T("SurverySetup");
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = _T("Count");
	oVariant = (long)m_icnt_rec;
	oElementParent.setAttribute(strKey, oVariant);
	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}
	// 增加新节点
	for(int i = 1; i < m_icnt_rec + 1; i++)
	{
		lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t\t"));
		oElementParent.appendChild(lpDispatch);
		lpDispatch = m_oXMLDOMDocument.createElement(_T("Record"));
		oElementChild.AttachDispatch(lpDispatch);
		AddToXML_rec(&oElementChild, &rec_file[i]);
		oElementParent.appendChild(lpDispatch);

		if(i == m_icnt_rec)
		{
			lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t"));
			oElementParent.appendChild(lpDispatch);
		}		
	}
}
//增加xml文件节点
void CFraseSPSToXML::AddToXML_rec(CXMLDOMElement* pElement, rec_file_struct *ptr_rec_file)
{
	CString strKey;	// 键名
	COleVariant oVariant;
	CString str;

	strKey = _T("LineName");
	oVariant = (long)ptr_rec_file->lineName;
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("ReceiverSection");
	str.Format(_T("%d-%d%s%d"), 1, ptr_rec_file->count, _T("p"), ptr_rec_file->type);
	oVariant = str;
	pElement->setAttribute(strKey, oVariant);

}

void CFraseSPSToXML::AddToXML_comm(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt, int cnt_line)       //外从1开始，里从0开始
{
	CString strKey;	// 键名
	COleVariant oVariant;
	CString str;

	strKey = _T("Nb");
	oVariant = (long)(cnt_line + 1);
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("Label");
	str.Format(_T("%s%d"), _T("Absolute"),cnt_line + 1);
	oVariant = str;
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("Spread");
	str.Format(_T("%d:%d-%d"), ptr_comm_file->rec_lineName[cnt_line], ptr_comm_file->channel_lower[cnt_line], ptr_comm_file->channel_upper[cnt_line]);
	oVariant = str;
	pElement->setAttribute(strKey, oVariant);
}

//保存xml文件各项，先全部删除，再增加节点
void CFraseSPSToXML::OnSave_comm(void)
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = _T("AbsoluteSetup");
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = _T("Count");
	oVariant = (long)m_icnt_comm;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}

	for (int j = 1; j < m_icnt_comm + 1; j++)
	{

		lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t\t"));  //无尖括号
		oElementParent.appendChild(lpDispatch);

		strKey.Format(_T("%s%d"), _T("AbsoluteSetup"), j);
		oElementChild = m_oXMLDOMDocument.createElement(strKey);
		strKey = _T("Count");
		oVariant = (long)comm_file[j].rec_line_num;
		oElementChild.setAttribute(strKey, oVariant);
		oElementParent.appendChild(oElementChild);
		strKey = _T("TabCount");
		oVariant = _T("4");
		oElementChild.setAttribute(strKey, oVariant);
		oElementParent.appendChild(oElementChild);
		strKey = _T("ShotPoint");
		oVariant = comm_file[j].shot_point;
		oElementChild.setAttribute(strKey, oVariant);
		oElementParent.appendChild(oElementChild);

		oElementParent = oElementChild;

		// 增加新节点
		for(int i = 0; i < comm_file[j].rec_line_num; i++)
		{
			lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t\t\t"));
			oElementParent.appendChild(lpDispatch);

			lpDispatch = m_oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);
			AddToXML_comm(&oElementChild, &comm_file[j], j, i);
			oElementParent.appendChild(lpDispatch);

			if(i == comm_file[j].rec_line_num)
			{
				lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t\t"));
				oElementParent.appendChild(lpDispatch);
			}		
		}
		oElementParent.appendChild(m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t\t")));  

		oElementParent = oElementParent.get_parentNode();
	}
	lpDispatch = m_oXMLDOMDocument.createTextNode(_T("\n\t\t\t"));  //无尖括号
	oElementParent.appendChild(lpDispatch);
}


void CFraseSPSToXML::AddToXML_shot(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt)  
{
	CString strKey;	// 键名
	COleVariant oVariant;
	CString str;

	strKey = _T("VPStatus");
	oVariant = _T("0");
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("ShotID");
	oVariant = (long)cnt;
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("BreakPoint");
	oVariant = _T("N");
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SwathNb");
	oVariant = _T("1");
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SourcePointIndex");
	oVariant = (long)ptr_comm_file->rec_point_index[0];
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SourceLine");
	oVariant = ptr_comm_file->shot_lineName;
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SourceReceiver");
	oVariant = ptr_comm_file->shot_point;
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SpreadSFL");
	oVariant = (long)ptr_comm_file->rec_lineName[0];
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SpreadSFN");
	oVariant = (long)ptr_comm_file->channel_lower[0];
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("SpreadType");
	oVariant = _T("1");
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("ProcessTypeID");
	oVariant = _T("1");
	pElement->setAttribute(strKey, oVariant);

	strKey = _T("Comment");
	oVariant = _T("注释");
	pElement->setAttribute(strKey, oVariant);

}

void CFraseSPSToXML::OnSave_shot(void)
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = _T("OperationTable");
	lpDispatch = m_oXMLDOMDocument_operation.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = _T("Count");
	oVariant = (long)m_icnt_comm;
	oElementParent.setAttribute(strKey, oVariant);
	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}
	// 增加新节点
	for(int i = 1; i < m_icnt_comm + 1; i++)
	{
		lpDispatch = m_oXMLDOMDocument_operation.createTextNode(_T("\n\t\t\t\t"));
		oElementParent.appendChild(lpDispatch);
		lpDispatch = m_oXMLDOMDocument_operation.createElement(_T("Record"));
		oElementChild.AttachDispatch(lpDispatch);
		AddToXML_shot(&oElementChild, &comm_file[i], i);
		oElementParent.appendChild(lpDispatch);

		if(i == m_icnt_comm)
		{
			lpDispatch = m_oXMLDOMDocument_operation.createTextNode(_T("\n\t\t\t"));
			oElementParent.appendChild(lpDispatch);
		}		
	}  
}


//打开xml文件
BOOL CFraseSPSToXML::OpenMatrixIniXMLFile_operation(CString strPath)
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = _T("msxml2.domdocument");
	if (FALSE == m_oXMLDOMDocument_operation.CreateDispatch(strOLEObject, &oError))
	{
		AfxMessageBox(_T("Create Operation XML dispatch error!"));
		return FALSE;
	}
	oVariant = strPath;
	if (FALSE == m_oXMLDOMDocument_operation.load(oVariant))
	{
		AfxMessageBox(_T("Load Operation XML file error!"));
		return FALSE;
	}
	return TRUE;
}

//关闭xml文件
void CFraseSPSToXML::CloseMatrixIniXMLFile_operation(void)
{
	m_oXMLDOMDocument_operation.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

//保存xml文件
void CFraseSPSToXML::SaveMatrixIniXMLFile_operation(CString strPath)
{
	COleVariant oVariant;
	oVariant = strPath;
	m_oXMLDOMDocument_operation.save(oVariant);
}
// 载入SPS文件
BOOL CFraseSPSToXML::LoadSPSFile(CString strRFilePath, CString strXFilePath)
{
	if (FALSE == read_file_rec(strRFilePath))
	{
		return FALSE;
	}
	if (FALSE == read_file_comm(strXFilePath))
	{
		return FALSE;
	}
	return TRUE;
}

// 保存修改到测线XML文件
void CFraseSPSToXML::SaveLineXML(CString strPath)
{
	// 打开配置文件
	if (FALSE == OpenMatrixIniXMLFile(strPath))
	{
		return;
	}
	// 保存FormLine设置
	OnSave_rec();
	OnSave_comm();
	// 保存配置文件
	SaveMatrixIniXMLFile(strPath);
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

// 保存修改到施工XML
void CFraseSPSToXML::SaveOperationXML(CString strPath)
{
	if (FALSE == OpenMatrixIniXMLFile_operation(strPath))
	{
		return;
	}
	// 保存FormLine设置
	OnSave_shot();
	// 保存配置文件
	SaveMatrixIniXMLFile_operation(strPath);
	// 关闭配置文件
	CloseMatrixIniXMLFile_operation();
}

// 生成测线仪器排列文件
void CFraseSPSToXML::GenLineInitTXT(CString strPath)
{
	CFile file;
	unsigned int uiMax = OnCalMax();
	unsigned int uiLAULNum = 0;		// 电源站个数
	unsigned int uiLineNum = m_vecLine.size();
	unsigned int uiCharSize = 0;
	unsigned int uiPosition = 0;
	if (uiMax == 0)
	{
		return;
	}
	if (uiLineNum == 0)
	{
		return;
	}
	if (uiMax < (m_uiLAUXNum + m_uiBlastMachineNum + m_uiLAUXNum))
	{
		uiMax = m_uiLAUXNum + m_uiBlastMachineNum + m_uiLAUXNum;
	}
	uiLAULNum = (uiMax / 2) / (m_uiIntervalLAUL + 1) + (uiMax - uiMax / 2) / (m_uiIntervalLAUL + 1);
	unsigned int uiLineCount = (uiLineNum + 1) * 2 - 1;
	unsigned int uiColumnCount = (m_uiLAUXNum + uiLAULNum + uiMax) * 2 - 1 + 2;
	uiCharSize = uiLineCount * uiColumnCount;
	unsigned char * pWriteChar = new unsigned char [uiCharSize];
	unsigned char ucInit = ' ';
	memset(pWriteChar, ucInit, uiCharSize);
	// 回车换行赋值
	for (unsigned int i=0; i<uiLineCount; i++)
	{
		uiPosition = (i + 1) * uiColumnCount - 2;
		pWriteChar[uiPosition] = '\r';
		uiPosition = (i + 1) * uiColumnCount - 1;
		pWriteChar[uiPosition] = '\n';
	}
	// 交叉站及交叉线赋值
	for (unsigned int i=0; i<uiLineCount; i++)
	{
		uiPosition = (uiMax - uiMax / 2 + (uiMax - uiMax / 2) / (m_uiIntervalLAUL + 1)) * 2 + i * uiColumnCount;
		if (i == 0)
		{
			pWriteChar[uiPosition] = LAUX_LOGO;
		}
		else if (i % 2 == 1)
		{
			pWriteChar[uiPosition] = LAUXLINE_LOGO;
		}
		else
		{
			pWriteChar[uiPosition] = LAU_LOGO;
		}
	}
	// 左侧爆炸机赋值
	uiPosition = (uiMax - uiMax / 2 + (uiMax - uiMax / 2) / (m_uiIntervalLAUL + 1)) * 2;
	for (unsigned int i=0; i<m_uiBlastMachineNum; i++)
	{
		pWriteChar[uiPosition - i * 2 - 1] = LINE_LOGO;
		pWriteChar[uiPosition - i * 2 - 2] = BLAST_LOGO;
	}
	// 右侧辅助道赋值
	uiPosition = (uiMax - uiMax / 2 + (uiMax - uiMax / 2) / (m_uiIntervalLAUL + 1)) * 2;
	for (unsigned int i=0; i<m_uiAuxNum; i++)
	{
		pWriteChar[uiPosition + i * 2 + 1] = LINE_LOGO;
		pWriteChar[uiPosition + i * 2 + 2] = AUX_LOGO;
	}
	unsigned int uiFDUNumLeft = 0;		// 左侧采集站个数
	unsigned int uiFDUNumRight = 0;		// 右侧采集站个数
	// 采集站及电源站赋值
	for (unsigned int i=2; i<uiLineCount; i++)
	{
		if (i % 2 == 0)
		{
			uiPosition = (uiMax - uiMax / 2 + (uiMax - uiMax / 2) / (m_uiIntervalLAUL + 1)) * 2 + i * uiColumnCount;
			uiFDUNumRight = m_vecLine[(i - 2) / 2] / 2;
			uiFDUNumLeft = m_vecLine[(i - 2) / 2] - uiFDUNumRight;
			// 左侧采集站及电源站赋值
			for (unsigned int j=0; j<(uiFDUNumLeft + uiFDUNumLeft / (m_uiIntervalLAUL + 1)); j++)
			{
				pWriteChar[uiPosition - j * 2 - 1] = LINE_LOGO;
				if (((j + 1) % (m_uiIntervalLAUL + 1) == 0) && (j != 0))
				{
					pWriteChar[uiPosition - j * 2 - 2] = LAUL_LOGO;
				}
				else
				{
					pWriteChar[uiPosition - j * 2 - 2] = FDU_LOGO;
				}
			}
			// 右侧采集站及电源站赋值
			for (unsigned int j=0; j<(uiFDUNumRight + uiFDUNumRight / (m_uiIntervalLAUL + 1)); j++)
			{
				pWriteChar[uiPosition + j * 2 + 1] = LINE_LOGO;
				if (((j + 1) % (m_uiIntervalLAUL + 1) == 0) && (j != 0))
				{
					pWriteChar[uiPosition + j * 2 + 2] = LAUL_LOGO;
				}
				else
				{
					pWriteChar[uiPosition + j * 2 + 2] = FDU_LOGO;
				}
			}
		}	
	}
	if (FALSE == file.Open(strPath, CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("Create LineInit file error, Please check it again!"));
		return;
	}
	// 写文件内容
	file.Write(pWriteChar, uiCharSize);
	file.Close();
	delete[] pWriteChar;
}

// 计算最大值并返回
unsigned int CFraseSPSToXML::OnCalMax(void)
{
	unsigned int uiMax = 0;
	for (vector <unsigned int>::size_type i=0; i<m_vecLine.size(); i++)
	{
		if (m_vecLine[i] >= uiMax)
		{
			uiMax = m_vecLine[i];
		}
	}
	return uiMax;
}
