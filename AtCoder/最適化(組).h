#pragma once
#include "header.h"
#include "有理数.h"
#include "関数.h"
// ■■■■■ 最適化（組） ■■■■■


//【組の存在判定 → 組の復元】O(β log n)
/*
* a[0..n) に条件 P を満たす組が存在するか？の判定が O(β) でできる場合，
* a をランダムシャッフルして二分割すれば，条件 P を満たす組が同じ側に集まる確率は約 1/2 以上である．
* これを組の全探索ができるほど列が短くなるまで繰り返せば良い．
*/


//【組の差の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(l<r) (a[r] - a[l])
* また必要なら最大値を与える (l, r) を格納する．
*/
template <class T>
T maximize_pair_diff(const vector<T>& a, int* l = nullptr, int* r = nullptr) {
	// verify : https://atcoder.jp/contests/abc408/tasks/abc408_d

	//【方法】
	// 累積 min をもちながら左から線形走査すればいい．

	int n = sz(a);

	if (!l) l = new int;
	if (!r) r = new int;

	T res = -(T)INFL;
	T a_min = a[0]; int l_min = 0;

	repi(i, 1, n - 1) {
		if (chmax(res, a[i] - a_min)) {
			*l = l_min;
			*r = i;
		}
		if (chmin(a_min, a[i])) l_min = i;
	}

	return res;
}


//【組の XOR の最小化】O(n log n)
/*
* a[0..n) に対して以下の値を返す：
*		min_(i<j) (a[j] XOR a[i])
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
* 値域が狭ければ度数分布に直して XOR 畳込みを用いることもできる．
* 
* verify : https://codeforces.com/contest/1847/problem/C
*/


//【組の AND の最小化】O(n + m + A log A) （A=max(a,b)）
/*
* a[0..n), b[0..m) に対して以下の値を返す：
*		min_i∈[0..n) min_j∈[0..m) (a[i] AND b[j])
*/
int minimize_pair_AND(const vi& a, const vi& b) {
	//【方法】
	// 度数分布に直して AND 畳込みを用いれば良い．

	int n = sz(a), m = sz(b);

	int a_max = *max_element(all(a));
	int b_max = *max_element(all(b));
	int N = msb(max(a_max, b_max)) + 1;

	// f, g : a, b の度数分布
	vl f(1LL << N), g(1LL << N);
	rep(i, n) f[a[i]]++;
	rep(j, m) g[b[j]]++;

	// f, g を上位集合ゼータ変換する．
	rep(i, N) repb(set, N) if (!getb(set, i)) f[set] += f[set | (1 << i)];
	rep(i, N) repb(set, N) if (!getb(set, i)) g[set] += g[set | (1 << i)];

	// 各点積をとる．
	repb(set, N) f[set] *= g[set];

	// 結果を上位集合メビウス変換する．
	rep(i, N) repb(set, N) if (!getb(set, i)) f[set] -= f[set | (1 << i)];

	rep(x, 1 << N) if (f[x]) return x;

	return INF;
}


//【組の AND の最大化（要素毎，復元）】O(n + m + A log A) （A=max(a,b)）
/*
* 与えられた a[0..n), b[0..m) について，各 i ごとの
*		max_j∈[0..m) (a[i] AND b[j])
* を格納したリストを返す．必要ならば id に argmax_j を格納する．
*/
vi maximize_pair_AND(const vi& a, const vi& b, vi* id = nullptr) {
	// 参考 : https://x.com/noya2ruler/status/1916902816138092629

	//【方法】
	// a[i] AND b[j] は，set⊂a[i] かつ set⊂b[j] を満たす "最大" の set と特徴付けられる．
	// よって求めたい値は max の max 形ゆえ緩和ができ，代わりに
	//		max_j∈[0..m) max{ set | set⊂a[i] かつ set⊂b[j] }
	//		= max{ set⊂a[i] | ∃j,set⊂b[j] }
	// を求めれば良い．
	// これは上位集合ゼータ変換と下位集合ゼータ変換を組み合わせて行える．

	int n = sz(a), m = sz(b);

	int a_max = *max_element(all(a));
	int b_max = *max_element(all(b));
	int N = msb(max(a_max, b_max)) + 1;

	// g[set] : set がある b[j] に一致するなら j，さもなくば -1
	vi g(1LL << N, -1);
	rep(j, m) g[b[j]] = j;

	// g を上位集合ゼータ変換する（上からの累積 max をとる）
	// g[set] : set がある b[j] の下位集合なら j，さもなくば -1
	rep(i, N) repb(set, N) if (!getb(set, i)) chmax(g[set], g[set | (1 << i)]);

	// f[set] : set がある b[j] の下位集合ならば {set, j}，さもなくば -∞
	vector<pii> f(1LL << N, { -INF, -1 });
	repb(set, N) if (g[set] != -1) f[set] = { set, g[set] };

	// f を下位集合ゼータ変換する（下からの累積 max をとる）
	// f[set] : set の下位集合 sub で，ある b[j] の下位集合でもあるような {sub の最大値, j}
	rep(i, N) repb(set, N) if (getb(set, i)) chmax(f[set], f[set ^ (1 << i)]);

	vi res(n);
	rep(i, n) res[i] = f[a[i]].first;

	if (id) {
		id->resize(n);
		rep(i, n) (*id)[i] = f[a[i]].second;
	}

	return res;
}


//【組の GCD の最小化】O(n + A log(log A))（A=max(a)）
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

	Div_mul_transform g(m);
	cnt = g.gcd_convolution(cnt, cnt);

	repi(j, 1, m) if (cnt[j]) return j;

	return INF;
}


//【組の GCD の最小化（復元）】O(n + A log A log n)（A=max(a)）
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

	Div_mul_transform D(m);
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


//【組の GCD の最大化（復元）】O(n + A log A)（A=max(a)）
/*
* 与えられた正整数列 a[0..n) に対して MAX_i<j GCD(a[i], a[j]) を返す．
* また最大値を与える (i, j) を ids に格納する．
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


//【組の GCD の最大化（要素毎，復元）】O(n + m + A log(log A)) （A=max(a,b)）
/*
* 与えられた a[0..n), b[0..m) について，各 i ごとの
*		max_j∈[0..m) gcd(a[i], b[j])
* を格納したリストを返す．必要ならば id に argmax_j を格納する．
*/
vi maximize_pair_GCD(const vi& a, const vi& b, vi* id = nullptr) {
	// 参考 : https://x.com/noya2ruler/status/1916902816138092629

	//【方法】
	// gcd(a[i], b[j]) は g|a[i] かつ g|b[j] を満たす "最大" の g と特徴付けられる．
	// よって求めたい値は max の max 形ゆえ緩和ができ，代わりに
	//		max_j∈[0..m) max{ g : g|a[i] かつ g|b[j] }
	//		= max{ g|a[i] : ∃j,g|b[j] }
	// を求めれば良い．
	// これは倍数ゼータ変換と約数ゼータ変換を組み合わせて行える．

	int n = sz(a), m = sz(b);

	int a_max = *max_element(all(a));
	int b_max = *max_element(all(b));
	int N = max(a_max, b_max);
	Assert(N >= 1);

	// ps : N 以下の素数のリスト
	vi ps;

	// エラトステネスの篩
	vb is_prime(N + 1, true);
	is_prime[0] = is_prime[1] = false;
	int i = 2;
	for (; i * i <= N; i++) if (is_prime[i]) {
		ps.push_back(i);
		for (int j = i * i; j <= N; j += i) is_prime[j] = false;
	}
	for (; i <= N; i++) if (is_prime[i]) ps.push_back(i);

	// g[x] : x がある b[j] に一致するなら j，さもなくば -1
	vi g(N + 1, -1);
	rep(j, m) g[b[j]] = j;

	// g を倍数ゼータ変換する（上からの累積 max をとる）
	// g[x] : x がある b[j] の約数なら j，さもなくば -1
	repe(p, ps) repir(i, N / p, 1) chmax(g[i], g[p * i]);

	// f[x] : x がある b[j] の約数ならば {x, j}，さもなくば -∞
	vector<pii> f(N + 1, { -INF, -1 });
	repi(x, 1, N) if (g[x] != -1) f[x] = { x, g[x] };

	// f を約数ゼータ変換する（下からの累積 max をとる）
	// f[x] : x の約数 g で，ある b[j] の約数でもあるような {g の最大値, j}
	repe(p, ps) repi(i, 1, N / p) chmax(f[p * i], f[i]);

	vi res(n);
	rep(i, n) res[i] = f[a[i]].first;

	if (id) {
		id->resize(n);
		rep(i, n) (*id)[i] = f[a[i]].second;
	}

	return res;
}


//【組の LCM の最小化（復元）】O(n + A log A)（A=max(a)）
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
* 
* 利用：【素因数分解（複数）】
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
* max_flag = true とすると最大値を返す．
*
* 制約 : b1[j] > 0 （∀j∈[0..m)）
*
* 利用：【Convex-Hull Trick】,【有理数】
*/
ll minimize_pair_inner_product(const vl& a1, const vl& a2, const vl& b1, const vl& b2, bool max_flag = false) {
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

	Convex_hull_trick<Frac<ll>> cht(!max_flag);

	// 直線群 {y = a2[i] x + a1[i]}_i で初期化する．
	rep(i, n) cht.insert(Frac(a2[i]), Frac(a1[i]));

	if (!max_flag) {
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


