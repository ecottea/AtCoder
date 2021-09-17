#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の判定問題 ■■■■■



//【無向グラフの閉路検出】O(|V| + |E|)
/*
* 無向グラフ g の閉路を何か 1 つ見つける．
*
* g : 無向グラフ
* cycle : 検出した閉路の頂点番号を順に格納したリスト（閉路なしなら空リスト）
*/
template <class G>
void cycle_detection(const G& g, vi& cycle) {
	int n = sz(g);

	vb seen(n);
	cycle.clear();

	// 深さ優先探索用の関数
	// s : 注目頂点，p : 親
	// 戻り値 : 検出した閉路の末端（-1: 未検出，-2: 抽出完了）
	function<int(int, int)> dfs = [&](int s, int p) {
		// 既に訪れたことのある頂点に辿り着いたら閉路を検出したことになる．
		if (seen[s]) {
			cycle.push_back(s);
			return s;
		}
		seen[s] = true;

		// s から辿れる頂点 t それぞれについて
		repe(t, g[s]) {
			// 親には戻らない（長さ 2 は閉路と認めない）
			if (t == p) {
				continue;
			}

			// t に対して深さ優先探索を行う．
			auto end = dfs(t, s);

			// 閉路が検出できなかったなら何もせず次の t を考える．
			if (end == -1) {
				continue;
			}

			// s が検出した閉路の末端であれば，閉路の記録をここで終わる．
			if (end == s || end == -2) {
				return -2;
			}

			// 検出した閉路を逆順に記録していく．
			if (end >= 0) {
				cycle.push_back(s);
			}

			return end;
		}

		return -1;
	};

	// 各頂点 v について
	rep(v, n) {
		// 既になぞった連結成分に属する頂点なら何もしない．
		if (seen[v]) {
			continue;
		}

		// v から深さ優先探索を始める．
		int end = dfs(v, v);

		// 閉路を検出していたら終了．
		if (end != -1) {
			return;
		}
	}
}


//【二部グラフ判定】O(|E|)
/*
* 連結無向グラフが二部グラフかどうか判定する．
* 二部グラフならその彩色例を col に格納する（色は 0, 1 で表す）
*/
bool bipartite_graphQ(const Graph& g, vi& col) {
	int n = sz(g);

	// 頂点の色（0,1 は色を，-1 は未探索を表す）
	col = vector<int>(n, -1);

	// 再帰用の関数
	function<bool(int)> dfs = [&](int s) {
		for (auto t : g[s]) {
			// 未彩色の頂点の場合
			if (col[t] == -1) {
				// s と異なる色で t を彩色する．
				col[t] = 1 - col[s];

				// t から先を彩色しにいく．
				if (!dfs(t)) {
					return false;
				}
			}
			// 彩色済の頂点の場合
			else {
				// s と t が同色だったら二部グラフではない．
				if (col[t] == col[s]) {
					return false;
				}
			}
		}
		return true;
	};

	// 0 を始点として再帰関数を呼び出す．
	return dfs(0);
}


//【グラフの関節点と橋】O(|V| + |E|)
/*
* グラフ g の関節点のリストを a に，橋のリストを b に格納する．
*
* a[i] : i 番目に見つけた関節点の頂点番号
* b[i] = {s, e} : i 番目に見つけた橋の始点が s，終点への辺が e
*/
template <class E>
void lowlink(const vector<vector<E>>& g, vi& a, vector<pair<int, E>>& b) {
	// 参考 : https://algo-logic.info/articulation-points/

	int n = sz(g);

	// in[s] : DFS で頂点 s を何番目に探索したか
	// low[s] : s から後退辺を高々 1 回用いて到達できる頂点 t についての min in[t]
	// （後退辺とは，DFS でなぞられなかった g の辺のことをいう）
	vi in(n), low(n);
	vb seen(n);

	a.clear();
	b.clear();
	int time = 0;

	// in, low を定める再帰用の関数
	function<void(int, int)> dfs = [&](int s, int p) {
		// s を最初に訪れた
		in[s] = time++;
		low[s] = in[s];
		seen[s] = true;

		bool ap = false;
		int ccnt = 0;
		for (auto t : g[s]) {
			// 親には戻らない．
			if (t == p) {
				continue;
			}

			// t を既に訪れていた場合
			if (seen[t]) {
				// 後退辺なので in[t] で low[s] を更新する．
				chmin(low[s], in[t]);
			}
			// t をまだ訪れていない場合
			else {
				// 再帰的になぞりにいく．
				dfs(t, s);

				// DFS 木の辺なので low[t] で low[s] を更新する．
				chmin(low[s], low[t]);

				// 橋であれば記録する．
				if (in[s] < low[t]) {
					b.push_back({ s, t });
				}

				// 関節点かどうかの判定用
				ap |= (in[s] <= low[t]);
				ccnt++;
			}
		}

		// 根の場合の例外処理
		if (s == 0) {
			ap = (ccnt >= 2);
		}

		// 関節点であれば記録する．
		if (ap) {
			a.push_back(s);
		}
	};

	// 適当な点を始点として DFS を行う．
	dfs(0, -1);
}


