#include <stdio.h>
#include <stdbool.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cmath>
#include <ctime>
using namespace std;

int n;
struct Point 
{
	 int x, y;
};
bool cmp_x(const Point& a,const Point& b) { 
    return a.x < b.x; 
} 

bool cmp_y(const Point& a,const Point& b) { 
    return a.y < b.y; 
} 
void printpoints(Point* t,int c) {
	printf("\n");
	for(int i = 0; i < c; i++)
		printf("(%d %d),", t[i].x , t[i].y);
	printf("\n");
	printf("points printed\n");
}


double sq_dst(Point &a, Point &b) {
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	return dx*dx + dy*dy;
}
double dst(Point &a, Point &b) {
	return sqrt(sq_dst(a, b));
}

bool in_range(Point &p, double l, double dist){
	double t = (double)p.x;
	return ((l - dist <= t) and (t <= l + dist));
}

inline tuple<Point, Point, double> closest3(Point& p1, Point& p2, Point& p3){
	auto d12 = sq_dst(p1, p2);
	auto d23 = sq_dst(p2, p3);
	auto d13 = sq_dst(p1, p3);
	if (d12  < d23)
		if (d12 < d13)
			return make_tuple(p1, p2, sqrt(d12));
		else
			return make_tuple(p1, p3, sqrt(d13));
	else
		if (d23 < d13)
			return make_tuple(p2, p3, sqrt(d23));
		else
			return make_tuple(p1, p3, sqrt(d13));
}

tuple<Point, Point, double> closest_pair(Point* tx, Point* ty, Point* typ, int start,int end){
	//printf("call %d %d\n", start, end);
	// base calls
	if (end - start == 1){
		//printf("base call 2\n");
		return make_tuple(tx[start], tx[end], dst(tx[start], tx[end]));
	} else if (end - start == 2){
		//printf("base call 3\n");
		return closest3(tx[start], tx[start+1], tx[start+2]);
	}

	// divide for L and R, create dividing lane
	int middle = (start + end)/2;
	double lane = (((start + end)%2 == 1) ? (((double)tx[middle].x + tx[middle + 1].x) / 2) : tx[middle].x);
	
	//printf("sl:%d el:%d sr:%d sl:%d, lane:%f\n", start, middle, middle + 1, end, lane);
	// recurrency 
	auto point_pair_l = closest_pair(tx, ty, typ, start, middle);
	auto point_pair_r = closest_pair(tx, ty, typ, middle + 1, end);
	auto min_dist_l = get<2>(point_pair_l);
	auto min_dist_r = get<2>(point_pair_r);
	
	// choose smaller from l and r
	double min_dist;
	tuple<Point, Point, double> point_pair;
	if (min_dist_l < min_dist_r) {
		point_pair = point_pair_l;
		min_dist = min_dist_l;	
	} else {
		point_pair = point_pair_r;
		min_dist = min_dist_r;
	}

	// look through middle strip
	double temp_dist;
	int* point1;
	int* point2;


	
	vector<Point> p_c;
	p_c.reserve(n / 2);
	for(int i = 0; i < n; i++)
		if (in_range(ty[i], lane, min_dist))
			p_c.push_back(ty[i]);
			//p_c.insert(ty[i], p_c.end());
	
	for(int i = 0; i < p_c.size(); i++){
		for(int j = 1; j <= 7 and j+i < p_c.size(); j++){
			temp_dist = dst(p_c.at(i), p_c.at(i+j));
			if (temp_dist < min_dist) {
				min_dist = temp_dist;
				point_pair = make_tuple(p_c.at(i), p_c.at(i+j), min_dist);
			}
		}
	}
//	*/
	
	
/*	
	int k = 0;
	for (int i = 0; i < n; i++)
		if (in_range(ty[i], lane, min_dist)){
			typ[k] = ty[i];
			k++;
		}
	for (int i = 0; i < k; i++){
		for (int j = 1; j<= 7 and i+j < k; j++){
			temp_dist = dst(typ[i], typ[i+j]);
			if (temp_dist < min_dist) {
				min_dist = temp_dist;
				point_pair = make_tuple(typ[i], typ[i+j], min_dist);
			}
		}
	}
*/
	
	return point_pair;
}

int main() {
  	clock_t begin = clock();
	//printf("start\n");
	scanf("%d", &n);
	Point * ax = new Point[n];
	Point * ay = new Point[n];
	Point * ay_prim = new Point[n];

	int x, y;
	for(int i = 0; i < n; i++){
		scanf("%d %d", &x, &y);
		ax[i].x = x;
		ax[i].y = y;
		ay[i].x = x;
		ay[i].y = y;
	}
	sort(ax, ax + n, cmp_x);
	sort(ay, ay + n, cmp_y);

	printpoints(ax, n);
	printpoints(ay, n);

	
	//printf("calling\n");
	
	auto z = closest_pair(ax, ay, ay_prim, 0, n-1);
	//printf("done1\n");
	auto p1 = get<0>(z);
	//printf("done2\n");
	auto p2 = get<1>(z);
	//printf("done3\n");
	//auto d = get<2>(z);
	printf("%d %d\n%d %d", p1.x, p1.y, p2.x, p2.y); 
	//printf("done4\n");
	
	
	/*
	for(int i = 0; i < n; ++i) {
		delete[] ax[i];   
		delete[] ay[i];   
		//delete[] ay_prim[i];   
	}
	delete[] ax;
	delete[] ay;
	//delete[] ay_prim;   
	*/
	clock_t end = clock();
	double t = double(end - begin) / CLOCKS_PER_SEC;
	printf("time :: %f\n", t);
	return 0;
}

