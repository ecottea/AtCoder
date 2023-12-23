#pragma once
#include "header.h"
// ■■■■■ ビット演算 ■■■■■


//【ビット演算の別表現】
/*
* XOR : 体 F2 上での加算[減算]
* NOT : 体 F2 上での 1 からの減算[インクリメント, デクリメント]
* OR  : 全順序集合 {0, 1} 上での max
* AND : 全順序集合 {0, 1} 上での min
*/


//【XOR と 和 の関係】
/*
* 非負整数 a, b について，a XOR b は 2 進法で繰り上がりを捨てて計算した a + b に等しい：
*	a + b = (a XOR b) + 2(a AND b)
* 
* verify : https://atcoder.jp/contests/abc172/tasks/abc172_f
*/


//【XOR と 差 の関係】
/*
* 非負整数 a, b について，a XOR b は 2 進法で繰り下がりを戻して計算した a - b に等しい：
*	a - b = (a XOR b) - 2(~a AND b)
* 
* verify : https://atcoder.jp/contests/toyota2023spring-final-open/tasks/toyota2023spring_final_c
*/


//【x との XOR の和】
/*
* XOR_sum(vT a) : O(n log max(a))
*	a[0..n) で初期化する．
*
* ll sum(T x) : O(log max(a))
*	Σi∈[0..n) a[i] XOR x の値を返す．
*/
template <class T>
struct XOR_sum {
	// verify : https://atcoder.jp/contests/arc135/tasks/arc135_c

	int d;
	vvi cnt;

	// a[0..n) で初期化する．
	XOR_sum(const vector<T>& a) {
		T a_max = *max_element(all(a));
		if (a_max > 0) d = msb((ll)a_max) + 1;
		else d = 0;

		cnt = vvi(d, vi(2));

		repe(v, a) {
			rep(j, d) {
				cnt[j][(v >> j) & 1]++;
			}
		}
	}

	// Σi=[0..n) a[i] XOR x の値を返す．
	ll sum(T x) {
		// ビット毎に独立に寄与を計算し和をとればよい．
		ll res = 0;
		rep(j, d) {
			// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
			res += (ll)cnt[j][1 - ((x >> j) & 1)] << j;
		}
		return res;
	}
};


//【連続自然数の XOR】O(1)
/*
* XOR[0..n) を返す．
*/
template <class T>
T acc_XOR(T n) {
	// verify : https://atcoder.jp/contests/abc121/tasks/abc121_d

	//【方法】
	// s[i] := XOR[0..i) とおく．
	//		(2 k) XOR (2 k + 1) = 1
	// に注意すると，s[i] が周期 4 をもつことが分かり，
	//		s[4 k]     = 0
	//		s[4 k + 1] = 4 k
	//		s[4 k + 2] = 1
	//		s[4 k + 3] = 4 k + 3
	// と簡単に表すことができる．

	if (n <= 0) return 0;

	T res = -1;
	if (n % 4 == 0) res = 0;
	else if (n % 4 == 1) res = n - 1;
	else if (n % 4 == 2) res = 1;
	else if (n % 4 == 3) res = n;

	return res;
}


//【XOR 区間の分割】O(log max(r, c))
/*
* 集合 {x∈[0..∞) | l ≦ x XOR c < r} を O(log max(r, c)) 個の半開区間に分割し，そのリストを返す．
*/
template <class T>
vector<pair<T, T>> XOR_interval_division(T l, T r, T c) {
	// verify : https://yukicoder.me/problems/no/2505

	chmax(l, T(0));
	if (l >= r) return vector<pair<T, T>>();

	int n = max(msb(r), msb(c));

	vector<pair<T, T>> lrs; T val_l = 0, val_r = 0; bool eq = true;
	repir(i, n, 0) {
		T lb = get(l, i), rb = get(r, i), cb = get(c, i);

		if (eq) {
			if (lb != rb) eq = false;
		}
		else {
			if (!lb) {
				T val_l2 = val_l + ((1 ^ cb) << i);
				lrs.emplace_back(val_l2, val_l2 + (T(1) << i));
			}
			if (rb) {
				T val_r2 = val_r + ((0 ^ cb) << i);
				lrs.emplace_back(val_r2, val_r2 + (T(1) << i));
			}
		}
		val_l += (lb ^ cb) << i;
		val_r += (rb ^ cb) << i;
	}
	lrs.emplace_back(val_l, val_l + 1);

	return lrs;
}


//【多重集合の一斉 XOR】
/*
* 多重集合 a[0..n) について，操作
*		i∈[0..n) を選び，a[0..n) - {a[i]} ^= a[i]
* が任意の回数行えるとき，操作の回数を 1 回以下に制限しても遷移可能な全体は変わらない．
* 
* verify : https://atcoder.jp/contests/agc052/tasks/agc052_b
*/


//【OR, AND と和の関係】
/*
* a + b = (a OR b) + (a AND b)
*
* 証明：ビット毎に a + b = min(a, b) + max(a, b) を適用する．
*
* verify : https://atcoder.jp/contests/abc238/tasks/abc238_d
*/


//【連続自然数の部分集合の OR】
/*
* [1..n] の部分集合の OR で作れる数の集合は ub=2^(msb(n)+1) として [1..ub) である．
*
* verify : https://atcoder.jp/contests/agc015/tasks/agc015_d
*/


//【popcount の別表現】
/*
* popcount(x) = x - Σi∈[1..∞) floor(x / 2^i)
* 
* verify : https://atcoder.jp/contests/abc283/tasks/abc283_h
*/


//【繰り上がりビット】O(log max(x, y))
/*
* 非負整数 x, y の 2 進法での加算で繰り上がりの起こるビットだけを 1 にした数を返す．
*/
template <class T>
T get_carry(T x, T y) {
	// verify : https://atcoder.jp/contests/arc156/tasks/arc156_d

	T res = 0;

	while (y != 0) {
		T ny = x & y;
		res |= ny;
		x ^= y;
		y = ny << 1;
	}

	return res;
}


//【最上位ビット】
/*
* n の最上位ビットの位置を返す．
*/
inline int msb(__int128 n) { return (n >> 64) != 0 ? (127 - __builtin_clzll((ll)(n >> 64))) : n != 0 ? (63 - __builtin_clzll((ll)(n))) : -1; }


