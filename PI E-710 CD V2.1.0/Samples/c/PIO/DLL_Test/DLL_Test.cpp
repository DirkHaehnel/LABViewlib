/*
*
* Test Program to clarify the use of the Simple_DLL for PIO communication with the E710
* can be used with both kinds of API, DAQmx or NI-DAQ
*/

#include "stdafx.h"
#include "..\NIDAQ_7_2\Simple_DLL\E710_PIO.h" // the same for both versions
#include <stdio.h>
#include <conio.h>
int _tmain(int argc, _TCHAR* argv[])
{
	int i = 1;
	unsigned int k;
	int iResult = Initialise_DIO96PIO_Interface(i);
	printf("Initialise_DIO96PIO_Interface(%d) = %d\n",i,iResult);
	if(iResult)
	{
		do{
			for (int c = 0;c < 4;c++)
			{
				// Read Position Values for each channel
				E710PIO_Read(c,(unsigned short*)&iResult);
				printf("Read ch %d:%d\t",c,iResult);
			}
			k = (unsigned int)getch();
			printf("\nyou typed %c , converting to %d\n type q to continue",k,(k-(int)'0')*1000);
			// Write a value corresponding to typed character to channel 0
			E710PIO_Write(0,(k-'0')*1000);
			E710PIO_Update();
			// the result can be seen either in closed loop by reading the actual position
			// or by commanding a relative move of 0 and querying the target position
			Sleep(100);
		}while(k!='q');
		do{
			printf("%c",13);
			for (int c = 0;c < 4;c++)
			{
				E710PIO_Read(c,(unsigned short*)&iResult);
				printf("Read ch %d:%d\t",c,iResult);
			}
			Sleep(100);
		}while(!kbhit());
		int iRequests = 0;
		// do a timing benchmark on the communication speed
		LARGE_INTEGER pF;
		QueryPerformanceFrequency(&pF);
		LARGE_INTEGER StartTime,StopTime;
		do{
			iRequests = 0;
			QueryPerformanceCounter(&StartTime);
			do{
				printf("%c",13);
				for (int c = 0;c < 4;c++)
				{
					E710PIO_Read(c,(unsigned short*)&iResult);
					printf("Read ch %d:%d\t",c,iResult);
				}
				iRequests+=4;
				//Sleep(100);
			}while(!kbhit());
			k = (unsigned int)getch();
		QueryPerformanceCounter(&StopTime);
		printf("Time Per Request = %f \n",1.0*(StopTime.QuadPart-StartTime.QuadPart)/pF.QuadPart/iRequests);
		}while(k!='x');
	}
	else
	{
		printf("Board couldn't be opened\nPress any key");
		k = (unsigned int)getch();
	}
 	return 0;
}

