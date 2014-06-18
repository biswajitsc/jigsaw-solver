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
#include "MST_solver.cpp"

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

vector<int> bbl, bbr, bbt, bbd;

Images pieces;

// struct edges
// {
//   int i,j,id;
//   double weight;
//   bool operator <(const edges & x)const
//   {
//     return this->weight>x.weight;
//   }
// };

// struct data
// {
//   int cc;
//   int id;
//   bool operator <(const data &x)const
//   {
//     return this->cc > x.cc;
//   }
// };


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
          double matemp=pieces.getWeight(ans,temp,pieces.block[i]);
          if(ind==-1||ma>matemp) ind=i,ma=matemp;
        }
        k=ind;
      }
    }

    used[k]=1;
    ans[temp]=pieces.block[k];
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
      ans+=pieces.adjr[c[i].idx][c[i+1].idx];
    }
  }
  for(int i=0;i<X-N;i++)
  {
    ans+=pieces.adjd[c[i].idx][c[i+N].idx];
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
      pieces.block[j].idx=j;
      temp.pb(pieces.block[j]);
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
      wt+=pieces.getWeight(gen[i],j,gen[i][j]);
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
        for(int k=0;k<X;k++) if(k!=i)if(a==-1||pieces.adjl[i][k]<pieces.adjl[i][a]) a=k;

          int b=-1;
        for(int k=0;k<X;k++) if(k!=j)if(b==-1||pieces.adjr[j][k]<pieces.adjr[j][b]) b=k;

          if(a==j&&b==i)
          {
            bbl[i]=j,bbr[j]=i;
          }
        }

        for(int i=0;i<X;i++)
          for(int j=i+1;j<X;j++)
          {
            int a=-1;
            for(int k=0;k<X;k++) if(a==-1||pieces.adjt[i][k]<pieces.adjt[i][a]) a=k;

              int b=-1;
            for(int k=0;k<X;k++) if(b==-1||pieces.adjd[j][k]<pieces.adjd[j][b]) b=k;

              if(a==j&&b==i)
              {
                bbt[i]=j,bbd[j]=i;
              }
            }

          }



int main(int argc, char *argv[])
{
  start_time=clock();
  vector<Block> ans;
  vector<Block> ans_mst;
  pieces.initializeAll();
  N=pieces.N;
  X=N*N;
//double score_mst=0,score_gen=0;
  // if(N>20)
  // {
    MST mst(N,&pieces);
    ans=mst.get_mst(pieces.height,pieces.width);
   saveResult(ans,pieces.height,pieces.width,"final.jpg");
//   }
//   else if(N<=20)
//   {
//     bestBuddy();
//     ans=runAlgo(height,width);
// //  testing(ans,"testit.txt",false);
//   saveResult(ans,height,width,"final.jpg");
//   }

  // writeToFile(ans);
  return 0;
}

