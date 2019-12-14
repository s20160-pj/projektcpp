#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <map>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;

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
    std::map<spolka::key_type, spolka> spolki;
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

void get_gpw() {
    auto g = pobierz();

    gielda gpw;
    std::istringstream input;
    input.str(g);

    auto lines = std::vector<std::string>{};
    for (string line; std::getline(input, line); ) {
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
        cout << spolka.nazwa << "\n";
    }
}

int main() {
    get_gpw();
    return 0;
}
