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

vector<winda> ruchwindy;

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

bool wolne = false;
bool zajete[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int miejsce[8] = { 610, 630, 650, 670, 690, 710, 730, 750 };

int dzialanie_windy = 0;        // 0 - stoi i sie nie rusza
								// 1 - ruch
								// 2 - otwieranie drzwi
								// 3 - stoi z otwartymi drzwiami
								// 4 - zamykanie drzwi

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

void przesuwanie();
void ustaw_winda();
void tworzenie_czlowieka(HDC hdc);


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

void ruch_ludzi(HDC hdc)
{
	int p;
	if (!wolne)
	{
		for (int i = 0; i < w_windzie.size(); i++)
		{
			if (w_windzie[i].pietro_koncowe == winda_s.pietro)
			{
				for (int j = 0; j < 8; j++)
				{
					if (w_windzie[i].x == miejsce[j])
						zajete[j] = 0;
				}
			}
		}
		wolne = true;
	}
	if (w_windzie.empty())
	{
		p = 0;
		for (int i = 0; i < w_windzie.size(); i++)
		{
			if (w_windzie[i].pietro_koncowe == winda_s.pietro && w_windzie[i].x > 50)
			{
				w_windzie[i].x = w_windzie[i].x - 5;
				p++;
			}
		}
	}
	int k = 0;
	if (p == 0)
	{
		koniec = true;
		while (!w_windzie.empty() && k < w_windzie.size())
		{
			if (w_windzie[k].pietro_koncowe == winda_s.pietro)
				w_windzie.erase(w_windzie.begin() + k);
			else
				k++;
		}
		wolne = false;
	}
}

void ruch_ludzi_2(HDC hdc)
{
	int p = 0;
	bool jest;
	if (!oczekiwanie_winda.empty())
	{
		int k = 0;
		while (!oczekiwanie_winda.empty() && k < oczekiwanie_winda.size())
		{
			jest = false;
			if (oczekiwanie_winda[k].pietro_poczatkowe == winda_s.pietro)
			{
				for (int j = 0; j < 8; j++)
				{
					if (!zajete[j])
					{
						if (oczekiwanie_winda[k].x == miejsce[j])
						{
							zajete[j] = 1;
							w_windzie.push_back(oczekiwanie_winda[k]);
							oczekiwanie_winda.erase(oczekiwanie_winda.begin() + k);
							jest = true;
						}
						else
						{
							oczekiwanie_winda[k].x = oczekiwanie_winda[k].x + 5;
							p++;
						}
						break;
					}
				}
			}
			if (!jest)
				k++;
		}
	}
	if (p == 0 && !ilosc)
	{
		przesuwanie();
		if (ilosc == 0)
		{
			dzialanie_windy = 4;
			koniec = false;
		}
	}

}

void przesuwanie()
{
	ilosc = 0;
	for (int i = 0; i < w_windzie.size(); i++)
	{
		if (w_windzie[i].pietro_poczatkowe == winda_s.pietro)
			ilosc++;
	}
	switch (winda_s.pietro)
	{
	case 0: 
		wartosc0 = wartosc0 - ilosc * 25;
		break;
	case 1:
		wartosc1 = wartosc1 - ilosc * 25;
		break;
	case 2:
		wartosc2 = wartosc2 - ilosc * 25;
		break;
	case 3:
		wartosc3 = wartosc3 - ilosc * 25;
		break;
	case 4:
		wartosc4 = wartosc4 - ilosc * 25;
		break;
	}
}

void bez_ruchu()
{
	koniec = false;
	if (winda_s.pietro != 0)
	{
		if (kolejka.empty())
			zmienna++;
		else
			koniec = true;
	}
	if (zmienna == 300)
	{
		for (int i = winda_s.pietro; i > -1; i--)
		{
			if (i != 0)
			{
				ruchwindy.push_back({ i, -1, 0 });
			}
			else
			{
				ruchwindy.push_back({ i, -1, 1 });
			}
		}
		koniec = true;
	}
	if (winda_s.pietro == 0 && !kolejka.empty())
	{
		koniec = true;
		ustaw_winda();
	}
}

void przesuwanie2()
{
	int p;
	int ile = 0;
	if (!kolejka.empty())
	{
		for (int i = 0; i < kolejka.size(); i++)
		{
			if (kolejka[i].pietro_poczatkowe == winda_s.pietro)
			{
				p = 0;
				for (int j = 0; j < i; j++)
				{
					if (kolejka[j].pietro_poczatkowe == kolejka[i].pietro_poczatkowe)
						p++;
				}
				if (kolejka[i].x != 575 - p * 25)
				{
					kolejka[i].x = kolejka[i].x + 5;
					ile++;
				}
			}
		}
	}
	if (!ile)
	{
		dzialanie_windy = 4;
		koniec = false;
		ilosc = 0;
	}
}

void otwieranie_zamykanie(HDC hdc, RECT* rect, int a)
{
	koniec = false;
	if (a == 1)
	{
		Graphics graphics(hdc);
		Pen pen(Color(255, 0, 0, 0), 2);
		int y = 640 - winda_s.pietro * 150;
		graphics.DrawLine(&pen, 600, y, 600, y + 110 - zmienna);
		tworzenie_czlowieka(hdc);
	}
	else
	{
		Graphics graphics(hdc);
		Pen pen(Color(255, 0, 0, 0), 2);
		int y = 640 - winda_s.pietro * 150;
		graphics.DrawLine(&pen, 600, y, 600, y + 110 - zmienna);
		tworzenie_czlowieka(hdc);
	}
	if (zmienna == 110)
		koniec = true;
}

void wsiadanie(HDC hdc)
{
	switch (winda_s.pietro)
	{
	case 0:
		otwieranie_zamykanie(hdc, &pietro_0_winda, -1);
		break;
	case 1:
		otwieranie_zamykanie(hdc, &pietro_1_winda, -1);
		break;
	case 2:
		otwieranie_zamykanie(hdc, &pietro_2_winda, -1);
		break;
	case 3:
		otwieranie_zamykanie(hdc, &pietro_3_winda, -1);
		break;
	case 4:
		otwieranie_zamykanie(hdc, &pietro_4_winda, -1);
		break;
	}
}

void zamykanie(HDC hdc)
{
	switch (winda_s.pietro)
	{
	case 0:
		otwieranie_zamykanie(hdc, &pietro_0_winda, 1);
		break;
	case 1:
		otwieranie_zamykanie(hdc, &pietro_1_winda, 1);
		break;
	case 2:
		otwieranie_zamykanie(hdc, &pietro_2_winda, 1);
		break;
	case 3:
		otwieranie_zamykanie(hdc, &pietro_3_winda, 1);
		break;
	case 4:
		otwieranie_zamykanie(hdc, &pietro_4_winda, 1);
		break;
	}
}

void tworzenie_czlowieka(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 255, 0, 0), 3);

	for (int i = 0; i < kolejka.size(); i++)
	{
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.FillEllipse(&solidBrush, kolejka[i].x, kolejka[i].y - 1, 16, 16);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 17, 18, 26);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 43, 8, 18);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 43, 8, 18);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 57, 8, 8);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 57, 8, 8);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 17, 4, 23);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 17, 4, 23);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 38, 4, 4);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 38, 4, 4);
	}

	for (int i = 0; i < w_windzie.size(); i++)
	{
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.FillEllipse(&solidBrush, kolejka[i].x, kolejka[i].y - 1, 16, 16);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 17, 18, 26);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 43, 8, 18);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 43, 8, 18);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 57, 8, 8);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 57, 8, 8);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 17, 4, 23);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 17, 4, 23);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 38, 4, 4);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 38, 4, 4);
	}

	for (int i = 0; i < oczekiwanie_winda.size(); i++)
	{
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.FillEllipse(&solidBrush, kolejka[i].x, kolejka[i].y - 1, 16, 16);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 17, 18, 26);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 43, 8, 18);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 43, 8, 18);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 1, kolejka[i].y + 57, 8, 8);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 9, kolejka[i].y + 57, 8, 8);
		graphics.FillRectangle(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 17, 4, 23);
		graphics.FillRectangle(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 17, 4, 23);
		graphics.FillEllipse(&solidBrush, kolejka[i].x - 6, kolejka[i].y + 38, 4, 4);
		graphics.FillEllipse(&solidBrush, kolejka[i].x + 18, kolejka[i].y + 38, 4, 4);
	}
}

int kierunek_windy(int p, int l)
{
	if (p > l)
		return -1;
	if (p < l)
		return 1;
	if (p == l)
		return 0;
}

void ruch_windy(HDC hdc, RECT* rect)
{
	if (!ruchwindy.empty())
	{
		if (winda_s.pietro == ruchwindy[0].pietro)
		{
			if (ruchwindy[0].zatrzymanie == 1)
			{
				winda_s.kierunek = 0;
				if (!w_windzie.empty() || !oczekiwanie_winda.empty())
					dzialanie_windy = 2;
				else
					dzialanie_windy = 0;
			}
			ruchwindy.erase(ruchwindy.begin());
		}
		else
		{
			if (winda_s.pietro < ruchwindy[0].pietro)
			{
				wartosc++;
				for (int i = 0; i < w_windzie.size(); i++)
				{
					w_windzie[i].y--;
				}
				winda_s.kierunek = 1;
			}
			if (winda_s.pietro > ruchwindy[0].pietro)
			{
				wartosc--;
				for (int i = 0; i < w_windzie.size(); i++)
				{
					w_windzie[i].y++;
				}
				winda_s.kierunek = -1;
			}
			if (wartosc % 150 == 0)
			{
				winda_s.pietro = winda_s.pietro + ruchwindy[0].kierunek;
			}
		}
	}
	zarys_windy(hdc, &winda_m);
	tworzenie_czlowieka(hdc);
}


void ustaw_poczatek(bool& ustawienie_poczatku, int i, int kier, int k)
{
	for (int j = k - 1; j > -1; j--)
	{
		if (kier == ruchwindy[j].kierunek == 1 && kolejka[i].pietro_poczatkowe <= ruchwindy[j].pietro)
		{
			if (kolejka[i].pietro_poczatkowe == ruchwindy[j].pietro)
			{
				if (j != 0 && ruchwindy[j - 1].pietro != ruchwindy[j].pietro)
				{
					ruchwindy[j].zatrzymanie = 1;
				}
				ruchwindy[k].zatrzymanie = 1;
				oczekiwanie_winda.push_back(kolejka[i]);
				kolejka.erase(kolejka.begin() + i);
				ustawienie_poczatku = true;
				break;
			}
		}
		if (kier == ruchwindy[j].kierunek == -1 && kolejka[i].pietro_poczatkowe >= ruchwindy[j].pietro)
		{
			if (kolejka[i].pietro_poczatkowe == ruchwindy[j].pietro)
			{
				if (j != 0 && ruchwindy[j - 1].pietro != ruchwindy[j].pietro)
				{
					ruchwindy[j].zatrzymanie = 1;
				}
				ruchwindy[k].zatrzymanie = 1;
				oczekiwanie_winda.push_back(kolejka[i]);
				kolejka.erase(kolejka.begin() + i);
				ustawienie_poczatku = true;
				break;
			}
		}
	}
}

void ustaw_koniec(bool& ustawienie_konca, int i, int kier, int &k)
{
	for (int j = ruchwindy.size() - 1; j > 0; j--)
	{
		if (kier == ruchwindy[j].kierunek == 1 && kolejka[i].pietro_koncowe <= ruchwindy[j].pietro)
		{
			if (kolejka[i].pietro_koncowe == ruchwindy[j].pietro)
			{
				ustawienie_konca = true;
				k = j;
				break;
			}
		}
		if (kier == ruchwindy[j].kierunek == -1 && kolejka[i].pietro_koncowe >= ruchwindy[j].pietro)
		{
			if (kolejka[i].pietro_koncowe == ruchwindy[j].pietro)
			{
				ustawienie_konca = true;
				k = j;
				break;
			}
		}
	}
	int j = ruchwindy.size() - 1;
	if (!ustawienie_konca && i == 0 && kier == ruchwindy[j].kierunek)
	{
		if (kier == 1)
		{
			for (int a = ruchwindy[j].pietro + 1; a <= kolejka[0].pietro_koncowe; a++)
			{
				if (a != kolejka[0].pietro_koncowe)
					ruchwindy.push_back({ a, 1, 0 });
				else
					ruchwindy.push_back({ a, 1, 1 });
			}
			ustawienie_konca = true;
			k = j;
		}
		else
		{
			for (int a = ruchwindy[j].pietro - 1; a >= kolejka[0].pietro_koncowe; a--)
			{
				if (a != kolejka[0].pietro_koncowe)
					ruchwindy.push_back({ a, -1, 0 });
				else
					ruchwindy.push_back({ a, -1, 1 });
			}
			ustawienie_konca = true;
			k = j;
		}
	}
}


void ustaw_winda()
{
	if (ruchwindy.empty() && !kolejka.empty())
	{
		if (kolejka[0].pietro_poczatkowe != winda_s.pietro)
		{
			if (kierunek_windy(winda_s.pietro, kolejka[0].pietro_poczatkowe) == 1)
			{
				for (int i = winda_s.pietro; i <= kolejka[0].pietro_poczatkowe; i++)
				{
					if (i != kolejka[0].pietro_poczatkowe)
						ruchwindy.push_back({ i, 1, 0 });
					else
						ruchwindy.push_back({ i, -1, 1 });
				}
			}
			if (kierunek_windy(winda_s.pietro, kolejka[0].pietro_poczatkowe) == -1)
			{
				for (int i = winda_s.pietro; i >= kolejka[0].pietro_poczatkowe; i--)
				{
					if (i != kolejka[0].pietro_poczatkowe)
						ruchwindy.push_back({ i, -1, 0 });
					else
						ruchwindy.push_back({ i, -1, 1 });
				}
			}
			if (dzialanie_windy == 0)
			{
				dzialanie_windy = 1;
				zmienna = 0;
			}
		}

		if (kierunek_windy(kolejka[0].pietro_poczatkowe, kolejka[0].pietro_koncowe) == 1)
		{
			for (int i = kolejka[0].pietro_poczatkowe; i <= kolejka[0].pietro_koncowe; i++)
			{
				if (i != kolejka[0].pietro_koncowe)
					ruchwindy.push_back({ i, 1, 0 });
				else
				{
					ruchwindy.push_back({ i, 1, 1 });
					oczekiwanie_winda.push_back(kolejka[0]);
				}
			}
		}
		else
		{
			for (int i = kolejka[0].pietro_poczatkowe; i >= kolejka[0].pietro_koncowe; i--)
			{
				if (i != kolejka[0].pietro_koncowe)
					ruchwindy.push_back({ i, -1, 0 });
				else
				{
					ruchwindy.push_back({ i, -1, 1 });
					oczekiwanie_winda.push_back(kolejka[0]);
				}
			}
		}
		if (dzialanie_windy == 0)
		{
			dzialanie_windy = 2;
			zmienna = 0;
		}
		kolejka.erase(kolejka.begin());
	}
	else
	{
		if (!ruchwindy.empty() && !kolejka.empty())
		{
			int i = 0;
			while (!kolejka.empty() && kolejka.size() > i)
			{
				int kier = kierunek_windy(kolejka[i].pietro_poczatkowe, kolejka[i].pietro_koncowe);
				bool ustawienie_poczatku = false;
				bool ustawienie_konca = false;
				if (kier == ruchwindy[0].kierunek && w_windzie.size() + oczekiwanie_winda.size() < 8)
				{
					int k;
					ustaw_koniec(ustawienie_konca, i, kier, k);
					if (ustawienie_konca)
						ustaw_poczatek(ustawienie_poczatku, i, kier, k);
					if (!ustawienie_konca || !ustawienie_poczatku)
						i++;
				}
				else
					i++;
			}
		}
	}
}

void sterowanie(HDC hdc)
{
	switch (dzialanie_windy)
	{
	case 0: 
		bez_ruchu();
		break;
	case 1:
		ruchwindy(hdc, &winda_m);
		break;
	case 2:
		wsiadanie(hdc);
		break;
	case 3:
		ruch_ludzi(hdc);
		break;
	case 4:
		zamykanie(hdc);
		break;
	}
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
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON02:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON03:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON04:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON10:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON12:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON13:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON14:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON20:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON21:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON23:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON24:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON30:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON31:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON32:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON34:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON40:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON41:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON42:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			break;
		case ID_BUTTON43:
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
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
