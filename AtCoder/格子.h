#pragma once
#include "header.h"
// ■■■■■ 格子上の問題 ■■■■■


//【最大正方形】O(h w)
/*
* a[0..h)[0..w) の able のマスのみを使って作られる，
* マス (i, j) を右下隅とする最大正方形の一辺の長さを len[i][j] に格納する．
*
*（二次元 DP）
*/
template <class T>
void largest_square(const vector<vector<T>>& a, vvi& len, T able = 1) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-131.html
	// verify : https://codeforces.com/problemset/problem/713/D

	int h = sz(a), w = sz(a[0]);

	// len[i][j] : (i, j) を右下端とする最大正方形の一辺の長さ
	len = vvi(h, vi(w));

	int res = 0;
	rep(i, h) {
		rep(j, w) {
			// (i, j) が使えない場合面積は 0 のまま
			if (a[i][j] != able) continue;

			// 上辺または左辺では明らかに一辺の長さは 1 が限界
			if (i == 0 || j == 0) {
				len[i][j] = 1;
				continue;
			}

			// 上，左，左上を見れば自身の値を計算できる．
			len[i][j] = min({ len[i - 1][j], len[i][j - 1], len[i - 1][j - 1] }) + 1;
		}
	}
}


//【ヒストグラム内最大長方形】O(n)
/*
* ヒストグラム hist[0..n) に包まれる長方形の面積の最大値を返す．
* また長方形が [l..r) * [0..h) であることを l, r, h に格納する．
*/
template <class T>
ll largest_rectangle_in_histogram(vector<T>& hist, int* l = nullptr, int* r = nullptr, ll* h = nullptr) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-132.html
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_3_C

	int n = sz(hist);

	ll res = 0; int res_l = -1, res_r = -1; ll res_h = -1;

	// 面積未確定の長方形を記憶しておくためのスタック
	// 要素 (l, h) は，左端位置が l，高さが h であることを表す．
	stack<pair<int, T>> st;

	// hist の末尾に番兵を仕込んでおき，左から順に見ていく．
	hist.push_back(0);
	rep(i, n + 1) {
		// 位置 i で高さ hist[i] を実現する長方形の左端位置
		int left = i;

		// スタックに記録されている長方形を順に見ていく．
		while (!st.empty()) {
			int l; T h;
			tie(l, h) = st.top();

			// 注目位置以下の高さをもつ長方形についてはまだ面積を増やせる．
			if (h <= hist[i]) break;

			// 注目位置より高い左端をもつ長方形はこれ以上伸ばせないので面積確定．
			if (chmax(res, (ll)h * (i - l))) {
				res_l = l; res_r = i; res_h = h;
			}
			left = l;
			st.pop();
		}

		// 面積未確定の長方形として高さ hist[i] をもつものを追加する．
		st.push({ left, hist[i] });
	}
	hist.pop_back();

	if (l != nullptr) *l = res_l;
	if (r != nullptr) *r = res_r;
	if (h != nullptr) *h = res_h;

	return res;
}


//【最大長方形】O(h w)
/*
* a[i][j] = 1[0] はマス (i, j) を使える[使えない]ことを意味するとき，
* a[0..h)[0..w) の使えるマスのみを使って作られる最大長方形の面積を返す．
*
* 利用：【ヒストグラム内最大長方形】
*/
ll largest_square(vvi& a) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-133.html
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_3_B

	int h = sz(a), w = sz(a[0]);

	// a[i][j] を i 行目より上の使えるマスからなるヒストグラムとする．
	repi(i, 1, h - 1) {
		rep(j, w) {
			if (a[i][j]) a[i][j] += a[i - 1][j];
		}
	}

	// それぞれのヒストグラム a[i] に対してヒストグラム内最大長方形を求める．
	ll res = 0;
	rep(i, h) chmax(res, largest_rectangle_in_histogram(a[i]));

	return res;
}


//【単純多角形判定】O(h w)
/*
* 盤面 c[0..h)[0..w) で，外部が o，内部がそれ以外で表された多角形が単純であるかを返す．
*/
bool simple_polygonQ(const vvc& c_, char o = '.') {
	int h = sz(c_) + 2;
	int w = sz(c_[0]) + 2;

	// 外周に空マスを追加しておく．
	vvc c(h, vc(w, o));
	rep(i, h - 2) {
		rep(j, w - 2) {
			c[i + 1][j + 1] = c_[i][j];
		}
	}

	// 探索済みかどうか
	vvb seen(h, vb(w));

	// 多角形内[外] のマスの 1 つを得る
	pii in = { -1, -1 }, out = { -1, -1 };
	rep(i, h) {
		rep(j, w) {
			if (c[i][j] == o) out = { i, j };
			else in = { i, j };
		}
	}

	// 多角形内[外] のマスそれぞれ 1 つずつをキューに登録する．
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
			if (seen[nx][ny]) continue;
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
			if (!seen[i][j]) return false;
		}
	}

	return true;
}


//【キング配置問題】O(h w 1.6^w)
/*
* h * w の穴あき盤に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
*
* hall[i][j] : マス (i, j) に穴が空いているなら true，さもなくば false
* 戻り値 : キングを配置する場合の数
* 
*（盤上 bitDP）
*/
mint king_problem(vvb& hall) {
	int h = sz(hall), w = sz(hall[0]);

	// 直前の m + 1 マスだけ切り出すマスク
	const ll mask_full = (1 << (w + 1)) - 1;

	// マスの位置が左端，中央，右端それぞれの場合に応じて
	// キングが配置されていてはいけない場所だけを切り出すマスク
	//（w = 1 がコーナーケースになるので注意）
	const ll mask_l = (w > 1 ? (3LL << (w - 2)) : 1);	// 0110..00
	const ll mask_m = (7LL << (w - 2)) + 1;				// 1110..01
	const ll mask_r = (3LL << (w - 1)) + 1;				// 1100..01

	// dp[i * w + j][pat] : 以下の条件を満たす配置の数
	//		マス (i, j) の直前の w + 1 マスの配置パターンが pat
	vector<unordered_map<ll, mint>> dp(w * h + 1);
	dp[0][0LL] = 1;

	rep(i, h) {
		rep(j, w) {
			repe(p, dp[i * w + j]) {
				ll pat; mint cnt;
				tie(pat, cnt) = p;

				// (i, j) にコマを置かない場合
				dp[i * w + j + 1][(2 * pat) & mask_full] += cnt;

				// 左端，中央，右端に応じて使うマスクを切り替える．
				ll mask;
				if (j == 0) mask = mask_l;
				else if (j == w - 1) mask = mask_r;
				else mask = mask_m;

				// マスクをかけた位置にコマがあるか，または
				// マス (i, j) が配置不能マスであれば (i, j) にコマを置けない．
				if ((pat & mask) || hall[i][j]) continue;

				// (i, j) にコマを置く場合
				dp[i * w + j + 1][(2 * pat + 1) & mask_full] += cnt;
			}
		}
	}

	mint res = 0;
	repe(v, dp[h * w]) res += v.second;

	return res;
}


//【欠損修復】O(h w)
/*
* 二次元配列 c の欠損領域を周囲の値の平均で塗りつぶす．
* 欠損値は defect で表されているとする．
*/
template <class T> void defect_repair(vector<vector<T>>& c, T defect = -1) {
	int h = sz(c), w = sz(c[0]);

	vvi seen(h, vi(w));

	rep(i, h) {
		rep(j, w) {
			if (c[i][j] > 0 || seen[i][j]) continue;

			// 周辺マスや欠損マスの位置を記録する連想配列
			set<pii> around, hole;

			// 欠損領域を幅優先探索するためのキュー
			queue<pii> q;

			// 欠損領域が確定するまで
			q.push({ i, j });
			while (!q.empty()) {
				int x, y;
				tie(x, y) = q.front(); q.pop();
				seen[x][y] = true;
				hole.insert({ x, y });

				// 欠損マスの 4 近傍について
				rep(i, 4) {
					int nx = x + dx4[i];
					int ny = y + dy4[i];

					if (nx < 0 || h <= nx || ny < 0 || w <= ny || seen[nx][ny]) {
						continue;
					}

					if (c[nx][ny] > 0) around.insert({ nx, ny });
					else q.push({ nx, ny });
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


//【長方形の扱い】
/*
* (x1, y1) を左上，(x2, y2) を右下にもつ長方形が存在する
* ⇔ 第 x1 行と第 x2 行の両方に辺 (y1, y2) が存在する．
* 
* verify : https://atcoder.jp/contests/arc019/tasks/arc019_4
*/

