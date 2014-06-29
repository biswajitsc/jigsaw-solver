#include <iostream>
#include <time.h>
#include "image.hpp"
#include <iostream>
#include <stdlib.h>
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

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "image.hpp"
#include "MST_solver.cpp"
#include "GA_solver.cpp"

using namespace std;

#define TIME_LIMIT 15.0

typedef std::pair<int,int> pii;
typedef std::pair<pii,int> ppi;


int N,X;
Images pieces;

void saveResult(vector<Block> &ans,int height,int width,const string output)
{
    IplImage* final;
    int i,start,stop,j,k,h;
    CvScalar pix;

    final = cvCreateImage(cvSize(width*N,height*N),IPL_DEPTH_8U,3);
    for(i=0;i<X;i++)
    {
        start = i/N;
        stop  = i%N;
        start = start*height;
        stop = stop*width;
        for(j =0; j<height; j++)
        {
            for(k = 0; k<width;k++)
            {
                for(h =0; h<3;h++)
                    pix.val[h]=ans[i].image[j][k].val[h];
                cvSet2D(final,j+start,k+stop,pix);
            }
        }
    }
    cvSaveImage(output.c_str(),final);
}

int main(int argc, char *argv[])
{
    vector<Block> ans;
    pieces.initializeAll();
    N=pieces.N;
    X=N*N;

    vector<Block> scrambled=pieces.getScrambledImage();
    saveResult(scrambled,pieces.height, pieces.width,"scrambled_image.jpg");

    if(N>15)
    {
        MST mst(N,&pieces);
        ans=mst.get_mst(pieces.height,pieces.width);
        saveResult(ans,pieces.height,pieces.width,"solved_image.jpg");
    }
    else
    {
        GA ga(N,&pieces);
        ans=ga.runAlgo(pieces.height,pieces.width);
        saveResult(ans,pieces.height,pieces.width,"solved_image.jpg");
    }
    return 0;
}

