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

//gir manhatten avstanden mellom punkt a og b
int dist(ii a, ii b) {
	return abs(a.first - b.first) + abs(b.second - a.second);
}

int main() {
	//les inn
	int N;
	cin >> N;

	//les inn alle punktene
	vii pizzas(N);
	REP(i, 0, N)
		cin >> pizzas[i].first >> pizzas[i].second;

	//det tar 15 min å spise på det første stedet
	ll ans = 15;
	REP(i, 1, N) {
		//det tar 15 min å spise + tiden for å reise fra
		//det forrige pizzatedet til det nåværende
		ans += 15 + dist(pizzas[i - 1], pizzas[i]);
	}

	//print svaret
	cout << ans << endl;

	system("pause");
	return 0;
}