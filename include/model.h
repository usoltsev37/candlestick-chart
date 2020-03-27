#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <QJsonArray>

class Model {
public:
    Model() = default;

    explicit Model(QJsonArray data_array);

    ~Model();

    std::string convert_to_std_string(QJsonValueRef arg) const;

    friend std::ostream &operator<<(std::ostream &out, const Model &model);

private:
    size_t size_of_data;
    std::string short_name;
    std::string secid;
    std::string *date_today;
    double *open;
    double *low;
    double *high;
    double *legal_close_price;
    double *close;
};

std::ostream &operator<<(std::ostream &out, const Model &model);

#endif // MODEL_H