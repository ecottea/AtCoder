#pragma once
#include "header.h"
// ■■■■■ 文字列に対する汎用性のある前処理 ■■■■■


//【ランレングス符号】O(n)
/*
* a[0..n) をランレングス符号化し，結果を格納したリスト cls を返す．
* cls[i] = {c, l} は前から i 番目の連が l 個の文字 c からなることを表す．
*/
template <class STR, class T = remove_reference_t<decltype(declval<STR>()[0])>>
vector<pair<T, int>> run_length_encoding(const STR& a) {
	// verify : https://atcoder.jp/contests/abc381/tasks/abc381_c

	int n = sz(a);
	vector<pair<T, int>> cls;

	if (n == 0) return cls;

	cls.emplace_back(a[0], 1);

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			cls.back().second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			cls.emplace_back(c, 1);
		}
	}

	return cls;
}


//【ランレングス符号（区切り位置）】O(n)
/*
* a[0..n) をランレングス符号化する．
* 前から k 番目の連が a[x[k]..x[k+1]) で，その文字が c[k] であったことを格納する．
*/
template <class STR, class T = remove_reference_t<decltype(declval<STR>()[0])>>
void run_length_encoding(const STR& a, vector<T>& c, vi& x) {
	// verify : https://atcoder.jp/contests/abc401/tasks/abc401_d

	int n = sz(a);
	c.clear(); x = vi{ 0 };

	if (n == 0) return;

	c.emplace_back(a[0]);
	repi(i, 1, n - 1) {
		// 最後に読んだ文字と異なる文字の場合
		if (a[i] != c.back()) {
			x.emplace_back(i);
			c.emplace_back(a[i]);
		}
	}
	x.emplace_back(n);
}


//【文字列上ジャンプ】
/*
* Jump_on_string(STR s, int C = 26, T a = 'a') : O(n C)
*	s[0..n) で初期化する．文字種は a から始まる連続する C 種類とする．
*
* int next(int l, T c, int k = 0) : O(1)
*	s[l..n) 内の文字 c の左から k 番目（0-indexed）の位置を返す（なければ n を返す）
*
* int prev(int r, T c, int k = 0) : O(1)
*	s[0..r) 内の文字 c の右から k 番目（0-indexed）の位置を返す（なければ -1 を返す）
*
* int count(int l, int r, T c) : O(1)
*	s[l..r) 内の文字 c の個数を返す．
*/
template <class STR, class T = remove_reference_t<decltype(declval<STR>()[0])>>
class Jump_on_string {
	int n;

	int C; int a;

	// pos[c] : s[0..n) 内の文字 c がある位置の昇順リスト
	vvi pos;

	// acc[c][i] : 文字 c が s[0..i) に含まれている個数
	vvi acc;

	//【備考】
	// acc に対してさらに c 方向に累積和をとっておけば，
	// 自身より大きい[小さい] 文字に関するなんやかんやも O(1) で処理できるようになる．

public:
	// S[0..Mn) = s[0..n)×M で初期化する．文字種は a から始まる連続する C 種類とする．
	Jump_on_string(const STR& s, int C = 26, T a = 'a') : n(sz(s)), C(C), a(a), pos(C), acc(C, vi(n + 1)) {
		// verify : https://atcoder.jp/contests/abc381/tasks/abc381_e

		rep(i, n) {
			int c = s[i] - a;
			pos[c].emplace_back(i);
			rep(c2, C) acc[c2][i + 1] += acc[c2][i] + (c2 == c);
		}
	}
	Jump_on_string() : n(0), C(0), a(0) {}

	// s[l..n) 内の文字 c の左から k 番目（0-indexed）の位置を返す（なければ n を返す）
	int next(int l, T c, int k = 0) {
		// verify : https://atcoder.jp/contests/abc381/tasks/abc381_e

		c -= a;
		Assert(0 <= c); Assert(c < C);

		if (l >= n) return n;
		chmax(l, 0);

		// K : s[0..n) 内の文字 c の個数
		int K = acc[c][n];

		// s[0..n) 内の左から k 番目とする．
		k += acc[c][l];
		if (k >= K) return n;

		return pos[c][k];
	}

	// s[0..r) 内の文字 c の右から k 番目（0-indexed）の位置を返す（なければ -1 を返す）
	int prev(int r, T c, int k = 0) {
		// verify : https://atcoder.jp/contests/abc381/tasks/abc381_e

		c -= a;
		Assert(0 <= c); Assert(c < C);

		if (r < 0) return -1;
		chmin(r, n);

		// s[0..n) 内の左から k 番目とする．
		k = acc[c][r] - 1 - k;
		if (k < 0) return -1;

		return pos[c][k];
	}

	// s[l..r) 内の文字 c の個数を返す．
	int count(int l, int r, T c) {
		// verify : https://atcoder.jp/contests/abc381/tasks/abc381_e

		c -= a;
		Assert(0 <= c); Assert(c < C);

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return acc[c][r] - acc[c][l];
	}
};


//【異なる文字の次の位置】O(n)
/*
* s[0..n) で，j > i かつ s[j] != s[i] なる最小の j（なければ n）を nxt[i] に格納し nxt を返す．
*/
template <class STR>
vi next_different_position(const STR& s) {
	int n = sz(s);
	vi nxt(n);

	auto c = s[n - 1]; // 走査中の文字
	int pos = n; // 走査中の文字以外が最後に現れた位置

	// 後ろから走査していく
	repir(i, n - 1, 0) {
		if (s[i] == c) nxt[i] = pos;
		else {
			nxt[i] = pos = i + 1;
			c = s[i];
		}
	}

	return nxt;
}


//【異なる文字の前の位置】O(n)
/*
* s[0..n) で，j < i かつ s[j] != s[i] なる最大の j（なければ -1）を prv[i] に格納し prv を返す．
*/
template <class STR>
vi prev_different_position(const STR& s) {
	int n = sz(s);
	vi prv(n);

	auto c = s[0]; // 走査中の文字
	int pos = -1; // 走査中の文字以外が最初に現れた位置

	// 前から走査していく
	rep(i, n) {
		if (s[i] == c) prv[i] = pos;
		else {
			prv[i] = pos = i - 1;
			c = s[i];
		}
	}

	return prv;
}


//【数列の圧縮埋め込み】
/*
* string encode(vi a) : O(n)
*	a を 10 進表現と比べて 5/9 倍程度に圧縮した文字列 s を返す．
*	制約：0 ≦ a[i] < 2^30
* 
* vi decode(string s) : O(n)
*	圧縮した文字列 s から元の数列 a を復元して返す．
*/
namespace Compress_embed {
	string encode(const vi& a) {
		// verify : https://atcoder.jp/contests/arc162/tasks/arc162_f
				
		int n = sz(a);

		string s;

		// 5 桁の 64 進数にし，0 から順に ['0'..'O'] 凵 ['_'..'~'] を割り当てる．
		rep(i, n) {
			rep(j, 5) {
				if (a[i] & (0b100000 << (6 * j))) {
					s += '_' + ((a[i] >> (6 * j)) & 0b011111);
				}
				else {
					s += '0' + ((a[i] >> (6 * j)) & 0b011111);
				}
			}
		}

		return s;
	}

	vi decode(const string& s) {
		// verify : https://atcoder.jp/contests/arc162/tasks/arc162_f
		
		int n = sz(s) / 5;

		vi a(n);

		rep(i, n) {
			rep(j, 5) {
				if (s[5 * i + j] >= '_') {
					a[i] |= ((s[5 * i + j] - '_') | 0b100000) << (6 * j);
				}
				else {
					a[i] |= (s[5 * i + j] - '0') << (6 * j);
				}
			}
		}

		return a;
	}
};


