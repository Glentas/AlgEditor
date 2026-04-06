#include "window.h"

#include "canvas.h"
#include "data_definitions.h"
#include "data_handler.h"
#include "debugger.h"
#include "figure_class.h"
#include "tools.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QString>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <memory>

namespace AlgorithmicEditor
{
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("Editor");
    this->resize(Config::INIT_WIDTH, Config::INIT_HEIGHT);
    this->setMaximumWidth(Config::MAX_WINDOW_WIDTH);
    this->setMaximumHeight(Config::MAX_WINDOW_HEIGHT);
    this->setMinimumWidth(Config::MIN_WINDOW_WIDTH);
    this->setMinimumHeight(Config::MIN_WINDOW_HEIGHT);

    // Init working area
    // The very "parent" widget
    QWidget *central = new QWidget(this);
    this->setCentralWidget(central);

    // Manage global working area
    QHBoxLayout *main_view = new QHBoxLayout(central);

    // Frame with canvas
    QFrame *border_frame = new QFrame(central);

    QScrollArea *scroll_area = new QScrollArea(border_frame);
    scroll_area->setWidgetResizable(false);

    this->canvas = std::make_unique<Canvas>(scroll_area);
    this->debugger = std::make_unique<Debugger>(this->canvas.get());
    this->data_handler = std::make_unique<DataHandler>(this->debugger.get());
    this->data_handler->set_figure(std::make_unique<CDAFigure>());

    this->connect(this->canvas.get(), &Canvas::click_on_pixel, this,
                  &MainWindow::on_click_on_pixel);

    scroll_area->setWidget(this->canvas.get());

    // Manage working area inside frame
    QVBoxLayout *frame_layout = new QVBoxLayout(border_frame);
    frame_layout->setContentsMargins(Config::CELL_SIZE, Config::CELL_SIZE,
                                     Config::CELL_SIZE, Config::CELL_SIZE);
    frame_layout->addWidget(scroll_area);

    main_view->addWidget(border_frame);

    // Init menu bar
    QMenu *file_menu = this->menuBar()->addMenu("Info");
    QMenu *settings_menu = this->menuBar()->addMenu("Settings");

    QAction *program_help = new QAction("Help", this);
    this->connect(program_help, &QAction::triggered, this,
                  &MainWindow::on_help);

    QAction *program_about = new QAction("About", this);
    this->connect(program_about, &QAction::triggered, this,
                  &MainWindow::on_info);

    QAction *active_figure_setting = new QAction("Set active figure", this);
    this->connect(active_figure_setting, &QAction::triggered, this,
                  &MainWindow::on_show_figures_dialog);

    QAction *grid_show_setting = new QAction("Show grid", this);
    grid_show_setting->setCheckable(true);
    grid_show_setting->setChecked(true);
    grid_show_setting->setToolTip("Enable/disable grid");
    this->connect(grid_show_setting, &QAction::toggled, this->canvas.get(),
                  &Canvas::on_grid_show);

    file_menu->addAction(program_help);
    file_menu->addAction(program_about);

    settings_menu->addAction(active_figure_setting);
    settings_menu->addAction(grid_show_setting);

    // Init toolbar
    QToolBar *tools_toolbar = this->addToolBar("Tools");
    tools_toolbar->addSeparator();

    // Setup misc buttons
    QToolButton *clear_canvas_btn = new QToolButton(tools_toolbar);
    clear_canvas_btn->setText("Clear");
    clear_canvas_btn->setToolTip(
        "Clears workspace by filling it with white color");
    clear_canvas_btn->setMaximumWidth(4 * Config::CELL_SIZE);
    tools_toolbar->addWidget(clear_canvas_btn);
    this->connect(clear_canvas_btn, &QToolButton::clicked, this, [this]() {
        this->canvas->on_clear();
        bool d = this->debugger->get_debug();
        this->debugger->reset();
        this->data_handler->reset();
        this->debugger->set_debug(d);
    });

    QCheckBox *debug_checkbox = new QCheckBox("Debug", tools_toolbar);
    debug_checkbox->setToolTip("Debug mode");
    debug_checkbox->setChecked(false);
    tools_toolbar->addWidget(debug_checkbox);
    this->connect(debug_checkbox, &QCheckBox::toggled, this,
                  [this](bool checked) {
                      if (checked) {
                          this->debugger->reset();
                          this->debugger->set_debug(true);
                      } else {
                          this->debugger->set_debug(false);
                          this->debugger->begin_debug();
                      }
                  });

    QToolBar *algorithms_toolbar = this->addToolBar("Algorithms");
    algorithms_toolbar->addSeparator();

    this->add_toolbar_algorithm_button(
        algorithms_toolbar, "FRLine",
        "Various first rank line drawing algorithms",
        {GType::CDA, GType::Bresenham, GType::Wu});
    this->add_toolbar_algorithm_button(
        algorithms_toolbar, "SRLine",
        "Various second rank line drawing algorithms",
        {GType::Circle, GType::Ellipse, GType::Parabola, GType::Hyperbola});
    this->add_toolbar_algorithm_button(
        algorithms_toolbar, "Interpolation", "Various interpolation algorithms",
        {GType::Hermite, GType::Bezier, GType::BSpline});
    this->add_toolbar_algorithm_button(algorithms_toolbar, "3D", "3D figures",
                                       {GType::Cube, GType::Tetrahedron});
    this->add_toolbar_algorithm_button(algorithms_toolbar, "Polygon",
                                       "Various polygon drawing algorithms",
                                       {GType::Polygon, GType::ConvexPolygon});
    this->add_toolbar_algorithm_button(algorithms_toolbar, "Polygonalisation",
                                       "Various polygonalisation algorithms",
                                       {GType::Delaunay, GType::Voronoi});

    // Setup shortcuts

    // Increase cell size
    new QShortcut(QKeySequence("+"), this, [this]() {
        int sz = this->canvas->get_pixel_size();
        sz++;
        canvas->set_pixel_size(sz);
    });

    // Decrease cell size
    new QShortcut(QKeySequence("-"), this, [this]() {
        int sz = canvas->get_pixel_size();
        sz--;
        canvas->set_pixel_size(sz);
    });

    new QShortcut(QKeySequence("x"), this,
                  [this]() { this->data_handler->rotate_f('x'); });
    new QShortcut(QKeySequence("y"), this,
                  [this]() { this->data_handler->rotate_f('y'); });
    new QShortcut(QKeySequence("z"), this,
                  [this]() { this->data_handler->rotate_f('z'); });
    new QShortcut(QKeySequence("u"), this,
                  [this]() { this->data_handler->scale_f('u'); });
    new QShortcut(QKeySequence("d"), this,
                  [this]() { this->data_handler->scale_f('d'); });
    new QShortcut(QKeySequence(Qt::Key_Right), this,
                  [this]() { this->data_handler->move_f('r'); });
    new QShortcut(QKeySequence(Qt::Key_Left), this,
                  [this]() { this->data_handler->move_f('l'); });
    new QShortcut(QKeySequence(Qt::Key_Up), this,
                  [this]() { this->data_handler->move_f('d'); });
    new QShortcut(QKeySequence(Qt::Key_Down), this,
                  [this]() { this->data_handler->move_f('u'); });
    new QShortcut(QKeySequence(Qt::Key_F10), this, [this]() {
        bool d = this->debugger->get_debug();
        if (d) {
            this->debugger->begin_debug();
        }
    });
    new QShortcut(QKeySequence(Qt::Key_A), this,
                  [this]() { this->data_handler->finish_and_draw(); });
}

MainWindow::~MainWindow() { std::cout << "Window out...\n"; }

void MainWindow::on_info()
{
    QMessageBox::information(this, "About",
                             "Awesome Editor created by AniGleb (feat. Owerk)\n"
                             "C++ and Qt6.10.2\n"
                             "Version 0.31-alpha\n"
                             "2026");
}

void MainWindow::on_help()
{
    QMessageBox::information(this, "Help",
                             "Shortcuts:\n"
                             "'F10' - debug step\n"
                             "'+' - increase grid size\n"
                             "'-' - decrease grid size\n"
                             "'a' - finish figure drawing\n"
                             "'x' - rotate figure in x axis\n"
                             "'y' - rotate figure in y axis\n"
                             "'z' - rotate figure in z axis\n"
                             "'u' - scale figure up\n"
                             "'d' - scale figure down\n"
                             "arrows - move figure\n");
}

void MainWindow::on_click_on_pixel(Point px)
{
    this->data_handler->add_point(px);
}

void MainWindow::on_show_figures_dialog()
{
    std::unique_ptr<QDialog> dialog = std::make_unique<QDialog>(this);

    dialog->setWindowTitle("List of figures");
    dialog->resize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog.get());

    QListWidget *list_widget = new QListWidget();
    list_widget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (const auto &fig : this->data_handler->get_figures()) {
        QString item_text =
            QString("%1 %2")
                .arg(Tools::gtype_to_string(fig->get_figure_type()))
                .arg(fig->get_id());
        list_widget->addItem(item_text);
    }

    layout->addWidget(list_widget);

    QPushButton *OK_button = new QPushButton("OK");
    connect(OK_button, &QPushButton::clicked, dialog.get(), &QDialog::accept);
    layout->addWidget(OK_button);

    dialog->setLayout(layout);

    if (dialog->exec() == QDialog::Accepted) {
        int row = list_widget->currentRow();
        if (row >= 0 && row < this->data_handler->get_figures().size()) {
            this->data_handler->set_current_active_idx(row);
        }
    }
}

void MainWindow::add_toolbar_algorithm_button(QToolBar *toolbar, QString &&text,
                                              QString &&tool_tip,
                                              std::vector<GType> data)
{
    QToolButton *button = new QToolButton(toolbar);
    button->setText(text);
    button->setToolTip(tool_tip);
    button->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *menu = new QMenu(button);

    for (auto type : data) {
        menu->addAction(QString::fromStdString(Tools::gtype_to_string(type)))
            ->setData(static_cast<int>(type));
    }

    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

    toolbar->addWidget(button);
    this->connect(menu, &QMenu::triggered, this, [this](QAction *act) {
        GType type = static_cast<GType>(act->data().toInt());
        this->data_handler->set_figure(Tools::get_figure_object_by_type(type));
    });
}
} // namespace AlgorithmicEditor