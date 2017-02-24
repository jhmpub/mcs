#ifndef TIRA_H
#define TIRA_H

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


#endif  // ifndef TIRA_H
