#include "generate_pieces.h"

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
        cout<<argv[1]<<": File not found."<<endl;
        exit(1);
    }

    cout<<"Image dimensions: "<<img->height<<" "<<img->width<<endl;
    cout<<"Enter side length of square grid: ";
    scanf("%d",&len);
    
    if(len<1)
    {
        cerr<<"Invalid Side Length. "<<endl<<"Aborting "<<endl;
        exit(1);
    }

    n = min(img->height/len,img->width/len);
    
    height = width = len;

    block = assignMemory(height,width,n*n);
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
    cerr<<"Picture Broken into total "<<n*n<<" pieces."<<endl;
    return 0;
}