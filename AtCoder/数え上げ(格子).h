#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数え上げ（格子） ■■■■■


//【互いの効きに入らないキング配置の数え上げ】O(h w 1.6^w + 2^w)
/*
* c[0..h)[0..w) 上に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
* ただし c[i][j] = ng であるようなマス (i, j) にはキングを配置できない．
*
*（格子上スライド bitDP）
*/
template <class T>
mint nonattacking_king_placement(vector<vector<T>>& c, T ng = '#') {
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


//【トロミノのタイリングの数え上げ】O(h 4^w)
/*
* h×w の盤面に トロミノを敷き詰める方法が何通りあるかを返す．
*/
ll count_Tromino_tiling(int h, int w) {
	// マスの数が 3 の倍数でなければ明らかに不可能．
	if (h * w % 3 != 0) return 0;

	// 盤は縦長だとする．
	if (h < w) swap(h, w);

	// 敷き詰めに使うピースのリスト
	// 各ピースは辞書順最小位置を {0, 0} としたマスの集合で表す．
	vector<vector<pii>> pieces{
		{ {0, 0}, {0, 1}, {0, 2} },
		{ {0, 0}, {0, 1}, {1, 0} },
		{ {0, 0}, {0, 1}, {1, 1} },
		{ {0, 0}, {1, -1}, {1, 0} },
		{ {0, 0}, {1, 0}, {1, 1} },
		{ {0, 0}, {1, 0}, {2, 0} }
	};

	// mat[set] : 盤の上 2 行分の敷き詰めが set のとき，そこから遷移できる次の 2 行分のパターンのリスト
	vvl mat(1LL << (2 * w));

	// set : 盤の上 2 行分の敷き詰めパターン
	repb(set, 2 * w) {
		// board[i][j] : 上 3 行の盤の位置 (i, j) にタイルが置かれているか
		vvb board(3, vb(w));
		rep(x, 2) rep(y, w) if (get(set, x * w + y)) board[x][y] = 1;

		// board[i][j] を返す（盤外なら true を返す）
		auto get_board = [&](int i, int j) {
			if (j < 0 || j >= w) return true;
			return (bool)board[i][j];
		};

		// (0, j): 注目位置
		function<void(int)> dfs = [&](int j) {
			// 1 行目が埋まったら次の 2 行の形状を記録
			if (j == w) {
				int nset = 0;
				rep(y, w) if (board[1][y]) nset |= 1 << y;
				rep(y, w) if (board[2][y]) nset |= 1 << (w + y);
				mat[set].push_back(nset);
				return;
			}

			// すでにタイルが敷かれていたら 1 つ右のマスへ
			if (get_board(0, j)) {
				dfs(j + 1);
				return;
			}

			// 各ピースを置くことができるかをチェックする．
			repe(piece, pieces) {
				bool ok = true;
				for (auto& [di, dj] : piece) {
					if (get_board(0 + di, j + dj)) {
						ok = false;
						break;
					}
				}
				if (!ok) continue;

				for (auto& [di, dj] : piece) board[0 + di][j + dj] = true;
				dfs(j + 1);
				for (auto& [di, dj] : piece) board[0 + di][j + dj] = false;
			}
		};
		dfs(0);
	}

	// dp_i[set] : i 行目までみて，盤の上 2 行のパターンが set である敷き詰め方の数
	vl dp(1LL << (2 * w));
	dp[0] = 1;

	rep(hoge, h) {
		vl ndp(1LL << (2 * w));
		repb(set, 2 * w) repe(nset, mat[set]) ndp[nset] += dp[set];
		dp = move(ndp);
	}

	return dp[0];
}

