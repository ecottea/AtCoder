#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ 合同式 ■■■■■


//【位数（法が素数）】O(√p)
/*
* a^x ≡ 1 (mod p) となる最小の自然数 x を返す．（なければ -1）
*
* 利用：【約数列挙】
*/
int ord_p(const mint& a) {
	const int p = mint::mod();

	// p - 1 の約数が位数の候補となる．
	vl divs = divisors(p - 1);

	// p - 1 の約数を昇順に調べていく．
	repe(d, divs) {
		if (a.pow(d) == 1) {
			return (int)d;
		}
	}

	return -1;
}


//【位数】O(√m)
/*
* a^x ≡ 1 (mod m) となる最小の自然数 x を返す．（なければ -1）
*
* 利用：【カーマイケル関数】,【約数列挙】
*/
int ord(const mint& a) {
	// verify : https://atcoder.jp/contests/jag2015summer-day4/tasks/icpc2015summer_day4_d

	const int m = mint::mod();

	// λ(m) を求める．
	ll lambda = carmichael_lambda(m);

	// λ(m) の約数が位数の候補となる．
	vl divs = divisors(lambda);

	// λ(m) の約数を昇順に調べていく．
	repe(d, divs) {
		if (a.pow(d) == 1) {
			return (int)d;
		}
	}

	return -1;
}


//【原始根】O(√p)
/*
* 素数の法 p における最小の原始根を返す．
* 
* 利用：【約数列挙】
*/
int find_primitive_root() {
	// verify : https://atcoder.jp/contests/agc047/tasks/agc047_c

	const int p = mint::mod();
	if (p == 2) return 1;

	// p - 1 の約数 divs を得る．
	vl divs = divisors(p - 1);

	// p - 1 自身だけ削除する．
	divs.pop_back();

	repi(r, 2, p - 1) {
		// p - 1 の真の約数が全て r の位数でないなら原始根
		repe(d, divs) if (mint(r).pow(d) == 1) goto NEXT_LOOP;

		return r;

	NEXT_LOOP:;
	}

	return -1;
}


//【離散対数問題（法が素数）】O(√p)
/*
* a^x ≡ b (mod p) の最小解 x >= 0 を返す．（なければ INF）
*
* 制約 : p = mint::mod() は素数
*
*（baby-step giant-step）
*/
int log(mint a, mint b) {
	// 参考：https://tjkendev.github.io/procon-library/python/math/baby-step-giant-step.html

	//【方法】
	// m = ceil(√p)，r = a^(-m) とおく．
	// 
	// まず x∈[0..m) について a^x を計算した集合 S を得る．（計算量 O(m)）
	// S の中に b に一致するものがあればそれでよい．
	// なかった場合は x >= m であることが確定する．
	// 
	// 次に解くべき方程式
	//		a^x = b
	// の両辺に r = a^(-m) を掛けて
	//		a^(x-m) = b r
	// とする．
	// もし S の中に b r に一致するものがあれば，そこから x-m が分かり，
	// その結果に m を加えたものが求める x の値である．
	// なかった場合は x >= 2 m であることが確定する．
	//
	// この調子で S の中に b, b r, b r^2, ... があるかどうかを調べていく．
	// a^(mod - 1) = 1 なので，同様のステップは高々 m 回で終了する．
	// 各回の S へのアクセスが O(1) で行えるなら，全体計算量は O(m) である．

	int m = (int)(ceil(sqrt(mint::mod())) + 1);

	// a = 0 の場合の例外処理
	if (a == 0) {
		if (b == 0) return 1; // 0^0 = 1 とする．
		else return -1;
	}

	// loga[a^i] = i を計算しておく．
	unordered_map<int, int> loga;
	mint a_pow = a.pow(m), a_inv = a.inv();
	repir(i, m - 1, 0) {
		a_pow *= a_inv;
		loga[a_pow.val()] = i;
	}

	// r = a^(-m)
	mint r = a_inv.pow(m);

	// 方程式の両辺に r = a^(-m) を掛けながら解を探していく．
	rep(i, m) {
		if (loga.count(b.val())) {
			return m * i + loga[b.val()];
		}
		b *= r;
	}

	// 見つからなかったら INF を返す．
	return INF;
}


//【離散対数問題】O(√m log m)
/*
* a x^d ≡ b (mod m) の最小解 d >= 0 を返す（なければ INF）
* ここで m = mint::mod() である．また 0^0 = 1 とする．
*
*（baby-step giant-step）
*/
int log(mint a, mint x, mint b) {
	// verify : https://judge.yosupo.jp/problem/discrete_logarithm_mod

	//【方法】
	// まず i∈[0..√m) について素朴に a x^i を計算し，b に一致するかを見る：O(√m)
	// 同時に i∈[0..√m) について b x^i を計算した集合 S を得ておく．
	// もし a x^i = b なる i があれば d=i でよく，なければ d >= √m であることが確定する．
	// 
	// 次に a x^√m の値を計算し，b x^i がそれと等しくなる i∈[0..√m) を探す：O(1)
	// もし
	//		a x^√m = b x^i (mod m)
	// なる i が見つかれば，これは
	//		a x^(√m - i) = b (mod m)
	// であるための必要条件なので，d = √m - i が解の候補となる．
	// 解かどうかは実際に計算してみればわかる：O(log m)
	// 解が見つからなければ d >= 2√m であることが確定する．
	// 
	// この調子で a x^2√m, a x^3√m, ... の値を計算し，b x^i がそれと等しくなる i を探す．
	// x^(φ(m) + 1) = x なので，同様のステップは √m 回行えば十分である：O(√m)

	if (x == 0) {
		if (a == b) return 0;
		if (b == 0) return 1;
		return INF;
	}

	int sqrt_m = (int)(ceil(sqrt(mint::mod())) + 1);

	// logx[v] : v = b x^j となる √m 未満の j の昇順リスト
	//（解の候補にすぎないので，最大の j を保持するだけではいけない．）
	unordered_map<int, vi> logx;
	mint x_pow = 1;
	rep(j, sqrt_m) {
		if (a * x_pow == b) return j;

		logx[(b * x_pow).val()].emplace_back(j);

		x_pow *= x;
	}

	// a に x_pow = x^√m を掛けながら解の候補を探していく．
	mint ax = a;
	repi(i, 1, sqrt_m) {
		ax *= x_pow;
		if (logx.count(ax.val())) {
			repir(t, sz(logx[ax.val()]) - 1, 0) {
				// a x^(i √m) = b x^j なる (i, j) が見つかった．
				int j = logx[ax.val()][t];
					
				// 解の候補を得て，実際に計算してみて一致するかを見る．
				int d = i * sqrt_m - j;
				if (a * x.pow(d) == b) return d;
			}
		}
	}

	return INF;
}


//【平方剰余】O((log p)^2)
/*
* x^2 ≡ a (mod p) の解 x の 1 つを返す．（なければ -1）
*
* 制約 : p = mint::mod() は素数
*/
int tonelli_shanks(const mint& a) {
	// 参考：https://tjkendev.github.io/procon-library/python/math/tonelli-shanks.html
	// verify : https://judge.yosupo.jp/problem/sqrt_mod

	//【方法】
	// p = mod, p-1 = 2^d q（q : 奇数）と表しておく．
	// 
	// a = 0 のときは 0^2 = 0 なので単に 0 を返せば良い．
	// 
	// p = 2 のときは x^2 = x (mod 2) なので単に a を返せば良い．
	// 
	// a が平方非剰余の場合を検出するには，オイラーの規準より
	//		a が平方非剰余 ⇔ a^((p-1)/2) = -1
	// であることを用いてればよい．この場合は -1 を返す．
	// 
	// p = 3 (mod 4) の場合は，単に x = a^((p+1)/4) を返せば良い．実際，オイラーの規準より
	//		x^2 = a^((p+1)/2) = a * a^((p-1)/2) = a * 1 = a
	// となる．
	// 
	// 以降の手順のため，オイラーの規準を用いて適当な平方非剰余 z を見つけておく．
	//
	// t = a^q と初期化する．a は平方剰余なので，オイラーの規準より
	//		t^(2^(d-1)) = a^(2^(d-1) q) = a^((p-1)/2) = 1
	// となる．
	//
	// i∈[d-2..0] について，t^(2^i) = -1 であれば
	//		t *= z^(2^(d-i-1) q)
	// と t を更新する．この因子の 2^i 乗は
	//		(z^(2^(d-i-1) q))^(2^i) = z^(2^(d-1) q) = z^((p-1)/2) = -1
	// より -1 なので，この更新により t^(2^i) = 1 となる．
	// i = 0 まで更新を終えれば最終的に t = 1 となり，ここまでの手順から
	//		1 = a^q z^(2^(d-i[1]-1) q) ... z^(2^(d-i[k]-1) q)
	// の形の等式が得られる．
	//
	// 先の等式を用いれば，求める x は
	//		x = (1 a)^(1/2)
	//		= (a^(q+1) z^(2^(d-i[1]-1) q) ... z^(2^(d-i[k]-1) q))^(1/2)
	//		= a^((q+1)/2) z^(2^(d-i[1]-2) q) ... z^(2^(d-i[k]-2) q)
	// と表される．

	// 法 p を得る．
	int p = mint::mod();

	// a = 0 の場合の例外処理
	if (a == 0) return 0;

	// p = 2 の場合の例外処理
	if (p == 2) return a.val();

	// a が平方非剰余なら -1 を返す．
	if (a.pow((p - 1) / 2) == -1) return -1;

	// p = 3 (mod 4) の場合は簡単に解決する．
	if (p % 4 == 3) return a.pow((p + 1) / 4).val();

	// mod - 1 = 2^d q（q : 奇数）なる d, q を得る．
	int q = p - 1, d = 0;
	while (q % 2 == 0) {
		q /= 2;
		d++;
	}

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<ll> rnd(2, p - 1);

	// 適当な平方非剰余 z を見つける．
	mint z; vm z_pow(d); // z_pow[i] = z^(2^i q)
	while (true) {
		z = rnd(mt);

		z_pow[0] = z.pow(q);
		repi(i, 1, d - 1) z_pow[i] = z_pow[i - 1] * z_pow[i - 1];

		if (z_pow[d - 1] == -1) break;
	}

	// t を更新しつつ結果を得る．
	mint tmp = a.pow((q - 1) / 2), res = tmp * a, t = tmp * res;
	repir(i, d - 2, 0) {
		if (t.pow(1LL << i) == -1) {
			t *= z_pow[d - i - 1];
			res *= z_pow[d - i - 2];
		}
	}

	return res.val();
}


