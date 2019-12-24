#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include "point.h"

class Vertex{
public:
    Vertex(const Point &point, int n);
    Vertex(const Vertex *point);
    ~Vertex();
    void unboundVertex(Vertex *vertex);
    void boundVertex(Vertex *vertex);
    const std::set<Vertex*>& getAdjacent() const;

    double x() const;
    double y() const;
    int index() const;

    Point point() const;

private:
    double pos_x;
    double pos_y;
    int n;
    std::set<Vertex*> adjacent_vertices;
};


struct Polygon{
    std::vector<unsigned int> points;
    unsigned int n;
};


class Graph
{
public:
    Graph(const std::vector<Point> &points);
    Graph(const std::vector<Vertex*> &vertices);

    const Vertex& operator[](unsigned int i) const;
    unsigned int verticesCount() const;

    void setPolygons(const std::vector<Polygon> &polygons);
    void addPolygon(const Polygon &polygon);
    void installEdgesFromPolygons();

    void setOuterTriangleVertices();
    void triangulateOuterTriangle(const std::vector<Vertex*> &hull);

    std::vector<Vertex*> grahamScan();

private:
    Vertex* getVertex(unsigned int n);

    std::vector<Polygon> polygons;
    std::vector<Vertex*> vertices;
};

#endif // GRAPH_H
