#include <NIDAQmx.h>
#define  DATABUS_LOWBYTE_PORTNR  8
#define  DATABUS_HIGHBYTE_PORTNR  7
#define  ADRBUS_PORTNR  10
#define  CTRLBUS_WRITE_PORTNR  9
#define  CTRLBUS_READ_PORTNR  4

#define  RW_LINE_BIT     0x01
#define  IOEN_LINE_BIT   0x02
#define  RESET_LINE_BIT  0x04   
#define  IRQ_LINE_BIT  0x01 


int  Init_DIO_mx(unsigned short int deviceNumber_PCI_DIO_96);
void  Close_DIO( void);

void DIO_ReadFromTask(unsigned short iTaskIndex,unsigned short *iData);
void DIO_WriteToTask(unsigned short iTaskIndex,unsigned short iData);
unsigned char  DIO_read_register( int registernumber);

typedef enum TaskIndices{
DATABUS_READ_IND ,
DATABUS_WRITE_IND,
ADRBUS_WRITE_IND  ,
CTRLBUS_WRITE_IND ,
CTRLBUS_READ_IND,
NUMBER_OF_BUS_TASKS
}TASKINDICES;



