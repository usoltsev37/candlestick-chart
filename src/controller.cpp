#include "controller.h"
#include <model.h>
#include <string>

controller::controller() {
    dialog = new MainDialog(company, dateFrom, dateTo);
}

std::string controller::get_url() {
    return std::string();
}

std::size_t controller::request_for_load() {
    return 0;
}

void controller::request_for_view(std::size_t index) {

}

void controller::show_dialog() {
    dialog->show();
}
