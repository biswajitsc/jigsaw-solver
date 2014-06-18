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

struct data
{
	int cc;
	int id;
	bool operator <(const data &x)const
	{
		return this->cc > x.cc;
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
};

edges asign(const int  i,const int j,const int id,const double w);

data asign_data(int c,int i);

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