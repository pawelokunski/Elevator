// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>

#define MAX_LOADSTRING 100
#define TMR_w 128


using namespace std;

struct czlowiek
{
	int pietro_poczatkowe;
	int pietro_koncowe;
	int x;
	int y;
};

vector<czlowiek> kolejka;
vector<czlowiek> czlowiek_winda;
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
int miejsce[8] = { 610, 635, 660, 685, 710, 735, 755, 780 };

int dzialanie_windy = 0;        // 0 - stoi i sie nie rusza
								// 1 - ruch
								// 2 - otwieranie drzwi
								// 3 - stoi z otwartymi drzwiami
								// 4 - zamykanie drzwi

// buttons
HWND hwndButton;

RECT winda_m = { 603, 13, 847, 759 };
RECT pietro_ruch_masa = { 880, 10, 1000, 150 };

RECT pietro_0_winda = { 100, 680, 840, 750 };
RECT pietro_1_winda = { 100, 530, 840, 600 };
RECT pietro_2_winda = { 100, 380, 840, 450 };
RECT pietro_3_winda = { 100, 230, 840, 300 };
RECT pietro_4_winda = { 100, 80, 840, 150 };

RECT pietro_0 = { 100, 680, 599, 750 };
RECT pietro_1 = { 100, 530, 599, 600 };
RECT pietro_2 = { 100, 380, 599, 450 };
RECT pietro_3 = { 100, 230, 599, 300 };
RECT pietro_4 = { 100, 80, 599, 150 };


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

void przesuwanie();
void ustaw_winda();
void tworzenie_czlowieka(HDC hdc);


void zarys_windy(HDC hdc, RECT *rect)
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
	int p = 0;
	if (!wolne)
	{
		for (int i = 0; i < czlowiek_winda.size(); i++)
		{
			if (czlowiek_winda[i].pietro_koncowe == winda_s.pietro)
			{
				for (int j = 0; j < 8; j++)
				{
					if (czlowiek_winda[i].x == miejsce[j])
						zajete[j] = 0;
				}
			}
		}
		wolne = true;
	}
	if (!czlowiek_winda.empty())
	{
		p = 0;
		for (int i = 0; i < czlowiek_winda.size(); i++)
		{
			if (czlowiek_winda[i].pietro_koncowe == winda_s.pietro && czlowiek_winda[i].x > 50)
			{
				czlowiek_winda[i].x -= 5;
				p++;
			}
		}
	}
	int k = 0;
	if (p == 0)
	{
		koniec = true;
		while (!czlowiek_winda.empty() && k < czlowiek_winda.size())
		{
			if (czlowiek_winda[k].pietro_koncowe == winda_s.pietro)
				czlowiek_winda.erase(czlowiek_winda.begin() + k);
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
							czlowiek_winda.push_back(oczekiwanie_winda[k]);
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
	for (int i = 0; i < czlowiek_winda.size(); i++)
	{
		if (czlowiek_winda[i].pietro_poczatkowe == winda_s.pietro)
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
		for (int i = winda_s.pietro; i >= 0; i--)
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
	int p = 0;
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
		graphics.DrawLine(&pen, 600, y, 600, y + (zmienna));
		tworzenie_czlowieka(hdc);
	}
	else
	{
		Graphics graphics(hdc);
		Pen pen(Color(255, 0, 0, 0), 2);
		int y = 640 - winda_s.pietro * 150;
		graphics.DrawLine(&pen, 600, y, 600, y + 110 - (zmienna));
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

	for (int i = 0; i < czlowiek_winda.size(); i++)
	{
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.FillEllipse(&solidBrush, czlowiek_winda[i].x, czlowiek_winda[i].y - 1, 16, 16);
		graphics.FillRectangle(&solidBrush, czlowiek_winda[i].x - 1, czlowiek_winda[i].y + 17, 18, 26);
		graphics.FillRectangle(&solidBrush, czlowiek_winda[i].x - 1, czlowiek_winda[i].y + 43, 8, 18);
		graphics.FillRectangle(&solidBrush, czlowiek_winda[i].x + 9, czlowiek_winda[i].y + 43, 8, 18);
		graphics.FillEllipse(&solidBrush, czlowiek_winda[i].x - 1, czlowiek_winda[i].y + 57, 8, 8);
		graphics.FillEllipse(&solidBrush, czlowiek_winda[i].x + 9, czlowiek_winda[i].y + 57, 8, 8);
		graphics.FillRectangle(&solidBrush, czlowiek_winda[i].x - 6, czlowiek_winda[i].y + 17, 4, 23);
		graphics.FillRectangle(&solidBrush, czlowiek_winda[i].x + 18, czlowiek_winda[i].y + 17, 4, 23);
		graphics.FillEllipse(&solidBrush, czlowiek_winda[i].x - 6, czlowiek_winda[i].y + 38, 4, 4);
		graphics.FillEllipse(&solidBrush, czlowiek_winda[i].x + 18, czlowiek_winda[i].y + 38, 4, 4);
	}

	for (int i = 0; i < oczekiwanie_winda.size(); i++)
	{
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.FillEllipse(&solidBrush, oczekiwanie_winda[i].x, oczekiwanie_winda[i].y - 1, 16, 16);
		graphics.FillRectangle(&solidBrush, oczekiwanie_winda[i].x - 1, oczekiwanie_winda[i].y + 17, 18, 26);
		graphics.FillRectangle(&solidBrush, oczekiwanie_winda[i].x - 1, oczekiwanie_winda[i].y + 43, 8, 18);
		graphics.FillRectangle(&solidBrush, oczekiwanie_winda[i].x + 9, oczekiwanie_winda[i].y + 43, 8, 18);
		graphics.FillEllipse(&solidBrush, oczekiwanie_winda[i].x - 1, oczekiwanie_winda[i].y + 57, 8, 8);
		graphics.FillEllipse(&solidBrush, oczekiwanie_winda[i].x + 9, oczekiwanie_winda[i].y + 57, 8, 8);
		graphics.FillRectangle(&solidBrush, oczekiwanie_winda[i].x - 6, oczekiwanie_winda[i].y + 17, 4, 23);
		graphics.FillRectangle(&solidBrush, oczekiwanie_winda[i].x + 18, oczekiwanie_winda[i].y + 17, 4, 23);
		graphics.FillEllipse(&solidBrush, oczekiwanie_winda[i].x - 6, oczekiwanie_winda[i].y + 38, 4, 4);
		graphics.FillEllipse(&solidBrush, oczekiwanie_winda[i].x + 18, oczekiwanie_winda[i].y + 38, 4, 4);
	}
}

int kierunek_windy(int a, int b)
{
	if (a > b)
		return -1;

	if (a < b)
		return 1;

	if (a == b)
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
				if (!czlowiek_winda.empty() || !oczekiwanie_winda.empty())
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
				for (int i = 0; i < czlowiek_winda.size(); i++)
				{
					czlowiek_winda[i].y--;
				}
				winda_s.kierunek = 1;
			}
			if (winda_s.pietro > ruchwindy[0].pietro)
			{
				wartosc--;
				for (int i = 0; i < czlowiek_winda.size(); i++)
				{
					czlowiek_winda[i].y++;
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
				if (kier == ruchwindy[0].kierunek && czlowiek_winda.size() + oczekiwanie_winda.size() < 8)
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
		ruch_windy(hdc, &winda_m);
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

void stworz_czlowieka()
{
	for (int i = 0; i < kolejka.size(); i++)
	{
		if (kolejka[i].x == 0)
		{
			int p;
			switch (kolejka[i].pietro_poczatkowe)
			{
			case 0:
				p = wartosc0;
				wartosc0 = wartosc0 + 25;
				break;
			case 1:
				p = wartosc1;
				wartosc1 = wartosc1 + 25;
				break;
			case 2:
				p = wartosc2;
				wartosc2 = wartosc2 + 25;
				break;
			case 3:
				p = wartosc3;
				wartosc3 = wartosc3 + 25;
				break;
			case 4:
				p = wartosc4;
				wartosc4= wartosc4 + 25;
				break;
			}
			kolejka[i].x = 575 - p;
			kolejka[i].y = 680 - 150 * kolejka[i].pietro_poczatkowe;
		}
	}
	for (int i = 0; i < oczekiwanie_winda.size(); i++)
	{
		if (oczekiwanie_winda[i].x == 0)
		{
			int p;
			switch (oczekiwanie_winda[i].pietro_poczatkowe)
			{
			case 0:
				p = wartosc0;
				wartosc0 = wartosc0 + 25;
				break;
			case 1:
				p = wartosc1;
				wartosc1 = wartosc1 + 25;
				break;
			case 2:
				p = wartosc2;
				wartosc2 = wartosc2 + 25;
				break;
			case 3:
				p = wartosc3;
				wartosc3 = wartosc3 + 25;
				break;
			case 4:
				p = wartosc4;
				wartosc4 = wartosc4 + 25;
				break;
			}
			oczekiwanie_winda[i].x = 575 - p;
			oczekiwanie_winda[i].y = 680 - 150 * oczekiwanie_winda[i].pietro_poczatkowe;
		}
	}
}

void tablice(HDC hdc, RECT* rect)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 0), 2);
	graphics.DrawRectangle(&pen, 900, 10, 50, 30);

	FontFamily fontFamily(L"Arial");
	Font font1(&fontFamily, 30, FontStyleRegular, UnitPixel);
	PointF pointF1(930, 15);
	SolidBrush solidBrush(Color(255, 0, 0, 0));

	wchar_t pietro[2];
	pietro[1] = '\0';
	switch (winda_s.pietro)
	{
	case 0:
		pietro[0] = '0';
		break;
	case 1:
		pietro[0] = '1';
		break;
	case 2:
		pietro[0] = '2';
		break;
	case 3:
		pietro[0] = '3';
		break;
	case 4:
		pietro[0] = '4';
		break;
	}
	graphics.DrawString(pietro, -1, &font1, pointF1, &solidBrush);

	switch (winda_s.kierunek)
	{
	case -1:
		graphics.DrawLine(&pen, 910, 20, 915, 30);
		graphics.DrawLine(&pen, 915, 30, 920, 20);
		break;
	case 0:
		graphics.DrawLine(&pen, 910, 25, 920, 25);
		break;
	case 1:
		graphics.DrawLine(&pen, 910, 30, 915, 20);
		graphics.DrawLine(&pen, 915, 20, 920, 30);
	}

	graphics.DrawRectangle(&pen, 900, 50, 70, 50);
	PointF pointF2(910, 52);
	graphics.DrawString(L"Masa", -1, &font1, pointF2, &solidBrush);
	PointF pointF3(910, 75);
	wchar_t masa[4];
	masa[3] = '\0';
	switch (czlowiek_winda.size())
	{
	case 0:
		masa[0] = '0';
		masa[1] = '0';
		masa[2] = '0';
		break;
	case 1:
		masa[0] = '0';
		masa[1] = '7';
		masa[2] = '0';
		break;
	case 2:
		masa[0] = '1';
		masa[1] = '4';
		masa[2] = '0';
		break;
	case 3:
		masa[0] = '2';
		masa[1] = '1';
		masa[2] = '0';
		break;
	case 4:
		masa[0] = '2';
		masa[1] = '8';
		masa[2] = '0';
		break;
	case 5:
		masa[0] = '3';
		masa[1] = '5';
		masa[2] = '0';
		break;
	case 6:
		masa[0] = '4';
		masa[1] = '2';
		masa[2] = '0';
		break;
	case 7:
		masa[0] = '4';
		masa[1] = '9';
		masa[2] = '0';
		break;
	case 8:
		masa[0] = '5';
		masa[1] = '6';
		masa[2] = '0';
		break;
	}
	graphics.DrawString(masa, -1, &font1, pointF3, &solidBrush);
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


int OnCreate(HWND window)
{
	SetTimer(window, TMR_w, 10, 0);
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
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON01:
			kolejka.push_back({ 0, 1 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_0, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON02:
			kolejka.push_back({ 0, 2 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_0, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON03:
			kolejka.push_back({ 0, 3 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_0, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON04:
			kolejka.push_back({ 0, 4 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_0, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON10:
			kolejka.push_back({ 1, 0 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_1, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON12:
			kolejka.push_back({ 1, 2 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_1, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON13:
			kolejka.push_back({ 1, 3 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_1, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON14:
			kolejka.push_back({ 1, 4 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_1, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON20:
			kolejka.push_back({ 2, 0 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_2, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON21:
			kolejka.push_back({ 2, 1 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_2, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON23:
			kolejka.push_back({ 2, 3 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_2, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON24:
			kolejka.push_back({ 2, 4 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_2, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON30:
			kolejka.push_back({ 3, 0 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_3, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON31:
			kolejka.push_back({ 3, 1 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_3, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON32:
			kolejka.push_back({ 3, 2 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_3, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON34:
			kolejka.push_back({ 3, 4 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_3, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON40:
			kolejka.push_back({ 4, 0 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_4, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON41:
			kolejka.push_back({ 4, 1 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_4, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON42:
			kolejka.push_back({ 4, 2 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_4, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		case ID_BUTTON43:
			kolejka.push_back({ 4, 3 });
			stworz_czlowieka();
			InvalidateRect(hWnd, &pietro_4, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			tworzenie_czlowieka(hdc);
			ustaw_winda();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyOnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
			case TMR_w:
			{
				switch (dzialanie_windy)
				{
				case 0:
					hdc = BeginPaint(hWnd, &ps);
					bez_ruchu();
					if (koniec)
					{
						dzialanie_windy = 1;
						zmienna = 0;
						koniec = false;
					}
					break;
				case 1:
					zmienna = 0;
					InvalidateRect(hWnd, &winda_m, TRUE);
					hdc = BeginPaint(hWnd, &ps);
					ruch_windy(hdc, &winda_m);
					break;
				case 2:
					if (zmienna % 9 == 0)
						ustaw_winda();
					switch (winda_s.pietro)
					{
					case 0:
						InvalidateRect(hWnd, &pietro_0_winda, TRUE);
						break;
					case 1:
						InvalidateRect(hWnd, &pietro_1_winda, TRUE);
						break;
					case 2:
						InvalidateRect(hWnd, &pietro_2_winda, TRUE);
						break;
					case 3:
						InvalidateRect(hWnd, &pietro_3_winda, TRUE);
						break;
					case 4:
						InvalidateRect(hWnd, &pietro_4_winda, TRUE);
						break;
					}
					hdc = BeginPaint(hWnd, &ps);
					tworzenie_czlowieka(hdc);
					wsiadanie(hdc);
					zmienna++;
					if (koniec)
					{
						zmienna = 0;
						koniec = false;
						dzialanie_windy = 3;
					}
					break;
				case 3:
					switch (winda_s.pietro)
					{
					case 0:
						InvalidateRect(hWnd, &pietro_0_winda, TRUE);
						break;
					case 1:
						InvalidateRect(hWnd, &pietro_1_winda, TRUE);
						break;
					case 2:
						InvalidateRect(hWnd, &pietro_2_winda, TRUE);
						break;
					case 3:
						InvalidateRect(hWnd, &pietro_3_winda, TRUE);
						break;
					case 4:
						InvalidateRect(hWnd, &pietro_4_winda, TRUE);
						break;
					}
					hdc = BeginPaint(hWnd, &ps);
					ruch_ludzi(hdc);
					if (koniec)
						ruch_ludzi_2(hdc);
					if (ilosc && koniec)
						przesuwanie2();
					tworzenie_czlowieka(hdc);
					break;
				case 4:
					if (zmienna % 9 == 0)
						ustaw_winda();
					switch (winda_s.pietro)
					{
					case 0:
						InvalidateRect(hWnd, &pietro_0_winda, TRUE);
						break;
					case 1:
						InvalidateRect(hWnd, &pietro_1_winda, TRUE);
						break;
					case 2:
						InvalidateRect(hWnd, &pietro_2_winda, TRUE);
						break;
					case 3:
						InvalidateRect(hWnd, &pietro_3_winda, TRUE);
						break;
					case 4:
						InvalidateRect(hWnd, &pietro_4_winda, TRUE);
						break;
					}
					hdc = BeginPaint(hWnd, &ps);
					zamykanie(hdc);
					zmienna++;
					if (koniec)
					{
						zmienna = 0;
						koniec = false;
						if (ruchwindy.empty())
							dzialanie_windy = 0;
						else
						{
							bool jeszcze = false;
							for (int i = 0; i < oczekiwanie_winda.size(); i++)
							{
								if (winda_s.pietro == oczekiwanie_winda[i].pietro_poczatkowe)
								{
									jeszcze = true;
									break;
								}
							}
							if (jeszcze)
								dzialanie_windy = 2;
							else
								dzialanie_windy = 1;
						}
					}
					break;
				}

				InvalidateRect(hWnd, &pietro_ruch_masa, TRUE);
				hdc = BeginPaint(hWnd, &ps);
				tablice(hdc, &pietro_ruch_masa);

				ReleaseDC(hWnd, hdc);
				EndPaint(hWnd, &ps);
			}
			break;
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
