// DlgSetInstrument.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "DlgSetInstrument.h"
// 定义仪器类型
// 交叉站
#define Instrument_Type_LAUX		1
// 电源站
#define Instrument_Type_LAUL		2
// 采集站
#define Instrument_Type_FDU			3
// 交叉站标识
#define LAU_LOGO				'A'
// 采集站标识
#define FDU_LOGO				'C'
// 电源站标识
#define LAUL_LOGO				'B'
// 设置仪器的操作类型
// 在该仪器之前加入
#define SetInstrument_Opt_AddPrevious	1
// 在该仪器之后加入
#define SetInstrument_Opt_AddNext		2
// 修改该仪器类型
#define	SetInstrument_Opt_Change			3
// 删除该仪器
#define SetInstrument_Opt_Delete				4
// CDlgSetInstrument 对话框

IMPLEMENT_DYNAMIC(CDlgSetInstrument, CDialog)

CDlgSetInstrument::CDlgSetInstrument(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetInstrument::IDD, pParent)
	, m_iSelectUnitColumnIndex(0)
	, m_iSelectUnitRowIndex(0)
	, m_uiInstrumentType(0)
	, m_uiColumnCount(0)
	, m_uiRowCount(0)
{

}

CDlgSetInstrument::~CDlgSetInstrument()
{
}

void CDlgSetInstrument::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_TYPE, m_ctrlInstrumentType);
}


BEGIN_MESSAGE_MAP(CDlgSetInstrument, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PREVIOUS, &CDlgSetInstrument::OnBnClickedButtonAddPrevious)
	ON_BN_CLICKED(IDC_BUTTON_ADD_NEXT, &CDlgSetInstrument::OnBnClickedButtonAddNext)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSetInstrument::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSetInstrument::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CDlgSetInstrument 消息处理程序
BOOL CDlgSetInstrument::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_uiInstrumentType == Instrument_Type_LAUX)
	{
		m_ctrlInstrumentType.SetCurSel(0);
	} 
	else if (m_uiInstrumentType == Instrument_Type_LAUL)
	{
		m_ctrlInstrumentType.SetCurSel(1);
	}
	else if (m_uiInstrumentType == Instrument_Type_FDU)
	{
		m_ctrlInstrumentType.SetCurSel(2);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSetInstrument::OnBnClickedButtonAddPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int uiInstrumentType = GetCurInstrumentType();
	if (uiInstrumentType == 0)
	{
		return;
	}
	OnSetInstrumentToFile(uiInstrumentType, SetInstrument_Opt_AddPrevious);
}

void CDlgSetInstrument::OnBnClickedButtonAddNext()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int uiInstrumentType = GetCurInstrumentType();
	if (uiInstrumentType == 0)
	{
		return;
	}
	OnSetInstrumentToFile(uiInstrumentType, SetInstrument_Opt_AddNext);
}

void CDlgSetInstrument::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int uiInstrumentType = GetCurInstrumentType();
	if (uiInstrumentType == 0)
	{
		return;
	}
	OnSetInstrumentToFile(uiInstrumentType, SetInstrument_Opt_Change);
}

void CDlgSetInstrument::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int uiInstrumentType = GetCurInstrumentType();
	if (uiInstrumentType == 0)
	{
		return;
	}
	OnSetInstrumentToFile(uiInstrumentType, SetInstrument_Opt_Delete);
}
// 得到当前待操作仪器的仪器类型
unsigned int CDlgSetInstrument::GetCurInstrumentType(void)
{
	unsigned int uiInstrumentType = 0;
	int iCurSel = 0;
	iCurSel = m_ctrlInstrumentType.GetCurSel();
	if (iCurSel == 0)
	{
		uiInstrumentType = Instrument_Type_LAUX;
	}
	else if (iCurSel == 1)
	{
		uiInstrumentType = Instrument_Type_LAUL;
	}
	else if (iCurSel == 2)
	{
		uiInstrumentType = Instrument_Type_FDU;
	}
	return uiInstrumentType;
}

// 在文件中设置仪器
void CDlgSetInstrument::OnSetInstrumentToFile(unsigned int uiInstrumentType, unsigned int uiOpt)
{
	if (uiOpt == SetInstrument_Opt_AddPrevious)
	{
		OnAddInstrumentPrevious(uiInstrumentType);
	}
	else if (uiOpt == SetInstrument_Opt_AddNext)
	{
		OnAddInstrumentNext(uiInstrumentType);
	}
	else if (uiOpt == SetInstrument_Opt_Change)
	{
		OnChangeInstrumentType(uiInstrumentType);
	}
	else if (uiOpt == SetInstrument_Opt_Delete)
	{
		OnDeleteInstrument();
	}
	OnOK();
}
// 在此仪器之前加入仪器
void CDlgSetInstrument::OnAddInstrumentPrevious(unsigned int uiInstrumentType)
{
	CFile file;
	CString strPath = _T("..\\Parameter\\LineInit.txt");
	LONGLONG llMoveRow = 0;
	unsigned int uiLAUXUnitColumnIndex = 0;
	unsigned char ucInstrument = ' ';
	unsigned char ucInstrumentAdd[2] = {' ', ' '};
	if (FALSE == file.Open(strPath, CFile::modeReadWrite))
	{
		AfxMessageBox(_T("Create LineInit file error, Please check it again!"));
		return;
	}
	file.SeekToBegin();
	llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2);
	file.Seek(llMoveRow, CFile::begin);
	// 找到该条测线上交叉站位置
	while(1)
	{
		file.Read(&ucInstrument, 1);
		if (ucInstrument == 'A')
		{
			break;
		}
		else
		{
			file.Seek(1, CFile::current);
			uiLAUXUnitColumnIndex ++;
		}
	}
	// 要增加的仪器在交叉站左侧
	if (uiLAUXUnitColumnIndex >= m_iSelectUnitColumnIndex)
	{
		if (uiInstrumentType == Instrument_Type_LAUX)
		{
			ucInstrumentAdd[0] = LAU_LOGO;
		}
		else if (uiInstrumentType == Instrument_Type_LAUL)
		{
			ucInstrumentAdd[0] = LAUL_LOGO;
		}
		else if (uiInstrumentType == Instrument_Type_FDU)
		{
			ucInstrumentAdd[0] = FDU_LOGO;
		}
		ucInstrumentAdd[1] = '-';
		if (m_iSelectUnitColumnIndex > 0)
		{
			file.Seek(llMoveRow, CFile::begin);
			file.Read(&ucInstrument, 1);
			// 如果添加仪器行的左侧有空缺位置
			if (ucInstrument == ' ')
			{
				AddInstrumentOnLAUXLeft(&file, ucInstrumentAdd);
			}
			else
			{
				// 先将所有仪器整体右移，左侧空缺用空格填补
				OnAddInstrumentPosLeft(&file);
				llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2 + 2) + 1;
				file.Seek(llMoveRow, CFile::begin);
				m_iSelectUnitColumnIndex += 1;
				AddInstrumentOnLAUXLeft(&file, ucInstrumentAdd);
			}
		}
		// 在最左端增加仪器
		else
		{
			// 先将所有仪器整体右移，左侧空缺用空格填补
			OnAddInstrumentPosLeft(&file);
			llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2 + 2);
			file.Seek(llMoveRow, CFile::begin);
			file.Write(ucInstrumentAdd, 2);
		}
	}
	// 要增加的仪器在交叉站右侧
	else
	{
		if (uiInstrumentType == Instrument_Type_LAUX)
		{
			ucInstrumentAdd[1] = LAU_LOGO;
		}
		else if (uiInstrumentType == Instrument_Type_LAUL)
		{
			ucInstrumentAdd[1] = LAUL_LOGO;
		}
		else if (uiInstrumentType == Instrument_Type_FDU)
		{
			ucInstrumentAdd[1] = FDU_LOGO;
		}
		ucInstrumentAdd[0] = '-';
		file.Seek(llMoveRow, CFile::begin);
		file.Seek((m_uiColumnCount - 1) * 2, CFile::current);
		file.Read(&ucInstrument, 1);
		// 如果添加仪器行的右侧有空缺位置
		if (ucInstrument == ' ')
		{
			AddInstrumentOnLAUXRight(&file, ucInstrumentAdd);
		}
		else
		{
			// 所有行右侧添加仪器
			OnAddInstrumentPosRight(&file);
 			m_uiColumnCount += 1;
  			llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2);
			file.Seek(llMoveRow, CFile::begin);
			file.Seek(m_uiColumnCount * 2 - 1, CFile::current);
 			AddInstrumentOnLAUXRight(&file, ucInstrumentAdd);
		}
	}
	file.Close();
}

// 在此仪器之后加入仪器
void CDlgSetInstrument::OnAddInstrumentNext(unsigned int uiInstrumentType)
{
	OnChangeInstrumentType(uiInstrumentType);
	OnAddInstrumentPrevious(m_uiInstrumentType);
}

// 修改仪器类型
// @@@@@@暂时不考虑有辅助道的情况
void CDlgSetInstrument::OnChangeInstrumentType(unsigned int uiInstrumentType)
{
	CFile file;
	CString strPath = _T("..\\Parameter\\LineInit.txt");
	LONGLONG llMoveColumn = 0;
	LONGLONG llMoveRow = 0;
	unsigned char ucInstrument = ' ';
	if (FALSE == file.Open(strPath, CFile::modeRead|CFile::modeWrite))
	{
		AfxMessageBox(_T("Create LineInit file error, Please check it again!"));
		return;
	}
	file.SeekToBegin();
	llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2);
	file.Seek(llMoveRow, CFile::begin);
	llMoveColumn = m_iSelectUnitColumnIndex * 2;
	file.Seek(llMoveColumn, CFile::current);
	if (uiInstrumentType == Instrument_Type_LAUX)
	{
		ucInstrument = LAU_LOGO;
	}
	else if (uiInstrumentType == Instrument_Type_LAUL)
	{
		ucInstrument = LAUL_LOGO;
	}
	else if (uiInstrumentType == Instrument_Type_FDU)
	{
		ucInstrument = FDU_LOGO;
	}
	file.Write(&ucInstrument, 1);
	file.Close();
}

// 删除仪器
void CDlgSetInstrument::OnDeleteInstrument(void)
{
	CFile file;
	CString strPath = _T("..\\Parameter\\LineInit.txt");
	LONGLONG llMoveColumn = 0;
	LONGLONG llMoveRow = 0;
	unsigned int uiLAUXUnitColumnIndex = 0;
	unsigned char ucInstrument = ' ';
	unsigned char ucInstrumentDelete[2] = {' ', ' '};
	unsigned char* upInstrumentSave = NULL;
	if (FALSE == file.Open(strPath, CFile::modeReadWrite))
	{
		AfxMessageBox(_T("Create LineInit file error, Please check it again!"));
		return;
	}
	file.SeekToBegin();
	llMoveRow = m_iSelectUnitRowIndex * 2 * (m_uiColumnCount * 2 - 1 + 2);
	file.Seek(llMoveRow, CFile::begin);
	// 找到该条测线上交叉站位置
	while(1)
	{
		file.Read(&ucInstrument, 1);
		if (ucInstrument == 'A')
		{
			break;
		}
		else
		{
			file.Seek(1, CFile::current);
			uiLAUXUnitColumnIndex ++;
		}
	}
	file.Seek(llMoveRow, CFile::begin);
	// 要删除的仪器在交叉站左侧
	if (uiLAUXUnitColumnIndex >= m_iSelectUnitColumnIndex)
	{
		if (m_iSelectUnitColumnIndex > 0)
		{
			upInstrumentSave = new unsigned char[m_iSelectUnitColumnIndex * 2];
			file.Read(upInstrumentSave, m_iSelectUnitColumnIndex * 2);
			file.Seek(0 - m_iSelectUnitColumnIndex * 2, CFile::current);
			file.Write(ucInstrumentDelete, 2);
			file.Write(upInstrumentSave, m_iSelectUnitColumnIndex * 2);
			delete[] upInstrumentSave;
		}
		// 删除最左端仪器
		else
		{
			file.Write(ucInstrumentDelete, 2);
		}
	}
	// 要删除的仪器在交叉站右侧
	else
	{
		llMoveColumn = m_iSelectUnitColumnIndex * 2 - 1;
		// 删除最右端仪器
		if (m_iSelectUnitColumnIndex == (m_uiColumnCount - 1))
		{
			file.Seek(llMoveColumn, CFile::current);
			file.Write(ucInstrumentDelete, 2);
		}
		else
		{
			upInstrumentSave = new unsigned char[(m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2];
			file.Seek(llMoveColumn + 2, CFile::current);
			file.Read(upInstrumentSave, (m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2);
			int imove = 0 - (m_uiColumnCount - m_iSelectUnitColumnIndex) * 2;
			file.Seek(imove, CFile::current);
			file.Write(upInstrumentSave, (m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2);
			file.Write(ucInstrumentDelete, 2);
			delete[] upInstrumentSave;
		}
	}
	file.Close();
}
// 在每行的左侧增加仪器位置
void CDlgSetInstrument::OnAddInstrumentPosLeft(CFile* pFile)
{
	ULONGLONG ullEnd = pFile->SeekToEnd();
	ULONGLONG ullSize = ullEnd + (2 * m_uiRowCount - 1) * 2;
	unsigned int uiLineSize = m_uiColumnCount * 2 - 1 + 2;
	unsigned char* upInstrumentSave = NULL;
	if (ullSize == 0)
	{
		return;
	}
	upInstrumentSave = new unsigned char [ullSize];
	// 缓冲区初始化为空格，空格的ASCII码为32
	memset(upInstrumentSave, 32, ullSize);
	pFile->SeekToBegin();
	for (int i=0; i<(2 * m_uiRowCount - 1); i++)
	{
		pFile->Read(&upInstrumentSave[(2 + uiLineSize) * i + 2], uiLineSize);
	}
	pFile->SeekToBegin();
	pFile->Write(upInstrumentSave, ullSize);
	delete [] upInstrumentSave;
}

// 在每行的右侧增加仪器位置
void CDlgSetInstrument::OnAddInstrumentPosRight(CFile* pFile)
{
	ULONGLONG ullEnd = pFile->SeekToEnd();
	ULONGLONG ullSize = ullEnd + (2 * m_uiRowCount - 1) * 2;
	unsigned int uiLineSize = m_uiColumnCount * 2 - 1;
	unsigned char* upInstrumentSave = NULL;
	if (ullSize == 0)
	{
		return;
	}
	upInstrumentSave = new unsigned char [ullSize];
	// 缓冲区初始化为空格，空格的ASCII码为32
	memset(upInstrumentSave, 32, ullSize);
	pFile->SeekToBegin();
	for (int i=0; i<(2 * m_uiRowCount - 1); i++)
	{
		pFile->Read(&upInstrumentSave[(4 + uiLineSize) * i], uiLineSize);
		pFile->Seek(2, CFile::current);
		upInstrumentSave[(4 + uiLineSize) * i + uiLineSize + 2] = '\r';
		upInstrumentSave[(4 + uiLineSize) * i + uiLineSize + 3] = '\n';
	}
	pFile->SeekToBegin();
	pFile->Write(upInstrumentSave, ullSize);
	delete [] upInstrumentSave;
}

// 在交叉站左侧添加仪器
void CDlgSetInstrument::AddInstrumentOnLAUXLeft(CFile * pfile, unsigned char* pInstrumentAdd)
{
	unsigned char* upInstrumentSave = NULL;
	if (m_iSelectUnitColumnIndex > 1)
	{
		upInstrumentSave = new unsigned char[m_iSelectUnitColumnIndex * 2 - 2];
		pfile->Seek(1, CFile::current);
		pfile->Read(upInstrumentSave, m_iSelectUnitColumnIndex * 2 - 2);
		pfile->Seek(0 - m_iSelectUnitColumnIndex * 2, CFile::current);
		pfile->Write(upInstrumentSave, m_iSelectUnitColumnIndex * 2 - 2);
		pfile->Write(pInstrumentAdd, 2);
		delete [] upInstrumentSave;
	}
	else
	{
		pfile->Seek(-1, CFile::current);
		pfile->Write(pInstrumentAdd, 2);
	}
}
// 在交叉站右侧添加仪器
void CDlgSetInstrument::AddInstrumentOnLAUXRight(CFile * pfile, unsigned char* pInstrumentAdd)
{
	unsigned char* upInstrumentSave = NULL;
	int imove = 0;
	upInstrumentSave = new unsigned char[(m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2];
	imove = (m_iSelectUnitColumnIndex - m_uiColumnCount) * 2;
	pfile->Seek(imove, CFile::current);
	pfile->Read(upInstrumentSave, (m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2);
	imove = (m_iSelectUnitColumnIndex - m_uiColumnCount + 1) * 2;
	pfile->Seek(imove, CFile::current);
	pfile->Write(pInstrumentAdd, 2);
	pfile->Write(upInstrumentSave, (m_uiColumnCount - m_iSelectUnitColumnIndex - 1) * 2);
	delete [] upInstrumentSave;
}