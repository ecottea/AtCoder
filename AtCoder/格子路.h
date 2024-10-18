#pragma once
#include "header.h"
#include "二項係数.h"
#include "座標圧縮.h"
// ■■■■■ 格子路の問題 ■■■■■


//【最短経路数】O(h w)
/*
* 通過できないマスが WALL で表された格子 c[0..h)[0..w) について，
* c[0][0] から c[i][j] までの最短路の数を格納した二次元リストを返す．
*
*（始点からの格子 DP）
*/
vvm count_lattice_path(const vvc& c, const char WALL = '#') {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_h

	int h = sz(c), w = sz(c[0]);

	// cnt[i][j] : c[0][0] から c[i][j] までの経路数
	vvm cnt(h, vm(w));
	cnt[0][0] = 1;

	// 貰う DP
	rep(i, h) rep(j, w) {
		// 通過できないマスの場合
		if (c[i][j] == WALL) continue;

		// 始点の場合
		if (i == 0 && j == 0) continue;

		// 上から来れる場合
		if (i > 0) cnt[i][j] += cnt[i - 1][j];

		// 左から来れる場合
		if (j > 0) cnt[i][j] += cnt[i][j - 1];
	}

	return cnt;
}


//【最短経路数】O(h w)
/*
* 通過の可否が c[i][j] = '.'['#'] で示された h×w 格子について，
* c[i][j] から c[h-1][w-1] までの最短路の数を cnt[i][j] に格納する．
*
*（終点からの格子 DP）
*/
void count_lattice_path_rev(const vvc& c, vvm& cnt, const char WALL = '#') {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_h

	int h = sz(c), w = sz(c[0]);

	// cnt[i][j] : c[i][j] から c[h-1][w-1] までの経路数
	cnt = vvm(h, vm(w));
	cnt[h - 1][w - 1] = 1;

	// 貰う DP
	repir(i, h - 1, 0) repir(j, w - 1, 0) {
		// 通過できないマスの場合
		if (c[i][j] == WALL) continue;

		// 終点の場合
		if (i == h - 1 && j == w - 1) continue;

		// 下に進める場合
		if (i < h - 1) cnt[i][j] += cnt[i + 1][j];

		// 右に進める場合
		if (j < w - 1) cnt[i][j] += cnt[i][j + 1];
	}
}


//【最短経路数（連続で曲がれない）】O(h w)
/*
* h×w 格子について，二連続で曲がることを禁止した場合の
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
	
	// 曲がった直後の停止が禁止なので，1 マス手前も結果に加える．
	mint res;
	res += dp[h - 1][w - 1][0];
	res += dp[h - 1][w - 1][1];
	res += dp[h - 1][w - 2][0];
	res += dp[h - 2][w - 1][1];

	return res;
}


//【スコア最大経路（スパーススコア指定）】O(n log n)
/*
* n 個の点 (x[i], y[i]) に非負スコア c[i] が与えられており，その他の点のスコアは 0 である．
* (-∞, -∞) から (∞, ∞) までの最短路のうち，スコアの和が最大のもののスコアを返す．
*
* 利用：【座標圧縮】
*
*（平面走査）
*/
ll op_mcp(ll a, ll b) { return max(a, b); }
ll e_mcp() { return 0; }
ll maximize_score_path(const vl& x_, const vl& y_, const vl& c) {
	// verify : https://yukicoder.me/problems/no/1826

	//【備考】
	// (x, y) でソートして y だけ抜き出せば，重み付き広義最長増加部分列問題に等しい．

	int n = sz(x_);

	// 座標圧縮を行う．
	vi x, y;
	int h = coordinate_compression(x_, x);
	int w = coordinate_compression(y_, y);

	// x 座標昇順，次いで y 座標昇順にソートする．
	vector<tuple<int, int, ll>> xyc(n);
	rep(i, n) xyc[i] = { x[i], y[i], c[i] };
	sort(all(xyc));

	// dp_i[j] : 点 i までで，y 座標が j である点までの最大スコア 
	segtree<ll, op_mcp, e_mcp> dp(w);

	rep(i, n) {
		auto [x, y, c] = xyc[i];

		ll pc = dp.prod(0, y + 1);
		dp.set(y, c + pc);
	}

	return dp.all_prod();
}


//【最短経路数（スパース禁止点指定）】O(n^2)
/*
* (0, 0) から (h, w) までの最短格子路のうち，
* n 個の禁止点 (x[i], y[i]) を 1 つも通らないものの個数を返す．
* 
* 制約：fm は (h+w)! まで計算可能
*
* 利用：【階乗など（法が大きな素数）】
*/
mint dummy_path_lemma(int h, int w, const vi& x, const vi& y, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_y

	int n = sz(x);

	// s, t : 対応する始点と終点の列
	// 番号の小さい方へ戻るパスは存在してはいけない．
	vector<pii> s = { {0, 0} }, t = { {h, w} };
	rep(i, n) {
		if (x[i] < 0 || x[i] > h || y[i] < 0 || y[i] > w) continue;
		s.emplace_back(x[i], y[i]);
	}
	n = sz(s) - 1;
	sort(next(s.begin()), s.end());
	repi(i, 1, n) t.push_back(s[i]);

	// DPL 用の行列を作成する（第 0 列と上三角部分以外の値は 0）
	vvm dpl(n + 1, vm(n + 1));
	repi(i, 0, n) repi(j, 0, n) {
		int h = t[j].first - s[i].first;
		int w = t[j].second - s[i].second;
		dpl[i][j] = fm.bin(h + w, h);
	}

	// 列基本変形で第 0 列の 1 行目以降を消去する．
	repir(j, n, 1) rep(i, j) dpl[i][0] -= dpl[j][0] * dpl[i][j];

	// DPL 行列の行列式，すなわち (0,0) 成分が答え．
	return dpl[0][0];
}


//【最短経路数（固定幅通路内）】O((h+w)/(l+r))
/*
* (0, 0) から (h, w) までの最短格子路のうち，常に -l < y-x < r を満たすものの個数を返す．
*
* 制約：fm は (h+w)! まで計算可能
*/
mint count_lattice_path_in_band(int h, int w, int l, int r, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/agc013/tasks/agc013_d

	//【方法】
	// 無制限での経路は bin(h+w, h) 通り．
	// そのうち先に y-x=-l に触れるものは反射原理より bin(h+w, h-l) 通りあるので引く．
	// その中でも先に y-x=-2l-r に触れるものが反射原理より bin(h+w, h-l-r) 通りあるので足す．
	// その中でも・・・を bin() = 0 になるまで繰り返す．
	// 先に y-x=r に触れるものについても同様である．

	if (-l >= w - h || w - h >= r) return 0;

	int d = l + r;
	int kl = (h + d - 1) / d;
	int kr = (w + d - 1) / d;

	mint res = 0;
	repi(k, -kl, kr) {
		res -= fm.bin(h + w, h + k * d - l);
		res += fm.bin(h + w, h + k * d);
	}

	return res;
}


//【最短経路数（任意通路内）】
/*
* 数え上げ(列).h の【広義単調増加列の数え上げ（上下限指定，mod 998244353）】を利用すれば良い．
*/


//【最短経路数（x≧y 内）】
/*
* (0, 0) から (h, w) までの最短格子路のうち，常に x≧y を満たすものの総数は
*	bin(h+w,h) (h-w+1) / (h+1)
* 
* 特に，(0, 0) から (n, n) までの最短格子路のうち，常に x≧y を満たすものの総数は
*	bin(2n,n) / (n+1) （カタラン数）
* 
* 参考 : https://oeis.org/A009766
* verify : https://projecteuler.net/problem=739
*/


//【最短経路数（x≧y 内，折れ曲がり回数指定）】
/*
* (0, 0) から (n, n) までの最短格子路のうち，常に x≧y を満たし，
* 直線 x=y に向かって折れ曲がる回数が k 回のものの個数は，Narayana 数
*	N(n, k) = (1/n) bin(n,k) bin(n,k-1)
* で与えられる．
* 
* 参考 : https://en.wikipedia.org/wiki/Narayana_number
*/


//【自由経路数】O(1)
/*
* (0, 0) から (x, y) まで n 回の移動で到達する格子路の数を返す．
*
* 制約：fm は n! まで計算可能
*/
mint count_free_lattice_path(int n, int x, int y, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_g

	//【方法】
	// x, y ≧ 0 とする．ローラン多項式の言葉に直すと，求める場合の数は
	//		[s^x t^y] (s + 1/s + t + 1/t)^n
	// である．以下明らかに 0 とわかる場合は無視する．
	//
	// 指数の底は因数分解できて，以下のように書き直せる：
	//		[s^x t^y] ( (s + t)^n (1 + 1/st)^n )
	// 
	// 第一因子からは s, t の次数の和が n の項しか作れないので，
	// 第二因子から作るべき項の次数の和は x + y - n である．
	// それが第 k 項だとすると，次数についての方程式
	//		0 * (n - k) + (-2) * k = x + y - n
	// を解いて
	//		k = (n - x - y) / 2
	// と分かり，その係数は二項定理より bin(n, (n-x-y)/2) である．
	//
	// 第二因子からは s, t の次数の差が 0 の項しか作れないので，
	// 第一因子から作るべき項の次数の差は x - y である．
	// それが第 k 項だとすると，次数についての方程式
	//		(n - k) - k = x - y
	// を解いて
	//		k = (n - x + y) / 2
	// と分かり，その係数は二項定理より bin(n, (n-x+y)/2) である．
	//
	// 以上より，求める場合の数は
	//		bin(n, (n-x-y)/2) bin(n, (n-x+y)/2)
	// である．

	//【別の方法】
	// 45°回転すれば，移動可能な箇所が x, y について独立（長方形状）になり，
	// 座標ごとに独立に問題を解いて積をとるだけでよくなる．

	x = abs(x); y = abs(y);

	// 明らかに 0 通りの場合
	if (x + y > n || (n - x - y) % 2) return 0;

	return fm.bin(n, (n - x - y) / 2) * fm.bin(n, (n - x + y) / 2);
}


//【直線に沿った格子路上の積】O(log(n + m))
/*
* (0, 0) から (n, (an+b)//m) までの直線 y=(ax+b)/m 以下の上方向優先の最短格子路について，
* 右に進むときは f，上に進むときは g を順に掛け合わせたモノイド (S, op, e) の元を返す．
*
* 制約：n≧0, m≧1, a≧0, b≧0
*/
template <class T, class S, S(*op)(S, S), S(*e)()>
S multiple_along_line(T n, T m, T a, T b, S f, S g) {
	// 参考 : https://github.com/hos-lyric/libra/blob/master/number/gojo.cpp
	// verify : https://judge.yosupo.jp/problem/sum_of_floor_of_linear

	Assert(n >= 0); Assert(m >= 1); Assert(a >= 0); Assert(b >= 0);

	// x^n を返す
	auto pow = [](const S& x, T n) {
		S res(e()), pow2 = x;
		while (n > 0) {
			if (n & 1) res = op(res, pow2);
			pow2 = op(pow2, pow2);
			n /= 2;
		}
		return res;
	};

	S resL = e(), resR = e(); bool rev = false;

	while (true) {
		// 傾きを 1 未満，切片を 1 未満にする．
		if (rev) {
			resR = op(pow(g, b / m), resR);
			f = op(pow(g, a / m), f);
		}
		else {
			resL = op(resL, pow(g, b / m));
			f = op(f, pow(g, a / m));
		}

		a %= m;
		b %= m;
		if (a == 0 || n == 0) break;

		// 左側の中途半端に余っている部分を切り取る．
		T l = (m - b + a - 1) / a;
		if (l > n) {
			if (rev) {
				resR = op(pow(f, n), resR);
			}
			else {
				resL = op(resL, pow(f, n));
			}
			n = 0;
			break;
		}

		if (rev) {
			resR = op(op(g, pow(f, l)), resR);
		}
		else {
			resL = op(resL, op(pow(f, l), g));
		}

		b = a * l + b - m;
		n -= l;
		if (n == 0) break;

		// 軸を取り直して傾きを 1 より大きくする．
		T nn = (a * n + b) / m;
		T nm = a;
		T na = m;
		T nb = a * n + b - m * nn;

		n = nn; m = nm; a = na; b = nb; swap(f, g);
		rev = !rev;
	}

	return op(resL, op(pow(f, n), resR));
}


//【迷路】O(h w)
/*
* 壁が wall で表された h×w の迷路 c について，スタート (sx, sy) から
* 各マス c[i][j] への最短経路長（到達不能なら INF）を返す．
*
*（格子上の幅優先探索）
*/
template <class T>
vvi solve_maze(const vector<vector<T>>& c, int sx, int sy, const T wall = '#') {
	// verify : https://atcoder.jp/contests/abc317/tasks/abc317_e

	int h = sz(c), w = sz(c[0]);

	vvi dist(h, vi(w, INF));
	dist[sx][sy] = 0;

	// q : 未探索のマスを記録しておくキュー
	queue<pii> q;
	q.push({ sx, sy });

	while (!q.empty()) {
		auto [x, y] = q.front(); q.pop();

		// マス (x, y) の 4 近傍を調べる．
		rep(k, 4) {
			// (nx, ny) : (x, y) の近傍の座標
			int nx = x + DX[k];
			int ny = y + DY[k];

			// 範囲外または壁マスなら何もしない．
			if (!inQ(nx, ny, 0, 0, h, w) || c[nx][ny] == wall) continue;

			// 既に最短経路長が確定済みなら何もしない．
			if (dist[nx][ny] != INF) continue;

			// 最短経路長の確定
			dist[nx][ny] = dist[x][y] + 1;

			q.push({ nx, ny });
		}
	}

	return dist;
}


//【迷路（複数始点）】O(h w)
/*
* 壁が wall で表された h×w の迷路 c について，スタートの集合 s[i] = (sx, sy) から
* 各マス c[i][j] への最短経路長の最小値を dist[i][j] に格納する．（到達不能なら -1）
*
*（格子上の幅優先探索）
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


//【格子上ハミルトンサイクルの構築】
/*
* h×w 格子上の 4 近傍ハミルトンサイクルをなす点列を構築し返す（なければ空リスト）
*/
vector<pii> create_grid_hamilton_cycle(int h, int w) {
	// verify : https://atcoder.jp/contests/arc118/tasks/arc118_d

	// 幅が 1 の場合は蛇腹が構築できない．
	if (h == 1 || w == 1) return vector<pii>();;

	// h, w ともに奇数だと頂点数が奇数個になるが，
	// グリッドグラフは二部グラフなのでハミルトンサイクルは存在し得ない．
	if (h % 2 == 1 && w % 2 == 1) return vector<pii>();

	vector<pii> res{ {0, 0} };

	// h が偶数のときは，横方向に往復する蛇腹状に構築できる．
	if (h % 2 == 0) {
		rep(i, h) {
			if (i % 2 == 0)	repi(j, 1, w - 1) res.emplace_back(i, j);
			else repir(j, w - 1, 1) res.emplace_back(i, j);
		}
		repir(i, h - 1, 1) res.emplace_back(i, 0);
	}
	// w が偶数のときは，縦方向に往復する蛇腹状に構築できる．
	else {
		rep(j, w) {
			if (j % 2 == 0)	repi(i, 1, h - 1) res.emplace_back(i, j);
			else repir(i, h - 1, 1) res.emplace_back(i, j);
		}
		repir(j, w - 1, 1) res.emplace_back(0, j);
	}

	return res;
}


//【格子 DAG の座標圧縮】
/*
* 座標圧縮.h へ
*/


