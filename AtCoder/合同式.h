#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ 合同式 ■■■■■



//【位数】O(√mod)
/*
* a^x = 1 となる最小の自然数 x を返す．（なければ -1）
*
* 利用：【約数列挙】
*/
int ord(const mint& a) {
	const int p = mint::mod();

	// p - 1 の約数が位数の候補となる．
	vl divs;
	divisors(p - 1, divs);

	// p - 1 の約数を昇順に調べていく．
	repe(d, divs) {
		if (a.pow(d) == 1) {
			return (int)d;
		}
	}

	return -1;
}


//【原始根】O(√p)
/*
* 素数の法 p における原始根を 1 つ返す．
* 
* 利用：【約数列挙】
*/
int find_primitive_root() {
	const int p = mint::mod();

	// p - 1 の約数 divs を得る．
	vl divs;
	divisors(p - 1, divs);

	// p - 1 自身だけ削除する．
	divs.pop_back();

	repi(r, 2, p - 2) {
		// p - 1 の真の約数が全て r の位数でないなら原始根
		repe(d, divs) {
			if (mint(r).pow(d) == 1) {
				goto NEXT_LOOP;
			}
		}
		return r;

	NEXT_LOOP:;
	}
	return -1;
}


//【離散対数問題／baby-step giant-step】O(√p)
/*
* a^x = b mod p の最小解 x >= 0 を返す．（なければ -1）
*
* 制約 : p = mint::mod() は素数
*
*（平方分割）
*/
int log(const mint& a, mint b) {
	// 参考：https://tjkendev.github.io/procon-library/python/math/baby-step-giant-step.html

	//【方法】
	// m = ceil(√p)，r = a^(-m) とおく．
	// 
	// まず 0 <= x < m の範囲の x について a^x を計算した集合 S を得る．
	// S の中に b に一致するものがあればそれでよい．
	// なかった場合は x >= m であることが確定する．
	// 
	// 次に解くべき方程式
	//		a^x = b
	// の両辺に r を掛けて
	//		a^(x-m) = b r
	// とする．
	// もし S の中に b r に一致するものがあれば，そこから x-m が分かり，
	// その結果に m を加えたものが求める x の値である．
	// なかった場合は x >= 2 m であることが確定する．
	//
	// a^(mod - 1) = 1 なので，同様のステップは高々 m 回で終了する．
	// 各回の S へのアクセスが O(1) で行えるなら，全体計算量は O(m) である．

	int m = (int)(ceil(sqrt(mint::mod())) + 0.5);

	// a = 0 の場合の例外処理
	if (a == 0) {
		if (b == 0) return 1; // 0^0 = 1 とする．
		else return -1;
	}

	// loga[a^i] = i を計算しておく．
	unordered_map<int, int> loga;
	mint p = a.pow(m), a_inv = a.inv();
	repir(i, m - 1, 0) {
		p *= a_inv;
		loga[p.val()] = i;
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

	// 見つからなかったら -1 を返す．
	return -1;
}


//【平方剰余／トネリ－シャンクスのアルゴリズム】O(√p)
/*
* x^2 = a mod p の解 x の一方を返す．（なければ -1）
*
* 制約 : p = mint::mod() は素数
*/
int sqrt(const mint& a) {
	// 参考：https://tjkendev.github.io/procon-library/python/math/tonelli-shanks.html

	//【方法】
	// p = mod, p - 1 = 2^d q（q : 奇数）と表しておく．
	// 
	// 適当な平方非剰余 z を見つける．
	// オイラーの基準より，
	//		z が平方非剰余 ⇔ z^((p-1)/2) = -1
	// である．
	//
	// t = a^q と初期化する．a は平方剰余なので，オイラーの基準より
	//		t^(2^(d-1)) = a^(2^(d-1) q) = a^((p-1)/2) = 1
	// となる．
	//
	// i = [d-2..0] について，t^(2^i) = -1 であれば
	//		t *= z^((p-1) / 2^(i+1))
	// と t を更新する．
	//		(z^((p-1) / 2^(i+1)))^(2^i) = z^(2^i (p-1) / 2^(i+1)) = z^((p-1) / 2) = -1
	// なので，この更新により t^(2^i) = 1 となる．
	// i = 0 まで更新を終えれば，最終的に t = 1 となる．
	//
	// 求める x は
	//		x = a^(1/2) = (t a)^(1/2)
	// と表されるから，2 のべきを 1 つ小さくしながら途中計算することにより，
	// 先の計算と同時に x を得ることができる．

	// 法 p を得る．
	const int p = mint::mod();

	// p = 2 の場合の例外処理
	if (p == 2) return a.val();

	// a が平方非剰余なら -1 を返す．
	if (a.pow((p - 1) / 2) == -1) {
		return -1;
	}

	// mod - 1 = 2^d q（q : 奇数）なる d, q を得る．
	int q = p - 1, d = 0;
	while (q % 2 == 0) {
		q /= 2;
		d++;
	}

	// 適当な平方非剰余 z を見つける．
	mint z = 2;
	while (z.pow((p - 1) / 2) == 1) {
		z += 1;
	}

	// t を更新しつつ結果を得る．
	mint t = a.pow(q), res = a.pow((q + 1) / 2);
	repir(i, d - 2, 0) {
		if (t.pow(1LL << i) == -1) {
			t *= z.pow((p - 1) >> (i + 1));
			res *= z.pow((p - 1) >> (i + 2));
		}
	}

	return res.val();
}


//【有限体上の計算】
struct mint {
	ll v;
	static const ll MOD = (ll)1e9 + 7; // 適切な法（素数）に書き換えてから用いる．

	// コンストラクタ
	mint() : v(0) {};
	mint(const mint& a) = default;
	mint(const int& a) : v(a % MOD) {};

	// 代入
	mint& operator=(const mint& a) { v = a.v; return *this; }
	mint& operator=(const int& a) { v = ((a % MOD) + MOD) % MOD; return *this; }

	// 入出力
	friend istream& operator>> (istream& is, mint& x) { is >> x.v; x.v = ((x.v % MOD) + MOD) % MOD; return is; } // mint の入力用
	friend ostream& operator<< (ostream& os, const mint& x) { os << x.v; return os; } // mint の出力用

	// 比較
	bool operator==(const mint& b) const { return v == b.v; }
	bool operator==(const int& b) const { return v == ((b % MOD) + MOD) % MOD; }
	friend bool operator==(const int& a, const mint& b) { return b == a; }

	// 演算
	mint& operator+=(const mint& b) { v = ((v + b.v) % MOD + MOD) % MOD; return *this; }
	mint& operator-=(const mint& b) { v = ((v - b.v) % MOD + MOD) % MOD; return *this; }
	mint& operator*=(const mint& b) { v = ((v * b.v) % MOD + MOD) % MOD; return *this; }
	mint& operator/=(const mint& b) { *this *= b.inv(); return *this; }
	mint operator+(const mint& b) const { mint a = *this; return a += b; }
	mint operator-(const mint& b) const { mint a = *this; return a -= b; }
	mint operator*(const mint& b) const { mint a = *this; return a *= b; }
	mint operator/(const mint& b) const { mint a = *this; return a /= b; }
	mint operator-() const { mint a = *this; return a *= -1; }

	// int との演算
	mint& operator+=(const int& b) { v = ((v + b % MOD) % MOD + MOD) % MOD; return *this; }
	mint& operator-=(const int& b) { v = ((v - b % MOD) % MOD + MOD) % MOD; return *this; }
	mint& operator*=(const int& b) { v = ((v * b % MOD) % MOD + MOD) % MOD; return *this; }
	mint& operator/=(const int& b) { *this *= mint(b).inv(); return *this; }
	mint operator+(const int& b) const { mint a = *this; return a += b; }
	mint operator-(const int& b) const { mint a = *this; return a -= b; }
	mint operator*(const int& b) const { mint a = *this; return a *= b; }
	mint operator/(const int& b) const { mint a = *this; return a /= b; }
	friend mint operator+(const int& a, const mint& b) { return b + a; }
	friend mint operator-(const int& a, const mint& b) { return -(b - a); }
	friend mint operator*(const int& a, const mint& b) { return b * a; }
	friend mint operator/(const int& a, const mint& b) { return mint(a) * b.inv(); }

	// 累乗
	mint pow(ll d) const {
		mint res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1LL) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 逆元
	mint inv() const { return pow(MOD - 2); }

	// 法の確認
	static int mod() { return MOD; }

	// 値の確認
	int val() const { return int(v % MOD); }
};
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;


