#pragma once
#include "canvas.h"
#include "figure_class.h"
namespace AlgorithmicEditor
{
class Debugger
{
  public:
    Debugger(Canvas *canvas = nullptr, bool debug_mode = false, int step = 0);
    ~Debugger();

    void set_debug(bool debug);
    void set_canvas(Canvas *canvas);
    void set_figure(Figure *figure);

    bool get_debug() const;
    const Canvas *get_canvas() const;

    void reset();
    void clear_canvas();
    bool step();
    void begin_debug();

  private:
    bool debug_mode;
    Canvas *canvas;
    Figure *figure;
    int step_i;
};
} // namespace AlgorithmicEditor