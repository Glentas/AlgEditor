#include "data_handler.h"

#include "data_definitions.h"
#include "debugger.h"
#include "figure_class.h"
#include "transformations.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace AlgorithmicEditor
{
DataHandler::DataHandler(Debugger *debugger)
    : id_counter(0), current_active_idx(Config::CODE_ERROR),
      previous_active_idx(Config::CODE_ERROR), debugger(debugger)
{
}

DataHandler::~DataHandler() { std::cout << "Data handler out...\n"; }

void DataHandler::set_current_active_idx(int idx)
{
    if (idx < 0) {
        return;
    }
    this->current_active_idx = idx;
}
void DataHandler::set_previous_active_idx(int idx)
{
    if (idx < 0) {
        return;
    }
    this->previous_active_idx = idx;
}
void DataHandler::set_figure(std::unique_ptr<Figure> figure)
{
    if (figure == nullptr) {
        return;
    }
    this->current_figure = std::move(figure);
}
void DataHandler::set_figures(std::vector<std::unique_ptr<Figure>> figures)
{
    this->figures = std::move(figures);
}

int DataHandler::get_current_active_idx() const
{
    return this->current_active_idx;
}
int DataHandler::get_previous_active_idx() const
{
    return this->previous_active_idx;
}
const Figure *DataHandler::get_figure() const
{
    return this->current_figure.get();
}
const std::vector<std::unique_ptr<Figure>> &DataHandler::get_figures() const
{
    return this->figures;
}

void DataHandler::reset()
{
    this->current_figure.reset();
    this->figures.clear();
    this->id_counter = 0;
    this->current_active_idx = Config::CODE_ERROR;
    this->current_active_idx = Config::CODE_ERROR;
}

void DataHandler::save_figure(std::unique_ptr<Figure> figure)
{
    if (figure == nullptr) {
        return;
    }
    std::unique_ptr<Figure> renewed_figure = figure->clone();
    renewed_figure->clear();
    this->id_counter++;
    figure->set_id(this->id_counter);
    this->figures.push_back(std::move(figure));
    this->current_active_idx = this->figures.size() - 1;
    // To not select new figure every time
    this->current_figure = std::move(renewed_figure);
}

void DataHandler::launch_debugger()
{
    this->debugger->clear_canvas();
    bool current_debug = this->debugger->get_debug();

    this->debugger->set_debug(false);

    for (int i = 0; i < this->figures.size(); i++) {
        if (i != this->current_active_idx) {
            this->debugger->set_figure(this->figures[i].get());
            this->debugger->begin_debug();
        }
    }

    this->debugger->set_debug(current_debug);

    this->debugger->set_figure(this->figures[this->current_active_idx].get());
    this->debugger->begin_debug();
}

void DataHandler::add_point(Point pt)
{
    if (this->current_figure == nullptr) {
        return;
    }
    if (this->current_figure->add_and_validate_point(pt)) {
        this->finish_and_draw();
    }
}

void DataHandler::finish_and_draw()
{
    if (this->current_figure == nullptr) {
        return;
    }
    this->update_figure(this->current_figure.get());
    this->save_figure(std::move(this->current_figure));
    this->launch_debugger();
}

void DataHandler::update_figure(Figure *figure) { figure->regenerate_cache(); }

void DataHandler::rotate_f(char axis)
{
    if (this->figures.size() <= 0) {
        return;
    }

    Figure *active_figure = this->figures[this->current_active_idx].get();

    std::vector<Point> updated_points;
    if (active_figure->get_figure_type() == GType::Circle ||
        active_figure->get_figure_type() == GType::Ellipse ||
        active_figure->get_figure_type() == GType::Parabola ||
        active_figure->get_figure_type() == GType::Hyperbola) {
        updated_points = rotate_figure(active_figure->get_points(), axis, true,
                                       active_figure->get_points()[0]);
    } else {
        updated_points = rotate_figure(active_figure->get_points(), axis);
    }
    active_figure->set_points(updated_points);

    this->update_figure(active_figure);
    this->launch_debugger();
}

void DataHandler::scale_f(char method)
{
    if (this->figures.size() <= 0) {
        return;
    }

    Figure *active_figure = this->figures[this->current_active_idx].get();

    std::vector<Point> updated_points;
    if (active_figure->get_figure_type() == GType::Circle ||
        active_figure->get_figure_type() == GType::Ellipse ||
        active_figure->get_figure_type() == GType::Parabola ||
        active_figure->get_figure_type() == GType::Hyperbola) {
        updated_points = scale_figure(active_figure->get_points(), method, true,
                                      active_figure->get_points()[0]);
    } else {
        updated_points = scale_figure(active_figure->get_points(), method);
    }
    active_figure->set_points(updated_points);

    this->update_figure(active_figure);
    this->launch_debugger();
}

void DataHandler::move_f(char direction)
{
    if (this->figures.size() <= 0) {
        return;
    }

    Figure *active_figure = this->figures[this->current_active_idx].get();

    std::vector<Point> updated_points;

    updated_points = move_figure(active_figure->get_points(), direction);

    active_figure->set_points(updated_points);

    this->update_figure(active_figure);
    this->launch_debugger();
}

} // namespace AlgorithmicEditor