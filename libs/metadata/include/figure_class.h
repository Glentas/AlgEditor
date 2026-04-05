#pragma once
#include "data_definitions.h"

#include <memory>
#include <vector>

namespace AlgorithmicEditor
{

class Figure
{
  public:
    Figure() = delete;
    Figure(GType type);
    virtual ~Figure() = default;

    void set_id(uint16_t id);
    void set_figure_type(GType figure_type);
    void set_points(const std::vector<Point> &points);

    uint16_t get_id() const;
    GType get_figure_type() const;
    const std::vector<Point> &get_points() const;
    const std::vector<Point> &get_connected_points() const;

    void clear();

    void regenerate_cache() const;
    virtual bool add_and_validate_point(const Point &pt) = 0;
    virtual std::unique_ptr<Figure> clone() = 0;

  protected:
    void add_point(const Point &pt);
    virtual void
    connect_figure_points(std::vector<Point> &cache_points) const = 0;

  private:
    uint16_t id;
    GType figure_type;
    std::vector<Point> points;
    mutable std::vector<Point> connected_points_cache;
};

class CDAFigure : public Figure
{
  public:
    CDAFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class BresenhamFigure : public Figure
{
  public:
    BresenhamFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class WuFigure : public Figure
{
  public:
    WuFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class CircleFigure : public Figure
{
  public:
    CircleFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class EllipseFigure : public Figure
{
  public:
    EllipseFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class ParabolaFigure : public Figure
{
  public:
    ParabolaFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class HyperbolaFigure : public Figure
{
  public:
    HyperbolaFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class HermiteFigure : public Figure
{
  public:
    HermiteFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class BezierFigure : public Figure
{
  public:
    BezierFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class BSplineFigure : public Figure
{
  public:
    BSplineFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class CubeFigure : public Figure
{
  public:
    CubeFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class TetrahedronFigure : public Figure
{
  public:
    TetrahedronFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class PolygonFigure : public Figure
{
  public:
    PolygonFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class ConvexPolygonFigure : public Figure
{
  public:
    ConvexPolygonFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class DelaunayFigure : public Figure
{
  public:
    DelaunayFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

class VoronoiFigure : public Figure
{
  public:
    VoronoiFigure();
    virtual bool add_and_validate_point(const Point &pt) override;
    virtual std::unique_ptr<Figure> clone() override;

  protected:
    void connect_figure_points(std::vector<Point> &cache_points) const override;
};

} // namespace AlgorithmicEditor