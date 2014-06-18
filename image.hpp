#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <iostream>
#include <stdlib.h>
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
#include <opencv/highgui.h>
#include <opencv/cv.h>

#define pb push_back
#define bin 10
#define limit 1

using namespace std;


struct Pixel
{
    float val[3];
    int val_gray[3];
};

struct Block {
    Pixel** image;
    int n;
    bool id[4];
    int neigh[4];
    int* bins;
    int idx;
};

Block* assignMemory(int height, int width, int n)
{
    Block *block = new Block[n];
    int i, j, k;
    for(i = 0; i < n; i++)
    {
        block[i].image = new Pixel*[height];
        block[i].bins = new int[bin];

        for(j = 0; j < 4; j++){
            block[i].id[j]=false;
        }

        for(j = 0; j < bin; j++){
            block[i].bins[j] =0;
        }

        for(j = 0; j < height; j++){
            block[i].image[j] = new Pixel[width];
        }
    }
    return block;
}
#endif