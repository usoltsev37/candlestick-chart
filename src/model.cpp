#include "model.h"
#include <iostream>

void Model::set_fields(QJsonArray data_array, std::size_t type) {
    if (data_array.size() == 0) {
        std::cout << "\nDataArraySize\n"; // exceptions?
        return;
    }
    if (type == ALL_INSTRUMENTS) {
        number_of_instruments = data_array.size();
        for (int i = 0; i < number_of_instruments; ++i) {
            list_of_futures.push_back(convert_to_std_string(data_array[i].toArray()[0]));
        }
        std::cout << "All instrument request done\n";
    }
    else if (type == ONE_INSTRUMENT) {
        size_of_data = data_array.size();
        for (std::size_t i = 0; i < size_of_data; ++i) {
            QJsonArray current_array = data_array[i].toArray();
            open.push_back(current_array[0].toDouble());
            close.push_back(current_array[1].toDouble());
            high.push_back(current_array[2].toDouble());
            low.push_back(current_array[3].toDouble());
            begin_time.push_back(convert_to_std_string(current_array[6]));
            end_time.push_back(convert_to_std_string(current_array[7]));
        }
        current_line_number += size_of_data;
        std::cout << "One instrument request done: current_line_number = " << current_line_number << "\n";
    }
    else {
        std::cout << "Unknown type of request\n";
    }
}

std::string Model::convert_to_std_string(QJsonValueRef arg) const {
    return arg.toString().toStdString();
}


std::ostream &operator<<(std::ostream &out, const Model &model) {
    out << "Start:\n";
    for (std::size_t i = 0; i < model.current_line_number; ++i) {
        out << i << ": " << model.begin_time[i] << '\n';
        out << "open = " << model.open[i] << "; low = " << model.low[i] << "; high = " << model.high[i]
            << "; close = " << model.close[i] << '\n';
    }
    out << "--------------------------------------------------\n";
    return out;
}
