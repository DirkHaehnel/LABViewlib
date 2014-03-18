#ifndef __E710PIO_DLL_H__
#define __E710PIO_DLL_H__


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _MSC_VER
#define FUNC_DECL __declspec(dllimport) WINAPI
#else
#define FUNC_DECL __stdcall
#endif


int FUNC_DECL Initialise_DIO96PIO_Interface(unsigned short int deviceNumber_DIO_96);

void FUNC_DECL Close_DIO96PIO_Interface(void);

void FUNC_DECL E710PIO_Write( unsigned char  address,
							unsigned short int data);

void FUNC_DECL E710PIO_Update(void);

void FUNC_DECL E710PIO_Read(unsigned char  address,
							unsigned short int *data);


#ifdef __cplusplus
}
#endif

#endif  // __E710PIO_DLL_H__

