/*
 * Rfm22.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#include <Rfm22.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include <math.h>
#include <string.h>

static const Rfm22::ModemConfig MODEM_CONFIG_TABLE[] =
   {
       { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x00, 0x08 }, // Unmodulated carrier
       { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x33, 0x08 }, // FSK, PN9 random modulation, 2, 5

       // All the following enable FIFO with reg 71
       //  1c,   1f,   20,   21,   22,   23,   24,   25,   2c,   2d,   2e,   58,   69,   6e,   6f,   70,   71,   72
       // FSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
       { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x22, 0x08 }, // 2, 5
       { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x22, 0x3a }, // 2.4, 36
       { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x22, 0x48 }, // 4.8, 45
       { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x22, 0x48 }, // 9.6, 45
       { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x22, 0x0f }, // 19.2, 9.6
       { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x22, 0x1f }, // 38.4, 19.6
       { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x22, 0x2e }, // 57.6. 28.8
       { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x22, 0xc8 }, // 125, 125

       { 0x2b, 0x03, 0xa1, 0xe0, 0x10, 0xc7, 0x00, 0x09, 0x40, 0x0a, 0x1d,  0x80, 0x60, 0x04, 0x32, 0x2c, 0x22, 0x04 }, // 512 baud, FSK, 2.5 Khz fd for POCSAG compatibility
       { 0x27, 0x03, 0xa1, 0xe0, 0x10, 0xc7, 0x00, 0x06, 0x40, 0x0a, 0x1d,  0x80, 0x60, 0x04, 0x32, 0x2c, 0x22, 0x07 }, // 512 baud, FSK, 4.5 Khz fd for POCSAG compatibility

       // GFSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
       // These differ from FSK only in register 71, for the modulation type
       { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x23, 0x08 }, // 2, 5
       { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x23, 0x3a }, // 2.4, 36
       { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x23, 0x48 }, // 4.8, 45
       { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x23, 0x48 }, // 9.6, 45
       { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x23, 0x0f }, // 19.2, 9.6
       { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x23, 0x1f }, // 38.4, 19.6
       { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x23, 0x2e }, // 57.6. 28.8
       { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x23, 0xc8 }, // 125, 125

       // OOK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
       { 0x51, 0x03, 0x68, 0x00, 0x3a, 0x93, 0x01, 0x3d, 0x2c, 0x11, 0x28, 0x80, 0x60, 0x09, 0xd5, 0x2c, 0x21, 0x08 }, // 1.2, 75
       { 0xc8, 0x03, 0x39, 0x20, 0x68, 0xdc, 0x00, 0x6b, 0x2a, 0x08, 0x2a, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x21, 0x08 }, // 2.4, 335
       { 0xc8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x29, 0x04, 0x29, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x21, 0x08 }, // 4.8, 335
       { 0xb8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x82, 0x29, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x21, 0x08 }, // 9.6, 335
       { 0xa8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x41, 0x29, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x21, 0x08 }, // 19.2, 335
       { 0x98, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x20, 0x29, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x21, 0x08 }, // 38.4, 335
       { 0x98, 0x03, 0x96, 0x00, 0xda, 0x74, 0x00, 0xdc, 0x28, 0x1f, 0x29, 0x80, 0x60, 0x0a, 0x3d, 0x0c, 0x21, 0x08 }, // 40, 335
   };

Rfm22::Rfm22(SpiGeneric& spi) : spi(spi), currentMode(Rfm22ModeInitialising),_txHeaderTo(0xff),
								_thisAddress(0xff),_txHeaderFrom(0xff),_txHeaderId(0),_txHeaderFlags(0),
								_bufLen(0),_txBufSentIndex(0){
	// TODO Auto-generated constructor stub
}

void Rfm22::init(){

	spi.init();
	reset();
	setupInterrupts();
	enableInterrupts();
	setModeIdle();

	/*// Most of these are the POR default
    spiWrite(RH_RF22_REG_7D_TX_FIFO_CONTROL2, RH_RF22_TXFFAEM_THRESHOLD);
    spiWrite(RH_RF22_REG_7E_RX_FIFO_CONTROL,  RH_RF22_RXFFAFULL_THRESHOLD);
    spiWrite(RH_RF22_REG_30_DATA_ACCESS_CONTROL, RH_RF22_ENPACRX | RH_RF22_ENPACTX | RH_RF22_ENCRC | (_polynomial & RH_RF22_CRC));
   */

	uint8_t data[] = {RH_RF22_TXFFAEM_THRESHOLD,RH_RF22_RXFFAFULL_THRESHOLD};
    spi.writeRegBytes(RH_RF22_REG_7D_TX_FIFO_CONTROL2,data,2);
    //reg 0x30 = 10001111
    spi.writeReg(RH_RF22_REG_30_DATA_ACCESS_CONTROL, RH_RF22_ENPACRX | RH_RF22_ENPACTX | RH_RF22_ENCRC | RH_RF22_CRC);


    // Configure the message headers
    // Here we set up the standard packet format for use by the RH_RF22 library
    // 8 nibbles preamble
    // 2 SYNC words 2d, d4
    // Header length 4 (to, from, id, flags)
    // 1 octet of data length (0 to 255)
    // 0 to 255 octets data
    // 2 CRC octets as CRC16(IBM), computed on the header, length and data
    // On reception the to address is check for validity against RH_RF22_REG_3F_CHECK_HEADER
    // or the broadcast address of 0xff
    // If no changes are made after this, the transmitted
    // to address will be 0xff, the from address will be 0xff
    // and all such messages will be accepted. This permits the out-of the box
    // RH_RF22 config to act as an unaddresed, unreliable datagram service
    data[0] = RH_RF22_BCEN_HEADER3 | RH_RF22_HDCH_HEADER3;
    data[1] = RH_RF22_HDLEN_4 | RH_RF22_SYNCLEN_2;
    spi.writeRegBytes(RH_RF22_REG_32_HEADER_CONTROL1,data,2);

    // Set some defaults. An innocuous ISM frequency, and reasonable pull-in
    setFrequency(434.0, 0.05);
//    setFrequency(900.0);
    // Some slow, reliable default speed and modulation
    setModemConfig(FSK_Rb2_4Fd36);
//    setModemConfig(FSK_Rb125Fd125);
    setGpioReversed(false);
    // Lowish power
    setTxPower(RH_RF22_TXPOW_8DBM);


}

uint8_t Rfm22::rssiRead()
{
    return spi.readReg(RH_RF22_REG_26_RSSI);
}

uint8_t Rfm22::ezmacStatusRead()
{
    return spi.readReg(RH_RF22_REG_31_EZMAC_STATUS);
}

bool Rfm22::setFHStepSize(uint8_t fhs){
    spi.writeReg(RH_RF22_REG_7A_FREQUENCY_HOPPING_STEP_SIZE, fhs);
    return !(statusRead() & RH_RF22_FREQERR);
}
bool Rfm22::setFHChannel(uint8_t fhch){
	spi.writeReg(RH_RF22_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT, fhch);
	return !(statusRead() & RH_RF22_FREQERR);
}

void Rfm22::setTxPower(uint8_t power){
	 spi.writeReg(RH_RF22_REG_6D_TX_POWER, power | RH_RF22_LNA_SW); // On RF23, LNA_SW must be set.
}

void Rfm22::reset(){
	spi.writeReg(RH_RF22_REG_07_OPERATING_MODE1, RH_RF22_SWRES);
	for(int i =0; i<1000;i++){

	}
}

void Rfm22::setModeIdle()
{
    if (currentMode != Rfm22ModeIdle)
    {
	setOpMode(idleMode);
	currentMode = Rfm22ModeIdle;
    }
}

void Rfm22::setModeRx()
{
    if (currentMode != Rfm22ModeRx)
    {
	setOpMode(idleMode | RH_RF22_RXON);
	currentMode = Rfm22ModeRx;
    }
}

void Rfm22::setModeTx()
{
    if (currentMode != Rfm22ModeTx)
    {
	setOpMode(idleMode | RH_RF22_TXON);
	// Hmmm, if you dont clear the RX FIFO here, then it appears that going
	// to transmit mode in the middle of a receive can corrupt the
	// RX FIFO
	resetRxFifo();
	currentMode = Rfm22ModeTx;
    }
}

void Rfm22::setOpMode(uint8_t mode)
{
    spi.writeReg(RH_RF22_REG_07_OPERATING_MODE1, mode);
}

void Rfm22::resetRxFifo(){
    spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, RH_RF22_FFCLRRX);
    spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, 0);
}

void Rfm22::resetTxFifo()
{
	spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, RH_RF22_FFCLRTX);
	spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, 0);
}

bool Rfm22::setFrequency(float centre, float afcPullInRange)
{
    uint8_t fbsel = RH_RF22_SBSEL;
    uint8_t afclimiter;
    if (centre < 240.0 || centre > 960.0) // 930.0 for early silicon
	return false;
    if (centre >= 480.0)
    {
	if (afcPullInRange < 0.0 || afcPullInRange > 0.318750)
	    return false;
	centre /= 2;
	fbsel |= RH_RF22_HBSEL;
	afclimiter = afcPullInRange * 1000000.0 / 1250.0;
    }
    else
    {
	if (afcPullInRange < 0.0 || afcPullInRange > 0.159375)
	    return false;
	afclimiter = afcPullInRange * 1000000.0 / 625.0;
    }
    centre /= 10.0;
    float integerPart = floor(centre);
    float fractionalPart = centre - integerPart;

    uint8_t fb = (uint8_t)integerPart - 24; // Range 0 to 23
    fbsel |= fb;
    uint16_t fc = fractionalPart * 64000;
    /*spiWrite(RH_RF22_REG_73_FREQUENCY_OFFSET1, 0);  // REVISIT
    spiWrite(RH_RF22_REG_74_FREQUENCY_OFFSET2, 0);
    spiWrite(RH_RF22_REG_75_FREQUENCY_BAND_SELECT, fbsel);
    spiWrite(RH_RF22_REG_76_NOMINAL_CARRIER_FREQUENCY1, fc >> 8);
    spiWrite(RH_RF22_REG_77_NOMINAL_CARRIER_FREQUENCY0, fc & 0xff);*/

    uint8_t data[] ={0, 0, fbsel, fc >> 8, fc & 0xff};
    spi.writeRegBytes(RH_RF22_REG_73_FREQUENCY_OFFSET1,data,5);
    spi.writeReg(RH_RF22_REG_2A_AFC_LIMITER, afclimiter);
    return !(statusRead() & RH_RF22_FREQERR);
}

uint8_t Rfm22::statusRead()
{
    return spi.readReg(RH_RF22_REG_02_DEVICE_STATUS);
}

bool Rfm22::setModemConfig(ModemConfigChoice index)
{
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    Rfm22::ModemConfig cfg;
    memcpy(&cfg,&MODEM_CONFIG_TABLE[index],sizeof(Rfm22::ModemConfig));
    setModemRegisters(&cfg);

    return true;
}

void Rfm22::setModemRegisters(ModemConfig* config)
{

    spi.writeReg(RH_RF22_REG_1C_IF_FILTER_BANDWIDTH,                    config->reg_1c);
    spi.writeReg(RH_RF22_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE,      config->reg_1f);
    spi.writeRegBytes(RH_RF22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE, &config->reg_20, 6);
    spi.writeRegBytes(RH_RF22_REG_2C_OOK_COUNTER_VALUE_1,              &config->reg_2c, 3);
    spi.writeReg(RH_RF22_REG_58_CHARGE_PUMP_CURRENT_TRIMMING,           config->reg_58);
    spi.writeReg(RH_RF22_REG_69_AGC_OVERRIDE1,                          config->reg_69);
    spi.writeRegBytes(RH_RF22_REG_6E_TX_DATA_RATE1,                    &config->reg_6e, 5);


}

void Rfm22::setGpioReversed(bool gpioReversed)
{
    // Ensure the antenna can be switched automatically according to transmit and receive
    // This assumes GPIO0(out) is connected to TX_ANT(in) to enable tx antenna during transmit
    // This assumes GPIO1(out) is connected to RX_ANT(in) to enable rx antenna during receive
	uint8_t data[2];
    if (gpioReversed)
    {
	// Reversed for HAB-RFM22B-BOA HAB-RFM22B-BO, also Si4432 sold by Dorji.com via Tindie.com.
    	data[0] = 0x15;
    	data[1] = 0x12;
    }
    else
    {
    	data[0] = 0x12;
    	data[1] = 0x15;
    }
    spi.writeRegBytes(RH_RF22_REG_0B_GPIO_CONFIGURATION0,data,2);
}

bool Rfm22::send(uint8_t* data, uint8_t len)
{
    bool ret = true;
    waitPacketSent();
    //TODO toto treba prerobit na burst write
    spi.writeReg(RH_RF22_REG_3A_TRANSMIT_HEADER3, _txHeaderTo);
    spi.writeReg(RH_RF22_REG_3B_TRANSMIT_HEADER2, _txHeaderFrom);
    spi.writeReg(RH_RF22_REG_3C_TRANSMIT_HEADER1, _txHeaderId);
    spi.writeReg(RH_RF22_REG_3D_TRANSMIT_HEADER0, _txHeaderFlags);
    if (!fillTxBuf(data, len))
	ret = false;
    else
	startTransmit();
//    printBuffer("send:", data, len);
    return ret;
}

bool Rfm22::waitPacketSent()
{
    while (currentMode == Rfm22ModeTx){}
    return true;
}

bool Rfm22::fillTxBuf(const uint8_t* data, uint8_t len)
{
    clearTxBuf();
    if (!len)
	return false;
    return appendTxBuf(data, len);
}

void Rfm22::clearTxBuf()
{
    _bufLen = 0;
    _txBufSentIndex = 0;
}

bool Rfm22::appendTxBuf(const uint8_t* data, uint8_t len)
{
    if (((uint16_t)_bufLen + len) > RH_RF22_MAX_MESSAGE_LEN)
	return false;
    memcpy(_buf + _bufLen, data, len);
    _bufLen += len;
//    printBuffer("txbuf:", _buf, _bufLen);
    return true;
}

void Rfm22::startTransmit()
{
    sendNextFragment(); // Actually the first fragment
    spi.writeReg(RH_RF22_REG_3E_PACKET_LENGTH, _bufLen); // Total length that will be sent
    setModeTx(); // Start the transmitter, turns off the receiver
}

void Rfm22::sendNextFragment()
{
    if (_txBufSentIndex < _bufLen)
    {
	// Some left to send?
	uint8_t len = _bufLen - _txBufSentIndex;
	// But dont send too much
	if (len > (RH_RF22_FIFO_SIZE - RH_RF22_TXFFAEM_THRESHOLD - 1))
	    len = (RH_RF22_FIFO_SIZE - RH_RF22_TXFFAEM_THRESHOLD - 1);
	spi.writeRegBytes(RH_RF22_REG_7F_FIFO_ACCESS, _buf + _txBufSentIndex, len);
//	printBuffer("frag:", _buf  + _txBufSentIndex, len);
	_txBufSentIndex += len;
    }
}

void Rfm22::readNextFragment()
{
    if (((uint16_t)_bufLen + RH_RF22_RXFFAFULL_THRESHOLD) > RH_RF22_MAX_MESSAGE_LEN)
	return; // Hmmm receiver overflow. Should never occur

    // Read the RH_RF22_RXFFAFULL_THRESHOLD octets that should be there
    spi.readRegBytes(RH_RF22_REG_7F_FIFO_ACCESS, _buf + _bufLen, RH_RF22_RXFFAFULL_THRESHOLD);
    _bufLen += RH_RF22_RXFFAFULL_THRESHOLD;
}

// Clear the FIFOs
void Rfm22::resetFifos()
{
    spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, RH_RF22_FFCLRRX | RH_RF22_FFCLRTX);
    spi.writeReg(RH_RF22_REG_08_OPERATING_MODE2, 0);
}

void Rfm22::restartTransmit()
{
    currentMode = Rfm22ModeIdle;
    _txBufSentIndex = 0;
    startTransmit();
}

void Rfm22::clearRxBuf()
{
    _bufLen = 0;
    _rxBufValid = false;
}

uint8_t Rfm22::recv(uint8_t* buf, uint8_t len)
{
    if (!available())
	return 0;

    if (buf)
    {
	if (len > _bufLen)
	    len = _bufLen;
	memcpy(buf, _buf, len);
    }
    clearRxBuf();
//    printBuffer("recv:", buf, *len);
    return len;
}

bool Rfm22::available()
{
    if (!_rxBufValid)
	setModeRx(); // Make sure we are receiving
    return _rxBufValid;
}

Rfm22::~Rfm22() {
	// TODO Auto-generated destructor stub
}
/*
 *
 * Protected functions
 *
 * */

void Rfm22::setupInterrupts(){
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);

	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	EXTI_ClearITPendingBit(EXTI_Line10);
}

void Rfm22::enableInterrupts(){
	uint8_t data[] = {RH_RF22_ENTXFFAEM | RH_RF22_ENRXFFAFULL | RH_RF22_ENPKSENT | RH_RF22_ENPKVALID | RH_RF22_ENCRCERROR | RH_RF22_ENFFERR, RH_RF22_ENPREAVAL};
	spi.writeRegBytes(RH_RF22_REG_05_INTERRUPT_ENABLE1,data,2);

}

void Rfm22::irqHandler(){

    // Read the interrupt flags which clears the interrupt
    spi.readRegBytes(RH_RF22_REG_03_INTERRUPT_STATUS1, _lastInterruptFlags, 2);

    if (_lastInterruptFlags[0] & RH_RF22_IFFERROR){
    	resetFifos(); // Clears the interrupt
    	if (currentMode == Rfm22ModeTx )
    	    restartTransmit();
    	else if (currentMode == Rfm22ModeRx)
    	    clearRxBuf();
    }
        // Caution, any delay here may cause a FF underflow or overflow
    if (_lastInterruptFlags[0] & RH_RF22_ITXFFAEM){
        	// See if more data has to be loaded into the Tx FIFO
        	sendNextFragment();
    }
    if (_lastInterruptFlags[0] & RH_RF22_IRXFFAFULL){
    	// Caution, any delay here may cause a FF overflow
    	// Read some data from the Rx FIFO
    	readNextFragment();
    }
    if (_lastInterruptFlags[0] & RH_RF22_IPKSENT){
    //	Serial.println("IPKSENT");
    	// Transmission does not automatically clear the tx buffer.
    	// Could retransmit if we wanted
    	// RH_RF22 transitions automatically to Idle
    	currentMode = Rfm22ModeIdle;
    }
    if (_lastInterruptFlags[0] & RH_RF22_IPKVALID){
    	uint8_t len = spi.readReg(RH_RF22_REG_4B_RECEIVED_PACKET_LENGTH);
    //	Serial.println("IPKVALID");

    	// May have already read one or more fragments
    	// Get any remaining unread octets, based on the expected length
    	// First make sure we dont overflow the buffer in the case of a stupid length
    	// or partial bad receives
    	if (   len >  RH_RF22_MAX_MESSAGE_LEN
    	    || len < _bufLen)
    	{
    	    currentMode = Rfm22ModeIdle;
    	    clearRxBuf();
    	    return; // Hmmm receiver buffer overflow.
    	}

    	spi.readRegBytes(RH_RF22_REG_7F_FIFO_ACCESS, _buf + _bufLen, len - _bufLen);
    	_rxHeaderTo = spi.readReg(RH_RF22_REG_47_RECEIVED_HEADER3);
    	_rxHeaderFrom = spi.readReg(RH_RF22_REG_48_RECEIVED_HEADER2);
    	_rxHeaderId = spi.readReg(RH_RF22_REG_49_RECEIVED_HEADER1);
    	_rxHeaderFlags = spi.readReg(RH_RF22_REG_4A_RECEIVED_HEADER0);
    	_bufLen = len;
    	_rxBufValid = true;
    	currentMode = Rfm22ModeIdle;
    	//_rxBufValid = true;
    }
    if (_lastInterruptFlags[0] & RH_RF22_ICRCERROR){
    //	Serial.println("ICRCERR");
    	clearRxBuf();
    	resetRxFifo();
    	currentMode = Rfm22ModeIdle;
    	setModeRx(); // Keep trying
    }
    if (_lastInterruptFlags[1] & RH_RF22_IPREAVAL){
    //	Serial.println("IPREAVAL");
    	//_lastRssi = (int8_t)(-120 + ((spiRead(RH_RF22_REG_26_RSSI) / 2)));
    	//_lastPreambleTime = millis();
    	resetRxFifo();
    	clearRxBuf();
    }
    return;
}
