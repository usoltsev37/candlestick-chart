#include "model.h"
#include <iostream>

Model::Model(QJsonArray data_array) {
    size_of_data = data_array.size();
    if (size_of_data == 0) {
        std::cout << "\nERROR\n"; // exceptions?
        return;
    }
    short_name = convert_to_std_string(data_array[0].toArray()[2]);
    secid = convert_to_std_string(data_array[0].toArray()[3]);
//    date_today = new std::string[size_of_data];
//    open = new double[size_of_data];
//    low = new double[size_of_data];
//    high = new double[size_of_data];
//    close = new double[size_of_data];
    for (std::size_t i = 0; i < data_array.size(); ++i) {
        QJsonArray current_array = data_array[i].toArray();
        date_today[i] = convert_to_std_string(current_array[1]);
        open[i] = current_array[6].toDouble();
        low[i] = current_array[7].toDouble();
        high[i] = current_array[8].toDouble();
        close[i] = current_array[11].toDouble();
    }
}

Model::Model(QJsonArray data_array, size_t size_of_array) {
    number_of_futures = size_of_array;
//    list_of_futures = new std::string[size_of_array];
    for (int i = 0; i < size_of_array; ++i) {
        list_of_futures[i] = convert_to_std_string(data_array[i].toArray()[0]);
        std::cout << i << ':' << list_of_futures[i] << '\n';
    }
}


std::string Model::convert_to_std_string(QJsonValueRef arg) const {
    return arg.toString().toStdString();
}

std::ostream &operator<<(std::ostream &out, const Model &model) {
    out << "Name : " << model.short_name << "; Secid : " << model.secid << ";\n";
    for (std::size_t i = 0; i < model.size_of_data; ++i) {
        out << "date : " << model.date_today[i] << '\n';
        out << "open = " << model.open[i] << "; low = " << model.low[i] << "; high = " << model.high[i]
            << "; close = " << model.close[i] << '\n';
    }
    out << "--------------------------------------------------\n";
    return out;
}

Model::~Model() {
//    delete[] list_of_futures;
//    delete[] open;
//    delete[] low;
//    delete[] high;
//    delete[] close;
//    delete[] date_today;
}
