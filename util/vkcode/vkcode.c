#include <windows.h>

static unsigned control;
static unsigned shift;
static unsigned alt;
const char g_szClassName[] = "myWindowClass";
const char g_szAppName[] = "Virtual Key Code";
void msgBox(unsigned);

// handle OS messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
    switch(msg)
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (lParam &= 1<<29)
                alt = 1;
                
            if (wParam == VK_CONTROL)
                control = 1;
                
            if (wParam == VK_SHIFT)
                shift = 1;
                
            if (wParam != VK_MENU && wParam != VK_CONTROL && wParam != VK_SHIFT) 
                msgBox(wParam);
                
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (lParam |= ~(1<<29))
                alt = 0;
                
            if (wParam == VK_CONTROL)
                control = 0;
                
            if (wParam == VK_SHIFT)
                shift = 0;
                
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void msgBox(unsigned vkCode) {

    char szVkCode[128];

    if (alt)
       vkCode = (vkCode << 8) | VK_MENU;   
       
    if (control)
       vkCode = (vkCode << 8) | VK_CONTROL;   
       
    if (shift)
       vkCode = (vkCode << 8) | VK_SHIFT;   
       
    sprintf(szVkCode,"0x%08X", vkCode);
    MessageBox(NULL, szVkCode, "Result", MB_OK);
    alt=control=shift=0;
}    


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    
    // register the window class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed", "Error",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // create the window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        g_szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed", "Error",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // start the message loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

