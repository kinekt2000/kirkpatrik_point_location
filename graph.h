#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include "point.h"

class Vertex{
public:
    Vertex(const Point &point, int n);
    ~Vertex();
    void unboundVertex(Vertex *vertex);
    void boundVertex(Vertex *vertex);
    const std::set<Vertex*>& getAdjacent() const;

    double x() const;
    double y() const;
    int index() const;

    Point point();

private:
    double pos_x;
    double pos_y;
    int n;
    std::set<Vertex*> adjacent_vertices;
};

class Graph
{
public:
    Graph(const std::vector<Point> &points);
    const Vertex& operator[](unsigned int i) const;
    unsigned int verticesCount() const;
    void setPolygons(const std::vector<std::vector<unsigned int>> &polygons);
    void installEdgesFromPolygons();

    void setOuterTriangleVertices();

private:
    std::vector<std::vector<unsigned int>> polygons;
    std::vector<Vertex*> vertices;
};

#endif // GRAPH_H
