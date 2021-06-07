// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>

#define MAX_LOADSTRING 100

using namespace std;

struct czlowiek
{
	int pietro_poczatkowe;
	int pietro_koncowe;
	int x;
	int y;
};

vector<czlowiek> kolejka;
vector<czlowiek> w_windzie;
vector<czlowiek> oczekiwanie_winda;


struct winda
{
	int pietro;
	int kierunek;
	bool zatrzymanie;
};

winda winda_s = { 0, 0, 1 };          //poczatkowo winda jest na pierwszym pietrze, nie rusza sie



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int wartosc = 0;
int wartosc0 = 0;
int wartosc1 = 0;
int wartosc2 = 0;
int wartosc3 = 0;
int wartosc4 = 0;
int zmienna = 0;
int ilosc = 0;
bool koniec;

// buttons
HWND hwndButton;

RECT winda_m = { 603, 13, 847, 759 };

RECT pietro_0_winda = { 100, 680, 840, 750 };
RECT pietro_1_winda = { 100, 530, 840, 600 };
RECT pietro_2_winda = { 100, 380, 840, 450 };
RECT pietro_3_winda = { 100, 230, 840, 300 };
RECT pietro_4_winda = { 100, 80, 840, 150 };

RECT pietro_0 = { 100, 680, 590, 750 };
RECT pietro_1 = { 100, 530, 590, 600 };
RECT pietro_2 = { 100, 380, 590, 450 };
RECT pietro_3 = { 100, 230, 590, 300 };
RECT pietro_4 = { 100, 80, 590, 150 };


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void zarys_windy(HDC hdc, RECT* rect)
{
	Graphics graphics(hdc);
	Pen pen1(Color(255, 255, 0, 0), 3);
	GraphicsPath path;
	path.AddLine(603, 645 - wartosc, 844, 645 - wartosc);
	path.AddLine(844, 645 - wartosc, 844, 755 - wartosc);
	path.AddLine(603, 755 - wartosc, 844, 755 - wartosc);
	graphics.DrawPath(&pen1, &path);
}



void MyOnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255), 2);
	graphics.DrawRectangle(&pen, 600, 10, 250, 750);
	graphics.DrawLine(&pen, 100, 755, 600, 755);
	graphics.DrawLine(&pen, 100, 605, 600, 605);
	graphics.DrawLine(&pen, 100, 455, 600, 455);
	graphics.DrawLine(&pen, 100, 305, 600, 305);
	graphics.DrawLine(&pen, 100, 155, 600, 155);

	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, 60, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(Color(255, 0, 0, 0));
	
	PointF      pointF(50, 650);
	graphics.DrawString(L"0", -1, &font, pointF, &solidBrush);
	PointF      pointF1(50, 500);
	graphics.DrawString(L"1", -1, &font, pointF1, &solidBrush);
	PointF      pointF2(50, 350);
	graphics.DrawString(L"2", -1, &font, pointF2, &solidBrush);
	PointF      pointF3(50, 200);
	graphics.DrawString(L"3", -1, &font, pointF3, &solidBrush);
	PointF      pointF4(50, 50);
	graphics.DrawString(L"4", -1, &font, pointF4, &solidBrush);

	Pen pen1(Color(255, 255, 0, 0), 3);
	GraphicsPath path;
	path.AddLine(603, 645, 844, 645);
	path.AddLine(844, 645, 844, 755);
	path.AddLine(603, 755, 844, 755);
	graphics.DrawPath(&pen1, &path);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	
}




int OnCreate(HWND window)
{
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1500, 850, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       
	
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 700,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON01,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 670,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON02,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 640,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON03,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 610,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON04,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 430,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON20,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 400,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON21,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 340,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON23,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 310,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON24,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 130,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON40,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 100,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON41,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 70,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON42,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 40,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON43,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 580,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON10,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 520,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON12,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 490,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON13,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 460,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON14,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 280,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON30,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 250,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON31,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 220,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON32,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		10, 160,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON34,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	// create button and store the handle       



	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON01:
			break;
		case ID_BUTTON02:
			break;
		case ID_BUTTON03:
			break;
		case ID_BUTTON04:
			break;
		case ID_BUTTON10:
			break;
		case ID_BUTTON12:
			break;
		case ID_BUTTON13:
			break;
		case ID_BUTTON14:
			break;
		case ID_BUTTON20:
			break;
		case ID_BUTTON21:
			break;
		case ID_BUTTON23:
			break;
		case ID_BUTTON24:
			break;
		case ID_BUTTON30:
			break;
		case ID_BUTTON31:
			break;
		case ID_BUTTON32:
			break;
		case ID_BUTTON34:
			break;
		case ID_BUTTON40:
			break;
		case ID_BUTTON41:
			break;
		case ID_BUTTON42:
			break;
		case ID_BUTTON43:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		MyOnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
