#include "MST_solver.h"

void MST::fill_greedy(vector<Block> &ans,bool *used)
{
	int CC[X];
	int a,bb,a1,b1;
	minDis top;
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
	priority_queue<minDis> Q;
	for(int i=0;i<X;i++)
	  if(!used[ans[i].idx])
		Q.push(minDis(CC[i],i));

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
			matemp=pieces->getWeight(ans,top.id,pieces->block[i]);
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
				Q.push(minDis(CC[a1*N+b1],a1*N+b1)); 
			}
		}

		ans[top.id] = pieces->block[ind]; 
		used[ind] =1;
	}
}

vector<Block> MST::get_mst(int height,int width)
{
	int u,v,u1,v1,ma;
	vector<Block> ans;
	edges temp;
	priority_queue<edges> Q;
	while(Q.size()) Q.pop();

	int ind=1;
	bool used[X];

	for(int i=0;i<X;i++) used[i]=0;
	vector<pii> cood;
	set<pii> S;
	for(int i=0;i<X;i++) cood.pb(make_pair(INF,INF));

	ans.pb(pieces->block[ind]);
	used[ind]=1;
	cood[ind]=pii(0,0);
	S.insert(pii(0,0));

	for(int i=0;i<X;i++) 
	if(i!=ind)
	{
		Q.push(edges(ind,i,R,pieces->adjr[ind][i]));
		Q.push(edges(ind,i,L,pieces->adjl[ind][i]));
		Q.push(edges(ind,i,T,pieces->adjt[ind][i]));
		Q.push(edges(ind,i,D,pieces->adjd[ind][i]));
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
		if(S.find(pii(u1,v1))!=S.end()) continue;
		if(used[ttop.j]) continue;
		S.insert(pii(u1,v1));
		ans.pb(pieces->block[ttop.j]);
		used[ttop.j]=1;
		cood[ttop.j] = pii(u1,v1);
		for(int i=0;i<X;i++) if(!used[i])
		{
			Q.push(edges(ttop.j,i,R,pieces->adjr[ttop.j][i]));
			Q.push(edges(ttop.j,i,L,pieces->adjl[ttop.j][i]));
			Q.push(edges(ttop.j,i,T,pieces->adjt[ttop.j][i]));
			Q.push(edges(ttop.j,i,D,pieces->adjd[ttop.j][i]));
		}
	}

	ans.clear();
	ind=-1,ma=0;
	for(int i=0;i<X;i++)
	{
		int x=cood[i].first;
		int y=cood[i].second;
		int cnt=0;
		for(int j=0;j<X;j++) if(i!=j)
		{
			if(0<=cood[j].first-x&&0<=cood[j].second-y) if(N>cood[j].first-x&&N>cood[j].second-y) cnt++;
		}
		if(ind==-1||ma<cnt) ind=i,ma=cnt;
	}

	ans.resize(X);
	for(int i=0;i<X;i++) used[i]=0,ans[i] = pieces->dull,ans[i].idx=-1;
	
	int x=cood[ind].first;
	int y=cood[ind].second;

	for(int i=0;i<X;i++)
	{
		if(0<=cood[i].first-x&&0<=cood[i].second-y) if(N>cood[i].first-x&&N>cood[i].second-y)
		{
			used[i]=1;
			int aa=cood[i].first-x;
			int bb=cood[i].second-y;
			ans[aa*N+bb]=pieces->block[i];
		}
	}
	fill_greedy(ans,used);
	return ans;
}
