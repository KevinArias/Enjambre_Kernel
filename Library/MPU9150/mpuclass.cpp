/*
 * i2cdevice.cpp
 *
 *  Created on: Jul 26, 2015
 *      Author: kevin
 */

#include "mpuclass.h"

namespace i2cdevn {

i2cdevice::i2cdevice(unsigned char address, char bus, int buffersize) {
	this->buffsize=buffersize;
	this->buffer = new unsigned char[buffersize];

		switch(bus)
		{
		case 1:
		if ((this->fd = open("/dev/i2c-1", O_RDWR)) < 0) {
			std::cout << "Falló apertura del bus i2c" << std::endl;
		}

		if (ioctl(this->fd, I2C_SLAVE, address) < 0) {
			std::cout << "Falló al conectar al sensor " << std::endl;
		}

		break;

		case 2:
			if ((this->fd = open("/dev/i2c-2", O_RDWR)) < 0) {
				std::cout << "Falló apertura del bus i2c" << std::endl;
			}

			if (ioctl(this->fd, I2C_SLAVE, address) < 0) {
				std::cout << "Falló al conectar al sensor " << std::endl;
			}
		break;
		}

}
void i2cdevice::writebyte(unsigned char data)
{
	this->bufferout[0] = data;

			if (write(this->fd, this->bufferout, 1) != 1) {
				std::cout << "Falló al escribir archivo" << std::endl;
			}
}
void i2cdevice::writebuff()
{
	if (write(this->fd, this->buffer, this->buffsize) != this->buffsize) {
					std::cout << "Falló al escribir archivo" << std::endl;
				}
}
void i2cdevice::writeread()
{
	if (write(this->fd, this->out, sizeof(this->out)) != sizeof(this->out)) {
						std::cout << "Falló al escribir archivo" << std::endl;
					}
	if (read(this->fd, this->in, sizeof(this->in)) != sizeof(this->in)) {
		std::cout << "Falló al leer buffer" << std::endl;
	}
}
void i2cdevice::readreg(unsigned char data)
{
	this->writebyte(data);

	if (read(this->fd, this->buffer, this->buffsize) != this->buffsize) {
		std::cout << "Falló al leer buffer" << std::endl;
	}
}
void i2cdevice::readpoint()
{
	if (read(this->fd, this->buffer, this->buffsize) != this->buffsize) {
			std::cout << "Falló al leer buffer" << std::endl;
		}
}
i2cdevice::~i2cdevice() {
	// TODO Auto-generated destructor stub
}




AK8975::AK8975(char bus): i2cdevice(0x0c,bus,2){

}
void AK8975::readMag(){
	this->buffsize=2;
	this->buffer = new unsigned char [2];

	this->buffer[0] = 0x0a ;	//wake up sensor
	this->buffer[1] = 0x01 ;
	this->writebuff();
	usleep(10000);

	this->buffsize=6;
	this->buffer = new unsigned char [6];
	this->readreg(0x03);

}
AK8975::~AK8975(){
	}

AS5048::AS5048(unsigned char address, char bus, int buffersize):
											i2cdevice(address,bus,buffersize){

}
float AS5048::readang()
{
	this->buffsize = 2;
	this->buffer = new unsigned char [2];
	this->readreg(0x0FF);
	return (this->buffer[0] << 6 | (this->buffer[1] & 0x3E))* 360.0 / 16384.0;
}
AS5048::~AS5048() {
	// TODO Auto-generated destructor stub
}

MPU9150::MPU9150(char bus) : i2cdevice(0x068,bus,2){
	this->buffer[0] = 0x6b ;	//wake up sensor
	this->buffer[1] = 0x00 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x37 ;	//wake up sensor
	this->buffer[1] = 0x02 ;
	this->writebuff();
	usleep(10000);
	/*this->buffer[0] = 0x24 ;	//Wait for Data at Slave0
	this->buffer[1] = 0x40 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x25 ;	//Set i2c address at slave 0 at 0x0C
	this->buffer[1] = 0x8c ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x26 ;	//Set where reading at slave 0 starts
	this->buffer[1] = 0x02 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x27 ;	//set offset at start reading and enable
	this->buffer[1] = 0x88 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x28 ;	//set i2c address at slv1 at 0x0C
	this->buffer[1] = 0x0c ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x29 ;	//Set where reading at slave 1 starts
	this->buffer[1] = 0x0a ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x2a ;	//Enable at set length to 1
	this->buffer[1] = 0x81 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x64 ;	//overvride register
	this->buffer[1] = 0x01 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x67 ;	 //set delay rate
	this->buffer[1] = 0x03 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x01 ;
	this->buffer[1] = 0x80 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x34 ;	 //set i2c slv4 delay
	this->buffer[1] = 0x04 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x64 ;	//override register
	this->buffer[1] = 0x00 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x6a ;	//clear usr setting
	this->buffer[1] = 0x00 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x64 ;	//override register
	this->buffer[1] = 0x01 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x6a ;	//enable master i2c mode
	this->buffer[1] = 0x20 ;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x34 ;	//disable slv4
	this->buffer[1] = 0x13 ;
	this->writebuff();
	usleep(10000);*/

	this->buffsize=14;
	this->buffer = new unsigned char [14];
}
void MPU9150::readMPU(){
	this->readreg(0x3b);

}
MPU9150::~MPU9150(){

}


HMC5883L::HMC5883L(char bus) : i2cdevice(0x01E,bus,2){
	this->buffer[0] = 0x00;
	this->buffer[1] = 0x70;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x01;
	this->buffer[1] = 0xA0;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x02;
	this->buffer[1] = 0x00;
	this->writebuff();
	usleep(10000);
	this->buffer[0] = 0x00;
	this->buffer[1] = 0x18;
	this->writebuff();
	usleep(10000);

	this->buffsize=6;
	this->buffer = new unsigned char [6];

}
void HMC5883L::readmag(){

	this->readreg(0x03);
	//this->xmag=(int16_t)(this->buffer[1] | ((int16_t)this->buffer[0] << 8));
	//this->ymag=(int16_t)(this->buffer[3] | ((int16_t)this->buffer[2] << 8));
	//this->zmag=(int16_t)(this->buffer[5] | ((int16_t)this->buffer[4] << 8));

}
HMC5883L::~HMC5883L(){

}

ADXL345::ADXL345(char bus) : i2cdevice(0x053,bus,2){
	this->buffer[0]=0x2D;
	this->buffer[1]=0x08;
	this->writebuff();
	usleep(10000);
	this->buffer[0]=0x31;
	this->buffer[1]=0x08;
	this->writebuff();
	usleep(10000);
	this->buffer[0]=0x2C;
	this->buffer[1]=0x09;
	this->writebuff();
	usleep(10000);
	this->buffsize=6;
	this->buffer = new unsigned char[6];
}
void ADXL345::readacc(){
	this->readreg(0x32);
//this->xacc=(float)((int16_t) (this->buffer[0]|(this->buffer[1] << 8)))*0.004;
//this->yacc=(float)((int16_t) (this->buffer[2]|(this->buffer[3] << 8)))*0.004;
//this->zacc=(float)((int16_t)( this->buffer[4]|(this->buffer[5] << 8)))*0.004;

}
ADXL345::~ADXL345(){

}

ITG3200::ITG3200(char bus) : i2cdevice(0x68,bus,2){
	this->buffer[0]=0x3E;
	this->buffer[1]=0x80;
	this->writebuff();
	usleep(10000);
	this->buffer[0]=0x16;
	this->buffer[1]=0x1E;
	this->writebuff();
	usleep(10000);
	this->buffer[0]=0x15;
	this->buffer[1]=0x0A;
	this->writebuff();
	usleep(10000);
	this->buffer[0]=0x3E;
	this->buffer[1]=0x00;
	this->writebuff();
	usleep(10000);
	this->buffer = new unsigned char [8];
	this->buffsize=8;
}
void ITG3200::readgyro(){

	this->readreg(0x1B);
/*
this->temp=
35.0+(((int16_t)((int)this->buffer[0]<<8 | (int) this->buffer[1]))+13200)/280.0;
this->xgyro =
(float)((int16_t)(((int)this->buffer[2]<<8)|((int)this->buffer[3])))/14.375;
this->ygyro =
(float)((int16_t)(((int)this->buffer[4]<<8)|((int)this->buffer[5])))/14.375;
this->zgyro =
(float)((int16_t)(((int)this->buffer[6]<<8)|((int)this->buffer[7])))/14.375;
*/
}
ITG3200::~ITG3200(){

}

} /* namespace i2cdevice */
