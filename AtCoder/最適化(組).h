#pragma once
#include "header.h"
#include "有理数.h"
#include "関数.h"
// ■■■■■ 最適化（組） ■■■■■


//【組の差の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) (a[j] - a[i])
* また最大値を与える (i, j) を ids に格納する．
*/
template <class T>
T maximize_pair_diff(const vector<T>& a, pii* ids = nullptr) {
	//【方法】
	// 累積 min をもちながら左から線形走査すればいい．

	int n = sz(a);
	T res = numeric_limits<T>::lowest(), a_min = a[0]; int i_min = 0;
	if (ids == nullptr) ids = new pii;

	repi(i, 1, n - 1) {
		if (chmax(res, a[i] - a_min)) *ids = { i_min, i };
		if (chmin(a_min, a[i])) i_min = i;
	}

	return res;
}


//【組の XOR の最小化】O(n log n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) (a[j] XOR a[i])
* また最小値を与える (i, j) を ids に格納する．
*/
template <class T>
T minimize_pair_XOR(const vector<T>& a, pii* ids = nullptr) {
	// 参考 : https://twitter.com/pro_anyone/status/1666487254175604736

	//【方法】
	// ソートして隣接要素の XOR のみを調べれば良い．

	//【証明】
	// x < y < z のとき
	//		min(x XOR y, y XOR z) < x XOR z
	// であることを示せば良い．
	// x, z を上位ビットから順に見ていき，初めて食い違ったのが d 桁目とする．
	// y[d] = 0 なら x[d] = y[d] なので x XOR y < x XOR z となり，
	// y[d] = 1 なら y[d] = z[d] なので y XOR z < x XOR z となる．

	int n = sz(a);
	if (ids == nullptr) ids = new pii;

	vector<pair<T, int>> ai(n);
	rep(i, n) ai[i] = { a[i], i };
	sort(all(ai));

	T res = numeric_limits<T>::max();
	rep(i, n - 1) if (chmin(res, ai[i].first ^ ai[i + 1].first)) {
		*ids = { ai[i].second, ai[i + 1].second };
	}

	return res;
}


//【組の XOR の最大化】O(n log n)
/*
*【binary trie】の max_element(mask) を用いれば良い．
* 
* verify : https://codeforces.com/contest/1847/problem/C
*/


//【組の GCD の最小化】O(n + A log(log A))（A = max(a)）
/*
* 与えられた正整数列 a[0..n) に対して MIN_i<j GCD(a[i], a[j]) を返す．
*
* 利用：【約数倍数変換】
*/
int minimize_pair_gcd(const vi& a) {
	//【方法】
	// 度数分布に直して GCD 畳込みし，非 0 の最小要素を見れば良い．

	int n = sz(a);

	int m = *max_element(all(a));
	vl cnt(m + 1);
	rep(i, n) cnt[a[i]] = 1;

	Div_mul_transform<ll> g(m);
	cnt = g.gcd_convolution(cnt, cnt);

	repi(j, 1, m) if (cnt[j]) return j;

	return INF;
}


//【組の GCD の最小化（復元）】O(n + A log A log n)（A = max(a)）
/*
* 与えられた正整数列 a[0..n) に対して MIN_i<j GCD(a[i], a[j]) の値と，
* それを実現する i, j を合わせた 3 つ組 (g, i, j) を返す．
*
* 利用：【約数倍数変換】
*/
tuple<int, int, int> minimize_pair_gcd_construct(const vi& a) {
	//【方法】
	// 度数分布に直して GCD 畳込みし，非 0 の最小要素を見れば g は求まる．
	// a をランダムシャッフルして二分割すれば，
	// GCD = g となる組が同じ側に集まる確率は約 1/2 以上である．
	// これを全探索できるほど列が短くなるまで繰り返せば良い．

	int n = sz(a);

	int m = *max_element(all(a));
	vl cnt(m + 1);
	rep(i, n) cnt[a[i]] = 1;

	Div_mul_transform<ll> D(m);
	cnt = D.gcd_convolution(cnt, cnt);

	// g = MIN_i<j GCD(a[i], a[j])
	int g = -1;
	repi(j, 1, m) if (cnt[j]) {
		g = j;
		break;
	}

	mt19937_64 mt((int)time(NULL));

	// GCD = g となる値の組
	int ai = -1, aj = -1;

	auto b(a);
	constexpr int lim = (int)1e6;

	while (1) {
		// n : 配列の要素数
		n = sz(b);

		// 全探索できるほど要素が少なければ全探索する．
		if ((ll)n * (n - 1) / 2 <= max(m, lim)) {
			rep(i, n) repi(j, i + 1, n - 1) {
				if (gcd(b[i], b[j]) == g) {
					ai = b[i];
					aj = b[j];
					break;
				}
				if (ai != -1) break;
			}
			break;
		}

		// 配列をランダムシャッフルする．
		shuffle(all(b), mt);

		// 前半に GCD = g となる組があるなら後半を削除する．
		vl cnt1(m + 1);
		rep(i, n / 2) cnt1[b[i]] = 1;
		cnt1 = D.gcd_convolution(cnt1, cnt1);

		if (cnt1[g]) {
			b.resize(n / 2);
			continue;
		}

		// 後半に GCD = g となる組があるなら前半を削除する．
		vl cnt2(m + 1);
		repi(i, n / 2, n - 1) cnt2[b[i]] = 1;
		cnt2 = D.gcd_convolution(cnt2, cnt2);

		if (cnt2[g]) {
			b.erase(b.begin(), b.begin() + (n / 2));
			continue;
		}
	}

	// 値から添字を復元する．
	int ig = -1, jg = -1;

	rep(i, sz(a)) {
		if (a[i] == ai) {
			ig = i;
			break;
		}
	}
	rep(j, sz(a)) {
		if (j != ig && a[j] == aj) {
			jg = j;
			break;
		}
	}

	return { g, ig, jg };
}


//【組の GCD の最大化（復元）】O(n + A log A)（A = max(a)）
/*
* 与えられた正整数列 a[0..n) に対して MAX_i<j GCD(a[i], a[j]) を返す．
* また最小値を与える (i, j) を ids に格納する．
*/
int maximize_pair_gcd(const vi& a, pii* ids = nullptr) {
	//【方法】
	// g 降順に GCD(a[i], a[j]) = g なる (i, j) が存在するか調べていく．
	// 条件を緩和して g | GCD(a[i], a[j]) とすると，
	// これは g の倍数が 2 個以上存在することと同値である．
	// 緩和してはいるが，g 降順の走査なので等号が成立するものが最初に見つかるから問題ない．

	int n = sz(a);

	int m = *max_element(all(a));
	vi cnt(m + 1);
	rep(i, n) cnt[a[i]]++;

	int res = -INF, ai = -1, aj = -1;

	repir(g, m, 1) {
		ai = -1, aj = -1;

		for (int v = g; v <= m; v += g) {
			if (cnt[v] == 0) continue;

			if (cnt[v] == 1) {
				if (ai == -1) ai = v;
				else {
					aj = v;
					break;
				}
			}
			else {
				ai = aj = v;
				break;
			}
		}

		if (aj != -1) {
			res = g;
			break;
		}
	}

	if (ids) {
		rep(i, n) {
			if (a[i] == ai) {
				ids->first = i;
				break;
			}
		}
		rep(j, n) {
			if (j != ids->first && a[j] == aj) {
				ids->second = j;
				break;
			}
		}
	}

	return res;
}


//【組の LCM の最小化（復元）】O(n + A log A)（A = max(a)）
/*
* 与えられた正整数列 a[0..n) に対して MIN_i<j LCM(a[i], a[j]) を返す．
* また最小値を与える (i, j) を ids に格納する．
*/
ll minimize_pair_lcm(const vi& a, pii* ids = nullptr) {
	// 参考 : https://drken1215.hatenablog.com/entry/2019/04/17/193100
	// verify : https://codeforces.com/contest/1154/problem/G

	//【方法】
	// g = GCD(a[i], a[j]) を決め打ち全探索する．
	// このとき LCM(a[i], a[j]) の最小化は積 a[i]a[j] の最小化と等価である．
	// 
	// そこで，g の倍数であるような a[0..n) を昇順に 2 つ選ぶことにする．
	// これでは g | GCD(a[i], a[j]) となることしか保証されないが，
	// 等号が成立する g のときにも同じ組 (a[i], a[j]) が選ばれ，
	// かつ a[i]a[j]/g がより小さいので上書きされるため問題ない．

	int n = sz(a);

	int m = *max_element(all(a));
	vi cnt(m + 1);
	rep(i, n) cnt[a[i]]++;

	ll res = INFL, ai_min = -1, aj_min = -1;

	// g : GCD(a[i], a[j])
	repi(g, 1, m) {
		int ai = -1, aj = -1;

		for (int v = g; v <= m; v += g) {
			if (cnt[v] == 0) continue;

			if (cnt[v] == 1) {
				if (ai == -1) ai = v;
				else if (aj == -1) {
					aj = v;
					break;
				}
			}
			else {
				if (ai == -1) ai = v;
				if (aj == -1) aj = v;
				break;
			}
		}

		if (aj == -1) continue;

		if (chmin(res, (ll)ai * aj / g)) {
			ai_min = ai;
			aj_min = aj;
		}
	}

	if (ids) {
		rep(i, n) {
			if (a[i] == ai_min) {
				ids->first = i;
				break;
			}
		}
		rep(j, n) {
			if (j != ids->first && a[j] == aj_min) {
				ids->second = j;
				break;
			}
		}
	}

	return res;
}


//【組の LCM の最大化（復元）】O(?)
/*
* 与えられた正整数列 a[0..n) に対して MAX_i<j LCM(a[i], a[j]) を返す．
* また最大値を与える (i, j) を ids に格納する．
*/
ll maximize_pair_lcm(const vi& a, pii* ids = nullptr) {
	// 参考 : https://drken1215.hatenablog.com/entry/2020/01/14/023500
	// verify : https://codeforces.com/contest/1285/problem/F

	int n = sz(a);

	int m = *max_element(all(a));

	// ex[j] : a[0..n) 内に j があるか
	vb ex(m + 1);
	rep(i, n) ex[a[i]] = 1;

	Osa_k O(m);

	// ds[j] : j の約数のリスト
	// ps[j] : j の素因数のリスト（重複除く）
	vvi ds(m + 1), ps(m + 1);
	repi(j, 1, m) {
		ds[j] = O.divisors(j);
		ps[j] = O.unique_prime_factors(j);
	}

	ll res = lcm<ll>(a[0], a[1]); int ai_max = a[0], aj_max = a[1];

	// g : GCD(a[i], a[j])
	repi(g, 1, m) {
		// 注目している数 v 以下の数との LCM が答えになる可能性のある数のリスト
		stack<int> stk;

		// cnt[j] : stk 内の j の倍数の個数
		int mg = m / g;
		vi cnt(mg + 1);

		// g の倍数 t g を降順に見ていく．
		repir(t, m / g, 1) {
			if (!ex[t * g]) continue;

			// 包除原理で t と互いに素な数の個数を求める．
			int c = 0;
			repb(set, sz(ps[t])) {
				int sgn = popcount(set) & 1 ? -1 : 1;

				int t_set = 1;
				repis(k, set) t_set *= ps[t][k];

				c += sgn * cnt[t_set];
			}

			// t の相手は t と互いに素な数のうち最大のもの t2 にするのが最善である．
			// t2 未満の数は，この先 t 未満の数と組んでも最大値を更新できないので無視できる．
			// よって候補リストに t と互いに素な数がなくなるまで昇順に値を削除する．
			while (c > 0) {
				int t2 = stk.top(); stk.pop();
				repe(d2, ds[t2]) cnt[d2]--;

				if (gcd(t, t2) != 1) continue;

				c--;

				if (c == 0 && chmax(res, (ll)t * t2 * g)) {
					ai_max = t * g;
					aj_max = t2 * g;
				}
			}

			// リストに t を追加する．
			stk.push(t);
			repe(d, ds[t]) cnt[d]++;
		}
	}

	if (ids) {
		rep(i, n) {
			if (a[i] == ai_max) {
				ids->first = i;
				break;
			}
		}
		rep(j, n) {
			if (j != ids->first && a[j] == aj_max) {
				ids->second = j;
				break;
			}
		}
	}

	return res;
}


//【組の内積の最小化】O((n + m) log n)
/*
* min_(i,j)∈[0..n)×[0..m) (a1[i], a2[i])・(b1[j], b2[j]) を返す．
* min_flag = false とすると最大値を返す．
*
* 制約 : b1[j] > 0 （∀j∈[0..m)）
*
* 利用：【Convex-Hull Trick】,【有理数】
*/
ll minimize_pair_inner_product(const vl& a1, const vl& a2, const vl& b1, const vl& b2, bool min_flag = true) {
	// verify : https://atcoder.jp/contests/arc051/tasks/arc051_d

	//【方法】
	// 式を整理すると
	//		min_(i,j) (a1[i], a2[i])・(b1[j], b2[j])
	//		= min_(i,j) (a1[i] b1[j] + a2[i] b2[j])
	//		= min_j (b1[j] min_i (a1[i] + a2[i] b2[j]/b1[j])) （b1[j] > 0 より）
	// となる．最後の式で i について最小化すべきものは，
	//		直線 y = a2[i] x + a1[i] の x = b2[j]/b1[j] における値
	// とみなせるから，CHT を用いて高速に求められる．

	int n = sz(a1), m = sz(b1);

	Convex_hull_trick<Frac<ll>> cht(min_flag);

	// 直線群 {y = a2[i] x + a1[i]}_i で初期化する．
	rep(i, n) cht.insert(Frac(a2[i]), Frac(a1[i]));

	if (min_flag) {
		ll res = INFL;

		// 各 j について，x = b2[j]/b1[j] における最小値を求める．
		rep(j, m) {
			auto frac = cht.get(Frac(b2[j], b1[j])) * b1[j];
			frac.reduction();
			chmin(res, frac.num);
		}

		return res;
	}
	else {
		ll res = -INFL;

		// 各 j について，x = b2[j]/b1[j] における最大値を求める．
		rep(j, m) {
			auto frac = cht.get(Frac(b2[j], b1[j])) * b1[j];
			frac.reduction();
			chmax(res, frac.num);
		}

		return res;
	}
}


//【判定 → 復元】O(β log n)
/*
* a[0..n) に条件 P を満たす組が存在するか？の判定が O(β) でできる場合，
* a をランダムシャッフルして二分割すれば，条件 P を満たす組が同じ側に集まる確率は約 1/2 以上である．
* これを組の全探索ができるほど列が短くなるまで繰り返せば良い．
*/


