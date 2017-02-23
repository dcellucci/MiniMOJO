/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver
  
  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/

// ---------------------------------------------------------

// INA3221 ADDRESS PINS AND SLAVE ADDRESSES
// |----------|---------------|
// |    A0    | SLAVE ADDRESS |
// |----------|---------------|
// |    GND   |    100 0000   |
// |    VS    |    100 0001   |
// |    SDA   |    100 0010   |
// |    SCL   |    100 0011   |
// |----------|---------------|

// ---------------------------------------------------------

#ifndef INA3221_H
#define INA3221_H

// ---------------------------------------------------------

/*----------------------------------------------------------
  INA3221 I2C ADDRESS
----------------------------------------------------------*/
#define INA3221_ADDR    0x40

/*----------------------------------------------------------
  INA3221 REGISTER MAP
----------------------------------------------------------*/
#define CONFIG_REG      0x00
#define SHUNT_V_1_REG   0x01
#define BUS_V_1_REG     0x02
#define SHUNT_V_2_REG   0x03
#define BUS_V_2_REG     0x04
#define SHUNT_V_3_REG   0x05
#define BUS_V_3_REG     0x06
#define CRITICAL_1_REG  0x07
#define WARNING_1_REG   0x08
#define CRITICAL_2_REG  0x09
#define WARNING_2_REG   0x0A
#define CRITICAL_3_REG  0x0B
#define WARNING_3_REG   0x0C

#define MASK_ENABLE_REG 0x0F

/*----------------------------------------------------------
  CONVERSIONS/OTHER DEF
----------------------------------------------------------*/
#define CURRENT_CONV 5
#define VOLTAGE_CONV 125

#define I2C_COMM_TIMEOUT 1000

/*----------------------------------------------------------
  @Functions
----------------------------------------------------------*/
uint16 read_config();

uint16 read_critical_voltage_1();
uint16 read_critical_voltage_2();
uint16 read_critical_voltage_3();

uint16 read_warning_voltage_1();
uint16 read_warning_voltage_2();
uint16 read_warning_voltage_3();

uint16 read_bus_voltage_1();
uint16 read_bus_voltage_2();
uint16 read_bus_voltage_3();

uint16 read_shunt_voltage_1();
uint16 read_shunt_voltage_2();
uint16 read_shunt_voltage_3();

// I2C
void initI2C();
void I2CwriteByte(uint8_t reg, uint8_t data);
uint8_t I2CreadByte(uint8_t reg);
uint8_t I2CreadBytes(uint8_t reg, uint8_t * dest, uint8_t count);

#endif /* INA3221_H */