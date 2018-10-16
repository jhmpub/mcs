#ifndef TIRA_DLL_H
#define TIRA_DLL_H

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <windef.h>
#endif

#define TIRA_DLL "Tira2.dll"

typedef int (WINAPI * tira_set_handler_t) (int (__stdcall *)(const char *));
typedef int (WINAPI * tira_init_t) ();
typedef int (WINAPI * tira_cleanup_t) ();
typedef int (WINAPI * tira_start_t) (int);
typedef int (WINAPI * tira_stop_t) ();
typedef int (WINAPI * tira_start_capture_t) ();
typedef int (WINAPI * tira_cancel_capture_t) ();
typedef int (WINAPI * tira_get_captured_data_t) (const unsigned char**, int*);
typedef int (WINAPI * tira_transmit_t)
       (int repeat, int frequency, const unsigned char* data, const int dataSize );
typedef int (WINAPI * tira_delete_t)(const unsigned char* ptr);

// tira dll function pointers
extern tira_init_t              tira_init;
extern tira_cleanup_t           tira_cleanup;
extern tira_set_handler_t       tira_set_handler;
extern tira_start_t             tira_start;
extern tira_stop_t              tira_stop;
extern tira_start_capture_t     tira_start_capture;
extern tira_cancel_capture_t    tira_cancel_capture;
extern tira_get_captured_data_t tira_get_captured_data;
extern tira_transmit_t          tira_transmit;
extern tira_delete_t            tira_delete;

#endif  // ifndef TIRA_DLL_H
