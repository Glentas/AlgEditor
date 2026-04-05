#include "canvas.h"

#include "data_definitions.h"

#include <QInputDialog>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QWidget>
#include <iostream>
#include <vector>

namespace AlgorithmicEditor
{
Canvas::Canvas(QWidget *parent, bool is_grid, int pixel_size)
    : QWidget(parent), show_grid(is_grid), pixel_size(pixel_size), pixels({}),
      is_locked(false)
{
    this->set_pixel_size(pixel_size);
}

Canvas::~Canvas() { std::cout << "Canvas out...\n"; }

void Canvas::set_pixel_size(int size)
{
    if (size < 1 || size > 2 * Config::CELL_SIZE) {
        return;
    }

    this->pixel_size = size;
    this->setFixedSize(Config::CANVAS_SIZE * this->pixel_size,
                       Config::CANVAS_SIZE * this->pixel_size);
    this->update();
}
void Canvas::set_grid_visibilty(bool grid)
{
    this->show_grid = grid;
    this->update();
}
void Canvas::set_locked(bool lock) { this->is_locked = lock; }

int Canvas::get_pixel_size() const { return this->pixel_size; }
bool Canvas::get_grid_visibilty() const { return this->show_grid; }
bool Canvas::get_locked() const { return this->is_locked; }

void Canvas::add_pixel(Point px)
{
    if (px.x < 0 || px.x > Config::CANVAS_SIZE - 1 || px.y < 0 ||
        px.y > Config::CANVAS_SIZE - 1) {
        return;
    }
    this->pixels.push_back(px);
    this->update();
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(), Qt::white);

    QPen grey_pen(Qt::lightGray, 1, Qt::SolidLine);
    QPen black_pen(Qt::black, 1, Qt::SolidLine);
    p.setPen(grey_pen);

    if (this->pixel_size > 1 && this->show_grid == true) {
        int step = this->pixel_size;

        for (int x = 0; x <= this->width(); x += step) {
            if (x % (10 * step) == 0) {
                p.setPen(black_pen);
                p.drawLine(x, 0, x, this->height());
                p.setPen(grey_pen);
            } else {
                p.drawLine(x, 0, x, this->height());
            }
        }

        for (int y = 0; y <= this->height(); y += step) {
            if (y % (10 * step) == 0) {
                p.setPen(black_pen);
                p.drawLine(0, y, this->width(), y);
                p.setPen(grey_pen);
            } else {
                p.drawLine(0, y, this->width(), y);
            }
        }
    }

    p.setPen(black_pen);

    int side = this->pixel_size > 1 ? this->pixel_size - 1 : 1;
    for (auto px : this->pixels) {
        int x = this->pixel_size > 1 ? px.x_int() * this->pixel_size + 1
                                     : px.x_int();
        int y = this->pixel_size > 1 ? px.y_int() * this->pixel_size + 1
                                     : px.y_int();
        QColor col(px.r, px.g, px.b);
        p.fillRect(x, y, side, side, col);
    }
}

void Canvas::on_clear()
{
    this->pixels.clear();
    this->update();
}

void Canvas::on_grid_show()
{
    if (this->show_grid == true) {
        this->set_grid_visibilty(false);
    } else {
        this->set_grid_visibilty(true);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && this->is_locked == false) {
        int widget_x = event->pos().x();
        int widget_y = event->pos().y();

        int grid_x = widget_x / this->pixel_size;
        int grid_y = widget_y / this->pixel_size;

        if (grid_x >= 0 && grid_x < Config::CANVAS_SIZE && grid_y >= 0 &&
            grid_y < Config::CANVAS_SIZE) {
            Point px = {static_cast<double>(grid_x),
                        static_cast<double>(grid_y), 255, 64, 64};

            this->add_pixel(px);

            emit this->click_on_pixel(px);
        }
    }
}

} // namespace AlgorithmicEditor