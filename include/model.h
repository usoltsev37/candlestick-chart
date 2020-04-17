#ifndef MODEL_H
#define MODEL_H

#include "load.h"
#include <string>
#include <QJsonArray>
#include <vector>

namespace ModelData {
    class Data {
    public:
        std::vector<std::string> begin_time;
        std::vector<std::string> end_time;
        std::vector<double> open;
        std::vector<double> close;
        std::vector<double> high;
        std::vector<double> low;
    };
} // ModelData

class Model {
public:
    Model() = default;
    ~Model() = default;

    void set_fields(QJsonArray data_array, std::size_t type);
    std::string convert_to_std_string(QJsonValueRef arg) const;
    friend std::ostream &operator<<(std::ostream &out, const Model &model);

    //временно влез, потом сделаю через контроллер
    std::vector<std::string> get_bt();
    std::vector<std::string> get_et();
    std::vector<double> get_op();
    std::vector<double> get_cl();
    std::vector<double> get_hi();
    std::vector<double> get_lo();
    //извиняюсь)

private:
    ModelData::Data data;
    std::size_t size_of_data;
    std::size_t current_line_number = 0;
    std::size_t number_of_instruments;
    std::vector<std::string> list_of_futures;
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H
