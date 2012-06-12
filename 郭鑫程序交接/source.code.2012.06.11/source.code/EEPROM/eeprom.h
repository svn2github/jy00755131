/*
*	描述：该文件为EEPROM基本操作类的声明文件
*	文件：eeprom.h
*	注释：具体功能实现现实见eeprom.cpp
*	参考文献：有关AMTEL AT24C32C.pdf
*	版本：1.0.0.0
*	完成人：
*	example:
		eeprom::uint8_t value = 0;
		eeprom::uint8_t addr[2] = {0xff,0x0f};
		eeprom::uint8_t value_array[4] = {0x00, 0xf, 0xf0, 0xff};

		eeprom eeprom(eeprom::DEFAULT_PORT_ADDRESS, eeprom::DEFAULT_DEVICE_ADDRESS);
		printf("************init*************\n");
		eeprom.init();

		//read a byte
		//printf("**********read_current_address_byte***************\n");
		//value = eeprom.read_current_address_byte();

		//read random address
		//printf("**********read_random_address_byte***************\n");
		//value = eeprom.read_random_address_byte(addr);

		//read sequence data
		//printf("**********read_block***************\n");
		//eeprom.read_block(value_array);

		//printf("**********write_byte***************\n");
		//eeprom.write_byte(addr, 0xff);

		//	printf("**********write_block***************\n");
		//	eeprom.write_block(addr, value_array, sizeof(value_array));

		printf("******Release********\n");
		eeprom.release();
*/
#ifndef __EEPROM_H__
#define __EEPROM_H__

class eeprom
{
public:
	enum{
		ACK_LOW = 0x0, //
		ACK_HIGH = 0x1, //
		ACK_OPERAND = 0x2,
		
		LOW_BIT	= 0x1,
		LOW_QUAD_BIT = 0xF,// &	

		READ_ADDRESS = 0xa1, //
		WRITE_ADDRESS = 0xa0, //

		DEFAULT_PORT_DATA = 0xFF,
		DEFAULT_VALID_DATA = 0x07,
		DEFAULT_DEVICE_ADDRESS = 0xA0,
		DEFAULT_PORT_ADDRESS = 0x378,

		SCL_OPERAND = 0x01,
		SDA_OPERAND = 0x02,
		WP_OPERAND = 0x04,

		SET_SCL_ZERO_OPERAND = 0x6, //&
		SET_SCL_ONE_OPERAND = 0x1, // |

		SCL_LOOP_NUM = 0x3,

		MAX_SEQUENTIAL_READ_SIZE = 4,

		UINT8_SIZE = 8,
	};

	enum __enuEEPROM_TYPE
	{
		EEPROM_4K,
		EEPROM_8K,
		EEPROM_32K,
		EEPROM_64K,
	};

public:
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef enum __enuFLUSH_DATA_TYPE{
		FLUSH_START_CMD,
		FLUSH_DATA,
		FLUSH_STOP_CMD,
	} FLUSH_DATA_TYPE;

public:
	eeprom();
	eeprom(uint16_t port_address, uint8_t device_address, eeprom::__enuEEPROM_TYPE type = eeprom::EEPROM_32K);
	~eeprom();

public:
	//初始化eeprom操作
	bool init();
	bool init(uint16_t port_address, uint8_t device_address);

	//释放并口eeprom操作
	void release();

	//检测EEPROM是否准备好。OK返回1
	bool is_ready();
	
	//读取当前地址的一个字节8bit的EEPROM数据
	uint8_t read_current_address_byte();

	//读取随机指定地址的一个字节8bit的EEPROM数据
	uint8_t read_random_address_byte(const uint8_t *addr);

	//读取由指定地址开始的4byte指定长度的EEPROM数据
	// -1:表示错误， 0 正确
	int read_block(uint8_t *data);

	//向指定地址写入一个字节8bit的EEPROM数据
	void write_byte (uint8_t *addr, uint8_t val);
	
	//由指定地址开始写入指定长度的EEPROM数据
	void write_block(const uint8_t* addr, const uint8_t* buf, uint16_t buf_size);
	
protected:
	void start_cmd();
	void stop_cmd();
	uint8_t get_ack();
private:
	void write_byte_32k_private(uint8_t *addr, uint8_t val);
	void write_byte_8k_private(uint8_t *addr, uint8_t val);

	void write_block_32k_private(const uint8_t* addr, const uint8_t* buf, uint16_t buf_size);
	void write_block_8k_private(const uint8_t* addr, const uint8_t* buf, uint16_t buf_size);

	uint8_t read_random_address_byte_8k(const uint8_t *addr);
	uint8_t read_random_address_byte_32k(const uint8_t *addr);

	void flush_data_private(const uint8_t* data, uint16_t data_size = 1, FLUSH_DATA_TYPE flush_data_type = FLUSH_DATA);
	uint8_t get_bit_private();
	uint8_t get_bit_private2();
	uint8_t get_byte_private();
	void clock_in();
private:
	uint8_t device_address_; //设备地址
	uint16_t port_address_; //并口地址
	static uint8_t start_data_[];
	static uint8_t stop_data_[];

	enum __enuEEPROM_TYPE type_;//!< 指定当前使用的EEPROM类型
};

#endif /*__EEPROM_H__*/
