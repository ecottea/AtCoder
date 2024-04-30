#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 2 変数 FPS ■■■■■


//【二次元畳込み（mod 998244353）】O((ha + hb) (wa + wb) (log(ha + hb) + log(wa + wb)))
/*
* a[0..ha)[0..wa) と b[0..hb)[0..wb) の二次元畳込みを返す．
*/
vvm convolution_2D(vvm a, vvm b) {
	// verify : https://atcoder.jp/contests/abc345/tasks/abc345_g

	int ha = sz(a), wa = sz(a[0]);
	int hb = sz(b), wb = sz(b[0]);

	// 縦方向，横方向ともに素朴に畳み込む．
	if ((ll)ha * wa * hb * wb <= 100000LL) {
		vvm c(ha + hb - 1, vm(wa + wb - 1));
		rep(ia, ha) rep(ib, hb) rep(ja, wa) rep(jb, wb) {
			c[ia + ib][ja + jb] += a[ia][ja] * b[ib][jb];
		}

		return c;
	}

	// 列方向には素朴に畳込み，行方向には NTT で畳み込む．
	if ((ll)ha * hb <= 800LL) {
		// 幅を 2 冪に拡張しておく．
		int W = 1 << (msb(wa + wb - 2) + 1);
		rep(i, ha) a[i].resize(W);
		rep(i, hb) b[i].resize(W);

		// 行方向の NTT
		rep(i, ha) internal::butterfly(a[i]);
		rep(i, hb) internal::butterfly(b[i]);

		vvm c(ha + hb - 1, vm(wa + wb - 1)); vm tmp(W);
		rep(ia, ha) rep(ib, hb) {
			// 各点積
			rep(j, W) tmp[j] = a[ia][j] * b[ib][j];

			// 行方向の INTT
			internal::butterfly_inv(tmp);

			rep(j, wa + wb - 1) c[ia + ib][j] += tmp[j];
		}

		// 定数倍の調整
		mint inv = mint(W).inv();
		rep(i, ha + hb - 1) rep(j, wa + wb - 1) c[i][j] *= inv;

		return c;
	}

	// 行方向には素朴に畳込み，列方向には NTT で畳み込む．
	if ((ll)wa * wb <= 800LL) {
		// 高さを 2 冪に拡張しつつ転置する．
		int H = 1 << (msb(ha + hb - 2) + 1);
		vvm aT(wa, vm(H)), bT(wb, vm(H));
		rep(i, ha) rep(j, wa) aT[j][i] = a[i][j];
		rep(i, hb) rep(j, wb) bT[j][i] = b[i][j];

		// 列方向の NTT
		rep(j, wa) internal::butterfly(aT[j]);
		rep(j, wb) internal::butterfly(bT[j]);

		vvm c(ha + hb - 1, vm(wa + wb - 1)); vm tmp(H);
		rep(ja, wa) rep(jb, wb) {
			// 各点積
			rep(i, H) tmp[i] = aT[ja][i] * bT[jb][i];

			// 列方向の INTT
			internal::butterfly_inv(tmp);

			rep(i, ha + hb - 1) c[i][ja + jb] += tmp[i];
		}

		// 定数倍の調整
		mint inv = mint(H).inv();
		rep(i, ha + hb - 1) rep(j, wa + wb - 1) c[i][j] *= inv;

		return c;
	}

	// 両方向とも NTT で畳み込む．

	// 高さと幅を 2 冪に拡張しておく．
	int H = 1 << (msb(ha + hb - 2) + 1);
	int W = 1 << (msb(wa + wb - 2) + 1);
	a.resize(H); b.resize(H);
	rep(i, H) { a[i].resize(W); b[i].resize(W); }

	// 行方向の NTT
	rep(i, H) { internal::butterfly(a[i]); internal::butterfly(b[i]); }

	// 転置
	vvm aT(W, vm(H)), bT(W, vm(H));
	rep(i, H) rep(j, W) { aT[j][i] = a[i][j]; bT[j][i] = b[i][j]; }

	// 列方向の NTT
	rep(j, W) { internal::butterfly(aT[j]); internal::butterfly(bT[j]); }

	// 各点積
	rep(j, W) rep(i, H) aT[j][i] *= bT[j][i];

	// 列方向の INTT
	rep(j, W) internal::butterfly_inv(aT[j]);

	// 転置
	rep(i, H) rep(j, W) a[i][j] = aT[j][i];

	// 行方向の INTT
	rep(i, H) internal::butterfly_inv(a[i]);

	// 不要な部分の削除
	a.resize(ha + hb - 1);
	rep(i, ha + hb - 1) a[i].resize(wa + wb - 1);

	// 定数倍の調整
	mint inv = mint(H * W).inv();
	rep(i, ha + hb - 1) rep(j, wa + wb - 1) a[i][j] *= inv;

	return a;
}


//【二変数展開係数】O(n m^2 (log n + log m) log N) ?
/*
*	f(z,w) = Σi∈[0..n) Σj∈[0..m) f[i][j] z^i w^j
*	g(z,w) = Σi∈[0..n) Σj∈[0..m) g[i][j] z^i w^j
* と定め，[z^N] [w^[0..M]] f(z,w) / g(z,w) を返す．
*
* 制約 : [z^0]g(z,w) = 1
*
* 利用：【二次元畳込み（mod 998244353）】
*/
vm bostan_mori(vvm f, vvm g, int N, int M) {
	// 参考 : https://noshi91.hatenablog.com/entry/2024/03/16/224034
	// verify : https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large

	//【方法】
	// 1 変数のときのボスタン-森法と全く同じ．

	// f(z,w) = 0 のときは 0 を返す．
	if (sz(f) == 0) return vm(M + 1);

	while (N > 0) {
		// f2(z,w) = f(z,w) g(-z,w), g2(z,w) = g(z,w) g(-z,w) を求める．
		vvm f2, g2 = g;
		rep(i, sz(g2)) if (i & 1) rep(j, sz(g2[i])) g2[i][j] *= -1;
		f2 = convolution_2D(f, g2);
		g2 = convolution_2D(g, g2);

		// f3(z,w) = E(z,w) or O(z,w), g3(z,w) = e(z,w) を求める．
		f.clear(); g.clear();
		if (N & 1) rep(i, min<ll>(sz(f2) / 2, N / 2 + 1)) f.push_back(f2[2 * i + 1]);
		else rep(i, min<ll>((sz(f2) + 1) / 2, N / 2 + 1)) f.push_back(f2[2 * i]);
		rep(i, min<ll>((sz(g2) + 1) / 2, N / 2 + 1)) g.push_back(g2[2 * i]);

		rep(i, sz(f)) if (sz(f[i]) > M + 1) f[i].resize(M + 1);
		rep(i, sz(g)) if (sz(g[i]) > M + 1) g[i].resize(M + 1);

		// N を半分にして次のステップに進む．
		N /= 2;
	}

	// N = 0 になったら [z^0]g(z,w) = 1 なので [z^0]f(z,w) を返せば良い．
	f[0].resize(M + 1);
	return f[0];
}


//【累乗の係数列挙】O(N (log N)^2)
/*
*	a(z) = Σi∈[0..n] a[i] z^i
*	b(z) = Σi∈[1..n] b[i] z^i
* と定め，各 i∈[0..N] についての [z^N] a(z) b(z)^i のリストを返す．
* 
* 利用：【二変数展開係数】
*/
vm coefficients_of_power(const vm& a, const vm& b, int N) {
	// 参考 : https://noshi91.hatenablog.com/entry/2024/03/16/224034
	// verify : https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large

	//【方法】
	// 求めたいものは
	//		[w^[0..N]] Σj∈[0..∞) ([z^N] a(z) b(z)^j) w^j
	//		= [z^N] [w^[0..N]] a(z) Σj∈[0..∞) (b(z) w)^j
	//		= [z^N] [w^[0..N]] a(z) / (1 - b(z)w)
	// と書き直せるので，2 変数ボスタン-森法で求められる．

	int na = sz(a), nb = sz(b);
	if (na == 0 || nb == 0) return vm(0, N + 1);

	Assert(b[0] == 0);

	vvm f(na, vm(1));
	rep(i, na) f[i][0] = a[i];

	vvm g(nb, vm(2));
	g[0][0] = 1;
	repi(i, 1, nb - 1) g[i][1] = -b[i];

	return bostan_mori(f, g, N, N);
}


//【関数の合成】O(N (log N)^2)
/*
* FPS f(z), g(z) を
*	f(z) = Σi∈[0..n) f[i] z^i
*	g(z) = Σj∈[1..m) g[j] z^j
* と定め，[z^[0..N)] f(g(z)) を返す．
*
* 利用：【二次元畳込み（mod 998244353）】
*/
vm composition(const vm& f, const vm& g, int N) {
	// 参考 : https://qiita.com/ryuhe1/items/23d79bb84b270f7359e0
	// verify : https://judge.yosupo.jp/problem/composition_of_formal_power_series_large

	if (N == 0) return vm();
	if (N == 1) return vm{ f[0] };
	if (sz(g) == 0) {
		vm res(N);
		res[0] = f[0];
		return res;
	}

	// 2^K : N 以上の最小の 2 冪
	int K = msb(N - 1) + 1;

	vvvm q(K);

	q[0] = vvm(sz(g), vm(2));
	q[0][0][0] = 1;
	repi(i, 1, sz(g) - 1) q[0][i][1] = -g[i];

	repi(k, 1, K - 1) {
		auto q_pos(q[k - 1]);
		int sz_q = sz(q[k - 1]);
		for (int i = 1; i < sz_q; i += 2) rep(j, sz(q[k - 1][i])) q[k - 1][i][j] *= -1;
		auto qk_dbl = convolution_2D(q_pos, q[k - 1]);
		rep(i, min((sz(qk_dbl) + 1) / 2, (1 << (K - k)))) q[k].emplace_back(move(qk_dbl[2 * i]));
		if (sz(q[k]) > N) q[k].resize(N);
	}

	int sz_q = sz(q[K - 1]);
	for (int i = 1; i < sz_q; i += 2) rep(j, sz(q[K - 1][i])) q[K - 1][i][j] *= -1;
	if (sz(q[K - 1]) > N) q[K - 1].resize(N);

	vvm p(1, vm(N));
	rep(i, min(sz(f), N)) p[0][N - 1 - i] = f[i];

	auto tmp = convolution_2D(p, q[K - 1]);

	int sz_p = min(2, sz(tmp));
	p.resize(sz_p);
	rep(i, sz_p) {
		int j_min = N - (1 << (K - 1));
		int j_max = min(N, sz(tmp[i])) - 1;
		p[i].resize(j_max - j_min + 1);
		repi(j, j_min, j_max) p[i][j - j_min] = tmp[i][j];
	}

	repir(k, K - 2, 0) {
		vvm p_dbl(sz(p) * 2 - 1, vm(sz(p[0])));
		rep(i, sz(p)) rep(j, sz(p[i])) p_dbl[i * 2][j] = p[i][j];

		auto tmp = convolution_2D(p_dbl, q[k]);

		int sz_p = min({ 1 << (K - k), N, sz(tmp) });
		p.resize(sz_p);
		rep(i, sz_p) {
			int j_min = 1 << k;
			int j_max = min(1 << (k + 1), sz(tmp[i])) - 1;
			p[i].resize(j_max - j_min + 1);
			repi(j, j_min, j_max) p[i][j - j_min] = tmp[i][j];
		}
	}

	vm res(N);
	rep(i, min(N, sz(p))) res[i] = p[i][0];

	return res;
}


//【関数の合成（多項式）】O(N (log N)^2)
/*
* 多項式 f(z), FPS g(z) を
*	f(z) = Σi∈[0..n) f[i] z^i
*	g(z) = Σj∈[0..m) g[j] z^j
* と定め，[z^[0..N)] f(g(z)) を返す．
*
* 制約 : deg(f) < N, fm は (3N)! まで計算可能
*
* 利用：【二次元畳込み（mod 998244353）】
*/
vm composition(const vm& f, const vm& g, int N, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/ryuhe1/items/23d79bb84b270f7359e0
	// verify : https://judge.yosupo.jp/problem/composition_of_formal_power_series_large

	if (N == 0) return vm();

	// f(0) = f[0]
	if (sz(g) == 0) {
		vm res(N);
		res[0] = f[0];
		return res;
	}

	// [z^0]f(g(z)) = f(g[0])
	if (N == 1) {
		mint val = 0;
		repir(i, sz(f) - 1, 0) val = val * g[0] + f[i];
		return vm{ val };
	}

	// 2^K : N 以上の最小の 2 冪
	int K = msb(N - 1) + 1;
	dump(K);

	vvvm q(K);

	q[0] = vvm(sz(g), vm(2));
	q[0][0][0] = 1;
	repi(i, 0, sz(g) - 1) q[0][i][1] = -g[i];

	repi(k, 1, K - 1) {
		auto q_pos(q[k - 1]);
		int sz_q = sz(q[k - 1]);
		for (int i = 1; i < sz_q; i += 2) rep(j, sz(q[k - 1][i])) q[k - 1][i][j] *= -1;
		auto qk_dbl = convolution_2D(q_pos, q[k - 1]);
		rep(i, min((sz(qk_dbl) + 1) / 2, (1 << (K - k)))) q[k].emplace_back(move(qk_dbl[2 * i]));
		if (sz(q[k]) > N) q[k].resize(N);
	}

	int sz_q = sz(q[K - 1]);
	for (int i = 1; i < sz_q; i += 2) rep(j, sz(q[K - 1][i])) q[K - 1][i][j] *= -1;
	if (sz(q[K - 1]) > N) q[K - 1].resize(N);

	vvm p(1, vm(N));
	rep(i, min(sz(f), N)) p[0][N - 1 - i] = f[i];

	// dnm : 1/(1-g[0]w)^(2^K) を負の二項定理で計算する．
	vm dnm(N); mint g0_pow = 1;
	rep(i, N) {
		dnm[i] = fm.bin((1 << K) - 1 + i, i) * g0_pow;
		g0_pow *= g[0];
	}

	p[0] = convolution(p[0], dnm);
	p[0].resize(N);

	auto tmp = convolution_2D(p, q[K - 1]);

	int sz_p = min(2, sz(tmp));
	p.resize(sz_p);
	rep(i, sz_p) {
		int j_min = N - (1 << (K - 1));
		int j_max = min(N, sz(tmp[i])) - 1;
		p[i].resize(j_max - j_min + 1);
		repi(j, j_min, j_max) p[i][j - j_min] = tmp[i][j];
	}

	repir(k, K - 2, 0) {
		vvm p_dbl(sz(p) * 2 - 1, vm(sz(p[0])));
		rep(i, sz(p)) rep(j, sz(p[i])) p_dbl[i * 2][j] = p[i][j];

		auto tmp = convolution_2D(p_dbl, q[k]);

		int sz_p = min({ 1 << (K - k), N, sz(tmp) });
		p.resize(sz_p);
		rep(i, sz_p) {
			int j_min = 1 << k;
			int j_max = min(1 << (k + 1), sz(tmp[i])) - 1;
			p[i].resize(j_max - j_min + 1);
			repi(j, j_min, j_max) p[i][j - j_min] = tmp[i][j];
		}
	}

	vm res(N);
	rep(i, min(N, sz(p))) res[i] = p[i][0];

	return res;
}


//【逆関数】O(N (log N)^2)
/*
* f^(-1)(z) mod z^N を返す．
*
* 制約 : [z^1] f(z) != 0，fm は (N-1)! まで計算可能
*
* 利用：【累乗の係数列挙】,【累乗（有理数）】
*/
MFPS compositional_inverse(MFPS f, int N, const Factorial_mint& fm) {
	// 参考 : https://codeforces.com/blog/entry/77551
	// verify : https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large

	//【方法】
	// n=N-1 とおく．f(z) の逆関数を g(z) とおくと，【ラグランジュの反転公式（累乗）】より
	//		[z^n]f(z)^i = i/n [z^(n-i)](z / g(z))^n
	// が成り立つ．左辺を a(i) とおきこれを g(z) について解くと，
	//		[z^(n-i)](z / g(z))^n = n/i a(i)
	//		[z^i](z / g(z))^n = n/(n-i) a(n-i)
	//		(z / g(z))^n = Σi∈[0..n) n/(n-i) a(n-i) z^i
	//		g(z) = z (Σi∈[0..n) n/(n-i) a(n-i) z^i)^(-1/n)
	// となる．
	//
	// -1/n 乗するためには a(n)=1 である必要があり，そのためには [z^1]f(z)=1 でなければならない．
	// そこで c = [z^1]f(z) とおいて F(z)=f(z/c) と定めれば [z^1]F(z)=1 を満たし，
	// F(z) の逆関数は G(z) = c g(z) となるのでこれを c で割れば良い．

	Assert(sz(f) >= 2 && f[0] == 0 && f[1] != 0);

	mint f1_inv = f[1].inv(), f1_inv_pow = f1_inv;
	repi(i, 1, sz(f) - 1) {
		f[i] *= f1_inv_pow;
		f1_inv_pow *= f1_inv;
	}

	auto a = coefficients_of_power(vm{ 1 }, f.c, N - 1);
	
	MFPS g(0, N - 1);
	rep(i, N - 1) g[i] = (N - 1) * fm.inv(N - 1 - i) * a[N - 1 - i];
	
	g = rational_pow_fps(g, -1, N - 1, N - 1, fm);
	g *= f1_inv;
	g >>= 1;
	
	return g;
}


//【多次元畳込み（切り詰め，mod 998244353）】O(D n log n)
/*
* n = Πns[0..D) とおき，a[0..n), b[0..n) を添字が ns 進表記（ns[0] が最下位）で与えられたものと解釈し，
* a, b の多次元畳込み（切り詰め）を行った結果を返す．
*/
vm multivariate_convolution(const vm& a, const vm& b, const vi& ns) {
	// 参考 : https://nyaannyaan.github.io/library/ntt/multivariate-multiplication.hpp
	// verify : https://judge.yosupo.jp/problem/multivariate_convolution

	//【方法】
	// 添字 x に対して rank(x) を
	//		rank(x) = Σi∈[d..D) floor(x / Πns[0..d))
	// と定めると，ns 進表記（ただし ns[D-1]=∞ とする）したときの x+y の繰り上がり回数は
	//		rank(x+y) - (rank(x) + rank(y)) ∈ [0..D)
	// と表される．
	//
	// よって列方向に rank mod D（狭変域），行方向に添字（広変域）をもった二次元畳込みを行い，
	// rank(x+y) - (rank(x) + rank(y)) ≡ 0 (mod d) となる項だけを拾い集めれば良い．

	//【備考】
	// ns が全て 2 なら【非交和畳込み】であり，そちらを利用する方が速い．

	// n : 要素数, D : 次元
	int n = sz(a), D = sz(ns);

	// acc[i] = Πns[0..i)
	vi acc(D + 1);
	acc[0] = 1;
	rep(d, D) acc[d + 1] = acc[d] * ns[d];
	Assert(n == acc[D]);

	if (D == 0) return vm{ a[0] * b[0] };

	// rank[i] = Σd∈[1..D) floor(i / Πns[0..d))
	vi rank(n);
	rep(i, n) {
		repi(d, 1, D - 1) rank[i] += i / acc[d];
		rank[i] %= D;
	}

	// 列の長さを 2 冪に拡張しつつ，rank(i) mod D で要素を振り分ける．
	int N = 1 << (msb(acc[D] - 1) + 2);
	vvm as(D, vm(N)), bs(D, vm(N));
	rep(i, n) {
		as[rank[i]][i] = a[i];
		bs[rank[i]][i] = b[i];
	}

	// 行方向の NTT
	rep(d, D) {
		internal::butterfly(as[d]);
		internal::butterfly(bs[d]);
	}

	vm c(n); vm tmp(N);

	// 列方向には素朴に畳み込む．
	rep(da, D) rep(db, D) {
		// 各点積
		rep(i, N) tmp[i] = as[da][i] * bs[db][i];

		// 行方向の INTT
		internal::butterfly_inv(tmp);

		// i ≧ n は最上位で繰り上がりが生じていることに対応するので無視する．
		rep(i, n) if ((rank[i] - da - db) % D == 0) c[i] += tmp[i];
	}

	// 定数倍の調整
	mint inv = mint(N).inv();
	rep(i, n) c[i] *= inv;

	return c;
}


