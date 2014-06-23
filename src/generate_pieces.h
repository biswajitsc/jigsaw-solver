#ifndef GENERATE_PIECES_H
#define GENERATE_PIECES_H

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

Block* block;
void generateImages(IplImage* img, int n, int height, int width);
void assignMemory(int height,int width,int X);
vector<Block> permute(int n);
#endif
