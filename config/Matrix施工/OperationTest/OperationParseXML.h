#pragma once

class COperationParseXML
{

private:
	
	int ParseRecord(CXMLDOMElement* pElement);
public:
	COperationParseXML(void);
	~COperationParseXML(void);
	int ParseOperationTable(LPCTSTR pstrXMLFilePath);

};
