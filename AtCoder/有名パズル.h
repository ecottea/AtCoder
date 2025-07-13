#pragma once
#include "header.h"
#include "bit行列.h"
#include "最短路.h"
// ■■■■■ 有名パズル ■■■■■


//【N クイーン問題】O(n! n)
/*
* 既に (x0[i], y0[i]) に置かれたクイーンを含む，n×n の格子上で互いに効きに入らないような
* クイーンの配置例を見つけ，i 行目のクイーンが何列目にあるかを格納したリストを返す（不可能なら空リスト）
*
*（順列全探索）
*/
vi N_queen_problem(int n, const vi& x0, const vi& y0) {
	// verify : https://atcoder.jp/contests/arc001/tasks/arc001_3

	int k = sz(x0);

	vi x2y(n, -1);
	rep(i, k) {
		// 同じ行に 2 つクイーンがある場合は明らかに不可能．
		if (x2y[x0[i]] != -1) return vi();

		x2y[x0[i]] = y0[i];
	}

	// p[i] : i 行目のクイーンを何列目に置くか
	vi p(n);
	iota(all(p), 0);

	// 順列全探索を行うので，各行各列に 1 つずつであることまでは保証されている．
	repp(p) {
		vb rd(2 * n - 1), ld(2 * n - 1); bool ok = true;

		rep(i, n) {
			// 初期配置と矛盾がないかのチェック
			if (x2y[i] != -1 && x2y[i] != p[i]) {
				ok = false;
				break;
			}

			// 左下がりの対角線方向のチェック
			if (ld[i + p[i]]) {
				ok = false;
				break;
			}
			ld[i + p[i]] = true;

			// 右下がりの対角線方向のチェック
			if (rd[i - p[i] + n - 1]) {
				ok = false;
				break;
			}
			rd[i - p[i] + n - 1] = true;
		}

		if (ok) return p;
	}

	return vi();
}


//【ライツアウト】O(h^3 w^3 / 64)
/*
* ライツアウトの盤面 s[0..h)[0..w) に対し，全マスを消灯する押し方を sol に格納する．
* s[i][j] = one はマス (i, j) が点灯状態であることを表す．
* sol[i][j] = false[true] はマス (i, j) を押さない[押す] ことを表す．
* 解が存在しないなら false を返す．
*
*（呼び出すとき lights_out<M> としないと gcc でエラーになるので注意．）
*
* 制約：h * w ≦ M
*
* 利用：【ビット行列】,【線形方程式】
*/
template <int M>
bool lights_out(const vvc& s, vvb* sol = nullptr, char one = '1') {
	int h = sz(s), w = sz(s[0]);

	Bit_matrix<M> mat(h * w, h * w); vb vec(h * w);
	rep(x, h) rep(y, w) {
		// 押したマスは点灯状態が反転する．
		mat[x * w + y][x * w + y] = 1;

		// 押したマスの 4 近傍も点灯状態が反転する．
		rep(k, 4) {
			int nx = x + DX[k];
			int ny = y + DY[k];

			if (nx < 0 || h <= nx || ny < 0 || w <= ny) continue;

			mat[nx * w + ny][x * w + y] = 1;
		}

		// 目標とする点灯パターン
		vec[x * w + y] = (s[x][y] == one);
	}

	// 線形方程式を解いて押し方を見つける．
	bitset<M> res;
	bool able = gauss_jordan_elimination<M>(mat, vec, &res);

	// 線形方程式に解が無いなら不可能．
	if (!able) return false;

	// 線形方程式の解から押し方を復元する．
	if (sol != nullptr) {
		*sol = vvb(h, vb(w));
		rep(x, h) rep(y, w) (*sol)[x][y] = res[x * w + y];
	}

	return true;
}


//【15 パズル】O(?)
/*
* h * w スライドパズルの盤面 a[0..h)[0..w) を解くのにかかる最短手数を返す．
* 0 以上 h*w-1 未満の数はパネルを，数 h*w-1 は空きマスを表す．
* max_step 手以内に解けない場合は -1 を返す．
*
*（A* 探索）
*/
int solve_15puzzle(const vvi& a_, int max_step = 45) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_13_C

	int h = sz(a_), w = sz(a_[0]);
	const int OPEN = h * w - 1;

	// 盤面のハッシュ化用関数
	function<void(ull, vi&)> integer_digits = [&](ull n, vi& digits) {
		int b = h * w, i = 0;
		digits.resize(b);
		while (n > 0) {
			digits[i++] = n % b;
			n /= b;
		}
	};
	function<ull(vi&)> from_digits = [&](vi& digits) {
		int b = h * w; ull res = 0, powb = 1;
		repe(d, digits) {
			res += d * powb;
			powb *= b;
		}
		return res;
	};

	// 盤面のベクトル化，空きマス (x, y) の特定
	vi a(h * w);
	int x = -1, y = -1;
	rep(i, h) rep(j, w) {
		a[i * w + j] = a_[i][j];

		if (a[i * w + j] == OPEN) {
			x = i; y = j;
		}
	}

	// 完成形のハッシュ値の計算
	vi gl(h * w);
	iota(all(gl), 0);
	ull gl_hash = from_digits(gl);

	// mh[k][x * w + y] : パネル k が位置 (x, y) にある場合の正しい位置までのマンハッタン距離
	vvi mh(h * w, vi(h * w));
	rep(i, h) rep(j, w) rep(x, h) rep(y, w) {
		mh[i * w + j][x * w + y] = abs(i - x) + abs(j - y);
	}

	// 初期盤面の各パネルの完成形における位置までの距離の和を求める．（距離の下界）
	int d = 0;
	rep(i, h) rep(j, w) {
		if (a[i * w + j] != OPEN) d += mh[a[i * w + j]][i * w + j];
	}

	// ハッシュ化された盤面 → 総距離の下界
	unordered_map<ull, int> dist;

	// 総距離の下界，盤面, 空きマスの x, y 座標，操作回数を昇順に取り出すキュー
	//（総距離の下界 = 操作回数とするとただの幅優先探索になる．）
	priority_queue_rev<tuple<int, ull, int, int, int>> q;

	ull hash = from_digits(a);
	dist[hash] = d;
	q.push({ d, hash, x, y, 0 });

	// 解けるまで探索する．
	while (!q.empty()) {
		auto [d, hash, x, y, step] = q.top(); q.pop();
		
		vi a;
		integer_digits(hash, a);

		// 解けたなら手数を返す．
		if (hash == gl_hash) return step;

		// (nx, ny) : 空きマスを移動できる 4 近傍
		rep(k, 4) {
			int nx = x + DX[k];
			int ny = y + DY[k];

			if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;

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


//【スライドパズル】
/*
* スライドパズルが解けるための必要十分条件は，
* パネルおよび空きマスの置換の偶奇と空きマスの移動距離の偶奇が一致することである．
* 
* verify : https://yukicoder.me/problems/no/3131
*/


//【こおりのぬけみち】O(n log n)
/*
* n 個の岩が (x[i], y[i]) ≧ 0 に配置されたこおりのぬけみちについて，
* (sx, sy) ≧ 0 から (tx, ty) ≧ 0 で停止するまでの最小移動回数を返す（不可能なら INF を返す）
*
*（幅優先探索）
*/
int ice_path(const vl& x, const vl& y, ll sx, ll sy, ll tx, ll ty) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_f

	int n = sz(x);

	ll w = max({ *max_element(all(y)), sy, ty }) + 1;

	ll sxy = sx * w + sy, txy = tx * w + ty;

	// x_to_y[x] : x 座標が x である行にある岩の y 座標の昇順列
	// y_to_x[y] : y 座標が y である列にある岩の x 座標の昇順列
	unordered_map<ll, vl> x_to_y, y_to_x;
	rep(i, n) {
		x_to_y[x[i]].push_back(y[i]);
		y_to_x[y[i]].push_back(x[i]);
	}
	repea(p, x_to_y) sort(all(p.second));
	repea(p, y_to_x) sort(all(p.second));

	// dist[xy] : (sx, sy) から (x, y) への最小移動回数
	unordered_map<ll, int> dist;
	dist[sxy] = 0;

	// 未探索の頂点を記録しておくキュー
	queue<ll> que;
	que.push(sxy);

	// (x, y) から移動できる頂点 (nx, ny) のリストを返す．
	auto get_next = [&](ll xy) {
		ll x = xy / w, y = xy % w;

		vl res;

		// 左右それぞれで最も近くにある岩を探し，その手前を移動先の候補とする．
		auto it_y = x_to_y.find(x);
		if (it_y != x_to_y.end()) {
			auto& ys = it_y->second;
			auto it = lower_bound(all(ys), y);
			if (it != ys.end()) res.emplace_back(x * w + (*it - 1));
			if (it != ys.begin()) res.emplace_back(x * w + (*prev(it) + 1));
		}

		// 上下それぞれで最も近くにある岩を探し，その手前を移動先の候補とする．
		auto it_x = y_to_x.find(y);
		if (it_x != y_to_x.end()) {
			auto& xs = it_x->second;
			auto it = lower_bound(all(xs), x);
			if (it != xs.end()) res.emplace_back((*it - 1) * w + y);
			if (it != xs.begin()) res.emplace_back((*prev(it) + 1) * w + y);
		}

		return res;
	};

	// 幅優先探索で最短距離を決定する．
	while (!que.empty()) {
		// 未探索の頂点 (x, y) を得る．
		auto xy = que.front(); que.pop();
		int d = dist[xy];

		// ゴール (tx, ty) に到達したなら最短距離を返す．
		if (xy == txy) return d;

		repe(nxy, get_next(xy)) {
			// (nx, ny) が発見済みの頂点なら何もしない．
			if (dist.count(nxy)) continue;

			// スタート (sx, sy) からの最短距離を確定する．
			dist[nxy] = d + 1;

			// 未探索の頂点として (nx, ny) を追加する．
			que.push(nxy);
		}
	}

	// ゴール (tx, ty) に到達できなかった場合は INF を返す．
	return INF;
}


//【ラテン方格】O(?)（n=5 まで動く）
/*
* 数字 [0..n) からなる n×n ラテン方格のリストを返す．
*/
vvvi enumerate_latin_squares(int n) {
	// verify : https://atcoder.jp/contests/abc326/tasks/abc326_d

	vvb used_x(n, vb(n)), used_y(n, vb(n));

	vvi a(n, vi(n, -1));
	vvvi res;

	// (i, j): 注目位置
	function<void(int, int)> dfs = [&](int x, int y) {
		if (x == n) {
			res.push_back(a);
			return;
		}

		if (y == n) {
			dfs(x + 1, 0);
			return;
		}

		rep(k, n) {
			if (used_x[x][k] || used_y[y][k]) continue;

			used_x[x][k] = used_y[y][k] = true;
			a[x][y] = k;
			dfs(x, y + 1);
			used_x[x][k] = used_y[y][k] = false;
		}
	};
	dfs(0, 0);

	return res;
}


//【魔方陣】O(?)
/*
* 一部の数字が書き込まれた方陣 a[0..n)[0..n) について，条件に合う魔方陣をそこに構築する（なければ空）
* 書き込まれる値は [1..n^2] とし，欠損値は 0 で表す．
*/
void find_magic_square(vvi& a) {
	int n = sz(a);

	int s = n * (n * n + 1) / 2;

	// 残っている数を記録する双方向リスト（0 番目は根として利用する）
	vi prv(n * n + 2), nxt(n * n + 2);
	iota(all(prv), -1);
	iota(all(nxt), 1);

	vi x_sum(n), x_cnt(n), y_sum(n), y_cnt(n);
	int d_sum = 0, d_cnt = 0, ad_sum = 0, ad_cnt = 0;

	rep(i, n) rep(j, n) {
		x_sum[i] += a[i][j];
		x_cnt[i] += a[i][j] != 0;

		y_sum[j] += a[i][j];
		y_cnt[j] += a[i][j] != 0;

		if (i == j) {
			d_sum += a[i][j];
			d_cnt += a[i][j] != 0;
		}

		if (i + j == n - 1) {
			ad_sum += a[i][j];
			ad_cnt += a[i][j] != 0;
		}

		if (a[i][j] != 0) {
			int v = a[i][j];
			nxt[prv[v]] = nxt[v];
			prv[nxt[v]] = prv[v];
		}
	}

	// (i, j): 注目位置
	function<bool(int, int)> dfs = [&](int x, int y) {
		if (x == n) return true;

		if (y == n) return dfs(x + 1, 0);

		if (a[x][y] != 0) return dfs(x, y + 1);

		vi is;
		if (x_cnt[x] == n - 1) is.push_back(s - x_sum[x]);
		if (y_cnt[y] == n - 1) is.push_back(s - y_sum[y]);
		if (x == y && d_cnt == n - 1) is.push_back(s - d_sum);
		if (x + y == n - 1 && ad_cnt == n - 1) is.push_back(s - ad_sum);

		uniq(is);
		if (sz(is) >= 2) return false;

		if (sz(is) == 1) {
			int i = is[0];
			if (i <= 0 || n * n < i) return false;
			if (prv[nxt[i]] != i) return false;

			a[x][y] = i;
			x_sum[x] += i; x_cnt[x]++;
			y_sum[y] += i; y_cnt[y]++;
			if (x == y) { d_sum += i; d_cnt++; }
			if (x + y == n - 1) { ad_sum += i; ad_cnt++; }
			nxt[prv[i]] = nxt[i];
			prv[nxt[i]] = prv[i];

			if (dfs(x, y + 1)) return true;

			a[x][y] = 0;
			x_sum[x] -= i; x_cnt[x]--;
			y_sum[y] -= i; y_cnt[y]--;
			if (x == y) { d_sum -= i; d_cnt--; }
			if (x + y == n - 1) { ad_sum -= i; ad_cnt--; }
			prv[nxt[i]] = i;
			nxt[prv[i]] = i;

			return false;
		}

		for (int i = nxt[0]; i <= n * n; i = nxt[i]) {
			a[x][y] = i;
			x_sum[x] += i; x_cnt[x]++;
			y_sum[y] += i; y_cnt[y]++;
			if (x == y) { d_sum += i; d_cnt++; }
			if (x + y == n - 1) { ad_sum += i; ad_cnt++; }
			nxt[prv[i]] = nxt[i];
			prv[nxt[i]] = prv[i];

			if (dfs(x, y + 1)) return true;

			a[x][y] = 0;
			x_sum[x] -= i; x_cnt[x]--;
			y_sum[y] -= i; y_cnt[y]--;
			if (x == y) { d_sum -= i; d_cnt--; }
			if (x + y == n - 1) { ad_sum -= i; ad_cnt--; }
			prv[nxt[i]] = i;
			nxt[prv[i]] = i;
		}

		return false;
	};

	if (!dfs(0, 0)) a.clear();
}


