#pragma once
#include "header.h"
#include "FPS(bit).h"
// ■■■■■ ハッシュ ■■■■■


//【ローリングハッシュ（列）】
/*
* 列 s[0..n) の連続部分列 s[l, r) のハッシュ値を計算する．
*
* Rolling_hash(STR s) : O(n)
*	列 s[0..n) で初期化する．
*	STR は string，vector<T> など．
*
* ll get(int l, int r) : O(1)
*	連続部分列 s[l, r) のハッシュ値を返す（空なら 0）
*
* ll join(ll hs, ll ht, int ls) : O(1)
*	ハッシュ値 hs をもつ s[0..ls) とハッシュ値 ht をもつ t を連結した s+t のハッシュ値を返す．
*/
template <class STR, int MOD, int BASE, int SHIFT> struct Rolling_hash_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;

	mint B = BASE; // 適当な基数
	mint S = SHIFT; // 適当なシフト

	// 列の長さ
	int n;

	// powB[i] : B^i, powB_inv[i] : B^(-i)
	vm powB, powB_inv;

	// v[i] : s[0, i) のハッシュ値
	vm v;

	// コンストラクタ（列 s で初期化）
	Rolling_hash_sub() : n(0) {}
	Rolling_hash_sub(const STR& s) : n(sz(s)), powB(n + 1), powB_inv(n + 1), v(n + 1) {
		// ハッシュ値計算用の B の累乗
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B;

		// ハッシュ値計算用の B の逆元の累乗
		mint invB = B.inv();
		powB_inv[0] = 1;
		rep(i, n) powB_inv[i + 1] = powB_inv[i] * invB;

		// s[0, i) のハッシュ値 v[i] の計算
		rep(i, n) v[i + 1] = v[i] + (s[i] + S) * powB[i];
	}

	// 代入
	Rolling_hash_sub(const Rolling_hash_sub& rh) = default;
	Rolling_hash_sub& operator=(const Rolling_hash_sub& rh) = default;

	// s[l, r) のハッシュ値の取得
	int get(int l, int r) {
		// ハッシュ値は Σi=[0..r-l) (s[l+i] + S) * B^i (mod MOD)
		return ((v[r] - v[l]) * powB_inv[l]).val();
	}

	// ハッシュ値 hs をもつ s[0..ls) とハッシュ値 ht をもつ t を連結した s+t のハッシュ値を返す．
	int join(int hs, int ht, int ls) {
		return (hs + ht * powB[ls]).val();
	}
};
template <class STR>
class Rolling_hash {
	int n; // 列の長さ

	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	Rolling_hash_sub<STR, 1999987657, 114521, 17> rh1;
	Rolling_hash_sub<STR, 1999901261, 314159, 91> rh2;

public:
	// コンストラクタ（文字列 s で初期化）
	Rolling_hash() : n(0) {}
	Rolling_hash(const STR& s) : n(sz(s)), rh1(s), rh2(s) {}

	// 代入
	Rolling_hash(const Rolling_hash& rh) = default;
	Rolling_hash& operator=(const Rolling_hash& rh) = default;

	// 列の長さの取得
	int size() { return n; }

	// s[l, r) のハッシュ値の取得
	ll get(int l, int r) {
		// verify : https://atcoder.jp/contests/abc175/tasks/abc175_f

		if (r <= 0 || l >= n || l >= r) return 0;
		chmax(l, 0); chmin(r, n);
		return (ll(rh1.get(l, r)) << 32) + ll(rh2.get(l, r));
	}

	// ハッシュ値 hs をもつ s[0..ls) とハッシュ値 ht をもつ t を連結した s+t のハッシュ値を返す．
	ll join(ll hs, ll ht, int ls) {
		// verify : https://atcoder.jp/contests/arc050/tasks/arc050_d

		int hs1 = (int)(hs >> 32), hs2 = (int)(hs % (1LL << 32));
		int ht1 = (int)(ht >> 32), ht2 = (int)(ht % (1LL << 32));
		return (ll(rh1.join(hs1, ht1, ls)) << 32) + ll(rh2.join(hs2, ht2, ls));
	}
};


//【二次元ローリングハッシュ（格子）】
/*
* 二次元配列 a の部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を計算する．
*
* Rolling_hash_2D(vvT a) : O(|w| |h|)
*	二次元配列 a で初期化する．
*
* ll get(int x1, int y1, int x2, int y2) : O(1)
*	部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
*/
template <class T, int MOD, int BASE_X, int BASE_Y, int SHIFT> struct Rolling_hash_2D_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;
	using vvm = vector<vm>;

	mint BX = BASE_X; // 適当な基数
	mint invBX = BX.inv(); // 基数の逆数
	mint BY = BASE_Y;
	mint invBY = BY.inv();
	mint S = SHIFT; // 適当なシフト

	// 二次元配列とその大きさ
	vector<vector<T>> a; int h, w;

	// v[i][j] : 長方形領域 [0, i) * [0, j) のハッシュ値
	vvm v;

	// ハッシュ値計算用の B の累乗，B の逆数の累乗
	vm pow_BX, pow_BY, pow_invBX, pow_invBY;

	// コンストラクタ（文字列 s で初期化）
	Rolling_hash_2D_sub() : h(0), w(0) {}
	Rolling_hash_2D_sub(vector<vector<T>>& a_) :
		a(a_), h(sz(a)), w(sz(a[0])), v(h + 1, vm(w + 1)),
		pow_BX(h + 1), pow_BY(w + 1), pow_invBX(h + 1), pow_invBY(w + 1) {

		// ハッシュ値計算用の B の累乗の前計算
		pow_BX[0] = pow_BY[0] = pow_invBX[0] = pow_invBY[0] = 1;
		rep(i, h) {
			pow_BX[i + 1] = pow_BX[i] * BX;
			pow_invBX[i + 1] = pow_invBX[i] * invBX;
		}
		rep(j, w) {
			pow_BY[j + 1] = pow_BY[j] * BY;
			pow_invBY[j + 1] = pow_invBY[j] * invBY;
		}

		// 長方形領域 [0, i) * [0, j) のハッシュ値の計算
		rep(i, h) {
			rep(j, w) {
				v[i + 1][j + 1] = v[i + 1][j] + v[i][j + 1] - v[i][j]
					+ (a[i][j] + S) * pow_BX[i] * pow_BY[j];
			}
		}
	}

	// 代入
	Rolling_hash_2D_sub(const Rolling_hash_2D_sub& rh) = default;
	Rolling_hash_2D_sub& operator=(const Rolling_hash_2D_sub& rh) = default;

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	int get(int x1, int y1, int x2, int y2) {
		// ハッシュ値は次の式により計算する：
		// Σi=[0..x2-x1] j=[0..y2-y1] (a[x1+i][y1+j] + SHIFT) * BASE_X^i * BASE_Y^j (mod MOD)
		mint v_sum = v[x2][y2] - v[x1][y2] - v[x2][y1] + v[x1][y1];
		return (v_sum * pow_invBX[x1] * pow_invBY[y1]).val();
	}
};
template <class T>
struct Rolling_hash_2D {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	Rolling_hash_2D_sub<T, 1000000007, 100007, 26627, 17> rh1;
	Rolling_hash_2D_sub<T, 998244353, 99991, 54401, 91> rh2;

	// コンストラクタ（二次元配列 a で初期化）
	Rolling_hash_2D() {}
	Rolling_hash_2D(vector<vector<T>>& a) : rh1(a), rh2(a) {}

	// 代入
	Rolling_hash_2D(const Rolling_hash_2D& rh) = default;
	Rolling_hash_2D& operator=(const Rolling_hash_2D& rh) = default;

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	ll get(int x1, int y1, int x2, int y2) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_C
		
		return (ll(rh1.get(x1, y1, x2, y2)) << 32) + ll(rh2.get(x1, y1, x2, y2));
	}
};


//【ローリングハッシュ（数値文字列，加減可能）】
/*
* 数値文字列 s の連続部分列 s[l, r) が表す数値のハッシュ値を計算する．
* ハッシュ値のまま加減算を行うことができる．
*
* Number_rolling_hash(string s) : O(n)
*	列 s[0..n) で初期化する．
*
* ll get(int l, int r) : O(1)
*	連続部分列 s[l, r) が表す数値のハッシュ値を返す．
*
* ll add(ll hA, ll hB) : O(1)
*	ハッシュ値 hA, hB が表す数値の和のハッシュを返す．
*
* ll sub(ll hA, ll hB) : O(1)
*	ハッシュ値 hA, hB が表す数値の差（hA 側 - hB 側）のハッシュを返す．
*/
template <int MOD> struct Number_rolling_hash_sub {
	// 列とその長さ
	string s; int n;

	// v[i] : s[0, i) のハッシュ値
	vl v;

	// pow10[i] : 10^i
	vl pow10;

	// コンストラクタ（列 s で初期化）
	Number_rolling_hash_sub(const string& s_) : s(s_), n(sz(s)), v(n + 1), pow10(n + 1) {
		// ハッシュ値計算用の 10 の累乗
		pow10[0] = 1;
		rep(i, n) pow10[i + 1] = (pow10[i] * 10) % MOD;

		// 10^(-1) の計算
		ll inv10 = 1, pow2 = 10, d = MOD - 2;
		while (d > 0) {
			if (d & 1) inv10 = (inv10 * pow2) % MOD;
			pow2 = (pow2 * pow2) % MOD;
			d /= 2;
		}

		// s[0, i) のハッシュ値の計算
		ll pow_inv10 = 1;
		rep(i, n) {
			v[i + 1] = (v[i] + (s[i] - '0') * pow_inv10) % MOD;
			pow_inv10 = (pow_inv10 * inv10) % MOD;
		}
	}

	// s[l, r) のハッシュ値の取得
	int get(int l, int r) {
		// ハッシュ値は Σi=[0..r-l] s[l+i] * 10^(r-l-i) (mod MOD)
		return (int)smod((v[r] - v[l]) * pow10[r], MOD);
	}
};
struct Number_rolling_hash {
	static const int MOD1 = 1000000007;
	static const int MOD2 = 998244353;

	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	Number_rolling_hash_sub<MOD1> rh1;
	Number_rolling_hash_sub<MOD2> rh2;

	// コンストラクタ（文字列 s で初期化）
	Number_rolling_hash(const string& s) : rh1(s), rh2(s) {}

	// 連続部分列 s[l, r) が表す数値のハッシュ値を返す．
	ll get(int l, int r) {
		// verify : https://codeforces.com/contest/898/problem/F
		
		return ((ll)rh1.get(l, r) << 32) + rh2.get(l, r);
	}

	// ハッシュ値 hA, hB が表す数値の和のハッシュを返す．
	ll add(ll hA, ll hB) {
		// verify : https://codeforces.com/contest/898/problem/F
		
		ll hA1 = hA >> 32, hA2 = hA % (1LL << 32);
		ll hB1 = hB >> 32, hB2 = hB % (1LL << 32);

		ll h1 = (hA1 + hB1) % MOD1;
		ll h2 = (hA2 + hB2) % MOD2;

		return (h1 << 32) + h2;
	}

	// ハッシュ値 hA, hB が表す数値の差のハッシュを返す．
	ll sub(ll hA, ll hB) {
		ll hA1 = hA >> 32, hA2 = hA % (1LL << 32);
		ll hB1 = hB >> 32, hB2 = hB % (1LL << 32);

		ll h1 = smod(hA1 - hB1, MOD1);
		ll h2 = smod(hA2 - hB2, MOD2);

		return (h1 << 32) + h2;
	}
};


//【ローリングハッシュ（列，XOR 可能）】
/*
* 列 s[0..n) の連続部分列 s[l, r) のハッシュ値を計算する．
* ハッシュ値のまま列同士の XOR を計算することができる．
*
* Rolling_hash_XOR(STR s) : O(64 n)
*	列 s[0..n) で初期化する．
*	STR は string，vector<T> など．
*
* ll get(int l, int r) : O(1)
*	連続部分列 s[l, r) のハッシュ値を返す（空なら 0）
*
* ll join(ll hs, ll ht, int ls) : O(1)
*	ハッシュ値 hs をもつ s[0..ls) とハッシュ値 ht をもつ t を連結した s+t のハッシュ値を返す．
*
* ll xor_sum(ll hs, ll ht, int l) : O(1)
*	ハッシュ値 hs[ht] をもつ s[0..l)[ t[0..l) ] について s XOR t のハッシュ値を返す．
*
* 利用：【形式的冪級数（二元体 F2）】
*/
template <class STR>
struct Rolling_hash_XOR {
	//【方法】
	// 通常のローリングハッシュでは mod p での (和, 積)-半環上で計算するが，
	// それに代えて体 GF(2^63) ~= F_2[X] / (X^63 + X + 1) 上で計算を行えばいい．

	// 列の長さ
	int n;

	BFPS<128> MOD{ bitset<128>(9223372036854775811), 64 }; // X^63 + X + 1
	BFPS<128> B{ bitset<128>(8214269207820942862), 64 }; // ランダム
	BFPS<128> invB{ bitset<128>(7314534990125951741), 64 }; // B の逆元
	BFPS<128> S{ bitset<128>(5272143306228089744), 64 }; // ランダム

	// powB[i] : B^i, powB_inv[i] : B^(-i)
	vector<BFPS<128>> powB, powB_inv;

	// v[i] : s[0, i) のハッシュ値
	// v0[i] : 0 が i 個並んだ列のハッシュ値
	vector<BFPS<128>> v, v0;

	// コンストラクタ（列 s で初期化）
	Rolling_hash_XOR() : n(0) {}
	Rolling_hash_XOR(const STR& s) : n(sz(s)), powB(n + 1), powB_inv(n + 1), v(n + 1), v0(n + 1) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		// ハッシュ値計算用の B の累乗
		powB[0] = 1;
		rep(i, n) powB[i + 1] = (powB[i] * B).reminder(MOD);

		// ハッシュ値計算用の B の逆元の累乗
		powB_inv[0] = 1;
		rep(i, n) powB_inv[i + 1] = (powB_inv[i] * invB).reminder(MOD);

		// s[0, i) のハッシュ値 v[i] の計算
		rep(i, n) {
			BFPS<128> fs(bitset<128>(s[i]), 64);
			v[i + 1] = (v[i] + (fs + S) * powB[i]).reminder(MOD);
		}

		// 0 が i 個並んだ列のハッシュ値 v0[i] の計算
		rep(i, n) v0[i + 1] = (v0[i] + S * powB[i]).reminder(MOD);
	}

	// 代入
	Rolling_hash_XOR(const Rolling_hash_XOR& rh) = default;
	Rolling_hash_XOR& operator=(const Rolling_hash_XOR& rh) = default;

	// s[l, r) のハッシュ値の取得
	ll get(int l, int r) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		// ハッシュ値は Σi=[0..r-l) (s[l+i] + S) * B^i
		return (ll)(((v[r] + v[l]) * powB_inv[l]).reminder(MOD).c.to_ullong());
	}

	// ハッシュ値 hs をもつ s[0..ls) とハッシュ値 ht をもつ t を連結した s+t のハッシュ値を返す．
	ll join(ll hs, ll ht, int ls) {
		BFPS<128> fs(bitset<128>(hs), 64);
		BFPS<128> ft(bitset<128>(ht), 64);
		return (ll)((fs + ft * powB[ls]).reminder(MOD).c.to_ullong());
	}

	// ハッシュ値 hs[ht] をもつ s[0..l)[ t[0..l) ] について s XOR t のハッシュ値を返す．
	ll xor_sum(ll hs, ll ht, int l) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		return hs ^ ht ^ (ll)v0[l].c.to_ullong();
	}
};


//【Zobrist Hash（集合）】
/*
* 集合 S ⊂ X のハッシュ値を計算する．
*
* Zobrist_hash_set<X>() : O(1)
*	S を空集合として初期化する．
*
* flip(X x) : O(1)
*	S に対し要素 x の有無を反転する．
*
* ll get() : O(1)
*	現時点での集合 S のハッシュ値を返す．
*/
template <class X>
class Zobrist_hash_set {
	// ハッシュ値
	ll v;

	// 各 x ∈ X に対するハッシュの割り当て
	unordered_map<X, ll> x_to_hash;

	// 乱数生成器
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

public:
	// コンストラクタ（空集合で初期化）
	Zobrist_hash_set() : v(0) {
		// verify : https://www.codechef.com/problems/COOK82D
		
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(-INFL, INFL);
	}

	// S に対し要素 x の有無を反転する．
	void flip(const X& x) {
		// verify : https://www.codechef.com/problems/COOK82D

		// x が初めて出現した場合はハッシュ値を割り当てる．
		if (!x_to_hash.count(x)) x_to_hash[x] = rnd(mt);

		// ハッシュ値の更新
		v ^= x_to_hash[x];
	}

	// 現時点での集合 S のハッシュ値を返す．
	ll get() {
		// verify : https://www.codechef.com/problems/COOK82D
		
		return v;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_set z) {
		os << "v: " << z.v << endl;
		return os;
	}
#endif
};


//【Zobrist Hash（部分写像）】
/*
* 部分写像 f : X → Y のハッシュ値を計算する．
*
* Zobrist_hash_pmap<X, Y>(Y nullY) : O(1)
*	f を空の部分写像として初期化する．nullY は Y に含まれない任意の元とする．
*
* set(X x, Y y) : O(1)
*	f(x) = y を追加（または変更）する．
*
* erase(X x) : O(1)
*	f(x) を未定義にする．
*
* ll get() : O(1)
*	現時点での部分写像 f のハッシュ値を返す．
*/
template <class X, class Y>
struct Zobrist_hash_pmap {
	// ハッシュ値
	ll v;

	// 各 x ∈ X に対する番号の割り当て
	unordered_map<X, int> x_to_i;
	int i_cnt;

	// 各 x ∈ X に対する y ∈ Y の割り当て（未定義は nullY で表す）
	vector<Y> i_to_y;
	Y nullY;

	// 各 f(x) = y に対するハッシュ値の割り当て
	vector<unordered_map<Y, ll>> iy_to_v;

	// 乱数生成器
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

	// コンストラクタ（空の部分写像で初期化）
	Zobrist_hash_pmap(const Y& nullY_) : v(0), i_cnt(0), nullY(nullY_) {
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(numeric_limits<ll>::lowest(), numeric_limits<ll>::max());
	}

	// f(x) = y を追加（または変更）する．
	void set(const X& x, const Y& y) {
		// ハッシュ値の巻き戻し
		if (x_to_i.count(x)) {
			int i = x_to_i[x];
			if (i_to_y[i] != nullY) {
				v ^= iy_to_v[i][i_to_y[i]];
			}
		}

		// x が初めて出現した場合は番号を割り当てる．
		if (!x_to_i.count(x)) {
			x_to_i[x] = i_cnt++;
			i_to_y.push_back(y);
			iy_to_v.push_back(unordered_map<Y, ll>());
		}
		int i = x_to_i[x];

		// f(x) として y が初めて出現した場合はハッシュ値を割り当てる．
		if (!iy_to_v[i].count(y)) {
			iy_to_v[i][y] = rnd(mt);
		}

		// ハッシュ値の更新
		v ^= iy_to_v[i][y];

		// 部分写像の更新
		i_to_y[i] = y;
	}

	// f(x) を未定義にする．
	void erase(X& x) {
		if (x_to_i.count(x)) {
			int i = x_to_i[x];
			if (i_to_y[i] != nullY) {
				v ^= iy_to_v[i][i_to_y[i]];
				i_to_y[x_to_i[x]] = nullY;
			}
		}
	}

	// 現時点での部分写像 f のハッシュ値を返す．
	ll get() { return v; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_pmap z) {
		repe(xi, z.x_to_i) {
			os << xi.first << "->" << z.i_to_y[xi.second] << endl;
		}
		os << "v: " << z.v << endl;
		return os;
	}
#endif
};


//【Zobrist Hash（部分関数）】
/*
* 部分関数 f : X → Z のハッシュ値を計算する．
*
* Zobrist_hash_pfunc() : O(1)
*	f を空の部分関数として初期化する．
*
* set(X x, int y) : O(1)
*	f(x) = y を追加（または変更）する．
*
* erase(X x) : O(1)
*	f(x) を未定義にする．
*
* ll get() : O(1)
*	現時点での部分写像 f のハッシュ値を返す．
*/
template <class X>
struct Zobrist_hash_pfunc {
	// ハッシュ値
	ll v;

	// 各 x ∈ X に対する番号の割り当て
	unordered_map<X, int> x_to_i;
	int i_cnt;

	// 各 x ∈ X に対する y ∈ Y の割り当て（未定義は INF で表す）
	vi i_to_y;

	// 各 f(x) = y に対するハッシュ値の割り当て
	// (i, y) の組は (i << 32) + y で表す．
	unordered_map<ll, ll> iy_to_v;

	// 乱数生成器
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

	// コンストラクタ（空の部分関数で初期化）
	Zobrist_hash_pfunc() : v(0), i_cnt(0) {
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(numeric_limits<ll>::min(), numeric_limits<ll>::max());
	}

	// f(x) = y を追加（または変更）する．
	void set(const X& x, int y) {
		// ハッシュ値の巻き戻し
		if (x_to_i.count(x)) {
			int i = x_to_i[x];
			if (i_to_y[i] != INF) {
				v ^= iy_to_v[((ll)i << 32) + i_to_y[i]];
			}
		}

		// x が初めて出現した場合は番号を割り当てる．
		if (!x_to_i.count(x)) {
			x_to_i[x] = i_cnt++;
			i_to_y.push_back(y);
		}
		int i = x_to_i[x];
		ll iy = ((ll)i << 32) + y;

		// f(x) として y が初めて出現した場合はハッシュ値を割り当てる．
		if (!iy_to_v.count(iy)) {
			iy_to_v[iy] = rnd(mt);
		}

		// ハッシュ値の更新
		v ^= iy_to_v[iy];

		// 部分写像の更新
		i_to_y[i] = y;
	}

	// f(x) を未定義にする．
	void erase(X& x) {
		if (x_to_i.count(x)) {
			int i = x_to_i[x];
			if (i_to_y[i] != INF) {
				v ^= iy_to_v[((ll)i << 32) + i_to_y[i]];
				i_to_y[x_to_i[x]] = INF;
			}
		}
	}

	// 現時点での部分関数 f のハッシュ値を返す．
	ll get() { return v; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_pfunc z) {
		repe(xi, z.x_to_i) {
			os << xi.first << "->" << z.i_to_y[xi.second] << endl;
		}
		os << "v: " << z.v << endl;
		return os;
	}
#endif
};


//【Zobrist Hash（関数）】
/*
* 関数 f : X → Z のハッシュ値を計算する．
*
* Zobrist_hash_func<X>() : O(1)
*	f を零関数として初期化する．
*
* set(X x, int y) : O(1)
*	f(x) = y とする．
*
* add(X x, int y) : O(1)
*	f(x) += y とする．
*
* ll get() : O(1)
*	現時点での部分写像 f のハッシュ値を返す．
*/
template <class X>
struct Zobrist_hash_func {
	// ハッシュ値
	ll v;

	// 各 x ∈ X に対するハッシュの割り当て
	unordered_map<X, ll> x_to_hash;

	// 各 x ∈ X に対する y ∈ Y の割り当て
	unordered_map<X, int> x_to_y;

	// 乱数生成器
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

	// コンストラクタ（零関数で初期化）
	Zobrist_hash_func() : v(0) {
		// verify : https://atcoder.jp/contests/abc259/tasks/abc259_e

		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(numeric_limits<int>::min(), numeric_limits<int>::max());
	}

	// f(x) += y とする．
	void add(const X& x, int y) {
		// x が初めて出現した場合はハッシュ値を割り当てる．
		if (!x_to_hash.count(x)) {
			x_to_hash[x] = rnd(mt);
		}

		// ハッシュ値の更新
		v += x_to_hash[x] * y;

		// 関数の更新
		x_to_y[x] += y;
	}

	// f(x) = y とする．
	void set(const X& x, int y) {
		// verify : https://atcoder.jp/contests/abc259/tasks/abc259_e

		add(x, y - x_to_y[x]);
		x_to_y[x] = y;
	}

	// 現時点での関数 f のハッシュ値を返す．
	ll get() { 
		// verify : https://atcoder.jp/contests/abc259/tasks/abc259_e
		
		return v;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_func z) {
		os << z.x_to_y << endl << "v: " << z.v << endl;
		return os;
	}
#endif
};


//【Zobrist Hash（Z/mZ への関数）】
/*
* 関数 f : X → Z/mZ のハッシュ値を計算する．
*
* Zobrist_hash_func_mod<X>(int m) : O(1)
*	f を零関数として初期化する．
*
* set(X x, int y) : O(1)
*	f(x) = y とする．
*
* add(X x, int y) : O(1)
*	f(x) += y とする．
*
* ll get() : O(1)
*	現時点での部分写像 f のハッシュ値を返す．
*/
template <class X>
struct Zobrist_hash_func_mod {
	// verify : https://atcoder.jp/contests/abc238/tasks/abc238_g

	// ハッシュ値
	ll v;

	// 法
	int m;

	// 各 x ∈ X に対するハッシュの割り当て
	unordered_map<X, ll> x_to_hash;

	// 各 x ∈ X に対する y ∈ Y の割り当て
	unordered_map<X, int> x_to_y;

	// 乱数生成器
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

	// コンストラクタ（零関数で初期化）
	Zobrist_hash_func_mod() : v(0), m(1) {}
	Zobrist_hash_func_mod(int m_) : v(0), m(m_) {
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(-INFL / m, INFL / m);
	}

	// f(x) = y とする．
	void set(const X& x, int y) {
		// x が初めて出現した場合はハッシュ値を割り当てる．
		if (!x_to_hash.count(x)) {
			x_to_hash[x] = rnd(mt);
		}

		// ハッシュ値の巻き戻し
		v -= x_to_hash[x] * x_to_y[x];

		// 関数の更新
		x_to_y[x] = smod(y, m);

		// ハッシュ値の更新
		v += x_to_hash[x] * smod(y, m);
	}

	// f(x) += y とする．
	void add(const X& x, int y) { set(x, x_to_y[x] + y); }

	// 現時点での関数 f のハッシュ値を返す．
	ll get() { return v; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_func_mod z) {
		os << z.x_to_y << endl << "v: " << z.v << endl;
		return os;
	}
#endif
};


