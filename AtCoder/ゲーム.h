#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ ゲーム ■■■■■


//【グラフ上の点取りゲーム】O(|V| + |E|)　
/*
* ゲームのルール：
* コスト付き有向非巡回グラフ g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かし，辺のコスト分の得点を得る．
* コマが移動不可能になったとき，自分がより高い点数を得ていることを目指す．
*
* res[v] = {f, l} : v にコマがある状態から最善を尽くしたときの f:先手[l:後手] の点数
*/
void score_game(const WGraph& g, vector<pll>& res) {
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


//【mint → 有理数】（実験用）
/*
* mint の値をそれっぽい有理数に変換する．
*/
void mint_to_frac() {
	unordered_map<int, pii> nd;

	repi(j, 1, 1024) {
		mint j_inv = mint(j).inv();

		repi(i, 1, 1024) {
			int v = (i * j_inv).val();

			if (!nd.count(v)) nd[v] = { i, j };
		}
	}

	dump(nd[748683265]);
	dump(nd[457528662]);
}


