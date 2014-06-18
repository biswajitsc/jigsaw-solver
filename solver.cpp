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

typedef std::pair<int,int> pii;
typedef std::pair<pii,int> ppi;

#include <opencv/highgui.h>
#include <opencv/cv.h>
clock_t start_time;
using namespace std;


int N,X;

vector<vector <double> > adjl,adjr,adjt,adjd;
vector<int> bbl, bbr, bbt, bbd;
Block* block;
Block dull;

struct edges
{
  int i,j,id;
  double weight;
  bool operator <(const edges & x)const
  {
    return this->weight>x.weight;
  }
};

struct data
{
  int cc;
  int id;
  bool operator <(const data &x)const
  {
    return this->cc > x.cc;
  }
};

void initializeVector(int n)
{
  adjl.resize(n);
  for(int i=0;i<n;i++)
    adjl[i].resize(n);
  adjr.resize(n);
  for(int i=0;i<n;i++)
    adjr[i].resize(n);
  adjt.resize(n);
  for(int i=0;i<n;i++)
    adjt[i].resize(n);
  adjd.resize(n);
  for(int i=0;i<n;i++)
    adjd[i].resize(n);
}

void assignMemory(int height,int width)
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

void loadImages(int height,int width)
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
    //cout<<"\n"<<str<<"\n";
    //getchar();
    img=cvLoadImage(str);
    block[i].idx=i;
    for(int j=0;j<height;j++)
    {
      for(int k=0;k<width;k++)
      {
        pix=cvGet2D(img,j,k); 
        for(int h=0;h<3;h++)
          block[i].image[j][k].val[h] = pix.val[h];
      }
    }
  }
}


double SSD_right(int sure,int trial,int height,int width)
{
  double ssd=0;

  int i,j;

  for(i=0;i<height;i++)
  {
    for(j=1;j<=limit;j++)
    {
      for(int h=0;h<3;h++)
        ssd = ssd + (block[sure].image[i][width - j].val[h]-block[trial].image[i][j-1].val[h])*(block[sure].image[i][width - j].val[h]-block[trial].image[i][j-1].val[h]);      
    }

  }
  return ssd;
}


double SSD_left(int sure,int trial,int height,int width)
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

double SSD_top(int sure,int trial,int height,int width)
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

double SSD_bottom(int sure,int trial,int height,int width)
{
  double ssd=0;
  int i,j;

  for(i=1;i<=limit;i++)
  {
    for(j=0;j<width;j++)
    {
      for(int h=0;h<3;h++)
        ssd = ssd + (block[sure].image[height - i][j].val[h]-block[trial].image[i-1][j].val[h])*(block[sure].image[height - i][j].val[h]-block[trial].image[i-1][j].val[h]);
    }

  }

  return ssd;
}


void insertInLeftMatrix(int height,int width)
{
  double ssd;
  for(int i=0;i<X;i++)
  {
    for(int j=0;j<X;j++)
    {
      if(i==j)
        continue;
      ssd = SSD_left(i,j,height,width);
      adjl[i][j]=ssd;
      adjr[j][i]=ssd;
    }
  }
}

void insertInRightMatrix(int height,int width)
{
  double ssd;
  for(int i=0;i<X;i++)
  {
    for(int j=0;j<X;j++)
    {
      if(i==j)
        continue;
      ssd = SSD_right(i,j,height,width);
      adjr[i][j]=ssd;
    }
  }
}

void insertInTopMatrix(int height,int width)
{
  double ssd;
  for(int i=0;i<X;i++)
  {
    for(int j=0;j<X;j++)
    {
      if(i==j)
        continue;
      ssd = SSD_top(i,j,height,width);
      adjt[i][j]=ssd;
      adjd[j][i]=ssd;
    }
  }
}

void insertInBottomMatrix(int height,int width)
{
  double ssd;
  for(int i=0;i<X;i++)
  {
    for(int j=0;j<X;j++)
    {
      if(i==j)
        continue;
      ssd = SSD_bottom(i,j,height,width);
      adjd[i][j]=ssd;
    }
  }
}

int xx[]={0,1,0,-1};
int yy[]={1,0,-1,0};

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



int findbuddy(vector<Block> &c, bool * used, int k)
{
  int a=k/N,bb=k%N;
  for(int i=0;i<4;i++)
  {
    int a1=a+xx[i],b1=bb+yy[i];
    if(a1<0||a1>=N) continue;
    if(b1<0||b1>=N) continue;

    if(c[a1*N+b1].idx==-1) continue;
    if(i==0) {
      int l = bbl[c[a1*N+b1].idx];
      if(!used[l]) return l;
    }
    if(i==1) {
      int l = bbt[c[a1*N+b1].idx];
      if(!used[l]) return l;
    }
    if(i==2) {
      int l = bbr[c[a1*N+b1].idx];
      if(!used[l]) return l;
    }
    if(i==3) {
      int l = bbd[c[a1*N+b1].idx];
      if(!used[l]) return l;
    }
  }
  return -1;
}

vector<Block> crossover(vector<Block> &a, vector<Block> &b)
{
  bool* vis = new bool[X];
  bool* used = new bool[X];
  queue<int> boundary;
  vector<Block> ans(X);
  for(int i=0;i<X;i++)
  {
    vis[i]=0;
    used[i]=0;
    ans[i].idx=-1;
  }

  for(int i=0;i<X;i++)
  {
    if(a[i].idx==b[i].idx)
    {
      vis[i]=1;
      boundary.push(i);
      break;
    }
  }

  double ma;
  int ind;

  if(boundary.size()==0) boundary.push(rand()%X);

  vis[boundary.front()]=1;

  while(boundary.size())
  {
    int temp=boundary.front();
    int k;
    boundary.pop();
    if(a[temp].idx==b[temp].idx&&!used[a[temp].idx])
    {
      ind=a[temp].idx;
      k=ind;
    }
    else if((k=findbuddy(ans,used,temp))!=-1)
    {

    }
    else
    {
      ind=-1;
      for(int i=0;i<X;i++)
      {
        if(!used[i])
        {
          double matemp=getWeight(ans,temp,block[i]);
          if(ind==-1||ma>matemp) ind=i,ma=matemp;
        }
        k=ind;
      }
    }

    used[k]=1;
    ans[temp]=block[k];
    int aa,bb;
    bb=temp%N;
    aa=temp/N;
    for(int i=0;i<4;i++)
    {
      int a1=aa+xx[i],b1=bb+yy[i];
      if(a1<0||a1>=N) continue;
      if(b1<0||b1>=N) continue;
      if(vis[a1*N+b1]) continue;
      boundary.push(a1*N+b1);
      vis[a1*N+b1]=1;
    }
  }

  for(int i=0;i<X;i++)
  {
    assert(ans[i].idx<X&&ans[i].idx>=0);
  }
  
  return ans;
}


void writeToFile(vector<Block> &ans)
{
  FILE *fp;
  fp=fopen("Output.txt","w");
  for(int i=0;i<N;i++)
  {
    for(int j=0;j<N;j++)
    {
      if(ans[i*N +j].idx==-1)
        fprintf(fp, "%d\t",ans[i*N+j].idx);
      else  
        fprintf(fp, "%d\t",ans[i*N+j].idx+1);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
}

void saveResult(vector<Block> &ans,int height,int width,const char *output)
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
//Block[i][j] = new int*[Z];
      for(k = 0; k<width;k++)
      {
        for(h =0; h<3;h++)
          pix.val[h]=ans[i].image[j][k].val[h];
        cvSet2D(final,j+start,k+stop,pix);
      }
    }
  }
  cvSaveImage(output,final);
}

edges asign(const int  i,const int j,const int id,const double w)
{
  edges temp;
  temp.i = i,temp.j = j,temp.id = id,temp.weight = w;
  assert(i>=0 && i<X);
  assert(j>=0 && j<X);
  assert(id>=0 && id<4);
  return temp;
}

void debug_edge(edges q)
{
  printf("\n %d %d %d %lf",q.i,q.j,q.id,q.weight);
}

data asign_data(int c,int i)
{
  data temp;
  temp.cc = c;
  temp.id = i;
  return temp;
}


double fitness(vector<Block> &c)
{
double ans=0;
for(int i=0;i<X;i++)
{
  if((i+1)%N==0){}
    else
    {
      ans+=adjr[c[i].idx][c[i+1].idx];
    }
  }
  for(int i=0;i<X-N;i++)
  {
    ans+=adjd[c[i].idx][c[i+N].idx];
  }
  return ans;
}

struct Data
{
  double wt;
  int ind;
  bool operator < (const Data &x) const{
    return wt<x.wt;
  }
};

double cwt=10000.0,lwt=-1.0;

vector<vector<Block> > bestGen(vector<vector<Block> > &gen)
{
  priority_queue<Data> minque;
  vector<vector<Block> > answer;
  for(int i=0;i<gen.size();i++)
  {
    double wt=0;
//printf("At line %d \n",__LINE__ );
    wt = fitness(gen[i]);
// printf("At line %d \n",__LINE__ );
    Data temp;
    temp.wt=wt;
    temp.ind = i;
    minque.push(temp);
    if(minque.size() > 4)
      minque.pop();
  }
  while(minque.size())
  {
    answer.pb(gen[minque.top().ind]);
    cwt=minque.top().wt;
    minque.pop();
  }
  return answer;
}


void testing(vector<Block> ans,const char *output,bool fill)
{
  set<ppi> S;
  int count1=0;
  int count = 0;
  pii insertedge,tempedge;
  ppi insertorient,temporient;
  ofstream out;
  out.open(output,ofstream::app);
  for(int j=0;j<X;j++)
  {
    int a=j/N,bb=j%N;
    for(int i=0;i<4;i++)
    {
      int a1=a+xx[i],b1=bb+yy[i];
      if(a1<0||a1>=N) continue;
      if(b1<0||b1>=N) continue;
      insertedge.first = j;
      insertedge.second = a1*N+b1;
      insertorient.first=insertedge;
      if(i==0) insertorient.second=R;
      if(i==1) insertorient.second=D;
      if(i==2) insertorient.second=L;
      if(i==3) insertorient.second=T;
      S.insert(insertorient);
      count1++;
    }
  }
//printf("Edge count = %d\n",count);
  count=0;
  int cdull = 0;
  for(int j=0;j<X;j++)
  {
    int a=j/N,bb=j%N;
    for(int i=0;i<4;i++)
    {
      int a1=a+xx[i],b1=bb+yy[i];
      if(a1<0||a1>=N) continue;
      if(b1<0||b1>=N) continue;
      tempedge.first = ans[j].idx;
      tempedge.second = ans[a1*N+b1].idx;
      temporient.first=tempedge;
      if(i==0) temporient.second=R;
      if(i==1) temporient.second=D;
      if(i==2) temporient.second=L;
      if(i==3) temporient.second=T;
      if(fill)
      {
        if(S.find(temporient)!=S.end())
          count++;
      }
      else
      {
        if(tempedge.first==-1 || tempedge.second==-1)
          cdull++;
        else
        {
          if(S.find(temporient)!=S.end())
            count++;
        }
      }
    }
  }
  count/=2;count1/=2;cdull/=2;
  if(fill)
    out<<"The value fo count is: "<<count<<"// "<<count1<<".  Score  = "<<100*count - 40*(count1-count)<<"\n";
  else
  {
    out<<"The value fo count is: "<<count<<"// "<<count1<<".  Score  = "<<100*count - 40*(count1 - count - cdull)<<"\n";
    assert(count1-count-cdull >=0);
  }

  out.close();
}


void fill_greedy(vector<Block> &ans,bool *used)
{
int CC[X];
int a,bb,a1,b1;
data top;
int ind;
double matemp,ma;

for(int i=0;i<X;i++)
  CC[i] = 0;

for(int i=0;i<X;i++)
{
  a=i/N,bb=i%N;

  for(int j=0;j<4;j++)
  {
    a1=a+xx[j],b1=bb+yy[j];
    if(a1<0||a1>=N) continue;
    if(b1<0||b1>=N) continue;
    if(ans[a1*N+b1].idx!=-1)
      CC[i]++;
  }
}
priority_queue<data> Q;
for(int i=0;i<X;i++)if(!used[ans[i].idx])
  Q.push(asign_data(CC[i],i));

while(Q.size())
{
  top = Q.top();
  Q.pop();
  if(ans[top.id].idx!=-1) 
    continue;
  ind=-1;
  ma = 0;
  for(int i=0;i<X;i++)
    if(!used[i])
    {
      matemp=getWeight(ans,top.id,block[i]);
      if(ind==-1||ma>matemp) ind=i,ma=matemp;
    }

    a=top.id/N,bb=top.id%N;
    for(int j=0;j<4;j++)
    {
      a1=a+xx[j],b1=bb+yy[j];
      if(a1<0||a1>=N) continue;
      if(b1<0||b1>=N) continue;
      if(ans[a1*N+b1].idx==-1)
      {
        CC[a1*N+b1]++;
        Q.push(asign_data(CC[a1*N+b1],a1*N+b1)); 
      }
    }

    ans[top.id] = block[ind]; 
    used[ind] =1;

  }
}


vector<Block> mst(int height,int width)
{
int u,v,u1,v1,ma;
vector<Block> ans;
edges temp;
priority_queue<edges> Q;
while(Q.size())
  Q.pop();

int ind=1;
bool used[X];

for(int i=0;i<X;i++) used[i]=0;
  vector<pii > cood;
set<pii> S;
for(int i=0;i<X;i++) cood.pb(make_pair(INF,INF));


  ans.pb(block[ind]);
used[ind]=1;
cood[ind]=pii(0,0);
S.insert(pii(0,0));

for(int i=0;i<X;i++) 
  if(i!=ind)
  {
    Q.push(asign(ind,i,R,adjr[ind][i]));

    Q.push(asign(ind,i,L,adjl[ind][i]));

    Q.push(asign(ind,i,T,adjt[ind][i]));

    Q.push(asign(ind,i,D,adjd[ind][i]));
  }

  int cc=0;
  edges ttop;
  while(ans.size()<X && !Q.empty())
  {
    cc++;
    ttop=Q.top();
    Q.pop();

    u=cood[ttop.i].first;
    v=cood[ttop.i].second;

    u1=u;
    v1=v;
    if(ttop.id==R) v1++;
    if(ttop.id==L) v1--;
    if(ttop.id==T) u1--;
    if(ttop.id==D) u1++;

// printf("\n %d %d *",u1,v1);
    if(S.find(pii(u1,v1))!=S.end()) 
      continue;
    if(used[ttop.j])
      continue;
// printf("%d-->%d %d",ttop.i,ttop.j,ttop.id);
    S.insert(pii(u1,v1));
    ans.pb(block[ttop.j]);
    used[ttop.j]=1;
    cood[ttop.j] = pii(u1,v1);
    for(int i=0;i<X;i++) if(!used[i])
    {
      Q.push(asign(ttop.j,i,R,adjr[ttop.j][i]));

      Q.push(asign(ttop.j,i,L,adjl[ttop.j][i]));

      Q.push(asign(ttop.j,i,T,adjt[ttop.j][i]));

      Q.push(asign(ttop.j,i,D,adjd[ttop.j][i]));
    }
  }
//printf("\n %d",(int)ans.size());
  ans.clear();
  ind=-1,ma=0;
  for(int i=0;i<X;i++)
  {
    int x=cood[i].first;
    int y=cood[i].second;
    int cnt=0;
    for(int j=0;j<X;j++) if(i!=j)
    {
      if(0<=cood[j].first-x&&0<=cood[j].second-y)
        if(N>cood[j].first-x&&N>cood[j].second-y)
          cnt++;
      }
      if(ind==-1||ma<cnt) ind=i,ma=cnt;
    }

    ans.resize(X);
    for(int i=0;i<X;i++) used[i]=0,ans[i] = dull,ans[i].idx=-1;
      int x=cood[ind].first;
    int y=cood[ind].second;

    for(int i=0;i<X;i++)
    {
      if(0<=cood[i].first-x&&0<=cood[i].second-y)
        if(N>cood[i].first-x&&N>cood[i].second-y)
        {
          used[i]=1;
          int aa=cood[i].first-x;
          int bb=cood[i].second-y;
          ans[aa*N+bb]=block[i];
        }
      }


//testing(ans,"testit.txt",false);
saveResult(ans,height,width,"finala.jpg");
 fill_greedy(ans,used);
//testing(ans,"testit.txt",true);
      return ans;
    }

vector<vector<Block> > generation(vector<vector<Block> > &gen,int height,int width)
{
  vector<vector<Block> > answer;
  answer = bestGen(gen);

  while(answer.size()<1000)
  {
    int r1=0,r2=0;
    while(r1==r2)
    {
      r1=rand()%(int)answer.size();
      r2=rand()%(int)answer.size();
    }
//r2=r1+1;
    answer.pb(crossover(answer[r1],answer[r2]));
//r1++;
  }
  return answer;
}

vector<Block> runAlgo(int height,int width)
{
  vector<vector<Block> > gen;
  vector<Block> temp;
  vector<vector<Block> > answer;

  for(int i=0;i<1000;i++)
  {
    temp.clear();
    for(int j=0;j<X;j++)
    {
      block[j].idx=j;
      temp.pb(block[j]);
    }
    for(int j=0;j<X;j++)
    {
      int rnd;
      rnd=rand()%X;
      swap(temp[j],temp[rnd]);
    }
    gen.pb(temp);
  }

// printf("1000 generated!!\n");
  double ttime;
  for(int i=0;i<100;i++)
  {

    ttime = double(clock()-start_time)/(double)(CLOCKS_PER_SEC);
//printf("Iterator at i = %d %lf\n",i,ttime );
//answer = bestGen(gen);
//saveResult(answer[0],height,width,"final.jpg");
// printf("\n %d : %lf",i,ttime);
//  sleep(1);
// getchar();
    if(ttime>=TIME_LIMIT)
    {
      answer = bestGen(gen);
      saveResult(answer[0],height,width,"final.jpg");
      writeToFile(answer[0]);
      exit(0);
    }
    gen = generation(gen,height,width);
  }
  int pose=0;
  double min=0;
  for(int i=0;i<1000;i++)
  {
    double wt=0;

    for(int j=0;j<X;j++)
    {
      wt+=getWeight(gen[i],j,gen[i][j]);
    }
    if(i==0||wt<min)
      wt=min,pose=i;
  }

  return gen[pose];
}


void bestBuddy()
{
  bbl.resize(X);
  bbr.resize(X);
  bbt.resize(X);
  bbd.resize(X);

  for(int i=0;i<X;i++) bbl[i]=-1,bbr[i]=-1,bbt[i]=-1,bbd[i]=-1;

    for(int i=0;i<X;i++)
      for(int j=i+1;j<X;j++)
      {
        int a=-1;
        for(int k=0;k<X;k++) if(k!=i)if(a==-1||adjl[i][k]<adjl[i][a]) a=k;

          int b=-1;
        for(int k=0;k<X;k++) if(k!=j)if(b==-1||adjr[j][k]<adjr[j][b]) b=k;

          if(a==j&&b==i)
          {
            bbl[i]=j,bbr[j]=i;
          }
        }

        for(int i=0;i<X;i++)
          for(int j=i+1;j<X;j++)
          {
            int a=-1;
            for(int k=0;k<X;k++) if(a==-1||adjt[i][k]<adjt[i][a]) a=k;

              int b=-1;
            for(int k=0;k<X;k++) if(b==-1||adjd[j][k]<adjd[j][b]) b=k;

              if(a==j&&b==i)
              {
                bbt[i]=j,bbd[j]=i;
              }
            }

          }



int main(int argc, char *argv[])
{

//Declare variables

  IplImage* img,*gray,*img1;
  IplImage* gray_x,*gray_y,*abs_gray_x,*abs_gray_y,*final_grad,*magnitude,*angle;
  int k,i,j,h,start,stop,l,height,width;
  CvScalar pixel,pix,pix2;


  img = cvLoadImage("generated_pieces/1.jpg", CV_LOAD_IMAGE_COLOR);
  printf("Enter The Value of N in NxN : \n");
  scanf("%d",&N);
  // assert(argc==2);
  // stringstream ss(argv[1]);
//   // ss>>N;
  X=N*N;
  start_time=clock();
  height = img->height;
  width = img->width;
  vector<Block> ans;
  vector<Block> ans_mst;
  initializeVector(X);
  assignMemory(height,width);
  loadImages(height,width);
  insertInTopMatrix(height,width);
//insertInBottomMatrix(height,width);
  insertInLeftMatrix(height,width);
//insertInRightMatrix(height,width);
//double score_mst=0,score_gen=0;
//printf("Insert done!!\n");

  if(N>20)
  {
    ans=mst(height,width);
   saveResult(ans,height,width,"final.jpg");
  }
  else if(N<=20)
  {
    bestBuddy();
    ans=runAlgo(height,width);
//  testing(ans,"testit.txt",false);
//   saveResult(ans,height,width,"final.jpg");
  }

  writeToFile(ans);
  return 0;
}

