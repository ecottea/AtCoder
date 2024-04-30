#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 集合冪級数 ■■■■■


//【SPS 積】O(2^N N^2)
/*
* 与えられた [0..N) 上の集合冪級数 f(Z), g(Z) に対して，h(Z) = f(Z) g(Z)，すなわち
*       h[S] = Σ_(T凵U = S) f[T] g[U] （凵 は非交和）
* なる h(Z) を返す．
*
* 利用：【QCFium 法】
*/
template <class T>
vector<T> subset_convolution(const vector<T>& f, const vector<T>& g) {
	// 参考 : https://37zigen.com/subset-convolution/
	// verify : https://judge.yosupo.jp/problem/subset_convolution

	//【方法】
	// f[set] を多項式 f[set] z^|set| に対応させ，多項式として和集合畳込みを行うと，
	//		h[S](z) = Σ_(T∪U = S) f[T](z) g[U](z)
	// なる h が得られる．T∪U = S が非交和でないとき，
	//		[z^|S|] f[T](z) g[U](z)
	//		= [z^|S|] f[T] z^|T| g[U] z^|U|
	//		= [z^|S|] f[T] g[U] z^(|T| + |U|)
	//		= 0 （|T| + |U| > |S| より）
	// となるので，
	//		[z^|S|] h[S](z)
	//		= [z^|S|] Σ_(T凵U = S) f[T](z) g[U](z)
	//		= Σ_(T凵U = S) f[T] g[U]
	// が成り立つ．

	int N = msb(sz(f));

	// f, g にランク（集合の要素数）の情報を付加する．
	vector<vector<T>> f2(1LL << N, vector<T>(N + 1)), g2(1LL << N, vector<T>(N + 1));
	repb(set, N) {
		int r = popcount(set);
		f2[set][r] = f[set];
		g2[set][r] = g[set];
	}

	// f のランク付き下位ゼータ変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) f2[set + (1 << i)][r] += f2[set][r];
	}

	// g のランク付き下位ゼータ変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) g2[set + (1 << i)][r] += g2[set][r];
	}

	// 各点積 h = f * g（ただしランクが N より大きい項は無視する）
	vector<vector<T>> h2(1LL << N, vector<T>(N + 1));
	repb(set, N) {
		// f2[set] と g2[set] の畳込み積
		repi(r, 0, N) repi(rf, 0, r) h2[set][r] += f2[set][rf] * g2[set][r - rf];
	}

	// h のランク付き下位メビウス変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) h2[set + (1 << i)][r] -= h2[set][r];
	}

	// 非交和でなかったところは集合の要素数よりランクが大きくなっているので，
	// 集合の要素数とランクが一致している項だけを抜き出す．
	vector<T> h(1LL << N);
	repb(set, N) h[set] = h2[set][popcount(set)];

	return h;
}


//【SPS 指数関数】O(2^N N^2)
/*
* 与えられた [0..N) 上の集合冪級数 f(Z) に対して exp(f(Z)) を返す．
*
* 制約 : f[0] = 0
* 
* 利用：【QCFium 法】
*/
vm exp_sps(const vm& f) {
	// 参考 : https://37zigen.com/subset-convolution/
	// verify : https://judge.yosupo.jp/problem/exp_of_set_power_series

	//【方法】
	// SPS 積とほぼ同じ．exp はスパース FPS 用の方法を流用する．

	int N = msb(sz(f));

	vm inv(N + 1);
	repi(i, 1, N) inv[i] = mint(i).inv();

	// f にランク（集合の要素数）の情報を付加する．
	vvm f2(1LL << N, vm(N + 1));
	repb(set, N) f2[set][popcount(set)] = f[set];

	// f のランク付き下位ゼータ変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) f2[set + (1 << i)][r] += f2[set][r];
	}

	// 各点 exp（ただしランクが N より大きい項は無視する）
	vvm g2(1LL << N, vm(N + 1));
	repb(set, N) {
		// g[0] = 0
		g2[set][0] = 1;

		// r g[r] = Σk∈[1..r] k f[k] g[r-k]
		repi(r, 1, N) {
			repi(k, 1, r) g2[set][r] += k * f2[set][k] * g2[set][r - k];
			g2[set][r] *= inv[r];
		}
	}

	// g のランク付き下位メビウス変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) g2[set + (1 << i)][r] -= g2[set][r];
	}

	// 非交和でなかったところは集合の要素数よりランクが大きくなっているので，
	// 集合の要素数とランクが一致している項だけを抜き出す．
	vm g(1LL << N);
	repb(set, N) g[set] = g2[set][popcount(set)];

	return g;
}


//【SPS 対数関数】O(2^N N^2)
/*
* 与えられた [0..N) 上の集合冪級数 f(Z) に対して log(f(Z)) を返す．
*
* 制約 : f[0] = 1
* 
* 利用：【QCFium 法】
*/
vm log_sps(const vm& f) {
	// 参考 : https://37zigen.com/subset-convolution/
	// verify : https://atcoder.jp/contests/abc321/tasks/abc321_g

	//【方法】
	// 積とほぼ同じ．log はスパース FPS 用の方法を流用する．

	int N = msb(sz(f));

	vm inv(N + 1);
	repi(i, 1, N) inv[i] = mint(i).inv();

	// f にランク（集合の要素数）の情報を付加する．
	vvm f2(1LL << N, vm(N + 1));
	repb(set, N) f2[set][popcount(set)] = f[set];

	// f のランク付き下位ゼータ変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) f2[set + (1 << i)][r] += f2[set][r];
	}

	// 各点 log（ただしランクが N より大きい項は無視する）
	vvm g2(1LL << N, vm(N + 1));
	repb(set, N) {
		// g[0] = 0
		g2[set][0] = 0;

		// r g[r] = r f[r] - Σk∈[1..r-1] (r-k) g[r-k] f[k]
		repi(r, 1, N) {
			repi(k, 1, r - 1) g2[set][r] -= (r - k) * g2[set][r - k] * f2[set][k];
			g2[set][r] *= inv[r];
			g2[set][r] += f2[set][r];
		}
	}

	// g のランク付き下位メビウス変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) g2[set + (1 << i)][r] -= g2[set][r];
	}

	// 非交和でなかったところは集合の要素数よりランクが大きくなっているので，
	// 集合の要素数とランクが一致している項だけを抜き出す．
	vm g(1LL << N);
	repb(set, N) g[set] = g2[set][popcount(set)];

	return g;
}


//【SPS 合成】O(2^N N^2)
/*
* 与えられた多項式 f(z) と [0..N) 上の集合冪級数 s(Z) に対して，f(s(Z)) を返す．
*
* 利用：【SPS 積】
*/
template <class T>
vector<T> composition_sps(vector<T> f, const vector<T>& s) {
	// 参考 : https://maspypy.com/%e9%9b%86%e5%90%88%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0%e9%96%a2%e9%80%a3-2-%e5%a4%9a%e9%a0%85%e5%bc%8f%e3%81%a8%e3%81%ae%e5%90%88%e6%88%90
	// verify : https://judge.yosupo.jp/problem/polynomial_composite_set_power_series

	int m = sz(f), N = msb(sz(s));

	if (m == 0) return vector<T>(1LL << N, 0);

	// s0_pow[j] : s[0]^j
	vector<T> s0_pow(m);
	s0_pow[0] = 1;
	rep(j, m - 1) s0_pow[j + 1] = s0_pow[j] * s[0];

	// g(z) : 通常型母関数 f(z + s[0]) に対応する指数型母関数（以降 s[0] = 0 とみなす）
	vector<T> g(N + 1);
	rep(j, m) g[0] += f[j] * s0_pow[j];
	repi(i, 1, N) rep(j, m - i) {
		// f ← f'
		f[j] = f[j + 1] * (j + 1);

		// [z^i]g(z) = (d/dz)^i f(z) | z=s[0]（テイラー展開）
		g[i] += f[j] * s0_pow[j];
	}

	// gs[k] : (d/dz)^k g(z) | z=s(Z^[0..N-k))
	vector<vector<T>> gs(N + 1);
	gs[N] = vector<T>{ g[N] };

	repir(k, N - 1, 0) {
		gs[k].resize(1LL << (N - k));
		gs[k][0] = g[k];

		rep(k2, N - k) {
			// gs[k] は gs[k+1] と s それぞれの部分 SPS 積で計算できる．
			int W = 1 << k2;
			vector<T> g_sub(gs[k + 1].begin(), gs[k + 1].begin() + W);
			vector<T> s_sub(s.begin() + W, s.begin() + 2 * W);
			auto tmp = subset_convolution(g_sub, s_sub);
			rep(i, W) gs[k][W + i] = tmp[i];
		}
	}

	return gs[0];
}


//【SPS 累乗の係数列挙】O(2^N N^2)
/*
* 与えられた Ω=[0..N) 上の集合冪級数 c(Z), s(Z) に対して，
* 各 j∈[0..M) についての [Z^Ω] c(Z) s(Z)^j のリストを返す．
*
* 利用：【SPS 積】
*/
template <class T>
vector<T> coefficients_of_power_sps(const vector<T>& c, const vector<T>& s, int M) {
	// 参考 : https://maspypy.com/%e9%9b%86%e5%90%88%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0%e9%96%a2%e9%80%a3-3-%e5%a4%9a%e9%a0%85%e5%bc%8f%e3%81%a8%e3%81%ae%e5%90%88%e6%88%90%e3%81%ae%e8%bb%a2%e7%bd%ae
	// verify : https://judge.yosupo.jp/problem/power_projection_of_set_power_series

	//【方法】
	//【SPS 合成】の転置は
	//		Σ_set⊂Ω ([Z^set] h(Z)) ([Z^set] s(Z)^j))
	// を各 j∈[0..M) について列挙する問題になるので，h = c^R とすればよい．

	const int N = msb(sz(s));

	if (M == 0) return vector<T>();
	if (N == -1) return vector<T>(M, 0);

	// s0_pow[j] : s[0]^j
	vector<T> s0_pow(M);
	s0_pow[0] = 1;
	rep(j, M - 1) s0_pow[j + 1] = s0_pow[j] * s[0];

	vector<T> gs(1LL << N); vector<T> g(N + 1);
	rep(i, 1 << N) gs[i] = c[(1 << N) - 1 - i];

	repi(k, 0, N - 1) {
		vector<T> ngs(1LL << (N - k - 1));

		// ngs は gs と s それぞれの部分 SPS 積の転置（rev → SPS積 → rev）で計算できる．
		rep(k2, N - k) {
			const int W = 1 << k2;
			const int L = sz(gs);
			vector<T> g_sub(gs.rbegin() + (L - 2 * W), gs.rbegin() + (L - W));

			vector<T> s_sub(s.begin() + W, s.begin() + 2 * W);
			auto tmp = subset_convolution(g_sub, s_sub);

			rep(i, W) ngs[i] += tmp[W - 1 - i];
		}

		g[k] = gs[0];
		gs = move(ngs);
	}

	g[N] = gs[0];

	vector<T> f(M);
	repir(i, N, 1) {
		repir(j, M - i - 1, 0) {
			// テイラー展開の転置
			f[j] += g[i] * s0_pow[j];

			// 微分の転置
			f[j + 1] = f[j] * (j + 1);
		}
		f[0] = 0;
	}
	repir(j, M - 1, 0) f[j] += g[0] * s0_pow[j];

	return f;
}

