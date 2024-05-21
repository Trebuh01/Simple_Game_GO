
#include<stdio.h>
#include <stdlib.h>
#include"conio2.h"
#define menu 1//miejsce wywolania menu
#define plansza_x 60//miejsce wywolania planszy
#define plansza_y 5//miejsce wywolania planszy

void zbicie_wielokrotne(char ostatni_ruch, char ruchyT[][19], int r, int& pkt_bialych, int& pkt_czarnych);
void flood(int tabX, int tabY, char ostatni_ruch, char board[][19], const int boardsize, char visited[][19], int& pomocnicza);
void board(char tab[][19], int r);
void wypisz(char tab[19][19], int r);
void zapisz(char ruchyT[][19], int& r, int& pkt_bialych, int& pkt_czarnych, char& ostatni_ruch);
void granica(int& x, int& y, int r);
void ruchy(char tab[][19], int r, char ruchyT[][19]);
int czy_zbicie(int r, char ruchyT[][19], int pozycja_x, int pozycja_y);
void zbicie(int r, char ruchyT[][19], char ostatni_ruch, int& pkt_bialych, int& pkt_czarnych);
void czy_powinien_stac(int r, char ruchyT[][19], char& ostatni_ruch);
void czyszczenie_ruchow(char ruchyT[][19]);
void wczytaj(char ruchyT[][19], int& r, int& pkt_bialych, int& pkt_czarnych, char& ostatni_ruch);

int main()
{
	int zn = 0, x = plansza_x, y = plansza_y, attr = 7, back = 0, zero = 0, podswietlenie_plansza = 10, podswietlenie_rozmiar = 7, pkt_bialych = 0, pkt_czarnych = 0, obieg = 0;
	char txt[32], plansza[19][19], ruchyT[19][19] = {}, ostatni_ruch = 'X';
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			ruchyT[i][j] = '.';
		}
	}
	int KO = 0;//po uruchomieniu nie wykonaja sie funkcje zbicie i zbicie wielokrotne przez co zasada KO zostaje zachowana
#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("Hubert, Szymczak");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);

	// we set black to be the background color
	textbackground(BLACK);

	textcolor(attr);

	gotoxy(menu, 1);
	cputs("q       = exit");
	gotoxy(menu, 2);
	cputs("cursors = moving");
	gotoxy(menu, 3);
	cputs("n       =restart");
	gotoxy(menu, 4);
	cputs("i       =put");
	textcolor(4);
	gotoxy(menu, 5);
	cputs("1       =9x9 ");
	gotoxy(menu, 6);
	cputs("2       =13x13 ");
	gotoxy(menu, 7);
	cputs("3       =19x19 ");
	gotoxy(menu, 8);
	cputs("l       =wczytaj gre ");
	int r = 0;
	zn = getch();
	if (zn == '2')r = 13;
	if (zn == '1')r = 9;
	if (zn == '3')r = 19;
	if (zn == 'l')wczytaj(ruchyT, r, pkt_bialych, pkt_czarnych, ostatni_ruch);
	do {
		clrscr();
		textcolor(attr);
		gotoxy(menu, 1);
		cputs("q       = exit");
		gotoxy(menu, 2);
		cputs("cursors = moving");
		gotoxy(menu, 3);
		cputs("n       =restart");
		gotoxy(menu, 4);
		cputs("i       =put");
		gotoxy(menu, 5);
		cputs("s       =zapisz gre");
		textcolor(podswietlenie_plansza);

		board(plansza, r);
		if (KO == 1)
		{

			zbicie(r, ruchyT, ostatni_ruch, pkt_bialych, pkt_czarnych);//sprawdza czy nastepuje zbicie i przygotowywuje odpowiednio plansze do tego uwzgledniajac KO
			zbicie_wielokrotne(ostatni_ruch, ruchyT, r, pkt_bialych, pkt_czarnych);
		}

		czy_powinien_stac(r, ruchyT, ostatni_ruch);//sprawdza czy dany element powinien sie znalezc w tablicy ruchy

		ruchy(plansza, r, ruchyT);//przepisuje zmiany do gotowej tablicy do wypisania

		wypisz(plansza, r);//wypisuje plansze z gotowymi zmianami
		granica(x, y, r);//sprawia ze nie wyjdziemy kursorem poza plansze
		gotoxy(x, y);
		putch('*');

		gotoxy(menu, 6);
		if (ostatni_ruch == 'X')
		{
			cputs("teraz kolej: Czarnego (O)");
		}
		else cputs("teraz kolej: Bialego (X)");
		gotoxy(menu, 7);
		sprintf_s(txt, "PKT Bialych: %d", pkt_bialych);
		cputs(txt);
		gotoxy(menu, 8);
		sprintf_s(txt, "PKT Czarnych: %d", pkt_czarnych);
		cputs(txt);
		if (obieg == 0)
		{
			gotoxy(menu, 9);
			cputs("e       =edytuj");
			++obieg;
		}
		else
		{
			int pozycja_y = -(plansza_y - y), pozycja_x = -(plansza_x - x) / 2;
			gotoxy(menu, 9);
			sprintf_s(txt, "tabX: %d", pozycja_x + 1);
			cputs(txt);
			gotoxy(menu, 10);
			sprintf_s(txt, "tabY: %d", pozycja_y + 1);
			cputs(txt);
		}
		textbackground(back);

		// we wait for key press and get its code
		// most key codes correspond to the characters, like
		// a is 'a', 2 is '2', + is '+', but some special keys
		// like cursors provide two characters, where the first
		// one is zero, e.g., "up arrow" is zero and 'H'
		zero = 0;
		zn = getch();
		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if (zn == 0) {
			KO = 0;//false zeby nie zniknela zasada KO podczas przemieszczania sie po planszy
			zero = 1;		// if this is the case then we read
			zn = getch();		// the next code knowing that this
			if (zn == 0x48) y--;	// will be a special key
			else if (zn == 0x50) y++;
			else if (zn == 0x4b) x = x - 2;//o 2 dlatego zeby bylo czytelniej
			else if (zn == 0x4d) x = x + 2;
		}
		else if (zn == 'i')
		{
			int pozycja_y = -(plansza_y - y), pozycja_x = -(plansza_x - x) / 2;//okreslenie pozycji w tablicy a nie w konsoli
			if (ostatni_ruch == 'X' && ruchyT[pozycja_y][pozycja_x] != 'X' && ruchyT[pozycja_y][pozycja_x] != 'O' && ruchyT[pozycja_y][pozycja_x] != '!')
			{
				ruchyT[pozycja_y][pozycja_x] = 'O';
				ostatni_ruch = 'O';
				KO = 1;//jezeli mozliwe jest postawienie kamienia to zasada KO znika
			}
			else if (ostatni_ruch == 'O' && ruchyT[pozycja_y][pozycja_x] != 'X' && ruchyT[pozycja_y][pozycja_x] != 'O' && ruchyT[pozycja_y][pozycja_x] != '!')
			{
				ruchyT[pozycja_y][pozycja_x] = 'X';
				ostatni_ruch = 'X';
				KO = 1;
			}
			else KO = 0;//false powoduje ze nie wykona sie if odpowiedzialny za wywolanie funkcji zbicie czyli nasz ! nie zostanie usuniety z planszy
		}

		else if (zn == 'n')
		{
			clrscr();
			podswietlenie_rozmiar = 4;
			textcolor(podswietlenie_rozmiar);
			gotoxy(menu, 1);
			cputs("1       =9x9 ");
			gotoxy(menu, 2);
			cputs("2       =13x13 ");
			gotoxy(menu, 3);
			cputs("3       =19x19 ");
			gotoxy(menu, 4);
			cputs("l       =wczytaj gre ");
			textcolor(attr);
			do
			{
				zn = getch();
			} while (zn != '1' && zn != '2' && zn != '3' && zn != 'l');
			ostatni_ruch = 'X';
			pkt_bialych = 0;
			pkt_czarnych = 0;
			r = 0;
			if (zn == '2')r = 13;
			if (zn == '1')r = 9;
			if (zn == '3')r = 19;
			x = plansza_x;
			y = plansza_y;
			gotoxy(x, y);
			putch('*');
			czyszczenie_ruchow(ruchyT);
			if (zn == 'l')wczytaj(ruchyT, r, pkt_bialych, pkt_czarnych, ostatni_ruch);
			obieg = 0;//po restarcie mam mozliwosc edycji

		}
		else if (obieg == 1 && zn == 'e')
		{
			clrscr();
			gotoxy(menu, 1);
			cputs("x    =opusc edytor");
			gotoxy(menu, 2);
			cputs("i    =put black");
			do
			{
				board(plansza, r);
				ruchy(plansza, r, ruchyT);
				wypisz(plansza, r);
				granica(x, y, r);
				gotoxy(x, y);
				putch('*');
				zn = getch();
				if (zn == 0) {
					zero = 1;
					zn = getch();
					if (zn == 0x48) y--;
					else if (zn == 0x50) y++;
					else if (zn == 0x4b) x = x - 2;
					else if (zn == 0x4d) x = x + 2;
				}
				else if (zn == 'i')
				{
					int pozycja_y = -(plansza_y - y), pozycja_x = -(plansza_x - x) / 2;
					if (ruchyT[pozycja_y][pozycja_x] != 'O')
					{
						ruchyT[pozycja_y][pozycja_x] = 'O';

					}
				}

			} while (zn != 'x');

			ostatni_ruch = 'X';

		}
		else if (zn == 's')
		{
			zapisz(ruchyT, r, pkt_bialych, pkt_czarnych, ostatni_ruch);
		}
		else if (zn == 0x0d) back = (back + 1) % 16;	// enter key is 0x0d or '\r'	

	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
	// visible after the program ends
	return 0;

}
void zapisz(char ruchyT[][19], int& r, int& pkt_bialych, int& pkt_czarnych, char& ostatni_ruch)
{
	FILE* plik = NULL;
	char nazwa_pliku[200];
	int x = menu, y = 20, z = 0;
	char zn;
	gotoxy(x, y);
	cputs("Podaj nazwe pliku konczac ja .txt: ");
	y = 21;
	do
	{
		zn = getch();
		if (zn == 0x08)
		{

			if (x > menu)x--;
			if (z > 0)z--;
			gotoxy(x, y);
			putch(' ');
			nazwa_pliku[z] = '\0';
		}
		else
		{
			gotoxy(x, y);
			putch(zn);
			++x;
			nazwa_pliku[z] = zn;
			++z;
			nazwa_pliku[z] = '\0';

		}

	} while (zn != ' ');
	nazwa_pliku[z] = '\0';
	do
	{
		fopen_s(&plik, nazwa_pliku, "w");
	} while (plik == 0);

	fprintf(plik, "%c", ostatni_ruch);
	fprintf(plik, "%d", pkt_bialych);
	fprintf(plik, "%d", pkt_czarnych);
	if (r == 9)
	{
		fprintf(plik, "%d", r);
	}
	else if (r == 13)
	{
		fprintf(plik, "%d", 1);
	}
	else if (r == 19)
	{
		fprintf(plik, "%d", 2);
	}
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			fprintf(plik, "%c", ruchyT[i][j]);
		}
	}

	fclose(plik);
}
void wczytaj(char ruchyT[][19], int& r, int& pkt_bialych, int& pkt_czarnych, char& ostatni_ruch)
{
	FILE* plik = NULL;
	char znak = ' ';
	char nazwa_pliku[200];
	int x = menu, y = 20, z = 0;
	char zn;
	gotoxy(x, y);
	cputs("Podaj nazwe pliku konczac ja .txt: ");
	y = 21;
	do
	{
		zn = getch();
		if (zn == 0x08)
		{

			if (x > menu)x--;
			if (z > 0)z--;
			gotoxy(x, y);
			putch(' ');
			nazwa_pliku[z] = '\0';
		}
		else
		{
			gotoxy(x, y);
			putch(zn);
			++x;
			nazwa_pliku[z] = zn;
			++z;
			nazwa_pliku[z] = '\0';
			fopen_s(&plik, nazwa_pliku, "r");
		}

	} while (plik == 0);

	int licznik = 0;
	while (znak != EOF && licznik != 4)
	{
		znak = fgetc(plik);
		if (licznik == 0)ostatni_ruch = znak;
		if (licznik == 1)
		{
			int pktb = atoi(&znak);
			pkt_bialych = pktb;
		}
		if (licznik == 2)
		{
			int pktc = atoi(&znak);
			pkt_czarnych = pktc;
		}
		if (licznik == 3)
		{
			int rozmiar = atoi(&znak);
			if (rozmiar == 9)r = 9;
			if (rozmiar == 1)r = 13;
			if (rozmiar == 2)r = 19;

		}
		++licznik;
	}
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			znak = fgetc(plik);
			ruchyT[i][j] = znak;
		}
	}
	fclose(plik);
}
void flood(int tabX, int tabY, char ostatni_ruch, char board[][19], const int boardsize, char visited[][19], int& pomocnicza) {

	if (tabX < 0 || tabY < 0 || tabX >= boardsize || tabY >= boardsize) return;
	if (board[tabY][tabX] == ostatni_ruch) return;
	if (board[tabY][tabX] == '?') return;
	if (board[tabY][tabX] == '.' || board[tabY][tabX] == '!') {
		pomocnicza = 0;
		return;
	}
	visited[tabY][tabX] = '@';
	board[tabY][tabX] = '?';

	flood(tabX - 1, tabY, ostatni_ruch, board, boardsize, visited, pomocnicza);
	flood(tabX + 1, tabY, ostatni_ruch, board, boardsize, visited, pomocnicza);
	flood(tabX, tabY - 1, ostatni_ruch, board, boardsize, visited, pomocnicza);
	flood(tabX, tabY + 1, ostatni_ruch, board, boardsize, visited, pomocnicza);
}
void zbicie_wielokrotne(char ostatni_ruch, char ruchyT[][19], int r, int& pkt_bialych, int& pkt_czarnych)
{
	char ostatni_ruchp = ' ';
	if (ostatni_ruch == 'X')ostatni_ruchp = 'O';
	else if (ostatni_ruch == 'O')ostatni_ruchp = 'X';
	int ile;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			char visited[19][19] = {};
			int pomocnicza = 1;
			ile = 0;
			flood(j, i, ostatni_ruch, ruchyT, r, visited, pomocnicza);
			if (pomocnicza == 0) {
				for (int y = 0; y < r; y++) {
					for (int x = 0; x < r; x++)
					{
						if (visited[y][x] == '@')
						{
							ruchyT[y][x] = ostatni_ruchp;
						}
					}

				}
			}
			else {
				for (int y = 0; y < r; y++) {
					for (int x = 0; x < r; x++)
					{
						if (visited[y][x] == '@')
						{
							++ile;
							ruchyT[y][x] = '.';
						}
					}
				}
				if (ostatni_ruch == 'X')pkt_bialych = pkt_bialych + ile;
				if (ostatni_ruch == 'O')pkt_czarnych = pkt_czarnych + ile;
			}
		}
	}


}
void board(char tab[][19], int r)
{
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			tab[i][j] = '.';
		}
	}
}
void wypisz(char tab[19][19], int r)
{
	gotoxy(plansza_x, plansza_y);
	int pom1 = plansza_x, pom2 = plansza_y;
	for (int i = 0; i < r; ++i)
	{
		pom1 = pom1 - 2;
		gotoxy(pom1, pom2);
		putch('|');
		pom1++;
		putch(' ');
		pom1++;
		for (int j = 0; j < r; ++j)
		{
			gotoxy(pom1, pom2);
			putch(tab[i][j]);
			pom1++;
			gotoxy(pom1, pom2);
			putch(' ');
			pom1++;
		}
		gotoxy(pom1, pom2);
		putch('|');
		pom1 = plansza_x;
		pom2++;
	}
	pom1 = plansza_x - 1, pom2 = plansza_y - 1;
	for (int i = 0; i <= 2 * r; ++i)
	{
		gotoxy(pom1, pom2);
		putch('_');
		pom1++;
	}
	pom1 = plansza_x - 1, pom2 = plansza_y + r;
	for (int i = 0; i <= 2 * r; ++i)
	{
		gotoxy(pom1, pom2);
		putch('-');
		pom1++;
	}
}
void granica(int& x, int& y, int r)
{
	if (x >= plansza_x + (2 * r) - 2)
	{
		x = plansza_x + (2 * r) - 2;
		gotoxy(x, y);
	}
	if (x <= plansza_x)
	{
		x = plansza_x;
		gotoxy(x, y);
	}
	if (y <= plansza_y)
	{
		y = plansza_y;
		gotoxy(x, y);
	}
	if (y >= plansza_y + r - 1)
	{
		y = plansza_y + r - 1;
		gotoxy(x, y);
	}
}
void ruchy(char tab[][19], int r, char ruchyT[][19])
{
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			if (ruchyT[i][j] == 'X' || ruchyT[i][j] == 'O')
			{
				tab[i][j] = ruchyT[i][j];
			}
		}
	}
}
int czy_zbicie(int r, char ruchyT[][19], int pozycja_x, int pozycja_y)
{
	int oddechy = 4;
	char ostatni_ruchp;
	if (ruchyT[pozycja_y][pozycja_x] == 'X')ostatni_ruchp = 'O';
	else if (ruchyT[pozycja_y][pozycja_x] == 'O')ostatni_ruchp = 'X';
	else return 0;
	if (r > 0 && pozycja_y > 0 && pozycja_y < r - 1 && pozycja_x>0 && pozycja_x < r - 1)
	{
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
	}
	else if (r > 0 && pozycja_x == 0 && pozycja_y < r - 1 && pozycja_y > 0)
	{
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
		if (oddechy == 1)return 1;
	}
	else if (r > 0 && pozycja_x == r - 1 && pozycja_y < r - 1 && pozycja_y > 0)
	{
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (oddechy == 1)return 1;
	}
	else if (r > 0 && pozycja_y == 0 && pozycja_x < r - 1 && pozycja_x > 0)
	{
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 1)return 1;
	}
	else if (r > 0 && pozycja_y == r - 1 && pozycja_x < r - 1 && pozycja_x > 0)
	{
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 1)return 1;
	}
	else if (pozycja_y == 0 && pozycja_x == 0)
	{
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 2)return 1;
	}
	else if (r > 0 && pozycja_y == 0 && pozycja_x == r - 1)
	{
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y + 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 2)return 1;
	}
	else if (r > 0 && pozycja_y == r - 1 && pozycja_x == 0)
	{
		if (ruchyT[pozycja_y][pozycja_x + 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 2)return 1;
	}
	else if (r > 0 && pozycja_y == r - 1 && pozycja_x == r - 1)
	{
		if (ruchyT[pozycja_y][pozycja_x - 1] == ostatni_ruchp)--oddechy;
		if (ruchyT[pozycja_y - 1][pozycja_x] == ostatni_ruchp)--oddechy;
		if (oddechy == 2)return 1;
	}
	if (oddechy == 0)return 1;
	else return 0;
}
void zbicie(int r, char ruchyT[][19], char ostatni_ruch, int& pkt_bialych, int& pkt_czarnych)
{
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			if (ruchyT[i][j] == '!')ruchyT[i][j] = '.';
			else if (czy_zbicie(r, ruchyT, j, i) == 1 && ruchyT[i][j] != ostatni_ruch)
			{
				if (ruchyT[i][j] == 'O')++pkt_bialych;
				if (ruchyT[i][j] == 'X')++pkt_czarnych;
				ruchyT[i][j] = '!';
			}
		}
	}
}
void czy_powinien_stac(int r, char ruchyT[][19], char& ostatni_ruch)
{
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			if (czy_zbicie(r, ruchyT, j, i) == 1)
			{
				if (ruchyT[i][j] == 'X')ostatni_ruch = 'O';
				else if (ruchyT[i][j] == 'O')ostatni_ruch = 'X';
				ruchyT[i][j] = '.';
			}
		}
	}
}
void czyszczenie_ruchow(char ruchyT[][19])
{
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			ruchyT[i][j] = '.';
		}
	}
}



