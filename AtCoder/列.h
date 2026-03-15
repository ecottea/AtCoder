#pragma once
#include "header.h"
// ■■■■■ 列 ■■■■■


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


