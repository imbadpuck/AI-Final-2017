#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED
#include<TPoint.h>
const int MAX_POINT = 17;

class Polygon{
public:
    int numberOfPoints; ///so diem cua da giac
    TPoint points[MAX_POINT]; ///toa do cac diem cua da giac theo chieu kim dong ho
    double angles[MAX_POINT]; ///cac goc cua da giac

    void changePointsToDefaultPositions(); ///dua toan bo cac diem ve vi tri tren cung va trai cung
    void calculateAllAngles(); ///tinh toan bo cac goc cua da giac theo chieu kim dong ho (tinh theo radian)
    void rotatePolygon(); ///xoay da giac theo 1 goc 90?
};

void Polygon::changePointsToDefaultPositions(){

}

void Polygon::calculateAllAngles(){
    for (int j = 1; j < numberOfPoints-1; j++)
        angles[j-1] = angle(points[j-1], points[j], points[j+1]);

    angles[numberOfPoints-2] = angle(points[numberOfPoints-2], points[numberOfPoints-1], points[0]);
    angles[numberOfPoints-1] = angle(points[numberOfPoints-1], points[0], points[1]);
}

void Polygon::rotatePolygon(){

}

#endif // POLYGON_H_INCLUDED
