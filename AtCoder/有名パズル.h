#pragma once
#include "header.h"
#include "ビット行列.h"
#include "最短路.h"
// ■■■■■ 有名パズル ■■■■■


//【8 クイーン問題】O(n! n)
/*
* 既に (x0[i], y0[i]) に置かれたクイーンを含む，n * n の格子上で互いに効きに入らないような
* クイーンの配置例において，i 行目のクイーンが y[i] 列目にあることを格納する．
* 適切な配置が存在しないなら false を返す．
*
*（順列全探索）
*/
bool queen8_problem(int n, const vi& x0, const vi& y0, vi& y) {
	// verify : https://atcoder.jp/contests/arc001/tasks/arc001_3

	int k = sz(x0);
	y.clear();

	unordered_map<int, int> x_to_y;
	rep(i, k) {
		// 同じ行に 2 つクイーンがある場合は明らかに不可能．
		if (x_to_y.count(x0[i])) return false;

		x_to_y[x0[i]] = y0[i];
	}

	vi p(n);
	iota(all(p), 0);

	// p[i] : i 行目のクイーンを何列目に置くか
	// 順列全探索を行うので，各行各列に 1 つずつであることまでは保証されている．
	repp(p) {
		vb rd(2 * n - 1), ld(2 * n - 1);

		// 初期配置と矛盾がないかのチェック
		repe(xy, x_to_y) {
			int x, y;
			tie(x, y) = xy;

			if (p[x] != y) goto NEXT_LOOP;
		}

		rep(i, n) {
			// 左下がりの対角線方向のチェック
			if (ld[i + p[i]]) goto NEXT_LOOP;
			ld[i + p[i]] = true;

			// 右下がりの対角線方向のチェック
			if (rd[i - p[i] + n - 1]) goto NEXT_LOOP;
			rd[i - p[i] + n - 1] = true;
		}

		y = p;
		return true;

	NEXT_LOOP:;
	}

	return false;
}


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


//【15 パズル】
/*
* h * w スライドパズルの盤面 a[0..h)[0..w) を解くのにかかる最短手数を返す．
* 0 以上 h * w - 1 未満の数はパネルを，数 h * w - 1 は空きマスを表す．
* max_step 手以内に解けない場合は -1 を返す．
*
*（A* 探索）
*/
int solve_15puzzle(const vvi& a_, int h = 4, int w = 4, int max_step = 45) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_13_C

	const int OPEN = h * w - 1;

	// 盤面のハッシュ化用関数
	using ull = unsigned long long;
	function<void(ull, vi&)> integer_digits = [](ull n, vi& digits) {
		int i = 0;
		digits.resize(16);
		while (n > 0) {
			digits[i++] = n % 16;
			n /= 16;
		}
	};
	function<ull(vi&)> from_digits = [](vi& digits) {
		ull res = 0, pow16 = 1;
		repe(d, digits) {
			res += d * pow16;
			pow16 *= 16;
		}
		return res;
	};

	// 盤面のベクトル化，空きマス (x, y) の特定
	vi a(h * w);
	int x = -1, y = -1;
	rep(i, h) {
		rep(j, w) {
			a[i * w + j] = a_[i][j];

			if (a[i * w + j] == OPEN) {
				x = i; y = j;
			}
		}
	}

	// 完成形のハッシュ値の計算
	vi gl(h * w);
	iota(all(gl), 0);
	ull gl_hash = from_digits(gl);

	// mh[k][x * w + y] : パネル k が位置 (x, y) にある場合の正しい位置までのマンハッタン距離
	vvi mh(h * w, vi(h * w));
	rep(i, h) {
		rep(j, w) {
			rep(x, h) {
				rep(y, w) {
					mh[i * w + j][x * w + y] = abs(i - x) + abs(j - y);
				}
			}
		}
	}

	// 初期盤面の各パネルの完成形における位置までの距離の和を求める．（距離の下界）
	int d = 0;
	rep(i, h) {
		rep(j, w) {
			if (a[i * w + j] != OPEN) {
				d += mh[a[i * w + j]][i * w + j];
			}
		}
	}

	// ハッシュ化された盤面 → 総距離の下界
	unordered_map<ull, int> dist;

	// 総距離の下界，盤面, 空きマスの x, y 座標，操作回数を昇順に取り出すキュー
	//（総距離の下界 = 操作回数とするとただの幅優先探索になる．）
	priority_queue_rev<tuple< int, ull, int, int, int >> q;

	ull hash = from_digits(a);
	dist[hash] = d;
	q.push({ d, hash, x, y, 0 });

	// 解けるまで探索する．
	while (!q.empty()) {
		vi a; ull hash; int d, x, y, step;
		tie(d, hash, x, y, step) = q.top(); q.pop();
		integer_digits(hash, a);

		// 解けたなら手数を返す．
		if (hash == gl_hash) return step;

		// (nx, ny) : 空きマスを移動できる 4 近傍
		rep(k, 4) {
			int nx = x + dx4[k];
			int ny = y + dy4[k];

			if (nx < 0 || nx >= h || ny < 0 || ny >= w) {
				continue;
			}

			int num = a[nx * w + ny];
			a[x * w + y] = num;
			a[nx * w + ny] = OPEN;
			hash = from_digits(a);

			int nd = d - mh[num][nx * w + ny] + mh[num][x * w + y] + 1;
			if (!dist.count(hash)) dist[hash] = max_step + 1;
			if (chmin(dist[hash], nd)) {
				q.push({ nd, hash, nx, ny, step + 1 });
			}

			a[nx * w + ny] = num;
			a[x * w + y] = OPEN;
		}
	}

	return -1;
}


//【こおりのぬけみち】O(n log n)
/*
* n 個の岩が (x[i], y[i]) >= 0 に配置された「こおりのぬけみち」について，
* (sx, sy) から (tx, ty) までの最小移動回数を返す（到達不能なら INF を返す）
*
* 利用：【幅優先探索】
*/
int ice_path(const vl& x, const vl& y, ll sx, ll sy, ll tx, ll ty) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_f

	int n = sz(x); ll w = *max_element(all(y)) + 1;

	unordered_set<ll> stone;
	rep(i, n) stone.insert(x[i] * w + y[i]);

	unordered_map<ll, vl> x_to_y, y_to_x;
	unordered_map<ll, int> xy_to_i; int id = 0;

	xy_to_i[sx * w + sy] = id++;
	rep(i, n) {
		x_to_y[x[i]].push_back(y[i]);
		y_to_x[y[i]].push_back(x[i]);

		rep(k, 4) {
			ll nx = x[i] + dx4[k];
			ll ny = y[i] + dy4[k];

			if (stone.count(nx * w + ny)) continue;
			if (xy_to_i.count(nx * w + ny)) continue;

			xy_to_i[nx * w + ny] = id++;
		}
	}
	repea(p, x_to_y) sort(all(p.second));
	repea(p, y_to_x) sort(all(p.second));

	if (!xy_to_i.count(tx * w + ty)) return INF;

	Graph g(id);

	repe(p, xy_to_i) {
		ll xy, x, y; int id;
		tie(xy, id) = p;
		x = xy / w; y = xy % w;

		auto ity = lower_bound(all(x_to_y[x]), y);
		if (ity != x_to_y[x].end()) {
			g[id].push_back(xy_to_i[x * w + (*ity - 1)]);
		}
		ity = lower_bound(all(x_to_y[x]), y);
		if (ity != x_to_y[x].begin()) {
			g[id].push_back(xy_to_i[x * w + (*prev(ity) + 1)]);
		}
		auto itx = lower_bound(all(y_to_x[y]), x);
		if (itx != y_to_x[y].end()) {
			g[id].push_back(xy_to_i[(*itx - 1) * w + y]);
		}
		itx = lower_bound(all(y_to_x[y]), x);
		if (itx != y_to_x[y].begin()) {
			g[id].push_back(xy_to_i[(*prev(itx) + 1) * w + y]);
		}
	}

	vi dist;
	breadth_first_search(g, 0, dist);

	return dist[xy_to_i[tx * w + ty]];
}


