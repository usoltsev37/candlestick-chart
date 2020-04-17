#include "model.h"
#include <iostream>

void Model::set_fields(QJsonArray &data_array, std::size_t type) {
    if (data_array.empty()) {
        std::cout << "\nDataArraySize\n";
    } else if (type == ALL_INSTRUMENTS) {
        instruments_num_ = data_array.size();
        for (std::size_t i = 0; i < instruments_num_; i++) {
            futures_list_.push_back(convert_to_std_string(data_array[i].toArray()[0]));
        }
        std::cout << "All instrument request done\n";
    } else if (type == ONE_INSTRUMENT) {
        data_sz_ = data_array.size();
        for (std::size_t i = 0; i < data_sz_; i++) {
            QJsonArray array = data_array[i].toArray();
            ModelData current_data;
            current_data.open = array[0].toDouble();
            current_data.close = array[1].toDouble();
            current_data.high = array[2].toDouble();
            current_data.low = array[3].toDouble();
            current_data.begin_time = convert_to_std_string(array[6]);
            current_data.end_time = convert_to_std_string(array[7]);
            data_.push_back(current_data);
        }
        current_line_number_ += data_sz_;
        std::cout << "One instrument request done: current_line_number = " << current_line_number_ << "\n";
    } else {
        std::cout << "Unknown type of request\n";
    }
}

std::string Model::convert_to_std_string(QJsonValueRef arg) const {
    return arg.toString().toStdString();
}


std::ostream &operator<<(std::ostream &out, const Model &model) {
    out << "Start:\n";
    for (std::size_t i = 0; i < model.current_line_number_; i++) {
        out << i << ": " << model.data_[i].begin_time << '\n'
            << "open = " << model.data_[i].open
            << "; low = " << model.data_[i].low
            << "; high = " << model.data_[i].high
            << "; close = " << model.data_[i].close << '\n';
    }
    out << "--------------------------------------------------\n";
    return out;
}

ModelData Model::get_data_byIndex(std::size_t index) const {
    return data_[index];
}
