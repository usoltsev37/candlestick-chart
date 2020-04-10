#ifndef MODEL_H
#define MODEL_H

#include "load.h"
#include <string>
#include <QJsonArray>

class Model {
public:
    Model() = default;
    ~Model() = default;

    void set_fields(QJsonArray data_array, std::size_t type);
    std::string convert_to_std_string(QJsonValueRef arg) const;
    friend std::ostream &operator<<(std::ostream &out, const Model &model);

private:
    size_t size_of_data;
    size_t current_line_number = 0;
    std::string short_name;
    std::string secid;
    std::string begin_time[10000];
    std::string end_time[10000];
    double open[10000];
    double close[10000];
    double high[10000];
    double low[10000];
    size_t number_of_instruments;
    std::string list_of_futures[10000];
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H