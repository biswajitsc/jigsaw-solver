#include "GA_solver.h"

int GA::findbuddy(vector<Block> &c, bool * used, int k)
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

vector<Block> GA::crossover(vector<Block> &a, vector<Block> &b)
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
          double matemp=pieces->getWeight(ans,temp,pieces->block[i]);
          if(ind==-1||ma>matemp) ind=i,ma=matemp;
        }
        k=ind;
      }
    }

    used[k]=1;
    ans[temp]=pieces->block[k];
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




double GA::fitness(vector<Block> &c)
{
double ans=0;
for(int i=0;i<X;i++)
{
  if((i+1)%N==0){}
    else
    {
      ans+=pieces->adjr[c[i].idx][c[i+1].idx];
    }
  }
  for(int i=0;i<X-N;i++)
  {
    ans+=pieces->adjd[c[i].idx][c[i+N].idx];
  }
  return ans;
}

vector<vector<Block> > GA::bestGen(vector<vector<Block> > &gen)
{
  priority_queue<Data> minque;
  vector<vector<Block> > answer;
  for(int i=0;i<gen.size();i++)
  {
    double wt=0;
    wt = fitness(gen[i]);
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
    minque.pop();
  }
  return answer;
}

vector<vector<Block> > GA::generation(vector<vector<Block> > &gen,int height,int width)
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
    answer.pb(crossover(answer[r1],answer[r2]));
  }
  return answer;
}


void GA::bestBuddy()
{
  bbl.resize(X);
  bbr.resize(X);
  bbt.resize(X);
  bbd.resize(X);

  for(int i=0;i<X;i++) 
    bbl[i]=-1,bbr[i]=-1,bbt[i]=-1,bbd[i]=-1;

  for(int i=0;i<X;i++)
    for(int j=i+1;j<X;j++)
    {
      int a=-1;
      for(int k=0;k<X;k++) if(k!=i)if(a==-1||pieces->adjl[i][k]<pieces->adjl[i][a]) a=k;

        int b=-1;
      for(int k=0;k<X;k++) if(k!=j)if(b==-1||pieces->adjr[j][k]<pieces->adjr[j][b]) b=k;

        if(a==j&&b==i)
        {
          bbl[i]=j,bbr[j]=i;
        }
        }

  for(int i=0;i<X;i++)
    for(int j=i+1;j<X;j++)
    {
      int a=-1;
      for(int k=0;k<X;k++) if(a==-1||pieces->adjt[i][k]<pieces->adjt[i][a]) a=k;

        int b=-1;
      for(int k=0;k<X;k++) if(b==-1||pieces->adjd[j][k]<pieces->adjd[j][b]) b=k;

        if(a==j&&b==i)
        {
          bbt[i]=j,bbd[j]=i;
        }
    }
}
vector<Block> GA::runAlgo(int height,int width)
{
  vector<vector<Block> > gen;
  vector<Block> temp;
  vector<vector<Block> > answer;

  for(int i=0;i<1000;i++)
  {
    temp.clear();
    for(int j=0;j<X;j++)
    {
      pieces->block[j].idx=j;
      temp.pb(pieces->block[j]);
    }
    for(int j=0;j<X;j++)
    {
      int rnd;
      rnd=rand()%X;
      swap(temp[j],temp[rnd]);
    }
    gen.pb(temp);
  }

  double ttime;
  for(int i=0;i<100;i++)
  {

    ttime = double(clock()-start_time)/(double)(CLOCKS_PER_SEC);
    if(ttime>=TIME_LIMIT)
    {
      answer = bestGen(gen);
      return answer[0];
      // saveResult(answer[0],height,width,"final.jpg");
      // exit(0);
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
      wt+=pieces->getWeight(gen[i],j,gen[i][j]);
    }
    if(i==0||wt<min)
      wt=min,pose=i;
  }

  return gen[pose];
}