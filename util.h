#ifndef UTIL_H
#define UTIL_H

#include <cmath>

#include <graph.h>
#include <point.h>

double distanceBetweenVertices(const Vertex *a, const Vertex *b){
    double x_distance = fabs(a->x() - b->x());
    double y_distance = fabs(a->y() - b->y());

    return sqrt(x_distance*x_distance + y_distance*y_distance);
}

Point getNormalVector(const Point &vector1){
    double x_2 = 1., y_2;
    y_2 = -vector1.x()/vector1.y();
    Point vector2(x_2, y_2);
    return vector2;
}


Point getXaxisPoint(const Point &directional_vector, const Point &vector_point, const Point &origin_point){
    double x_0 = directional_vector.x()/directional_vector.y() * (origin_point.y()-vector_point.y()) + vector_point.x();
    Point Xaxis_point(x_0, origin_point.y());
    return Xaxis_point;
}


Point getYaxisPoint(const Point &directional_vector, const Point &vector_point, const Point &origin_point){
    double y_0 = directional_vector.y()/directional_vector.x() * (origin_point.x() - vector_point.x()) + vector_point.y();
    Point Yaxis_point(origin_point.x(), y_0);
    return Yaxis_point;
}

#endif // UTIL_H
