/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver
  
  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/

#include <Wire.h>


void set_default_current_alert_levels();
void set_default_current_warning_levels();


uint16 read_config(){
    return I2CreadByte(CONFIG_REG);
}


uint16 read_critical_voltage_1(){
    return I2CreadByte(CRITICAL_1_REG) >> 3;
}


uint16 read_critical_voltage_2(){
    return I2CreadByte(CRITICAL_2_REG) >> 3;
}


uint16 read_critical_voltage_3(){
    return I2CreadByte(CRITICAL_3_REG) >> 3;
}


uint16 read_warning_voltage_1(){
    return I2CreadByte(WARNING_1_REG) >> 3;
}


uint16 read_warning_voltage_2(){
    return I2CreadByte(WARNING_2_REG) >> 3;
}


uint16 read_warning_voltage_3(){
    return I2CreadByte(WARNING_3_REG) >> 3;
}


void set_critical_voltage(uint8 channel, uint16 current){
    current = current*CURRENT_CONV;
    
    if (channel==CHANNEL_1){
        I2CwriteByte(CRITICAL_1_REG, current<<3);
    }
    else if (channel==CHANNEL_2){
        I2CwriteByte(CRITICAL_2_REG, current<<3);
    }
    else if (channel==CHANNEL_3){
        I2CwriteByte(CRITICAL_3_REG, current<<3);
    }
}

// below code modified from original: SFE_LSMDS1.cpp by Jim Lindblom

void initI2C()
{
	Wire.begin();	// Initialize I2C library
}


// Wire.h read and write protocols
void I2CwriteByte(uint8_t reg, uint8_t data)
{
	Wire.beginTransmission(INA3221_ADDR);  // Initialize the Tx buffer
	Wire.write(reg);                       // Put slave register address in Tx buffer
	Wire.write(data);                      // Put data in Tx buffer
	Wire.endTransmission();                // Send the Tx buffer
}


uint8_t I2CreadByte(uint8_t reg)
{
	int timeout = I2C_COMM_TIMEOUT;
	uint8_t data;                          // "data" will store the register data	
	
	Wire.beginTransmission(INA3221_ADDR);  // Initialize the Tx buffer
	Wire.write(reg);	                   // Put slave register address in Tx buffer
	Wire.endTransmission(true);            // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(INA3221_ADDR, (uint8_t) 1);  // Read one byte from slave register address 
	while ((Wire.available() < 1) && (timeout-- > 0))
		delay(1);
	
	if (timeout <= 0)
		return 255;	//! Bad! 255 will be misinterpreted as a good value.
	
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}


uint8_t I2CreadBytes(uint8_t reg, uint8_t * dest, uint8_t count)
{  
	int timeout = I2C_COMM_TIMEOUT;
	Wire.beginTransmission(INA3221_ADDR);   // Initialize the Tx buffer
	// Next send the register to be read. OR with 0x80 to indicate multi-read.
	Wire.write(reg | 0x80);     // Put slave register address in Tx buffer

	Wire.endTransmission(true);             // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
	Wire.requestFrom(INA3221_ADDR, count);  // Read bytes from slave register address 
	while ((Wire.available() < count) && (timeout-- > 0))
		delay(1);
	if (timeout <= 0)
		return -1;
	
	for (int i=0; i<count;)
	{
		if (Wire.available())
		{
			dest[i++] = Wire.read();
		}
	}
	return count;
}