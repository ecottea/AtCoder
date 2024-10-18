#pragma once
#include "header.h"
#include "二項係数.h"
#include "FPS(抽象).h"
// ■■■■■ 包含関係のゼータ変換・メビウス変換，AND 畳込み，OR 畳込み ■■■■■


//【集合関数】
/*
* [0..N) の部分集合全体を定義域とする関数を [0..N) 上の集合関数とよび，vT(1 << N) で表す．
*/


//【違反がない条件の扱い】
/*
* どの要素にも違反がない列 x[0..N) に関する対象の数え上げは，[0..N) 上の集合関数 f を
*	f(S) = 条件 (どの x[0..N) の要素にも違反がない) を
*		   条件 (i∈S ⇔ x[i] に違反がある) に変更したときの対象の個数
* と定めたときの f(φ) を求めることに相当する．
*
* f を上位集合ゼータ変換した集合関数を g とすると，g は
*	g(S) = 条件 (どの x[0..N) の要素にも違反がない) を
*		   条件 (i∈S ⇒ x[i] に違反がある) に変更したときの対象の個数
* と解釈できる．g(S) であれば i∈S でない i は無条件となり考えやすくなる．
* これを【上位集合メビウス変換（最小元）】することで f(φ) を求めれば良い．
*/


//【上位集合ゼータ変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の上位集合からの累積和が
*       g[S] = ΣT⊃S f[T] （S : [0..N) の部分集合）
* であるとし，与えられた f[0..2^N) を対応する g[0..2^N) に上書きする．
*/
template <class T>
void set_superzeta(vector<T>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	//【例（N = 3 のとき）】
	//	g[0] = f[0] + f[1] + f[2] + f[3] + f[4] + f[5] + f[6] + f[7]
	//	g[1] =      + f[1]        + f[3]        + f[5]        + f[7]
	//	g[2] =             + f[2] + f[3]               + f[6] + f[7]
	//	g[3] =                    + f[3]                      + f[7]
	//	g[4] =                           + f[4] + f[5] + f[6] + f[7]
	//	g[5] =                                  + f[5]        + f[7]
	//	g[6] =                                         + f[6] + f[7]
	//	g[7] =                                                + f[7]
	//
	// シェルピンスキーのギャスケットのパターンが見えている．

	int N = msb(sz(f));

	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set] += f[set + (1 << i)];
}


//【上位集合メビウス変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の上位集合からの累積和が
*       g[S] = ΣT⊃S f[T] （S : [0..N) の部分集合）
* であるとし，与えられた g[0..2^N) を対応する f[0..2^N) に上書きする．
*
* 具体的には
*		f[S] = ΣT⊃S (-1)^|T-S| g[T]
* で表される．
*/
template <class T>
void set_supermobius(vector<T>& g) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	//【例（n = 3 のとき）】
	//	f[0] = g[0] - g[1] - g[2] + g[3] - g[4] + g[5] + g[6] - g[7]
	//	f[1] =      + g[1]        - g[3]        - g[5]        + g[7]
	//	f[2] =             + g[2] - g[3]               - g[6] + g[7]
	//	f[3] =                    + g[3]                      - g[7]
	//	f[4] =                           + g[4] - g[5] - g[6] + g[7]
	//	f[5] =                                  + g[5]        - g[7]
	//	f[6] =                                         + g[6] - g[7]
	//	f[7] =                                                + g[7]

	int N = msb(sz(g));

	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set] -= g[set + (1 << i)];
}


//【上位集合メビウス変換（最小元）】O(2^N)
/*
* [0..N) 上の集合関数 f(S) の上位集合からの累積和が
*       g[S] = ΣT⊃S f(T) （S : [0..N) の部分集合）
* であるとし，与えられた g[0..2^N) に対応する f(φ) を返す．
*
* 具体的には
*		f(φ) = ΣT⊂[0..N) (-1)^|T| g[T]
* で表される．
*/
template <class T>
T set_supermobius_bottom(const vector<T>& g) {
	// verify : https://atcoder.jp/contests/tokiomarine2020/tasks/tokiomarine2020_e
	
	int N = msb(sz(g));

	T f0(0);
	repb(set, N) f0 += (popcount(set) % 2 ? -1 : 1) * g[set];

	return f0;
}


//【上位集合メビウス変換（大きさ依存, mod 998244353）】O(N log N)
/*
* [0..N) 上の集合関数 f(S) が S の大きさ |S| のみに依存する関数を用いて
*		f(S) = f[|S|]
* と書けるとする．このとき f(S) の上位集合からの累積和
*		g(S) = ΣT⊃S f(T) （S : [0..N) の部分集合）
* もまた
*		g(S) = g[|S|]
* と書ける．与えられた g[0..N] に対応する f[0..N] を返す．
*
* 具体的には
*		f[n] = Σk∈[n..N] (-1)^(k-n) bin(N-n,k-n) g[k]
* で表される．
*
* 制約：fm は N! まで計算可能
*/
vm set_supermobius_size(vm g, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc217/tasks/abc217_g

	//【方法】
	// メビウス変換の式を変形すると，
	//		f[n] = (N-n)! Σk∈[n..N] ((-1)^(k-n) / (k-n)!) (g[k] / (N-k)!)
	// となるので，上側畳込みで高速に計算できる．

	//【例（N=3）】
	//		f[3] = 0C0 g[3]
	//		f[2] = 1C0 g[2] - 1C1 g[3]
	//		f[1] = 2C0 g[1] - 2C1 g[2] + 2C2 g[3]
	//		f[0] = 3C0 g[0] - 3C1 g[1] + 3C2 g[2] - 3C3 g[3]

	int N = sz(g) - 1;

	repi(n, 0, N) g[n] *= fm.fact_inv(N - n);

	vm coef(N + 1);
	repi(n, 0, N) coef[n] = ((N - n) % 2 ? -1 : 1) * fm.fact_inv(N - n);

	// coef と g を畳み込んで上側を取得する．
	vm f = convolution(coef, g);
	f.erase(f.begin(), f.begin() + N);

	repi(n, 0, N) f[n] *= fm.fact(N - n);

	return f;
}


//【上位集合メビウス変換（大きさ依存，最小元）】O(N)
/*
* [0..N) 上の集合関数 f(S) の上位集合からの累積和
*		g(S) = ΣT⊃S f(T) （S : [0..N) の部分集合）
* が S の大きさ |S| のみに依存する関数 g[|S|] を用いて
*		g(S) = g[|S|]
* と書けるとする．与えられた g[0..N] に対応する f(φ) を返す．
*
* 具体的には
*		f(φ) = Σk⊂[0..N] (-1)^k bin(N,k) g[k]
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint set_supermobius_size_bottom(const vm& g, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_e

	int N = sz(g) - 1;

	mint f0;
	repi(k, 0, N) f0 += (k % 2 ? -1 : 1) * fm.bin(N, k) * g[k];

	return f0;
}


//【上位集合メビウス変換（大きさごと，最小元）】O(N)
/*
* [0..N) 上の集合関数 f(S) の上位集合からの累積和を
*		g(S) = ΣT⊃S f(T) （S : [0..N) の部分集合）
* とする．各 k∈[0..N] に対する
*		G[k] = Σ|T|=k g[T]
* が求まっていれば，f(φ) は
*		f(φ) = Σk∈[0..N] (-1)^k G[k]
* として計算できる．
* 
* 注意：最小元以外ではこの方法は適用できない．
* 
* verify : https://atcoder.jp/contests/abc214/tasks/abc214_g
*/


//【積集合畳込み】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数 f, g に対して
*       h[S] = Σ_(T∩U = S) f[T] g[U]
* なる h[0..2^N) を返す．
*/
template <class T>
vector<T> and_convolution(vector<T> f, vector<T> g) {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int N = msb(sz(f));

	// f, g を上位集合ゼータ変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set] += f[set + (1 << i)];
	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set] += g[set + (1 << i)];

	// 各点積をとる．
	repb(set, N) f[set] *= g[set];

	// 結果を上位集合メビウス変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set] -= f[set + (1 << i)];

	return f;
}


//【上位集合 max ゼータ変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の上位集合からの累積 max が
*       g[S] = MAX_(T⊃S) f[T]
* であるとし，与えられた f[0..2^N) を対応する g[0..2^N) に上書きする．
*/
template <class T>
void set_max_superzeta(vector<T>& f) {
	// verify : https://atcoder.jp/contests/arc184/tasks/arc184_b

	int N = msb(sz(f));

	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(f[set], f[set + (1 << i)]);
}


//【上位積集合 max 畳込み】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数 f, g に対して
*       h[S] = MAX_(T∩U ⊃ S) (f[T] + g[U])
* なる h[0..2^N) を返す．
*/
template <class T>
vector<T> superset_and_max_convolution(vector<T> f, vector<T> g) {
	int N = msb(sz(f));

	// f, g を上位集合 max ゼータ変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(f[set], f[set + (1 << i)]);
	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(g[set], g[set + (1 << i)]);

	// 各点和をとる．
	repb(set, N) f[set] += g[set];

	return f;
}


//【下位集合ゼータ変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の下位集合からの累積和が
*       g[S] = ΣT⊂S f[T] （S : [0..N) の部分集合）
* であるとし，与えられた f[0..2^N) を対応する g[0..2^N) に上書きする．
*/
template <class T>
void set_subzeta(vector<T>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	//【例（N = 3 のとき）】
	//	g[0] = f[0]
	//	g[1] = f[0] + f[1]
	//	g[2] = f[0] +      + f[2]
	//	g[3] = f[0] + f[1] + f[2] + f[3]
	//	g[4] = f[0]                      + f[4]
	//	g[5] = f[0] + f[1]               + f[4] + f[5]
	//	g[6] = f[0] +      + f[2]        + f[4]        + f[6]
	//	g[7] = f[0] + f[1] + f[2] + f[3] + f[4] + f[5] + f[6] + f[7]
	//
	// シェルピンスキーのギャスケットのパターンが見えている．

	int n = sz(f);
	int N = msb(n - 1) + 1;

	// n が 2 冪でなくても [0..n) の範囲では正しく計算できる．
	rep(i, N) rep(set, n) if (getb(set, i)) f[set] += f[set - (1 << i)];
}


//【下位集合メビウス変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の下位集合からの累積和が
*       g[S] = ΣT⊂S f[T] （S : [0..N) の部分集合）
* であるとし，与えられた g[0..2^N) を対応する f[0..2^N) に上書きする．
* 
* 具体的には
*		f[S] = ΣT⊂S (-1)^|S-T| g[T]
* で表される．
*/
template <class T>
void set_submobius(vector<T>& g) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	//【例（N = 3 のとき）】
	//  f[0] =  g[0]
	//  f[1] = -g[0] + g[1]
	//  f[2] = -g[0]        + g[2]
	//  f[3] =  g[0] - g[1] - g[2] + g[3]
	//  f[4] = -g[0]                      + g[4]
	//  f[5] =  g[0] - g[1]               - g[4] + g[5]
	//  f[6] =  g[0]        - g[2]        - g[4]        + g[6]
	//  f[7] = -g[0] + g[1] + g[2] - g[3] + g[4] - g[5] - g[6] + g[7]

	int N = msb(sz(g));

	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set + (1 << i)] -= g[set];
}


//【下位集合メビウス変換（大きさ依存, mod 998244353）】O(N log N)
/*
* [0..N) 上の集合関数 f(S) が S の大きさ |S| のみに依存する関数を用いて
*		f(S) = f[|S|]
* と書けるとする．このとき f(S) の下位集合からの累積和
*		g(S) = ΣT⊂S f(T) （S : [0..N) の部分集合）
* もまた
*		g(S) = g[|S|]
* と書ける．与えられた g[0..N] に対応する f[0..N] を返す．
*
* 具体的には
*		f[n] = Σk∈[0..n] (-1)^(n-k) bin(n,k) g[k]
* で表される．
*
* 制約：fm は N! まで計算可能
*/
vm set_submobius_size(vm g, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/2388

	//【方法】
	// メビウス変換の式を変形すると，
	//		f[n] = n! Σk∈[0..n] ((-1)^(n-k) / (n-k)!) (g[k] / k!)
	// となるので，畳込みで高速に計算できる．

	int N = sz(g) - 1;

	repi(n, 0, N) g[n] *= fm.fact_inv(n);

	// coef : exp(-z) の係数
	vm coef(N + 1);
	repi(n, 0, N) coef[n] = (n % 2 ? -1 : 1) * fm.fact_inv(n);

	// coef と g を畳み込んで下側を取得する．
	vm f = convolution(coef, g);
	f.resize(N + 1);

	repi(n, 0, N) f[n] *= fm.fact(n);

	return f;
}


//【下位集合メビウス変換（大きさ依存，最大元）】O(N)
/*
* [0..N) 上の集合関数 f(S) の下位集合からの累積和
*		g(S) = ΣT⊂S f(T) （S : [0..N) の部分集合）
* が S の大きさ |S| のみに依存する関数 g[|S|] を用いて
*		g(S) = g[|S|]
* と書けるとする．与えられた g[0..N] に対応する f([0..N)) を返す．
*
* 具体的には
*		f([0..N)) = Σk⊂[0..N] (-1)^(N-k) bin(N,k) g[k]
* で表される．
*
* 制約：fm は N! まで計算可能
*/
mint set_submobius_size_top(const vm& g, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/1815

	int N = sz(g) - 1;

	mint f0;
	repi(k, 0, N) f0 += ((N - k) % 2 ? -1 : 1) * fm.bin(N, k) * g[k];

	return f0;
}


//【和集合畳込み】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数 f, g に対して
*       h[S] = Σ_(T∪U = S) f[T] g[U]
* なる h[0..2^N) を返す．
*/
template <class T>
vector<T> or_convolution(vector<T> f, vector<T> g) {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int N = msb(sz(f));

	// f, g を下位集合ゼータ変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set + (1 << i)] += f[set];
	rep(i, N) repb(set, N) if (!(set & (1 << i))) g[set + (1 << i)] += g[set];

	// 各点積をとる．
	repb(set, N) f[set] *= g[set];

	// 結果を下位集合メビウス変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) f[set + (1 << i)] -= f[set];

	return f;
}


//【下位集合 max ゼータ変換】O(2^N N)
/*
* [0..N) 上の集合関数 f[S] の下位集合からの累積 max が
*       g[S] = MAX_(T⊂S) f[T] （S : [0..N) の部分集合）
* であるとし，与えられた f[0..2^N) を対応する g[0..2^N) に上書きする．
*/
template <class T>
void set_max_subzeta(vector<T>& f) {
	// verify : https://yukicoder.me/problems/no/119

	int N = msb(sz(f));

	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(f[set + (1 << i)], f[set]);
}


//【下位和集合 max 畳込み】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数 f, g に対して
*       h[S] = MAX_(T∪U ⊂ S) (f[T] + g[U])
* なる h[0..2^N) を返す．
*/
template <class T>
vector<T> subset_or_max_convolution(vector<T> f, vector<T> g) {
	int N = msb(sz(f));

	// f, g を下位集合 max ゼータ変換する．
	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(f[set + (1 << i)], f[set]);
	rep(i, N) repb(set, N) if (!(set & (1 << i))) chmax(g[set + (1 << i)], g[set]);

	// 各点和をとる．
	repb(set, N) f[set] += g[set];

	return f;
}


//【下位和集合 max 自己畳込み（重複禁止）】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数 f に対して
*       h[S] = MAX_(T∪U ⊂ S, T≠U) (f[T] + f[U])
* なる h[0..2^N) を返す．
*/
template <class T>
vector<T> distinct_subset_or_max_convolution(const vector<T>& f) {
	// verify : https://atcoder.jp/contests/arc100/tasks/arc100_c

	int N = msb(sz(f));
	T T_MIN = numeric_limits<T>::lowest();

	// F[set] : sub⊂set についての f[sub] の {max, 2nd max}
	vector<pair<T, T>> F(1LL << N);
	rep(x, N) repb(set, N) F[set] = { f[set], T_MIN };

	// 下位 2nd max ゼータ変換を行い，各 set についてその下位集合の {max, 2nd max} を得る．
	rep(i, N) repb(set, N) {
		if (!(set & (1 << i))) {
			int nset = set + (1 << i);

			vector<T> vals(4);
			vals[0] = F[nset].first;
			vals[1] = F[nset].second;
			vals[2] = F[set].first;
			vals[3] = F[set].second;
			sort(all(vals), greater<T>());

			F[nset] = { vals[0], vals[1] };
		}
	}

	// 各 set についてその下位集合の max + 2nd max を得る．
	vector<T> h(1LL << N);
	repb(set, N) h[set] = F[set].first + F[set].second;
	h[0] = T_MIN;

	return h;
}


//【非交和畳込み】O(2^N N^2)
/*
* SPS.h の【SPS 積】を用いれば良い．
*/


//【集合の高速ゼータ／メビウス変換と行列のクロネッカー積】
/*
* f[0..2^n) を上位集合で高速ゼータ変換して g[0..2^n) にする線形変換の表現行列は，
* 上三角行列 [1 1] の d 個のクロネッカー積に等しい．
*           [0 1]
*
* f[0..2^n) を下位集合で高速ゼータ変換して g[0..2^n) にする線形変換の表現行列は，
* 下三角行列 [1 0] の d 個のクロネッカー積に等しい．
*           [1 1]
* verify : https://atcoder.jp/contests/arc137/tasks/arc137_d
*
* g[0..2^n) を上位集合で高速メビウス変換して f[0..2^n) にする線形変換の表現行列は，
* 上三角行列 [1 -1] の d 個のクロネッカー積に等しい．
*           [0  1]
*
* g[0..2^n) を下位集合で高速メビウス変換して f[0..2^n) にする線形変換の表現行列は，
* 下三角行列 [ 1 0] の d 個のクロネッカー積に等しい．
*           [-1 1]
*/


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ 以下抽象代数上の実装 ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


//【上位集合ゼータ変換（可換モノイド）】O(2^n n)
/*
* 可換モノイド (S, op, e) の元を要素とする a[0..2^n) を
*       A[set] = Πsup⊃set a[sup]
* なる A[0..2^n) に上書きする（上位集合の値全てを自身に加える）
*/
template <class S, S(*op)(S, S), S(*e)()>
void set_superzeta(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set] = op(f[set], f[set + (1 << i)]);
	}
}


//【上位集合メビウス変換（アーベル群）】O(2^n n)
/*
* アーベル群 (S, op, e, inv) の元を要素とする A[0..2^n) を
*       A[set] = Πsup⊃set a[sup]
* なる a[0..2^n) に上書きする（上位集合からの自身の値への寄与を取り除く）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void set_supermobius(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set] = op(f[set], inv(f[set + (1 << i)]));
	}
}


//【積集合畳込み（環）】O(2^n n)
/*
* 環 (S, add, o, mi, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sup1∩sup2 = set) a[sup1] b[sup2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（上位集合，可換モノイド）】,【メビウス変換（上位集合，アーベル群）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
vector<S> and_convolution(vector<S> a, vector<S> b) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(a));

	set_superzeta<S, add, o>(a);
	set_superzeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	set_supermobius<S, add, o, mi>(a);

	return a;
}


//【上位積集合畳込み（半環）】O(2^n n)
/*
* 半環 (S, add, o, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sup1∩sup2 ⊃ set) a[sup1] b[sup2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（上位集合，可換モノイド）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> superset_and_convolution(vector<S> a, vector<S> b) {
	int n = msb(sz(a));

	set_superzeta<S, add, o>(a);
	set_superzeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	return a;
}


//【下位集合ゼータ変換（可換モノイド）】O(2^n n)
/*
* 可換モノイド (S, op, e) の元を要素とする a[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる A[0..2^n) に上書きする（下位集合の値全てを自身に加える）
*/
template <class S, S(*op)(S, S), S(*e)()>
void set_subzeta(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set + (1 << i)] = op(f[set + (1 << i)], f[set]);
	}
}


//【下位集合メビウス変換（アーベル群）】O(2^n n)
/*
* アーベル群 (S, op, e, inv) の元を要素とする A[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる a[0..2^n) に上書きする（下位集合からの自身の値への寄与を取り除く）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void set_submobius(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set + (1 << i)] = op(f[set + (1 << i)], inv(f[set]));
	}
}


//【和集合畳込み（環）】O(2^n n)
/*
* 環 (S, add, o, mi, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1∪sub2 = set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（下位集合，可換モノイド）】,【メビウス変換（下位集合，アーベル群）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
vector<S> or_convolution(vector<S> a, vector<S> b) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(a));

	set_subzeta<S, add, o>(a);
	set_subzeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	set_submobius<S, add, o, mi>(a);

	return a;
}


//【下位和集合畳込み（半環）】O(2^n n)
/*
* 半環 (S, add, o, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1∪sub2 ⊂ set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（下位集合，可換モノイド）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> subset_or_convolution(vector<S> a, vector<S> b) {
	int n = msb(sz(a));

	set_subzeta<S, add, o>(a);
	set_subzeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	return a;
}


//【複数の二項式の下位和集合畳込み】O(2^n n^2)
/*
* 半環 (S, add, o, mul, e) の元を要素とする 2^n 個の二項式 (e {} + a[set] set)
* を全て下位和集合畳込みした結果を返す．
*
* 利用：【下位和集合畳込み（半環）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> multi_subset_or_convoluion(const vector<S>& a) {
	// verify : https://atcoder.jp/contests/abc215/tasks/abc215_h

	int N = sz(a);

	vector<vector<S>> f(N);
	rep(i, N) f[i] = { a[i] };

	// 2 冪個ずつまとめていく（分割統治法）
	for (int k = 1; k < N; k *= 2) {
		for (int i = 0; i + k < N; i += 2 * k) {
			auto c = subset_or_convolution<S, add, o, mul, e>(f[i], f[i + k]);
			rep(set, k) c[set] = add(c[set], f[i + k][set]);
			f[i].insert(f[i].end(), all(c));
		}
	}

	return f[0];
}


