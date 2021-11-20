#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 格子路の問題 ■■■■■


//【最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h * w 格子について，
* c[0][0] から c[h-1][w-1] までの最短格子路の数を返す．
*
*（始点からの格子 DP）
*/
mint count_lattice_path(const vvc& c) {
	// verify : https://atcoder.jp/contests/joi2007yo/tasks/joi2007yo_f

	int h = sz(c), w = sz(c[0]);

	// dp[i][j] : c[0][0] から c[i][j] までの経路数
	vvm dp(h, vm(w));

	// 貰う DP
	rep(i, h) {
		rep(j, w) {
			// 通過できないマスの場合
			if (c[i][j] == '#') continue;

			// 始点の場合
			if (i == 0 && j == 0) {
				dp[i][j] = 1;
				continue;
			}

			// 上から来れる場合
			if (i > 0) {
				dp[i][j] += dp[i - 1][j];
			}

			// 左から来れる場合
			if (j > 0) {
				dp[i][j] += dp[i][j - 1];
			}
		}
	}

	return dp[h - 1][w - 1];
}


//【最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h * w 格子について，
* c[0][0] から c[h-1][w-1] までの最短格子路の数を返す．
*
*（終点からの格子 DP）
*/
mint count_lattice_path_rev(const vvc& c) {
	// verify : https://atcoder.jp/contests/joi2007yo/tasks/joi2007yo_f

	int h = sz(c), w = sz(c[0]);

	// dp[i][j] : c[i][j] から c[h-1][w-1] までの経路数
	vvm dp(h, vm(w));

	// 貰う DP
	repir(i, h - 1, 0) {
		repir(j, w - 1, 0) {
			// 通過できないマスの場合
			if (c[i][j] == '#') continue;

			// 終点の場合
			if (i == h - 1 && j == w - 1) {
				dp[i][j] = 1;
				continue;
			}

			// 下に進める場合
			if (i < h - 1) {
				dp[i][j] += dp[i + 1][j];
			}

			// 右に進める場合
			if (j < w - 1) {
				dp[i][j] += dp[i][j + 1];
			}
		}
	}

	return dp[0][0];
}


//【最短経路数（連続で曲がれない）】O(h w)
/*
* h * w 格子について，二連続で曲がることを禁止した場合の
* c[0][0] から c[h-1][w-1] までの最短格子路の数を返す．
*
*（始点からの格子状態 DP）
*/
mint count_lattice_path_no_continuous_turns(int h, int w) {
	// verify : https://atcoder.jp/contests/joi2010yo/tasks/joi2010yo_e

	// 便宜上，曲がった直後は停止も禁止して直進させるものとする．
	// dp[i][j][k] : c[0][0] から c[i][j] に k=0:上[k=1:左] から来た経路の数
	vvvm dp(h, vvm(w, vm(2)));

	// 配る DP
	rep(i, h) {
		rep(j, w) {
			// 始点の場合
			if (i == 0 && j == 0) {
				dp[i + 1][j][0] = 1;
				dp[i][j + 1][1] = 1;
				continue;
			}

			// 下に行ける場合
			if (i < h - 1) {
				dp[i + 1][j][0] += dp[i][j][0];

				// 直前に左から来たなら下に 2 つ進まなければいけない
				if (i < h - 2) {
					dp[i + 2][j][0] += dp[i][j][1];
				}
			}

			// 右に行ける場合
			if (j < w - 1) {
				dp[i][j + 1][1] += dp[i][j][1];

				// 直前に上から来たなら右に 2 つ進まなければいけない
				if (j < w - 2) {
					dp[i][j + 2][1] += dp[i][j][0];
				}
			}
		}
	}
	dumpel(dp);

	// 曲がった直後の停止が禁止なので，1 マス手前も結果に加える．
	mint res;
	res += dp[h - 1][w - 1][0];
	res += dp[h - 1][w - 1][1];
	res += dp[h - 1][w - 2][0];
	res += dp[h - 2][w - 1][1];

	return res;
}


//【最短経路数（禁止点指定）】O(n^2)
/*
* h × w の格子路の (0, 0) から (h-1, w-1) までの最短路のうち，
* n 個の禁止点 fb[i] = {r[i], c[i]} を 1 つも通らないものの個数を返す．
*
* 利用：【階乗と二項係数（mint利用）】
* 
*（Dummy Path Lemma）
*/
mint dummy_path_lemma(int h, int w, const vector<pii>& fb) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_grid

	int n = sz(fb);

	factorial_mint fm(h + w);

	// s, t : 対応する始点と終点の列
	// 番号の小さい方へ戻るパスは存在してはいけない．
	vector<pii> s = { {0, 0} }, t = { {h - 1, w - 1} };
	rep(i, n) s.push_back(fb[i]);
	sort(next(s.begin()), s.end());
	repi(i, 1, n) t.push_back(s[i]);
	
	// DPL 用の行列を作成する．
	vvm dpl(n + 1, vm(n + 1));
	repi(i, 0, n) {
		repi(j, 0, n) {
			int h = t[j].first - s[i].first;
			int w = t[j].second - s[i].second;
			dpl[i][j] = fm.binomial(h + w, h);
		}
	}

	// 列基本変形で第 1 列の 2 行目以降を消去する．
	repir(j, n, 1) {
		rep(i, j) {
			dpl[i][0] -= dpl[j][0] * dpl[i][j];
		}
	}

	// DPL 行列の行列式，すなわち (0,0) 成分が答え．
	return dpl[0][0];
}


//【迷路】O(h w)
/*
* 壁が wall で表された h * w の迷路 c について，スタート s = (sx, sy) から
* 各マス c[i][j] への最短経路長を dist[i][j] に格納する．（到達不能なら -1）
*
*（幅優先探索）
*/
void solve_maze(const vvc& c, const pii& s, vvi& dist, const char wall = '#') {
	int h = sz(c), w = sz(c[0]);

	dist = vvi(h, vi(w, -1));
	dist[s.first][s.second] = 0;

	// q : 未探索のマスを記録しておくキュー
	queue<pii> q;
	q.push(s);

	while (!q.empty()) {
		int x, y;
		tie(x, y) = q.front();
		q.pop();

		// マス (x, y) の 4 近傍を調べる．
		rep(k, 4) {
			// (nx, ny) : (x, y) の近傍の座標
			int nx = x + dx4[k];
			int ny = y + dy4[k];

			// 範囲外または壁マスなら何もしない．
			if (nx < 0 || nx >= h || ny < 0 || ny >= w || c[nx][ny] == wall) {
				continue;
			}

			// 既に最短経路長が確定済みなら何もしない．
			if (dist[nx][ny] != -1) {
				continue;
			}

			// 最短経路長の確定
			dist[nx][ny] = dist[x][y] + 1;

			q.push({ nx, ny });
		}
	}
}


//【迷路（複数始点）】O(h w)
/*
* 壁が wall で表された h * w の迷路 c について，スタートの集合 s[i] = (sx, sy) から
* 各マス c[i][j] への最短経路長の最小値を dist[i][j] に格納する．（到達不能なら -1）
*
*（幅優先探索）
*/
void solve_maze(const vvc& c, const vector<pii>& s, vvi& dist, const char wall = '#') {
	int h = sz(c), w = sz(c[0]);
	dist = vvi(h, vi(w, -1));

	// q : 未探索のマスを記録しておくキュー
	queue<pii> q;
	repe(p, s) {
		q.push(p);
		dist[p.first][p.second] = 0;
	}

	while (!q.empty()) {
		int x, y;
		tie(x, y) = q.front();
		q.pop();

		// マス (x, y) の 4 近傍を調べる．
		rep(k, 4) {
			// (nx, ny) : (x, y) の近傍の座標
			int nx = x + dx4[k];
			int ny = y + dy4[k];

			// 範囲外または壁マスなら何もしない．
			if (nx < 0 || nx >= h || ny < 0 || ny >= w || c[nx][ny] == wall) {
				continue;
			}

			// 既に最短経路長が確定済みなら何もしない．
			if (dist[nx][ny] != -1) {
				continue;
			}

			// 最短経路長の確定
			dist[nx][ny] = dist[x][y] + 1;

			q.push({ nx, ny });
		}
	}
}


