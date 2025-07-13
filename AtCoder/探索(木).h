#pragma once
#include "header.h"
#include "構造(木).h"
// ■■■■■ 木の探索 ■■■■■


//【深さ優先探索】O(n)
/*
* 木 g に対し始点を st として深さ優先探索を行い，通った頂点を順に格納したリストを返す．
*/
template <class G>
vi tree_dfs(const G& g, int st) {
	int n = sz(g);
	vi seq;

	// 再帰用の関数
	function<void(int, int)> dfs = [&](int s, int p) {
		// 行きがけ順の処理を書くのはここ
		seq.push_back(s);

		repe(t, g[s]) {
			// 親には戻らない．
			if (t == p) continue;

			// 未探索の頂点を探索しにいく．
			dfs(t, s);

			// 通りがけ順（兼，最後は帰りがけ順）の処理を書くのはここ
			seq.push_back(s);
		}

		// 帰りがけ順の処理を書くのはここ
	};

	// 根を始点として再帰関数を呼び出す．
	dfs(st, -1);

	return seq;
}


//【深さ優先探索（森）】O(n)
/*
* 森 g の各木に対し適当な始点から深さ優先探索を行い，通った頂点を順に seq に格納する．
* 一度訪れた頂点には，帰り道以外で再び訪れることはない．
*/
template <class G>
void forest_dfs(const G& g, int st, vi& seq) {
	// verify : https://atcoder.jp/contests/abc213/tasks/abc213_d

	int n = sz(g);
	seq.clear();

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// 頂点を訪れたことを記録
		seen[s] = true;

		// 行きがけ順の処理
		seq.push_back(s);

		repe(t, g[s]) {
			// 探索済なら何もしない．
			if (seen[t]) continue;

			// 未探索の頂点を探索しにいく．
			dfs(t);

			// 通りがけ順（兼，最後は帰りがけ順）の処理
			seq.push_back(s);
		}

		// 帰りがけ順の処理を書くのはここだが，今回は必要ない．
	};

	rep(s, n) {
		if (seen[s]) continue;

		dfs(s);
	}
}


//【二分木の深さ優先探索】O(n)
/*
* 二分木 bt を深さ優先でなぞりながら，
* 行きがけ順に pre(i)，通りがけ順に in(i)，帰りがけ順に post(i) を実行する．
*
*（再帰呼び出し）
*/
void traverse_binary_tree(Binary_Tree& bt,
	function<void(int)>& pre, function<void(int)>& in, function<void(int)>& post) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_C

	function<void(int)> dfs = [&](int s) {
		// 行きがけ順の処理
		pre(s);

		// 左の子があれば左の子をなぞりにいく．
		if (bt[s].left != -1) {
			dfs(bt[s].left);
		}

		// 通りがけ順の処理
		in(s);

		// 右の子があれば右の子をなぞりにいく．
		if (bt[s].right != -1) {
			dfs(bt[s].right);
		}

		// 帰りがけ順の処理
		post(s);
	};

	dfs(bt.root);
}


//【二分木の深さ優先探索（スタック）】O(n)
/*
* 二分木 bt を深さ優先でなぞりながら，
* 行きがけ順に pre(i)，通りがけ順に in(i) を実行する．
*
*（スタック）
*/
void traverse_binary_tree(Binary_Tree& bt, function<void(int)>& pre, function<void(int)>& in) {
	stack<int> st;
	int v = 0;

	while (v != -1) {
		pre(v); // 行きがけ順の処理

		// 左の子が居る場合
		if (bt[v].left != -1) {
			st.push(v);
			v = bt[v].left;
		}
		// 左の子が居ない場合
		else {
			in(v); // 通りがけ順の処理

			v = bt[v].right;
			while (!st.empty() && v == -1) {
				// anc : まだ通りがけ順の処理を行っていない最小祖先
				// anc まで一気に戻ってしまうので帰りがけ順の処理は不可能．
				int anc = st.top(); st.pop();

				in(anc); // 通りがけ順の処理

				v = bt[anc].right;
			}
		}
	}
}


