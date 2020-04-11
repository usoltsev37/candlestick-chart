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
    std::vector<std::string> begin_time;
    std::vector<std::string> end_time;
    std::vector<double> open;
    std::vector<double> close;
    std::vector<double> high;
    std::vector<double> low;
    size_t number_of_instruments;
    std::vector<std::string> list_of_futures;
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H