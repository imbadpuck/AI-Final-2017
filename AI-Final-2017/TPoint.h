#ifndef TPOINT_H_INCLUDED
#define TPOINT_H_INCLUDED

typedef long long LL;
struct TPoint{
    int x,y;
    TPoint(int a = 0,int b = 0) : x(a),y(b){};
    bool operator <(const TPoint &b)const{
        return (x != b.x ? x<b.x : y<b.y);
    }
};

inline TPoint operator+(TPoint a, TPoint b){
    return TPoint(a.x+b.x,a.y+b.y);
}

inline TPoint operator-(TPoint a, TPoint b){
    return TPoint(a.x-b.x,a.y-b.y);
}

inline int operator*(TPoint a, TPoint b){
    return a.x*b.x + a.y*b.y;
}

inline TPoint operator*(LL k, TPoint a){
    return TPoint(k*a.x,k*a.y);
}

inline int operator^(TPoint a, TPoint b){
    return a.x*b.y - a.y*b.x;
}


inline int dis(TPoint a,TPoint b){//binh phuong khoang cach
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

inline int mandis(TPoint a, TPoint b){ //khoang cach manhattan giua 2 diem
    return abs(a.x-b.x) + abs(a.y-b.y);
}

inline int ccw(TPoint a,TPoint b,TPoint c){//chieu di quanh bien
    LL cur=(b-a)^(c-a);
    return (cur<0 ? -1:(cur>0 ? 1:0));
}

inline double angle(TPoint a, TPoint b, TPoint c){
    double dot = (c-b)*(b-a);
    double det = (c-b)^(b-a);
    double PI = 3.14159265;
    double ans = atan2(det, dot);

    return (ans > 0 ? ans : (2*PI + ans)) * 360 / (2*PI);
}

inline LL area(TPoint p[],int n){//dien tich da giac
    LL s=0LL;
    for(int i=1;i<n;++i) s+=(p[i].x-p[i-1].x)*(p[i].y+p[i-1].y);
    s+=(p[0].x-p[n-1].x)*(p[0].y+p[n-1].y);
    return (s<0 ? (-s)/2 : s/2);
}

struct seg{
    TPoint a,b;
    seg(TPoint a,TPoint b) : a(a),b(b){}
};

inline bool pinseg(TPoint c,seg d){//diem nam tren doan
    if (ccw(c,d.a,d.b)==0) return ((c-d.a)*(c-d.b)<0);
    return 0;
}


#endif // TPOINT_H_INCLUDED
