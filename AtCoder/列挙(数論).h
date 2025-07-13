#pragma once
#include "header.h"
#include "合同式.h"
#include "GCD.h"
// ■■■■■ 列挙（数論） ■■■■■


//【素数の列挙】O(n log(log n))
/*
* n 以下の素数を昇順に列挙したリストを返す．
*/
vi eratosthenes(int n) {
	// 参考 : https://qiita.com/peria/items/a4ff4ddb3336f7b81d50
	// verify : https://judge.yosupo.jp/problem/enumerate_primes

	if (n <= 1) return vi();

	vi ps;
	ps.reserve((int)(n / log(n + 1) * 1.1));
	ps.push_back(2); // 素数 2 だけ例外処理する．

	int hn = (n - 1) / 2;
	int sqrt_hn = (int)(sqrt(hn) + 1e-6);

	// is_prime[i] : 2i+1 が素数か
	vb is_prime(hn + 1, true);
	is_prime[0] = false;

	int i = 1;

	// √n 以下の i の処理
	for (; i <= sqrt_hn; i++) {
		if (!is_prime[i]) continue;

		int p = 2 * i + 1;
		ps.push_back(p);

		// 3(2i+1), ..., (2i-1)(2i+1) は既にふるい落とされているので (2i+1)^2 = 2(2i(i+1))+1 からで良い．
		// 増分は，(2j+1)+2(2i+1) = 2(j+2i+1)+1 なので 2i+1 である．
		for (int j = 2 * i * (i + 1); j <= hn; j += p) is_prime[j] = false;
	}

	// √n より大きい i の処理
	for (; i <= hn; i++) if (is_prime[i]) ps.push_back(2 * i + 1);

	return ps;
}


//【素数の列挙（テーブル）】O(n log(log n))
/*
* i∈[0..n] について，i が素数かを格納したリストを返す．
*/
vb eratosthenes_table(int n) {
	// verify : https://judge.yosupo.jp/problem/enumerate_primes

	// is_prime[i] : i が素数か
	vb is_prime(n + 1, true);
	is_prime[0] = is_prime[1] = false;

	int i = 2;

	// √n 以下の i の処理
	for (; i <= n / i; i++) {
		if (is_prime[i]) {
			for (int j = i * i; j <= n; j += i) is_prime[j] = false;
		}
	}

	return is_prime;
}


//【素数の列挙（区間）】O((√r + (r-l)) log(log r))
/*
* [l..r) に含まれる素数を昇順に格納したリストを返す．
*
* 利用：【素数の列挙】
*/
vl eratosthenes_interval(ll l, ll r) {
	// 参考 : https://37zigen.com/sieve-eratosthenes/
	// verify : https://algo-method.com/tasks/332

	//【備考】
	// メモリに乗り切らない範囲の数についてエラトステネスの篩を適用したいときに代用できる．

	// ps_sub : √r 以下の素数のリスト
	vi ps_sub = eratosthenes((int)(sqrt(r) + 0.01));

	// is_prime[i - l] : i が素数か
	vb is_prime(r - l, true);
	if (1 - l >= 0) is_prime[1 - l] = false;

	repe(p, ps_sub) {
		// j_min : [l..r) 内で p^2 以上の最小の p の倍数
		ll j_min = max((l + p - 1) / p * p, (ll)p * p);

		for (ll j = j_min; j < r; j += p) is_prime[j - l] = false;
	}

	vl ps;
	rep(i, r - l) if (is_prime[i]) ps.push_back(l + i);

	return ps;
}


//【約数列挙】O(√n)
/*
* n の約数全てを昇順に格納したリストを返す．
*/
template <class T>
vector<T> divisors(T n) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_fb

	vector<T> ds;

	if (n == 1) {
		ds.push_back(1);
		return ds;
	}

	T i = 1;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			ds.push_back(i);
			ds.push_back(n / i);
		}
	}
	if (i * i == n) ds.push_back(i);

	sort(all(ds));

	return ds;
}


//【約数列挙（素因数分解済）】O(σ(n))
/*
* n の素因数分解結果 pps を利用して n の約数全てを昇順に格納したリストを返す．
*/
template <class T>
vector<T> divisors(map<T, int>& pps) {
	// verify : https://atcoder.jp/contests/arc068/tasks/arc068_c

	vector<T> divs{ T(1) };
	repe(pp, pps) {
		auto [p, d] = pp;

		vector<T> powp(d);
		powp[0] = p;
		rep(i, d - 1) powp[i + 1] = powp[i] * p;

		int m = sz(divs);
		repir(j, m - 1, 0) rep(i, d) divs.push_back(divs[j] * powp[i]);
	}
	sort(all(divs));

	return divs;
}


//【B-smooth number の列挙】O((log n)^B)
/*
* n 以下の B-smooth number（素因数が全て B 以下である数）の昇順リストを返す．
*
* 利用：【素数の列挙】
*/
vl enumerate_smooth_number(ll n, int B) {
	// verify : https://projecteuler.net/problem=516

	vi ps = eratosthenes(B);
	int K = sz(ps);

	vl res;

	function<void(int, ll)> rf = [&](int k, ll x) {
		if (k == K) {
			res.push_back(x);
			return;
		}

		while (x <= n) {
			rf(k + 1, x);
			x *= ps[k];
		}
	};
	rf(0, 1);

	sort(all(res)); // 不要なら削除可能

	return res;
}


//【多冪数の列挙】O(√n log n)
/*
* n 以下の多冪数の昇順リストを返す．
* i が多冪数であるとは，任意の素数 p について p|i ⇒ p^2|i が成り立つことをいう．
*
* 利用：【素数の列挙】
*/
vl enumerate_powerful_number(ll n) {
	//【方法】
	// 多冪数は (整数)^2 * (無平方数)^3 の形で一意に表されるので，
	// (無平方数)^3 の方をバックトラッキングで全列挙する．

	auto ps = eratosthenes((int)(pow(n, 1. / 3) + 1e-6));
	int K = sz(ps);

	vl res;
	res.reserve((int)(2.174 * sqrt(n)));

	function<void(int, ll)> rf = [&](int k, ll val) {
		if (k == K) {
			// ここでループを回すのをやめれば，数え上げや総和を O(n^(1/3)) で行える．
			for (ll i = 1; i * i * val <= n; i++) {
				res.emplace_back(i * i * val);
			}
			return;
		}

		rf(k + 1, val);

		ll p3 = ps[k] * ps[k] * ps[k];
		if (val <= n / p3) {
			rf(k + 1, val * p3);
		}
	};
	rf(0, 1);

	sort(all(res)); // 不要なら削除可

	return res;
}


//【原始ピタゴラス数の列挙】O(n)
/*
* n 以下の正整数の 3 つ組 (a, b, c) で，以下の 3 条件
*	a^2 + b^2 = c^2 （a, b, c は ∠C = 90° の直角三角形の 3 辺の長さ）
*	GCD(a, b, c) = 1
*	b は偶数
* を満たすもののリストを返す．
*/
vector<tuple<int, int, int>> enumerate_primitive_pythagorean_triples(int n) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%8E%9F%E5%A7%8B%E3%83%94%E3%82%BF%E3%82%B4%E3%83%A9%E3%82%B9%E6%95%B0
	// verify : https://mojacoder.app/users/shogo314/problems/triplet

	//【方法】
	// b が偶数である各原始ピタゴラス数 (a, b, c) に対して，
	//		a = x^2 - y^2
	//		b = 2xy
	//		c = x^2 + y^2
	// を満たすような
	//		x > y
	//		GCD(x, y) = 1
	//		x ≠ y (mod 2)
	// なる正整数の組 (x, y) が 1:1 に対応する．
	// よって x^2 + y^2 ≦ n の範囲の (x, y) を調べ上げれば良い．

	//【備考】
	// 面積 S，周長 L，内接円の半径 r はそれぞれ次のように表される：
	//		S = ab/2  = xy(x+y)(x-y)
	//		L = a+b+c = 2x(x+y)
	//		r = 2S/L  = y(x-y)

	vector<tuple<int, int, int>> res;

	int x_max = (int)(sqrt(n - 1) + 1e-9);
	repi(x, 2, x_max) {
		int y_max = min((int)(sqrt(n - x * x) + 1e-9), x - 1);
		for (int y = (x & 1) + 1; y <= y_max; y += 2) {
			if (gcd(x, y) != 1) continue;

			int x2 = x * x;
			int y2 = y * y;

			int a = x2 - y2;
			int b = 2 * x * y;
			int c = x2 + y2;

			res.emplace_back(a, b, c);
		}
	}

	return res;
}


//【原始ピタゴラス数の列挙（120°）】O(n)
/*
* n 以下の正整数の 3 つ組 (a, b, c) で，以下の 3 条件
*	a^2 + ab + b^2 = c^2 （a, b, c は ∠C = 120° の三角形の 3 辺の長さ）
*	GCD(a, b, c) = 1
* を満たすもののリストを返す．
*/
vector<tuple<int, int, int>> enumerate_primitive_pythagorean_triples_120(int n) {
	// verify : https://projecteuler.net/problem=143

	//【方法】
	// 各 120° 版原始ピタゴラス数 (a, b, c) に対して，
	//		a = x^2 - y^2
	//		b = 2xy + y^2
	//		c = x^2 + xy + y^2
	// を満たすような
	//		x > y
	//		GCD(x, y) = 1
	//		x ≠ y (mod 3)
	// なる正整数の組 (x, y) または
	//		a = 2xy + y^2
	//		b = x^2 - y^2
	//		c = x^2 + xy + y^2
	// を満たすような
	//		x > y
	//		GCD(x, y) = 1
	//		x ≠ y (mod 3)
	// なる正整数の組 (x, y) が 1:1 に対応する．
	// よって x^2 + xy + y^2 ≦ n の範囲の (x, y) を調べ上げれば良い．

	vector<tuple<int, int, int>> res;

	for (int x = 2; x * x + x + 1 <= n; x++) {
		for (int y = 1; y < x && x * x + x * y + y * y <= n; y++) {
			if ((x - y) % 3 == 0) continue;
			if (gcd(x, y) != 1) continue;

			int a = x * x - y * y;
			int b = 2 * x * y + y * y;
			int c = x * x + x * y + y * y;

			res.emplace_back(a, b, c);
			res.emplace_back(b, a, c);
		}
	}

	return res;
}


//【原始ピタゴラス数の列挙（60°）】O(n)
/*
* n 以下の正整数の 3 つ組 (a, b, c) で，以下の 3 条件
*	a^2 - ab + b^2 = c^2 （a, b, c は ∠C = 60° の三角形の 3 辺の長さ）
*	GCD(a, b, c) = 1
* を満たすもののリストを返す．
*/
vector<tuple<int, int, int>> enumerate_primitive_pythagorean_triples_60(int n) {
	//【方法】
	// 各 60° 版原始ピタゴラス数 (a, b, c) に対して，
	//		a = 2xy + x^2
	//		b = 2xy + y^2
	//		c = x^2 + xy + y^2
	// を満たすような
	//		GCD(x, y) = 1
	//		x ≠ y (mod 3)
	// なる正整数の組 (x, y) か，または
	//		a = (2xy + x^2) / 3
	//		b = (2xy + y^2) / 3
	//		c = (x^2 + xy + y^2) / 3
	// を満たすような
	//		GCD(x, y) = 1
	//		x = y (mod 3)
	// なる正整数の組 (x, y) が 1:1 に対応する．
	// よって x^2 + xy + y^2 ≦ n の範囲の (x, y) を調べ上げれば良い．

	vector<tuple<int, int, int>> res;

	for (int x = 1; x * x + x + 1 <= 3 * n; x++) {
		for (int y = 1; x * x + x * y + y * y <= 3 * n; y++) {
			if (gcd(x, y) != 1) continue;

			int a = 2 * x * y + x * x;
			int b = 2 * x * y + y * y;
			int c = x * x + x * y + y * y;
			if ((x - y) % 3 == 0) {
				a /= 3;
				b /= 3;
				c /= 3;
			}

			if (max({ a, b, c }) <= n) res.emplace_back(a, b, c);
		}
	}

	return res;
}


//【フェルマーの二平方和定理（素数）】O(log p)
/*
* x^2 + y^2 = p なる (x,y) (0<x≦y，一意) を返す．（なければ (-1,-1) を返す）
*
* 制約 : p は素数
*
* 利用：【-1 の平方剰余】,【ガウス整数の最大公約数】
*/
pii fermats_4n_plus_1_prime(ll p) {
	// 参考 : https://maspypy.com/library-checker-gcd-of-gaussian-integers
	// verify : https://judge.yosupo.jp/problem/two_square_sum

	//【方法】
	// p=2 なら (x,y)=(1,1) を返せば良い．
	// p が 4n+3 型素数なら条件を満たす (x,y) は存在しない（mod 4 を考えれば明らか）
	// 以下では p が 4n+1 型素数であるとする．
	//
	// (x,y) が条件を満たすことをガウス整数の言葉で言い換えると，|x+iy| = p となる． 
	// -1 は mod p で平方剰余なので，
	//		a^2 = -1 (mod p) ⇔ (a+i)(a-i) = k p  (∃k∈[1..p-1])
	// なる a を取ることができ，
	//		|a+i| = k p
	// となる．また明らかに
	//		|p| = p^2
	// である．両者より，
	//		x+iy = gcd(a+i, p)
	// と選べば，k∈[1..p-1] より
	//		|x+iy| | gcd(kp, p^2) = p
	// とできる．(a+i)|p より |x+iy| != 1 なので，|x+iy| = p である．

	if (p == 2) return { 1, 1 };
	if (p % 4 == 3) return { -1, -1 };

	ll a = quadratic_residue_m1(p);

	auto [x, y] = gcd_gaussian_integers(a, 1, p, 0);
	x = abs(x);
	y = abs(y);
	if (x > y) swap(x, y);

	return { (int)x, (int)y };
}


//【フェルマーの二平方和定理】O(n^(1/4))
/*
* x^2 + y^2 = n なる (x,y) (0≦x≦y) 全てのリストを返す．（なければ空）
*
* 利用：【素因数分解】,【フェルマーの二平方和定理（素数）】
*/
vector<pii> fermats_4n_plus_1(ll n) {
	// 参考 : https://ja.wikipedia.org/wiki/%E4%BA%8C%E5%80%8B%E3%81%AE%E5%B9%B3%E6%96%B9%E6%95%B0%E3%81%AE%E5%92%8C
	// verify : https://judge.yosupo.jp/problem/two_square_sum

	// n = 0 については例外処理
	if (n == 0) return { {0, 0} };

	auto pps = factor_integer(n);

	// 0^2 + 1^2 = 1
	vector<pii> res{ {0, 1} };

	for (auto [p, e] : pps) {
		if (p == 2) {
			res.clear();
			if (e & 1) {
				// e = 2d+1 と表すと，
				// (2^d)^2 + (2^d)^2 = 2^(2d+1) = 2^e
				int x = 1 << (e / 2);
				res.emplace_back(x, x);
			}
			else {
				// e = 2d と表すと，
				// 0^2 + (2^d)^2 = 2^(2d) = 2^e
				res.emplace_back(0, 1 << (e / 2));
			}
		}
		else if (p % 4 == 1) {
			// xp^2 + yp^2 = p（一意）
			auto [xp, yp] = fermats_4n_plus_1_prime(p);

			// xyps : x^2 + y^2 = p^e となる (x, y) のリスト
			vector<pii> xyps{ {xp, yp} };

			rep(hoge, e - 1) {
				vector<pii> nxyps;
				for (auto [x, y] : xyps) {
					// ブラーマグプタの二平方恒等式
					// (x z - y w)^2 + (x w + y z)^2 = (x^2 + y^2)(z^2 + z^2)
					int nx = abs(x * xp - y * yp);
					int ny = x * yp + y * xp;
					if (nx > ny) swap(nx, ny);
					nxyps.emplace_back(nx, ny);

					// (x z + y w)^2 + (x w - y z)^2 = (x^2 + y^2)(z^2 + z^2)
					nx = abs(x * yp - y * xp);
					ny = x * xp + y * yp;
					if (nx > ny) swap(nx, ny);
					nxyps.emplace_back(nx, ny);
				}
				xyps = move(nxyps);
				uniq(xyps);
			}

			vector<pii> nres;
			for (auto [x, y] : res) {
				for (auto [xp, yp] : xyps) {
					// ブラーマグプタの二平方恒等式
					// (x z - y w)^2 + (x w + y z)^2 = (x^2 + y^2)(z^2 + z^2)
					int nx = abs(x * xp - y * yp);
					int ny = x * yp + y * xp;
					if (nx > ny) swap(nx, ny);
					nres.emplace_back(nx, ny);

					// (x z + y w)^2 + (x w - y z)^2 = (x^2 + y^2)(z^2 + z^2)
					nx = abs(x * yp - y * xp);
					ny = x * xp + y * yp;
					if (nx > ny) swap(nx, ny);
					nres.emplace_back(nx, ny);
				}
			}
			res = move(nres);
			uniq(res);
		}
		else {
			if (e & 1) {
				// n が 4Z+3 型素因数を奇数個含んでいる場合は二平方和で表せない．
				return {};
			}
			else {
				// 全体を定数倍する形で消費するしかない．
				int c = (int)powi(p, e / 2);
				for (auto& [x, y] : res) {
					x *= c;
					y *= c;
				}
			}
		}
	}

	return res;
}


