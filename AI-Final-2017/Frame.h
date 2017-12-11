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
    ///= -1 diem khong thuoc khung
    ///= 0 diem chua duoc phu boi cac hinh
    ///= 1 diem duoc phu boi bien cua mieng
    ///= 2 diem nam trong cac mieng

    TPoint borders[20]; ///cac diem bien cua khung

    void init();
    bool insideFrame(TPoint p); ///kiem tra xem diem co nam trong khung ko
    bool placePieceIntoFrame(Polygon &po, int stPoint, TPoint p); ///dat mot mieng vao trong khung tai diem p
    void reverseFrameState(Polygon po, int stPoint, TPoint p); ///khoi phuc trang thai cua bang ve trang thai truoc khi
                                                                ///dat hinh vao khung

    int howMuchCovered(); ///frame da duoc covered bao nhieu
};

bool Frame::insideFrame(TPoint p){///wrong
    if (p.x < minX || p.y < minY || p.x > maxX || p.y > maxY){
        return false;
    }
    return true;
}

void Frame::init(){
    memset(grid, 0xff, sizeof(grid));

    maxX = maxY = 0;
    minX = minY = maxColumn;
    for (int i = 0; i < numOfPoints; i++){
        minX = min(minX, borders[i].x);
        maxX = max(maxX, borders[i].x);
        minY = min(minY, borders[i].y);
        maxY = max(maxY, borders[i].y);
        grid[ borders[i].y ][ borders[i].x ] = 1;
    }

    for (int i = minX; i <= maxX; i++)
        for (int j = minY; j <= maxY; j++)
            //if (insideFrame(j, i))
                grid[j][i] = 0;
}


bool Frame::placePieceIntoFrame(Polygon &po, int stPoint, TPoint p){
    int stX = p.x;
    int stY = p.y;
    int disX, disY; ///khoang cach Manhattan giua toa do 2 diem trong da giac
    //grid[ stY ][ stX ] = 1;
 //   cerr << stX << ' ' << stY << endl;

    bool stillInside = true;
    for (int i = stPoint+1; i < po.numberOfPoints; i++){
        disX = po.points[i].x - po.points[i-1].x;
        disY = po.points[i].y - po.points[i-1].y;
        po.points[i-1] = TPoint(stX, stY);
        cerr << i-1 << ' ' << stX << ' ' << stY << endl;
        stX += disX;
        stY += disY;
        if (!insideFrame(TPoint(stX, stY))){
            stillInside = false;
         //   cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
        }

        //grid[ stY ][ stX ] = 1;
    }

    disX = po.points[0].x - po.points[po.numberOfPoints-1].x;
    disY = po.points[0].y - po.points[po.numberOfPoints-1].y;
    po.points[po.numberOfPoints-1] = TPoint(stX, stY);
    cerr << po.numberOfPoints-1 << ' ' << stX << ' ' << stY << endl;
    stX += disX;
    stY += disY;

    if (!insideFrame(TPoint(stX, stY))){
        stillInside = false;
        //cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
    }
    //grid[ stY ][ stX ] = 1;

    for (int i = 1; i <= stPoint; i++){
        disX = po.points[i].x - po.points[i-1].x;
        disY = po.points[i].y - po.points[i-1].y;
        po.points[i-1] = TPoint(stX, stY);
        cerr << i-1 << ' ' << stX << ' ' << stY << endl;
        stX += disX;
        stY += disY;

        if (!insideFrame(TPoint(stX, stY)) && i != stPoint){
            stillInside = false;
           // cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
        }
      //  grid[ stY ][ stX ] = 1;
    }

    if (!stillInside)
        return false;

    for (int i = 0; i < po.numberOfPoints; i++){
        stX = po.points[i].x;
        stY = po.points[i].y;
       // cerr << stX << ' ' << stY << endl;
        grid[ stY ][ stX ] = 1;
    }

    return true;
}

void Frame::reverseFrameState(Polygon po, int stPoint, TPoint p){

};

int Frame::howMuchCovered(){
 //   for (int i = 0; i < )
    return 1;
}

#endif // FRAME_H_INCLUDED
