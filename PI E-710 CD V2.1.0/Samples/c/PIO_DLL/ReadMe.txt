PIO_DLL_example\E710PIO_DLL\ contains
	- E710PIO_DLL.dll, DLL written with LabWindows/CVI 5.5.1
	- E710PIO_DLL.h, the header file for E710PIO_DLL.DLL
	- the E710PIO_DLL.lib for each programming environment is located in the appropriate subfolder

PIO_DLL_example\PIO_Example_Code_msvc\ contains
	- PIO_Example_Code_usingDLL.exe an XY-scan demo program using E710PIO_DLL
	- the complete a source code for this demo program, written with Microsoft Visual C++ 6.0.

The DLL was tested with the PIO_Example_Code_using DLL.exe under Windows 2000 and XP.
The driver used for the PCI-DIO-96 card was NI-DAQ 6.9.2

E710PIO_DLL.DLL is a DLL permitting use of the E-710 PIO Interface with the PCI-DIO-96 card  and the NI-DAQ Driver 6.9.2 from National Instruments. This DLL contains following functions:

int FUNC_DECL Initialise_DIO96PIO_Interface(unsigned short int deviceNumber_DIO_96);
Permits initializing the PCI-DIO-96 card for use with the PIO Interface of the E710.
Returns 1 if successfull, 0 otherwise.

void FUNC_DECL Close_DIO96PIO_Interface(void);
Permits closing the PCI-DIO-96 card.

void FUNC_DECL E710PIO_Write( unsigned char  address, unsigned short int data);
Permits writing a position data value for an axis.
"data" is the position value, ranging from 0 to 65535 (where 0 corresponds to the PIO Input Low Limit and 65535 to the PIO High Limit)
"address" is the axis number minus 1 (ranging from 0 to 3).

void FUNC_DECL E710PIO_Update(void);
Permits transfering position data values into a second-level latch which activates any target on all axes simultaneously.
The axes will only be moved to the new target positions if they are in closed-loop mode.

void FUNC_DECL E710PIO_Read(unsigned char  address, unsigned short int *data);
Permits reading the position data of an axis.
"data" is the position value, ranging from 0 to 65535 (where 0 corresponds to the PIO Input Low Limit and 65535 to the PIO High Limit)
"address" is the axis number minus 1 (ranging from 0 to 3).


Thursday, 11 September 2003.
