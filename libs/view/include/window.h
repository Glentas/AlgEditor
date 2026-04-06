#pragma once
#include "canvas.h"
#include "data_definitions.h"
#include "data_handler.h"
#include "debugger.h"

#include <QMainWindow>
#include <QString>
#include <QToolBar>
#include <QWidget>
#include <memory>
#include <vector>

namespace AlgorithmicEditor
{
class MainWindow : public QMainWindow
{
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  public slots:
    void on_info();
    void on_help();
    void on_click_on_pixel(Point px);
    void on_show_figures_dialog();

  signals:
  private:
    std::unique_ptr<Debugger> debugger;
    std::unique_ptr<DataHandler> data_handler;
    std::unique_ptr<Canvas> canvas;
    void add_toolbar_algorithm_button(QToolBar *toolbar, QString &&text,
                                      QString &&tool_tip,
                                      std::vector<GType> data);
    void init_working_area();
    void init_menu_bar();
    void init_toolbars();
    void setup_shortcuts();
};
} // namespace AlgorithmicEditor