

#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>

#define WM_CREATE                       0x0001
#define IDC_MAIN_BUTTON 101 
#define IDC_MAIN_EDIT 100

/* easy. Need to include this to update control styles to current OS user specified theme 
http://msdn.microsoft.com/en-us/library/windows/desktop/bb773175%28v=vs.85%29.aspx
*/
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
		// this is what specifies the use of the wc window class! 
		// because you set wc's class name to CLASS_NAME, then registered it with the OS using RegisterClass
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

		// taken from http://www.win32developer.com/tutorial/windows/windows_tutorial_3.shtm
		// Must add the L's because I am using wWinMain instead of WinMain as in the tutorial
		// wWinMain requires wide chars or something
		// also needed to change hWnd to hwnd to match how the first parameter, specifying window handle, is defined

	case WM_CREATE:
	{
		HWND hWndButton=CreateWindowEx(NULL, 
			L"BUTTON",
			L"OK",
			WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
			50,
			220,
			100,
			24,
			hwnd,
			(HMENU)IDC_MAIN_BUTTON,
			GetModuleHandle(NULL),
			NULL);
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);

		static HWND hEdit=CreateWindowEx(
			0, // This param sets the extended window style. If I change it from WS_EX_CLIENTEDGE, as the win32dev tut had it,
			   // to 0 like the MSDN docs have it, I get a borderless text box. Cool!
			L"EDIT",
			L"",
			WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
			50,
			100,
			200,
			100,
			hwnd,
			(HMENU)IDC_MAIN_EDIT,
			GetModuleHandle(NULL),
			NULL
		);

		
		::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
		HFONT hFont = ::CreateFontIndirect(&ncm.lfMessageFont);
		::SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
	}
	
	break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			// All painting occurs here, between BeginPaint and EndPaint.
			//BeginPaint fills in the ps PAINTSTRUCT structure. 
			// It will provide the update region by filling in ps.rcPaint
            HDC hdc = BeginPaint(hwnd, &ps);

			// fille in the update region
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
	
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}