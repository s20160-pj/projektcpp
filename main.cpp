#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;

struct gielda {
    vector <string> nazwa;
    vector <string> data;
    vector <double> otwarcie;
    vector <double> max;
    vector <double> min;
    vector <double> aktualny;
    vector <double> wolumen_obrotu;
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


    for (auto const& each : lines) {
        gpw.nazwa.push_back(each.substr(0, each.find(',')));
    }
        for(auto const & nazwa : gpw.nazwa) {
    cout << nazwa << "\n";
    }
}


int main() {
    get_gpw();
    return 0;
}
