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
#define INF 1000000000000
#define MOD 104729
#define REP(i, a, b) for (int i = int(a); i < int(b); ++i)

vector<vector<ll> > memo(501, vector<ll>(501, 0));	
vector<ll> ror_sum(501);
vector<ll> ror(501);

//inclusiv start og end
ll DP(int start, int end) {
	//det koster ikke noe å skaffe et startrør
	if (start == end) {
		return 0;
	}

	//vi har allerde ut minste pris for dette
	if (memo[start][end] > 0)
		return memo[start][end];

	//vi finner prisen for å lage et sammenhengende rør av [start, end]
	ll cur_cost;
	if (start == 0)
		cur_cost = (ror_sum[end] * 311) % MOD;
	else
		//det tar lang tid å summere rørene i mengden [start, end],
		//så denne er gitt ved  sum([0, end] ) - sum([0, start - 1])
		cur_cost =
			(((ror_sum[end] - ror_sum[start - 1]) % MOD) * 311) % MOD;

	//vi ser hvilke to subset av rør som er best å sveise sammen
	ll min_cost = INF;
	//vi prøver å sveise sammen rør [start, i] og [i + 1, end]
	REP(i, start, end) {
		//og velger det billigste av disse
		min_cost = min(min_cost,
			DP(start, i) + DP(i + 1, end) + cur_cost);
	}

	//lagrer og returnerer minste svar
	memo[start][end] = min_cost;
	return memo[start][end];
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	//input
	int N;
	cin >> N;

	ll sum = 0;

	REP(i, 0, N) {
		cin >> ror[i];

		//base case
		memo[i][i] = 0;

		//vi fyller ut en sum av ror i [0, i] for rask summering i DP
		sum += ror[i];
		ror_sum[i] = sum;
	}
	
	//top down DP solution
	//denne løsningen gir også riktig svar, men siden 
	//vi besøker alle statsene uansett, så bruker den lenger 
	//tid pga. mye rekursjonsstacking

	//cout << DP(0, N - 1) << endl;

	//bottom up DP solution
	//raskere fordi den løser det iterativt
	REP(l, 2, N + 1) {			//lengden på røret
		REP(i, 0, N - l + 1) {	//start inclusive
			int start = i, end = i + l - 1;	//indexer (inklusive)
			
			//kostnad for røret
			ll cur_cost;
			if (start == 0)
				cur_cost = (ror_sum[end] * 311) % MOD;
			else
				cur_cost =
				//se kommentar i DP
				(((ror_sum[end] - ror_sum[start - 1])) * 311) % MOD;

			//finn neste minste kostand
			ll min_cost = INF;
			REP(j, start, end) {
				min_cost = min(min_cost, 
					//se kommentar i DP
					memo[start][j] + memo[j + 1][end] + cur_cost);
			}
			//lagre svar
			memo[start][end] = min_cost;
		}
	}

	//print svaret
	cout << memo[0][N - 1] << endl;

	system("pause");
	return 0;
}