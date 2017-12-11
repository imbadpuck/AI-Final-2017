#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED
#include<TPoint.h>

const int MAX_POINT = 17;
const double eps = double (1e-6);

class Polygon{
public:
    int numberOfPoints; ///so diem cua da giac
    TPoint points[MAX_POINT]; ///toa do cac diem cua da giac theo chieu kim dong ho
    double angles[MAX_POINT]; ///cac goc cua da giac
    int euclideanDistance[MAX_POINT]; ///binh phuong do dai cac doan thang cua da giac (kc Euclid)
    int manhattanDistance[MAX_POINT]; ///khoang cach Manhattan giua cac doan thang cua da giac
    int minX, minY, maxX, maxY;

    void init(); ///initialize
    void changePointsToDefaultPositions(); ///dua toan bo cac diem ve vi tri tren cung va trai cung
    void calculateAllSegments(); ///tinh toan bo binh phuong do dai cac doan thang cua da giac
    void calculateAllAngles(); ///tinh toan bo cac goc cua da giac theo chieu kim dong ho (tinh theo radian)
    void rotatePolygon(); ///xoay da giac theo 1 goc 90?
    void flipPolygon(); ///lat hinh doi xung qua truc tung
    vector< pair<int, int> > whereToPut(Polygon a); ///cac diem co the dung de dat 2 hinh canh nhau

    void assignValue(Polygon *a); ///gan toa do cac diem cua mieng hien tai cho mieng a
};

inline void Polygon::init(){
    minX = minY = int(1e9);
    maxX = maxY = 0;
    for (int i = 0; i < numberOfPoints; i++){}

    calculateAllAngles();
    calculateAllSegments();
}

inline void Polygon::assignValue(Polygon *a){
    (*a).numberOfPoints = numberOfPoints;
    for (int i = 0; i < numberOfPoints; i++){
        (*a).points[i] = points[i];
    }
}

inline void Polygon::changePointsToDefaultPositions(){

}

inline void Polygon::calculateAllAngles(){
    for (int j = 1; j < numberOfPoints-1; j++)
        angles[j-1] = angle(points[j-1], points[j], points[j+1]);

    angles[numberOfPoints-2] = angle(points[numberOfPoints-2], points[numberOfPoints-1], points[0]);
    angles[numberOfPoints-1] = angle(points[numberOfPoints-1], points[0], points[1]);
}

inline void Polygon::calculateAllSegments(){
    for (int i = 0; i < numberOfPoints-1; i++){
        euclideanDistance[i] = dis(points[i], points[i+1]);
        manhattanDistance[i] = mandis(points[i], points[i+1]);
    }

    euclideanDistance[numberOfPoints-1] = dis(points[numberOfPoints-1], points[0]);
    manhattanDistance[numberOfPoints-1] = mandis(points[numberOfPoints-1], points[0]);
}

inline void Polygon::rotatePolygon(){
    for (int i = 0; i < numberOfPoints; i++){
        int newX = points[i].y;
        int newY = -points[i].x;
        points[i] = TPoint(newX, newY);
    }
}

inline void Polygon::flipPolygon(){
    for (int i = 0; i < numberOfPoints; i++){
        int newX = -points[i].x;
        int newY = points[i].y;
        points[i] = TPoint(newX, newY);
    }
}

inline vector< pair<int, int> > Polygon::whereToPut(Polygon a){
    vector< pair<int, int> > ans;

    ///check 2 goc co tong = 360 || 180 hay khong
    int numberOfAngles = numberOfPoints;
    for (int i = 0; i < numberOfAngles; i++){
        for (int j = 0; j < a.numberOfPoints; j++){
            double sumAngles = angles[i] + a.angles[j];
            if ((sumAngles <= 360.0+eps && sumAngles >= 360.0-eps) || (sumAngles <= 180.0+eps && sumAngles >= 180.0-eps))
                ans.push_back(make_pair(i,j));
        }
    }

    ///check 2 doan thang co cung do dai euclid & do dai manhattan
    int numberOfSegments = numberOfPoints-1;
    for (int i = 0; i < numberOfSegments; i++){
        for (int j = 0; j < a.numberOfPoints-1; j++){
            if (euclideanDistance[i] == a.euclideanDistance[j] && manhattanDistance[i] == a.manhattanDistance[j])
                ans.push_back(make_pair(i,j));
        }
    }

    sort(ans.begin(), ans.end());
    ans.erase( unique( ans.begin(), ans.end() ), ans.end() );
    //cout << ans.size() << ' ';

    return ans;
}

#endif // POLYGON_H_INCLUDED
