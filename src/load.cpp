#include "load.h"

void load::do_request() {

}

std::string load::date_to_string(QDateTimeEdit *date) {
    std::string s = date->text().toStdString();
    for (int i = 0; i < s.length(); ++i)
        if (s[i] == '.')
            s[i] = '-';
    return s;
}
void load::set_url(std::string str) {
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    url = QUrl(cstr);
}


QUrl load::get_url() {
    return url;
}

void load::set_url(std::string str, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo) {
    str += "?from=" + date_to_string(dateFrom) + "&till=" + date_to_string(dateTo) + "&start=" + std::to_string(start);
    start += 500;
    std::cout << str << '\n';
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    url = QUrl(cstr);
}
