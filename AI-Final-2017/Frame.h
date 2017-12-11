#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include<TPoint.h>
#include<Polygon.h>

const int maxRow = 65+1;
const int maxColumn = 100+1;

class Frame{
public:
    int numOfPoints;
    int minX, maxX, minY, maxY;
    int grid[maxRow][maxColumn]; ///mo ta luoi diem y = row, x = column
    short piecesOccupied[maxRow][maxColumn]; ///so mieng da chiem lay diem (x, y)
    ///= -1 diem khong thuoc khung
    ///= 0 diem chua duoc phu boi cac hinh
    ///= 1 diem duoc phu boi bien cua mieng
    ///= 2 diem nam trong cac mieng

    TPoint borders[20]; ///cac diem bien cua khung

    void init();
    bool insideFrame(TPoint p); ///kiem tra xem diem co nam trong khung ko
    bool placePieceIntoFrame(Polygon &po, int stPoint, TPoint p); ///dat mot mieng vao trong khung tai diem p
    void reverseFrameState(Polygon po); ///khoi phuc trang thai cua bang ve trang thai truoc khi dat hinh vao khung

    int howMuchCovered(); ///frame da duoc covered bao nhieu
};

bool Frame::insideFrame(TPoint p){///wrong
    if (p.x < minX || p.y < minY || p.x > maxX || p.y > maxY){
        return false;
    }

    return grid[ p.y ][ p.x ] > -1;
}

void Frame::init(){
    memset(grid, 0xff, sizeof(grid));
    memset(piecesOccupied, 0, sizeof(piecesOccupied));

    maxX = maxY = 0;
    minX = minY = maxColumn;
    for (int i = 0; i < numOfPoints; i++){
        minX = min(minX, borders[i].x);
        maxX = max(maxX, borders[i].x);
        minY = min(minY, borders[i].y);
        maxY = max(maxY, borders[i].y);
        grid[ borders[i].y ][ borders[i].x ] = 1;
        //cout << borders[i].y << ' ' << borders[i].x << ' ' <<  grid[ borders[i].y ][ borders[i].x ] << endl;
    }

    Polygon p;
    p.numberOfPoints = numOfPoints;
    for (int i = 0; i < p.numberOfPoints; i++){
        p.points[i] = TPoint(borders[i].x, borders[i].y);
    }

    p.init();

    for (int i = minX; i <= maxX; i++)
        for (int j = minY; j <= maxY; j++){
            int isInside = p.isPointInsidePolygon(TPoint(i, j));
            if (grid[j][i] == -1){
                grid[j][i] = isInside;
            }
           // else grid[j][i] = 2;
        }
}


bool Frame::placePieceIntoFrame(Polygon &po, int stPoint, TPoint p){
    int stX = p.x;
    int stY = p.y;
    int disX, disY; ///khoang cach Manhattan giua toa do 2 diem trong da giac
    //grid[ stY ][ stX ] = 1;
 //   cerr << stX << ' ' << stY << endl;

    bool isAvailable = true;
    for (int i = stPoint+1; i < po.numberOfPoints; i++){
        disX = po.points[i].x - po.points[i-1].x;
        disY = po.points[i].y - po.points[i-1].y;
        po.points[i-1] = TPoint(stX, stY);
       // cerr << i-1 << ' ' << stX << ' ' << stY << endl;
        stX += disX;
        stY += disY;
        if (!insideFrame(TPoint(stX, stY))){
            isAvailable = false;
         //   cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
        }

        if (grid[ stY ][ stX ] == 2){
            isAvailable = false;
        }
        //grid[ stY ][ stX ] = 1;
    }

    disX = po.points[0].x - po.points[po.numberOfPoints-1].x;
    disY = po.points[0].y - po.points[po.numberOfPoints-1].y;
    po.points[po.numberOfPoints-1] = TPoint(stX, stY);
   // cerr << po.numberOfPoints-1 << ' ' << stX << ' ' << stY << endl;
    stX += disX;
    stY += disY;

    if (!insideFrame(TPoint(stX, stY))){
        isAvailable = false;
        //cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
    }

    if (grid[ stY ][ stX ] == 2){
        isAvailable = false;
    }
    //grid[ stY ][ stX ] = 1;

    for (int i = 1; i <= stPoint; i++){
        disX = po.points[i].x - po.points[i-1].x;
        disY = po.points[i].y - po.points[i-1].y;
        po.points[i-1] = TPoint(stX, stY);
       // cerr << i-1 << ' ' << stX << ' ' << stY << endl;
        stX += disX;
        stY += disY;

        if (!insideFrame(TPoint(stX, stY)) && i != stPoint){
            isAvailable = false;
           // cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
        }

        if (grid[ stY ][ stX ] == 2){
            isAvailable = false;
        }
      //  grid[ stY ][ stX ] = 1;
    }

    if (!isAvailable)
        return false;

    for (int i = 0; i < po.numberOfPoints; i++){
        stX = po.points[i].x;
        stY = po.points[i].y;
       // cerr << stX << ' ' << stY << endl;
        grid[ stY ][ stX ] = 1;
    }

    for (int i = po.minX; i <= po.maxX; i++){
        for (int j = po.minY; j <= po.maxY; j++){
            int isInside = po.isPointInsidePolygon(TPoint(i,j));
            if (isInside == -1) continue;
            if (isInside == 0) grid[j][i] = 2;
            if (isInside == 1) grid[j][i] = 1;
            piecesOccupied[j][i] = 0;
        }
    }

    return true;
}

void Frame::reverseFrameState(Polygon po){
     for (int i = po.minX; i <= po.maxX; i++){
        for (int j = po.minY; j <= po.maxY; j++){
            int isInside = po.isPointInsidePolygon(TPoint(i,j));
            if (isInside == -1) continue;
            piecesOccupied[j][i]--;
            if (!piecesOccupied[j][i]) grid[j][i] = 0;
        }
    }
};

int Frame::howMuchCovered(){
    int ans = 0;
    for (int i = minX; i <= maxX; i++)
        for (int j = minX; j <= maxX; j++)
            ans += grid[j][i] > 0;

    return ans;
}

#endif // FRAME_H_INCLUDED
