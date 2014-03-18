// Simple_DLL.cpp : Definiert die Initialisierungsroutinen für die DLL.
//
#define E710_PIO_DLL_EXPORTS
#define E710_PIO_DLL



#include "stdafx.h"
#include "Simple_DLL.h"
#include ".\simple_dll.h"
#include ".\E710_PIO.h"
#include "dio96.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Hinweis:
//
//		Wird diese DLL dynamisch an die MFC-DLLs gebunden,
//		muss bei allen von dieser DLL exportierten Funktionen,
//		die MFC-Aufrufe durchführen, das Makro AFX_MANAGE_STATE
//		direkt am Beginn der Funktion eingefügt sein.
//
//		Beispiel:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//		// Hier normaler Funktionsrumpf
//		}
//
//		Es ist sehr wichtig, dass dieses Makro in jeder Funktion
//		vor allen MFC-Aufrufen angezeigt wird. Dies bedeutet, dass es
//		als erste Anweisung innerhalb der Funktion ausgeführt werden
//		muss, sogar vor jeglichen Deklarationen von Objektvariablen,
//		da ihre Konstruktoren Aufrufe in die MFC-DLL generieren
//		könnten.
//
//		Siehe MFC Technical Notes 33 und 58 für weitere
//		Details.
//

// CSimple_DLLApp

BEGIN_MESSAGE_MAP(CSimple_DLLApp, CWinApp)
END_MESSAGE_MAP()


// CSimple_DLLApp-Erstellung

CSimple_DLLApp::CSimple_DLLApp()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}

 
CSimple_DLLApp::~CSimple_DLLApp()
{
	Close_DIO();
}
// Das einzige CSimple_DLLApp-Objekt

CSimple_DLLApp theApp;


// CSimple_DLLApp Initialisierung

BOOL CSimple_DLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
void  write_adress( unsigned char aktuelle_adress)
{
	
	DIO_WriteToTask(ADRBUS_WRITE_IND,aktuelle_adress);
}


void  write_ctrl( int rw_leitung, int ioen_leitung, int  reset_leitung)
{
	unsigned char  steuerbus_status;
	
	steuerbus_status = 0;
	if  (rw_leitung)  steuerbus_status = steuerbus_status | RW_LINE_BIT;
	if  (ioen_leitung) steuerbus_status = steuerbus_status | IOEN_LINE_BIT;
	if  (reset_leitung) steuerbus_status = steuerbus_status | RESET_LINE_BIT;

	DIO_WriteToTask(CTRLBUS_WRITE_IND, steuerbus_status);
	
}


void  write_data( unsigned char  aktuelle_daten_low, unsigned char  aktuelle_daten_high)
{

	DIO_WriteToTask(DATABUS_WRITE_IND,aktuelle_daten_low + aktuelle_daten_high * 256);
}


void  read_data( unsigned char  *aktuelle_daten_low, unsigned char  *aktuelle_daten_high)
{

	unsigned short aktuelle_daten;
	DIO_ReadFromTask(DATABUS_READ_IND,&aktuelle_daten);
	*aktuelle_daten_low = aktuelle_daten & 0xFF;
	*aktuelle_daten_high = (aktuelle_daten & 0xFF00)>>8;
}

int  E710_PIO_FUNC_DECL Initialise_DIO96PIO_Interface(unsigned short int deviceNumber_DIO_96)	//DIO Initialisation
{
	int status;

	status = Init_DIO_mx(deviceNumber_DIO_96);
	
	
	return status;
}
void  E710_PIO_FUNC_DECL  E710PIO_Write( unsigned char  address,
							unsigned short int data)
{
	unsigned char low_data_byte, high_data_byte;
	
	low_data_byte = data & 0xFF;
	high_data_byte = (data  & 0xFF00)>> 8;

	write_adress( address);
	write_data( low_data_byte, high_data_byte);
	write_ctrl( 0, 1, 1);  /*RW , STRB, LDAC */
	write_ctrl( 0, 0, 1);  /*RW , STRB, LDAC */
	write_ctrl( 0, 1, 1);  /*RW , STRB, LDAC */
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC */

}


//void   E710_ldac_command( void)
void  E710_PIO_FUNC_DECL  E710PIO_Update( void)
{
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC*/
	write_ctrl( 1, 1, 0);  /*RW , STRB, LDAC*/
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC*/

}


//void  E710_read_command( unsigned char  address,
void E710_PIO_FUNC_DECL  E710PIO_Read( unsigned char  address,
							unsigned short int *data)
{
	unsigned char low_data_byte, high_data_byte;

	//Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_INPUT);
	//Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_INPUT);
	write_adress( address);
//	write_ctrl( TRUE, TRUE, TRUE);  /*RW , STRB, LDAC */
//	write_ctrl( TRUE, FALSE, TRUE);  /*RW , STRB, LDAC */
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC */
	write_ctrl( 1, 0, 1);  /*RW , STRB, LDAC */
	
	read_data( &low_data_byte, &high_data_byte);
//	write_ctrl( TRUE, TRUE, TRUE);  /*RW , STRB, LDAC */
	write_ctrl( 1, 1, 1);  /*RW , STRB, LDAC */
	
	//Config_DIO_registers( DATABUS_LOWBYTE_PORTNR, PORTMODUS_OUTPUT);
	//Config_DIO_registers( DATABUS_HIGHBYTE_PORTNR, PORTMODUS_OUTPUT);
	
	*data = low_data_byte   |  ((high_data_byte << 8) & 0xFF00);
}