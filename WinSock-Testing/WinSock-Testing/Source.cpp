#ifndef UNICODE
#define UNICODE
#endif 

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


#define WM_CREATE                       0x0001
#define IDC_MAIN_BUTTON 101 
#define IDC_MAIN_EDIT 100

#define DEFAULT_PORT "80" //80 for http

#define DEFAULT_BUFLEN 10000

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
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

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	// define handle to button control window here so all cases can access it
	static HWND hEdit;
	HWND hWndExample;
	switch (uMsg)
	{

		// taken from http://www.win32developer.com/tutorial/windows/windows_tutorial_3.shtm
		// Must add the L's because I am using wWinMain instead of WinMain as in the tutorial
		// wWinMain requires wide chars or something
		// also needed to change hWnd to hwnd to match how the first parameter, specifying window handle, is defined

	case WM_CREATE:
	{
				    HWND hWndButton = CreateWindowEx(NULL,
					    L"BUTTON",
					    L"OK",
					    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
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

				    hEdit = CreateWindowEx(
					    0, // This param sets the extended window style. If I change it from WS_EX_CLIENTEDGE, as the win32dev tut had it,
					    // to 0 like the MSDN docs have it, I get a borderless text box. Cool!
					    L"EDIT",
					    L"",
					    WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
					    50,
					    100,
					    200,
					    100,
					    hwnd,
					    (HMENU)IDC_MAIN_EDIT,
					    GetModuleHandle(NULL),
					    NULL
					    );

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
				   FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				   EndPaint(hwnd, &ps);
	}

		// handle the messages arising from interaction with button/text box
	case WM_COMMAND:
		// Examine the lower word of wParam to determine what specific event occured.
		switch (LOWORD(wParam))
		{
			// in case it is the click of the defined button
		case IDC_MAIN_BUTTON:
		{
			// run winsock code

			WSADATA wsaData;

			int iResult;

			// Initialize Winsock with 2 for both major and minor winsock specification version 
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {

				MessageBox(NULL,
					(LPWSTR)L"WSAStartup failed",
					L"Information",
					MB_ICONINFORMATION);
				return 1;
			}

			struct addrinfo *result = NULL,
				*ptr = NULL,
				hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;





			// Resolve the server address and port
			iResult = getaddrinfo("www.google.com", DEFAULT_PORT, &hints, &result);
			if (iResult != 0) {
				MessageBox(NULL,
					(LPWSTR)L"getaddrinfo failed: %d\n",
					L"Information",
					MB_ICONINFORMATION);
				WSACleanup();
				return 1;
			}

			SOCKET ConnectSocket = INVALID_SOCKET;

			// Attempt to connect to the first address returned by
			// the call to getaddrinfo
			ptr = result;

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);

			if (ConnectSocket == INVALID_SOCKET) {
				MessageBox(NULL,
					(LPWSTR)L"Error at socket(): %ld\n",
					L"Information",
					MB_ICONINFORMATION);
				freeaddrinfo(result);
				WSACleanup();
				return 1;
			}


			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				MessageBox(NULL,
					(LPWSTR)L"Error connecting to server",
					L"Information",
					MB_ICONINFORMATION);
			}
			else {
				MessageBox(NULL,
					(LPWSTR)L"success",
					L"Information",
					MB_ICONINFORMATION);
			}

			// Should really try the next address returned by getaddrinfo
			// if the connect call failed
			// But for this simple example we just free the resources
			// returned by getaddrinfo and print an error message

			freeaddrinfo(result);

			if (ConnectSocket == INVALID_SOCKET) {
				MessageBox(NULL,
					(LPWSTR)"Unable to connect to server!\n",
					L"Information",
					MB_ICONINFORMATION);
				WSACleanup();
				return 1;
			}


			int recvbuflen = DEFAULT_BUFLEN;

			char recvbuf[DEFAULT_BUFLEN];


			// Send an initial buffer
			iResult = send(ConnectSocket, "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n", (int)strlen("GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n"), 0);
			if (iResult == SOCKET_ERROR) {
				MessageBox(NULL,
					(LPWSTR)L"Send Failed",
					L"Information",
					MB_ICONINFORMATION);
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}

			//HDC hdc;
			//PAINTSTRUCT ps;
			do {
				iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
				if (iResult > 0){
					MessageBox(NULL,
						(LPWSTR)L"Bytes received",
						L"Information",
						MB_ICONINFORMATION);

					/*Nope:
					hdc = BeginPaint(hwnd, &ps);
					TextOut(hdc, 70, 50, (LPCWSTR)recvbuf, strlen(recvbuf));
					EndPaint(hwnd, &ps);*/

					//SetWindowText(hWndExample, TEXT("Control string"));
					hWndExample = CreateWindowEx(0, L"EDIT", (LPCWSTR)recvbuf, WS_VISIBLE | WS_CHILD | ES_LEFT, 10, 10, 100, 100, hwnd, NULL, GetModuleHandle(NULL), NULL);
					/*MessageBox(NULL,
						(LPWSTR)recvbuf,
						L"Information",
						MB_ICONINFORMATION);*/
				}
				else if (iResult == 0){
				MessageBox(NULL,
					(LPWSTR)L"Connection closed\n",
					L"Information",
					MB_ICONINFORMATION);
				}
				else {
					MessageBox(NULL,
						(LPWSTR)L"recv failed: %d\n",
						L"Information",
						MB_ICONINFORMATION);
				}
			} while (iResult > 0);


		}
			break;
		}
		break;

		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}