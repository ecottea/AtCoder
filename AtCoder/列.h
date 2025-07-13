#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ 列 ■■■■■


//【変数係数線形漸化式の発見】O(n L^2 D^2 + N (L D + log(mod)))
/*
* 係数多項式の次数が D 次未満の L 項間漸化式
*	Σi∈[0..L) Σj∈[0..D) c(i,j) (m+i)^j a[m+i] = 0
* の存在を仮定して a[0..n) を延長し a[0..N] にする（失敗したら false を返す）
*
* 制約 : n ≧ L(D+1)-1（ランク落ちしてるとこれでも足りないかも）
*
* 利用：【行列】,【線形方程式】
*/
bool p_recursive(int N, vm& a, int L, int D, vm* coef = nullptr) {
	// verify : https://atcoder.jp/contests/abc222/tasks/abc222_h

	int n = sz(a);

	// 既に十分な長さがある場合はそのままで良い．
	if (N <= n - 1) {
		a.resize(N + 1);
		return true;
	}

	// 式が足りないといつでも非自明解をもってしまって意味がない（とも限らない）
	if (n < L * (D + 1) - 1) return false;

	// 行列方程式 A x = 0 を解いて一般解の基底 xs を求める．
	Matrix<mint> A(n - L + 1, L * D);
	repi(n0, 0, n - L) {
		rep(i, L) rep(j, D) {
			A[n0][i * D + j] = mint(n0 + i).pow(j) * a[n0 + i];
		}
	}
	vvm xs;
	gauss_jordan_elimination(A, vm(n - L + 1), &xs);

	// 自明解 x = 0 しか存在しない場合は失敗．
	if (xs.empty()) return false;

	a.resize(N + 1);

	// 得られた非自明解 xs.back() から漸化式を復元し，それに基づき a[0..n) を延長する．
	auto& x = xs.back();
	repi(n0, n - L + 1, N - L + 1) {
		mint num = 0;
		rep(i, L - 1) {
			mint pow_n0i = 1;
			rep(j, D) {
				num += x[i * D + j] * pow_n0i * a[n0 + i];
				pow_n0i *= n0 + i;
			}
		}

		mint dnm = 0;
		mint pow_n0L = 1;
		rep(j, D) {
			dnm += x[(L - 1) * D + j] * pow_n0L;
			pow_n0L *= n0 + L - 1;
		}

		// num + dnm * a[n0 + L - 1] = 0 を解く．
		// dnm は規則正しいことが多いので O(log(mod)) が落とせるかも．
		a[n0 + L - 1] = -num / dnm;
	}

	if (coef) *coef = move(x);

	return true;
}


//【変数係数線形漸化式の発見（2 次元）】O(h w L^4 D^4 + H W (L^2 D^2 + log(mod)))
/*
* 係数多項式の次数が D 次未満の L 項間漸化式
*	Σi1,i2∈[0..L) Σj1,j2∈[0..D) c(i1,i2,j1,j2) (m1+i1)^j1 (m2+i2)^j2 a[m1+i1][m2+j2] = 0
* の存在を仮定して a[0..h)[0..w) を延長し a[0..H][0..W] にする（失敗したら false を返す）
*
* 利用：【変数係数線形漸化式の発見】
*/
bool p_recursive(int H, int W, vvm& a, int L, int D) {
	// verify : https://atcoder.jp/contests/arc162/tasks/arc162_f

	int h = sz(a), w = sz(a[0]);

	// 既に十分な大きさがある場合はそのままで良い．
	if (H <= h - 1 && W <= w - 1) {
		a.resize(H + 1);
		repi(i, 0, H) a[i].resize(W + 1);
		return true;
	}

	// cE : 式の数, cV : 未知数の数
	int cE = (h - L + 1) * (w - L + 1);
	int cV = L * D * L * D;

	// 式が足りないといつでも非自明解をもってしまって意味がない．
	if (h < L || cE < cV) return false;

	// 行列方程式 A x = 0 を解いて一般解の基底 xs を求める．
	Matrix<mint> A(cE, cV);
	repi(n1, 0, h - L) repi(n2, 0, w - L) {
		int n = n1 * (w - L + 1) + n2;
		rep(i1, L) rep(j1, D) rep(i2, L) rep(j2, D) {
			int k = ((i1 * D + j1) * L + i2) * D + j2;
			A[n][k] = mint(n1 + i1).pow(j1) * mint(n2 + i2).pow(j2) * a[n1 + i1][n2 + i2];
		}
	}
	vvm xs;
	gauss_jordan_elimination(A, vm(cE), &xs);

	// 自明解 x = 0 しか存在しない場合は失敗．
	if (xs.empty()) return false;

	a.resize(H + 1);
	repi(i, 0, H) a[i].resize(W + 1);

	// a[0..L)[0..W] を 1 次元 P-recursive を仮定して埋める．（L, D は見直すべき）
	rep(i, L - 1) {
		vm seq(w);
		rep(j, w) seq[j] = a[i][j];

		if (!p_recursive(W, seq, L, D)) return false;
		repi(j, 0, W) a[i][j] = seq[j];
	}

	// a[0..H][0..L) を 1 次元 P-recursive を仮定して埋める．（L, D は見直すべき）
	rep(j, L - 1) {
		vm seq(h);
		rep(i, h) seq[i] = a[i][j];

		if (!p_recursive(H, seq, L, D)) return false;
		repi(i, 0, H) a[i][j] = seq[i];
	}

	// 得られた非自明解 xs.back() から漸化式を復元し，それに基づき a[0..h)[0..w) を延長する．
	auto& x = xs.back();
	repi(n1, 0, H - L + 1) repi(n2, 0, W - L + 1) {
		mint num = 0, dnm = 0;

		rep(i1, L) {
			mint pow_ni1 = 1;
			rep(j1, D) {
				rep(i2, L) {
					mint pow_ni2 = 1;
					rep(j2, D) {
						int k = ((i1 * D + j1) * L + i2) * D + j2;
						mint coef = x[k] * pow_ni1 * pow_ni2;

						if (i1 == L - 1 && i2 == L - 1) dnm += coef;
						else num += coef * a[n1 + i1][n2 + i2];

						pow_ni2 *= n2 + i2;
					}
				}
				pow_ni1 *= n1 + i1;
			}
		}

		// num + dnm * a[n1 + L - 1][n2 + L - 1] = 0（dnm が 0 になるようなら場所を変える）
		a[n1 + L - 1][n2 + L - 1] = -num / dnm;
	}

	return true;
}


//【変数係数線形漸化式の発見（Mathematica）】
/*
Mathematica で以下のプログラムを実行すればよい：

Clear[c, nn, dpsub];
seq = { 愚直に計算した a[1..] } (* 添字が 1 始まりなのに注意！ *);
terms = 3 (* 何項間漸化式の存在を仮定するか *);
degree = 2 (* 係数多項式の次数を何次未満と仮定するか *);
eqs = Table[Sum[c[i, j] (nn - i)^j seq[[nn - i]], {i, 0, terms - 1}, {j, 0, degree - 1}] == 0, {nn, terms, Length@seq}];
fi = FindInstance[eqs, Flatten@Table[c[i, j], {i, 0, terms - 1}, {j, 0, degree - 1}], Integers, 2][[1]]
sol = Solve[Sum[c[i, j] (nn - i)^j dpsub[nn - i], {i, 0, terms - 1}, {j, 0, degree - 1}] == 0 /. fi, dpsub[nn]][[1]]
CForm@FullSimplify@sol[[1, 2]]

時間がかかりすぎるようなら，fi の 1 行を以下の 2 行に置き換える：

eqs = eqs~Join~{c[0, degree-1] == 1} (* 1 に固定する係数の位置は適宜調整する *);
fi = FindInstance[eqs, Flatten@Table[c[i, j], {i, 0, terms - 1}, {j, 0, degree - 1}]][[1]]

コピペ後の整形では以下の関数を利用できる：

auto dpsub = [&](const mint& x) { return dp[x.val()]; };
auto Power = [&](const mint& x, int n) { mint res = 1; rep(hoge, n) res *= x; return res; };

一般項が欲しい場合は最後の 2 行を以下の 2 行に置き換える：

req = (Sum[c[i, j] (nn - i)^j  dpsub[nn - i], {i, 0, terms - 1}, {j, 0, degree - 1}] == 0 /. fi) // FullSimplify
RSolve[req && dpsub[1] == seq[[1]], dpsub[nn], {nn}]

あと積のオーバーフローや逆数にも要注意．

verify : https://atcoder.jp/contests/arc174/tasks/arc174_c
*/


//【変数係数線形漸化式の発見（2 次元，Mathematica）】
/*
Mathematica で以下のプログラムを実行すればよい：

Clear[c, nn1, nn2, dpsub];
tbl = { 愚直に計算した a[1..][1..] } (* 添字が 1 始まりなのに注意！ *);
{terms1, terms2} = {1, 2} (* 何項間漸化式の存在を仮定するか *);
degree = 2 (* 係数多項式の次数を何次未満と仮定するか *);
validQ[n1_, n2_] := True (* tbl[[n1, n2]] が有効な値か *);
eqs = Reap[Do[ sum = 0; Do[If[! validQ[n1 - i1, n2 - i2], sum = -1; Break[]];sum +=c[i1, j1, i2, j2] (n1 - i1)^j1 (n2 - i2)^j2 tbl[[n1 - i1, n2 - i2]];, {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0,terms2 - 1}, {j2, 0, degree - 1}]; If[! (sum === -1), Sow[sum == 0]] , {n1, terms1, Length@tbl}, {n2, terms2, Length@tbl[[n1]]}]][[2, 1]];
fi = FindInstance[eqs, Flatten@Table[c[i1, j1, i2, j2], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}], Integers, 2][[1]];
sol = Solve[Sum[c[i1, j1, i2, j2] (nn1 - i1)^j1 (nn2 - i2)^j2 dpsub[nn1 - i1, nn2 - i2], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}] == 0 /. fi, dpsub[nn1, nn2]][[1]]
CForm@FullSimplify@sol[[1, 2]]

時間がかかりすぎるようなら，fi の 1 行を以下の 2 行に置き換える：

eqs = eqs~Join~{c[0, 0, 0, 0] == 1} (* 1 に固定する係数の位置は適宜調整する *);
fi = FindInstance[eqs, Flatten@Table[c[i1, j1, i2, j2], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}]][[1]];

コピペ後の整形では以下の関数を利用できる：

auto dpsub = [&](const mint& x, const mint& y) { return dp[x.val()][y.val()]; };
auto Power = [&](const mint& x, int n) { mint res = 1; rep(hoge, n) res *= x; return res; };

一般項が欲しい場合は最後の 2 行を以下の 2 行に置き換える：

req = (Sum[c[i1, j1, i2, j2] (nn1 - i1)^j1 (nn2 - i2)^j2 dpsub[nn1 - i1, nn2 - i2], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}] == 0 /. fi) // FullSimplify
RSolve[req && dpsub[nn1, 1] == (nn1 の式), dpsub[nn1, nn2], {nn1, nn2}]

あと積のオーバーフローや逆数にも要注意．

verify : https://atcoder.jp/contests/tupc2023/tasks/tupc2023_l
*/


//【変数係数線形漸化式の発見（3 次元，Mathematica）】
/*
Mathematica で以下のプログラムを実行すればよい：

Clear[c, nn1, nn2, nn3, dpsub];
tbl = { 愚直に計算した a[1..][1..][1..] } (* 添字が 1 始まりなのに注意！ *);
{terms1, terms2, terms3} = {1, 1, 2} (* 何項間漸化式の存在を仮定するか *);
degree = 3 (* 係数多項式の次数を何次未満と仮定するか *);
validQ[n1_, n2_, n3_] := True (* tbl[[n1, n2, n3]] が有効な値か *);
eqs = Reap[Do[ sum = 0; Do[If[! validQ[n1 - i1, n2 - i2, n3 - i3], sum = -1; Break[]];sum +=c[i1, j1, i2, j2, i3, j3] (n1 - i1)^j1 (n2 - i2)^j2 (n3 - i3)^ j3 tbl[[n1 - i1, n2 - i2, n3 - i3]];, {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0,terms2 - 1}, {j2, 0, degree - 1}, {i3, 0, terms3 - 1}, {j3, 0,degree - 1}]; If[! (sum === -1), Sow[sum == 0]] , {n1, terms1, Length@tbl}, {n2, terms2, Length@tbl[[n1]]}, {n3, terms3, Length@tbl[[n1,n2]]} ]][[2, 1]];
fi = FindInstance[eqs, Flatten@Table[c[i1, j1, i2, j2, i3, j3], {i1, 0, terms1 - 1}, {j1, 0,degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}, {i3, 0, terms3 - 1}, {j3, 0, degree - 1}], Integers, 2][[1]];
sol = Solve[ Sum[c[i1, j1, i2, j2, i3, j3] (nn1 - i1)^j1 (nn2 - i2)^j2 (nn3 - i3)^j3 dpsub[nn1 - i1, nn2 - i2, nn3 - i3], {i1, 0,terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0,degree - 1}, {i3, 0, terms3 - 1}, {j3, 0, degree - 1}] == 0 /. fi, dpsub[nn1, nn2, nn3]][[1]];
CForm@FullSimplify@sol[[1, 2]]

時間がかかりすぎるようなら，fi の 1 行を以下の 2 行に置き換える：

eqs = eqs~Join~{c[0, 0, 0, 0, 0, 0] == 1} (* 1 に固定する係数の位置は適宜調整する *);
fi = FindInstance[eqs, Flatten@Table[c[i1, j1, i2, j2, i3, j3], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}, {i3, 0, terms3 - 1}, {j3, 0, degree - 1}]][[1]];

コピペ後の整形では以下の関数を利用できる：

auto dpsub = [&](const mint& x, const mint& y, const mint& z) { return dp[x.val()][y.val()][z.val()]; };
auto Power = [&](const mint& x, int n) { mint res = 1; rep(hoge, n) res *= x; return res; };

一般項が欲しい場合は最後の 2 行を以下の 2 行に置き換える：

req = (Sum[c[i1, j1, i2, j2, i3, j3] (nn1 - i1)^j1 (nn2 - i2)^j2 (nn3 - i3)^j3 dpsub[nn1 - i1, nn2 - i2, nn3 - i3], {i1, 0, terms1 - 1}, {j1, 0, degree - 1}, {i2, 0, terms2 - 1}, {j2, 0, degree - 1}, {i3, 0, terms3 - 1}, {j3, 0, degree - 1}] == 0 /. fi) // FullSimplify
RSolve[req && dpsub[nn1, nn2, 1] == (nn1, nn2 の式), dpsub[nn1, nn2, nn3], {nn1, nn2, nn3}]

あと積のオーバーフローや逆数にも要注意．

verify : https://atcoder.jp/contests/agc070/tasks/agc070_c
*/


//【等差数列区間】
/*
* Arithmetic_range<T>(l, r, m, k) : O(1)
*	x∈[l..r) で x ≡ k (mod m) を満たすものからなる昇順列 a で初期化する．
*
* T sum() : O(1)
*	Σa を返す．
*
* T size() : O(1)
*	a の要素数を返す．
*
* T get(T i) : O(1)
*	a[i] を返す．
*
* T front() : O(1)
*	a の先頭の要素を返す．
*
* T back() : O(1)
*	a の末尾の要素を返す．
*
* T count(T x) : O(1)
*	a に含まれる x の個数 (∈{0,1}) を返す．
*
* T lower_bound(T x) : O(1)
*	a に含まれる x 以上の最小の要素の位置を返す（なければ a.size() を返す)
*
* T upper_bound(T x) : O(1)
*	a に含まれる x より大きいの最小の要素の位置を返す（なければ a.size() を返す)
*/
template <class T>
class Arithmetic_range {
	T li, ri, m, k;

public:
	// x∈[l..r) で x ≡ k (mod m) を満たすものからなる昇順列で初期化する．
	Arithmetic_range(T l, T r, T m, T k_) : m(m), k(k_) {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		Assert(m > 0);

		k %= m;
		if (k < 0) k += m;

		l -= k;
		r -= k;

		li = (l >= 0 ? (l + m - 1) / m : -((-l) / m));
		ri = (r >= 0 ? (r + m - 1) / m : -((-r) / m));
	}
	Arithmetic_range() : li(0), ri(0), m(1), k(0) {}

	// Σa を返す．
	inline T sum() const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		// Σi∈[li..ri) (mi+k)
		return (li + ri - 1) * (ri - li) / 2 * m + (ri - li) * k;
	}

	// a の要素数を返す．
	inline T size() const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		return ri - li;
	}

	// a[i] を返す．
	inline T get(T i) const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		Assert(0 <= i && i < ri - li);
		return m * (li + i) + k;
	}

	// a の先頭の要素を返す．
	inline T front() const {
		// verify : https://atcoder.jp/contests/arc176/tasks/arc176_b

		Assert(ri - li > 0);
		return m * li + k;
	}

	// a の末尾の要素を返す．
	inline T back() const {
		Assert(ri - li > 0);
		return m * (ri - 1) + k;
	}

	// a に含まれる x の個数 (∈{0,1}) を返す．
	inline T count(T x) const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		if ((x - k) % m != 0) return 0;
		T xi = (x - k) / m;
		return li <= xi && xi < ri ? 1 : 0;
	}

	// a に含まれる x 以上の最小の要素の位置を返す（なければ a.size() を返す)
	inline T lower_bound(T x) const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		x -= k;
		T xi = (x >= 0 ? (x + m - 1) / m : -((-x) / m));
		return min(max(xi - li, T(0)), ri - li);
	}

	// a に含まれる x より大きいの最小の要素の位置を返す（なければ a.size() を返す)
	inline T upper_bound(T x) const {
		// verify : https://mojacoder.app/users/shogo314/problems/range_query

		x -= k;
		T xi = (x >= -1 ? (x + m) / m : -((-x - 1) / m));
		return min(max(xi - li, T(0)), ri - li);
	}
};


//【円環上の移動】
/*
* Jump_on_cycle<T>(T n) : O(1)
*	長さ n の円環で初期化する．
*
* T jump_fwd(T s, T i) : O(1)
*	位置 s から前方に i 進んだ位置を返す．
*
* T jump_bak(T s, T i) : O(1)
*	 位置 s から後方に i 進んだ位置を返す．
*
* T dist_fwd(T s, T t)  : O(1)
*	位置 s から位置 t まで前方にいくつ移動すれば到達できるかを返す．
*
* T dist_bak(T s, T t) : O(1)
*	位置 s から位置 t まで後方にいくつ移動すれば到達できるかを返す．
*
* T dist(T s, T t) : O(1)
*	位置 s から位置 t まで最短でいくつ移動すれば到達できるかを返す．
*
* bool orderQ_fwd(T s, T m, T t) : O(1)
*	位置 s から位置 t まで前方に移動する間に位置 m を通るかを返す（両端含む）
*
* bool orderQ_bak(T s, T m, T t) : O(1)
*	位置 s から位置 t まで後方に移動する間に位置 m を通るかを返す（両端含む）
*/
template <class T>
class Jump_on_cycle {
	T n;

public:
	// 長さ n の円環で初期化する．
	Jump_on_cycle(T n) : n(n) {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f
	}
	Jump_on_cycle() : n(0) {}

	// 位置 s から前方に i 進んだ位置を返す．
	T jump_fwd(T s, T i) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return smod(s + i, n);
	}

	// 位置 s から後方に i 進んだ位置を返す．
	T jump_bak(T s, T i) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return smod(s - i, n);
	}

	// 位置 s から位置 t まで前方にいくつ移動すれば到達できるかを返す．
	T dist_fwd(T s, T t) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return smod(t - s, n);
	}

	// 位置 s から位置 t まで後方にいくつ移動すれば到達できるかを返す．
	T dist_bak(T s, T t) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return smod(s - t, n);
	}

	// 位置 s から位置 t まで最短でいくつ移動すれば到達できるかを返す．
	T dist(T s, T t) const {
		// verify : https://atcoder.jp/contests/nupc2024/tasks/nupc2024_j

		T d = smod(t - s, n);
		chmin(d, n - d);
		return d;
	}

	// 位置 s から位置 t まで前方に移動する間に位置 m を通るかを返す（両端含む）
	bool orderQ_fwd(T s, T m, T t) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return dist_fwd(s, m) <= dist_fwd(s, t);
	}

	// 位置 s から位置 t まで後方に移動する間に位置 m を通るかを返す（両端含む）
	bool orderQ_bak(T s, T m, T t) const {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_f

		return dist_bak(s, m) <= dist_bak(s, t);
	}
};


//【下に凸判定（広義）】O(n)
/*
* a[0..n) が広義に下に凸（階差数列が広義単調増加）かを返す．
*/
template <class T>
bool convexQ(const vector<T>& a) {
	int n = sz(a);
	T diff = numeric_limits<T>::lowest();

	rep(i, n - 1) {
		T d = a[i + 1] - a[i];
		if (d < diff) return false;
		// if (d <= diff) return false; // 狭義凸ならこっち
		diff = d;
	}
	return true;
}


//【上に凸判定（広義）】O(n)
/*
* a[0..n) が広義に上に凸（階差数列が広義単調減少）かを返す．
*/
template <class T>
bool concaveQ(const vector<T>& a) {
	int n = sz(a);
	T diff = numeric_limits<T>::max();

	rep(i, n - 1) {
		T d = a[i + 1] - a[i];
		if (d > diff) return false;
		// if (d >= diff) return false; // 狭義凸ならこっち
		diff = d;
	}
	return true;
}


//【凸数列の和 → 凸】
/*
* a[0..n), b[0..n) が広義に下に凸であるとき，
*		c[i] = a[i] + b[i]  (i∈[0..n))
* で定まる数列 c[0..n) も広義に下に凸である．
* 
* (証明)
* a[0..n), b[0..n) は広義に下に凸なので，
*		a[i] - a[i-1] ≦ a[i+1] - a[i]  (i∈[1..n-2])
*		b[i] - b[i-1] ≦ b[i+1] - b[i]  (i∈[1..n-2])
* が成り立つ．c[i] = a[i] + b[i] なので，これらの不等式を辺々加え合わせることにより
*		c[i] - c[i-1] ≦ c[i+1] - c[i]  (i∈[1..n-2])
* なる不等式を得る．よって c[0..n) は広義に下に凸である．
*/


//【下に凸な数列の各点 max → 下に凸】
/*
* a[0..n), b[0..n) が広義に下に凸であるとき，
*		c[i] = max(a[i], b[i])  (i∈[0..n))
* で定まる数列 c[0..n) も広義に下に凸である．
*
* (証明)
* a[0..n), b[0..n) は広義に下に凸なので，
*		a[i] - a[i-1] ≦ a[i+1] - a[i]  (i∈[1..n-2])
*		b[i] - b[i-1] ≦ b[i+1] - b[i]  (i∈[1..n-2])
* が成り立つ．i∈[1..n-2] を任意に固定し，一般性を失わず a[i] ≧ b[i] と仮定すると，
*		(c[i+1] - c[i]) - (c[i] - c[i-1])
*		= c[i-1] + c[i+1] - 2 c[i]
*		= max(a[i-1], b[i-1]) + max(a[i+1], b[i+1]) - 2 max(a[i], b[i])
*		≧ a[i-1] + a[i+1] - 2 a[i]
*		= (a[i+1] - a[i]) - (a[i] - a[i-1])
*		≧ 0
* となるので c[0..n) は広義に下に凸である．
*/


//【下に凸な数列の各点 min → 下に凸とは限らない】
/*
* a[0..n), b[0..n) が広義に下に凸であるとき，
*		c[i] = min(a[i], b[i])  (i∈[0..n))
* で定まる数列 c[0..n) は広義に下に凸であるとは限らない．
* 
* (反例)
*		a = [1, 2, 4]
*		b = [4, 2, 1]
* はそれぞれ下に凸な数列であるが，
*		c = [min(1,4), min(2,2), min(4,1)] = [1, 2, 1]
* は下に凸な数列ではない．
*/


//【ヒストグラムの横切り】
/*
* ヒストグラム hist を横切りしたときの (高さ, 長さ) の組を hls に格納し hls を返す．
* ただし高さが狭義単調増加かつ長さ狭義単調減少となる部分だけを格納する．
*
* hls[i] = {h, l} : 低い方から i 番目の高さが h，長さが l であることを表す．
*/
vector<pli> cut_histogram_horizontal(vl hist) {
	int n = sz(hist);
	vector<pli> hls;

	// ヒストグラムをソートしておく．
	sort(all(hist));

	// 直前に見ていた高さ
	ll bh = -1;

	rep(i, n) {
		// 直前に見ていた高さより高い場合
		if (bh < hist[i]) {
			// 高さと長さの組を格納する．
			hls.push_back({ hist[i], n - i });

			bh = hist[i];
		}
	}

	return hls;
}


//【隣接要素への加算に対する不変量】
/*
* 列 a に対して a[i..i+1] += x という操作だけが許される場合，
* a の交代和 Σi (-1)^i a[i] は不変量となる．
*
* verify : https://atcoder.jp/contests/arc135/tasks/arc135_d
*/


//【非等差数列】
/*
* どの p（素数）要素も等差数列をなさないような集合 a[0..n) は以下の方法で構築できる：
*	a[i] = (i の p-1 進表示を p 進表示だと解釈しなおしたもの)
* 
* これは 0 から順に条件を満たす限り昇順に数を追加していくという貪欲な構成でも得られる．
* 
* verify : https://atcoder.jp/contests/monamieHB2021/tasks/monamieHB2021_b
*/


//【全要素の一致】
/*
* a[0..n) の全要素が等しい ⇔ n Σ( a[0..n)^2 ) = ( Σa[0..n) )^2
* 
*（証明）a[0..n) の全要素が等しいとき，そのときに限り a[0..n) の分散は 0 である．
* 分散 = 2 乗の平均 - 平均の 2 乗 であり，両辺 n^2 倍して移項することで所望の等式を得る．
* 
* verify : https://atcoder.jp/contests/abc315/tasks/abc315_d
*/


//【辞書順最小部分列（長さごと）】
/*
* 辞書順.h へ
*/


