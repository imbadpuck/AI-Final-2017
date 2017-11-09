using namespace std;
#include<bits/stdc++.h>
#include<Polygon.h>

int main(){
#ifndef ONLINE_JUDGE
    freopen("sample_input.inp", "r", stdin);
#endif // ONLINE_JUDGE

    int n;
    cin >> n;
    for (int i = 0; i < n; i++){
        Polygon p;
        int c;
        cin >> c;
        cout << c << ' ';
        cin >> p.numberOfPoints;
        cout << p.numberOfPoints << ' ';
        for (int j = 0; j < p.numberOfPoints; j++){
            cin >> p.points[j].x >> p.points[j].y;
            cout << p.points[j].x << ' ' << p.points[j].y << ' ';
        }
        cout << '\n';
        p.calculateAllAngles();
        for (int j = 0; j < p.numberOfPoints; j++)
            cout << p.angles[j] << ' ';
        cout << '\n';

    }

    return 0;
}
