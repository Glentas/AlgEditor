#pragma once
#include "data_definitions.h"

#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QWidget>
#include <vector>

namespace AlgorithmicEditor
{
class Canvas : public QWidget
{
    Q_OBJECT
  public:
    explicit Canvas(QWidget *parent = nullptr, bool is_grid = true,
                    int pixel_size = Config::CELL_SIZE);
    ~Canvas();

    void set_pixel_size(int size);
    void set_grid_visibilty(bool grid);
    void set_locked(bool is_locked);

    int get_pixel_size() const;
    bool get_grid_visibilty() const;
    bool get_locked() const;

    void add_pixel(Point px);

  public slots:
    void on_clear();
    void on_grid_show();

  signals:
    void click_on_pixel(Point p);

  protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

  private:
    bool show_grid;
    bool is_locked;
    int pixel_size;
    std::vector<Point> pixels;
};
} // namespace AlgorithmicEditor