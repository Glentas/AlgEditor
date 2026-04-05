#include "debugger.h"

#include "canvas.h"
#include "data_definitions.h"

#include <iostream>
#include <vector>

namespace AlgorithmicEditor
{
Debugger::Debugger(Canvas *canvas, bool debug_mode, int step)
    : canvas(canvas), debug_mode(debug_mode), step_i(step), figure(nullptr)
{
}

Debugger::~Debugger() { std::cout << "Debugger out...\n"; }

void Debugger::set_debug(bool debug) { this->debug_mode = debug; }

void Debugger::set_canvas(Canvas *canvas) { this->canvas = canvas; }

void Debugger::set_figure(Figure *figure) { this->figure = figure; }

bool Debugger::get_debug() const { return this->debug_mode; }

const Canvas *Debugger::get_canvas() const { return this->canvas; }

void Debugger::reset()
{
    this->step_i = 0;
    this->canvas->set_locked(false);
    this->debug_mode = false;
    this->figure = nullptr;
}

void Debugger::clear_canvas() { this->canvas->on_clear(); }

bool Debugger::step()
{
    if (this->figure == nullptr) {
        return false;
    }
    const std::vector<Point> &pts = this->figure->get_connected_points();

    if (pts.size() >= 1) {
        this->canvas->add_pixel(pts[step_i]);
        this->step_i++;
        if (this->step_i >= pts.size() - 1) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void Debugger::begin_debug()
{
    if (this->figure == nullptr) {
        return;
    }

    if (this->figure->get_points().empty()) {
        return;
    }

    this->canvas->set_locked(true);

    if (this->debug_mode == false) {
        while (this->step()) {
        }
        this->reset();
        this->debug_mode = false;
        this->step_i = 0;
    } else {
        if (this->step()) {
        } else {
            this->reset();
            this->debug_mode = true;
        }
    }
}

} // namespace AlgorithmicEditor