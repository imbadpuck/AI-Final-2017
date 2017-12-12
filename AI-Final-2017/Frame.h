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
    for (int i = stPoint; i < stPoint + po.numberOfPoints; i++){
        //cerr << (i+1) % po.numberOfPoints << ' ' << i % po.numberOfPoints << endl;
        disX = po.points[(i+1) % po.numberOfPoints].x - po.points[i % po.numberOfPoints].x;
        disY = po.points[(i+1) % po.numberOfPoints].y - po.points[i % po.numberOfPoints].y;
        po.points[i % po.numberOfPoints] = TPoint(stX, stY);
       // cerr << i-1 << ' ' << stX << ' ' << stY << endl;
        if (!insideFrame(TPoint(stX, stY))){
            isAvailable = false;
         //   cerr << "sai tai day: " << ' ' << stX << ' ' << stY << endl;
        } else {
          //  cerr << stX << ' ' << stY << endl;
            if (grid[ stY ][ stX ] == 2){
                isAvailable = false;
            }
        }
        stX += disX;
        stY += disY;
        //grid[ stY ][ stX ] = 1;
    }

///    0 1 2 3 4 5.0 6.1 7.2 8.3

    if (!isAvailable)
        return false;

   /* for (int i = 0; i < po.numberOfPoints; i++){
        stX = po.points[i].x;
        stY = po.points[i].y;
       // cerr << stX << ' ' << stY << endl;
        grid[ stY ][ stX ] = 1;
    }*/
  //  cerr << "Available" << endl;

    po.init();
    vector<TPoint> hasOccupied;

    for (int i = po.minX; i <= po.maxX; i++){
        for (int j = po.minY; j <= po.maxY; j++){
            if (!insideFrame(TPoint(i, j))) continue;
            int isInside = po.isPointInsidePolygon(TPoint(i,j));
            if (isInside == -1) continue;
            bool pass = true;
            if (grid[j][i] != 2 && grid[j][i] != -1){
                if (isInside == 0) {
                    if (grid[j][i] == 0) grid[j][i] = 2;
                    else pass = false;
                }
                if (isInside == 1) grid[j][i] = 1;
                piecesOccupied[j][i]++;
                hasOccupied.push_back(TPoint(i,j));
            } else pass = false;
            if (!pass){
                for (vector<TPoint>::iterator it=hasOccupied.begin(); it != hasOccupied.end(); it++){
                    int x = it->x;
                    int y = it->y;
                    piecesOccupied[y][x]--;
                    if (!piecesOccupied[y][x]) grid[y][x] = 0;
                }
                return false;
            }
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
