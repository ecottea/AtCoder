#pragma once
#include "header.h"
#include "最短路.h"
#include "分析(グラフ).h"
#include "DAG.h"
// ■■■■■ 線形計画問題 ■■■■■


//【牛ゲー】
/*
* Ushige(int n) : O(n)
*	n 変数で初期化する．
*
* set_ub(int a, int b, ll d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．
*
* set_lb(int a, int b, ll d) : O(1)
*	v[b] - v[a] >= d という制約を追加する．
*
* bool maximize_diff(int a, vl& diff) : O(n m)（m : 制約の数）
*	v[b] - v[a] の最大値（無いなら INFL）を diff[b] に格納する．
*	制約を満たすことが不可能なら false を返す．
*/
struct Ushige {
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/0304

	int n;
	WGraph g;

	Ushige(int n_) : n(n_), g(n_) {}

	void set_ub(int a, int b, ll d) {
		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	void set_lb(int a, int b, ll d) {
		// 差の下限に対応する重みを持つ辺を張る．
		g[b].push_back({ a, -d });
	}

	bool maximize_diff(int a, vl& diff) {
		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		// ただし負の閉路を持っていた場合は制約を満たせない．
		return bellman_ford(g, a, diff);
	}

	bool bellman_ford(const WGraph& g, int st, vl& cost) {
		cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル
		cost[st] = 0;

		rep(i, n) {
			bool updated = false;

			// 全ての辺についての操作
			rep(s, n) {
				repe(e, g[s]) {
					// INFL からの引き算も認めて計算しているので，
					// st から到達不可能な負閉路も含めて検出することに注意．
					if (cost[s] + e.cost < cost[e.to]) {
						cost[e.to] = cost[s] + e.cost;
						updated = true;
					}
				}
			}

			// もしコストの更新が起こらなければ最小コスト確定
			if (!updated) return true;
		}

		// もし全ての辺についての操作を |V| 回繰り返してもコストの更新があったなら，
		// どこかに負の閉路を持っているので false を返す．
		return false;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（上界指定のみ）】
/*
* Ushige_ub_only(n) : O(1)
*	n 変数で初期化する．
*
* set_ub(a, b, d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．（d >= 0）
*
* maximize_diff(a, diff) : O(n + m log n)（m : 制約の数）
*	v[b] - v[a] の最大値（無いなら INFL）を diff[b] に格納する．
*
* 利用：【単一始点最短路／ダイクストラ法】
*/
struct Ushige_ub_only {
	int n;
	WGraph g;

	Ushige_ub_only() : n(0) {}
	Ushige_ub_only(int n_) : n(n_), g(n_) {}

	void set_ub(int a, int b, ll d) {
		Assert(d >= 0);
		
		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	void maximize_diff(int a, vl& diff) {
		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		dijkstra(g, a, diff);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_ub_only& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（01-上界指定のみ）】
/*
* Ushige_ub01_only(n) : O(1)
*	n 変数で初期化する．
*
* set_ub(a, b, d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．（d ∈ {0, 1}）
*
* maximize_diff(a, diff) : O(n + m)（m : 制約の数）
*	v[b] - v[a] の最大値（無いなら INFL）を diff[b] に格納する．
*
* 利用：【01-BFS】
*/
struct Ushige_ub01_only {
	// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c

	int n;
	WGraph g;

	Ushige_ub01_only() : n(0) {}
	Ushige_ub01_only(int n_) : n(n_), g(n_) {}

	void set_ub(int a, int b, int d) {
		Assert(d == 0 || d == 1);

		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	void maximize_diff(int a, vi& diff) {
		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		binary_bfs(g, a, diff);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_ub01_only& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（1-下界指定のみ）】
/*
* Ushige_lb1_only(int n) : O(n)
*	n 変数で初期化する．
*
* set_lb1(int a, int b) : O(1)
*	v[b] - v[a] >= 1 という制約を追加する．
*
* bool minimize_range(vi& val) : O(n + m)（m : 制約の数）
*	min(v) = 0 で max(v) を最小とする v[i] の一例を val[i] に格納する．
*	制約を満たすことが不可能なら false を返す．
*
* 利用：【トポロジカルソート】,【最長パス】
*/
struct Ushige_lb1_only {
	// verify : https://codeforces.com/contest/1635/problem/E

	int n;
	Graph g; // 辺の重みが -1 のグラフ

	Ushige_lb1_only(int n_) : n(n_), g(n_) {}

	void set_lb1(int a, int b) {
		// 差の下限に対応する重みを持つ辺を張る．
		g[b].push_back(a);
	}

	bool minimize_range(vi& val) {
		// 負閉路がどこかにあれば制約充足不可能
		vi seq;
		bool top_res = topological_sort(g, seq);
		if (!top_res) return false;

		// DAG が保証されたので最長パスを求める．
		longest_path(g, val);
		return true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_lb1_only& u) {
		rep(s, u.n) {
			repe(t, u.g[s]) {
				os << "v[" << t << "] - v[" << s << "] >= " << 1 << endl;
			}
		}
		return os;
	}
#endif
};


//【整数計画問題（2 変数，1 条件）】O(√e)
/*
* 変数 x, y(>=0) についての整数計画問題
*	maximize :   a x + b y
*	subject to : c x + d y <= e
* の解の目的関数値を返し，実行可能解を sx, sy に格納する．
*
* 制約：c > 0, d > 0, e >= 0
*
*（平方分割）
*/
ll integer_programming_2var_1sub(ll a, ll b, ll c, ll d, ll e, ll* sx_ = nullptr, ll* sy_ = nullptr) {
	// verify : https://atcoder.jp/contests/arc139/tasks/arc139_b

	Assert(c > 0 && d > 0 && e >= 0);

	ll sx = -1, sy = -1, res = -INFL;

	// 直線 a x + b y = const を左下方向に移動させる場合
	if (a <= 0 && b <= 0) {
		// 明らかに原点で最大となる．
		sx = 0;
		sy = 0;
		res = 0;
	}
	// 直線 a x + b y = const を左上方向に移動させる場合
	else if (a <= 0 && b > 0) {
		// 明らかに y 軸上で最大となる．
		sx = 0;
		sy = e / d;
		res = b * sy;
	}
	// 直線 a x + b y = const を右下方向に移動させる場合
	else if (a > 0 && b <= 0) {
		// 明らかに x 軸上で最大となる．
		sx = e / c;
		sy = 0;
		res = a * sx;
	}
	// 以降は直線 a x + b y = const を右上方向に移動させる場合について考える．
	else {
		// a d - b c >= 0 としておく．
		bool swap_flag = false;
		if (a * d - b * c < 0) {
			swap(a, b); swap(c, d);
			swap_flag = true;
		}

		// O(e/c) の全探索を採用する場合
		if (e / c < c) {
			// x の動ける範囲は 0 <= x <= e/c なので，x を決め打ち全探索する．
			repi(x, 0, e / c) {
				ll y = (e - c * x) / d;

				if (chmax(res, a * x + b * y)) { sx = x; sy = y; };
			}
		}
		// O(c) の全探索を採用する場合
		else {
			// 最適解 (x0, y0) においては 0 <= y0 < c なので，y を決め打ち全探索する．
			//（もし y0 >= c だと (x0 + d, y0 - c) の方が目的関数値を大きくする．）
			repi(y, 0, min(c - 1, e / d)) {
				ll x = (e - d * y) / c;

				if (chmax(res, a * x + b * y)) { sx = x; sy = y; };
			}
		}

		if (swap_flag) swap(sx, sy);
	}

	if (sx_ != nullptr) *sx_ = sx;
	if (sy_ != nullptr) *sy_ = sy;
	return res;
}


