// sps_vcDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define PATH_FILE_X "..\\JO.X"
#define PATH_FILE_R "..\\JO.R"
#define PATH_FILE_S "..\\JO.S"
#define PATH_FILE_XML_LINE "..\\Matrix.xml"
#define PATH_FILE_XML_OPERATION "..\\MatrixOperation.xml" 

#define FLAG_X "X"
#define FLAG_S "S"
#define FLAG_R "R"
#define PKG 128
#define ARRAY_SIZE 2048
#define HEADER_INSTRUMENT_TYPE _T("400")
#define HEADER_REC_TYPE _T("600")
#define HEADER_SHOT_TYPE _T("700")
#define LINE_SUM 5

struct survey_setup_struct 
{
	int m_count;
	int m_lineName;
	CString m_recSection;
};

typedef struct rec_file_struct 
{
	int count;
	int lineName;
	double point_upper;
	double point_lower;
	double interval;
	int type;

}rec_file_struct, *ptr_rec_file_struct;

typedef struct comm_file_struct 
{
	double shot_lineName;
	double shot_point;
	int rec_lineName[LINE_SUM];
	double rec_point_upper[LINE_SUM];
	double rec_point_lower[LINE_SUM];
	int record_channel_interval[LINE_SUM];
	int record_channel_upper[LINE_SUM];
	int record_channel_lower[LINE_SUM];
	int rec_point_index[LINE_SUM];
	int rec_line_num;
	double shot_SFL;
	double shot_SFN;
	int channel_upper[LINE_SUM];
	int channel_lower[LINE_SUM];

}comm_file_struct, *ptr_comm_file_struct;


// Csps_vcDlg 对话框
class Csps_vcDlg : public CDialog
{
// 构造
public:
	Csps_vcDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SPS_VC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_edt_record;
	CEdit m_edt_rec_incr;
	CEdit m_edt_instr;
	CEdit m_edt_line;
	CEdit m_edt_pnt_nb;
	CEdit m_edt_pnt_indx;
	CEdit m_edt_frm_chnl;
	CEdit m_edt_to_chnl;
	CEdit m_edt_chnl_incr;
	CEdit m_edt_line_rec;
	CEdit m_edt_frm_rec;
	CEdit m_edt_to_rec;
	CEdit m_edt_rec_indx;
	CButton m_btn_read;
	CButton m_btn_xml;
	CButton m_btn_operation;

	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	CXMLDOMDocument m_oXMLDOMDocument_operation;

	/** 总数*/
	unsigned int m_uiCountAll;
	/** 队列*/
	CList<int, int> m_ols;

	/**  Tab键数量*/
	unsigned int m_uiTabCount;
	/** 写格式化XML文件用*/
	CString m_strTabChild;
	/** 写格式化XML文件用*/
	CString m_strTabParent;

	CString str_shot_type;
	CString str_rec_type;
	CString str_instr_type;

	struct survey_setup_struct survey_setup[ARRAY_SIZE];
	struct rec_file_struct rec_file[ARRAY_SIZE];
	struct comm_file_struct comm_file[ARRAY_SIZE];
	int cnt_rec;  //解析sps文件中的第几行，结构
	int cnt_comm;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	int read_file_rec(void);
	int read_file_comm(void);
	CString readbyte(int m_frm_byte, int m_to_byte, CString m_str);

	void OpenMatrixIniXMLFile();
	void CloseMatrixIniXMLFile();
	void SaveMatrixIniXMLFile();

	void OpenMatrixIniXMLFile_operation();
	void CloseMatrixIniXMLFile_operation();
	void SaveMatrixIniXMLFile_operation(); 

	void OnSave_rec(void);	
	void OnSave_comm(void);	
    void AddToXML_rec(CXMLDOMElement* pElement, rec_file_struct *ptr_rec_file);
	void AddToXML_comm(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt, int cnt_line);
	void AddToXML_shot(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt);
	void OnSave_shot(void);

	afx_msg void OnBnClickedButton3();
};
