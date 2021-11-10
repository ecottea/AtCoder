#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "構造(幾何).h"
#include "ヒストグラム.h"
#include "二項係数.h"
// ■■■■■ 格子上の問題 ■■■■■


//【グリッド → グラフ】O(h w)
/*
* h 行 w 列のグリッドから nb_type 近傍を連結としたグラフ g を構築する．
* 壁マスは wall，空きマスはその他とする．
* i 行目の j 列目にあるマス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
void grid_to_graph(const vector<vector<T>>& c, Graph& g, T wall = '#', int nb_type = 4) {
	int h = sz(c);
	int w = sz(c[0]);

	const vi& dx = (nb_type == 4 ? dx4 : dx8);
	const vi& dy = (nb_type == 4 ? dy4 : dy8);

	g = Graph(h * w);
	rep(i, h) {
		rep(j, w) {
			// 空きマスでなかったら辺は追加しない．
			if (c[i][j] == wall) {
				continue;
			}

			// 今考えている近傍それぞれについて
			rep(k, nb_type) {
				// 近傍のマスの座標
				int ni = i + dx[k];
				int nj = j + dy[k];

				// 範囲外だったり空きマスでなかったら辺は追加しない．
				if (ni < 0 || ni >= h || nj < 0 || nj >= w || c[ni][nj] == wall) {
					continue;
				}

				// 近傍に空きマスがあったら辺を追加する．
				g[i * w + j].push_back(ni * w + nj);
			}
		}
	}
}


//【迷路】O(h w)
/*
* 壁が wall で表された h * w の迷路 c について，スタート s = (sx, sy) から
* 各マス c[i][j] への最短経路長を dist[i][j] に格納する．（到達不能なら -1）
*
*（幅優先探索）
*/
void solve_maze(const vvc& c, const pii& s, vvi& dist, const char wall = '#') {
	int h = sz(c);
	int w = sz(c[0]);

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
	int h = sz(c);
	int w = sz(c[0]);
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


//【単純多角形判定】O(h w)
/*
* h × w の盤面で，外部が '.'，内部がそれ以外で表された多角形が単純であるかを返す．
*/
bool simple_polygonQ(const vvc& c_) {
	int h = sz(c_) + 2;
	int w = sz(c_[0]) + 2;

	// 外周に空マスを追加しておく．
	vvc c(h, vc(w, '.'));
	rep(i, h - 2) {
		rep(j, w - 2) {
			c[i + 1][j + 1] = c_[i][j];
		}
	}

	// 探索済みかどうか
	vvb seen(h, vb(w));

	// 多角形内[外]のマスの 1 つ
	pii in = { -1, -1 }, out = { -1, -1 };
	rep(i, h) {
		rep(j, w) {
			if (c[i][j] == '.') {
				out = { i, j };
			}
			else {
				in = { i, j };
			}
		}
	}

	// 多角形内[外]のマスそれぞれをキューに登録する．
	queue<pii> q;
	if (in != make_pair(-1, -1)) {
		q.push(in);
		seen[in.first][in.second] = true;
	}
	q.push(out);
	seen[out.first][out.second] = true;

	// 幅優先探索を行う．
	while (!q.empty()) {
		int x, y;
		tie(x, y) = q.front();
		q.pop();

		// マス (x, y) の 4 近傍を調べる．
		rep(k, 4) {
			// (nx, ny) : (x, y) の近傍の座標
			int nx = x + dx4[k];
			int ny = y + dy4[k];

			// 盤面の外に出たり，異種のマスへ移動することはない．
			if (nx < 0 || nx >= h || ny < 0 || ny >= w || c[nx][ny] != c[x][y]) {
				continue;
			}

			// 探索したことを記録しておく．
			if (seen[nx][ny]) {
				continue;
			}
			seen[nx][ny] = true;

			// 後で探索するためキューに追加する．
			q.push({ nx, ny });
		}
	}

	// 多角形内のマスで未探索のマスがあるなら多角形が非連結または自己交差あり．
	// 多角形外のマスで未探索のマスがあるなら多角形に穴が空いているまたは自己交差あり．
	// どちらにせよ単純多角形ではないので false を返す．
	rep(i, h) {
		rep(j, w) {
			if (!seen[i][j]) {
				return false;
			}
		}
	}

	return true;
}


//【最大正方形】O(h w)
/*
* a[i][j] = 1[0] はマス (i, j) を使える[使えない]ことを意味するとき，
* 使えるマスのみを使って作られる最大正方形の面積を返す．
*
*（二次元 DP）
*/
int largest_square(const vvi& a) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-131.html

	int h = sz(a), w = sz(a[0]);

	// dp[i + 1][j + 1] : 
	//  (i, j) を右下端とする最大正方形の一辺の長さ
	vvi dp(h + 1, vi(w + 1));

	int res = 0;
	rep(i, h) {
		rep(j, w) {
			// (i, j) が使えない場合面積は 0 のまま
			if (!a[i][j]) {
				continue;
			}

			// 上，左，左上を見れば自身の値を計算できる．
			dp[i + 1][j + 1] = min({ dp[i + 1][j], dp[i][j + 1], dp[i][j] }) + 1;
			chmax(res, dp[i + 1][j + 1]);
		}
	}

	// 2 乗して面積にして返す．
	return res * res;
}


//【最大長方形】O(h w)
/*
* a[i][j] = 1[0] はマス (i, j) を使える[使えない]ことを意味するとき，
* 使えるマスのみを使って作られる最大長方形の面積を返す．
*
* 利用：【ヒストグラム内最大長方形】
*/
ll largest_square(vvi& a) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-133.html

	int h = sz(a), w = sz(a[0]);

	// a[i][j] を i 行目より上の使えるマスからなるヒストグラムとする．
	repi(i, 1, h - 1) {
		rep(j, w) {
			if (a[i][j]) {
				a[i][j] += a[i - 1][j];
			}
		}
	}

	// それぞれのヒストグラム a[i] に対してヒストグラム内最大長方形を求める．
	ll res = 0;
	rep(i, h) {
		chmax(res, largest_rectangle_in_histogram(a[i]));
	}

	return res;
}


//【キング配置問題】O(h w 1.6^w)
/*
* h * w の穴あき盤に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
*
* hall[i][j] : マス (i, j) に穴が空いているなら true，さもなくば false
* 戻り値 : キングを配置する場合の数
* 
*（盤上 DP）
*/
mint king_problem(vvb& hall) {
	int h = sz(hall);
	int w = sz(hall[0]);

	// 直前の m + 1 マスだけ切り出すマスク
	const ll mask_full = (1LL << (w + 1)) - 1;

	// マスの位置が左端，中央，右端それぞれの場合に応じて
	// キングが配置されていてはいけない場所だけを切り出すマスク
	//（w = 1 がコーナーケースになるので注意）
	const ll mask_l = (w > 1 ? (3LL << (w - 2)) : 1);	// 0110..00
	const ll mask_m = (7LL << (w - 2)) + 1;				// 1110..01
	const ll mask_r = (3LL << (w - 1)) + 1;				// 1100..01

	// dp[i * w + j][pat] : 以下の条件を満たす配置の数
	//	マス (i, j) の直前の w + 1 マスの配置パターンが pat
	vector<map<ll, mint>> dp(w * h + 1);
	dp[0][0LL] = 1;

	rep(i, h) {
		rep(j, w) {
			repe(p, dp[i * w + j]) {
				ll pat;
				mint cnt;
				tie(pat, cnt) = p;

				// (i, j) にコマを置かない場合
				dp[i * w + j + 1][(2 * pat) & mask_full] += cnt;

				// 左端，中央，右端に応じて使うマスクを切り替える．
				ll mask;
				if (j == 0) {
					mask = mask_l;
				}
				else if (j == w - 1) {
					mask = mask_r;
				}
				else {
					mask = mask_m;
				}

				// マスクをかけた位置にコマがあるか，
				// マス (i, j) が配置不能マスであれば (i, j) にコマを置けない．
				if ((pat & mask) || hall[i][j]) {
					continue;
				}

				// (i, j) にコマを置く場合
				dp[i * w + j + 1][(2 * pat + 1) & mask_full] += cnt;
			}
		}
	}

	mint res = 0;
	repe(v, dp[h * w]) {
		res += v.second;
	}

	return res;
}


//【欠損修復】O(h w)
/*
* 二次元配列 c の欠損領域を周囲の値の平均で塗りつぶす．
* 欠損値は defect で表されているとする．
*/
template <class T>
void defect_repair(vector<vector<T>>& c, T defect) {
	int h = sz(c);
	int w = sz(c[0]);

	vector<vector<int>> seen(h, vector<int>(w));

	rep(i, h) {
		rep(j, w) {
			if (c[i][j] > 0 || seen[i][j]) {
				continue;
			}

			// 周辺マスや欠損マスの位置を記録する連想配列
			set<pii> around, hole;

			// 欠損領域を幅優先探索するためのキュー
			queue<pii> q;

			// 欠損領域が確定するまで
			q.push({ i, j });
			while (!q.empty()) {
				int x, y;
				tie(x, y) = q.front();
				q.pop();
				seen[x][y] = true;
				hole.insert({ x, y });

				// 欠損マスの 4 近傍について
				rep(i, 4) {
					int nx = x + dx4[i];
					int ny = y + dy4[i];

					if (nx < 0 || h <= nx || ny < 0 || w <= ny || seen[nx][ny]) {
						continue;
					}

					if (c[nx][ny] > 0) {
						around.insert({ nx, ny });
					}
					else {
						q.push({ nx, ny });
					}
				}
			}

			// 欠損領域を塗りつぶす色の計算
			int new_col = 0;
			repe(p, around) {
				int x, y;
				tie(x, y) = p;

				new_col += c[x][y];
			}
			new_col /= sz(around);

			// 欠損領域の塗りつぶし
			repe(p, hole) {
				int x, y;
				tie(x, y) = p;

				c[x][y] = new_col;
			}
		}
	}

	/* テスト用
	10 8
	13 18 19 17 15 22 -1 28
	16 15 98 19 15 27 -1 26
	12 93 -1 95 14 29 -1 25
	12 93 92 12 12 15 28 12
	18 99 96 99 99 19 18 15
	16 16 15 15 17 18 17 18
	12 13 16 13 16 16 16 16
	14 17 14 15 -1 -1 16 17
	14 15 18 18 -1 -1 18 18
	13 13 13 13 13 13 13 13
	*/
}


//【最短経路数（禁止点あり）】O(n^2)
/*
* h × w の格子路の (0, 0) から (h-1, w-1) までの最短路のうち，
* n 個の禁止点 fb[i] = {r[i], c[i]} を 1 つも通らないものの個数を返す．
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint dummy_path_lemma(int h, int w, const vector<pii>& fb) {
	int n = sz(fb);

	factorial_mint fm(h + w);

	// s, t : 対応する始点と終点の列
	// 番号の小さい方へ戻るパスは存在してはいけない．
	vector<pii> s = { {0,0} }, t = { {h - 1, w - 1} };
	rep(i, n) {
		s.push_back(fb[i]);
	}
	sort(next(s.begin()), s.end());
	repi(i, 1, n) {
		t.push_back(s[i]);
	}

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


