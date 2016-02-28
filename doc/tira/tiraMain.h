// $Header: E:\\CVS/GirPlugin/tiraMain.h,v 1.6 2003/09/20 22:25:47 Administrator Exp $

#ifndef TiraMainH
#define TiraMainH

#define TIRA_API extern "C" __declspec(dllexport)
#define TIRA_API_H extern "C"

#define  MaxIRData 20

TIRA_API_H  __stdcall int tira_init ();
TIRA_API_H  __stdcall int tira_cleanup ();

#ifndef   NO_CALLBACK
typedef int    (__stdcall * tira_six_byte_cb) (const char * eventstring);
TIRA_API_H  __stdcall int tira_set_handler ( tira_six_byte_cb cb );
#endif

TIRA_API_H __stdcall int tira_start ( int PortID );
// Activates Tira

TIRA_API_H  __stdcall int tira_stop ();
// Disactivates Tira



TIRA_API_H  __stdcall int tira_start_capture ();
// Activates capture mode. After that Tira will expect some IR data

TIRA_API_H  __stdcall int tira_cancel_capture ();

TIRA_API_H  __stdcall int tira_get_captured_data (const unsigned char** data, int* size );
// Returns pointer to the captured IR data. Receiving application assumes
// ownership of the pointer and is responsible for freeing the memory

TIRA_API_H  __stdcall int tira_transmit (
   int repeat, int frequency, const unsigned char* data, const dataSize );

TIRA_API_H  __stdcall int tira_delete (const unsigned char* ptr);

// Alternative to the callback function
// returns
TIRA_API_H __stdcall int tira_get_ir_data ( char IRDataString[], int* StringLen );

TIRA_API_H __stdcall int tira_get_captured_data_vb (unsigned char Data[], int* size );
// This function was added to allow use of the API from VisualBasic
// Due to restrictions on the argument types that can be accessed from VB,
// this function copies data into out array DATA and the size (in/out)
// size must indicate maximum number of bytes that can be written into
// Data
// the function fails (returns non-zero value) if the size of the input data array is not enough
//

#endif

//
