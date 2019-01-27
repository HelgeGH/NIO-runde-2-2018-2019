#define _CRT_SECURE_NO_DEPRECATE
#include <algorithm>
#include <numeric>
#include <iterator>
#include <string>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;
typedef long long		ll;
typedef pair<int, int>	ii;
typedef vector<ii>		vii;
typedef vector<int>		vi;
#define INF 1000000000
#define REP(i, a, b) for (int i = int(a); i < int(b); ++i)

//vi bruker Disjoint-set data strukturen
//kort sagt, så lar den oss holde styr på
//hvilke grupper hvert element er i, og vi
//kan koble sammen ulike grupper og sjekke om
//ulike elementer er i samme gruppe

class UnionFind {
private:
	vi p, rank, setSize;
	int numSets; 
public:
	UnionFind(int N) {
		setSize.assign(N, 1);
		numSets = N;
		rank.assign(N, 0);
		p.assign(N, 0);
		REP(i, 0, N)
			p[i] = i;
	}
	int findSet(int i) {
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			numSets--;
			int x = findSet(i), y = findSet(j);
				if (rank[x] > rank[y]) {
					p[y] = x;
					setSize[x] += setSize[y];
				}
				else {
					p[x] = y;
					setSize[y] += setSize[x];
					if (rank[x] == rank[y])
						rank[y]++;
				}
		}
	}
	int numDisjointSets() { return numSets; }
	int sizeOfSet(int i) { return setSize[findSet(i)]; }
};

int main() {
	//les input
	int N, M, a, b;
	cin >> N >> M;

	//initialiser Union-Find med alle byene
	UnionFind UF(N);

	REP(i, 0, M) {
		cin >> a >> b;
		//dersom Leibniz er i en av gruppene mens Newton er
		//i den andre gruppa som skal sammenslås, så kan
		//ikke dette gjøres
		if (UF.isSameSet(0, a) && UF.isSameSet(1, b) ||
			UF.isSameSet(1, a) && UF.isSameSet(0, b)) {
			cout << "nei" << endl;
		}
		//ellers slår vi de to gruppene sammen
		else {
			cout << "ja" << endl;
			UF.unionSet(a, b);
		}
	}
	system("pause");
	return 0;
}