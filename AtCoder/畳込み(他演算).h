#pragma once
#include "header.h"
// ■■■■■ 広義の畳込み ■■■■■


//【max-plus 畳込み（素朴）】O(n m)
/*
* 数列 a[0..n) と b[0..m) を max-plus 代数にて畳み込んだ数列 c[0..n+m-1) を返す．
* すなわち c[k] = MAX_(i+j=k) (a[i] + b[j]) である．
*/
template <class T>
vector<T> naive_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	int n = sz(a), m = sz(b);
	if (min(n, m) == 0) return vector<T>();

	T T_MIN = numeric_limits<T>::lowest();

	// c[k] = MAX_(i+j=k) (a[i] + b[j])
	vector<T> c(n + m - 1, T_MIN);
	rep(i, n) rep(j, m) chmax(c[i + j], a[i] + b[j]);

	return c;
}


//【max-plus 畳込み（広義単調減少）】O(n m)
/*
* 広義単調減少な数列 a, b を max-plus 代数にて畳み込んだ数列 c は広義単調減少である．
* ただし，だからといって高速に畳込みが計算できるというわけではない．
*/


//【max-plus 畳込み（上に凸）】O(n + m)
/*
* 上に凸な数列 a[0..n), b[0..m) を max-plus 代数にて畳み込んだ上に凸な数列 c[0..n+m-1) を返す．
* 数列が上に凸であるとは，階差数列が広義単調減少であることをいう．
*/
template <class T>
vector<T> concave_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://twitter.com/maspy_stars/status/1396750434824450051
	// verify : https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex

	//【方法】
	// 添字が小さい順に c を決定していく．c[0], c[1] は
	//		c[0] = a[0] + b[0]
	//		c[1] = max(a[0] + b[1], a[1] + b[0])
	// となる．
	// 
	// c[1] = a[0] + b[1] の場合を例として考える．c[2] は定義通りだと
	//		c[2] = max(a[0] + b[2], a[1] + b[1], a[2] + b[0])
	// であるが，
	//		a[2] + b[0]
	//		= (a[1] + b[0]) + (a[2] - a[1])
	//		≦ (a[0] + b[1]) + (a[2] - a[1]) （c[1] = a[0] + b[1] より）
	//		≦ (a[0] + b[1]) + (a[1] - a[0]) （a が上に凸より）
	//		= a[1] + b[1]
	// なので，候補を 2 つに絞り
	//		c[2] = max(a[0] + b[2], a[1] + b[1])
	// としてしまって良い．
	//
	// c[3] 以降も同様に考え候補を 2 つに絞ることができる．

	int n = sz(a), m = sz(b);

	// 一方が空数列だった場合は空数列を返す．
	if (min(n, m) == 0) return vector<T>();

	vector<T> c(n + m - 1);
	c[0] = a[0] + b[0];

	int i = 0, j = 0;
	while (i + j < n + m - 2) {
		if (i == n - 1 || (j != m - 1 && a[i + 1] - a[i] < b[j + 1] - b[j])) {
			c[i + j + 1] = c[i + j] + (b[j + 1] - b[j]);
			j++;
		}
		else {
			c[i + j + 1] = c[i + j] + (a[i + 1] - a[i]);
			i++;
		}
	}

	return c;
}


//【max-plus 畳込み（片方が上に凸）】O(n log(n + m) + m)
/*
*（上に凸とは限らない）数列 a[0..n) と上に凸な数列 b[0..m) を
* max-plus 代数にて畳み込んだ（上に凸とは限らない）数列 c[0..n+m-1) を返す．
* 数列が上に凸であるとは，階差数列が広義単調減少であることをいう．
*/
template <class T>
vector<T> semiconcave_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://noshi91.github.io/Library/algorithm/concave_max_plus_convolution.cpp
	// verify : https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary

	//【方法】
	// (n+m-1)×n 行列 M を
	//		M[i][j] = a[j] + b[i-j]
	// と定めると，これは anti-monotone なので monotone minima で行最小値が求まる．

	//【例（n=4, m=3）】
	// M は以下の通り．これの各行の行最大値を求めれば良い．
	//	[ a[0]+b[0]                               ]
	//	[ a[0]+b[1] a[1]+b[0]                     ]
	//	[ a[0]+b[2] a[1]+b[1] a[2]+b[0]           ]
	//	[           a[1]+b[2] a[2]+b[1] a[3]+b[0] ]
	//	[                     a[2]+b[2] a[3]+b[1] ]
	//	[                               a[3]+b[2] ]

	//【備考】
	// 実は M は anti-totally monotone なので，SMAWK Algorithm で O(log n) を落とせる．

	int n = sz(a), m = sz(b);

	// 一方が空数列だった場合は空数列を返す．
	if (min(n, m) == 0) return vector<T>();

	vector<T> c(n + m - 1);

	// 行 [iL..iR) についての答えを求める（答えが列 [jL..jR) の範囲にあることはわかっている）
	function<void(int, int, int, int)> rf = [&](int iL, int iR, int jL, int jR) {
		if (iR - iL <= 0) return;

		// iM : 行 [iL..iR) の真ん中の行の番号
		int iM = (iL + iR) / 2;

		// jM : 行 iM の中の最大要素のある列の番号
		int jM = -1; T a_max = -2 * T(INFL);
		repi(j, max(jL, iM - m + 1), min(jR - 1, iM)) {
			if (chmax(a_max, a[j] + b[iM - j])) jM = j;
		}

		// 行 iM の行最大値の位置が jM であることがわかったので c[iM] が決定できる．
		c[iM] = a[jM] + b[iM - jM];

		// 左上と右下の部分を再帰的に調べていく．
		rf(iL, iM, jL, jM + 1);
		rf(iM + 1, iR, jM, jR);
	};
	rf(0, n + m - 1, 0, n);

	return c;
}


//【max-plus オンライン畳込み（片側固定，上に凸）】
/*
* Semi_online_max_plus_convolution<T>(vT b) : O(n)
*	a[0..n) と固定された上に凸な b[0..n) の畳込み c[0..n) を計算できるよう初期化する．
*	制約：b[0..n) は上に凸
*
* void set(T a) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a=a[t] を与える．
*
* T [](int i) : O(1)
*	c[i] = MAX_j∈[0..i] (a[j] + b[i-j]) を返す．
*	制約：a[0..i] を指定済でなくてはならない．
*
* void update(int i, T c) : O(1)
*	c[i] を強制的に c に書き換える．
*
* 利用：【max-plus 畳込み（片方が上に凸）】
*/
template <class T>
class Semi_online_max_plus_convolution {
	// 参考 : https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c

	int n, t; // t : 次が何回目の呼び出しか
	vector<T> as, cs; vector<vector<T>> bss;

public:
	// 長さ n の数列同士の畳込みを行えるよう初期化する．
	Semi_online_max_plus_convolution(const vector<T>& bs)
		: n(sz(bs)), t(0), as(n), cs(n, numeric_limits<T>::lowest()), bss(msb(n) + 1) {
		// b[0], b[1] だけは例外的に bss[0] に格納しておく．
		int len = min(2, n);
		copy(bs.begin(), bs.begin() + len, back_inserter(bss[0]));

		// b[2..n) を幅 2^i の区間にあらかじめ分割しておく．
		repi(i, 1, msb(n)) {
			int y_min = 1 << i;
			int len = min(1 << i, n - y_min);
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bss[i]));
		}
	}
	Semi_online_max_plus_convolution() : n(0), t(0) {}

	// t 回目に呼び出すときは，a=a[t] を与える．
	void set(T a) {
		as[t] = a;

		// b[0], b[1] との和だけは例外処理
		chmax(cs[t], as[t] + bss[0][0]);
		if (t + 1 < n) chmax(cs[t + 1], as[t] + bss[0][1]);

		int i_max = lsb(t);

		// 2^i : 正方形の一辺の長さ
		repi(i, 1, i_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bss[i] を畳み込む．
			int x_min = t - (1 << i);

			vector<T> as_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));

			vector<T> cs_sub = semiconcave_max_plus_convolution(as_sub, bss[i]);
			repi(j, 0, j_max) chmax(cs[t + j], cs_sub[j]);
		}

		t++;
	}

	// c[i] を返す．
	T const& operator[](int i) const {
		Assert(i < t);

		return cs[i];
	}

	// c[i] を強制的に c に変更する．
	void update(int i, T c) {
		cs[i] = c;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Semi_online_max_plus_convolution& c) {
		os << "a: " << c.as << endl;
		os << "c: " << c.cs;
		return os;
	}
#endif
};


//【sum-AND 畳込み】O((n + m) log(n + m) log max(a, b))
/*
* 与えられた数列 a[0..n) と b[0..m) について，
*	c[k] = Σ_(i+j=k) (a[i] AND b[j])
* で定まる数列 c[0..n+m-1) を返す．
*/
template <class T>
vector<T> sum_AND_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://mojacoder.app/users/uni_kakurenbo/contests/TG-BsAC-003/tasks/14

	//【方法】
	// ビットごとに独立に考えれば，AND は積とみなせるので通常の畳込みを用いることができる．

	int n = sz(a), m = sz(b);
	if (min(n, m) == 0) return vector<T>();

	int K = msb(max(*max_element(all(a)), *max_element(all(b)))) + 1;

	vector<T> c(n + m - 1);
	rep(k, K) {
		vm ak(n), bk(m);
		rep(i, n) ak[i] = getb(a[i], k);
		rep(j, m) bk[j] = getb(b[j], k);

		// intt() をまとめれば定数倍を 2/3 に抑えられる．
		auto ck = convolution(ak, bk);

		rep(i, n + m - 1) c[i] += T(ck[i].val()) << k;
	}

	return c;
}


//【sum-OR 畳込み】O((n + m) log(n + m) log max(a, b))
/*
* 与えられた数列 a[0..n) と b[0..m) について，
*	c[k] = Σ_(i+j=k) (a[i] OR b[j])
* で定まる数列 c[0..n+m-1) を返す．
*/
template <class T>
vector<T> sum_OR_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc291/tasks/abc291_g

	//【方法】
	// ビットごとに独立に考えれば，OR は 1 - 積とみなせるので通常の畳込みを用いることができる．

	int n = sz(a), m = sz(b);
	if (min(n, m) == 0) return vector<T>();

	int K = msb(max(*max_element(all(a)), *max_element(all(b)))) + 1;

	vector<T> c(n + m - 1);
	rep(k, K) {
		vm ak(n), bk(m);
		rep(i, n) ak[i] = 1 - getb(a[i], k);
		rep(j, m) bk[j] = 1 - getb(b[j], k);

		// intt() をまとめれば定数倍を 2/3 に抑えられる．
		auto ck = convolution(ak, bk);

		rep(i, n + m - 1) {
			//		c[i] = Σt (a[t] OR b[i-t])
			// であるが，意味のある t の範囲は
			//		0 ≦ t ≦ n-1 かつ 0 ≦ i-t ≦ m-1
			//		max(0, i-m+1) ≦ t ≦ min(n-1,i)
			int t_min = max(0, i - m + 1);
			int t_max = min(n - 1, i);
			c[i] += T((t_max - t_min + 1) - ck[i].val()) << k;
		}
	}

	return c;
}


//【畳込み ⇔ モノイド半環上の積】
/*
* R(+,×) を要素の属する半環，M(o) を添字の属するモノイドとし，モノイド半環 R[M] を考える．
* R[M] の元，すなわち M(o) の元の R(+,×) 係数の形式和
*	A = Σi a[i] [i]
*	B = Σj b[j] [j]
* の積 C は
*	C = Σk c[k] [k]
*	c[k] = +_(ioj=k) a[i] × b[j]
* によって定義され，数列 c は数列 a, b のある種の畳込みとなる．
*/


//【畳込み ⇔ 級数の積】
/*
* M(o) が特定のモノイドのとき，数列の畳込みをある種の自然な級数の積と解釈できる．
*
* o が和 + : 形式的冪級数
*	数列 a, b を畳込んだ数列 c が
*		c[k] = Σ_(i+j=k) a[i] b[j]
*	で定義されるとき，数列に対応する形式的冪級数を
*		A(z) = Σi a[i] z^i
*	などとおけば，
*		A(z) B(z) = C(z)
*	が成り立つ．
*
* o が積 × : ディリクレ級数
*	数列 a, b を畳込んだ数列 c が
*		c[k] = Σ_(i×j=k) a[i] b[j]
*	で定義されるとき，数列に対応するディリクレ級数を
*		A(s) = Σi a[i] / i^s
*	などとおけば，
*		A(s) B(s) = C(s)
*	が成り立つ．
*/


