#ifdef __USE_STDAFX_H
#include "stdafx.h"
#else
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef FUNC_DECL
#ifdef E710_PIO_DLL_EXPORTS
#define E710_PIO_FUNC_DECL __declspec(dllexport) WINAPI
#else
#define E710_PIO_FUNC_DECL __declspec(dllimport) WINAPI
#endif

/////////////////////////////////////////////////////////////////////////////
// DLL initialization and comm functions

int		E710_PIO_FUNC_DECL Initialise_DIO96PIO_Interface(unsigned short int deviceNumber_DIO_96);
void E710_PIO_FUNC_DECL E710PIO_Write( unsigned char  address,
							unsigned short int data);
void E710_PIO_FUNC_DECL E710PIO_Update(void);
void E710_PIO_FUNC_DECL E710PIO_Read(unsigned char  address,
							unsigned short int *data);
#ifdef __cplusplus
}
#endif