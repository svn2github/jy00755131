
#pragma once
//////////////////////////////////////////////////////////////////////////
/**
 * @class CFDUConfig
 * @brief 采集站寄存器属性配置
 *
 * 保存采集站属性所对应的寄存器可能取值及索引值。从该配置对象中读取配置数据，然后通过命令
 下发给采集站。
 */
class CFDUConfig
{
public:
	CString m_strCrewName;
	DWORD   m_dwGain3301Index;
	DWORD   m_dwMux3301Index;
	DWORD   m_dwMode5373Index;
	DWORD   m_dwAttr5373Index;
	DWORD	m_dwFilterIIRIndex;	

	DWORD	m_dwSampleRateIndex;	
	DWORD	m_dwFilterFIRIndex;
	DWORD   m_dwLineDataRateIndex;
	DWORD   m_dwTimeManageIndex;
	DWORD   m_dwGPSConnIndex;
	DWORD   m_dwSourceConnIndex;
	DWORD   m_dwHighpassFrequencyIndex;
	// 属性
private:
	
	// 前置放大: x1	x2	x4	x8	x16	x32	x64
	unsigned char set_3301_gain[7];    
	// 模拟门选择
	unsigned char set_3301_mux[4]; 
	// 5373模式
	unsigned char set_5373_mode[8];	    
	// DA衰减
	unsigned char set_5373_att[8];	   
	// 采样率依次为：4k、2k、1k、500、333、250、200、125、100、50、40、25、20、10、5
	unsigned long ad_rate[5];			
	//  IIR选择高位	
	unsigned long filter_iir_high[16]; 
	// IIR选择低位
 	unsigned long filter_iir_low[16];  
	// FIR选择
	unsigned long filter_fir[2];	
	unsigned long line_data_rate[2];

	// 采样率（以微秒us为单位）：4k、2k、1k、500、333、250、200、125、100、50、40、25、20、10、5
	unsigned long m_dwSampleRates[5];
	FLOAT high_pass_frequency[2];
	CString time_management[3];
	CString gps_conn[3];
	CString source_conn[3];


public:
	CFDUConfig(void);
	~CFDUConfig(void);

	void InitFUDConfig(void);
	
	unsigned char GetGainCode(int nMultipleValue);
//	unsigned char Get3301Gain(void) { return set_3301_gain[m_dwGain3301Index]; };
	unsigned char Get3301Mux(void)  { return set_3301_mux[m_dwMux3301Index];  };
	unsigned char Get5373Mode(void) { return set_5373_mode[m_dwMode5373Index]; };
	unsigned char Get5373Attr(void) { return set_5373_att[m_dwAttr5373Index];  };
	// 获得以微秒为单位的采样率
	DWORD GetSampleRateByUS(void)    { return m_dwSampleRates[m_dwSampleRateIndex]; };
	// 获得相应采样率下的硬件编码
	DWORD GetSampleRateCode(void)    { return ad_rate[m_dwSampleRateIndex];         };
	DWORD GetFilterIIRHigh(void) { return filter_iir_high[m_dwFilterIIRIndex]; };
	DWORD GetFilterIIRLow(void)  { return filter_iir_low[m_dwFilterIIRIndex];  };
	DWORD GetFilterFIR(void)     { return filter_fir[m_dwFilterFIRIndex];  };
	DWORD GetLineDataRate(void)  { return line_data_rate[m_dwLineDataRateIndex]; };
	FLOAT GetHighpassFrequency(void)  {return high_pass_frequency[m_dwHighpassFrequencyIndex]; };

//	unsigned char Get3301GainByIndex(DWORD  dwIndex) { return set_3301_gain[dwIndex]; };
//	unsigned char Get3301MuxByIndex(DWORD  dwIndex)  { return set_3301_mux[dwIndex];  };
//	unsigned char Get5373ModeByIndex(DWORD  dwIndex) { return set_5373_mode[dwIndex]; };
//	unsigned char Get5373AttrByIndex(DWORD  dwIndex) { return set_5373_att[dwIndex];  };
	
	DWORD GetSampleRateByIndex(DWORD  dwIndex)    { return ad_rate[dwIndex];         };
//	DWORD GetFilterIIRHighByIndex(DWORD  dwIndex) { return filter_iir_high[dwIndex]; };
//	DWORD GetFilterIIRLowByIndex(DWORD  dwIndex)  { return filter_iir_low[dwIndex];  };
	DWORD GetFilterFIRByIndex(DWORD  dwIndex)     { return filter_fir[dwIndex];  };

};
