#include "model.h"
#include <iostream>

void Model::set_fields(QJsonArray data_array, std::size_t type) {
    if (data_array.size() == 0) {
        std::cout << "\nERROR\n"; // exceptions?
        return;
    }
    if (type == ALL_INSTRUMENTS) {
        number_of_instruments = data_array.size();
        for (int i = 0; i < number_of_instruments; ++i) {
            list_of_futures[i] = convert_to_std_string(data_array[i].toArray()[0]);
        }
        std::cout << "All instrument request done\n";
    }
    else if (type == ONE_INSTRUMENT) {
        size_of_data = data_array.size();
        current_line_number += size_of_data;

        for (std::size_t i = 0; i < size_of_data; ++i) {
            QJsonArray current_array = data_array[i].toArray();
            open[i] = current_array[0].toDouble();
            close[i] = current_array[1].toDouble();
            high[i] = current_array[2].toDouble();
            low[i] = current_array[3].toDouble();
            begin_time[i] = convert_to_std_string(current_array[6]);
            end_time[i] = convert_to_std_string(current_array[7]);
        }
        std::cout << "One instrument request done\n";
    }
    else {
        std::cout << "Unknown type of request\n";
    }
}

std::string Model::convert_to_std_string(QJsonValueRef arg) const {
    return arg.toString().toStdString();
}


std::ostream &operator<<(std::ostream &out, const Model &model) {
    out << "Name : " << model.short_name << "; Secid : " << model.secid << ";\n";
    for (std::size_t i = 0; i < model.size_of_data; ++i) {
        out << i << ": " << '\n';
        out << "open = " << model.open[i] << "; low = " << model.low[i] << "; high = " << model.high[i]
            << "; close = " << model.close[i] << '\n';
    }
    out << "--------------------------------------------------\n";
    return out;
}
