#include "graph.h"
#include "util.h"

#include <QDebug>

Vertex::Vertex(const Point &point, int n):
    n(n)
{
    pos_x = point.x();
    pos_y = point.y();
}


void Vertex::boundVertex(Vertex *vertex){
    adjacent_vertices.insert(vertex);
    vertex->adjacent_vertices.insert(this);
}


const std::set<Vertex*>& Vertex::getAdjacent() const{
    return adjacent_vertices;
}


double Vertex::x() const{
    return pos_x;
}


double Vertex::y() const{
    return pos_y;
}


int Vertex::index() const{
    return n;
}


Point Vertex::point(){
    return Point(x(), y());
}


Graph::Graph(const std::vector<Point> &points)
{
    for(unsigned int i = 0; i< points.size(); i++){
        Vertex *vertex = new Vertex(points[i], int(i));
        vertices.push_back(vertex);
    }
}


const Vertex& Graph::operator[](unsigned int i) const{
    return *(vertices[i]);
}


unsigned int Graph::verticesCount() const{
    return vertices.size();
}


void Graph::setPolygons(const std::vector<std::vector<unsigned int> > &polygons){
    for(auto vector: polygons){
        this->polygons.push_back(vector);
    }
}


void Graph::installEdgesFromPolygons(){
    for(auto vector: polygons){
        for(unsigned int i = 0; i < vector.size()-1; i++){
            vertices[vector[i]]->boundVertex(vertices[vector[i+1]]);
        }
        vertices[vector.back()]->boundVertex(vertices[vector[0]]);
    }
}


void Graph::setOuterTriangleVertices(){
    double min_x = vertices[0]->x();
    double min_y = vertices[1]->y();
    for(unsigned int i = 1; i < vertices.size(); i++){
        if(min_x > vertices[i]->x()) min_x = vertices[i]->x();
        if(min_y > vertices[i]->y()) min_y = vertices[i]->y();
    }

    // move out origin point by unit from each side
    Vertex *origin = new Vertex(Point(min_x - 1., min_y - 1.), int(vertices.size()));

    double farthest_x = min_x,
           farthest_y = min_y;
    double longest_distance = 0;

    for(auto vertex: vertices){
        double distance = distanceBetweenVertices(origin, vertex);
        if (distance > longest_distance){
            farthest_x = vertex->x();
            farthest_y = vertex->y();
            longest_distance = distance;
        }
    }

    Point hypotenuse_point(farthest_x, farthest_y);
    Point height_vector = hypotenuse_point - origin->point();
    double height = sqrt(height_vector.x()*height_vector.x() + height_vector.y()*height_vector.y());
    height_vector = Point(height_vector.x()/height, height_vector.y()/height); // normilize vector;

    // move out hypotenuse_point
    hypotenuse_point = hypotenuse_point + height_vector;

    Point directional_vector = getNormalVector(hypotenuse_point - origin->point());

    Point xAxisPoint = getXaxisPoint(directional_vector, hypotenuse_point, origin->point());
    Point yAxisPoint = getYaxisPoint(directional_vector, hypotenuse_point, origin->point());

    Vertex *xAxisVertex = new Vertex(xAxisPoint, origin->index()+1);
    Vertex *yAxisVertex = new Vertex(yAxisPoint, origin->index()+2);

    vertices.push_back(origin);
    vertices.push_back(xAxisVertex);
    vertices.push_back(yAxisVertex);

    origin->boundVertex(xAxisVertex);
    xAxisVertex->boundVertex(yAxisVertex);
    yAxisVertex->boundVertex(origin);

    qDebug() << min_x << min_y;
}














