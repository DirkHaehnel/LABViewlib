#include <nidaq.h>
#define  DATABUS_LOWBYTE_PORTNR  8
#define  DATABUS_HIGHBYTE_PORTNR  7
#define  ADRBUS_PORTNR  10
#define  CTRLBUS_WRITE_PORTNR  9
#define  CTRLBUS_READ_PORTNR  4

#define  RW_LINE_BIT     0x01
#define  IOEN_LINE_BIT   0x02
#define  RESET_LINE_BIT  0x04   
#define  IRQ_LINE_BIT  0x01 

int  Init_DIO(unsigned short int deviceNumber_PCI_DIO_96);
void  Close_DIO( void);
void  Config_DIO_registers( int registernumber, unsigned char configuration);
void  Read_DIO( void);
void  Write_DIO( void);
void  DIO_write_register( int registernumber, unsigned char wert);
unsigned char  DIO_read_register( int registernumber);



#define  PORTMODUS_INPUT   1
#define  PORTMODUS_OUTPUT   0


