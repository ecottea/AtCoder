#pragma once
#include "header.h"
// ■■■■■ 広義の畳込み ■■■■■


//【畳込みの使い所】
/*
* 数列 a[0..n), b[0..n) が与えられているとする．
* 例えば通常の畳込みは，全ての k∈[0..n) についての Σi∈[0..k] a[i] b[k-i] を
* 一括で O(n log n) で求めたいときに使う．
* 特定の k に対して求めたいだけであれば，愚直に和をとっても O(k) で計算できる．
*
* verify : https://atcoder.jp/contests/abc276/tasks/abc276_g
*/


//【上側畳込み】
/*
* 与えられた a[0..N], b[0..N] に対して
*		c[i] = Σj∈[i..N] a[N+i-j] b[j]
* なる c[0..N] を求めたい場合，convolution(a, b)[N..2N] を取得すればよい．
*
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【畳込み（素朴）】O(n m)
/*
* a[0..n) と b[0..m) を畳み込んだ数列 c[0..n+m-1) を返す．
* すなわち c[k] = Σ_(i+j=k) a[i] b[j] である．
*/
template <class T>
vector<T> naive_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc214/tasks/abc214_g

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vector<T>();

	// c[k] = Σ_(i+j=k) a[i] b[j]
	vector<T> c(n + m - 1);
	rep(i, n) rep(j, m) c[i + j] += a[i] * b[j];

	return c;
}


//【自己畳込み（素朴）】O(n^2 log k)
/*
* a[0..n) を k 個畳み込んで切り詰めた数列 c[0..n) を返す．
*
* 利用：【畳込み（素朴）】
*/
template <class T>
vector<T> naive_self_convolution(const vector<T>& a, ll k) {
	// verify : https://atcoder.jp/contests/arc059/tasks/arc059_d

	int n = sz(a);
	if (n == 0) return vector<T>();

	vector<T> res(n);
	res[0] = 1;

	// 繰り返し二乗法
	vector<T> pow2(a);
	while (k > 0) {
		if (k & 1) {
			res = naive_convolution(res, pow2);
			res.resize(n);
		}

		pow2 = naive_convolution(pow2, pow2);
		pow2.resize(n);

		k /= 2;
	}

	return res;
}


//【畳込み（複数，素朴）】O(n^2)
/*
* 数列の集合 a の要素を全て畳込んだ結果（長さは n）を返す．
*/
template <class T>
vector<T> naive_multi_convoluion(vector<vector<T>> a) {
	// verify : https://atcoder.jp/contests/nomura2020/tasks/nomura2020_d

	int m = sz(a);
	if (m == 0) return vector<T>{ 1 };

	// (要素数, 数列の番号) の組を要素数昇順に記録する．
	priority_queue_rev<pii> q;
	rep(i, m) {
		if (a[i].empty()) return vector<T>();
		q.push({ sz(a[i]), i });
	}

	// 積のコストが小さい順に掛けていく（マージテク）
	while (sz(q) >= 2) {
		auto [ni, i] = q.top(); q.pop();
		auto [nj, j] = q.top(); q.pop();

		vector<T> c(ni + nj - 1);
		rep(x, ni) rep(y, nj) c[x + y] += a[i][x] * a[j][y];

		a[i] = move(c);
		q.push({ ni + nj - 1, i });
	}

	return a[q.top().second];
}


//【二次元畳込み（素朴）】O(h1 w1 h2 w2)
/*
* a[0..h1)[0..w1) と b[0..h2)[0..w2) を畳み込んだ二次元配列 c[0..h1+h2-1)[0..w1+w2-1) を返す．
* すなわち c[k][k'] = Σ_(i+j=k) Σ_(i'+j'=k') a[i][i'] b[j][j'] である．
*/
template <class T>
vector<vector<T>> naive_convolution_2D(const vector<vector<T>>& a, const vector<vector<T>>& b) {
	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	// c[k][k'] = Σ_(i+j=k) Σ_(i'+j'=k') a[i][i'] b[j][j']
	vector<vector<T>> c(h1 + h2 - 1, vector<T>(w1 + w2 - 1));
	rep(i, h1) rep(j, h2) rep(k, w1) rep(l, w2) {
		c[i + j][k + l] += a[i][k] * b[j][l];
	}

	return c;
}


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


//【max-plus 畳込み（上に凸）】O(n + m)
/*
* 上に凸な数列 a[0..n), b[0..m) を max-plus 代数にて畳み込んだ上に凸な数列 c[0..n+m-1) を返す．
* 数列が上に凸であるとは，階差数列が広義単調減少であることをいう．
*/
template <class T>
vector<T> concave_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://twitter.com/maspy_stars/status/1396750434824450051
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_h

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
		int jM = -1; T a_max = numeric_limits<T>::min();
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
* Semi_online_max_plus_convolution<T>(vm b) : O(n)
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


