#pragma once
#include "header.h"
// ■■■■■ 文字列 ■■■■■


//【ローリングハッシュ】
/*
* 文字列 s の部分文字列 s[l, r) のハッシュ値を計算する．
* ハッシュ値は Σi=0..r-l (s[l + i] + SHIFT) * BASE ^ i (mod MOD)
*
* rolling_hash(s) : O(|s|)
*	文字列を s として初期化する．
*
* get(l, r) : O(1)
*	部分文字列 s[l, r) のハッシュ値を返す．
*/
template <class STR, int MOD, int BASE, int SHIFT>
struct rolling_hash_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;

	const mint B = BASE; // 適当な基数
	const mint invB = B.inv(); // 基数の逆数
	const mint S = SHIFT; // 適当なシフト

	// 文字列とその長さ
	STR s;
	int n;

	// v[i] : s[0, i) のハッシュ値
	vm v;

	// pow_invB[i] : B^(-i)
	vm pow_invB;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_sub(const STR& s_) : s(s_), n(sz(s_)), v(n + 1), pow_invB(n) {
		// ハッシュ値計算用の B の累乗
		pow_invB[0] = 1;
		rep(i, n - 1) {
			pow_invB[i + 1] = pow_invB[i] * invB;
		}

		// s[0, i) のハッシュ値の計算
		mint powB = 1;
		rep(i, n) {
			v[i + 1] = v[i] + (s[i] + S) * powB;
			powB *= B;
		}
	}

	// s[l, r) のハッシュ値の取得
	int get(int l, int r) {
		return ((v[r] - v[l]) * pow_invB[l]).val();
	}
};
template <class STR> // STR は例えば string, vector<int>
struct rolling_hash {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_sub<STR, 1000000007, 100007, 17> rh1;
	rolling_hash_sub<STR, 998244353, 99991, 91> rh2;

	// コンストラクタ（文字列 s で初期化）
	rolling_hash(const STR& s) : rh1(s), rh2(s) {}

	// s[l, r) のハッシュ値の取得
	ll get(int l, int r) {
		return (ll(rh1.get(l, r)) << 32) + ll(rh2.get(l, r));
	}
};


//【二次元ローリングハッシュ】
/*
* 二次元配列 a の部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を計算する．
* ハッシュ値は次の式により計算する：
*   Σi=0..x2-x1 j=0..y2-y1 (a[x1 + i][y1 + j] + SHIFT) * BASE_X ^ i * BASE_Y ^ j (mod MOD)
*
* rolling_hash(a) : O(|w| |h|)
*	二次元配列を a として初期化する．
*
* get(x1, y1, x2, y2) : O(1)
*	部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
*/
template <class T, int MOD, int BASE_X, int BASE_Y, int SHIFT>
struct rolling_hash_2d_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;
	using vvm = vector<vm>;

	const mint BX = BASE_X; // 適当な基数
	const mint invBX = BX.inv(); // 基数の逆数
	const mint BY = BASE_Y;
	const mint invBY = BY.inv();
	const mint S = SHIFT; // 適当なシフト

	// 二次元配列とその大きさ
	vector<vector<T>> a;
	int h, w;

	// v[i][j] : 長方形領域 [0, i) * [0, j) のハッシュ値
	vvm v;

	// ハッシュ値計算用の B の累乗
	vm pow_BX, pow_BY, pow_invBX, pow_invBY;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_2d_sub(vector<vector<T>>& a_) :
		a(a_), h(sz(a)), w(sz(a[0])), v(h + 1, vector<mint>(w + 1)),
		pow_BX(h), pow_BY(w), pow_invBX(h), pow_invBY(w) {

		// ハッシュ値計算用の B の累乗の前計算
		pow_BX[0] = pow_BY[0] = pow_invBX[0] = pow_invBY[0] = 1;
		rep(i, h - 1) {
			pow_BX[i + 1] = pow_BX[i] * BX;
			pow_invBX[i + 1] = pow_invBX[i] * invBX;
		}
		rep(j, w - 1) {
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

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	int get(int x1, int y1, int x2, int y2) {
		return ((v[x2][y2] - v[x1][y2] - v[x2][y1] + v[x1][y1])
			* pow_invBX[x1] * pow_invBY[y1]).val();
	}
};
template <class T> // T は例えば int, ll, char
struct rolling_hash_2d {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_2d_sub<T, 1000000007, 100007, 26627, 17> rh1;
	rolling_hash_2d_sub<T, 998244353, 99991, 54401, 91> rh2;

	// コンストラクタ（二次元配列 a で初期化）
	rolling_hash_2d(vector<vector<T>>& a) : rh1(a), rh2(a) {}

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	ll get(int x1, int y1, int x2, int y2) {
		return (ll(rh1.get(x1, y1, x2, y2)) << 32) + ll(rh2.get(x1, y1, x2, y2));
	}
};


//【クヌース・モリス・プラット法】O(|s| + |w|)
/*
* s の部分文字列として w が含まれているかどうか調べ，
* 最初に見つかった場所の先頭位置を返す．（見つからなかったら -1 を返す．）
*/
template <class STR> // STR は例えば string, vector<int>
int knuth_morris_pratt(const STR& s, const STR& w) {
	int n = sz(s);
	int m = sz(w);

	// 部分マッチテーブル tbl の作成
	// s[i] から照合を開始して s[i + j] != w[j] となった場合，
	// 次に照合を開始すべき位置が s[i + j - tbl[j]] であるように構築する．
	vi tbl(m);
	tbl[0] = -1; tbl[1] = 0;
	int i = 2; // いま tbl[i] を計算中であることを表す．
	int j = 0; // いま w[j] まで見ていることを表す．
	while (i < m) {
		// サブ文字列が w の先頭と一致し続けている場合
		if (w[i - 1] == w[j]) {
			// 一致した長さの分だけバックトラッキングしなければならない．
			tbl[i] = j + 1;

			// それぞれ 1 文字先を見に行く．
			i++; j++;
		}
		// サブ文字列と w の先頭との一致が終わった場合？
		else if (j > 0) {
			// 次のサブ文字列を走査するため j を戻す．？
			j = tbl[j];
		}
		// ？
		else {
			tbl[i] = 0;
			i++;
		}
	}

	// 連続部分列を探す．
	i = 0; // いま s[i] から始まる連続部分列を見ていることを表す．
	j = 0; // いま w[j] まで見ていることを表す．
	while (i + j < n) {
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致した場合．
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見．
			if (j == m) {
				return i;
			}
		}
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致しなかった場合．
		else {
			// 部分マッチテーブルに従い i, j を再設定する．
			// i = i + j としたいが tbl[j] だけのバックトラッキングが入る．
			i = i + j - tbl[j];

			// その代わり w との照合を tbl[j] だけ進んだところから始められる．
			if (j > 0) {
				j = tbl[j];
			}
		}
	}

	return -1;
}


