#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <model.h>
#include <string>
#include <vector>

class controller {
public:
    controller(std::string company, std::string period);

    std::string get_url();    // по company выдаёт url
    std::size_t request_for_load(); // "url" -> load.cpp - "data" -> model.cpp - data.push_back(model) - index
    void request_for_view(std::size_t index);  //  index - &model.pop() -> view.cpp - view
private:
    std::string company;
    std::string period;
    std::string url;
    std::vector<Model> data;
};

#endif // CONTROLLER_H
