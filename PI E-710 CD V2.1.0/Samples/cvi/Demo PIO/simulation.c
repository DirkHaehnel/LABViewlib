#include <userint.h>
#include "constant.h"
#include "dio96.h"

#define  DATABUS_LOWBYTE_PORTNR  8
#define  DATABUS_HIGHBYTE_PORTNR  7
#define  ADRBUS_PORTNR  10
#define  CTRLBUS_WRITE_PORTNR  9
#define  CTRLBUS_READ_PORTNR  4

#define  RW_LINE_BIT     0x01
#define  IOEN_LINE_BIT   0x02
#define  RESET_LINE_BIT  0x04   
#define  IRQ_LINE_BIT  0x01 


int  irq_handle( void)
{
	int  irq_pending;
	
	irq_pending = FALSE;
	if  (DIO_read_register(CTRLBUS_READ_PORTNR) & IRQ_LINE_BIT)
	{
		irq_pending = TRUE;
	}
	
	return(irq_pending);
}


void  write_adress( unsigned char aktuelle_adress)
{

	DIO_write_register( ADRBUS_PORTNR, aktuelle_adress);
}



void  write_ctrl( int rw_leitung, int ioen_leitung, int  reset_leitung)
{
	unsigned char  steuerbus_status;
	
	steuerbus_status = 0;
	if  (rw_leitung)  steuerbus_status = steuerbus_status | RW_LINE_BIT;
	if  (ioen_leitung) steuerbus_status = steuerbus_status | IOEN_LINE_BIT;
	if  (reset_leitung) steuerbus_status = steuerbus_status | RESET_LINE_BIT;

	DIO_write_register( CTRLBUS_WRITE_PORTNR, steuerbus_status);
	
}


void  write_data( unsigned char  aktuelle_daten_low, unsigned char  aktuelle_daten_high)
{

	DIO_write_register( DATABUS_LOWBYTE_PORTNR, aktuelle_daten_low);
	DIO_write_register( DATABUS_HIGHBYTE_PORTNR, aktuelle_daten_high);
}


void  read_data( unsigned char  *aktuelle_daten_low, unsigned char  *aktuelle_daten_high)
{

	*aktuelle_daten_low = DIO_read_register( DATABUS_LOWBYTE_PORTNR);
	*aktuelle_daten_high = DIO_read_register( DATABUS_HIGHBYTE_PORTNR);
}



void  init_simulation_interface( void)
{
	Init_DIO( );
	Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_INPUT);
	Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_INPUT);
	Config_DIO_registers( ADRBUS_PORTNR, PORTMODUS_OUTPUT);
	Config_DIO_registers( CTRLBUS_WRITE_PORTNR, PORTMODUS_OUTPUT);
	Config_DIO_registers( CTRLBUS_READ_PORTNR, PORTMODUS_INPUT);


	write_adress( 0);
	
}


void  close_simulation_interface( void)
{
	Close_DIO( );
}



void   E710_write_command( unsigned char  address,
							unsigned short int data)
{
	unsigned char low_data_byte, high_data_byte;
	
	low_data_byte = data & 0xFF;
	high_data_byte = (data >> 8) & 0xFF;
	
	Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_OUTPUT);
	Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_OUTPUT);
	write_adress( address);
	write_data( low_data_byte, high_data_byte);
	write_ctrl( 0, 1, 1);  /*RW , STRB, LDAC */
	write_ctrl( 0, 0, 1);  /*RW , STRB, LDAC */
	write_ctrl( 0, 1, 1);  /*RW , STRB, LDAC */
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC */

}


void   E710_ldac_command( void)
{
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC*/
	write_ctrl( 1, 1, 0);  /*RW , STRB, LDAC*/
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC*/

}


void  E710_read_command( unsigned char  address,
							unsigned short int *data)
{
	unsigned char low_data_byte, high_data_byte;

	Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_INPUT);
	Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_INPUT);
	write_adress( address);
	write_ctrl( TRUE, TRUE, TRUE);  /*RW , STRB, LDAC */
	write_ctrl( TRUE, FALSE, TRUE);  /*RW , STRB, LDAC */
	
	read_data( &low_data_byte, &high_data_byte);
	write_ctrl( TRUE, TRUE, TRUE);  /*RW , STRB, LDAC */
	
	Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_OUTPUT);
	Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_OUTPUT);
	
	*data = low_data_byte   |  ((high_data_byte << 8) & 0xFF00);
}


