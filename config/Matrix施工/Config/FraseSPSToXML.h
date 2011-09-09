#pragma once

#define FLAG_X											_T("X")
#define FLAG_R											_T("R")
#define ARRAY_SIZE									2048
#define HEADER_INSTRUMENT_TYPE	_T("400")
#define HEADER_REC_TYPE						_T("600")
#define HEADER_SHOT_TYPE					_T("700")
#define LINE_SUM										5

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


class CFraseSPSToXML
{
public:
	CFraseSPSToXML(void);
	~CFraseSPSToXML(void);

private:
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	CXMLDOMDocument m_oXMLDOMDocument_operation;

	CString m_str_shot_type;
	CString m_str_rec_type;
	CString m_str_instr_type;

	struct rec_file_struct rec_file[ARRAY_SIZE];
	struct comm_file_struct comm_file[ARRAY_SIZE];
	int m_icnt_rec;  //解析sps文件中的第几行，结构
	int m_icnt_comm;

	BOOL read_file_rec(CString strPath);
	BOOL read_file_comm(CString strPath);
	CString readbyte(int ifrm_byte, int ito_byte, CString str);

	BOOL OpenMatrixIniXMLFile(CString strPath);
	void CloseMatrixIniXMLFile();
	void SaveMatrixIniXMLFile(CString strPath);

	BOOL OpenMatrixIniXMLFile_operation(CString strPath);
	void CloseMatrixIniXMLFile_operation();
	void SaveMatrixIniXMLFile_operation(CString strPath); 

	void OnSave_rec(void);	
	void OnSave_comm(void);	
	void AddToXML_rec(CXMLDOMElement* pElement, rec_file_struct *ptr_rec_file);
	void AddToXML_comm(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt, int cnt_line);
	void AddToXML_shot(CXMLDOMElement* pElement, comm_file_struct *ptr_comm_file, int cnt);
	void OnSave_shot(void);
public:
	// 载入SPS文件
	void LoadSPSFile(CString strPath);
	// 保存修改到测线XML文件
	void SaveLineXML(CString strPath);
	// 保存修改到施工XML
	void SaveOperationXML(CString strPath);
};
