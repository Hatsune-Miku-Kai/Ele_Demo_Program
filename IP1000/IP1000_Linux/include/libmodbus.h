#ifndef __LIB_MODBUS_H__
#define __LIB_MODBUS_H__

#include <string>
#include "semaphore.h"

class libmodbusData;

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
typedef short int			int16_t;

class ModbusBase
{
public:
	static uint8_t getHighByte(const uint16_t data);
	static uint8_t getLowByte(const uint16_t data);
	static float getFloat(const uint16_t *src, int endian_type);
	static void setFloat(float f, uint16_t *dest, int endian_type);
	static void getFloatBuff(uint16_t *src, unsigned int snum, float f[], int endian_type);
	static void setFloatBuff(float f[], unsigned int fnum, uint16_t *dest, int endian_type);
	static void shortBuffToCharBuff(unsigned short *src, unsigned int snum, char *dest);
	static void charBuffToShortBuff(const char *src, unsigned int snum, unsigned short *dest);

public:
	enum Mode { RTU_, TCP_, EC_RTU_};
	void setRTUParm(int port,int baud=115200, char parity='N', int data_bit=8, int stop_bit=1);//parity : 'N' ,  'E'  ,  'O'  //port:从0开始
	void setRTUSlaveID(int id);
	void setTCPParm(const std::string &ip, int port, int endian_type = 1);
	void setTimeout(unsigned int responseTimeout, unsigned int byteTimeout);
	void setModbusCheckHeart(bool enble);
	bool isCheckHeartEnable(){return _modbuscheckheart;};
protected:
	ModbusBase(const std::string &name, Mode mode);
	~ModbusBase();
	ModbusBase(ModbusBase&) = delete;
	ModbusBase& operator=(ModbusBase&) = delete;

	struct RTUData
	{
		int port;
		int baud;
		char parity;
		int data_bit;
		int stop_bit;
		int slave_id;
		RTUData() : port(2), baud(115200), parity('N'), data_bit(8), stop_bit(1), slave_id(1) {};
	} m_rtu;
	struct TCPData
	{
		std::string ip;
		int port;
		int socket;
		int endian_type;
		TCPData() : ip("192.168.1.13"), port(4000), socket(0), endian_type(1) {};
	} m_tcp;

	const std::string name;
	const Mode m_mode;
	struct timeval response_timeout;
	struct timeval byte_timeout;
	bool _modbuscheckheart;
	libmodbusData* mb;
};

class ModbusSlave : public ModbusBase
{
public:
	ModbusSlave(const std::string &name, Mode mode, unsigned int holdRegMaxNum=2000, unsigned int inputRegMaxNum=2000, unsigned int holdBitMaxNum=0, unsigned int inputBitMaxNum=0);
	~ModbusSlave();
	void start();
	void stop();
	void stopSync();

	void setTimeoutCount(unsigned int timeoutCount);
	void setThreadParm(const std::string& name, int priority, int stackSize);

	uint16_t& getHoldRegs(unsigned int n) const;
	uint16_t& getInputRegs(unsigned int n) const;
	uint8_t& getHoldBits(unsigned int n) const;
	uint8_t& getInputBits(unsigned int n) const;

	bool isCommunication() const { return isCommunicationFlag; };

private:
	void create();
	void destroy();
	bool connect();
	void disconnect();
	static void PollingTask(void* args);
	void Polling();

	const unsigned int holdRegMax;
	const unsigned int inRegMax;
	const unsigned int holdBitMax;
	const unsigned int inBitMax;

	std::string taskName;
	int taskPriority;
	int taskStackSize;

	bool toConnectFlag;
	bool exitPollingFlag;
	bool isCommunicationFlag;

	unsigned int noResponseMaxCount;

	sem_t semExitPollingFinish;
	sem_t semExitPollingNotice;
	pthread_t * pvtPollingThread;

};

class ModbusMaster : public ModbusBase
{
public:
	ModbusMaster(const std::string &name, Mode mode);
	~ModbusMaster();

	bool readHoldOneBit(int addr, uint8_t &status);
	bool readHoldBits(int addr, int nb, uint8_t *dest);
	bool readInputOneBit(int addr, uint8_t &status);
	bool readInputBits(int addr, int nb, uint8_t *dest);
	bool readHoldOneRegister(int addr, uint16_t &value);
	bool readHoldRegisters(int addr, int nb, uint16_t *dest);
	bool readInputOneRegister(int addr, uint16_t &value);
	bool readInputRegisters(int addr, int nb, uint16_t *dest);
	bool writeHoldOneBit(int addr, int status);
	bool writeHoldOneRegister(int addr, int value);
	bool writeHoldBits(int addr, int nb, const uint8_t *data);
	bool writeHoldRegisters(int addr, int nb, const uint16_t *data);

	bool isConnect() const;

	void create();
	void destroy();
	bool connect();
	void disconnect();

	inline  void setAddrBase0Flag(bool flag){addrBase0Flag = flag;};

private:
	bool semReadWriteWait();
	void semReadWritePost();
	sem_t semReadWrite;
	bool addrBase0Flag;
};

#endif
