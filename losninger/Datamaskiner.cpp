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

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	//les inn N
	int N, A;
	cin >> N;

	//vi holder oversikt over avhengighetene til hver
	//teknologi, og marker dem som "besøkt" (utforsket)
	//når vi besøker dem
	map<string, vector<string> > neighbor;
	map<string, bool> visited;

	//fyll naboliste med input
	REP(i, 0, N) {
		string name;
		cin >> name >> A;
		REP(i, 0, A) {
			string temp;
			cin >> temp;
			neighbor[name].push_back(temp);
		}
		//dette trengs vel egentlig ikke, men jeg tar det med uansett
		visited[name] = false;
	}

	//finn alle dependecies
	queue<string> Q;
	Q.push("datamaskiner");

	//vi gjør et slags bredde først søk der vi bygger oss ned til
	//basisteknologiene fra toppen av
	while (!Q.empty()) {
		//hent øverste element
		string top = Q.front();
		Q.pop();

		//allerede besøkt
		if (visited[top])
			continue;

		visited[top] = true;
		
		//gå gjennom alle naboene
		vector<string> n = neighbor[top];
		for (int i = 0; i < n.size(); ++i) {
			string v = n[i];
			//finn ut hva de trenger senere
			Q.push(v);
		}
	}

	//se hvor mange teknolgiene vi måtte besøke
	int ans = 0;
	for (auto it = visited.begin(); it != visited.end(); ++it) {
		if (it->second)
			++ans;
	}

	//print svaret
	cout << ans << endl;
	system("pause");
	return 0;
}