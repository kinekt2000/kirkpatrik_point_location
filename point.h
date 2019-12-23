#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point(double x, double y);
    double x() const;
    double y() const;

    Point operator-(const Point &subtrahend);
    Point operator+(const Point &term);

private:
    double pos_x;
    double pos_y;
};

#endif // POINT_H
