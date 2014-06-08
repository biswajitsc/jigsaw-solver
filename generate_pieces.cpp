#include <iostream>
#include <algorithm>
#include <highgui.h>
#include <cv.h>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <utility>
#include <vector>

#define bin 10
#define pb push_back

using namespace std;

typedef struct Pixel
{
    float val[3];
    int val_gray[3];
} Pixel;

typedef struct Block {
    Pixel** image;
    int n;
    bool id[4];
    int neigh[4];
    int* bins;
    int idx;
} Block;

Block* block;

void assignMemory(int height, int width, int n)
{
    block = new Block[n];
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
}

void generateImages(IplImage* img, int n, int height, int width)
{
    int start,stop;
    int X=n*n;
    CvScalar pix;
    for(int i = 0; i < X; i++)
    {
        start = i/n;
        stop  = i%n;
        start = start*height;
        stop = stop*width;
        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < width; k++)
            {
                pix=cvGet2D(img, j+start, k+stop); 
                for(int h = 0; h < 3; h++)
                {
                    block[i].image[j][k].val[h]=pix.val[h];
                }
            }
        }
    }
}

vector<Block> permute(int n)
{
    vector<Block> temp;
    for(int j = 0; j < n; j++)
    {
        block[j].idx = j;
        temp.pb(block[j]);
    }
    for(int j = 0; j < n; j++)
    {
        int rnd;
        rnd=rand()%n;
        swap(temp[j], temp[rnd]);
    }
    return temp;
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        cout<<"No file name found. Please pass a file name as a parameter."<<endl;
        exit(1);
    }

    IplImage *img,*scrambled;

    int height,width,n;
    CvScalar pix;
    img=cvLoadImage(argv[1]);
    if(img==0)
    {
        cout<<argv[1]<<": File not found."<<endl;
        exit(1);
    }
    cout<<"Image dimensions: "<<img->height<<" "<<img->width<<endl;
    cout<<"Enter side length of square grid: ";
    cin>>n;

    height=(img->height)/n;
    width=(img->width)/n;

    assignMemory(height,width,n*n);
    generateImages(img,n,height,width);

    vector<Block> permuted = permute(n*n);

    scrambled=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);

    for(int i=0;i<n*n;i++)
    {
        char filename[50]="./generated_pieces/",fileid[10];
        sprintf(fileid,"%d",i+1);
        strcat(fileid,".jpg");
        for(int j=0;j<height;j++)
        {
            for(int k=0;k<width;k++)
            {
                for(int h=0;h<3;h++)
                {
                    pix.val[h]=permuted[i].image[j][k].val[h];
                }
                cvSet2D(scrambled,j,k,pix);
            }
        }
        strcat(filename,fileid);
        cvSaveImage(filename,scrambled);
        printf("Generated %s\n",fileid);
    }
    return 0;

}