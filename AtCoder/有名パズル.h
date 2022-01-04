#pragma once
#include "header.h"
#include "ビット行列.h"
// ■■■■■ 有名パズル ■■■■■


//【ライツアウト】O(h^3 w^3)
/*
* ライツアウトの盤面 s[0..h)[0..w) に対し，全マスを消灯する押し方を push に格納する．
* s[i][j] = one はマス (i, j) が点灯状態であることを表す．
* sol[i][j] = false[true] はマス (i, j) を押さない[押す] ことを表す．
* 解が存在しないなら false を返す．
*
*（呼び出すとき lights_out<N> としないと gcc でエラーになるので注意．）
*
* 制約：N は h * w + 1 以上の定数
*
* 利用：【ビット行列】，【連立一次方程式】
*/
template <int N> bool lights_out(const vvc& s, vvb* sol = nullptr, char one = '1') {
	int h = sz(s), w = sz(s[0]);

	Bit_matrix<N> mat(h * w);
	rep(x, h) {
		rep(y, w) {
			// 押したマスは点灯状態が反転する．
			mat[x * w + y][x * w + y] = 1;

			// 押したマスの 4 近傍も点灯状態が反転する．
			rep(k, 4) {
				int nx = x + dx4[k];
				int ny = y + dy4[k];

				if (nx < 0 || h <= nx || ny < 0 || w <= ny) {
					continue;
				}

				mat[x * w + y][nx * w + ny] = 1;
			}

			// 目標とする点灯パターン
			mat[x * w + y][h * w] = (s[x][y] == one);
		}
	}

	// 連立方程式を解いて押し方を見つける．
	bitset<N> res;
	bool able = solve_eq<N>(mat, h * w, &res);

	// 連立方程式に解が無いなら不可能．
	if (!able) return false;

	// 連立方程式の解から押し方を復元する．
	if (sol != nullptr) {
		*sol = vvb(h, vb(w));
		rep(x, h) {
			rep(y, w) {
				(*sol)[x][y] = res[x * w + y];
			}
		}
	}

	return true;
}


