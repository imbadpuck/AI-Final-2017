using namespace std;
#include<bits/stdc++.h>
#include<TPoint.h>
#include<Polygon.h>
#include<Frame.h>

const int MAXN = 1+30;
int numberOfPieces;
Polygon pieces[MAXN];
Frame frame;

void input(){
    ///read Piece
    cin >> numberOfPieces;
   // cout << numberOfPieces << endl;
    for (int i = 0; i < numberOfPieces; i++){
        int c;
        cin >> c;
    //    cout << c << ' ';
        cin >> pieces[i].numberOfPoints;
    //    cout << pieces[i].numberOfPoints << ' ';
        for (int j = 0; j < pieces[i].numberOfPoints; j++){
            cin >> pieces[i].points[j].x >> pieces[i].points[j].y;
     //       cout << pieces[i].points[j].x << ' ' << pieces[i].points[j].y << ' ';
        }

        pieces[i].init();
        pieces[i].calculateAllAngles();
        pieces[i].calculateAllSegments();
    //    cout << '\n';
    //    for (int j = 0; j < pieces[i].numberOfPoints; j++)
    //        cout << pieces[i].angles[j] << ' ';
    //    cout << '\n';
    }

    ///read Frame
    int frameID;
    cin >> frameID;
  //  cout << frameID << ' ';

    cin >> frame.numOfPoints;
   // cout << frame.numOfPoints << ' ';
    for (int i = 0; i < frame.numOfPoints; i++){
        cin >> frame.borders[i].x >> frame.borders[i].y;
      //  cout << frame.borders[i].x << ' ' << frame.borders[i].y << ' ';
    }
    frame.init();
/*    for (int j = frame.minY; j <= frame.maxY; j++){
        for (int i = frame.minX; i <= frame.maxX; i++)
            cout << frame.grid[j][i] << ' ';
        cout << endl;
    }*/


    //cout << '\n';
}

typedef pair<int, int> II;

struct nextPiece{
    int indexNext; ///chi so cua mieng duoc ghep canh mieng u
    int whereThis, whereThat; ///tai diem nao cua mieng u, tai diem nao cua mieng duoc ghep canh mieng u

    nextPiece(int a = 0, int b = 0, int c = 0) : indexNext(a),whereThis(b), whereThat(c){};
};

vector <nextPiece> g[MAXN];
//VVII g; ///g[u]: = (st, nd) st = mieng co the ghep duoc voi mieng u, nd = i = tai vi tri diem i
//VVII::iterator it;
void initGraph(){ ///init g, tao cac cap hinh co the ghep cung nhau
  //  g.resize(numberOfPieces+1);
    vector< pair<int, int> > index;
    for (int i = 0; i < numberOfPieces; i++){
        for (int j = 0; j < numberOfPieces; j++){
            if (i == j) continue;
            try {
                index = pieces[i].whereToPut(pieces[j]);

            }
            catch (std::bad_alloc& ba){
                std::cerr << "bad alloc caught: " << ba.what() << endl;
            }
            for (vector< pair<int, int> >::iterator it=index.begin(); it != index.end(); it++){
                g[i].push_back(nextPiece(j, it->first, it->second));
    //            cout << i << ' ' << j << ' ' << *it << endl;
       //         cout << i << ' ' << j << ' ' << it->first << ' ' << it->second << endl;
            }
      //      cerr << i << ' ' << j << ' ' << index.size() << endl;
         }
      //  cerr << i << ' ' << g[i].size() << endl;
    }
}

int bestCover;
Polygon finalResult[MAXN];
bool visit[MAXN];
bool reduceState[maxRow][maxColumn][MAXN][17][8];

void tryToFillFrame(int u, int stPoint, TPoint p, int putInFrame){///dat hinh u
    visit[u] = true;
    int cnt = 0;
  //  cerr << u << ' ' << putInFrame << endl;
    while (cnt < 8){
        int canBePut = 1;
        if (!reduceState[p.y][p.x][u][stPoint][cnt])
            canBePut = frame.placePieceIntoFrame(pieces[u], stPoint, p);
        else
            canBePut = -1;
     //   cerr << u << ' ' << cnt << ' ' << p.x << ' ' << p.y << ' ' << stPoint << ' ' << canBePut << endl;
        if (canBePut == 1){
            bool foundNewPoint = false;
            for (vector< nextPiece >::iterator it = g[u].begin(); it != g[u].end(); it++){
                int v = it->indexNext;
               // cerr << u << ' ' << v << ' ' << visit[v] << endl;
                if (!visit[v]){
                    int pThis = it->whereThis;
                    int pThat = it->whereThat;
                    TPoint newP = TPoint(pieces[u].points[pThis].x, pieces[u].points[pThis].y);
                    tryToFillFrame(v, pThat, newP, putInFrame+1);
                    foundNewPoint = true;
                }
            }

            if (!foundNewPoint && putInFrame == numberOfPieces){
                int covered = frame.howMuchCovered();
                if (covered > bestCover){
                    cerr << "OK" << ' ' << covered << ' ' << bestCover << endl;
                    for (int i = 0; i < numberOfPieces; i++){
                        pieces[i].assignValue(&finalResult[i]);
                    }

                    cout << numberOfPieces << endl;
                    for (int i = 1; i <= numberOfPieces; i++){
                        cout << i << ' ';
                        cout << finalResult[i-1].numberOfPoints << ' ';
                        for (int j = 0; j < finalResult[i-1].numberOfPoints; j++){
                            cout << finalResult[i-1].points[j].x << ' ' << finalResult[i-1].points[j].y << ' ';
                        }
                        cout << endl;
                    }

                    cout << numberOfPieces+1 << ' ';
                    cout << frame.numOfPoints << ' ';
                    for (int i = 0; i < frame.numOfPoints; i++){
                        cout << frame.borders[i].x << ' ' << frame.borders[i].y << ' ';
                    }
                    bestCover = covered;
                    exit(0);
                }
            }
            frame.reverseFrameState(pieces[u]);
        } else{
            if (canBePut == -1)
                reduceState[p.y][p.x][u][stPoint][cnt] = true;
        }

        cnt++;
        pieces[u].rotatePolygon();
        if (cnt == 4) pieces[u].flipPolygon();
        pieces[u].init();
    }

    pieces[u].flipPolygon();
    pieces[u].init();

    visit[u] = false;

}

void initValue(){
    bestCover = 0;
    for (int i = 0; i < numberOfPieces; i++){
        pieces[i].assignValue(&finalResult[i]);
    }

    memset(visit, false, sizeof(visit));
    memset(reduceState, 0, sizeof(reduceState));
}

void solver_dummy(){
    //cerr << g[0].size() << ' ';
    //tryToFillFrame(0, 0, TPoint(0, 0), 1);
    //pieces[4].flipPolygon();
   // tryToFillFrame(4, 4, TPoint(15, 9), 1);
    for (int i = 0; i < numberOfPieces; i++){
        for (int k = 0; k < 8; k++){
            for (int j = 0; j < pieces[i].numberOfPoints; j++){
                int canBePut = frame.placePieceIntoFrame(pieces[i], j, frame.borders[0]);
                if (canBePut == 1){
                    frame.reverseFrameState(pieces[i]);
                    tryToFillFrame(i,j,frame.borders[0],1);
                }
            }
            pieces[i].rotatePolygon();
            if (k == 4) pieces[i].flipPolygon();
        }
        pieces[i].flipPolygon();
    }
   //    pieces[0].rotatePolygon();
   // pieces[0].rotatePolygon();
  //  pieces[0].flipPolygon();
    //frame.placePieceIntoFrame(pieces[0],6,TPoint(0,64));
   // tryToFillFrame(0, 6, TPoint(0, 64), 1);
}

void output(){
    cout << numberOfPieces << endl;
    for (int i = 1; i <= numberOfPieces; i++){
        cout << i << ' ';
        cout << finalResult[i-1].numberOfPoints << ' ';
        for (int j = 0; j < finalResult[i-1].numberOfPoints; j++){
            cout << finalResult[i-1].points[j].x << ' ' << finalResult[i-1].points[j].y << ' ';
        }
        cout << endl;
    }

    cout << numberOfPieces+1 << ' ';
    cout << frame.numOfPoints << ' ';
    for (int i = 0; i < frame.numOfPoints; i++){
        cout << frame.borders[i].x << ' ' << frame.borders[i].y << ' ';
    }
}

int main(){
    ios::sync_with_stdio(false);
    freopen("sample_input.inp", "r", stdin);
    freopen("sample_output.out", "w", stdout);

    input();
    initGraph();
    initValue();
    solver_dummy();
    output();
 /*   pieces[4].flipPolygon();
    int c1 = frame.placePieceIntoFrame(pieces[4], 4, TPoint(15, 9));
    for (int i = 0; i < pieces[4].numberOfPoints; i++){
        cerr << pieces[4].points[i].x << ' ' << pieces[4].points[i].y << endl;
    }*/
    //cerr << c1 << endl;
  /*  bool c2 = frame.placePieceIntoFrame(pieces[1], 0, TPoint(15, 30));
    bool c3 = frame.placePieceIntoFrame(pieces[2], 0, TPoint(0, 0));
    bool c4 = frame.placePieceIntoFrame(pieces[3], 0, TPoint(30, 20));
    bool c5 = frame.placePieceIntoFrame(pieces[4], 0, TPoint(40, 20));*/
 //   cerr << pieces[2].isPointInsidePolygon(TPoint(40, 19)) << endl;
//    cout << c1 << ' ' << c2 << endl;
    /*for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///1;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///2;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///3;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
   cerr << endl;
    pieces[2].rotatePolygon(); ///4;
    pieces[2].flipPolygon();
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///5;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///6;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///7;
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0
    cerr << endl;
    pieces[2].rotatePolygon(); ///8;
    pieces[2].flipPolygon(); ///8
    for (int i = 0; i < pieces[2].numberOfPoints; i++){
        cerr << pieces[2].points[i].x << ' ' << pieces[2].points[i].y << " ";
    }///0*/
   // p2.

    for (int j = frame.minY; j <= frame.maxY; j++){
        for (int i = frame.minX; i <= frame.maxX; i++)
            if (frame.grid[j][i] == -1) cout << "4 ";
            else cout << frame.grid[j][i] << ' ';
        cout << endl;
    }


  /*  for (int i = 0; i < numberOfPieces; i++){
        for (vector<nextPiece>::iterator it = g[i].begin(); it != g[i].end(); it++){
            cout << i << ' ' << it->indexNext << ' ' << it->whereThis << ' ' << it->whereThat << endl;
        }
    }

    for (int i = 0; i < pieces[0].numberOfPoints; i++) cerr << pieces[0].angles[i] << ' '; cerr << endl;
    for (int i = 0; i < pieces[1].numberOfPoints; i++) cerr << pieces[1].angles[i] << ' '; cerr << endl;
    cerr << pieces[0].angles[1] + pieces[1].angles[2] << endl;*/

    return 0;
}

