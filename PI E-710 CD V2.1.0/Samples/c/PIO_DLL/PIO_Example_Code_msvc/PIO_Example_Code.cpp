// PIO_Example_Code_usingDLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <stdio.h>
#include <conio.h>
#include "E710PIO_DLL.h"

unsigned short int DIO96_DeviceNr = 1;

double	PIO_min = 0.0, PIO_max = 100.0;
int		x_channel = 0, y_channel = 1;

unsigned short int	Convert_RealPos_to_PIO_Data(unsigned short int real_pos){
	unsigned short int pio_data;
	pio_data = (unsigned short int)(65535*(real_pos-PIO_min)/(PIO_max-PIO_min) + 0.5);
	return pio_data;
}

void Report_Position(int x_pos, int y_pos) {
	unsigned short int posx, posy;

	printf("\nx, y theo: %d, %d\n", x_pos, y_pos);
	E710PIO_Read(x_channel, &posx);
	E710PIO_Read(y_channel, &posy);
	printf("x, y read: %d, %d\n", posx, posy);
}

int main(int argc, char* argv[])
{
 	int i, status;
	unsigned short int y_pos, x_pos0, x_pos1;

	int n_loopx2 = 10, pause_ms = 100, x_step=50, y_step=5;

	printf("\n(press key to start..)");
	getch();
	printf("\n\n");

	status = Initialise_DIO96PIO_Interface(DIO96_DeviceNr);
	if(status) {
		x_pos0 = Convert_RealPos_to_PIO_Data(x_step);
		x_pos1 = Convert_RealPos_to_PIO_Data(0);

		printf("\n");
		for(i=0; i<n_loopx2; i++) {
			printf("\nLoop %d", i);

			y_pos = Convert_RealPos_to_PIO_Data(y_step*i++);
			E710PIO_Write( y_channel, y_pos);
			E710PIO_Update();
			Sleep(pause_ms);
			Report_Position(x_pos1, y_pos);

			E710PIO_Write( x_channel, x_pos0);
			E710PIO_Update();
			Sleep(pause_ms);
			Report_Position(x_pos0, y_pos);

			y_pos = Convert_RealPos_to_PIO_Data(y_step*i);
			E710PIO_Write( y_channel, y_pos);
			E710PIO_Update();
			Sleep(pause_ms);
			Report_Position(x_pos0, y_pos);

			E710PIO_Write( x_channel, x_pos1);
			E710PIO_Update();
			Sleep(pause_ms);
			Report_Position(x_pos1, y_pos);
		}

		Close_DIO96PIO_Interface();
	}

	printf("\n\n(press key to exit..)");
	getch();

	return 0;
}
