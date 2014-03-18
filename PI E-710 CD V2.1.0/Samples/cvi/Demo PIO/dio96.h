void  Init_DIO( void);
void  Close_DIO( void);
void  Config_DIO_registers( int registernumber, unsigned char configuration);
void  Read_DIO( void);
void  Write_DIO( void);
void  DIO_dummy_write_register( int registernumber, unsigned char wert);
unsigned char  DIO_dummy_read_register( int registernumber);
void  DIO_write_register( int registernumber, unsigned char wert);
unsigned char  DIO_read_register( int registernumber);



#define  PORTMODUS_INPUT   1
#define  PORTMODUS_OUTPUT   0


