#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <QJsonArray>

class Model {
public:
    Model() = default;

    explicit Model(QJsonArray data_array);
    explicit Model(QJsonArray data_array, size_t size_of_array);

    ~Model();

    std::string convert_to_std_string(QJsonValueRef arg) const;

    friend std::ostream &operator<<(std::ostream &out, const Model &model);

private:
    size_t size_of_data;
    std::string short_name;
    std::string secid;
    std::string begin_time[10000];
    std::string end_time[10000];
    double open[10000];
    double low[10000];
    double high[10000];
    double close[10000];
    size_t number_of_futures;
    std::string list_of_futures[10000];
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H