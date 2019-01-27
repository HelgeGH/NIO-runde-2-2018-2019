#define _CRT_SECURE_NO_DEPRECATE
#include <algorithm>
#include <numeric>
#include <iterator>
#include <string>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <ctime>
#include <queue>
using namespace std;
typedef long long		ll;
typedef pair<int, int>	ii;
typedef vector<ii>		vii;
typedef vector<int>		vi;
#define INF 1000000000
#define REP(i, a, b) for (int i = int(a); i < int(b); ++i)

//vi bruker globale variabler pga. alle funksjonene
int N, K, R;
vector<vi> neighbours(16000);

//binomialformel med memoisering
int memo_bin[21][6];
int binom(int n, int r) {
	//stored
	if (memo_bin[n][r] > 0)
		return memo_bin[n][r];

	//n choose r == n choose (n - r)
	if (r > n / 2)
		r = n - r;

	int ans = 1;

	//80 choose 10 gir f.eks. (71/1 * 72/2 * 73/3 ...)
	for (int i = 1; i <= r; i++) {
		ans *= n - r + i;
		ans /= i;
	}
	
	//lagre og returner
	return memo_bin[n][r] = ans;
}

//vi vil gi alle kombinasjonene av spillere en
//spesefik ID s.a. alle IDene utgjør en sammenhengende sekvens
//av de ((N over K) - 1) første naturlige tallene (med 0)
int get_id(const vi & v) {
	//border case :(((((((
	if (K == 1)
		return v[0];

	int ret = 0;

	//base case for første spiller
	REP(p, 0, v[0])
		ret += binom(N - p - 1, K - 1);

	//går gjennom alle spillerne
	REP(i, 1, K - 1) {	//K=v.size()
		//går igjennom alle tallene i [a_(i-1) + 1, a_i - 1]
		REP(p, v[i - 1] + 1, v[i]) {
			ret += binom(N - p - 1, K - (i + 1)); //1 indeksert i
		}
	}

	//spesialtilfelle for det siste tallet
	ret += (v[K - 1] - v[K - 2] - 1);

	return ret;
}

//o er hvor vi starter (offset) og k er elementer vi skal velge, N = antall spillere
//vi finner alle kombinasjonene av de (N over K) lagene
void comb(vector<vi> & combs, int k, int N, int o = 0, vi cur = {}) {
	//når vi kommer til k==0, så er en av kombinasjonene klare
	if (k == 0) {
		int id = get_id(cur);

		//vi lagrer alle naboene til denne spillersammensetningen
		REP(k, 0, K) {		//bytter ut spiller k
			REP(l, 0, N) {	//med spiller l
							//vi sjekker om spilleren ikke alt er på laget vårt (om gyldig bytte)
				bool is_possible = true;
				REP(m, 0, K) {
					if (l == cur[m])
						is_possible = false;
				}
				//hvis det går, så lagrer vi naboenes ID
				if (is_possible) {
					vi new_team = cur;
					new_team[k] = l;

					//get_id krever en sortert sammensetning
					sort(new_team.begin(), new_team.end());

					//legger til naboene
					neighbours[id].push_back(get_id(new_team));
				}
			}
		}
		combs.push_back(cur);
		return;
	}

	//vi sørger for at vi ikke går for langt ut. (eg. velge maks 4. element hvis N=5 og k=2)
	REP(i, o, N - k + 1) {
		cur.push_back(i);
		comb(combs, k - 1, N, i + 1, cur);
		cur.pop_back();
	}
}

//lager de (N choose K) mulighetene vi har av spillere
vector<vi> generate_indices(int N, int K) {
	vector<vi> ret;
	comb(ret, K, N);
	return ret;
}

//finner scoren til lag indice i sesong s
int team_score(const vector<vi> & score, const vi & indice, int s) {
	int ret = 0;
	REP(i, 0, indice.size()) {
		ret += score[indice[i]][s];
	}
	return ret;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	//leser input
	int ans = 0;
	cin >> N >> K >> R;

	//score til spiller x i runde y = score[x][y]
	vector<vi> score(N, vi(R));
	//les inn score
	REP(i, 0, R)
		REP(j, 0, N)
		cin >> score[j][i];

	//jeg bruker bare 2 arrays for å spare plass 
	//(men man kan bruke memo[60][16000] i stedet
	int memo1[16000];
	int memo2[16000];

	//indices for each possible team combination
	vector<vi> indices = generate_indices(N, K);

	//base cases (s=0)
	REP(i, 0, indices.size()) {
		vi indice = indices[i];
		ll key = get_id(indice);
		memo1[key] = team_score(score, indice, 0);
		//dersom R=1, så finner vi svaret her
		ans = max(ans, memo1[key]);
	}

	//buttom up DP solution
	REP(i, 1, R) {		//season
		//for å spare plass, bruker vi bare to arrays
		int* new_memo;
		int* old_memo;
		if (i & 1) {//i odde
			new_memo = memo2;
			old_memo = memo1;
		}
		else {
			new_memo = memo1;
			old_memo = memo2;
		}

		//Da vil old_memo tilsvare memoiseringen fra forrige sesong
		//og new_memo tilsvarer denne sesongen

		//vi går gjennom alle lagene
		REP(j, 0, indices.size()) {
			int id = get_id(indices[j]);

			//score denne sesongen
			int cur_score = team_score(score, indices[j], i);

			//lar best score være den når vi ikke endrer på laget
			int max_score = *(old_memo + id) + cur_score;

			//går gjennom alle forrige lag man kan komme fra
			//og ser hvilken score vi kan få fra de
			REP(n, 0, neighbours[id].size()) {
				//lagrer beste score
				max_score = max(max_score,
				*(old_memo + neighbours[id][n]) + cur_score);
			}

			//vi lagrer så beste score i memo'en vår
			*(new_memo + id) = max_score;
			//og holder alltid kontroll på største poengsum hittil
			ans = max(ans, max_score);
		}
	}

	//som vi printer ut når vi er ferdig
	cout << ans << endl;
	
	system("pause");
	return 0;
}