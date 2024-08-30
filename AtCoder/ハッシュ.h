#pragma once
#include "header.h"
#include "探索.h"
#include "不偏ゲーム.h"
// ■■■■■ ハッシュ ■■■■■


//【ローリングハッシュ（列）】
/*
* Rolling_hash<STR>(STR s, bool reversible = false) : O(n)
*	列 s[0..n) で初期化する．reversible = true にすると逆順のハッシュも計算可能になる．
*	制約：STR は string，vector<T> など．ll 範囲の負数は扱えない．
*
* ull get(int l, int r) : O(1)
*	部分文字列 s[l..r) のハッシュ値を返す（空なら 0）
*
* ull get_rev(int l, int r) : O(1)
*	部分文字列 s[l..r) を反転した文字列のハッシュ値を返す（空なら 0）
*
* ull join(ull hs, ull ht, int len) : O(1)
*	ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
*/
template <class STR>
class Rolling_hash {
	// 参考 : https://qiita.com/keymoon/items/11fac5627672a6d6a9f6

	//【方法】
	// 2^61 - 1 は十分大きい素数であるからローリングハッシュの法として適切である．
	// a, b < 2^61 - 1 とし，積 a b mod (2^61 - 1) を高速に計算できればよい．
	// 
	// まず a, b を上位と下位に分解し
	//		a = 2^31 ah + al, b = 2^31 bh + bl  (ah, bh < 2^30, al, bl < 2^31)
	// とする．これらの積をとると，
	//		a b
	//		= (2^31 ah + al)(2^31 bh + bl)
	//		= 2^62 ah bh + 2^31 (ah bl + bh al) + al bl
	// となる．2^61 ≡ 1 (mod 2^61 - 1) に注意してそれぞれの項を mod 2^61 - 1 で整理する．
	//
	// 第 1 項については，
	//		2^62 ah bh
	//		= 2 ah bh
	//		≦ 2 (2^30-1) (2^30-1)
	// となる．
	//
	// 第 2 項については，c := ah bl + bh al < 2^62 を上位と下位に分解し
	//		c = 2^30 ch + cl  (ch < 2^32, cl < 2^30)
	// とすると，
	//		2^31 c
	//		= 2^31 (2^30 ch + cl)
	//		= ch + 2^31 cl
	//		≦ (2^32-1) + 2^31 (2^30-1)
	// となる．
	//
	// 第 3 項については，
	//		al bl
	//		≦ (2^31-1) (2^31-1)
	// となる．
	// 
	// これらの和は
	//		2 ah bh + ch + 2^31 cl + al bl
	//		≦ 2 (2^30-1) (2^30-1) + (2^32-1) + 2^31 (2^30-1) + (2^31-1) (2^31-1)
	//		= 9223372030412324866 < 9223372036854775808 = 2^63 << 2^64
	// となるのでオーバーフローの心配はない．

	static constexpr ull MASK30 = (1ULL << 30) - 1;
	static constexpr ull MASK31 = (1ULL << 31) - 1;
	static constexpr ull MOD = (1ULL << 61) - 1; // 法（素数）

	// a mod (2^61 - 1) を返す．
	inline ull get_mod(ull a) const {
		ull ah = a >> 61, al = a & MOD;
		ull res = ah + al;
		if (res >= MOD) res -= MOD;
		return res;
	}

	// x ≡ a b mod (2^61 - 1) なる x < 2^63 を返す（ただし a, b < 2^61）
	inline ull mul(ull a, ull b) const {
		ull ah = a >> 31, al = a & MASK31;
		ull bh = b >> 31, bl = b & MASK31;

		ull c = ah * bl + bh * al;
		ull ch = c >> 30, cl = c & MASK30;

		ull term1 = 2 * ah * bh;
		ull term2 = ch + (cl << 31);
		ull term3 = al * bl;

		return term1 + term2 + term3; // < 2^63
	}

	static constexpr ull BASE = 1234567891011; // 適当な基数（本当は実行時に乱択すべき）
	static constexpr ull SHIFT = 4295090752; // 適当なシフト

	// 列の長さ
	int n;

	// powB[i] : BASE^i
	vector<ull> powB;

	// v[i] : s[0..i) のハッシュ値 Σj∈[0..i) (s[j]+SHIFT) BASE^(i-1-j)
	// v_rev[i] : s[n-i..n) を反転した文字列のハッシュ値
	vector<ull> v, v_rev;

public:
	// 列 s[0..n) で初期化する．
	Rolling_hash(const STR& s, bool reversible = false) : n(sz(s)), powB(n + 1), v(n + 1) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ec

		powB[0] = 1;
		rep(i, n) powB[i + 1] = get_mod(mul(powB[i], BASE));

		rep(i, n) v[i + 1] = get_mod(mul(v[i], BASE) + (ull)s[i] + SHIFT);

		if (reversible) {
			v_rev.resize(n + 1);
			rep(i, n) v_rev[i + 1] = get_mod(mul(v_rev[i], BASE) + (ull)s[n - 1 - i] + SHIFT);
		}
	}
	Rolling_hash() : n(0) {}

	// s[l..r) のハッシュ値の取得
	ull get(int l, int r) const {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ec

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return get_mod(v[r] + 4 * MOD - mul(v[l], powB[r - l]));
	}

	// s[l..r) を反転した文字列のハッシュ値の取得
	ull get_rev(int l, int r) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ec

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;
		Assert(!v_rev.empty());

		// s[l..r) を反転した文字列は s_rev[n-r..n-l) に等しい．
		return get_mod(v_rev[n - l] + 4 * MOD - mul(v_rev[n - r], powB[r - l]));
	}

	// ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
	ull join(ull hs, ull ht, int len) const {
		// verify : https://atcoder.jp/contests/abc284/tasks/abc284_f

		Assert(len <= n);
		return get_mod(ht + mul(hs, powB[len]));
	}

	// ハッシュ値 h をもつ s[0..len) を K 個連結した文字列のハッシュ値を返す．
	ull repeat(ull h, int len, ll K) const {
		// verify : https://mojacoder.app/users/bayashiko/problems/rps

		Assert(len <= n);

		ull res = 0, pow2 = h; ll len_pow2 = len;
		while (K > 0) {
			if (K & 1) res = join(res, pow2, len_pow2);
			pow2 = join(pow2, pow2, len_pow2);
			len_pow2 *= 2;
			K /= 2;
		}

		return res;
	}
};


//【辞書順比較（ローリングハッシュ）】O(log(r-l))
/*
* ハッシュ rh1, rh2 をもつ文字列 s1, s2 について，s1[l1..r1) < s2[l2..r2) かを返す．
*
* 利用：【ローリングハッシュ（列）】，【めぐる式二分探索】
*/
template <class STR>
bool comp(const STR& s1, const Rolling_hash<STR>& rh1, int l1, int r1,
	const STR& s2, const Rolling_hash<STR>& rh2, int l2, int r2)
{
	// verify : https://judge.yosupo.jp/problem/suffixarray

	chmax(l1, 0); chmin(r1, sz(s1)); chmax(l2, 0); chmin(r2, sz(s2));
	if (l1 >= r1 || l2 >= r2) return 0;

	// 0 文字目（あれば）を見るだけで決まる場合も多いはず．
	if (r2 - l2 == 0) return false;
	if (r1 - l1 == 0) return true;
	if (s1[l1] < s2[l2]) return true;
	if (s1[l1] > s2[l2]) return false;

	// 1 文字目（あれば）を見るだけで決まる場合も多いはず．
	if (r2 - l2 == 1) return false;
	if (r1 - l1 == 1) return true;
	if (s1[l1 + 1] < s2[l2 + 1]) return true;
	if (s1[l1 + 1] > s2[l2 + 1]) return false;

	// 接頭辞が len 文字一致しているか
	function<bool(int)> okQ = [&](int len) {
		auto hash1 = rh1.get(l1, l1 + len);
		auto hash2 = rh2.get(l2, l2 + len);
		return hash1 == hash2;
	};
	int len = meguru_search(2, min(r1 - l1, r2 - l2) + 1, okQ);

	// len 文字目（あれば）を見て比較する．
	if (r2 - l2 == len) return false;
	if (r1 - l1 == len) return true;
	return s1[l1 + len] < s2[l2 + len];
}


//【動的ローリングハッシュ（列）】
/*
* Dynamic_rolling_hash<STR>(STR s) : O(n)
*	列 s[0..n) で初期化する．
*	制約：STR は string，vector<T> など．ll 範囲の負数は扱えない．
*
* ull get(int l, int r) : O(log n)
*	部分文字列 s[l..r) のハッシュ値を返す（空なら 0）
*
* void set(int i, ull x) : O(log n)
*	s[i] = x とする．
*
* 利用：【フェニック木（アーベル群）】
*/
ull opdrh(ull x, ull y) {
	ull a = x + y, ah = a >> 61, al = a & ((1ULL << 61) - 1), res = ah + al;
	if (res >= ((1ULL << 61) - 1)) res -= ((1ULL << 61) - 1);
	return res;
}
ull odrh() { return 0ULL; }
ull invdrh(ull a) { return ((1ULL << 61) - 1) ^ a; }
template <class STR>
class Dynamic_rolling_hash {
	// 参考 : https://qiita.com/keymoon/items/11fac5627672a6d6a9f6

	static constexpr ull MASK30 = (1ULL << 30) - 1;
	static constexpr ull MASK31 = (1ULL << 31) - 1;
	static constexpr ull MOD = (1ULL << 61) - 1; // 法（素数）

	// a mod (2^61 - 1) を返す．
	inline ull get_mod(ull a) const {
		ull ah = a >> 61, al = a & MOD;
		ull res = ah + al;
		if (res >= MOD) res -= MOD;
		return res;
	}

	// x ≡ a b mod (2^61 - 1) なる x < 2^63 を返す（ただし a, b < 2^61）
	inline ull mul(ull a, ull b) const {
		ull ah = a >> 31, al = a & MASK31;
		ull bh = b >> 31, bl = b & MASK31;

		ull c = ah * bl + bh * al;
		ull ch = c >> 30, cl = c & MASK30;

		ull term1 = 2 * ah * bh;
		ull term2 = ch + (cl << 31);
		ull term3 = al * bl;

		return term1 + term2 + term3; // < 2^63
	}

	static constexpr ull BASE = 1234567891011; // 適当な基数
	static constexpr ull BASE_INV = 212042116942762790ULL;
	static constexpr ull SHIFT = 4295090752; // 適当なシフト

	// 列の長さ
	int n;

	// powB[i] : BASE^i
	vector<ull> powB, powB_inv;

	// v[i] : (s[i] + SHIFT) BASE^(-i)
	Fenwick_tree<ull, opdrh, odrh, invdrh> v;

public:
	// 列 s[0..n) で初期化する．
	Dynamic_rolling_hash(const STR& s) : n(sz(s)), powB(n + 1), powB_inv(n + 1) {
		// verify : https://atcoder.jp/contests/abc331/tasks/abc331_f

		powB[0] = powB_inv[0] = 1;
		rep(i, n) {
			powB[i + 1] = get_mod(mul(powB[i], BASE));
			powB_inv[i + 1] = get_mod(mul(powB_inv[i], BASE_INV));
		}

		vector<ull> ini(n);
		rep(i, n) ini[i] = get_mod(mul((ull)s[i] + SHIFT, powB_inv[i]));
		v = Fenwick_tree<ull, opdrh, odrh, invdrh>(ini);
	}
	Dynamic_rolling_hash() : n(0) {}

	// s[l..r) のハッシュ値の取得
	ull get(int l, int r) const {
		// verify : https://atcoder.jp/contests/abc331/tasks/abc331_f

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return get_mod(mul(v.sum(l, r), powB[r - 1]));
	}

	// s[i] = x とする．
	void set(int i, ull x) {
		// verify : https://atcoder.jp/contests/abc331/tasks/abc331_f

		Assert(0 <= i && i < n);

		v.set(i, get_mod(mul(x + SHIFT, powB_inv[i])));
	}
};


//【二次元ローリングハッシュ（格子）】
/*
* Rolling_hash_2D(vvT a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*
* ull get(int x1, int y1, int x2, int y2) : O(1)
*	部分長方形領域 [x1..x2)×[y1..y2) のハッシュ値を返す．
*/
template <class T>
class Rolling_hash_2D {
	static constexpr ull MASK30 = (1ULL << 30) - 1;
	static constexpr ull MASK31 = (1ULL << 31) - 1;
	static constexpr ull MOD = (1ULL << 61) - 1; // 法（素数）

	// a mod (2^61 - 1) を返す．
	inline ull get_mod(ull a) const {
		ull ah = a >> 61, al = a & MOD;
		ull res = ah + al;
		if (res >= MOD) res -= MOD;
		return res;
	}

	// x ≡ a b mod (2^61 - 1) なる x < 2^63 を返す（ただし a, b < 2^61）
	inline ull mul(ull a, ull b) const {
		ull ah = a >> 31, al = a & MASK31;
		ull bh = b >> 31, bl = b & MASK31;

		ull c = ah * bl + bh * al;
		ull ch = c >> 30, cl = c & MASK30;

		ull term1 = 2 * ah * bh;
		ull term2 = ch + (cl << 31);
		ull term3 = al * bl;

		return term1 + term2 + term3; // < 2^63
	}

	static constexpr ull BX = 1234567891011, BY = 3141592653589; // 適当な基数
	static constexpr ull S = 4295090752; // 適当なシフト

	// 格子の縦と横の長さ
	int h, w;

	// powBX[i] : BX^i, powBY[i] : BY^i
	vector<ull> powBX, powBY;

	// v[i][j] : a[0..i)[0..j) のハッシュ値
	//	v[i][j] = Σx∈[0..i) Σy∈[0..j) (a[i][j] + S) BX^(i-1-x) BY^(j-1-y)
	vector<vector<ull>> v;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Rolling_hash_2D(const vector<vector<T>>& a) : h(sz(a)), w(sz(a[0])),
		powBX(h + 1), powBY(w + 1), v(h + 1, vector<ull>(w + 1))
	{
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_C

		powBX[0] = 1;
		rep(i, h) powBX[i + 1] = get_mod(mul(powBX[i], BX));

		powBY[0] = 1;
		rep(j, w) powBY[j + 1] = get_mod(mul(powBY[j], BY));

		rep(i, h) rep(j, w) v[i + 1][j + 1] = get_mod(mul(v[i][j + 1], BX) + (ull)a[i][j] + S);
		rep(i, h) rep(j, w) v[i + 1][j + 1] = get_mod(mul(v[i + 1][j], BY) + v[i + 1][j + 1]);
	}
	Rolling_hash_2D() : h(0), w(0) {}

	// 部分長方形領域 [x1..x2)×[y1..y2) のハッシュ値を返す．
	ull get(int x1, int y1, int x2, int y2) const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_C

		chmax(x1, 0); chmax(y1, 0); chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return 0;

		// 右下を基点とするとここが重くなる．左上基点にして inv をもつべき？
		ull res = v[x2][y2];
		res += 4 * MOD - mul(v[x1][y2], powBX[x2 - x1]);
		res = get_mod(res);
		res += 4 * MOD - mul(v[x2][y1], powBY[y2 - y1]);
		res += mul(get_mod(mul(v[x1][y1], powBX[x2 - x1])), powBY[y2 - y1]);
		res = get_mod(res);

		return res;
	}
};


//【ローリングハッシュ（数値文字列，加減可能）】
/*
* Number_rolling_hash(string s, bool reversible = false) : O(n)
*	数値文字列 s[0..n) で初期化する．reversible = true にすると逆順のハッシュも計算可能になる．
*
* ull get(int l, int r) : O(1)
*	部分数値文字列 s[l..r) のハッシュ値を返す（空なら 0）
*
* ull get_rev(int l, int r) : O(1)
*	部分数値文字列 s[l..r) を反転した数値文字列のハッシュ値を返す（空なら 0）
*
* ull join(ull hs, ull ht, int len) : O(1)
*	ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
*
* ull add(ull hA, ull hB) : O(1)
*	ハッシュ値 hA, hB が表す数値の和のハッシュを返す．
*
* ull sub(ull hA, ull hB) : O(1)
*	ハッシュ値 hA, hB が表す数値の差（hA 側 - hB 側）のハッシュを返す．
*/
class Number_rolling_hash {
	static constexpr ull MASK30 = (1ULL << 30) - 1;
	static constexpr ull MASK31 = (1ULL << 31) - 1;
	static constexpr ull MOD = (1ULL << 61) - 1; // 法（素数）

	// a mod (2^61 - 1) を返す．
	inline ull get_mod(ull a) const {
		ull ah = a >> 61, al = a & MOD;
		ull res = ah + al;
		if (res >= MOD) res -= MOD;
		return res;
	}

	// x ≡ a b mod (2^61 - 1) なる x < 2^63 を返す（ただし a, b < 2^61）
	inline ull mul(ull a, ull b) const {
		ull ah = a >> 31, al = a & MASK31;
		ull bh = b >> 31, bl = b & MASK31;

		ull c = ah * bl + bh * al;
		ull ch = c >> 30, cl = c & MASK30;

		ull term1 = 2 * ah * bh;
		ull term2 = ch + (cl << 31);
		ull term3 = al * bl;

		return term1 + term2 + term3; // < 2^63
	}

	// 列の長さ
	int n;

	// pow10[i] : 10^i
	vector<ull> pow10;

	// v[i] : s[0..i) のハッシュ値 Σj∈[0..i) s[j] 10^(i-1-j)
	// v_rev[i] : s[n-i..n) を反転した文字列のハッシュ値
	vector<ull> v, v_rev;

public:
	// 数値文字列 s[0..n) で初期化する．
	Number_rolling_hash(const string& s, bool reversible = false) : n(sz(s)), pow10(n + 1), v(n + 1) {
		// verify : https://codeforces.com/contest/898/problem/F

		pow10[0] = 1;
		rep(i, n) pow10[i + 1] = get_mod(mul(pow10[i], 10ULL));

		rep(i, n) v[i + 1] = get_mod(mul(v[i], 10ULL) + (ull)(s[i] - '0'));

		if (reversible) {
			v_rev.resize(n + 1);
			rep(i, n) v_rev[i + 1] = get_mod(mul(v_rev[i], 10ULL) + (ull)(s[n - 1 - i] - '0'));
		}
	}
	Number_rolling_hash() : n(0) {}

	// s[l..r) のハッシュ値の取得
	ull get(int l, int r) const {
		// verify : https://codeforces.com/contest/898/problem/F

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return get_mod(v[r] + 4 * MOD - mul(v[l], pow10[r - l]));
	}

	// s[l..r) を反転した文字列のハッシュ値の取得
	ull get_rev(int l, int r) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;
		Assert(!v_rev.empty());

		// s[l, r) を反転した文字列は s_rev[n-r, n-l) に等しい．
		return get_mod(v_rev[n - l] + 4 * MOD - mul(v_rev[n - r], pow10[r - l]));
	}

	// ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
	ull join(ull hs, ull ht, int len) const {
		Assert(len <= n);
		return get_mod(ht + mul(hs, pow10[len]));
	}

	// ハッシュ値 hA, hB が表す数値の和のハッシュを返す．
	ull add(ull hA, ull hB) {
		// verify : https://codeforces.com/contest/898/problem/F

		return get_mod(hA + hB);
	}

	// ハッシュ値 hA, hB が表す数値の差のハッシュを返す．
	ull sub(ull hA, ull hB) {
		return get_mod(hA + MOD - hB);
	}
};


//【ローリングハッシュ（列，XOR 可能）】
/*
* 列 s[0..n) の連続部分列 s[l, r) のハッシュ値を計算する．
* ハッシュ値のまま列同士の XOR を計算することができる．
*
* Rolling_hash_XOR<STR>(STR s) : O(65536 + 216 n)
*	列 s[0..n) で初期化する．
*	STR は string，vector<T> など．
*
* ull get(int l, int r) : O(1)
*	部分文字列 s[l..r) のハッシュ値を返す（空なら 0）
*
* ull join(ull hs, ull ht, int len) : O(1)
*	ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
*
* ull xor_sum(ull hs, ull ht, int len) : O(1)
*	ハッシュ値 hs[ht] をもつ s[0..len)[ t[0..len) ] について s XOR t のハッシュ値を返す．
*
* 利用：【ニム積】
*/
template <class STR>
struct Rolling_hash_XOR {
	//【方法】
	// 通常のローリングハッシュでは mod p での (和, 積)-半環上で計算するが，
	// それに代えて (ニム和, ニム積)-半環上で計算を行えばいい（ニム和 = XOR）

	using vul = vector<ull>;

	static constexpr ull BASE = 1410923993237113318; // 適当な基数
	static constexpr ull SHIFT = 1504164860675582848; // 適当なシフト

	// 列の長さ
	int n;

	Nim_product NP;

	// powB[i] : BASE^i
	vul powB;

	// v[i] : s[0..i) のハッシュ値
	// v0[i] : 0 が i 個並んだ列のハッシュ値
	vul v, v0;

	// 列 s[0..n) で初期化する．
	Rolling_hash_XOR(const STR& s) : n(sz(s)), NP(), powB(n + 1), v(n + 1), v0(n + 1) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		powB[0] = 1;
		rep(i, n) powB[i + 1] = NP.prod(powB[i], BASE);

		rep(i, n) v[i + 1] = NP.prod(v[i], BASE) ^ (ull)s[i] ^ SHIFT;
		rep(i, n) v0[i + 1] = NP.prod(v0[i], BASE) ^ SHIFT;
	}
	Rolling_hash_XOR() : n(0) {}

	// s[l..r) のハッシュ値を返す．
	ull get(int l, int r) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return v[r] ^ NP.prod(v[l], powB[r - l]);
	}

	// ハッシュ値 hs をもつ s とハッシュ値 ht をもつ t[0..len) を連結した s+t のハッシュ値を返す．
	ull join(ull hs, ull ht, int len) {
		Assert(len <= n);
		return ht ^ NP.prod(hs, powB[len]);
	}

	// ハッシュ値 hs[ht] をもつ s[0..len)[ t[0..len) ] について s XOR t のハッシュ値を返す．
	ull xor_sum(ull hs, ull ht, int len) {
		// verify : https://atcoder.jp/contests/abc274/tasks/abc274_h

		return hs ^ ht ^ v0[len];
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
*	集合 S に対し要素 x の有無を反転する．
*
* ll get() : O(1)
*	現時点での集合 S のハッシュ値を返す．
*
* clear() : O(1)
*	集合 S を空にする．
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
	// S を空集合として初期化する．
	Zobrist_hash_set() : v(0) {
		// verify : https://www.codechef.com/problems/COOK82D

		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(-INFL, INFL);
	}

	// 集合 S に対し要素 x の有無を反転する．
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

	// 集合 S を空にする．
	void clear() {
		v = 0;
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
*	現時点での関数 f のハッシュ値を返す．
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


//【Zobrist Hash（[0..n) からの関数）】
/*
* Zobrist_hash_func(int n) : O(1)
*	f : [0..n) → Z を零関数として初期化する．
*
* set(int i, ll v) : O(1)
*	f(i) = v とする．
*
* add(int x, ll v) : O(1)
*	f(i) += v とする．
*
* ull get() : O(1)
*	現時点での関数 f のハッシュ値を返す．
*/
class Zobrist_hash_Nfunc {
	// 乱数生成器
	static inline mt19937_64 mt;
	static inline uniform_int_distribution<ull> rnd;

	// 各 i∈[0..n) に対するハッシュの割り当て
	static inline vector<ull> i_to_hash;

	static inline bool first_call = true;

	// ハッシュ値
	ull h;

	// 各 i∈[0..n) に対する v ∈ Z の割り当て
	vl i_to_v;

public:
	// コンストラクタ（零関数で初期化）
	Zobrist_hash_Nfunc(int n) : h(0) {
		// verify : https://atcoder.jp/contests/abc367/tasks/abc367_f

		if (first_call) {
			first_call = false;
			mt.seed((int)time(NULL));
			rnd = uniform_int_distribution<ull>(0ULL, ~0ULL);
		}

		int L = sz(i_to_hash);
		if (L < n) {
			i_to_hash.resize(n);
			repi(i, L, n - 1) i_to_hash[i] = rnd(mt);
		}

		i_to_v.resize(n);
	}

	// f(i) += v とする．
	void add(int i, ll v) {
		// verify : https://atcoder.jp/contests/abc367/tasks/abc367_f

		// ハッシュ値の更新
		h += i_to_hash[i] * (ull)v;

		// 関数の更新
		i_to_v[i] += v;
	}

	// f(i) = v とする．
	void set(int i, ll v) {
		add(i, v - i_to_v[i]);
		i_to_v[i] = v;
	}

	// 現時点での関数 f のハッシュ値を返す．
	ull get() {
		// verify : https://atcoder.jp/contests/abc367/tasks/abc367_f

		return h;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Zobrist_hash_Nfunc Z) {
		os << "hash: " << Z.h << endl;
		return os;
	}
#endif
};


//【ハッシュ（unordered 用）】
/*
* unordered_set[map] の第二[三] 引数に Hash を渡して使う．
*/
struct Hash {
	// 参考 : https://qiita.com/ganyariya/items/df35d253726269bda436
	// verify : https://yukicoder.me/problems/no/1648

	// pair<int, ll> の場合の例
	size_t operator()(const pair<int, ll>& p) const {
		auto hash1 = hash<int>{}(p.first);
		auto hash2 = hash<ll>{}(p.second);

		size_t seed = 0;
		seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};


