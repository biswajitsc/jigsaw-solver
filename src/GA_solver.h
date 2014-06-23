#ifndef GA_SOLVER_H
#define GA_SOLVER_H

#include <iostream>
#include <time.h>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <utility>
#include <vector>
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

typedef vector<Block> vb;

struct Data
{
  double wt;
  int ind;
  Data() : wt(0.0) , ind(-1) {}
  bool operator < (const Data &x) const{
    return wt<x.wt;
  }
};
class GA
{
	int N,X;
	Images* pieces;
	vector<int> bbl, bbr, bbt, bbd;
	clock_t start_time;
	void bestBuddy();
	int findbuddy(vb &c, bool * used, int k);
	vb crossover(vb &a, vb &b);
	double fitness(vb &c);
	vector< vb > bestGen(vector<vb > &gen);
	vector< vb > generation(vector<vb > &gen,int height,int width);
	

public:
	GA(int n, Images * image)
	{
		N=n;
  		start_time=clock();
		X = n*n;
		pieces = image;
		bestBuddy();
	}
	vb runAlgo(int height,int width);
};

#endif