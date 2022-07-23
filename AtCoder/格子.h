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


//【ボンバーマン】O(h w)
/*
* 壁が wall で表された盤面 c[0..h)[0..w) において，位置 c[i][j] に最も近い
* 上[下] の壁の位置の x 座標を u[i][j][ d[i][j] ] に，
* 左[右] の壁の位置の y 座標を l[i][j][ r[i][j] ] にそれぞれ格納する．
*/
template <class T> void bomberman(const vector<vector<T>>& c, vvi& u, vvi& d, vvi& l, vvi& r, T wall = '#') {
	// verify : https://atcoder.jp/contests/hhkb2020/tasks/hhkb2020_e

	int h = sz(c), w = sz(c[0]);
	u = vvi(h, vi(w)); d = vvi(h, vi(w)); l = vvi(h, vi(w)); r = vvi(h, vi(w));

	// 上方向に最も近い壁の x 座標を求める．
	rep(j, w) {
		int pos = -1;
		rep(i, h) {
			if (c[i][j] == wall) pos = i;
			u[i][j] = pos;
		}
	}

	// 下方向に最も近い壁の x 座標を求める．
	rep(j, w) {
		int pos = h;
		repir(i, h - 1, 0) {
			if (c[i][j] == wall) pos = i;
			d[i][j] = pos;
		}
	}

	// 左方向に最も近い壁の y 座標を求める．
	rep(i, h) {
		int pos = -1;
		rep(j, w) {
			if (c[i][j] == wall) pos = j;
			l[i][j] = pos;
		}
	}

	// 右方向に最も近い壁の y 座標を求める．
	rep(i, h) {
		int pos = w;
		repir(j, w - 1, 0) {
			if (c[i][j] == wall) pos = j;
			r[i][j] = pos;
		}
	}
}


//【単純多角形判定】O(h w)
/*
* 盤面 c[0..h)[0..w) で，外部が o，内部がそれ以外で表された多角形が単純であるかを返す．
*/
bool simple_polygonQ(const vvc& c_, char o = '.') {
	// verify : https://atcoder.jp/contests/abc219/tasks/abc219_e

	int h = sz(c_) + 2, w = sz(c_[0]) + 2;

	// 外周に空マスを追加しておく．
	vvc c(h, vc(w, o));
	rep(i, h - 2) rep(j, w - 2) c[i + 1][j + 1] = c_[i][j];

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
		tie(x, y) = q.front(); q.pop();

		// マス (x, y) の 4 近傍を調べる．
		rep(k, 4) {
			// (nx, ny) : (x, y) の近傍の座標
			int nx = x + DX[k];
			int ny = y + DY[k];

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
	rep(i, h) rep(j, w) if (!seen[i][j]) return false;

	return true;
}


//【互いの効きに入らないキング配置の数え上げ】O(h w 1.6^w + 2^w)
/*
* c[0..h)[0..w) 上に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
* ただし c[i][j] = ng であるようなマス (i, j) にはキングを配置できない．
*
*（格子上スライド bitDP）
*/
template <class T> mint nonattacking_king_placement(vector<vector<T>>& c, T ng = '#') {
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
	// もはや O(1) でアクセスできるとはみなせなくなり計算量が悪化する．

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
					int nx = x + DX[i];
					int ny = y + DY[i];

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

