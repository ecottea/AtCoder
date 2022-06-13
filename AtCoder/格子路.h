#pragma once
#include "header.h"
#include "二項係数.h"
#include "座標圧縮.h"
// ■■■■■ 格子路の問題 ■■■■■


//【最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h * w 格子について，
* c[0][0] から c[h-1][w-1] までの最短路の数を返す．
*
*（始点からの格子 DP）
*/
mint count_lattice_path(const vvc& c) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_h

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
* c[0][0] から c[h-1][w-1] までの最短路の数を返す．
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
* c[0][0] から c[h-1][w-1] までの最短路の数を返す．
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


//【コスト最大経路（スパースコスト指定）】O(n log n)
/*
* n 個の点 (x[i], y[i]) に非負コスト c[i] が与えられており，その他の点のコストは 0 である．
* (-inf, -inf) から (inf, inf) までの最短路のうち，コストの和が最大のもののコストを返す．
*
*（平面走査 DP）
*/
ll op_mcp(ll a, ll b) { return max(a, b); }
ll e_mcp() { return 0; }
ll maximize_cost_path(const vl& x_, const vl& y_, const vl& c) {
	// verify : https://yukicoder.me/problems/no/1826

	int n = sz(x_);

	// 座標圧縮を行う．
	vi x, y;
	int h = coordinate_compression(x_, x);
	int w = coordinate_compression(y_, y);

	// x 座標降順，次いで y 座標降順にソートする．
	vector<tuple<int, int, ll>> xyc(n);
	rep(i, n) xyc[i] = { x[i], y[i], c[i] };
	sort(all(xyc), greater< tuple<int, int, ll> >());

	// dp_i[j] : 点 i までで，y 座標が j である点からの最大コスト 
	segtree<ll, op_mcp, e_mcp> dp(w);

	rep(i, n) {
		int x, y; ll c;
		tie(x, y, c) = xyc[i];

		ll c2 = dp.prod(y, w);
		dp.set(y, c + c2);
	}

	return dp.all_prod();
}


//【最短経路数（スパース禁止点指定）】O(n^2)
/*
* h * w の格子の (0, 0) から (h-1, w-1) までの最短路のうち，
* n 個の禁止点 fb[i] = {r[i], c[i]} を 1 つも通らないものの個数を返す．
*
* 利用：【階乗など（mint利用）】
*/
mint dummy_path_lemma(int h, int w, const vector<pii>& fb) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_y

	int n = sz(fb);

	Factorial_mint fm(h + w);

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


//【自由経路数】O(1)（前計算 O(n)）
/*
* 原点から (x, y) まで n 回の移動で到達する経路の数を返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_free_lattice_path(int n, int x, int y, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_g

	//【方法】
	// x, y >= 0 とする．ローラン多項式の言葉に直すと，求める場合の数は
	//		[s^x t^y] (s + 1/s + t + 1/t)^n
	// である．以下明らかに 0 と分かる場合は無視する．
	//
	// 指数の底は因数分解できるので，これは
	//		[s^x t^y] ( (s + t)^n (1 + 1/st)^n )
	// と書き直せる．
	// 
	// 第一因子からは s, t の次数の和が n の項しか作れないので，
	// 第二因子から作るべき項の次数の和は x + y - n である．
	// それが第 k 項だとすると，次数についての方程式
	//		0 * (n - k) + (-2) * k = x + y - n
	// を解いて
	//		k = (n - x - y) / 2
	// と分かり，その係数は二項定理より binomial(n, (n-x-y)/2) である．
	//
	// 第二因子からは s, t の次数の差が 0 の項しか作れないので，
	// 第一因子から作るべき項の次数の差は x - y である．
	// それが第 k 項だとすると，次数についての方程式
	//		(n - k) - k = x - y
	// を解いて
	//		k = (n - x + y) / 2
	// と分かり，その係数は二項定理より binomial(n, (n-x+y)/2) である．
	//
	// 以上より，求める場合の数は
	//		binomial(n, (n-x-y)/2) binomial(n, (n-x+y)/2)
	// である．

	//【別の方法】
	// 45°回転すれば，移動可能な箇所が x, y について独立（長方形状）になり，
	// 座標ごとに独立に問題をといて積をとるだけでよくなる．

	x = abs(x); y = abs(y);

	// 明らかに 0 通りの場合
	if (x + y > n || (n - x - y) % 2 == 1) return 0;

	return fm.binomial(n, (n - x - y) / 2) * fm.binomial(n, (n - x + y) / 2);
}


//【迷路】O(h w)
/*
* 壁が wall で表された h * w の迷路 c について，スタート s = (sx, sy) から
* 各マス c[i][j] への最短経路長を dist[i][j] に格納する．（到達不能なら -1）
*
*（格子上の幅優先探索）
*/
void solve_maze(const vvc& c, const pii& s, vvi& dist, const char wall = '#') {
	// verify : https://algo-method.com/tasks/424

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
			int nx = x + DX[k];
			int ny = y + DY[k];

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
			int nx = x + DX[k];
			int ny = y + DY[k];

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


