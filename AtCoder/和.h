#pragma once
#include "header.h"
// ■■■■■ 添字和畳込み（FPS の積） ■■■■■


//【上側畳込み】
/*
* 与えられた a[0..n], b[0..n] に対して
*		c[i] = Σj∈[i..n] a[n+i-j] b[j]
* なる c[0..n] を求めたい場合，convolution(a, b)[n..2n] を取得すればよい．
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


//【畳込み（法が任意）】O((n + m) log(n + m))
/*
* a と b の mod を法とした畳込みを返す．
*/
vi convolution_arbitrary_mod(const vi& a, const vi& b, int mod = (int)1e9 + 7) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vi();

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = smod(a[i], mod);
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = smod(b[i], mod);
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vi c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = (int)((c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2) % mod);
	}

	return c;
}


//【畳込み（法が任意，mint）】O((n + m) log(n + m))
/*
* a と b の mod を法とした畳込みを返す．
*/
vm convolution_arbitrary_mod(const vm& a, const vm& b) {
	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vm();

	int mod = mint::mod();

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = a[i].val();
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = b[i].val();
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vm c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2;
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
* 上に凸な数列 a[0..n) と b[0..m) を max-plus 代数にて畳み込んだ数列 c[0..n+m-1) を返す．
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


//【高速フーリエ変換】
/*
* FFT<D>(int n) : O(n)
*	長さ n 以下の数列を扱えるよう初期化を行う．
*	制約 : D は double または long double（誤差には注意！）
*
* fft(vD a, vcD& A) : O(n log n)
*	実数列 a[0..n) に対し高速フーリエ変換を行った結果を A[0..n) に格納する（cD = complex<D>）
*	制約 : n は 2 の冪乗
*
* ifft(vcD A, vD& a) : O(n log n)
*	複素数列 A[0..n) に対し逆高速フーリエ変換を行った結果を a[0..n) に格納する．
*	制約 : n は 2 の冪乗
*
* vD convolution(vD a, vD b) : O((n + m) log(n + m))
*	実数列 a[0..n) と b[0..m) の畳込みを返す．
*
* vi convolution(vi a, vi b, int mod = 1e9+7) : O(3 (n + m) log(n + m))
*	整数列 a[0..n) と b[0..m) の mod を法とした畳込みを返す．
*
* vD cyclic_convolution(vD a, vD b) : O(n log n)
*	実数列 a[0..n) と b[0..n) の巡回畳込みを返す．
*	制約 : n は 2 の冪乗
*/
template <class D = double>
struct FFT {
	using vD = vector<D>;
	using cD = complex<D>;
	using vcD = vector<cD>;

	// N : 扱える数列の長さの上限（N = 2^M）
	int N, M;

	// w : 1 の原始 2^M 乗根とする．
	// w_pow[i] : w^i，w_pow[N-i] : w^(-i)
	vcD w_pow;

	// 長さ n 以下の数列を扱えるよう初期化を行う．
	FFT(int n) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		// n 以上の最小の 2 冪 2^M を求める．
		M = msb(n - 1) + 1;
		N = 1 << M;

		// w の累乗を前計算しておく．
		w_pow.resize(N + 1); D pi = acos(D(-1));
		repi(i, 0, N) {
			D th = 2 * pi * i / N;
			w_pow[i] = cD(cos(th), sin(th));
		}
	}

	// x を (y, z) に分割する
	void butterfly(const vcD& x, vcD& y, vcD& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[(ll)i << (M - m)];
		}
	}

	// x を (y, z) に分割する（逆変換用）
	void butterfly_inv(const vcD& x, vcD& y, vcD& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[N - (i << (M - m))];
		}
	}

	// (y, z) を x に統合する
	void riffle(const vcD& y, const vcD& z, vcD& x) {
		int n = sz(y);
		x = vcD(2 * n);

		rep(i, n) {
			x[2 * i] = y[i];
			x[2 * i + 1] = z[i];
		}
	}

	// 長さが 2 冪の複素数列 a に対し高速フーリエ変換を行った結果を A に格納する．
	void fft_sub(const vcD& a, vcD& A) {
		int n = sz(a);
		if (n == 1) {
			A = a;
			return;
		}

		vcD b, c, B, C;
		butterfly(a, b, c);
		fft_sub(b, B); fft_sub(c, C);
		riffle(B, C, A);
	}

	// 長さが 2 冪の実数列 a に対し高速フーリエ変換を行った結果を A に格納する．
	void fft(const vD& a_, vcD& A) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(a_);
		Assert(n <= N);

		vcD a(n);
		rep(i, n) a[i] = cD(a_[i], 0);

		fft_sub(a, A);
	}

	// 長さが 2 冪の複素数列 a に対し逆高速フーリエ変換を行った結果を A に格納する．
	void intt_sub(const vcD& A, vcD& a) {
		int n = sz(A);
		if (n == 1) {
			a = A;
			return;
		}

		vcD b, c, B, C;
		butterfly_inv(A, B, C);
		intt_sub(B, b); intt_sub(C, c);
		riffle(b, c, a);
	}

	// 長さが 2 冪の実数列 A に対し逆高速フーリエ変換を行った結果を a に格納する．
	void ifft(const vcD& A, vD& a_) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(A);
		Assert(n <= N);

		vcD a;
		intt_sub(A, a);

		a_.resize(n);
		rep(i, n) a_[i] = a[i].real() / n; // 定数倍も調整しておく
	}

	// a と b の畳込みを返す．
	vD convolution(vD a, vD b) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(a), m = sz(b);
		Assert(n + m - 1 <= N);

		int k = 1 << (msb(n + m - 2) + 1);
		a.resize(k); b.resize(k);

		vcD A, B; fft(a, A); fft(b, B);
		rep(i, k) A[i] *= B[i];
		ifft(A, a);

		a.resize(n + m - 1);

		return a;
	}

	// a と b の畳込みを返す．
	vi convolution(const vi& a, const vi& b, int mod = (int)1e9 + 7) {
		// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6%E3%83%BBnumpy-%E9%AB%98%E9%80%9F%E3%83%95%E3%83%BC%E3%83%AA%E3%82%A8%E5%A4%89%E6%8F%9Bfft%E3%81%AB%E3%82%88%E3%82%8B%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF
		// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

		int n = sz(a), m = sz(b);
		Assert(n + m - 1 <= N);

		vD a0(n), a1(n), b0(m), b1(m); int pow2 = 1 << 15;
		rep(i, n) {
			a0[i] = a[i] % pow2;
			a1[i] = a[i] / pow2;
		}
		rep(i, m) {
			b0[i] = b[i] % pow2;
			b1[i] = b[i] / pow2;
		}

		vD c00_d = this->convolution(a0, b0);
		vD c11_d = this->convolution(a1, b1);
		rep(i, n) a0[i] += a1[i];
		rep(i, m) b0[i] += b1[i];
		vD c01_d = this->convolution(a0, b0);

		vl c00(n + m - 1), c11(n + m - 1), c01(n + m - 1);
		rep(i, n + m - 1) {
			c00[i] = (ll)(c00_d[i] + 0.5) % mod;
			c11[i] = (ll)(c11_d[i] + 0.5) % mod;
			c01[i] = ((ll)(c01_d[i] + 0.5) - c00[i] - c11[i] + 2LL * mod) % mod;
		}

		vi c(n + m - 1);
		rep(i, n + m - 1) {
			c[i] = (int)((c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2) % mod);
		}

		return c;
	}

	// 長さが 2 冪の列 a と b の巡回畳込みを返す．
	vD cyclic_convolution(vD a, vD b) {
		Assert(sz(a) == sz(b) && sz(a) <= N);

		int n = sz(a);

		vcD A, B; fft(a, A); fft(b, B);
		rep(i, n) A[i] *= B[i];
		ifft(A, a);

		return a;
	}
};


//【オンライン畳込み（実数）】
/*
* Online_convolution_double<D>(int n) : O(n)
*	a[0..n) と b[0..n) の畳込み c[0..n) を計算できるよう初期化する．
*	制約 : D は double または long double（誤差には注意！）
*
* void set(D a, D b) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a=a[t], b=b[t] を与える．
*
* D [](int i) : O(1)
*	c[i] = Σj∈[0..i] a[j] b[i-j] を返す．
*	制約 : set を i 回以上呼び出し済でなくてはならない．
*
* void update(int i, D c) : O(1)
*	c[i] を強制的に c に書き換える．
*
* 利用：【高速フーリエ変換】
*/
template <class D = double>
class Online_convolution_double {
	// 参考 : https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c

	int n, t; // t : 次が何回目の呼び出しか
	vector<D> as, bs, cs;
	FFT<D> fft;

public:
	// 長さ n の数列同士の畳込みを行えるよう初期化する．
	Online_convolution_double(int n) : n(n), t(0), as(n), bs(n), cs(n), fft(n) {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c
	}
	Online_convolution_double() : n(0), t(0) {}

	// t 回目に呼び出すときは，a=a[t], b=b[t] を与える．
	void set(D a, D b) {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c

		as[t] = a; bs[t] = b;

		int i1_max = lsb(t + 2), i2_max = i1_max;

		// 対角線上の正方形領域に対する処理を行う場合
		if (popcount(t + 2) == 1) { i1_max -= 2; i2_max -= 1; }

		// 2^i : 正方形の一辺の長さ（対角線より下）
		repi(i, 0, i1_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bs[y_min..y_min+len) を畳み込む．
			int x_min = t + 1 - (1 << i);
			int y_min = (1 << i) - 1;

			vector<D> as_sub, bs_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bs_sub));

			vector<D> cs_sub = fft.convolution(as_sub, bs_sub);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		// 2^i : 正方形の一辺の長さ（対角線以上）
		repi(i, 0, i2_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bs[y_min..y_min+len) を畳み込む．
			int x_min = (1 << i) - 1;
			int y_min = t + 1 - (1 << i);

			vector<D> as_sub, bs_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bs_sub));

			vector<D> cs_sub = fft.convolution(as_sub, bs_sub);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		t++;
	}

	// c[i] を返す．
	D const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c

		Assert(i < t);

		return cs[i];
	}

	// c[i] を強制的に c に変更する．
	void update(int i, D c) {
		cs[i] = c;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Online_convolution_double& c) {
		os << "a: " << c.as << endl;
		os << "b: " << c.bs << endl;
		os << "c: " << c.cs;
		return os;
	}
#endif
};


//【オンライン畳込み（片側固定，実数）】
/*
* Semi_online_convolution_double<D>(vD b) : O(n)
*	a[0..n) と固定された b[0..n) の畳込み c[0..n) を計算できるよう初期化する．
*	制約 : D は double または long double（誤差には注意！）
*
* void set(D b) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a=a[t] を与える．
*
* D [](int i) : O(1)
*	c[i] = Σj∈[0..i] a[j] b[i-j] を返す．
*	制約 : set を i 回以上呼び出し済でなくてはならない．
*
* void update(int i, D c) : O(1)
*	c[i] を強制的に c に書き換える．
*
* 利用：【高速フーリエ変換】
*/
template <class D = double>
class Semi_online_convolution_double {
	// 参考 : https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c

	int n, t; // t : 次が何回目の呼び出しか
	vector<D> as, cs; vector<vector<D>> bss;
	FFT<D> fft;

public:
	// 長さ n の数列同士の畳込みを行えるよう初期化する．
	Semi_online_convolution_double(const vector<D>& bs) : n(sz(bs)), t(0), as(n), cs(n), bss(msb(n) + 1), fft(n) {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c

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
	Semi_online_convolution_double() : n(0), t(0) {}

	// t 回目に呼び出すときは，a=a[t] を与える．
	void set(D a) {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c

		as[t] = a;

		// b[0], b[1] との積だけは例外処理
		cs[t] += as[t] * bss[0][0];
		if (t + 1 < n) cs[t + 1] += as[t] * bss[0][1];

		int i_max = lsb(t);

		// 2^i : 正方形の一辺の長さ
		repi(i, 1, i_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bss[i] を畳み込む．
			int x_min = t - (1 << i);

			vector<D> as_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));

			vector<D> cs_sub = fft.convolution(as_sub, bss[i]);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		t++;
	}

	// c[i] を返す．
	D const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/jag2017summer-day1/tasks/jag2017summer_day1_c

		Assert(i < t);

		return cs[i];
	}

	// c[i] を強制的に c に変更する．
	void update(int i, D c) {
		cs[i] = c;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Semi_online_convolution_double& c) {
		os << "a: " << c.as << endl;
		os << "c: " << c.cs;
		return os;
	}
#endif
};


