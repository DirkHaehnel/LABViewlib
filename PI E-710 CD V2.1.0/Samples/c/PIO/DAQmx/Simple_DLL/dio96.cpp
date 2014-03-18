#include "stdafx.h"
#include "dio96.h"


int  m_deviceNumber_PCI_DIO_96 = 1;
// Several Tasks are needed to handle the different types of port accesses
TaskHandle DAQmxTasks[NUMBER_OF_BUS_TASKS];
int   DIO_initialisiert;
int   DIO_MX;
bool	bReading;


int  Init_DIO_mx(unsigned short int deviceNumber_PCI_DIO_96)
{
	int  dio_96_status;
	char 	strErrorMessage[500];
	char szChannelName[30];
	char szChannelSoftName[30];
	DIO_initialisiert = 0;//FALSE;
	char *szTaskNames[] = {"DATABUS_READ" ,
						"DATABUS_WRITE",
						"ADRBUS_WRITE_IND " ,
						"CTRLBUS_WRITE_IND" ,
						"CTRLBUS_READ_IND"};

	dio_96_status = 0;
	
	m_deviceNumber_PCI_DIO_96 = deviceNumber_PCI_DIO_96;
	for(int iBusIndex = 0;((iBusIndex < NUMBER_OF_BUS_TASKS)&&(dio_96_status == 0));iBusIndex++)
	{
		//Create a task for each port and direction needed
		dio_96_status = DAQmxCreateTask(szTaskNames[iBusIndex], &DAQmxTasks[iBusIndex]);

		if(iBusIndex == DATABUS_READ_IND)
		{
			//The Databus read task consists of two 8bit input channels
	   		sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,DATABUS_LOWBYTE_PORTNR);
			sprintf(szChannelSoftName,"InputPort%d",DATABUS_LOWBYTE_PORTNR);
			dio_96_status = DAQmxCreateDIChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,DATABUS_HIGHBYTE_PORTNR);
			sprintf(szChannelSoftName,"InputPort%d",DATABUS_HIGHBYTE_PORTNR);
			dio_96_status = DAQmxCreateDIChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			//As the PCI-DIO-96 card only allows unidirectional port access, only one of the two tasks 
			// for the Data Bus may be running, either the read or the write task. This is being controlled 
			// with the boolean variable bReading. That way, the tasks are switched only when necessary, and 
			// time is being saved. Note that it might be necessary to implement synchronization methods
			// here!
			dio_96_status = DAQmxStartTask(DAQmxTasks[iBusIndex]);
			bReading = true;


		}
		else if(iBusIndex == DATABUS_WRITE_IND)
		{
			//This is the second task accessing the data bus, it will not be started until the first write
			// access is issued.
	   		sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,DATABUS_LOWBYTE_PORTNR);
			sprintf(szChannelSoftName,"OutputPort%d",DATABUS_LOWBYTE_PORTNR);
			dio_96_status = DAQmxCreateDOChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,DATABUS_HIGHBYTE_PORTNR);
			sprintf(szChannelSoftName,"OutputPort%d",DATABUS_HIGHBYTE_PORTNR);
			dio_96_status = DAQmxCreateDOChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			
		}
		// all other ports must be used only in one direction, so the tasks are running from the beginning
		else if(iBusIndex == ADRBUS_WRITE_IND)
		{
	   		sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,ADRBUS_PORTNR);
			sprintf(szChannelSoftName,"OutputPort%d",ADRBUS_PORTNR);
			dio_96_status = DAQmxCreateDOChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			dio_96_status = DAQmxStartTask(DAQmxTasks[iBusIndex]);

		}
		else if(iBusIndex == CTRLBUS_WRITE_IND)
		{
	   		sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,CTRLBUS_WRITE_PORTNR);
			sprintf(szChannelSoftName,"OutputPort%d",CTRLBUS_WRITE_PORTNR);
			dio_96_status = DAQmxCreateDOChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			dio_96_status = DAQmxStartTask(DAQmxTasks[iBusIndex]);

		}
		else if(iBusIndex == CTRLBUS_READ_IND)
		{
	   		sprintf(szChannelName,"Dev%d/port%d",m_deviceNumber_PCI_DIO_96,CTRLBUS_READ_PORTNR);
			sprintf(szChannelSoftName,"InputPort%d",CTRLBUS_READ_PORTNR);
			dio_96_status = DAQmxCreateDIChan (DAQmxTasks[iBusIndex],szChannelName,szChannelSoftName,
				DAQmx_Val_ChanForAllLines);
			dio_96_status = DAQmxStartTask(DAQmxTasks[iBusIndex]);

		}
		
		DIO_initialisiert = 1;//TRUE;
		DIO_MX = 1;
	      	
	}

	if(dio_96_status)
	{
		// an error occured, translate the error code into a string
		char szDAQStr[100];
		DAQmxGetErrorString (dio_96_status,szDAQStr,100);
		sprintf(strErrorMessage,"%s<%s %d : %s", "Init_DA_Brds", "Error Code",
			dio_96_status,szDAQStr);
			printf ("DIO Board Error,%s\n", strErrorMessage);
		DIO_initialisiert = 0;//FALSE
		
	}
	return(DIO_initialisiert);
}
/**
* This function is used to send data to the port(s) controlled by the specified task. 
*  If the Task has two ports, the 16 bit parameter iData is splitted into two 8-bit values
*  Otherwise, the data is written directly to the task
*/
void DIO_WriteToTask(unsigned short iTaskIndex,unsigned short iData)
{
	int status;
	int32 iNSamples;
	switch(iTaskIndex)
	{
	case(DATABUS_WRITE_IND):
		{
			// If the last action on the data bus was reading, the input task must
			// be stopped and the output task started
			if (bReading)
			{
				DAQmxStopTask(DAQmxTasks[DATABUS_READ_IND]);
				DAQmxStartTask(DAQmxTasks[iTaskIndex]);
				bReading = false;
			}
			uInt8 _iData[2];
			_iData[0] = iData&0xFF;
			_iData[1] = (iData&0xFF00)/256;
			status = DAQmxWriteDigitalU8 (DAQmxTasks[iTaskIndex], 1,true, 1.0, DAQmx_Val_GroupByChannel ,
				_iData, &iNSamples, NULL);

		};break;
	case(ADRBUS_WRITE_IND):
		{
			status = DAQmxWriteDigitalU8 (DAQmxTasks[iTaskIndex], 1,true, 1.0, DAQmx_Val_GroupByChannel ,
				(uInt8*)&iData, &iNSamples, NULL);
		};break;
	case(CTRLBUS_WRITE_IND):
		{
			status = DAQmxWriteDigitalU8 (DAQmxTasks[iTaskIndex], 1,true, 1.0, DAQmx_Val_GroupByChannel ,
				(uInt8*)&iData, &iNSamples, NULL);
		};break;


	}
}
/**
* Function to read an 8 or 16-bit value from the port(s) of a task
* Depending of the number of ports controlled by the task
*/
void DIO_ReadFromTask(unsigned short iTaskIndex,unsigned short *iData)
{
	int status;
	int32 iNSamples;
	unsigned short iTest;
	switch(iTaskIndex)
	{
	case(DATABUS_READ_IND):
		{
			// If the last action on the data bus was writing, the output task must
			// be stopped and the input task started
			if (!bReading)
			{
				DAQmxStopTask(DAQmxTasks[DATABUS_WRITE_IND]);
				DAQmxStartTask(DAQmxTasks[iTaskIndex]);
				bReading = true;
			}
			uInt8 _iData[2];
			status = DAQmxReadDigitalU8 (DAQmxTasks[iTaskIndex], 1, 1.0, DAQmx_Val_GroupByChannel ,
				_iData, 2, &iNSamples, NULL);
			iTest = _iData[0] + (_iData[1] *256);
			*iData = iTest;
			iTest = *iData;

		};break;
	case(CTRLBUS_READ_IND):
		{
			status = DAQmxReadDigitalU8 (DAQmxTasks[iTaskIndex], 1, 1.0, DAQmx_Val_GroupByChannel ,
				(uInt8*)iData, 1, &iNSamples, NULL);
		};break;
	}
}
void  Close_DIO( void)
{
	int dio_96_status = 0;
	if (DIO_MX&&DIO_initialisiert)
	{
		char szDAQStr[500];
		for(int iBusIndex = 0;((iBusIndex < NUMBER_OF_BUS_TASKS)&&(dio_96_status == 0));iBusIndex++)
		{
			// stop all running tasks.
			dio_96_status = DAQmxStopTask(DAQmxTasks[iBusIndex]);
			if(dio_96_status)
				DAQmxGetErrorString (dio_96_status,szDAQStr,500);
			dio_96_status = DAQmxWaitUntilTaskDone( DAQmxTasks[iBusIndex],10.0);
			
			if(dio_96_status)
				DAQmxGetErrorString (dio_96_status,szDAQStr,500);
			

			// for some reason, this next line causes the program to hang:
			//dio_96_status = DAQmxClearTask(DAQmxTasks[iBusIndex]);
			//if(dio_96_status)
			//	DAQmxGetErrorString (dio_96_status,szDAQStr,500);
		}
	}
}








