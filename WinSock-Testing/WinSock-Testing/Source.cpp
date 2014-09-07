#ifndef UNICODE
#define UNICODE
#endif 

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock with 2 for both major and minor winsock specification version 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;



#define DEFAULT_PORT "80" //80 for http

	// Resolve the server address and port
	iResult = getaddrinfo("www.google.com", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
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
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}




	/* Instead of getting addr info by ip address as in the MS docs example...*/
	// Resolve the server address and port
	/* iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}*/

	/* I will get addr info by looking up hostname as in the StackOverflow example:
	http://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c
	*/




	return 0;
}