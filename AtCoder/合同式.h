#pragma once
#include "header.h"
#include "数論.h"
#include "mint.h"
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
	// verify : https://yukicoder.me/problems/no/981

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


//【離散対数問題（M-集合）】O(√N)
/*
* f^n s = t を満たす N 未満の最小の非負整数 n を返す（存在しなければ INFL）
* f[s,t] は M-集合 (S, F, act, comp, id) の F[S,S] の元とする．
* HASH はハッシュ関数 size_t operator()(const S& p) の定義された関数オブジェクトとする．
*/
template <class S, class F, S(*act)(F, S), F(*comp)(F, F), F(*id)(), class HASH>
ll discrete_logarithm(const F& f, const S& s, const S& t, ll N) {
	// 参考 : https://maspypy.com/%e3%83%a2%e3%83%8e%e3%82%a4%e3%83%89%e4%bd%9c%e7%94%a8%e3%81%ab%e9%96%a2%e3%81%99%e3%82%8b%e9%9b%a2%e6%95%a3%e5%af%be%e6%95%b0%e5%95%8f%e9%a1%8c
	// verify : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k

	int m = (int)(sqrt(N) + 1e-12) + 1;

	// T : {f^i t | i∈[1..m]}
	unordered_set<S, HASH> T;
	S f_t(t);
	repi(i, 1, m) {
		// f_t : f^i t
		f_t = act(f, f_t);

		if (T.count(f_t)) break;
		T.insert(f_t);
	}

	// fm : f^m
	F fm(id()), pow2 = f; int m_tmp(m);
	while (m_tmp > 0) {
		if ((m_tmp & 1) != 0) fm = comp(fm, pow2);
		pow2 = comp(pow2, pow2);
		m_tmp /= 2;
	}

	S fm_s_bak(s); int fail_cnt = 0;
	repi(k, 1, m) {
		// fm_s : f^(m k) s, fm_s_bak : f^(m (k-1)) s
		S fm_s = act(fm, fm_s_bak);

		// f^(m k) s ∈ T となったなら，∃i∈[0..m), f^(m (k-1) + i) s = t となることが期待される．
		if (T.count(fm_s)) {
			S f_s(fm_s_bak);

			// f^(m (k-1) + i) s = t となっているかを全て調べる．
			rep(i, m) {
				// f_s : f^(m (k-1) + i) s
				if (f_s == t) return (ll)m * (k - 1) + i;

				f_s = act(f, f_s);
			}

			// t → f t なる有向辺をもった functional graph S を考える．
			// 先の手続きに失敗したとしても，いま初めて t を含むループに s から合流してきた可能性が残されている．
			// だがもしもう一度失敗したならば，t がループに含まれていないことを意味するので非存在が確定する．
			if (++fail_cnt == 2) return INFL;
		}

		fm_s_bak = fm_s;
	}

	return INFL;
}


//【オイラーの規準】
/*
* p が奇素数，a が p と互いに素な整数であるとき，以下の合同式が成り立つ：
*		a^((p-1)/2) ≡ 1  (mod p)  (a が p を法とする平方剰余のとき)
*			        ≡ -1 (mod p)  (a が p を法とする平方非剰余のとき)
* 
* 一般には，p が素数，a が p と互いに素な整数，n が非負整数であるとき，
*		g = GCD(n, p-1)
* とおけば以下が成り立つ：
*		a^((p-1)/g) ≡ 1 (mod p) ⇔ a が p を法とする n 乗剰余
*/


//【平方剰余】O(log p)
/*
* x^2 ≡ a (mod p) の解 x の 1 つを返す．（なければ -1）
*
* 制約：p = mint::mod() は素数
*
* 利用：【二次拡大体】
*/
int cipolla(const mint& a) {
	// 参考 : https://37zigen.com/cipolla-algorithm/
	// verify : https://judge.yosupo.jp/problem/sqrt_mod

	//【方法】
	// a ≡ 0 なら x ≡ 0 でよいから a ≠ 0 と仮定する．
	// p = 2 なら a^2 ≡ a (mod p) より x = a でよいから p は奇素数と仮定する．
	// 
	// オイラーの規準より
	//		a^((p-1)/2) ≡ 1 (mod p) ⇔ a が p を法とする平方剰余
	// である．解が存在しない場合はこれで判定できるので，以下解が存在すると仮定する．
	//
	// p = 3 (mod 4) の場合は，単に x = a^((p+1)/4) を返せば良い．実際，オイラーの規準より
	//		x^2 = a^((p+1)/2) = a * a^((p-1)/2) = a * 1 = a
	// となる．
	//
	// モニックな 2 次多項式 f(b; x) ∈ F_p[x] を
	//		f(b; x) = (x-b)^2 - b^2 + a
	// と定める．f(b; x) の根は
	//		x = b ± √(b^2 - a)
	// と表される．よって α = b^2 - a が平方非剰余であれば f(b; x) は F_p に根をもたず既約となる．
	// そのような b は十分多く存在するので，乱択とオイラーの規準による判定で素早く得ることができる．
	//
	// f(b; x) の 1 つの根 θ !∈ F_p を固定すると，
	// F_p(θ) ~= F_(p^2) におけるフロベニウス写像の性質より f(b; x) の全ての根は
	//		θ, θ^p
	// と表される．f(b; x) についての根と係数の関係より，定数項について
	//		θ θ^p ≡ [x^0] f(b; x) (mod p)
	//		⇔ θ^(1+p) ≡ a (mod p)
	// が成り立つ．p は奇素数より 1+p は偶数なので，
	//		θ^((1+p)/2) ∈ F_p
	// が求める a の平方根である．
	//
	// F_p(θ) = F_p(√(b^2 - a)) なので，この上で θ^((1+p)/2) を計算すればいい．

	// a ≡ 0 (mod p) の場合の例外処理 : O(1)
	if (a == 0) return 0;

	auto p = mint::mod();

	// p = 2 の場合の例外処理 : O(1)
	if (p == 2) return a.val();

	// a が平方非剰余なら -1 を返す． : O(log p)
	if (a.pow((p - 1) / 2) == -1) return -1;

	// p = 3 (mod 4) の場合は簡単に解決する． : O(log p)
	if (p % 4 == 3) return a.pow((p + 1) / 4).val();

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<ll> rnd(2, p - 1);

	// b^2 - a が平方非剰余となる適当な b を見つける． : 平均 O(log p)
	mint b;
	while (true) {
		b = rnd(mt);
		if ((b * b - a).pow((p - 1) / 2) == -1) break;
	}

	// 二次拡大体 F_p(√b^2-a) を作る．
	QF::set_base(b * b - a);

	// θ = b + √(b^2 - a) とする．
	QF th(b, 1);

	// θ^((1+p)/2) ∈ F_p を返す． : O(log p)
	return th.pow((1 + p) / 2).a.val();
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
	// であることを用いればよい．この場合は -1 を返す．
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


