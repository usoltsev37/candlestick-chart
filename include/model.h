#ifndef MODEL_H
#define MODEL_H

//#include "load.h"
#include <string>
#include <QJsonArray>
#include <vector>

class ModelData {
public:
    double open;
    double close;
    double high;
    double low;
    std::string begin_time;
    std::string end_time;
};

class Model {
public:
    Model() = default;

    ~Model() = default;

    std::string get_future_name(int index);
    std::size_t get_number_of_instruments();

    void set_fields(QJsonArray &data_array, std::size_t type);

    std::string convert_to_std_string(QJsonValueRef arg) const;

    friend std::ostream &operator<<(std::ostream &out, const Model &model);

    ModelData get_data_byIndex(std::size_t index) /*const*/;

    std::size_t get_size() const;

private:
    std::vector<ModelData> data_;
    std::size_t data_sz_;
    std::size_t current_line_number_ = 0;
    std::size_t instruments_num_;
    std::vector<std::string> futures_list_;
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H
