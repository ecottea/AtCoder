#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数え上げ（格子） ■■■■■


//【チェス盤距離 d 以内ジャンプ最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h×w 格子について，右下方向へのチェス盤距離 d 以下の
* ジャンプを繰り返すとき，c[0][0] から c[i][j] までの経路の数を seq[i][j] に格納する．
*
*（二次元いもす法で高速化した格子 DP）
*/
void count_chebyshev_path(const vvc& c, int d, vvm& seq, char WALL = '#') {
	int h = sz(c), w = sz(c[0]);

	seq = vvm(h, vm(w));
	seq[0][0] = 1;

	// 配る DP
	rep(i, h) rep(j, w) {
		// v : dp[i][j] の正しい値
		mint v = seq[i][j];

		// (i, j) からの寄与の種を蒔く．
		if (c[i][j] != WALL) {
			if (true)							seq[i][j] += v;					// 注目地点
			if (j + d + 1 < w)					seq[i][j + d + 1] -= v;			// 右
			if (i + d + 1 < h)					seq[i + d + 1][j] -= v;			// 下
			if (i + d + 1 < h && j + d + 1 < w)	seq[i + d + 1][j + d + 1] += v;	// 右下
		}

		// v2 : 以降のマスに seq[i][j] が与える影響
		mint v2 = (i == 0 && j == 0 ? 1 : seq[i][j]);

		// 累積和をとる（種蒔きの範囲を d = 0 に縮小して符号反転する）
		if (true)					seq[i][j] -= v2;			// 注目地点
		if (j + 1 < w)				seq[i][j + 1] += v2;		// 右
		if (i + 1 < h)				seq[i + 1][j] += v2;		// 下
		if (i + 1 < h && j + 1 < w)	seq[i + 1][j + 1] -= v2;	// 右下

		// seq[i][j] を正しい値に戻しておく．
		seq[i][j] = v;
	}
}


//【マンハッタン距離 d 以内ジャンプ最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h×w 格子について，右下方向へのマンハッタン距離 d 以下の
* ジャンプを繰り返すとき，c[0][0] から c[i][j] までの経路の数を seq[i][j] に格納する．
*
*（二次元いもす法で高速化した格子 DP）
*/
void count_manhattan_path(const vvc& c_, int d, vvm& seq, char WALL = '#') {
	// verify : https://yukicoder.me/problems/no/2003

	int h = sz(c_), w = sz(c_[0]);

	// 左右に番兵を付けておく．
	// 番兵が O(h w) 個に収まるよう，必要ならば h <= w になるよう転置する．
	vvc c; bool swap_flag = false;
	if (h <= w) {
		c = vvc(h, vc(h + w + h, WALL));
		rep(i, h) rep(j, w) c[i][h + j] = c_[i][j];
	}
	else {
		swap(h, w);
		c = vvc(h, vc(h + w + h, WALL));
		rep(i, h) rep(j, w) c[i][h + j] = c_[j][i];
		swap_flag = true;
	}
	w += 2 * h;

	vvm dp(h, vm(w));
	dp[0][h] = 1;

	// 配る DP
	rep(i, h) rep(j, w) {
		// v : dp[i][j] の正しい値
		mint v = dp[i][j];

		// (i, j) からの寄与の種を蒔く．
		if (c[i][j] != WALL) {
			// 注目地点
			if (true)							dp[i][j] += v;
			if (i + 1 < h && j - 1 >= 0)		dp[i + 1][j - 1] -= v;

			// 右
			if (i + 1 < h && j + d + 1 < w)		dp[i + 1][j + d + 1] += v;
			if (j + d + 1 < w)					dp[i][j + d + 1] -= v;

			// 下
			if (i + d + 2 < h && j - 1 >= 0)	dp[i + d + 2][j - 1] += v;
			if (i + d + 2 < h)					dp[i + d + 2][j] -= v;
		}

		// v2 : 以降のマスに dp[i][j] が与える影響
		mint v2 = (i == 0 && j == h ? 1 : dp[i][j]);

		// 累積和をとる（種蒔きの範囲を d = 0 に縮小して符号反転する）
		// 注目地点
		if (true)						dp[i][j] -= v2;
		if (i + 1 < h && j - 1 >= 0)	dp[i + 1][j - 1] += v2;

		// 右
		if (i + 1 < h && j + 1 < w)		dp[i + 1][j + 1] -= v2;
		if (j + 1 < w)					dp[i][j + 1] += v2;

		// 下
		if (i + 2 < h && j - 1 >= 0)	dp[i + 2][j - 1] -= v2;
		if (i + 2 < h)					dp[i + 2][j] += v2;

		// dp[i][j] を正しい値に戻しておく．
		dp[i][j] = v;
	}

	// 結果を格納する．
	w -= 2 * h;
	if (swap_flag) {
		swap(h, w);
		seq = vvm(h, vm(w));
		rep(i, h) rep(j, w) seq[i][j] = dp[j][w + i];
	}
	else {
		seq = vvm(h, vm(w));
		rep(i, h) rep(j, w) seq[i][j] = dp[i][h + j];
	}
}


//【互いの効きに入らないキング配置の数え上げ】O(1.618^w h w + 2^w)
/*
* c[0..h)[0..w) 上に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
* ただし c[i][j] = ng であるようなマス (i, j) にはキングを配置できない．
*
*（格子上スライド bit DP）
*/
template <class T>
mint count_nonattacking_king_placement(vector<vector<T>>& c, T ng = '#') {
	// 参考 : https://twitter.com/e869120/status/1386138990361726978
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_w

	int h = sz(c), w = sz(c[0]);

	// 直前の w + 1 マスだけ切り出すマスク
	const int mask = (1 << (w + 1)) - 1;

	// マスの位置が左端，中央，右端それぞれの場合に応じて
	// キングが配置されていてはいけない場所だけを切り出すマスク
	//（w = 1 がコーナーケースになるので注意）
	const int fb_l = (w > 1 ? (3 << (w - 2)) : 1);	// 0110..00
	const int fb_m = (7 << (w - 2)) + 1;			// 1110..01
	const int fb_r = (3 << (w - 1)) + 1;			// 1100..01

	// pats : 注目マスの直前の w+1 マスの配置パターンとしてありうるものの昇順列
	//（O(2^w) かかっているが，ちゃんとバックトラッキングで書けば O(1.6^w) にできる）
	vi pats;
	repb(pat, w + 1) if (popcount(pat & (pat << 1)) <= 1) pats.push_back(pat);
	int n = sz(pats);

	// nxt0[k] : k 番目のパターンについて，コマを置かない場合に何番目のパターンに遷移するか
	// nxt1[k] : k 番目のパターンについて，コマを置く場合に何番目のパターンに遷移するか
	vi nxt0(n), nxt1(n);
	rep(k, n) {
		nxt0[k] = lbpos(pats, (pats[k] << 1) & mask);
		nxt1[k] = lbpos(pats, ((pats[k] << 1) + 1) & mask);
	}

	// dp_(i,j)[k] : マス (i, j) の直前の w+1 マスの配置パターンが k 番目である配置の数
	vm dp(n);
	dp[0] = 1;

	//【注意】
	// DP テーブルを unordered_map で持ちたいところだが，
	// w = 24 くらいまでいくと unordered_map の要素数が 190000 個ほどになり，
	// もはや O(1) でアクセスできるとはみなせないほどに計算量が悪化する．

	// 左上から始めて右方向（突き当たったら下方向）に走査していく．
	rep(i, h) rep(j, w) {
		vm ndp(n);

		rep(k, n) {
			// (i, j) にコマを置かない場合
			ndp[nxt0[k]] += dp[k];

			// マス (i, j) が配置不能マスであれば (i, j) にコマを置けない．
			if (c[i][j] == ng) continue;

			// (i, j) にコマを置く場合
			// 中央，左端，右端に応じて使うマスクを切り替える．
			int fb = fb_m;
			if (j == 0) fb = fb_l;
			else if (j == w - 1) fb = fb_r;

			// マスクをかけた位置にコマがなければ (i, j) にコマを置ける．
			if (!(pats[k] & fb)) ndp[nxt1[k]] += dp[k];
		}

		dp = move(ndp);
	}

	return accumulate(all(dp), mint(0));
}


//【指定バウンディングボックスをもつ点配置の数え上げ】O(1)
/*
* バウンディングボックスが [0..h)×[0..w) になるような n 個の格子点の配置の数を返す．
*
* 制約：fm は (h * w)! まで計算可能であること
*/
mint count_points_in_BB(int n, int h, int w, Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc003/tasks/abc003_4

	mint res = 0;

	// 無条件の場合
	res += fm.bin(h * w, n);

	// 少なくとも 1 個の辺が条件を満たしていない場合
	res -= 2 * fm.bin((h - 1) * w, n); // 上または下
	res -= 2 * fm.bin(h * (w - 1), n); // 左または右

	// 少なくとも 2 個の辺が条件を満たしていない場合
	res += fm.bin((h - 2) * w, n); // 上下
	res += fm.bin(h * (w - 2), n); // 左右
	res += 4 * fm.bin((h - 1) * (w - 1), n); //（上または下）かつ（左または右）

	// 少なくとも 3 個の辺が条件を満たしていない場合
	res -= 2 * fm.bin((h - 2) * (w - 1), n); // 上下かつ（左または右）
	res -= 2 * fm.bin((h - 1) * (w - 2), n); //（上または下）かつ左右

	// 全ての辺が条件を満たしていない場合
	if (h >= 2 && w >= 2) res += fm.bin((h - 2) * (w - 2), n); // 上下かつ左右

	return res;
}


