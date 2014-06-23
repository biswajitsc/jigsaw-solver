#ifndef MST_SOLVER_H
#define MST_SOLVER_H

#include <iostream>
#include <time.h>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <utility>
#include <vector>
#include <time.h>
#include <sstream>
#include <fstream>
#include <cmath>
#include "image.hpp"

using namespace std;
#define pb push_back
#define bin 10
#define limit 1
#define ABS(Y) ((Y)<0?-(Y):(Y))
#define R 0 
#define T 1
#define D 2
#define L 3
#define INF 1000000000
#define TIME_LIMIT 15.0


struct minDis
{
	int cc;
	int id;
	bool operator <(const minDis &x)const
	{
		return this->cc > x.cc;
	}
	minDis() : cc(0),id(-1) {}
	minDis(int c,int i)
	{
		cc = c;
		id = i;
	}
};

struct edges
{
	int i,j,id;
	double weight;
	bool operator <(const edges & x)const
	{
		return this->weight>x.weight;
	}
	edges() : id(-1),i(-1),j(-1) {}
	edges(const int  u,const int v,const int c,const double w) 
	{
	  i = u;
	  j = v;
	  id = c;
	  weight = w;
	}
	void debug()
	{
	  printf("\n %d %d %d %lf",i,j,id,weight);
	}
};
class MST
{
	int N,X;
	Images* pieces;
	
public:
	MST(int n, Images * image):N(n),X(n*n),pieces(image){}
	vector<Block> get_mst(int height, int width);
	void fill_greedy(vector<Block> & ans, bool * used);
};

#endif