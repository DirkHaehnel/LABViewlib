#include <userint.h>
#include <utility.h>
#include <analysis.h>
#include  "constant.h"
#include "simulation.h"
#include  "sequences.h"
#include "test01.h"


void  read_current_position(void)
{
	unsigned short int  pio_data;
	
	/* read value from PIO output register at address 0 = axis 1 */
	E710_read_command( 0, &pio_data);
	/* display value from PIO address 0*/
	SetCtrlVal (main_panel, main_panel_DISPLAY_POSITION1, pio_data);

	/* read value from PIO output register at address 1 = axis 2 */
	E710_read_command( 1, &pio_data);
	/* display value from PIO address 1*/
	SetCtrlVal (main_panel, main_panel_DISPLAY_POSITION2, pio_data);

	/* read value from PIO output register at address 2 = axis 3 */
	E710_read_command( 2, &pio_data);
	/* display value from PIO address 2*/
	SetCtrlVal (main_panel, main_panel_DISPLAY_POSITION3, pio_data);
	
	/* read value from PIO output register at address 3 = axis 4 */
	E710_read_command( 3, &pio_data);
	/* display value from PIO address 3*/
	SetCtrlVal (main_panel, main_panel_DISPLAY_POSITION4, pio_data);
	
	
}

void  copy_current_position(void)  
{
	unsigned short int  pio_data;
	
	/*copy the values which were read from the PIO*/
	GetCtrlVal (main_panel, main_panel_DISPLAY_POSITION1, &pio_data);
	SetCtrlVal (main_panel, main_panel_INPUT_POSITION1, pio_data);

	GetCtrlVal (main_panel, main_panel_DISPLAY_POSITION2, &pio_data);
	SetCtrlVal (main_panel, main_panel_INPUT_POSITION2, pio_data);

	GetCtrlVal (main_panel, main_panel_DISPLAY_POSITION3, &pio_data);
	SetCtrlVal (main_panel, main_panel_INPUT_POSITION3, pio_data);

	GetCtrlVal (main_panel, main_panel_DISPLAY_POSITION4, &pio_data);
	SetCtrlVal (main_panel, main_panel_INPUT_POSITION4, pio_data);
}

void  write_new_position_to_controller(void)
{
	unsigned short int  pio_data;

	GetCtrlVal (main_panel, main_panel_INPUT_POSITION1, &pio_data);
	/*write to PIO input register at address number 0 = axis 1*/
	E710_write_command( 0, pio_data);

	GetCtrlVal (main_panel, main_panel_INPUT_POSITION2, &pio_data);
	/*write to PIO input register at address number 1 = axis 2*/
	E710_write_command( 1, pio_data);

	GetCtrlVal (main_panel, main_panel_INPUT_POSITION3, &pio_data);
	/*write to PIO input register at address number 2 = axis 3*/
	E710_write_command( 2, pio_data);
	
	GetCtrlVal (main_panel, main_panel_INPUT_POSITION4, &pio_data);
	/*write to PIO input register at address number 3 = axis 4*/
	E710_write_command( 3, pio_data);
	
	/*after writing valid values to all input registers, 
	  data can be transferred to the controller by activating the LD line*/
	E710_ldac_command( );

}



