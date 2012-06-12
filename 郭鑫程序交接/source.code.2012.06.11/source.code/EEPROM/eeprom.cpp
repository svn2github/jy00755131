/*
*	描述：该文件为EEPROM基本操作类的实现文件
*	文件：eeprom.cpp
*	注释：具体声明和调用见eeprom.h
*	参考文献：有关AMTEL AT24C32C.pdf
*	版本：1.0.0.0
*	完成人：
*/

#include <windows.h>
//#include "PortTalk_IOCTL.h"
#include "eeprom.h"

#ifdef _DEBUG
#include <conio.h>
#	define printf(X)	_cprintf(X)
#else
#	define printf(X)	
#endif

#pragma comment(lib, "inpout32")

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);

eeprom::uint8_t eeprom::start_data_[] = {0x00, 0x01, 0x03, 0x3,0x3,0x3,0x03,0x3};
eeprom::uint8_t eeprom::stop_data_[] = {0x3, 0x01, 0x0};

eeprom::eeprom():port_address_(eeprom::DEFAULT_PORT_ADDRESS), 
				device_address_(eeprom::DEFAULT_DEVICE_ADDRESS),
				type_(eeprom::EEPROM_32K)
{

}

eeprom::eeprom(eeprom::uint16_t port_address, eeprom::uint8_t device_address, eeprom::__enuEEPROM_TYPE type):
	port_address_(port_address), device_address_(device_address), type_(type)
{

}

eeprom::~eeprom()
{
	if(port_address_ != 0x00 ){
		release();
	}
}

bool eeprom::init()
{
	return init(port_address_, device_address_);
}

bool eeprom::init(eeprom::uint16_t port_address, eeprom::uint8_t device_address)
{
	bool result = false;

	if(port_address == 0x0 || device_address == 0)
		return false;

	if(port_address_ != port_address)
		port_address_ = port_address;

	if(device_address_ != device_address)
		device_address_ = device_address;
	
	//if(PortTalk_Handle == NULL)
	//	result = (OpenPortTalk() != -1 ? true : false);

	return result;
}

//释放并口eeprom操作
void eeprom::release()
{
	//if(PortTalk_Handle != NULL)
	//	ClosePortTalk();
}

//检测EEPROM是否准备好。OK返回1
bool eeprom::is_ready()
{
	return true;
}

//读取当前地址的一个字节8bit的EEPROM数据
eeprom::uint8_t eeprom::read_current_address_byte()
{
	int i = 0;
	uint8_t result = NULL;
	uint8_t ack = 0x0;
	start_cmd();

	uint8_t data = READ_ADDRESS;
	flush_data_private(&data, 1);

	ack = get_bit_private();	
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	result = get_byte_private();

	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_HIGH)
		stop_cmd();

	return result;
}

eeprom::uint8_t eeprom::read_random_address_byte(const uint8_t *addr)
{
	uint8_t val = 0x0;
	switch(type_){
		case EEPROM_8K:
			val = read_random_address_byte_8k(addr);
			break;
		case EEPROM_32K:
			val = read_random_address_byte_32k(addr);
			break;
	}
	
	return val;
}

eeprom::uint8_t eeprom::read_random_address_byte_8k(const uint8_t *addr)
{
	int i = 0;
	uint8_t result = NULL;
	uint8_t ack = 0x0;
	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	ack = get_bit_private();

	flush_data_private(addr, 1);

	//ack
	ack = get_bit_private();

	//start
	start_cmd();

	//add address
	data = READ_ADDRESS;
	flush_data_private(&data, 1);

	//ack
	ack = get_bit_private();

	//get a data
	result = get_byte_private();

	ack = get_bit_private();

	stop_cmd();

	return result;
}

//读取随机指定地址的一个字节8bit的EEPROM数据
eeprom::uint8_t eeprom::read_random_address_byte_32k(const eeprom::uint8_t *addr)
{
	int i = 0;
	uint8_t result = NULL;
	uint8_t ack = 0x0;
	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	//add two address
	flush_data_private(addr + 1, 1);

	//ack
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	//add two address
	flush_data_private(addr, 1);

	//ack
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	//start
	start_cmd();

	//add address
	data = READ_ADDRESS;
	flush_data_private(&data, 1);

	//ack
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	//get a data
	result = get_byte_private();

	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_HIGH)
		stop_cmd();

	return result;
}

//读取由指定地址开始的指定长度的EEPROM数据
int eeprom::read_block(uint8_t *buf)
{
	int i = 0;
	uint8_t ack = 0x0;
	uint8_t data = 0x0;

	//start
	start_cmd();

	//add address
	data = READ_ADDRESS;
	flush_data_private(&data, 1);

	//get a data for 4 byte
	for(i = 0; i < MAX_SEQUENTIAL_READ_SIZE; ++i){

		ack = get_bit_private();
		//ack &= ACK_OPERAND;
		//if( (ack/2) == ACK_LOW)
			buf[i] = get_byte_private();
	}

	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_HIGH)
		stop_cmd();

	return 0;
}

void eeprom::write_byte(uint8_t *addr, uint8_t val)
{
	switch(type_){
		case EEPROM_8K:
			write_byte_8k_private(addr, val);
			break;
		case EEPROM_32K:
			write_byte_32k_private(addr, val);
			break;
	}
}

void eeprom::write_byte_8k_private(uint8_t *addr, uint8_t val)
{
	int i = 0;
	uint8_t ack = 0x0;

	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	ack = get_bit_private();

	printf("flush address\n");
	flush_data_private(addr, 1);

	ack = get_bit_private();

	flush_data_private(&val, 1);

	ack = get_bit_private();
	stop_cmd();
}

//向指定地址写入一个字节8bit的EEPROM数据
void eeprom::write_byte_32k_private(uint8_t *addr, uint8_t val)
{
	int i = 0;
	uint8_t ack = 0x0;

	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	printf("flush address1\n");
	ack = get_bit_private();
	printf("flush address1\n");

	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	flush_data_private(addr + 1, 1);

	printf("get ack\n");
	ack = get_bit_private();	
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	printf("flush address2\n");
	flush_data_private(addr, 1);

	printf("get ack\n");
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	printf("flush data\n");
	flush_data_private(&val, 1);

	printf("get ack\n");
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW)
		stop_cmd();

}

void eeprom::write_block(const uint8_t* addr, const uint8_t* buf, uint16_t buf_size)
{
	switch(type_){
		case EEPROM_8K:
			write_block_8k_private(addr, buf, buf_size);
			break;
		case EEPROM_32K:
			write_block_32k_private(addr, buf, buf_size);
				break;
	}
}

void eeprom::write_block_8k_private(const uint8_t* addr, const uint8_t* buf, uint16_t buf_size)
{
	int i = 0;
	uint8_t ack = 0x0;

	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	ack = get_bit_private();

	flush_data_private(addr, 1);

	printf("get ack\n");
	ack = get_bit_private();

	for(int i = 0; i < buf_size; ++i){

		flush_data_private(buf+i, 1);
		ack = get_bit_private();
	}

	stop_cmd();
}

//由指定地址开始写入指定长度的EEPROM数据
void eeprom::write_block_32k_private(const uint8_t *addr, const eeprom::uint8_t *buf, eeprom::uint16_t buf_size)
{
/*	int i = 0;
	uint8_t result = NULL;
	uint8_t ack = 0x0;
	uint8_t data = WRITE_ADDRESS;

	printf("*********start cmd*********\n");
	start_cmd();

	printf("*********flush device address*********\n");
	flush_data_private(&data, 1);

	printf("*********get ack*********\n");
	ack = get_bit_private();	
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	printf("*********flush word address1*********\n");
	flush_data_private(addr, 1);

	printf("*********get ack*********\n");
	ack = get_bit_private();	
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	printf("*********flush word address2*********\n");
	flush_data_private(addr + 1, 1);

	for(i = 0; i < buf_size; ++i){

		printf("*********get ack*********\n");
		ack = get_bit_private();
		//ack &= ACK_OPERAND;
		//if( (ack/2) != ACK_LOW) return 0;
		printf("*********flush data*********\n");
		flush_data_private(buf + i, 1);
	}

	printf("*********get ack*********\n");
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW)
	printf("*********stop cmd*********\n");
	stop_cmd();
*/

	int i = 0;
	uint8_t ack = 0x0;

	start_cmd();

	uint8_t data = WRITE_ADDRESS;
	flush_data_private(&data, 1);

	printf("flush address1\n");
	ack = get_bit_private();
	printf("flush address1\n");

	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	flush_data_private(addr + 1, 1);

	printf("get ack\n");
	ack = get_bit_private();	
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	printf("flush address2\n");
	flush_data_private(addr, 1);

	printf("get ack\n");
	ack = get_bit_private();
	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW) return 0;

	for(int i = 0; i < buf_size; ++i){

		flush_data_private(buf+i, 1);
		printf("ACK");
		ack = get_bit_private();
	}

	//ack &= ACK_OPERAND;
	//if( (ack/2) != ACK_LOW)
	stop_cmd();


}

eeprom::uint8_t eeprom::get_byte_private()
{
	int i = 0;
	uint8_t result = 0x0;
	uint8_t ack = 0x0;
	uint8_t ss = 0x0;
	for(i = 0; i < UINT8_SIZE; ++i){

		result <<= 1;
		ack = get_bit_private();
		ss = ((ack & SDA_OPERAND) >> 1);
		result += ss;
	}

	return result;
}

eeprom::uint8_t eeprom::get_bit_private()
{
	uint8_t u8 = get_bit_private2();
	return u8;
}

eeprom::uint8_t eeprom::get_bit_private2()
{
	int i = 0;
	uint8_t u8 = WP_OPERAND ^ SCL_OPERAND;
	uint8_t u8_1 = u8;

	Inp32(port_address_ + 2);

	Out32(port_address_, 0x0);
	Out32(port_address_, 0x1);
	u8_1 = Inp32(port_address_ + 2);
	Out32(port_address_, 0x0);
	u8 = u8_1 ^ 0x0F;


	//for(i = 0; i < SCL_LOOP_NUM; ++i){

	//	clock_in(u8);

	//	if(i == 2){

	//		u8_1 = inportb(port_address_ + 2);
	//		u8 = ~u8_1;
	//	}
	//	else
	//		outportb(port_address_, u8);
	//}	

	return u8;
}

void eeprom::flush_data_private(const eeprom::uint8_t* data, eeprom::uint16_t data_size, FLUSH_DATA_TYPE flush_data_type)
{
	int i = 0, j = 0;
	int loop = UINT8_SIZE;
	uint8_t u8 = 0x0;
	uint8_t u8_1 = 0x0;

	switch(flush_data_type){
		case FLUSH_START_CMD:
		case FLUSH_STOP_CMD:
			u8 = (*data) ^ 0x0f;
			Out32(port_address_ + 2, u8);
			Out32(port_address_, *data);
			break;
		case FLUSH_DATA:{

				for(i = 0; i < data_size; ++i){

					loop = UINT8_SIZE;

					while(loop--){

						u8 = (((data[i]>>loop) & LOW_BIT) << 1);
						
						u8_1 = u8 ^ 0x1f;
						Out32(port_address_ + 2, u8_1);
						clock_in();
					}
				}
			}
			break;
		default:
			break;
	}
}

void eeprom::clock_in()
{

	Out32(port_address_, 0x0);
	Out32(port_address_, 0x1);
	Out32(port_address_, 0x0);
}

void eeprom::start_cmd()
{
	int i = sizeof(start_data_) / sizeof(start_data_[0]);

	while(i--)
		flush_data_private(start_data_ + i, 1, FLUSH_START_CMD);
}
void eeprom::stop_cmd()
{
	int i = sizeof(stop_data_) / sizeof(stop_data_[0]);
	while(i--)
		flush_data_private(stop_data_ + i, 1, FLUSH_STOP_CMD);
 }

eeprom::uint8_t eeprom::get_ack()
{
	uint8_t ack = 0x0;

	ack = get_bit_private();

	return (ack & SDA_OPERAND) >> 1;
}



