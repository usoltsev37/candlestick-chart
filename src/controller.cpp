#include "controller.h"
#include <model.h>
#include <string>

controller::controller(std::string company, std::string period) {

}

std::string controller::get_url() {
    return std::string();
}

std::size_t controller::request_for_load() {
    return 0;
}

void controller::request_for_view(std::size_t index) {

}
