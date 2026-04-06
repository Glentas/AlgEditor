#pragma once

#include "data_definitions.h"
#include "debugger.h"

#include <memory>
#include <vector>

namespace AlgorithmicEditor
{
class DataHandler
{
  public:
    DataHandler(Debugger *debugger);
    ~DataHandler();
    void set_current_active_idx(int idx);
    void set_previous_active_idx(int idx);
    void set_figure(std::unique_ptr<Figure> figure);
    void set_figures(std::vector<std::unique_ptr<Figure>> figures);

    int get_current_active_idx() const;
    int get_previous_active_idx() const;
    const Figure *get_figure() const;
    const std::vector<std::unique_ptr<Figure>> &get_figures() const;

    void reset();
    void add_point(Point p);
    void finish_and_draw();

    void launch_debugger();

    void rotate_f(char axis);
    void scale_f(char method);
    void move_f(char direction);

  private:
    uint16_t id_counter;
    int current_active_idx;
    std::unique_ptr<Figure> current_figure;
    std::vector<std::unique_ptr<Figure>> figures;
    Debugger *debugger;
    void save_figure(std::unique_ptr<Figure> figure);
    void update_figure(Figure *figure);
};
} // namespace AlgorithmicEditor