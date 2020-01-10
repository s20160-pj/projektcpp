#include <iostream>
#include <string>
#include <curl/curl.h>
#include <ncurses.h>
#include <vector>
#include <map>
#include <sstream>
#include <bits/stdc++.h> //toupper
#include <fstream> //pliki

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;
using std::map;
using std::getline;

string kurs, nazwa;

struct spolka {
	string nazwa;
	string data;
	double otwarcie;
	double max;
	double min;
	double aktualny;
	double wolumen_obrotu;

	using key_type = decltype(nazwa);
};

struct gielda {
	map<spolka::key_type, spolka> spolki;
};

static size_t write(void* prt, size_t size, size_t nmemb, string* data) {
	data->append((char*) prt, size * nmemb);
	return size * nmemb;
}

string pobierz() {
	string response;
	auto curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://info.bossa.pl/pub/ciagle/mstock/sesjacgl/sesjacgl.prn");
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl = NULL;
	return response;
}

gielda get_gpw() {
	auto g = pobierz();

	gielda gpw;
	std::istringstream input;
	input.str(g);

	auto lines = vector<string>{};
	for (string line; getline(input, line); ) {
	lines.push_back(line);
	}


	for (auto each : lines) {
	auto nazwa = each.substr(0, each.find(','));
	each = each.substr(nazwa.size() + 1);
	auto data = each.substr(0, each.find(','));
	each = each.substr(data.size() + 1);
	auto otwarcie = each.substr(0, each.find(','));
	each = each.substr(otwarcie.size() + 1);
	auto max = each.substr(0, each.find(','));
	each = each.substr(max.size() + 1);
	auto min = each.substr(0, each.find(','));
	each = each.substr(min.size() + 1);
	auto aktualny = each.substr(0, each.find(','));
	each = each.substr(aktualny.size() + 1);
	auto wolumen_obrotu = each.substr(0, each.find(','));
	each = each.substr(wolumen_obrotu.size());

	//std::cerr << each << "\n";
	/* gpw.nazwa.push_back(nazwa); */
	/* gpw.data.push_back(data); */

	spolka s;
	s.nazwa = nazwa;
	s.data = data;
	s.otwarcie = stod(otwarcie);
	s.max = stod(max);
	s.min = stod(min);
	s.aktualny = stod(aktualny);
	s.wolumen_obrotu = stod(wolumen_obrotu);

	gpw.spolki[s.nazwa] = s;
	}

	for (auto const& each : gpw.spolki) {
	auto const& spolka = each.second;
	}
	return gpw;
}

void sprawdz_kurs() {
	char nazwa_tab[20];
	char kurs_tab[20];
	mvprintw(0, 0, "Podaj nazwe spolki\n");
	getstr(nazwa_tab);
	clear();
	nazwa = nazwa_tab;
	transform(nazwa.begin(), nazwa.end(), nazwa.begin(), ::toupper);
	spolka s = get_gpw().spolki.at(nazwa);
	kurs = std::to_string(s.aktualny);
	size_t pozycja = kurs.find(".");
	kurs.erase(pozycja + 3, kurs.length() - 1);
	for (int i = 0; i < kurs.length() ; i++) {
		kurs_tab[i] = kurs[i];
	}
	mvprintw(0, 0, "Kurs spolki");
	mvprintw(1, 0, nazwa_tab);
	mvprintw(2, 0, "wynosi:");
	mvprintw(3, 0, kurs_tab);
}
	
void kup() {
	char ilosc_tab[20];
	char warunek;
	clear();
	sprawdz_kurs();
	mvprintw(4, 0, "Podaj ilosc akcji jaka chcesz kupic\n");
	getstr(ilosc_tab);
	clear();
	mvprintw(0, 0, "Czy na pewno chcesz zakupic akcje? t/n\n");
	getch(warunek);
	while ((warunek != "t" && warunek != "T") && (warunek != "n" && warunek != "N")) {
		getch(warunek);
	}
	if (warunek == "t" || warunek == "T") {
		std::fstream portfel;
	}
	
	//std::ofstream plik("portfel.txt");

}

int main() {
	initscr();
	const char txt1[] = "Sprawdz kurs spolki";
	const char txt2[] = "Kup akcje";
	const char txt3[] = "Wyjscie";
	int ktory = 1;
	int znak;
	const short int min_wybor = 1;
	const short int max_wybor = 3;

	mvprintw(5, 5, txt1);
	mvprintw(6, 5, txt2);
	mvprintw(7, 5, txt3);
	do
	{
	znak = getch();
	clear();
	if (znak == 'a' && ktory != min_wybor)
	{
	    ktory--;
	}
	else if (znak == 'z' && ktory != max_wybor)
	{
	    ktory++;
	}
	if (znak == 10)
	{
	    switch(ktory)
	    {
	    case 1:
	    	sprawdz_kurs();
	    	kurs.clear();
    		nazwa.clear();
		break;
	    case 2:
		kup();
		break;
	    }
	}
	switch(ktory)
	{
	case 1:
	    mvprintw(6, 5, txt2);
	    mvprintw(7, 5, txt3);
	    attron(A_REVERSE);
	    mvprintw(5, 5, txt1);
	    break;

	case 2:
	    mvprintw(5, 5, txt1);
	    mvprintw(7, 5, txt3);
	    attron(A_REVERSE);
	    mvprintw(6, 5, txt2);
	    break;

	case 3:
	    mvprintw(5, 5, txt1);
	    mvprintw(6, 5, txt2);
	    attron(A_REVERSE);
	    mvprintw(7, 5, txt3);
	    break;
	}
	attroff(A_REVERSE);
	} while (ktory != 3 || znak != 10);

	move(9, 0);
	endwin();
	return 0;
}
