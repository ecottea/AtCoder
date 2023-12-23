#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 分割束のゼータ変換・メビウス変換 ■■■■■


//【集合の分割】
/*
* 集合 [0..N) の分割を内部含めソートされた 2 次元リスト vvi で表す．
* 例えば [0..6) の分割 014|25|3 は vvi{{0, 1, 4}, {2, 5}, {3}} で表す．
*/


//【分割関数】
/*
* [0..N) の分割全体を定義域とする関数を [0..N) 上の分割関数とよび，map<vvi, T> で表す．
*/


//【互いに異なる条件の扱い】
/*
* 各要素が互いに異なる列 x[0..N) に関する対象の数え上げは，[0..N) 上の分割関数 f を
*	f(π) = 条件 (x[0..N) の要素が互いに異なる) を
*		   条件 (i,j が π の同じブロックに属する ⇔ x[i]=x[j]) に変更したときの対象の個数
* と定めたときの f(0) を求めることに相当する（0 は分割の最小元）
* 
* f を上位ゼータ変換した分割関数を g とすると，g は
*	g(π) = 条件 (x[0..N) の要素が互いに異なる) を
*		   条件 (i,j が π の同じブロックに属する ⇒ x[i]=x[j]) に変更したときの対象の個数
* と解釈できる．g(π) であればブロックごとに独立に考えやすくなる．
*
* verify : https://atcoder.jp/contests/abc236/tasks/abc236_h
*/


//【上位ゼータ変換】O(BellB(N)^2) （N=8 くらいまで動く）
/*
* [0..N) 上の分割関数 a[π] の上位分割からの累積和が
*		b[π] = Σπ:ρの細分 a[ρ] （π:[0..N) の分割）
* であるとし，与えられた a に対応する b を返す．
*/
template <class T>
map<vvi, T> partition_superzeta(int N, const map<vvi, T>& a) {
	//【方法】
	// 集合の包含関係なら要素ごとの直積，整除関係なら素因数ごとの直積で束が表せて高速ゼータ変換できるが，
	// 分割の細分関係の束は何かの直積で簡単に表せるわけではない（と思う）
	// これだけで高速化の方法が無いと言い切れるわけではないが，わからないので素朴にやる．

	if (N == 0) return a;

	// B : [0..N) の分割の総数
	int B = sz(a);

	// g : [0..N) の分割束を表すグラフ（辺は上位から下位に向かって張る）
	Graph g(B);

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，a[π] の値
	vector<T> vals(B);

	// ids[π] : 分割 π が g の何番目の頂点に対応するか
	map<vvi, int> ids; int id = 0;

	// 分割 p に対応する g の頂点を返す（なければ作成する）
	auto get_v = [&](const vvi& p) {
		auto it = ids.find(p);
		if (it == ids.end()) return ids[p] = id++;
		else return it->second;
	};

	// グラフを構築する．
	for (auto& [p, val] : a) {
		// v : 分割 p に対応する頂点
		int v = get_v(p);
		vals[v] = val;

		// L : 分割 p のブロック数
		int L = sz(p);

		rep(l, L) {
			// K : ブロック l の要素数
			int K = sz(p[l]);

			repb(set, K - 1) {
				// 空集合は認めない．
				if (set == 0) continue;

				// np : 分割 p のブロック l を細分したもの
				vvi np(L + 1);
				rep(l2, L) if (l2 != l) np[l2] = p[l2];

				rep(k, K) {
					if (get(set, k)) np[l].push_back(p[l][k]);
					else np[L].push_back(p[l][k]);
				}

				sort(all(np));
				int nv = get_v(np);
				g[v].push_back(nv);
			}
		}
	}

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，b[π] の値
	vector<T> acc(B);

	// BFS で累積和を計算する．
	rep(v, B) {
		vb seen(B);
		seen[v] = true;

		queue<int> q;
		q.push(v);

		while (!q.empty()) {
			auto s = q.front(); q.pop();
			acc[s] += vals[v];

			repe(t, g[s]) {
				if (seen[t]) continue;
				seen[t] = true;

				q.push(t);
			}
		}
	}

	// 分割関数 b を構成する．
	map<vvi, T> b;

	repe(tmp, a) {
		auto& p = tmp.first;

		int v = get_v(p);
		b[p] = acc[v];
	}

	return b;
}


//【上位メビウス変換】O(BellB(N)^2 N) （N=8 くらいまで動く）
/*
* [0..N) 上の分割関数 a[π] の上位分割からの累積和が
*		b[π] = Σπ:ρの細分 a[ρ] （π:[0..N) の分割）
* であるとし，与えられた b に対応する a を返す．
*
* 具体的には
*		a[π] = Σπ:ρの細分 μ(ρ,π) b[ρ]
*		μ(ρ,π) = ΠS:ρのブロック (-1)^(k-1) (k-1)! （k : S が π で何個に分割されるか）
* で表される．
*/
template <class T>
map<vvi, T> partition_supermobius(int N, const map<vvi, T>& b) {
	//【方法】
	// メビウス変換の式の通りに計算する．
	// 高速メビウス変換できるわけでもないので本当は除原理の方が速い．

	if (N == 0) return b;

	// B : [0..N) の分割の総数
	int B = sz(b);

	// g : [0..N) の分割束を表すグラフ（辺は上位から下位に向かって張る）
	Graph g(B);

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，b[π] の値
	vector<T> vals(B);

	// ids[π] : 分割 π が g の何番目の頂点に対応するか
	map<vvi, int> ids; int id = 0;

	// v_to_p[v] : g の頂点 v に対応する分割
	vector<vvi> v_to_p(B);

	// 分割 p に対応する g の頂点を返す（なければ作成する）
	auto get_v = [&](const vvi& p) {
		auto it = ids.find(p);
		if (it == ids.end()) {
			v_to_p[id] = p;
			return ids[p] = id++;
		}
		else return it->second;
	};

	// グラフを構築する．
	for (auto& [p, val] : b) {
		// v : 分割 p に対応する頂点
		int v = get_v(p);
		vals[v] = val;

		// L : 分割 p のブロック数
		int L = sz(p);

		rep(l, L) {
			// K : ブロック l の要素数
			int K = sz(p[l]);

			repb(set, K - 1) {
				// 空集合は認めない．
				if (set == 0) continue;

				// np : 分割 p のブロック l を細分したもの
				vvi np(L + 1);
				rep(l2, L) if (l2 != l) np[l2] = p[l2];

				rep(k, K) {
					if (get(set, k)) np[l].push_back(p[l][k]);
					else np[L].push_back(p[l][k]);
				}

				sort(all(np));
				int nv = get_v(np);
				g[v].push_back(nv);
			}
		}
	}

	// fact[i] : i!
	vector<T> fact(N);
	fact[0] = 1;
	repi(i, 1, N - 1) fact[i] = fact[i - 1] * i;

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，a[π] の値
	vector<T> acc(B);

	// BFS でメビウス関数を掛けながらの累積和を計算する．
	rep(v, B) {
		auto& vp = v_to_p[v];

		vb seen(B);
		seen[v] = true;

		queue<int> q;
		q.push(v);

		while (!q.empty()) {
			auto s = q.front(); q.pop();
			auto vs = v_to_p[s];

			T coef = 1;
			vi col(N);
			rep(i, sz(vs)) repe(x, vs[i]) col[x] = i;
			repe(b, vp) {
				vi c;
				repe(x, b) c.push_back(col[x]);
				uniq(c);
				int k = sz(c);
				coef *= (k % 2 ? 1 : -1) * fact[k - 1];
			}
			acc[s] += coef * vals[v];

			repe(t, g[s]) {
				if (seen[t]) continue;
				seen[t] = true;

				q.push(t);
			}
		}
	}

	// 分割関数 a を構成する．
	map<vvi, T> a;

	repe(tmp, b) {
		auto& p = tmp.first;

		int v = get_v(p);
		a[p] = acc[v];
	}

	return a;
}


//【上位メビウス変換（最小元）】O(BellB(N) N) （N=12 くらいまで動く）
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和が
*		b[π] = Σπ:ρの細分 a(ρ) （π:[0..N) の分割）
* であるとし，0 を分割束の最小元としたとき，与えられた b に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σρ:[0..N)の分割 (-1)^(N-|ρ|) b[ρ] ΠS:ρのブロック (|S|-1)!
* で表される．
*/
template <class T>
T partition_supermobius_bottom(const vector<pair<vvi, T>>& b) {
	// verify : https://projecteuler.net/problem=636

	if (b.empty()) return 0;

	int N = 0;
	repe(s, b[0].first) N += sz(s);

	vector<T> fact(N);
	fact[0] = 1;
	repi(i, 1, N - 1) fact[i] = fact[i - 1] * i;

	T a0 = 0;
	for (auto& [sp, val] : b) {
		T coef = ((N - sz(sp)) % 2 ? -1 : 1);
		repe(s, sp) coef *= fact[sz(s) - 1];

		a0 += coef * val;
	}

	return a0;
}


//【上位ゼータ変換（分割の型依存，最小元）】O(PartitionsP(N) N)
/*
* [0..N) 上の分割関数 a(π) が π の型 ip(π) のみに依存する自然数分割関数を用いて
*		a(π) = a[ip(π)]
* と書けるとする．また a(π) の上位分割からの累積和を
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* とおく．0 を分割束の最小元としたとき，与えられた a に対応する b(0) を返す．
*
* 具体的には
*		b(0) = Σλ:Nの分割 mul(N,λ)/(Πj c[j]!) a[λ]
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint partition_superzeta_type_bottom(const vector<pair<vi, mint>>& a, const Factorial_mint& fm) {
	int N = accumulate(all(a[0].first), 0);

	mint b0 = 0;

	for (auto& [ip, val] : a) {
		mint coef = fm.mul(ip);

		vi cnt(N);
		repe(x, ip) cnt[x - 1]++;
		rep(i, N) coef *= fm.fact_inv(cnt[i]);

		b0 += coef * val;
	}

	return b0;
}


//【上位メビウス変換（分割の型依存，最小元）】O(PartitionsP(N) N)
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* が π の型 ip(π) のみに依存する自然数分割関数を用いて
*		b(π) = b[ip(π)]
* と書けるとする．0 を分割束の最小元としたとき，与えられた b に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σλ:Nの分割 (-1)^(N-|λ|) mul(N,λ)/(Πj c[j]!) a[λ] Πi (λ_i-1)!
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint partition_supermobius_type_bottom(const vector<pair<vi, mint>>& b, const Factorial_mint& fm) {
	// verify : https://projecteuler.net/problem=495

	int N = accumulate(all(b[0].first), 0);

	mint a0 = 0;

	for (auto& [ip, val] : b) {
		mint coef = ((N - sz(ip)) % 2 ? -1 : 1) * fm.mul(ip);

		vi cnt(N);
		repe(x, ip) {
			coef *= fm.fact(x - 1);
			cnt[x - 1]++;
		}
		rep(i, N) coef *= fm.fact_inv(cnt[i]);

		a0 += coef * val;
	}

	return a0;
}


//【上位ゼータ変換（ブロック依存，最小元，mod 998244353）】O(2^N N^2)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S のみに依存する集合関数 a[S] を用いて
*		a(π) = ΠS:πのブロック a[S]
* と書けるとする．また a(π) の上位分割からの累積和を
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* とおく．0 を分割束の最小元としたとき，与えられた a[0..2^N) に対応する b(0) を返す．
*
* 具体的には
*		b(0) = Σπ:[0..N)の分割 ΠT:πのブロック a[T]
* で表され，これは【下位ゼータ変換（ブロック依存）】の b[(1<<N)-1] で代用できる．
*/


//【上位メビウス変換（ブロック依存，最小元）】O(3^N)
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* が π のブロック S のみに依存する集合関数 b[S] を用いて
*		b(π) = ΠS:πのブロック b[S]
* と書けるとする．0 を分割束の最小元としたとき，与えられた b[0..2^N) に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σπ:[0..N)の分割 (-1)^(N-|π|) ΠT:πのブロック (|T|-1)! b[T]
* で表される．
*/
template <class T>
T partition_supermobius_block_bottom(const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_h

	//【方法】
	// c[T] = -(|T|-1)! b[T] とおけば
	//		a(0) = (-1)^N Σπ:[0..N)の分割 ΠT:πのブロック c[T]
	// となる．これは SoS-bitDP で計算できる．

	int N = msb(sz(b));

	vector<T> fact(N + 1);
	fact[0] = 1;
	repi(i, 1, N) fact[i] = fact[i - 1] * i;

	vector<T> c(1LL << N);
	repi(set, 1, (1 << N) - 1) c[set] = -fact[popcount(set) - 1] * b[set];

	vector<T> a(1LL << N);
	a[0] = 1;

	// SoS bit DP
	repb(set, N) {
		if (set == 0) continue;

		// set1 : set から特定の要素 x を取り除いた集合
		int set1 = (set - 1) & set;

		// set の分割において x を含む部分集合 set-sub を全探索する．
		// sub = {} は分割完了を意味する．
		for (int sub = set1, tmp = 1; tmp > 0; tmp = sub, sub = (sub - 1) & set1) {
			a[set] += a[sub] * c[set - sub];
		}
	}

	return (N % 2 ? -1 : 1) * a[(1 << N) - 1];
}


//【上位メビウス変換（ブロック依存，最小元，mod 998244353）】O(2^N N^2)
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* が π のブロック S のみに依存する集合関数 b[S] を用いて
*		b(π) = ΠS:πのブロック b[S]
* と書けるとする．0 を分割束の最小元としたとき，与えられた b[0..2^N) に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σπ:[0..N)の分割 (-1)^(N-|π|) ΠT:πのブロック (|T|-1)! b[T]
* で表される．
*
* 制約：fm は (N+1)! まで計算可能
*
* 利用：【形式的冪級数】，【指数関数】
*/
mint partition_supermobius_block_bottom(const vm& b, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_h

	//【方法】
	// -(|S|-1)! b[S] を改めて b[S] とし，exp 版の非交和畳込みを用いれば良い． 

	int N = msb(sz(b));

	vector<MFPS> g(1LL << N);
	repi(set, 1, (1 << N) - 1) {
		int pc = popcount(set);
		g[set] = -fm.fact(pc - 1) * b[set] * MFPS::monomial(pc);
	}

	// ゼータ変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set + (1 << i)] += g[set];

	vector<MFPS> f(1LL << N);
	repb(set, N) f[set] = exp_fps(g[set], N + 1, fm);

	// メビウス変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set + (1 << i)] -= f[set];

	return (N % 2 ? -1 : 1) * f[(1 << N) - 1][N];
}


//【上位ゼータ変換（ブロック数依存）】O(N^2)
/*
* [0..N) 上の分割関数 a(π) が π のブロック数 |π| のみに依存する関数を用いて
*		a(π) = a[|π|]
* と書けるとする．このとき a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* もまた
*		b(π) = b[|π|]
* と書ける．与えられた a[0..N] に対応する b[0..N] を返す．
*
* 具体的には
*		b[n] = Σk∈[0..n] s2(n,k) a[k] （s2 は第 2 種スターリング数）
* で表される．
*/
template <class T>
vector<T> partition_superzeta_block_count(const vector<T>& a) {
	//【方法】
	// 第 2 種スターリング数は漸化式
	//		s2(n,r) = s2(n-1,r-1) + r s2(n-1,r)
	// を用いて一括で求められるので，素朴に和を計算すればよい．

	int N = sz(a) - 1;

	// s2[n][r] : 第 2 種スターリング数 s2(n,r)
	vector<vector<T>> s2(N + 1, vector<T>(N + 1));
	s2[0][0] = 1;
	repi(n, 1, N) repi(r, 1, n) s2[n][r] = s2[n - 1][r - 1] + r * s2[n - 1][r];

	vector<T> b(N + 1);
	repi(n, 0, N) repi(k, 0, n) b[n] += s2[n][k] * a[k];

	return b;
}


//【上位メビウス変換（ブロック数依存）】O(N^2)
/*
* [0..N) 上の分割関数 a(π) が π のブロック数 |π| のみに依存する関数を用いて
*		a(π) = a[|π|]
* と書けるとする．このとき a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* もまた
*		b(π) = b[|π|]
* と書ける．与えられた b[0..N] に対応する a[0..N] を返す．
*
* 具体的には
*		a[n] = Σk∈[0..n] (-1)^(n-k) S1(n,k) b[k] （S1 は符号なし第 1 種スターリング数）
* で表される．
*/
template <class T>
vector<T> partition_supermobius_block_count(const vector<T>& b) {
	//【方法】
	// 第 1 種スターリング数は漸化式
	//		S1(n,r) = S1(n-1,r-1) + (n-1) S1(n-1,r)
	// を用いて一括で求められるので，素朴に和を計算すればよい．

	int N = sz(b) - 1;

	// S1[n][r] : 第 1 種スターリング数 S1(n,r)
	vector<vector<T>> S1(N + 1, vector<T>(N + 1));
	S1[0][0] = 1;
	repi(n, 1, N) repi(r, 1, n) S1[n][r] = S1[n - 1][r - 1] + (n - 1) * S1[n - 1][r];

	vector<T> a(N + 1);
	repi(n, 0, N) repi(k, 0, n) a[n] += ((n - k) % 2 ? -1 : 1) * S1[n][k] * b[k];

	return a;
}


//【上位メビウス変換（ブロック数依存，最小元，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* が π のブロック数 |π| のみに依存して
*		b(π) = b[|π|]
* と書けるとする．0 を分割束の最小元としたとき，与えられた b[0..N] に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σk∈[0..n] (-1)^(n-k) S1(n,k) b[k] （S1 は符号なし第 1 種スターリング数）
* で表される．
*
* 制約：fm は N 以上の最小の 2 冪までの階乗計算が可能であること（2N で良い）
*
* 利用：【下降階乗冪（符号付き第 1 種スターリング数）】
*/
mint partition_supermobius_block_count_bottom(const vm& b, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc278/tasks/abc278_h

	int N = sz(b) - 1;

	auto S1N = falling_factorial(N, fm);

	mint a0 = 0;
	repi(k, 1, N) a0 += S1N[k] * b[k];

	return a0;
}


//【上位ゼータ変換（ブロックの大きさ依存，最小元，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S の大きさ |S| のみに依存する a[|S|] を用いて
*		a(π) = ΠS:πのブロック a[|S|]
* と書けるとする．また a(π) の上位分割からの累積和を
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* とおく．0 を分割束の最小元としたとき，与えられた a[0..N] に対応する b(0) を返す．
*
* 具体的には
*		b(0) = Σλ:Nの分割 mul(N,λ)/(Πj c[j]!) Πi a[λ_i]
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表され，これは【下位ゼータ変換（ブロックの大きさ依存）】の b[N] で代用できる．
*/


//【上位メビウス変換（ブロックの大きさ依存，最小元，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) の上位分割からの累積和
*		b(π) = Σπ:ρの細分 a(ρ) （π:[0..n) の分割）
* が π のブロック S の大きさ |S| のみに依存する b[|S|] を用いて
*		b(π) = ΠS:πのブロック b[|S|]
* と書けるとする．0 を分割束の最小元としたとき，与えられた b[0..N] に対応する a(0) を返す．
*
* 具体的には
*		a(0) = Σλ:nの分割 mul(n,λ)/(Πj c[j]!) Πi (-1)^(λ_i-1) (λ_i-1)! b[λ_i]
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表される．
*
* 制約：b[0] = 1, fm は 2(N+1)! まで計算可能
*
* 利用：【形式的冪級数】，【対数関数】
*/
mint partition_supermobius_block_size_bottom(const vm& b, const Factorial_mint& fm) {
	//【方法】
	// (-1)^(j-1) (j-1)! b[j] を改めて b[j] としその指数型母関数を g(z) とする．
	// exp(g(z)) を計算すればよい．

	int N = sz(b) - 1;

	MFPS g(0, N + 1);
	repi(i, 1, N) g[i] = (i % 2 ? 1 : -1) * b[i] * fm.inv(i);

	MFPS f = exp_fps(g, N + 1, fm);

	vm a(N + 1);
	repi(i, 0, N) a[i] = f[i] * fm.fact(i);

	return a[N];
}


//【下位ゼータ変換】O(BellB(N)^2) （N=8 くらいまで動く）
/*
* [0..N) 上の分割関数 a[π] の下位分割からの累積和が
*		b[π] = Σρ:πの細分 a[ρ] （π:[0..N) の分割）
* であるとし，与えられた a に対応する b を返す．
*/
template <class T>
map<vvi, T> partition_subzeta(int N, const map<vvi, T>& a) {
	//【方法】
	// 集合の包含関係なら要素ごとの直積，整除関係なら素因数ごとの直積で束が表せて高速ゼータ変換できるが，
	// 分割の細分関係の束は何かの直積で簡単に表せるわけではない（と思う）
	// これだけで高速化の方法が無いと言い切れるわけではないが，わからないので素朴にやる．

	if (N == 0) return a;

	// B : [0..n) の分割の総数
	int B = sz(a);

	// g : [0..n) の分割束を表すグラフ（辺は上位から下位に向かって張る）
	Graph g(B);

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，a[π] の値
	vector<T> vals(B);

	// ids[π] : 分割 π が g の何番目の頂点に対応するか
	map<vvi, int> ids; int id = 0;

	// 分割 p に対応する g の頂点を返す（なければ作成する）
	auto get_v = [&](const vvi& p) {
		auto it = ids.find(p);
		if (it == ids.end()) return ids[p] = id++;
		else return it->second;
	};

	// グラフを構築する．
	for (auto& [p, val] : a) {
		// v : 分割 p に対応する頂点
		int v = get_v(p);
		vals[v] = val;

		// L : 分割 p のブロック数
		int L = sz(p);

		rep(l, L) {
			// K : ブロック l の要素数
			int K = sz(p[l]);

			repb(set, K - 1) {
				// 空集合は認めない．
				if (set == 0) continue;

				// np : 分割 p のブロック l を細分したもの
				vvi np(L + 1);
				rep(l2, L) if (l2 != l) np[l2] = p[l2];

				rep(k, K) {
					if (get(set, k)) np[l].push_back(p[l][k]);
					else np[L].push_back(p[l][k]);
				}

				sort(all(np));
				int nv = get_v(np);
				g[v].push_back(nv);
			}
		}
	}

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，b[π] の値
	vector<T> acc(B);

	// BFS で累積和を計算する．
	rep(v, B) {
		vb seen(B);
		seen[v] = true;

		queue<int> q;
		q.push(v);

		while (!q.empty()) {
			auto s = q.front(); q.pop();
			acc[v] += vals[s];

			repe(t, g[s]) {
				if (seen[t]) continue;
				seen[t] = true;

				q.push(t);
			}
		}
	}

	// 分割関数 b を構成する．
	map<vvi, T> b;

	repe(tmp, a) {
		auto& p = tmp.first;

		int v = get_v(p);
		b[p] = acc[v];
	}

	return b;
}


//【下位メビウス変換】O(BellB(N)^2 N) （N=8 くらいまで動く）
/*
* [0..N) 上の分割関数 a[π] の下位分割からの累積和が
*		b[π] = Σρ:πの細分 a[ρ] （π:[0..N) の分割）
* であるとし，与えられた b に対応する a を返す．
*
* 具体的には
*		a[π] = Σρ:πの細分 μ(π,ρ) b[ρ]
*		μ(π,ρ) = ΠS:πのブロック (-1)^(k-1) (k-1)! （k : S が ρ で何個に分割されるか）
* で表される．
*/
template <class T>
map<vvi, T> partition_submobius(int N, const map<vvi, T>& b) {
	//【方法】
	// メビウス変換の式の通りに計算する．
	// 高速メビウス変換できるわけでもないので本当は除原理の方が速い．

	if (N == 0) return b;

	// B : [0..n) の分割の総数
	int B = sz(b);

	// g : [0..n) の分割束を表すグラフ（辺は上位から下位に向かって張る）
	Graph g(B);

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，b[π] の値
	vector<T> vals(B);

	// ids[π] : 分割 π が g の何番目の頂点に対応するか
	map<vvi, int> ids; int id = 0;

	// v_to_p[v] : g の頂点 v に対応する分割
	vector<vvi> v_to_p(B);

	// 分割 p に対応する g の頂点を返す（なければ作成する）
	auto get_v = [&](const vvi& p) {
		auto it = ids.find(p);
		if (it == ids.end()) {
			v_to_p[id] = p;
			return ids[p] = id++;
		}
		else return it->second;
	};

	// グラフを構築する．
	for (auto& [p, val] : b) {
		// v : 分割 p に対応する頂点
		int v = get_v(p);
		vals[v] = val;

		// L : 分割 p のブロック数
		int L = sz(p);

		rep(l, L) {
			// K : ブロック l の要素数
			int K = sz(p[l]);

			repb(set, K - 1) {
				// 空集合は認めない．
				if (set == 0) continue;

				// np : 分割 p のブロック l を細分したもの
				vvi np(L + 1);
				rep(l2, L) if (l2 != l) np[l2] = p[l2];

				rep(k, K) {
					if (get(set, k)) np[l].push_back(p[l][k]);
					else np[L].push_back(p[l][k]);
				}

				sort(all(np));
				int nv = get_v(np);
				g[v].push_back(nv);
			}
		}
	}

	// fact[i] : i!
	vector<T> fact(N);
	fact[0] = 1;
	repi(i, 1, N - 1) fact[i] = fact[i - 1] * i;

	// vals[i] : グラフの頂点 i に分割 π が対応するとし，a[π] の値
	vector<T> acc(B);

	// BFS でメビウス関数を掛けながらの累積和を計算する．
	rep(v, B) {
		auto& vp = v_to_p[v];

		vb seen(B);
		seen[v] = true;

		queue<int> q;
		q.push(v);

		while (!q.empty()) {
			auto s = q.front(); q.pop();
			auto vs = v_to_p[s];

			T coef = 1;
			vi col(N);
			rep(i, sz(vs)) repe(x, vs[i]) col[x] = i;
			repe(b, vp) {
				vi c;
				repe(x, b) c.push_back(col[x]);
				uniq(c);
				int k = sz(c);
				coef *= (k % 2 ? 1 : -1) * fact[k - 1];
			}
			acc[v] += coef * vals[s];

			repe(t, g[s]) {
				if (seen[t]) continue;
				seen[t] = true;

				q.push(t);
			}
		}
	}

	// 分割関数 a を構成する．
	map<vvi, T> a;

	repe(tmp, b) {
		auto& p = tmp.first;

		int v = get_v(p);
		a[p] = acc[v];
	}

	return a;
}


//【下位ゼータ変換（ブロック依存，mod 998244353）】O(2^N N^2)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S のみに依存する集合関数 a[S] を用いて
*		a(π) = ΠS:πのブロック a[S]
* と書けるとする．このとき a(π) の下位分割からの累積和
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* もまた集合関数 b[S] を用いて
*		b(π) = ΠS:πのブロック b[S]
* と書ける．与えられた a[0..2^N) に対応する b[0..2^N) を返す．
*
* 具体的には
*		b[S] = Σπ:{S}の細分 ΠT:πのブロック a[T]
* で表される．
*
* 制約：a[0] = 0, fm は (2(N+1))! まで計算可能
*
* 利用：【形式的冪級数】，【指数関数】
* 
*（集合冪級数の exp）
*/
vm partition_subzeta_block(const vm& a, const Factorial_mint& fm) {
	// 参考 : https://atcoder.jp/contests/abc236/editorial/3910

	//【方法】
	// 集合関数同士の非交和畳込みを * で表すと，
	//		b = (1/1!)a + (1/2!)a*a + (1/3!)a*a*a + ... =: exp(a) 
	// となる．非交和畳込みは FPS の積についての和集合畳込みで計算していたが，
	// その代わりに FPS の exp を用いれば良い．

	int N = msb(sz(a));

	vector<MFPS> f(1LL << N);
	repb(set, N) f[set] = a[set] * MFPS::monomial(popcount(set));

	// ゼータ変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set + (1 << i)] += f[set];

	vector<MFPS> g(1LL << N);
	repb(set, N) g[set] = exp_fps(f[set], N + 1, fm);

	// メビウス変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set + (1 << i)] -= g[set];

	vm b(1LL << N);
	repb(set, N) b[set] = g[set][popcount(set)];

	return b;
}


//【下位メビウス変換（ブロック依存，mod 998244353）】O(2^N N^2)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S のみに依存する集合関数 a[S] を用いて
*		a(π) = ΠS:πのブロック a[S]
* と書けるとする．このとき a(π) の下位分割からの累積和
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* もまた集合関数 b[S] を用いて
*		b(π) = ΠS:πのブロック b[S]
* と書ける．与えられた b[0..2^N) に対応する a[0..2^N) を返す．
*
* 具体的には
*		a[S] = Σπ:{S}の細分 (-1)^(|π|-1) (|π|-1)! ΠT:πのブロック b[T]
* で表される．
*
* 制約：b[0] = 1, fm は (2(N+1))! まで計算可能
*
* 利用：【形式的冪級数】，【対数関数】
* 
*（集合冪級数の log）
*/
vm partition_submobius_block(const vm& b, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc321/tasks/abc321_g

	//【方法】
	// 集合関数同士の非交和畳込みを * で表すと，
	//		b = (1/1!)a + (1/2!)a*a + (1/3!)a*a*a + ... =: exp(a) 
	// となるので，a = log(b) である．非交和畳込みは FPS の積についての和集合畳込みで計算していたが，
	// その代わりに FPS の log を用いれば良い．

	int N = msb(sz(b));

	vector<MFPS> g(1LL << N);
	repb(set, N) g[set] = b[set] * MFPS::monomial(popcount(set));

	// ゼータ変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set + (1 << i)] += g[set];

	vector<MFPS> f(1LL << N);
	repb(set, N) f[set] = log_fps(g[set], N + 1, fm);

	// メビウス変換（下位集合）
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set + (1 << i)] -= f[set];

	vm a(1LL << N);
	repb(set, N) a[set] = f[set][popcount(set)];

	return a;
}


//【下位ゼータ変換（ブロックの大きさ依存，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S の大きさ |S| のみに依存する関数 a[|S|] を用いて
*		a(π) = ΠS:πのブロック a[|S|]
* と書けるとする．このとき a(π) の下位分割からの累積和
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* もまた関数 b[|S|] を用いて
*		b(π) = ΠS:πのブロック b[|S|]
* と書ける．与えられた a[0..N] に対応する b[0..N] を返す．
*
* 具体的には
*		b[n] = Σλ:nの分割 mul(n,λ)/(Πj c[j]!) Πi a[λ_i]
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表される．
*
* 制約：a[0] = 0, fm は 2(N+1)! まで計算可能
*
* 利用：【形式的冪級数】，【指数関数】
*/
vm partition_subzeta_block_size(const vm& a, const Factorial_mint& fm) {
	// 参考 : https://37zigen.com/exponential-generating-function/

	//【方法】
	// a, b の指数型母関数を f(z), g(z) とおくと，
	//		g(z) = exp(f(z))
	// が成り立つ．

	int N = sz(a) - 1;

	MFPS f(0, N + 1);
	repi(i, 0, N) f[i] = a[i] * fm.fact_inv(i);

	MFPS g = exp_fps(f, N + 1, fm);

	vm b(N + 1);
	repi(i, 0, N) b[i] = g[i] * fm.fact(i);

	return b;
}


//【下位メビウス変換（ブロックの大きさ依存，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) が π のブロック S の大きさ |S| のみに依存する関数 a[|S|] を用いて
*		a(π) = ΠS:πのブロック a[|S|]
* と書けるとする．このとき a(π) の下位分割からの累積和
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* もまた関数 b[|S|] を用いて
*		b(π) = ΠS:πのブロック b[|S|]
* と書ける．与えられた b[0..N] に対応する a[0..N] を返す．
*
* 具体的には
*		a[n] = Σλ:nの分割 (-1)^(|λ|-1) (|λ|-1)! mul(n,λ)/(Πj c[j]!) Πi b[λ_i]
*		（mul は多項係数，c[j] は λ に含まれる j の個数）
* で表される．
*
* 制約：b[0] = 1, fm は 2(N+1)! まで計算可能
*
* 利用：【形式的冪級数】，【対数関数】
*/
vm partition_submobius_block_size(const vm& b, const Factorial_mint& fm) {
	// 参考 : https://37zigen.com/exponential-generating-function/

	//【方法】
	// a, b の指数型母関数を f(z), g(z) とおくと，
	//		f(z) = log(g(z))
	// が成り立つ．

	int N = sz(b) - 1;

	MFPS g(0, N + 1);
	repi(i, 0, N) g[i] = b[i] * fm.fact_inv(i);

	MFPS f = log_fps(g, N + 1, fm);

	vm a(N + 1);
	repi(i, 0, N) a[i] = f[i] * fm.fact(i);

	return a;
}


//【下位ゼータ変換（ブロック数依存，最大元，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) が π のブロック数 |π| のみに依存して
*		a(π) = a[|π|]
* と書けるとし，a(π) の下位分割からの累積和を
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* とおく．1 を分割束の最大元としたとき，与えられた a[0..N] に対応する b(1) を返す．
*
* 具体的には
*		b(1) = Σk∈[0..N] s2(N,k) a[k] （s2 は第 2 種スターリング数）
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint partition_subzeta_block_count_top(const vm& a, const Factorial_mint& fm) {
	//【方法】
	// 第 2 種スターリング数の一般項は
	//		s2(n,k)
	//		= (1/k!) Σm=[1..k] (-1)^(k-m) bin(k, m) m^n
	//		= Σm=[1..k] ((-1)^(k-m) / (k-m)!) (m^n / m!)
	// と書け，これは畳込みの形であるから高速に列挙できる．あとは素朴に計算すれば良い．

	int N = sz(a) - 1;

	vm f(N + 1), g(N + 1);
	repi(i, 0, N) {
		f[i] = (i % 2 ? -1 : 1) * fm.fact_inv(i);
		g[i] = mint(i).pow(N) * fm.fact_inv(i);
	}
	auto s2N = convolution(f, g);

	mint b1 = 0;
	repi(k, 0, N) b1 += s2N[k] * a[k];

	return b1;
}


//【下位メビウス変換（ブロック数依存，最大元，mod 998244353）】O(N log N)
/*
* [0..N) 上の分割関数 a(π) の下位分割からの累積和
*		b(π) = Σρ:πの細分 a(ρ) （π:[0..n) の分割）
* が π のブロック数 |π| のみに依存して
*		b(π) = b[|π|]
* と書けるとする．1 を分割束の最大元としたとき，与えられた b[0..N] に対応する a(1) を返す．
*
* 具体的には
*		a(1) = Σk∈[0..N] (-1)^(k-1) (k-1)! s2(N,k) b[k] （s2 は第 2 種スターリング数）
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint partition_submobius_block_count_top(const vm& b, const Factorial_mint& fm) {
	//【方法】
	// 第 2 種スターリング数の一般項は
	//		s2(n,k)
	//		= (1/k!) Σm=[1..k] (-1)^(k-m) bin(k, m) m^n
	//		= Σm=[1..k] ((-1)^(k-m) / (k-m)!) (m^n / m!)
	// と書け，これは畳込みの形であるから高速に列挙できる．あとは素朴に計算すれば良い．

	int N = sz(b) - 1;

	vm f(N + 1), g(N + 1);
	repi(i, 0, N) {
		f[i] = (i % 2 ? -1 : 1) * fm.fact_inv(i);
		g[i] = mint(i).pow(N) * fm.fact_inv(i);
	}
	auto s2N = convolution(f, g);

	mint a1 = 0;
	repi(k, 1, N) a1 += (k % 2 ? 1 : -1) * fm.fact(k - 1) * s2N[k] * b[k];

	return a1;
}


