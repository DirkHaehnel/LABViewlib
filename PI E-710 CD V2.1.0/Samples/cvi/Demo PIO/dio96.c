#include <Dataacq.h>
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "constant.h"
#include "dio96.h"
#define  CHIP_82C55A_ANZAHL  4
#define  PORT_A_REG_ADRESS        0
#define  PORT_B_REG_ADRESS        1
#define  PORT_C_REG_ADRESS        2
#define  CHIP_82C55A_IO_REGISTER_ANZAHL  3
#define  CONFIG_REG_ADRESS        3
#define  CHIP_82C55A_REGISTERSATZLAENGE            4
#define  CONFIG_PORT_C2_INPUT        0x01 
#define  CONFIG_PORT_C1_INPUT        0x08
#define  CONFIG_PORT_B_INPUT        0x02 
#define  CONFIG_PORT_A_INPUT        0x10
#define  CONFIG_MODUS_0               0x80
#define  IO_REGISTERZAHL              CHIP_82C55A_ANZAHL * CHIP_82C55A_IO_REGISTER_ANZAHL


typedef struct 
{
   unsigned char read_port_content;
   unsigned char write_port_content;
   unsigned char port_mode;
   unsigned char *port_adress;
   int config_register_number;
   unsigned char configurationsbits;
} tchip_82C55_io_register;


typedef  struct
{
	unsigned char port_inhalt;
	unsigned char *port_adress;
} tchip_82C55_config_register;   
	

tchip_82C55_io_register  io_register[IO_REGISTERZAHL];
tchip_82C55_config_register  config_register[CHIP_82C55A_ANZAHL];

int  deviceNumber_PCI_DIO_96 = 1;
short  deviceNumberCode_PCI_DIO_96 = 200;
unsigned int pci_port_startadress;
unsigned int pci_port_mem_size = 0x1000;
unsigned char			*pci_port_memoryadress;
int 					pci_port_memoryhandle;
int   DIO_initialisiert;



void  Init_DIO( void)
{
	int  chip_index;
	int  register_index_1;
	int  register_index_2;

	int  dio_96_status;
	int map_status;
	char 	strErrorMessage[100];
	
	DIO_initialisiert = FALSE;
	
	dio_96_status = Init_DA_Brds (deviceNumber_PCI_DIO_96, &deviceNumberCode_PCI_DIO_96);
	if  (dio_96_status == 0)
	{
		dio_96_status = Get_DAQ_Device_Info (deviceNumber_PCI_DIO_96, ND_BASE_ADDRESS, &pci_port_startadress);
		if  (dio_96_status == 0)
		{

			map_status = MapPhysicalMemory (pci_port_startadress, pci_port_mem_size, &pci_port_memoryadress,&pci_port_memoryhandle);
    	
    		if  (map_status == 1)
    		{
		   		register_index_1 = 0;
	    		for  (chip_index = 0; chip_index < CHIP_82C55A_ANZAHL; chip_index++)
			    {
			    	config_register[chip_index].port_inhalt = 
			    		CONFIG_MODUS_0 | CONFIG_PORT_A_INPUT | CONFIG_PORT_B_INPUT | CONFIG_PORT_C1_INPUT | CONFIG_PORT_C2_INPUT;
			    	config_register[chip_index].port_adress = 
			    		pci_port_memoryadress + CHIP_82C55A_REGISTERSATZLAENGE * chip_index + CONFIG_REG_ADRESS;
    		
			    	*(config_register[chip_index].port_adress) = config_register[chip_index].port_inhalt;
    	
					for  (register_index_2 = 0; register_index_2 < CHIP_82C55A_IO_REGISTER_ANZAHL; register_index_2++)
			        {
			        	io_register[register_index_1].read_port_content = 0;
		    	    	io_register[register_index_1].write_port_content = 0;
		        		io_register[register_index_1].port_mode = PORTMODUS_INPUT;
		        		io_register[register_index_1].port_adress = 
		        			pci_port_memoryadress + CHIP_82C55A_REGISTERSATZLAENGE * chip_index + register_index_2;
			        	switch (register_index_2)
			        	{
			        		case PORT_A_REG_ADRESS: 
		    	    				io_register[register_index_1].configurationsbits = CONFIG_PORT_A_INPUT;
		        					break;	
		        			case PORT_B_REG_ADRESS: 
		        					io_register[register_index_1].configurationsbits = CONFIG_PORT_B_INPUT;
		        					break;	
			        		case PORT_C_REG_ADRESS: 
			        				io_register[register_index_1].configurationsbits = CONFIG_PORT_C1_INPUT | CONFIG_PORT_C2_INPUT;
			        				break;	
		    	    	}
		        		io_register[register_index_1].config_register_number = chip_index;
		        		register_index_1 = register_index_1 + 1;
			        }
        
		    	}
		    	
		    	DIO_initialisiert = TRUE;
	    	
	    	}
	    	else
	    	{
		  		Fmt(strErrorMessage,"%s<%s<%i", "MapPhysicalMemory, Fehler-Nr:", map_status);
				MessagePopup ("DIO Board Error", strErrorMessage);
	    	}
    	
    	}

		else
		{
	  		Fmt(strErrorMessage,"%s<%s<%i", "Get_DAQ_Device_Info, Fehler-Nr:", dio_96_status);
			MessagePopup ("DIO Board Error", strErrorMessage);
		
		}
	}
	else
	{
	  	Fmt(strErrorMessage,"%s<%s<%i", "Init_DA_Brds, Fehler-Nr:", dio_96_status);
		MessagePopup ("DIO Board Error", strErrorMessage);
		
	}
	

}

	
void  Close_DIO( void)
{

	int  registerindex;
	
	for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
	{
		DIO_write_register( registerindex, 0);
	}

	if  (DIO_initialisiert)
	{
		UnMapPhysicalMemory (pci_port_memoryhandle);
	}
}


void  Config_DIO_registers( int registernumber, unsigned char configuration)
{
	int config_register_number;

	
	if  (DIO_initialisiert)
	{
		config_register_number = io_register[registernumber].config_register_number;

		if  (configuration == PORTMODUS_INPUT)
		{
			io_register[registernumber].port_mode = PORTMODUS_INPUT;
			config_register[config_register_number].port_inhalt = 
				config_register[config_register_number].port_inhalt | io_register[registernumber].configurationsbits;
		}
		else
		{
			io_register[registernumber].port_mode = PORTMODUS_OUTPUT;
			config_register[config_register_number].port_inhalt = 
				config_register[config_register_number].port_inhalt & ~io_register[registernumber].configurationsbits;
		}
	
	
		*(config_register[config_register_number].port_adress) = config_register[config_register_number].port_inhalt;
	}	
}


void  Read_DIO( void)
{
	int registerindex;
	
	if  (DIO_initialisiert)
	{
		for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
		{
	   		if  (io_register[registerindex].port_mode == PORTMODUS_INPUT)
	   		{
		   		io_register[registerindex].read_port_content = *(io_register[registerindex].port_adress);
		   	}
		}
	}
	
}


void  Write_DIO( void)
{
	int registerindex;
	
	if  (DIO_initialisiert)
	{
		for  (registerindex = 0; registerindex < IO_REGISTERZAHL; registerindex++)
		{
	   		if  (io_register[registerindex].port_mode == PORTMODUS_OUTPUT)
	   		{
	   			*(io_register[registerindex].port_adress) = io_register[registerindex].write_port_content;
	   		}
		}
	}

}


void  DIO_dummy_write_register( int registernumber, unsigned char wert)
{
	io_register[registernumber].write_port_content = wert;
}


unsigned char  DIO_dummy_read_register( int registernumber)
{
	return(io_register[registernumber].read_port_content);
}

void  DIO_write_register( int registernumber, unsigned char wert)
{
	io_register[registernumber].write_port_content = wert;
    *(io_register[registernumber].port_adress) = io_register[registernumber].write_port_content;
/*    *(io_register[registernumber].port_adress) = wert;*/
}


unsigned char  DIO_read_register( int registernumber)
{
	io_register[registernumber].read_port_content = *(io_register[registernumber].port_adress);
	return(io_register[registernumber].read_port_content);
/*	return(*(io_register[registernumber].port_adress));*/
}

