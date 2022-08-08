#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの探索 ■■■■■


//【幅優先探索】O(|V| + |E|)
/*
* グラフ g に対し始点を st として幅優先探索を行い，
* st から各頂点 s への最短経路長を dist[s] に格納する．
* s が st から到達不能な頂点の場合は dist[s] = INF となる．
*/
void breadth_first_search(const Graph& g, int st, vi& dist) {
	// verify : https://algo-method.com/tasks/414

	int n = sz(g);

	dist = vi(n, INF); // スタートからの最短距離を保持するテーブル
	dist[st] = 0;

	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(st);

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			q.push(t);
		}
	}
}


//【幅優先探索（距離上限指定）】O((max deg(v))^D)
/*
* グラフ g に対し始点を st として距離 D 以下の範囲の幅優先探索を行い，
* st から各頂点 s への最短経路長を dist[s] に格納する．
*/
void breadth_first_search(const Graph& g, int st, int D, unordered_map<int, int>& dist) {
	// verify : https://atcoder.jp/contests/abc254/tasks/abc254_e

	int n = sz(g);

	dist.clear(); // スタートからの最短距離を保持するテーブル
	dist[st] = 0;
	if (D == 0) return;

	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(st);

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 探索済みの頂点なら何もしない．
			if (dist.count(t)) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			if (dist[t] < D) q.push(t);
		}
	}
}


//【幅優先探索（複数始点）】O(|V| + |E|)
/*
* グラフ g に対し始点集合を st として幅優先探索を行い，
* st から各頂点 s への最短経路長の最小値を dist[s] に格納する．
* s が st のいずれからも到達不能な頂点の場合は dist[s] = INF となる．
*/
void multi_bfs(const Graph& g, const vi& st, vi& dist) {
	// verify : https://atcoder.jp/contests/arc049/tasks/arc049_c

	int n = sz(g);

	dist = vi(n, INF); // スタートからの最短距離を保持するテーブル
	queue<int> q; // 次に探索する頂点を入れておくキュー

	repe(s, st) {
		dist[s] = 0;
		q.push(s);
	}

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			q.push(t);
		}
	}
}


//【トポロジカル探索（無向グラフ）】O(|V| + |E|)
/*
* 無向グラフ g に対し始点の集合 st から幅優先探索を行う．
* ただし各頂点 s からの「出口」が確定するまで先の探索は保留する．
* s の出口を out[s] に格納する．（終点なら -1, 未確定なら -2 とする）
*/
template <class G>
void topological_bfs_undirected(const G& g, const vi& st, vi& out) {
	// verify : https://codeforces.com/contest/1613/problem/E

	int n = sz(g);
	out = vi(n, -2);

	// deg[s] : 頂点 s の次数
	vi deg(n);
	rep(s, n) repe(t, g[s]) deg[t]++;

	// 次に探索する頂点を入れておくキュー
	queue<int> q;
	repe(s, st) q.push(s);

	while (!q.empty()) {
		auto s = q.front(); q.pop();

		// 出口が確定済みなら何もしない
		if (out[s] >= -1) continue;

		// s の次数が 0 なら s は終点
		if (deg[s] == 0) {
			out[s] = -1;
			continue;
		}

		// s の次数が 2 以上なら探索は保留
		if (deg[s] >= 2) continue;

		// s の次数は 1 なので出口 t が確定している．
		repe(t, g[s]) {
			if (out[t] != -2) continue;
			out[s] = t;

			// 頂点 s を取り除いて t の次数を更新し，t を探索する．
			deg[t]--;
			q.push(t);
			break;
		}
	}
}


//【トポロジカル探索】O(|V| + |E|)
/*
* 有向グラフ g に対し始点の集合 st から幅優先探索を行う．
* ただし各頂点 s への全ての入力辺を通るまで s 以降の探索は保留する．
* s の探索を何番目に終えたかを time[s] に格納する．（未探索なら -1 とする）
*/
void topological_bfs(const Graph& g, const vi& st, vi& time) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_4_A

	int n = sz(g);
	time = vi(n, -1);

	// in_deg[s] : 頂点 s の入次数
	vi in_deg(n);
	rep(s, n) repe(t, g[s]) in_deg[t]++;
	dump(in_deg);

	// 次に探索する頂点を入れておくキュー
	queue<int> q;
	repe(s, st) q.push(s);

	int now = 0;
	while (!q.empty()) {
		auto s = q.front(); q.pop();

		// 既に探索を終えた頂点なら何もしない
		if (time[s] != -1) continue;

		// s へ入ってくる辺がまだあるなら探索は保留
		if (in_deg[s] > 0) continue;

		// s から先の探索を始めた時刻を記録
		time[s] = now++;

		repe(t, g[s]) {
			// 頂点 s を取り除いて t の入次数を更新し，t を探索する．
			in_deg[t]--;
			q.push(t);
		}
	}
}


//【深さ優先探索】O(|V| + |E|)
/*
* グラフ g に対し始点を st として深さ優先探索を行い，通った頂点を順に seq に格納する．
* 一度訪れた頂点には，帰り道以外で再び訪れることはない．
*/
template <class G> void depth_first_search(G& g, int st, vi& seq) {
	// verify : https://atcoder.jp/contests/abc213/tasks/abc213_d

	int n = sz(g);
	seq.clear();

	// seen[s] : 頂点 s を探索済か
	vb seen(n);

	function<void(int, int)> dfs = [&](int s, int p) {
		// 頂点を訪れたことを記録
		seen[s] = true;

		// 行きがけ順の処理
		seq.push_back(s);

		repe(t, g[s]) {
			// 親へは戻らない．
			if (t == p) continue;

			// 探索済なら何もしない．
			if (seen[t]) continue;

			// 未探索の頂点を探索しにいく．
			dfs(t, s);

			// 通りがけ順（兼，最後は帰りがけ順）の処理
			seq.push_back(s);
		}

		// 帰りがけ順の処理を書くのはここ
	};

	// st を始点として DFS を行う．
	dfs(st, -1);
}


//【バックトラッキング】O(d^|V|) （d = max deg(v[i]) ）
/*
* グラフ g に対し始点を st としてバックトラッキングを行い，
* i 番目に見つけた単純パスを頂点の列として path[i] に格納する．
*/
template <class G>
void back_tracking(G& g, int st, vvi& path) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_bt

	int n = sz(g);
	path.clear();
	vi seq; // 訪れた頂点の列

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// s を訪れたことを記録
		seen[s] = true;
		seq.push_back(s);

		// 単純パスを記録
		path.push_back(seq);

		repe(t, g[s]) {
			// 探索済なら何もしない．
			if (seen[t]) continue;

			// 未探索の頂点を探索しにいく．
			dfs(t);
		}

		// s を訪れた記録を削除
		seen[s] = false;
		seq.pop_back();
	};

	// st を始点として再帰関数を呼び出す．
	dfs(st);
}


