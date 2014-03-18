#include "stdafx.h"
#include "dio96.h"

#define  CHIP_82C55A_ANZAHL  4

#define  CHIP_82C55A_IO_REGISTER_ANZAHL  3

#define  IO_REGISTERZAHL              CHIP_82C55A_ANZAHL * CHIP_82C55A_IO_REGISTER_ANZAHL




typedef struct 
{
   unsigned char read_port_content;
   unsigned char write_port_content;
   unsigned char port_mode;
} tchip_82C55_io_register;


 
	

tchip_82C55_io_register  io_register[IO_REGISTERZAHL];

int  m_deviceNumber_PCI_DIO_96 = 1;
short  deviceNumberCode_PCI_DIO_96 = 200;

int   DIO_initialisiert;



int  Init_DIO(unsigned short int deviceNumber_PCI_DIO_96)
{
	int  chip_index;
	int  register_index_1;
	int  dio_96_status;
	char 	strErrorMessage[500];
	
	DIO_initialisiert = 0;//FALSE;
	
	dio_96_status = Init_DA_Brds (deviceNumber_PCI_DIO_96, &deviceNumberCode_PCI_DIO_96);
	if  (dio_96_status == 0)
	{
		m_deviceNumber_PCI_DIO_96 = deviceNumber_PCI_DIO_96;
	   		
	    for  (register_index_1 = 0; register_index_1 < IO_REGISTERZAHL; register_index_1++)
		{

    		dio_96_status = DIG_Prt_Config (deviceNumber_PCI_DIO_96, register_index_1, 0, 0);// mode 0, input
			io_register[register_index_1].read_port_content = 0;
		   	io_register[register_index_1].write_port_content = 0;
		    io_register[register_index_1].port_mode = PORTMODUS_INPUT;

		}
		
		DIO_initialisiert = 1;//TRUE;
	      	
	}
	else
	{
		sprintf(strErrorMessage,"%s<%s %d : ", "Init_DA_Brds", "Error Code",
			dio_96_status);
			printf ("DIO Board Error,%s\n", strErrorMessage);
		
	}
	return(DIO_initialisiert);
}

	
void  Close_DIO( void)
{

	int  registerindex;
	
	for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
	{
		DIO_write_register( registerindex, 0);
	}
}


void  Config_DIO_registers( int registernumber, unsigned char configuration)
{
	int status;
	short handshakeStatus;
	if  (DIO_initialisiert)
	{
		status = DIG_Prt_Status (m_deviceNumber_PCI_DIO_96, registernumber, &handshakeStatus);
		if  (configuration == PORTMODUS_INPUT)
		{
			status = DIG_Prt_Config (m_deviceNumber_PCI_DIO_96, registernumber, 0, 0);// mode 0, input
			io_register[registernumber].port_mode = PORTMODUS_INPUT;}
		else
		{
			status = DIG_Prt_Config (m_deviceNumber_PCI_DIO_96, registernumber, 0, 1);// mode 0, output
			io_register[registernumber].port_mode = PORTMODUS_OUTPUT;
		}
	}	
}


void  Read_DIO( void)
{
	int registerindex;
	int status;
	if  (DIO_initialisiert)
	{
		for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
		{
	   		if  (io_register[registerindex].port_mode == PORTMODUS_INPUT)
	   		{
				status = DIG_In_Prt(m_deviceNumber_PCI_DIO_96,registerindex,(long*)(&io_register[registerindex].read_port_content));
				if (status)
				{
					char strErrorMessage[100];
					sprintf(strErrorMessage,"%s<%s %d", "DIG_In_Prt", "Error Code", status);
					printf ("DIO Board Error,%s\n", strErrorMessage);
				}
		   	}
		}
	}
	
}


void  Write_DIO( void)
{
	int registerindex;
	int status;
	if  (DIO_initialisiert)
	{
		for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
		{
	   		if  (io_register[registerindex].port_mode == PORTMODUS_OUTPUT)
	   		{
				status = DIG_Out_Prt(m_deviceNumber_PCI_DIO_96,registerindex,(long)io_register[registerindex].write_port_content);
				if (status)
				{
					char strErrorMessage[100];
					sprintf(strErrorMessage,"%s<%s %d", "DIG_Out_Prt", "Error Code", status);
					printf ("DIO Board Error,%s\n", strErrorMessage);
				}
	   		}
		}
	}

}

void  DIO_write_register( int registernumber, unsigned char wert)
{
	int status;
	if  (DIO_initialisiert)
	{
		io_register[registernumber].write_port_content = wert;
		status = DIG_Out_Prt(m_deviceNumber_PCI_DIO_96,registernumber,(long)io_register[registernumber].write_port_content);
		if (status)
		{
			char strErrorMessage[100];
			sprintf(strErrorMessage,"%s<%s %d", "DIG_Out_Prt", "Error Code", status);
			printf ("DIO Board Error,%s\n", strErrorMessage);
		}
	}
}


unsigned char  DIO_read_register( int registernumber)
{
	int status;
	if  (DIO_initialisiert)
	{
		status = DIG_In_Prt(m_deviceNumber_PCI_DIO_96,registernumber,(long*)&io_register[registernumber].read_port_content);
		if (status)
		{
			char strErrorMessage[100];
			sprintf(strErrorMessage,"%s<%s %d", "DIG_In_Prt", "Error Code", status);
			printf ("DIO Board Error,%s\n", strErrorMessage);
		}
		return(io_register[registernumber].read_port_content);
	}
	return(io_register[registernumber].read_port_content);
}

