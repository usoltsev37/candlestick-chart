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

void load::set_url(QString company_, QDateTimeEdit* dateFrom_, QDateTimeEdit* dateTo_) {
    std::string company = company_.toStdString();
    std::string dateFrom = date_to_string(dateFrom_);
    std::string dateTo = date_to_string(dateTo_);
    std::string str = "http://iss.moex.com/iss/history/engines/stock/markets/shares/securities/"
                       + company + ".json?from=" + dateFrom + "&till=" + dateTo;
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    std::cout << str << '\n';
    url = QUrl(cstr);
}

QUrl load::get_url() {
    return url;
}

