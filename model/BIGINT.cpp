#include<iostream>
#include<cstring>
#include<vector>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<cassert>

using namespace std;
#define LEN(n) (n!=0)?(int)log10(n)+1:0

class BIGINT{
public:
	static const int BASE=1e8;
	static const int WIDE=8;
    vector<int> s;

	BIGINT(long long num=0){*this=num;}

	BIGINT operator=(long long num){
		s.clear();
		do
		{
			s.push_back(num%WIDE);
			num/=BASE;
		}while(num);
		return *this;
	}
    BIGINT operator=(const string& str){
		s.clear();
		int x,len=(str.size()-1)/WIDE + 1;
		for(int i=0;i<len;i++){
			int end=str.size()-i*WIDE;
			int begin=max(0,end-WIDE);
			sscanf(str.substr(begin,end-begin).c_str(),"%d",&x);
			s.push_back(x);
		}
		return *this;
	}

  BIGINT operator-(){				//negative 默认不使用，一般用于输出负数。
    s[s.size()-1]=-s[s.size()-1];
    return *this;
	}

	bool operator<(const BIGINT b) const{
		if(s.size()!=b.s.size()) return s.size()<b.s.size();
		int i=s.size();
		while(--i>=0) if(s[i]!=b.s[i]) return s[i]<b.s[i];
		return 0;
	}
	bool operator>=(const BIGINT b) const{return !(*this<b);}
	bool operator>(const BIGINT b)  const{return b<*this;}
	bool operator<=(const BIGINT b) const{return !(b<*this);}
	bool operator!=(const BIGINT b) const{return b<*this||*this<b;}
	bool operator==(const BIGINT b) const{return !(b!=*this);}

	BIGINT operator+(const BIGINT& b) const {
		BIGINT c;
		c.s.clear();
		for(int i=0,g=0; ;i++){
			if(g==0 && i>=s.size() && i>=b.s.size()) break;
			int x=g;
			if(i<s.size()) x+=s[i];
			if(i<b.s.size()) x+=b.s[i];
			c.s.push_back(x % BASE);
			g=x/BASE;
		}
		return c;
	}
	BIGINT operator+(const int& b)const {
		BIGINT c;
		c.s.clear();
		c.s.push_back(b);
		return *this+c;
	}
	BIGINT operator+=(const BIGINT& b){
		*this=*this+b;
		return *this;
	}
	BIGINT operator+=(const int& b){
		*this=*this+b;
		return *this;
	}
	BIGINT operator-(const BIGINT& b) const {
		BIGINT c;
		c.s.clear();
		vector<int>::const_iterator it;
		if(*this>b){
		   	for(it=b.s.begin();it!=b.s.end();++it)
				c.s.push_back(-(*it));
			c=c+*this;
			for(int i=0;i<c.s.size()-1;i++)
        if(c.s[i]<0){c.s[i]+=BASE;c.s[i+1]-=1;}
		}else if(*this<b){
		   	for(it=s.begin();it!=s.end();++it)
				c.s.push_back(-(*it));
			c=c+b;
			for(int i=0;i<c.s.size()-1;i++)
        if(c.s[i]<0){c.s[i]+=BASE;c.s[i+1]-=1;}
		}
		if(*this==b) c.s.push_back(0);
		int len;
		for(int i=c.s.size();i>0;i--)
			if(c.s[i]==0) len=i;
			else break;
		c.s.resize(len);
		return c;
	}
	BIGINT operator-(const int &b)const {
		BIGINT c;
		c.s.clear();
		c.s.push_back(b);
		return *this-c;
	}
	BIGINT operator-=(const BIGINT& b) {
		*this=*this-b;
		return *this;
	}
	BIGINT operator-=(const int &b){
		*this=*this-b;
		return *this;
	}
	BIGINT operator*(const BIGINT& b) const {
		BIGINT c;
		c.s.clear();
		c.s.resize(s.size()+b.s.size(),0);
		for(int i=0;i<s.size();i++)
			for(int j=0;j<b.s.size();j++){
        		long long t=(long long)s[i]*(long long)b.s[j];
				c.s[i+j+1]=c.s[i+j+1]+(c.s[i+j]+t)/BASE;
				c.s[i+j]=(c.s[i+j]+t)%BASE;
			}
		int len=c.s.size();
		while(--len) if(c.s[len]) break;
		c.s.resize(len+1);
		return c;
	}
	BIGINT operator*(const int& b) const {
		BIGINT c;
		c.s.clear();
		c.s.push_back(b);
		return *this*c;
	}
	BIGINT operator*=(const BIGINT &b){
		*this=*this*b;
		return *this;
	}
	BIGINT operator*=(const int &b){
		*this=*this*b;
		return *this;
	}
	BIGINT operator/(const int& b) const {
		BIGINT c;
		c.s.clear();
		c.s.resize(s.size(),0);
		assert(b!=0);	
		long long t=s[s.size()-1];
		for(int i=s.size()-1;i>=0;i--){
			if(i==0) {c.s[i]=t/b;break;}
			c.s[i]=t/b;
			t=(long long)((long long)(t%b)*BASE+s[i-1]);
		}
		int len=c.s.size();
		while(--len) if(c.s[len]) break;
		c.s.resize(len+1);
		return c;
    }
	BIGINT operator/(const BIGINT& b) const {
		BIGINT c;
		c.s.clear();
		if(*this<b){c.s.push_back(0); return c;}
		if(*this==b){c.s.push_back(1);return c;}
		BIGINT low=1,high=*this,mid=(high+low)/2;
		int t=0;
		while(high>low){
			BIGINT a=mid*b;
			if(a<=*this && *this-a<b) break;
			if(a>*this) {high=mid;mid=(high+low)/2;}
			if(a<*this && *this-a>=b) {low=mid;mid=(high+low+1)/2;}
			//cout<<low.s[0]<<" "<<mid.s[0]<<" "<<high.s[0]<<endl;
		}
		c=mid;
		int len=c.s.size();
		while(--len) if(c.s[len]) break;
		c.s.resize(len+1);
		return c;
	}
	BIGINT operator/=(const int& b){
		*this=*this/b;
		return *this;
	}
	BIGINT operator/=(const BIGINT& b){
		*this=*this/b;
		return *this;
	}
	BIGINT operator%(const int &b){
		return *this-(*this/b)*b;
	}
	BIGINT operator%(const BIGINT& b){
		return *this-(*this/b)*b;
	}
};

BIGINT power(const BIGINT &a,const int &b) {
	BIGINT c;
	c.s.clear();
	if(b==0) {c.s.push_back(1);return c;}
	c.s.resize((int)pow(a.s.size(),b),0);
	for(int i=0;i<a.s.size();i++) c.s[i]=a.s[i];
	for(int i=1;i<b;i++) c*=a;
	int len=c.s.size();
	while(--len) if(c.s[len]) break;
	c.s.resize(len+1);
	return c;
}

ostream& operator << (ostream &out,const BIGINT &x){
	for(int i=x.s.size()-1;i>=0;i--){
		char s[30];
		if(i==x.s.size()-1) sprintf(s,"%d",x.s[i]);
		else sprintf(s,"%08d",x.s[i]);
		for(int j=0;j<strlen(s);j++) out<<s[j];
	}
	return out;
}

istream& operator >> (istream& in,BIGINT& x){
	string s;
	if(!(in>>s)) return in;
	x=s;
	return in;
}



int main()
{
	//freopen("bigint.in","r",stdin);
	BIGINT x;
	BIGINT y;
	cin>>x>>y;
	cout<<(x%y);
	return 0;
}

