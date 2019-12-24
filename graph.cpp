#include "graph.h"
#include "util.h"

#include <QDebug>

Vertex::Vertex(const Point &point, int n):
    n(n)
{
    pos_x = point.x();
    pos_y = point.y();
}


Vertex::Vertex(const Vertex *vertex):
    n(vertex->n)
{
    pos_x = vertex->pos_x;
    pos_y = vertex->pos_y;
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


Point Vertex::point() const{
    return Point(x(), y());
}


Graph::Graph(const std::vector<Point> &points)
{
    for(unsigned int i = 0; i< points.size(); i++){
        Vertex *vertex = new Vertex(points[i], int(i));
        vertices.push_back(vertex);
    }
}


Graph::Graph(const std::vector<Vertex*> &vertices){
    for(unsigned int i = 0; i < vertices.size(); i++){
        Vertex *vertex = new Vertex(*(vertices[i]));
        this->vertices.push_back(vertex);
    }
}


const Vertex& Graph::operator[](unsigned int i) const{
    return *(vertices[i]);
}


Vertex* Graph::getVertex(unsigned int n){
    for(auto vertex: vertices){
        if(unsigned(vertex->index()) == n)
            return vertex;
    }
    return nullptr;
}


unsigned int Graph::verticesCount() const{
    return vertices.size();
}


void Graph::setPolygons(const std::vector<Polygon> &polygons){
    for(auto polygon: polygons){
        polygon.n = this->polygons.size();
        this->polygons.push_back(polygon);
    }
}


void Graph::addPolygon(const Polygon &polygon){
    Polygon to_emplace = polygon;
    to_emplace.n = polygons.size();
    polygons.push_back(to_emplace);
}


void Graph::installEdgesFromPolygons(){
    for(auto polygon: polygons){
        for(unsigned int i = 0; i < polygon.points.size()-1; i++){
            vertices[polygon.points[i]]->boundVertex(vertices[polygon.points[i+1]]);
        }
        vertices[polygon.points.back()]->boundVertex(vertices[polygon.points[0]]);
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
    Vertex *origin = new Vertex(Point(min_x - 0.1, min_y - 0.1), int(vertices.size()));

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
    height_vector = Point(height_vector.x()/height*0.1, height_vector.y()/height*0.1); // normilize vector;

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

    qDebug() << min_x << min_y;
}


void Graph::triangulateOuterTriangle(const std::vector<Vertex*> &hull){
    std::vector<Polygon> outer_polygons;

    auto origin = *(vertices.end()-3);
    Vertex *nearest = hull[0];

    double min_distance = distanceBetweenVertices(origin, nearest);

    int start_n = 0;
    for(unsigned int i = 1; i < hull.size(); i++){
        double distance = distanceBetweenVertices(origin, hull[i]);
        if(min_distance > distance){
            min_distance = distance;
            nearest = hull[i];
            start_n = int(i);
        }
    }

    double nearest_angle = vectorAngle(nearest->point()-origin->point());
    double prev_delta_angle = 0.;
    double curr_delta_angle = 0.;

    Vertex *adjacent_vertex = nullptr;

    //// Find tangent from top-left vertex to the left
    int n = start_n;
    do{
        prev_delta_angle = curr_delta_angle;

        if(--n < 0) n = int(hull.size())-1;
        adjacent_vertex = getVertex(hull[unsigned(n)]->index());

        curr_delta_angle = fabs(nearest_angle - vectorAngle(adjacent_vertex->point()-origin->point()));
    }while(curr_delta_angle >= prev_delta_angle);
    int left_point_index = n + 1;

    prev_delta_angle = 0;
    curr_delta_angle = 0;
    adjacent_vertex = nullptr;

    //// Find tangent from top-left vertex to the right
    n = start_n;
    do{
        prev_delta_angle = curr_delta_angle;

        if(++n >= int(hull.size())) n = 0;
        adjacent_vertex = getVertex(hull[unsigned(n)]->index());

        curr_delta_angle = fabs(nearest_angle - vectorAngle(adjacent_vertex->point()-origin->point()));
    }while(curr_delta_angle >= prev_delta_angle);
    int top_point_index = n - 1;

    for(int i = left_point_index; i != top_point_index; i++){
        if(i >= hull.size()) i = 0;
        Polygon polygon;
        polygon.points.push_back(unsigned(origin->index()));
        polygon.points.push_back(unsigned(hull[i]->index()));
        polygon.points.push_back(unsigned(hull[((i+1) >= hull.size()) ? 0 : i+1]->index()));
        outer_polygons.push_back(polygon);
    }

    Polygon polygon;
    polygon.points.push_back(origin->index());
    polygon.points.push_back(hull[top_point_index]->index());
    polygon.points.push_back((*(vertices.end()-2))->index());
    outer_polygons.push_back(polygon);

    //// Set top-right as origin
    origin = *(vertices.end()-2);
    nearest = hull[0];
    min_distance = distanceBetweenVertices(origin, nearest);

    start_n = 0;
    for(unsigned int i = 1; i < hull.size(); i++){
        double distance = distanceBetweenVertices(origin, hull[i]);
        if(min_distance > distance){
            min_distance = distance;
            nearest = hull[i];
            start_n = int(i);
        }
    }

    nearest_angle = vectorAngle(nearest->point()-origin->point());
    prev_delta_angle = 0;
    curr_delta_angle = 0;
    adjacent_vertex = nullptr;

    //// Find polygons from top right to the down
    n = start_n;
    do{
        prev_delta_angle = curr_delta_angle;

        if(++n >= int(hull.size())) n = 0;
        adjacent_vertex = getVertex(hull[unsigned(n)]->index());

        curr_delta_angle = fabs(nearest_angle - vectorAngle(adjacent_vertex->point()-origin->point()));
    }while(curr_delta_angle >= prev_delta_angle);
    int right_point_index = n - 1;

    for(int i = top_point_index; i != right_point_index; i++){
        if(i >= hull.size()) i = 0;
        Polygon polygon;
        polygon.points.push_back(unsigned(origin->index()));
        polygon.points.push_back(unsigned(hull[i]->index()));
        polygon.points.push_back(unsigned(hull[((i+1) >= hull.size()) ? 0 : i+1]->index()));
        outer_polygons.push_back(polygon);
    }

    polygon.points.clear();
    polygon.points.push_back(origin->index());
    polygon.points.push_back(hull[right_point_index]->index());
    polygon.points.push_back((*(vertices.end()-1))->index());
    outer_polygons.push_back(polygon);

    //// Set bottom as origin;
    origin = *(vertices.end()-1);
    for(int i = right_point_index; i != left_point_index; i++){
        Polygon polygon;
        polygon.points.push_back(unsigned(origin->index()));
        polygon.points.push_back(unsigned(hull[i]->index()));
        polygon.points.push_back(unsigned(hull[((i+1) >= hull.size()) ? 0 : i+1]->index()));
        outer_polygons.push_back(polygon);
    }

    polygon.points.clear();
    polygon.points.push_back(origin->index());
    polygon.points.push_back(hull[left_point_index]->index());
    polygon.points.push_back((*(vertices.end()-3))->index());
    outer_polygons.push_back(polygon);

    for(unsigned int i = 0; i < outer_polygons.size(); i++)
        outer_polygons[i].n = i+polygons.size();

    polygons.insert(polygons.end(), outer_polygons.begin(), outer_polygons.end());
}


std::vector<Vertex*> Graph::grahamScan(){
    double min_y = vertices[0]->y();
    int vertex_n = vertices[0]->index();

    std::vector<Vertex*> hull_bank;

    for(auto item: vertices){
        if(min_y > item->y()){
            min_y = item->y();
            vertex_n = item->index();
        }
        hull_bank.push_back(new Vertex(item));
    }

    Vertex *origin;

    for(unsigned int i = 0; i < hull_bank.size(); i++){
        if(hull_bank[i]->index() == vertex_n){
            origin = hull_bank[i];
            hull_bank.erase(hull_bank.begin()+int(i));
            break;
        }
    }

    sortVerticesOverVertex(hull_bank, origin);
    hull_bank.insert(hull_bank.begin(), origin);

    ////Graham algorithm
    std::vector<Vertex*> hull;
    hull.push_back(hull_bank[0]);
    hull.push_back(hull_bank[1]);

    auto m = hull_bank.size();
    for(unsigned int i = 2; i < m; i++){
        while(!counterClockWise(*(hull.end()-2), *(hull.end()-1), hull_bank[i])){
            hull.pop_back();
        }
        hull.push_back(hull_bank[i]);
    }

    return hull;
}











