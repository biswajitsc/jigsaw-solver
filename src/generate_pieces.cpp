#include "generate_pieces.h"


void assignMemory(int height,int width,int X)
{
    block = new Block[X];
    Block dull;
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
                pix = cvGet2D(img, j+start, k+stop); 
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
    int height,width,n;
    int len;
    if(argc!=2)
    {
        cerr<<"No file name found. Please pass a file name as a parameter."<<endl;
        exit(1);
    }

    IplImage *img,*scrambled;

    CvScalar pix;
    img=cvLoadImage(argv[1]);
    if(img==0)
    {
        cerr<<argv[1]<<": File not found."<<endl;
        exit(1);
    }

    cout<<"Image dimensions: "<<img->height<<" "<<img->width<<endl;
    cout<<"Enter side length of square piece: ";
    scanf("%d",&len);
    
    if(len<1)
    {
        cerr<<"Invalid side length."<<endl;
        exit(1);
    }

    n = min(img->height/len,img->width/len);
    
    height = width = len;

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
    cout<<"Picture Broken into total "<<n*n<<" pieces."<<endl<<endl;
    cout<<"N = "<<n<<endl<<endl;
    return 0;
}