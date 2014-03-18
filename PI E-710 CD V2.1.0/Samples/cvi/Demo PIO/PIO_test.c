#include <analysis.h>
#include <utility.h>
#include <Dataacq.h>
#include <cvirte.h>		
#include <userint.h>
#include "test01.h"
#include "simulation.h"
#include "constant.h"
#include  "sequences.h"

#define  PROGRAMM_VERSION_STRING  "Physik Instrumente, 24.09.2002"

static int panelHandle;


int main (int argc, char *argv[])
{
	int index;

	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "test01.uir", main_panel)) < 0)
		return -1;

	/* display version string*/
	SetCtrlVal (main_panel, main_panel_VERSIONDISPLAY, PROGRAMM_VERSION_STRING);
     
 	DisplayPanel (panelHandle);

	init_simulation_interface( );

	RunUserInterface ();
	
	close_simulation_interface( );
	
	DiscardPanel (panelHandle);
	return 0;
}


int CVICALLBACK cb_main_panel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			 QuitUserInterface (0);
			break;
		}
	return 0;
}






int CVICALLBACK cb_read_position (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			/*Command Button: read data from PIO*/
			read_current_position();
			break;
		}
	return 0;
}

int CVICALLBACK cb_copy_position1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			/*Command Button: copy data from display panels to input panels*/
			copy_current_position();
			break;
		}
	return 0;
}

int CVICALLBACK cb_write_position (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			/*Command Button: write data to PIO*/
			write_new_position_to_controller();
			break;
		}
	return 0;
}
