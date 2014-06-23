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
#define ABS(Y) ((Y)<0?-(Y):(Y))
#define R 0 
#define T 1
#define D 2
#define L 3
#define INF 1000000000
#define TIME_LIMIT 15.0

typedef std::pair<int,int> pii;
typedef std::pair<pii,int> ppi;

using namespace std;

int xx[]={0,1,0,-1};
int yy[]={1,0,-1,0};

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

class Images
{
public:
    vector<vector <double> > adjl,adjr,adjt,adjd;
    Block* block;
    Block dull;
    int height, width;
    int N,X;

    Images(){}

    void loadImages()
    {
        IplImage* img;
        CvScalar pix;
        for(int i=0;i<X;i++)
        {
            char str[50],str1[50];
            strcpy(str,"generated_pieces/");
            sprintf(str1,"%d",i+1);
            strcat(str1,".jpg");
            strcat(str,str1);
            img=cvLoadImage(str);
            block[i].idx=i;
            for(int j=0;j<height;j++)
            {
                for(int k=0;k<width;k++)
                {
                    pix=cvGet2D(img,j,k); 
                    for(int h=0;h<3;h++) block[i].image[j][k].val[h] = pix.val[h];
                }
            }
        }
    }

    vector<Block> getScrambledImage()
    {
        vector<Block> v;
        for(int i=0;i<X;i++) v.pb(block[i]);
        return v;
    }

    void initializeAll()
    {
        IplImage* img;
        img = cvLoadImage("generated_pieces/1.jpg", CV_LOAD_IMAGE_COLOR);
        printf("Enter The Value of N in NxN : \n");
        height = img->height;
        width = img->width;
        scanf("%d",&N);
        X=N*N;
        initializeVector(X);
        assignMemory();
        loadImages();
        insertInTopMatrix();
        insertInLeftMatrix();
    }

    void initializeVector(int n)
    {
        adjl.resize(n);
        for(int i=0;i<n;i++) adjl[i].resize(n);
        adjr.resize(n);
        for(int i=0;i<n;i++) adjr[i].resize(n);
        adjt.resize(n);
        for(int i=0;i<n;i++) adjt[i].resize(n);
        adjd.resize(n);
        for(int i=0;i<n;i++) adjd[i].resize(n);
    }

    double getWeight(vector<Block> &c, int k, Block b)
    {
        double ans=0;
        int a=k/N,bb=k%N;

        for(int i=0;i<4;i++)
        {
            int a1=a+xx[i],b1=bb+yy[i];
            if(a1<0||a1>=N) continue;
            if(b1<0||b1>=N) continue;

            if(c[a1*N+b1].idx==-1) continue;
            if(i==0) ans+=adjr[b.idx][c[a1*N+b1].idx];
            if(i==1) ans+=adjd[b.idx][c[a1*N+b1].idx];
            if(i==2) ans+=adjl[b.idx][c[a1*N+b1].idx];
            if(i==3) ans+=adjt[b.idx][c[a1*N+b1].idx];
        }
        return ans;
    }

    void assignMemory()
    {
        block = new Block[X];
        int i,j,k;
        for(i=0;i<X;i++)
        {
            block[i].image = new Pixel*[height];
            block[i].bins = new int[bin];

            for(j=0;j<4;j++)
                block[i].id[j]=false;
            for(j=0;j<bin;j++)
            {
                block[i].bins[j] =0;
            }
            for(j=0;j<height;j++)
            {
                block[i].image[j] = new Pixel[width];
            }
        }
        dull.image = new Pixel*[height];
        dull.bins = new int[bin];
        for(j=0;j<height;j++)
            dull.image[j] = new Pixel[width];
    }



    double SSD_left(int sure,int trial)
    {
        double ssd=0;
        int i,j;
        for(i=0;i<height;i++)
        {
            for(j=1;j<=limit;j++)
            {
                for(int h=0;h<3;h++)
                    ssd = ssd + (block[sure].image[i][j-1].val[h]-block[trial].image[i][width - j].val[h])*(block[sure].image[i][j-1].val[h]-block[trial].image[i][width - j].val[h]);
            }
        }
        return ssd;
    }

    double SSD_top(int sure,int trial)
    {
        double ssd=0;
        int i,j;
        for(i=1;i<=limit;i++)
        {
            for(j=0;j<width;j++)
            {
                for(int h=0;h<3;h++)
                    ssd = ssd + (block[sure].image[i-1][j].val[h]-block[trial].image[height - i][j].val[h])*(block[sure].image[i-1][j].val[h]-block[trial].image[height - i][j].val[h]);
            }
        }
        return ssd;
    }


    void insertInLeftMatrix()
    {
        double ssd;
        for(int i=0;i<X;i++)
        {
            for(int j=0;j<X;j++)
            {
                if(i==j)
                    continue;
                ssd = SSD_left(i,j);
                adjl[i][j]=ssd;
                adjr[j][i]=ssd;
            }
        }
    }

    void insertInTopMatrix()
    {
        double ssd;
        for(int i=0;i<X;i++)
        {
            for(int j=0;j<X;j++)
            {
                if(i==j)
                    continue;
                ssd = SSD_top(i,j);
                adjt[i][j]=ssd;
                adjd[j][i]=ssd;
            }
        }
    }
};
#endif