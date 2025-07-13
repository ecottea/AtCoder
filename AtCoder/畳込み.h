#pragma once
#include "header.h"
// ■■■■■ 畳込み ■■■■■


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
	if (n < m) {
		rep(i, n) rep(j, m) c[i + j] += a[i] * b[j];
	}
	else {
		rep(j, m) rep(i, n) c[i + j] += a[i] * b[j];
	}

	return c;
}


//【畳込みの表現行列】
/*
* 例えば [0..4) との畳込み
*		g[k] = Σ_(i+j=k) f[i] j
* の表現行列は次のようになる：
*		[g[0]]   [0        ] [f[0]]
*		[g[1]]   [1 0      ] [f[1]]
*		[g[2]]   [2 1 0    ] [f[2]]
*		[g[3]] = [3 2 1 0  ] [f[3]]
*		[g[4]]   [  3 2 1 0] [f[4]]
*		[g[5]]   [    3 2 1]
*		[g[6]]   [      3 2]
*		[g[7]]   [        3]
*/


//【畳込みの使い所】
/*
* 数列 a[0..n), b[0..n) が与えられているとする．
* 例えば通常の畳込みは，全ての k∈[0..n) についての Σi∈[0..k] a[i] b[k-i] を
* 一括で O(n log n) で求めたいときに使う．
* 特定の K に対して求めたいだけであれば，愚直に和をとっても O(K) で計算できる．
*
* verify : https://atcoder.jp/contests/abc276/tasks/abc276_g
*/


//【フィルタリング（素朴）】O(n m)
/*
* a[0..n+m-1) にフィルタ b[0..m) をかけた結果 c[0..n) を返す．c[i] は以下の式で表される：
*		c[i] = Σj∈[0..m) a[i+j] b[j]
* i はフィルタの平行移動量を表す．
*/
template <class T>
vector<T> naive_filtering(const vector<T>& a, const vector<T>& b) {
	int m = sz(b), n = sz(a) - m + 1;
	if (n <= 0) return vector<T>();

	// c[i] = Σj∈[0..m) a[i+j] b[j]
	vector<T> c(n);
	rep(i, n) rep(j, m) c[i] += a[i + j] * b[j];

	return c;
}


//【フィルタリングの表現行列】
/*
* 例えば [0..4) によるフィルタリング
*		g[k] = Σ_(i+k=j) f[i] j
* の表現行列は次のようになる：
*		[g[0]]   [0 1 2 3        ] [f[0]]
*		[g[1]]   [  0 1 2 3      ] [f[1]]
*		[g[2]] = [    0 1 2 3    ] [f[2]]
*		[g[3]]   [      0 1 2 3  ] [f[3]]
*		[g[4]]   [        0 1 2 3] [f[4]]
*                                  [f[5]]
*                                  [f[6]]
*                                  [f[7]]
* 
* また [0..8) へのフィルタリング
*		g[k] = Σ_(i+k=j) i f[j]
* の表現行列は次のようになる：
*		[g[0]]   [0 1 2 3] [f[0]]
*		[g[1]]   [1 2 3 4] [f[1]]
*		[g[2]] = [2 3 4 5] [f[2]]
*		[g[3]]   [3 4 5 6] [f[3]]
*		[g[4]]   [4 5 6 7]
*/


//【上側畳込み】
/*
* 与えられた a[0..N], b[0..N] に対して
*		c[i] = Σj∈[i..N] a[N+i-j] b[j]
* なる c[0..N] を求めたい場合，convolution(a, b)[N..2N] を取得すればよい．
*
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【自己畳込み（素朴）】O(n^2 log k)
/*
* a[0..n) を k 個畳み込んで切り詰めた数列 c[0..n) を返す．
*
* 利用：【畳込み（素朴）】
*/
template <class T>
vector<T> naive_self_convolution(const vector<T>& a, ll k) {
	// verify : https://atcoder.jp/contests/arc059/tasks/arc059_d

	//【備考】
	// a[0], [1..n) に逆元が存在するならば，FPS(スパース).h の【累乗（スパース）】のように
	// 漸化式を用いて計算していくことで O(n^2) を実現できる．

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
	// verify : https://projecteuler.net/problem=181
	
	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	// c[k][k'] = Σ_(i+j=k) Σ_(i'+j'=k') a[i][i'] b[j][j']
	vector<vector<T>> c(h1 + h2 - 1, vector<T>(w1 + w2 - 1));
	rep(i, h1) rep(j, h2) rep(k, w1) rep(l, w2) {
		c[i + j][k + l] += a[i][k] * b[j][l];
	}

	return c;
}


//【乗算畳込み（mod p）】
/*
* Multiplicative_convolution_prime_mod(int p) : O(√p)
*	添字の法を p として初期化する．
*	制約 : p は素数
*
* vm convolution(vm a, vm b) : O(p log p)
*	与えられた a[0..p), b[0..p) に対し，
*		c[k] = Σ_(i×j=k (mod p)) a[i] b[j]
*	なる c[0..p) を返す．
*/
class Multiplicative_convolution_prime_mod {
	// p : 法（素数）
	int p;

	// r_pow[i] : r^i（r は mod p での原始根）
	vi r_pow;

public:
	// 添字の法を p として初期化する．
	Multiplicative_convolution_prime_mod(int p) : p(p), r_pow(p - 1) {
		// verify : https://judge.yosupo.jp/problem/mul_modp_convolution

		// qs : p-1 の素因数
		vi qs; int m = p - 1;
		for (int i = 2; i * i <= m; i++) {
			if (m % i == 0) qs.emplace_back(i);
			while (m % i == 0) m /= i;
		}
		if (m > 1) qs.emplace_back(m);

		mt19937_64 mt((int)time(NULL));
		uniform_int_distribution<ll> rnd(1, p - 1);

		using mint_p = dynamic_modint<8583928>;
		mint_p::set_mod(p);

		// r : mod p-1 での原始根
		ll r = -1;
		while (r == -1) {
			// r : 原始根の候補をランダムに選ぶ
			r = rnd(mt);

			// p-1 の任意の素因数 q について r^((p-1)/q) が 1 でないことが
			// r が原始根であるための必要十分条件となる．
			repe(q, qs) {
				if (mint_p(r).pow((p - 1) / q) == 1) {
					r = -1;
					break;
				}
			}
		}

		r_pow[0] = 1;
		rep(i, p - 2) r_pow[i + 1] = (int)(r_pow[i] * r % p);
	}

	// 与えられた a[0..p), b[0..p) に対し，c[k] = Σ_(i×j=k (mod p)) a[i] b[j] なる c[0..p) を返す．
	vm conv(const vm& a, const vm& b) {
		// verify : https://judge.yosupo.jp/problem/mul_modp_convolution

		vm a2(p - 1), b2(p - 1);
		rep(i, p - 1) {
			a2[i] = a[r_pow[i]];
			b2[i] = b[r_pow[i]];
		}

		// 巡回畳込み（c[0] だけは例外処理）
		vm c2 = convolution(a2, b2);

		vm c(p);
		rep(i, sz(c2)) c[r_pow[i % (p - 1)]] += c2[i];
		rep(i, p) c[0] += a[0] * b[i];
		repi(i, 1, p - 1) c[0] += a[i] * b[0];

		return c;
	}
};


//【乗算畳込み（mod 2^N）】
/*
* Multiplicative_convolution_prime_mod(int N) : O(2^N)
*	添字の法を 2^N として初期化する．
*
* vm convolution(vm a, vm b) : O(2^N N)
*	与えられた a[0..2^N), b[0..2^N) に対し，
*		c[k] = Σ_(i×j=k (mod 2^N)) a[i] b[j]
*	なる c[0..2^N) を返す．
*/
class Multiplicative_convolution_2power_mod {
	int N;

	// pow3[t][d][i] : (-1)^t 3^i 2^d mod 2^N
	vvvi pow3;

	// pow_inv2[d] : 2^(-d)
	vm pow_inv2;

public:
	// 添字の法を 2^N として初期化する．
	Multiplicative_convolution_2power_mod(int N) : N(N), pow3(2, vvi(N)), pow_inv2(N + 1) {
		// verify : https://judge.yosupo.jp/problem/mul_mod2n_convolution

		pow_inv2[0] = 1;
		mint inv2 = mint(2).inv();
		repi(i, 1, N) pow_inv2[i] = pow_inv2[i - 1] * inv2;

		if (N == 0) return;
		pow3[0][N - 1] = { 1 << (N - 1) };
		pow3[1][N - 1] = { 1 << (N - 1) };

		if (N == 1) return;
		pow3[0][N - 2] = { 1 << (N - 2), 3 << (N - 2) };
		pow3[1][N - 2] = { 3 << (N - 2), 1 << (N - 2) };

		if (N == 2) return;
		int W = 1 << (N - 2); int maskN = (1 << N) - 1;
		pow3[0][0].resize(W);
		pow3[0][0][0] = 1;
		rep(i, W - 1) pow3[0][0][i + 1] = (pow3[0][0][i] * 3) & maskN;

		repi(d, 1, N - 3) {
			W >>= 1;
			pow3[0][d].resize(W);
			rep(i, W) pow3[0][d][i] = (pow3[0][d - 1][i] << 1) & maskN;
		}

		repi(d, 0, N - 3) {
			int W = sz(pow3[0][d]);
			pow3[1][d].resize(W);
			rep(i, W) pow3[1][d][i] = (-pow3[0][d][i]) & maskN;
		}
	}

	// 与えられた a[0..2^N), b[0..2^N) に対し，c[k] = Σ_(i×j=k (mod 2^N)) a[i] b[j] なる c[0..2^N) を返す．
	vm conv(const vm& a, const vm& b) {
		// verify : https://judge.yosupo.jp/problem/mul_mod2n_convolution

		vm c(1LL << N);
		rep(i, 1 << N) c[0] += a[0] * b[i];
		repi(i, 1, (1 << N) - 1) c[0] += a[i] * b[0];

		rep(da, N) rep(db, N) {
			int d = da + db;

			int ta_ub = (da >= N - 2 ? 1 : 2);
			int tb_ub = (db >= N - 2 ? 1 : 2);

			if (d >= N) {
				mint a_sum, b_sum;
				rep(ta, ta_ub) repe(i, pow3[ta][da]) a_sum += a[i];
				rep(tb, tb_ub) repe(i, pow3[tb][db]) b_sum += b[i];
				c[0] += a_sum * b_sum;
				continue;
			}

			int W = sz(pow3[0][d]); int maskW = W - 1;

			rep(ta, ta_ub) rep(tb, tb_ub) {
				vm a2(W), b2(W);

				int Wa = sz(pow3[ta][da]), Wb = sz(pow3[tb][db]);
				rep(i, Wa) a2[i & maskW] += a[pow3[ta][da][i]];
				rep(i, Wb) b2[i & maskW] += b[pow3[tb][db][i]];

				internal::butterfly(a2);
				internal::butterfly(b2);

				rep(i, W) a2[i] *= b2[i];

				internal::butterfly_inv(a2);

				int t = ta ^ tb;
				rep(i, W) c[pow3[t][d][i]] += a2[i] * pow_inv2[msb(W)];
			}
		}

		return c;
	}
};


//【切り捨て商畳込み（商ごと）】O(m + n log n)
/*
* 与えられた a[0..n), b[0..m) に対して
*       c[k] = Σ_(floor(i/j) = k) a[i] b[j]
* なる c[0..n) を返す．
*/
template<class T>
vector<T> floordiv_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://yukicoder.me/problems/no/1233

	//【方法】
	// j=0 は無視する．和の範囲についての条件は
	//		floor(i/j) = k
	//		⇔ k ≦ i/j < k+1
	//		⇔ j k ≦ i < j k + j
	// と書き直せるので，
	//		c[k]
	//		= Σj∈[1..m) Σi∈[j k..j k + j) a[i] b[j]
	//		= Σj∈[1..m) b[j] Σa[j k..j k + j)
	// となる．a の累積和を前計算しておけば，
	// k ごとに独立に c[k] を計算しても計算量は調和級数で抑えられる．

	int n = sz(a), m = sz(b);

	vector<T> acc_a(n + 1);
	rep(i, n) acc_a[i + 1] = acc_a[i] + a[i];

	vector<T> c(n);
	repi(j, 1, m - 1) c[0] += b[j] * acc_a[min(j, n)];
	repi(k, 1, n - 1) {
		repi(j, 1, min(m - 1, n / k)) {
			c[k] += b[j] * (acc_a[min(j * k + j, n)] - acc_a[j * k]);
		}
	}

	return c;
}


//【切り捨て商畳込み（商ごと）の表現行列】
/*
* 例えば [1..5] との切り捨て商畳込み（商ごと）
*		g[k] = Σ_(floor(i/j) = k) f[i] j
* の表現行列は次のようになる：
*		[g[0]]   [1+2+3+4+5 2+3+4+5 3+4+5 4+5 5        ] [f[0]]
*		[g[1]]   [          1       2     2+3 3+4 3+4+5] [f[1]]
*		[g[2]]   [                  1         2   2    ] [f[2]]
*		[g[3]] = [                        1            ] [f[3]]
*		[g[4]]   [                            1        ] [f[4]]
*		[g[5]]   [                                1    ] [f[5]]
* 
* また [0..5] との切り捨て商畳込み（商ごと）
*		g[k] = Σ_(floor(i/j) = k) i f[j]
* の表現行列は次のようになる：
*		[g[0]]   [0 0+1 0+1+2 0+1+2+3 0+1+2+3+4]
*		[g[1]]   [1 2+3 3+4+5 4+5     5        ] [f[1]]
*		[g[2]]   [2 4+5                        ] [f[2]]
*		[g[3]] = [3                            ] [f[3]]
*		[g[4]]   [4                            ] [f[4]]
*		[g[5]]   [5                            ] [f[5]]
*/


//【切り捨て商畳込み（分母ごと）】O(m + n log n)
/*
* 与えられた a[0..n), b[0..m) に対して
*       c[k] = Σ_(floor(i/k) = j) a[i] b[j]
* なる c[0..n] を返す．
*/
template<class T>
vector<T> floordiv_convolution_by_dnm(const vector<T>& a, const vector<T>& b) {
	// verify : https://yukicoder.me/problems/no/3054

	//【方法】
	// 和の範囲についての条件は
	//		floor(i/k) = j
	//		⇔ j ≦ i/k < j+1
	//		⇔ j k ≦ i < j k + k
	// と書き直せるので，
	//		c[k]
	//		= Σj∈[0..m) Σi∈[j k..j k + k) a[i] b[j]
	//		= Σj∈[0..m) b[j] Σa[j k..j k + k)
	// となる．a の累積和を前計算しておけば，
	// k ごとに独立に c[k] を計算しても計算量は調和級数で抑えられる．

	int n = sz(a), m = sz(b);

	vector<T> acc_a(n + 1);
	rep(i, n) acc_a[i + 1] = acc_a[i] + a[i];

	vector<T> c(n + 1);
	repi(k, 1, n) {
		repi(j, 0, min(m - 1, n / k)) {
			c[k] += b[j] * (acc_a[min(j * k + k, n)] - acc_a[j * k]);
		}
	}

	return c;
}


//【切り捨て商畳込み（分母ごと）の表現行列】
/*
* 例えば [0..5] との切り捨て商畳込み（分母ごと）
*		g[k] = Σ_(floor(i/k) = j) f[i] j
* の表現行列は次のようになる：
*		                       [f[0]]
*		[g[1]]   [0 1 2 3 4 5] [f[1]]
*		[g[2]]   [0 0 1 1 2 2] [f[2]]
*		[g[3]] = [0 0 0 1 1 1] [f[3]]
*		[g[4]]   [0 0 0 0 1 1] [f[4]]
*		[g[5]]   [0 0 0 0 0 1] [f[5]]
*
* また
*		g[k] = Σ_(floor(i/k) = j) i f[j]
* の表現行列は次のようになる：
*		                                     [f[0]]
*		[g[1]]   [0         1     2   3 4 5] [f[1]]
*		[g[2]]   [0+1       2+3   4+5      ] [f[2]]
*		[g[3]] = [0+1+2     3+4+5          ] [f[3]]
*		[g[4]]   [0+1+2+3   4+5            ] [f[4]]
*		[g[5]]   [0+1+2+3+4 5              ] [f[5]]
*/


//【切り上げ商畳込み（分母ごと）】O(m + n log n)
/*
* 与えられた a[0..n), b[0..m) に対して
*       c[k] = Σ_(ceil(i/k) = j) a[i] b[j]
* なる c[0..n] を返す．
*/
template<class T>
vector<T> ceildiv_convolution_by_dnm(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/arc126/tasks/arc126_c

	//【方法】
	// 和の範囲についての条件は
	//		ceil(i/k) = j
	//		⇔ j-1 < i/k ≦ j
	//		⇔ j k - k < i ≦ j k
	// と書き直せるので，
	//		c[k]
	//		= Σj∈[0..m) Σi∈(j k - k..j k] a[i] b[j]
	//		= Σj∈[0..m) b[j] Σa[j k - k + 1..j k + 1)
	// となる．a の累積和を前計算しておけば，
	// k ごとに独立に c[k] を計算しても計算量は調和級数で抑えられる．

	int n = sz(a), m = sz(b);

	vector<T> acc_a(n + 1);
	rep(i, n) acc_a[i + 1] = acc_a[i] + a[i];

	vector<T> c(n + 1);
	repi(k, 1, n) {
		repi(j, 0, min(m - 1, (n + k - 1) / k)) {
			int i1 = max(j * k - k + 1, 0);
			int i2 = min(j * k + 1, n);

			c[k] += b[j] * (acc_a[i2] - acc_a[i1]);
		}
	}

	return c;
}


