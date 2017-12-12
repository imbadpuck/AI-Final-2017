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
    cout << endl;

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

void tryToFillFrame(int u, int stPoint, TPoint p, int putInFrame){///dat hinh u
    visit[u] = true;
    int cnt = 0;
    cerr << u << ' ' << putInFrame << endl;
    while (cnt < 8){
        bool canBePut = frame.placePieceIntoFrame(pieces[u], stPoint, p);
        cerr << u << ' ' << cnt << ' ' << p.x << ' ' << p.y << ' ' << canBePut << endl;
        if (canBePut){
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
                    bestCover = covered;
                }
            }
            frame.reverseFrameState(pieces[u]);
        }

        cnt++;
        pieces[u].rotatePolygon();
        if (cnt == 4) pieces[u].flipPolygon();
        pieces[u].init();
    }

    visit[u] = false;

}

void initValue(){
    bestCover = 0;
    for (int i = 0; i < numberOfPieces; i++){
        pieces[i].assignValue(&finalResult[i]);
    }

    memset(visit, false, sizeof(visit));
}

void solver_dummy(){
    //cerr << g[0].size() << ' ';
    tryToFillFrame(0, 0, TPoint(0, 0), 1);

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
    //cerr << frame.minX << ' ' << frame.minY << ' ' << frame.maxX << ' ' << frame.maxY << endl;
    solver_dummy();
   /* pieces[6].flipPolygon();
    pieces[6].init();
    bool c1 = frame.placePieceIntoFrame(pieces[6], 0, TPoint(68, 20));
    pieces[9].rotatePolygon();
    pieces[9].rotatePolygon();
    pieces[9].rotatePolygon();
    //pieces[13].flipPolygon();
    pieces[9].init();
    bool c2 = frame.placePieceIntoFrame(pieces[9], 0, TPoint(80, 40));

//    cout << c1 << ' ' << c2 << endl;
    for (int j = frame.minY; j <= frame.maxY; j++){
        for (int i = frame.minX; i <= frame.maxX; i++)
            cout << frame.grid[j][i] << ' ';
        cout << endl;
    }*/

    output();

    return 0;
}
