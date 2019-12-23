#include "point.h"

Point::Point(double x, double y)
{
    pos_x = x;
    pos_y = y;
}


double Point::x() const{
    return pos_x;
}


double Point::y() const{
    return pos_y;
}


Point Point::operator-(const Point &subtrahend){
    return Point(x() - subtrahend.x(), y() - subtrahend.y());
}


Point Point::operator+(const Point &term){
    return Point(x() + term.x(), y() + term.y());
}
