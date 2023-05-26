#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ ゲーム ■■■■■


//【局面の勝敗】O(?)（遅いので実験用）
/*
* 先手番での初期局面 p_ini から遷移可能な局面とその勝敗を {{手番, 局面}, 勝敗} で表したリストを返す．
* nxt(t, p, nps) を呼ぶと，t=1:先手番[t=0:後手番] での局面 p から遷移可能な局面のリストを nps に格納する．
* ただし nps が空の場合は，先手勝ちなら 1，後手勝ちなら 0 を返すようにする．
*/
template <class T>
map<pair<int, T>, int> decide_WL(const T& p_ini, function<int(int, const T&, vector<T>&)>& nxt) {
	// verify : https://yukicoder.me/problems/no/2132

	map<pair<int, T>, int> res;

	// t=1:先手番[t=0:後手番] で局面 p であるときの勝敗を返す．
	function<int(int, const T&)> dfs = [&](int t, const T& p) {
		// 既に勝敗が確定済ならその結果を返す．
		if (res.count({ t, p })) return res[{t, p}];

		// 局面 p から遷移可能な局面の集合 nps を得る．
		vector<T> nps;
		int wl = nxt(t, p, nps);

		// p から遷移可能な局面が無い場合は決着．
		if (nps.empty()) {
			res[{t, p}] = wl;
			return wl;
		}

		// 遷移先に自分勝ちの局面が全く無ければ相手勝ち
		res[{t, p}] = 1 - t;

		// 遷移先に自分勝ちの局面が 1 つでもあれば自分勝ち
		repe(np, nps) {
			if (dfs(1 - t, np) == t) {
				res[{t, p}] = t;
			}
		}

		return res[{t, p}];
	};

	dfs(1, p_ini);

	return res;

	/* nxt の定義の雛形
	using T = tuple<ll, ll, vl>;
	function<int(int, const T&, vector<T>&)> nxt = [&](int t, const T& p, vector<T>& nps) {
		ll l, r; vl a;
		tie(l, r, a) = p;

		return 0;
	};
	*/
}


//【重み付き DAG 上のコマ移動ゲーム】O(n + m)　
/*
* ゲームのルール：
* 重み付き DAG g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かし，辺の重み分の得点を得る．
* コマが移動不可能になったとき，自分がより高い点数を得ていることを目指す．
*
* res[v] = {f, l} : v にコマがある状態から最善を尽くしたときの f:先手[l:後手] の点数
*/
void weighted_DAG_game(const WGraph& g, vector<pll>& res) {
	int n = sz(g);

	vb seen(n);
	res = vector<pll>(n);

	function<void(int)> dfs = [&](int s) {
		// s の情報を計算済だったら何もしない．
		if (seen[s]) return;
		seen[s] = true;

		// コマが移動不能になったら終了．
		if (g[s].empty()) {
			res[s] = { 0, 0 };
			return;
		}

		// s から出ている辺 e の情報を元に s の情報を得る．
		res[s] = { -INFL, INFL };
		repe(e, g[s]) {
			// e を辿った場合の結果を求める．
			dfs(e.to);

			// e を辿る方が利得が大きいなら更新する．
			if (res[s].first < res[e.to].second + e.cost) {
				res[s].first = res[e.to].second + e.cost;
				res[s].second = res[e.to].first;
			}
		}
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【重み付き有向グラフ上のコマ移動ゲーム】O(n + m logn)
/*
* 重み付き有向グラフ（閉路可）g のある頂点 v にコマが置かれている．
* 左と右は交互にコマを辺で繋がれた頂点のいずれかへ動かし，
* 通った辺の重みの総和を左は最小化，右は最大化する（終了せず +∞ になることも認める．）
* コマが v にある状態から，左[右] が先手だった場合の最終スコアを sc_l[sc_r] に格納する．
*
*（後退解析）
*/
void weighted_directed_graph_game(const WGraph& g, vl& sc_l, vl& sc_h) {
	// verify : https://atcoder.jp/contests/abc261/tasks/abc261_h

	int n = sz(g);
	sc_l.resize(n);
	sc_h.resize(n);

	// g_rev : g の辺の向きを逆にし，頂点を倍にしたグラフ（偶数：左先手，奇数：右先手）
	WGraph g_rev(2 * n);
	rep(s, n) repe(e, g[s]) {
		g_rev[2 * e.to].push_back({ 2 * s + 1, e.cost });
		g_rev[2 * e.to + 1].push_back({ 2 * s, e.cost });
	}

	vl sc(2 * n, INFL);

	// rem[i] : まだ調べていない局面 i から遷移可能な局面がいくつあるか
	vi rem(2 * n);

	// 終局までのスコア昇順に局面を取り出す優先度付きキュー
	priority_queue_rev<pli> q;

	rep(t, n) {
		// 遷移可能な局面数の記録
		rem[2 * t] = rem[2 * t + 1] = sz(g[t]);

		// 遷移不能な頂点ならどちらが先手でもスコアは 0
		if (sz(g[t]) == 0) {
			sc[2 * t] = sc[2 * t + 1] = 0;

			repe(e, g_rev[2 * t]) q.push({ e.cost, e.to });
			repe(e, g_rev[2 * t + 1]) q.push({ e.cost, e.to });
		}
	}

	// 後退解析を行う．スコアが有限で確定する局面がある限り処理を行う．
	while (!q.empty()) {
		ll c; int t;
		tie(c, t) = q.top(); q.pop();

		rem[t]--;

		// 左が先手の場合（スコア最小化）
		if (t % 2 == 0) {
			// スコア昇順に局面を取り出しているので，最初に見たものが最善
			if (c >= sc[t]) continue;

			sc[t] = c;
			repe(e, g_rev[t]) q.push({ c + e.cost, e.to });
		}
		// 右が先手の場合（スコア最大化）
		else {
			// スコア昇順に局面を取り出しているので，最後に見たものが最善
			if (rem[t] == 0) {
				sc[t] = c;
				repe(e, g_rev[t]) q.push({ c + e.cost, e.to });
			}
		}
	}
	// 後退解析が終わってもスコアが有限で確定していない局面は全てスコア +∞ になる．

	rep(i, n) {
		sc_l[i] = sc[2 * i];
		sc_h[i] = sc[2 * i + 1];
	}
}


//【カードめくり】O(n^2 log n)
/*
* n 枚のカードからなる山札があり，上から i 枚目には a[i] が書かれている．
* 先手が x，後手が y と書かれたカードを持った状態から以下のゲームを行う：
*	山札から任意に [1..k] 枚カードを引き，最後のカードを新たに手札とする．
*	スコアは最終的な両者の手札が p, q だったとき sc(p, q) で与えられる．
* 先手はスコアの最大化，後手はスコアの最小化を目指すときの最終スコアを返す．
*/
using S3 = ll;
S3 op3(S3 a, S3 b) { return max(a, b); }
S3 e3() { return -INFL; }
using S4 = ll;
S4 op4(S4 a, S4 b) { return min(a, b); }
S4 e4() { return INFL; }
ll card_flipping_game(const vl& a, ll x, ll y, int k, function<ll(ll, ll)>& sc) {
	int n = sz(a);

	// dpx[i][j] : 手札が a[i-1], a[j-1] の状態で先手番のときの最終スコア 
	using RmQ = segtree<S4, op4, e4>;
	vector<RmQ> dpx(n + 1, RmQ(n + 1));
	repi(i, 1, n - 1) dpx[i].set(n, sc(a[i - 1], a[n - 1]));
	dpx[0].set(n, sc(x, a[n - 1]));

	// dpy[j][i] : 手札が a[i-1], a[j-1] の状態で後手番のときの最終スコア 
	using RMQ = segtree<S3, op3, e3>;
	vector<RMQ> dpy(n + 1, RMQ(n + 1));
	repi(j, 1, n - 1) dpy[j].set(n, sc(a[n - 1], a[j - 1]));
	dpy[0].set(n, sc(a[n - 1], y));

	// 全探索しているが，無駄な探索を削れば O(n k log n) にできる．
	repir(i, n - 1, 0) {
		repir(j, n - 1, 0) {
			// 先手番の可能性がある場合
			if (i == 0 || i < j) {
				ll x2 = dpy[j].prod(i + 1, min(i + k + 1, n + 1));
				dpx[i].set(j, x2);
			}

			// 後手番の可能性がある場合
			if (j == 0 || j < i) {
				ll y2 = dpx[i].prod(j + 1, min(j + k + 1, n + 1));
				dpy[j].set(i, y2);
			}
		}
	}

	return dpx[0].get(0);
}


