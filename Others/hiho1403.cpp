#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

const int maxn = 2e4 + 10;
const int maxs = 110;

int n,k;
int cntA[maxn],cntB[maxn],A[maxn],B[maxn];
int sa[maxn],tsa[maxn],rk[maxn],height[maxn];
int s[maxn];

void SA()
{
	memset(cntA,0,sizeof maxs*sizeof(int));
	for(int i=1;i<=n;++i) cntA[s[i]]++;
	for(int i=1;i<maxs;++i) cntA[i] += cntA[i-1];
	for(int i=n;i;i--) sa[cntA[s[i]]--] = i;

	rk[sa[1]] = 1;
	for(int i=2;i<=n;++i)
	{
		rk[sa[i]] = rk[sa[i-1]];
		if(s[sa[i]] != s[sa[i-1]]) rk[sa[i]]++;
	}

	for(int l=1; rk[sa[n]] < n; l <<= 1)
	{
		memset(cntA,0,sizeof cntA);
		memset(cntB,0,sizeof cntB);

		for(int i=1;i<=n;++i)
		{
			cntA[A[i] = rk[i]] ++;
			cntB[B[i] = (i+l <= n)?rk[i+l]:0] ++;
		}
		for(int i=1;i<=n;++i) cntB[i] += cntB[i-1];
		for(int i=n; i; --i) tsa[cntB[B[i]]--] = i;

		for(int i=1;i<=n;++i) cntA[i] += cntA[i-1];
		for(int i=n;i;--i) sa[cntA[A[tsa[i]]] --] = tsa[i];

		rk[sa[1]] = 1;
		for(int i=2;i<=n;++i)
		{
			rk[sa[i]] = rk[sa[i-1]];
			if(A[sa[i]] != A[sa[i-1]] || B[sa[i]] != B[sa[i-1]]) rk[sa[i]]++;
		}
	}
	for(int i=1,j=0;i<=n;++i)
	{
		if(j) --j;
		while(s[i+j] == s[sa[rk[i]-1]+j]) ++j;
		height[rk[i]] = j;
	}
}

deque<int> q;

int main()
{
	//freopen("input.txt","r",stdin);
	scanf("%d %d",&n,&k);
	for(int i=1;i<=n;++i) scanf("%d",s+i);
	if(k == 1)
	{
		printf("%d\n",n);
		return 0;
	}
	--k;
	SA();
//	for(int i=1;i<=n;++i) printf("%d\n",height[i]);
	int ans = 0;
	for(int i=1;i<=k;++i)
	{
		while(q.size() && q.back() > height[i]) q.pop_back();
		q.push_back(height[i]);
	}
	ans = q.front();
	for(int i=2,j=k+1;j<=n;++i,++j)
	{
		if(height[i-1] == q.front()) q.pop_front();
		while(q.size() && q.back() > height[j]) q.pop_back();
		q.push_back(height[j]);
		if(q.size()) ans = max(ans,q.front());
	}
	printf("%d\n",ans);
	return 0;
}
