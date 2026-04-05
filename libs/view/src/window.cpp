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

    this->connect(this->canvas.get(), &Canvas::click_on_pixel, this,
                  &MainWindow::on_click_on_pixel);

    scroll_area->setWidget(this->canvas.get());

    // Manage working area inside frame
    QVBoxLayout *frame_layout = new QVBoxLayout(border_frame);
    frame_layout->setContentsMargins(Config::CELL_SIZE, Config::CELL_SIZE,
                                     Config::CELL_SIZE, Config::CELL_SIZE);
    frame_layout->addWidget(scroll_area);

    main_view->addWidget(border_frame);

    this->debugger = std::make_unique<Debugger>(this->canvas.get());
    this->data_handler = std::make_unique<DataHandler>(this->debugger.get());

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

    QAction *grid_size_setting = new QAction("Cell size", this);
    this->connect(grid_size_setting, &QAction::triggered, this,
                  &MainWindow::on_size_update);

    QAction *grid_show_setting = new QAction("Show grid", this);
    grid_show_setting->setCheckable(true);
    grid_show_setting->setChecked(true);
    grid_show_setting->setToolTip("Enable/disable grid");
    this->connect(grid_show_setting, &QAction::toggled, this->canvas.get(),
                  &Canvas::on_grid_show);

    file_menu->addAction(program_help);
    file_menu->addAction(program_about);

    settings_menu->addAction(active_figure_setting);
    settings_menu->addAction(grid_size_setting);
    settings_menu->addAction(grid_show_setting);

    // Init toolbar
    QToolBar *tool_bar = this->addToolBar("Tools");
    tool_bar->addSeparator();

    // set misc buttons
    QToolButton *clear_canvas_btn = new QToolButton(tool_bar);
    clear_canvas_btn->setText("Clear");
    clear_canvas_btn->setToolTip(
        "Clears workspace by filling it with white color");
    clear_canvas_btn->setMaximumWidth(4 * Config::CELL_SIZE);

    QCheckBox *debug_checkbox = new QCheckBox("Debug", tool_bar);
    debug_checkbox->setToolTip("Debug mode");
    debug_checkbox->setChecked(false);

    QToolButton *debug_step_btn = new QToolButton(tool_bar);
    debug_step_btn->setText(">");
    debug_step_btn->setEnabled(false);

    QToolButton *debug_stop_btn = new QToolButton(tool_bar);
    debug_stop_btn->setText("x");
    debug_stop_btn->setEnabled(false);

    // button with popup, goddamn
    QToolButton *frl_btn = new QToolButton(tool_bar);
    frl_btn->setText("FRLine");
    frl_btn->setToolTip("Various first rank line drawing algorithms");
    frl_btn->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *frl_menu = new QMenu(frl_btn);
    frl_menu->addAction("CDA")->setData(static_cast<int>(GType::CDA));
    frl_menu->addAction("Bresenham")
        ->setData(static_cast<int>(GType::Bresenham));
    frl_menu->addAction("Wu")->setData(static_cast<int>(GType::Wu));

    frl_btn->setMenu(frl_menu);
    frl_btn->setPopupMode(QToolButton::InstantPopup);
    //

    // button with popup, goddamn x2
    QToolButton *srl_btn = new QToolButton(tool_bar);
    srl_btn->setText("SRLine");
    srl_btn->setToolTip("Various second rank line drawing algorithms");
    srl_btn->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *srl_menu = new QMenu(srl_btn);
    srl_menu->addAction("Circle")->setData(static_cast<int>(GType::Circle));
    srl_menu->addAction("Elipsis")->setData(static_cast<int>(GType::Ellipse));
    srl_menu->addAction("Parabola")->setData(static_cast<int>(GType::Parabola));
    srl_menu->addAction("Hyperbola")
        ->setData(static_cast<int>(GType::Hyperbola));

    srl_btn->setMenu(srl_menu);
    srl_btn->setPopupMode(QToolButton::InstantPopup);
    //

    // button with popup, goddamn x3
    QToolButton *interpolation_btn = new QToolButton(tool_bar);
    interpolation_btn->setText("ARLine");
    interpolation_btn->setToolTip("Various interpolation algorithms");
    interpolation_btn->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *interplation_menu = new QMenu(interpolation_btn);
    interplation_menu->addAction("Hermite")->setData(
        static_cast<int>(GType::Hermite));
    interplation_menu->addAction("Bezier")->setData(
        static_cast<int>(GType::Bezier));
    interplation_menu->addAction("BSpline")->setData(
        static_cast<int>(GType::BSpline));

    interpolation_btn->setMenu(interplation_menu);
    interpolation_btn->setPopupMode(QToolButton::InstantPopup);
    //

    // button with popup, goddamn x4
    QToolButton *btn_3D = new QToolButton(tool_bar);
    btn_3D->setText("3D");
    btn_3D->setToolTip("3D figures");
    btn_3D->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *menu_3D = new QMenu(btn_3D);
    menu_3D->addAction("Cube")->setData(static_cast<int>(GType::Cube));
    menu_3D->addAction("Tetrahedron")
        ->setData(static_cast<int>(GType::Tetrahedron));

    btn_3D->setMenu(menu_3D);
    btn_3D->setPopupMode(QToolButton::InstantPopup);
    //

    // button with popup, goddamn x5
    QToolButton *btn_polygon = new QToolButton(tool_bar);
    btn_polygon->setText("Polygon");
    btn_polygon->setToolTip("Various polygon types");
    btn_polygon->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *menu_polygon = new QMenu(btn_polygon);
    menu_polygon->addAction("Polygon")->setData(
        static_cast<int>(GType::Polygon));
    menu_polygon->addAction("Convex polygon")
        ->setData(static_cast<int>(GType::ConvexPolygon));

    btn_polygon->setMenu(menu_polygon);
    btn_polygon->setPopupMode(QToolButton::InstantPopup);
    //

    // button with popup, goddamn x7
    QToolButton *btn_polygonalisation = new QToolButton(tool_bar);
    btn_polygonalisation->setText("PolyFill");
    btn_polygonalisation->setToolTip("Delone and Voronoi algorithms");
    btn_polygonalisation->setMaximumWidth(4 * Config::CELL_SIZE);

    QMenu *menu_polygonalisation = new QMenu(btn_polygonalisation);

    menu_polygonalisation->addAction("Delone")->setData(
        static_cast<int>(GType::Delaunay));
    menu_polygonalisation->addAction("Voronoi")->setData(
        static_cast<int>(GType::Voronoi));

    btn_polygonalisation->setMenu(menu_polygonalisation);
    btn_polygonalisation->setPopupMode(QToolButton::InstantPopup);
    //

    tool_bar->addWidget(frl_btn);
    this->connect(frl_menu, &QMenu::triggered, this, [this](QAction *act) {
        GType type = static_cast<GType>(act->data().toInt());
        switch (type) {
        case GType::CDA: {
            std::unique_ptr<CDAFigure> figure = std::make_unique<CDAFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Bresenham: {
            std::unique_ptr<BresenhamFigure> figure =
                std::make_unique<BresenhamFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Wu: {
            std::unique_ptr<WuFigure> figure = std::make_unique<WuFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        default: {
            break;
        }
        }
    });

    tool_bar->addWidget(srl_btn);
    this->connect(srl_menu, &QMenu::triggered, this, [this](QAction *act) {
        GType type = static_cast<GType>(act->data().toInt());
        switch (type) {
        case GType::Circle: {
            std::unique_ptr<CircleFigure> figure =
                std::make_unique<CircleFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Ellipse: {
            std::unique_ptr<EllipseFigure> figure =
                std::make_unique<EllipseFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Parabola: {
            std::unique_ptr<ParabolaFigure> figure =
                std::make_unique<ParabolaFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Hyperbola: {
            std::unique_ptr<HyperbolaFigure> figure =
                std::make_unique<HyperbolaFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        default: {
            break;
        }
        }
    });

    tool_bar->addWidget(interpolation_btn);
    this->connect(interplation_menu, &QMenu::triggered, this,
                  [this](QAction *act) {
                      GType type = static_cast<GType>(act->data().toInt());
                      switch (type) {
                      case GType::Hermite: {
                          std::unique_ptr<HermiteFigure> figure =
                              std::make_unique<HermiteFigure>();
                          this->data_handler->set_figure(std::move(figure));
                          break;
                      }
                      case GType::Bezier: {
                          std::unique_ptr<BezierFigure> figure =
                              std::make_unique<BezierFigure>();
                          this->data_handler->set_figure(std::move(figure));
                          break;
                      }
                      case GType::BSpline: {
                          std::unique_ptr<BSplineFigure> figure =
                              std::make_unique<BSplineFigure>();
                          this->data_handler->set_figure(std::move(figure));
                          break;
                      }
                      default: {
                          break;
                      }
                      }
                  });

    tool_bar->addWidget(btn_3D);
    this->connect(menu_3D, &QMenu::triggered, this, [this](QAction *act) {
        GType type = static_cast<GType>(act->data().toInt());
        switch (type) {
        case GType::Cube: {
            std::unique_ptr<CubeFigure> figure = std::make_unique<CubeFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::Tetrahedron: {
            std::unique_ptr<TetrahedronFigure> figure =
                std::make_unique<TetrahedronFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        default: {
            break;
        }
        }
    });

    tool_bar->addWidget(btn_polygon);
    this->connect(menu_polygon, &QMenu::triggered, this, [this](QAction *act) {
        GType type = static_cast<GType>(act->data().toInt());
        switch (type) {
        case GType::Polygon: {
            std::unique_ptr<PolygonFigure> figure =
                std::make_unique<PolygonFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }
        case GType::ConvexPolygon: {
            std::unique_ptr<ConvexPolygonFigure> figure =
                std::make_unique<ConvexPolygonFigure>();
            this->data_handler->set_figure(std::move(figure));
            break;
        }

        default: {
            break;
        }
        }
    });

    tool_bar->addWidget(btn_polygonalisation);
    this->connect(menu_polygonalisation, &QMenu::triggered, this,
                  [this](QAction *act) {
                      GType type = static_cast<GType>(act->data().toInt());
                      switch (type) {
                      case GType::Delaunay: {
                          std::unique_ptr<DelaunayFigure> figure =
                              std::make_unique<DelaunayFigure>();
                          this->data_handler->set_figure(std::move(figure));
                          break;
                      }
                      case GType::Voronoi: {
                          std::unique_ptr<VoronoiFigure> figure =
                              std::make_unique<VoronoiFigure>();
                          this->data_handler->set_figure(std::move(figure));
                          break;
                      }
                      default: {
                          break;
                      }
                      }
                  });

    // misc buttons
    tool_bar->addWidget(clear_canvas_btn);
    this->connect(clear_canvas_btn, &QToolButton::clicked, this, [this]() {
        this->canvas->on_clear();
        bool d = this->debugger->get_debug();
        this->debugger->reset();
        this->data_handler->reset();
        this->debugger->set_debug(d);
    });

    tool_bar->addWidget(debug_checkbox);
    this->connect(debug_checkbox, &QCheckBox::toggled, this,
                  [this](bool checked) {
                      if (checked) {
                          qDebug() << "Debug ON";
                          this->debugger->reset();
                          this->debugger->set_debug(true);
                      } else {
                          qDebug() << "Debug OFF";
                          this->debugger->set_debug(false);
                          this->debugger->begin_debug();
                      }
                  });

    tool_bar->addWidget(debug_step_btn);
    this->connect(debug_checkbox, &QCheckBox::toggled, debug_step_btn,
                  &QToolButton::setEnabled);
    this->connect(debug_step_btn, &QToolButton::clicked, this,
                  [this]() { this->debugger->begin_debug(); });

    tool_bar->addWidget(debug_stop_btn);
    this->connect(debug_checkbox, &QCheckBox::toggled, debug_stop_btn,
                  &QToolButton::setEnabled);
    this->connect(debug_stop_btn, &QToolButton::clicked, this, [this]() {
        bool d = this->debugger->get_debug();
        this->debugger->set_debug(false);
        this->debugger->begin_debug();
        this->debugger->set_debug(d);
    });

    // setup shortcuts

    // increase cell size
    new QShortcut(QKeySequence("+"), this, [this]() {
        int sz = this->canvas->get_pixel_size();
        sz++;
        canvas->set_pixel_size(sz);
    });

    // decrease cell size
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
                             "Awesome Editor created by Owerk and Glentas (and "
                             "Qwen)\n"
                             "C++ and Qt6.10.2\n"
                             "Version 2.3\n"
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

// connecting user clicks, in order to calculate when to launch algorithms
void MainWindow::on_click_on_pixel(Point px)
{
    this->data_handler->add_point(px);
}

void MainWindow::on_size_update()
{
    bool ok;
    QString *max_size = new QString();
    max_size->assign("Size (1 - " + std::to_string(2 * Config::CELL_SIZE) +
                     "):");

    int n = QInputDialog::getInt(this, "Size of a grid cell", *max_size, 1, 1,
                                 2 * Config::CELL_SIZE, 1, &ok);
    if (ok) {
        this->canvas->set_pixel_size(n);
    }
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
        QString item_text = QString("%1 %2")
                                .arg(gtype_to_string(fig->get_figure_type()))
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
            this->data_handler->set_previous_active_idx(row);
        }
    }
}
} // namespace AlgorithmicEditor