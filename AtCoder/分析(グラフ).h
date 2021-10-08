#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの性質の分析 ■■■■■



//【トポロジカルソート】O(|V| + |E|)
/*
* 有向グラフ g をトポロジカルソートした結果を seq に返す．
* g が閉路を持っている場合は失敗し，seq.size() < g.size() となる．
*/
void topological_sort(Graph& g, vi& seq) {
	int n = sz(g);

	// 入次数を求めておく．
	vi in_degree(n);
	rep(i, n) {
		repe(t, g[i]) {
			in_degree[t]++;
		}
	}

	// 入次数が 0 の頂点から順に取り除いていく．
	queue<int> q;
	rep(i, n) {
		if (in_degree[i] == 0) {
			q.push(i);
		}
	}

	seq.clear();
	while (!q.empty()) {
		auto v = q.front();
		q.pop();

		// 入次数が 0 の頂点を見つけ結果に格納する．
		seq.push_back(v);

		repe(t, g[v]) {
			// 頂点 t を取り除き，入次数を更新する．
			in_degree[t]--;

			// 新たに入次数 0 の頂点が生まれたらキューに追加する．
			if (in_degree[t] == 0) {
				q.push(t);
			}
		}
	}
}


//【強連結成分分解】O(|V| + |E|)
/*
* 有向グラフ g を強連結成分分解し，トポロジカルソートされた結果を scc に返す．
* scc[i] は i 番目の強連結成分の頂点からなるリストである．
*/
void strongly_connected_component_decomposition(Graph& g, vvi& scc) {
	// 参考 : https://hkawabata.github.io/technical-note/note/Algorithm/graph/scc.html

	int n = sz(g);

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) {
		repe(t, g[s]) {
			g_rev[t].push_back(s);
		}
	}

	// 各頂点の状態（0:未探索，1:順探索済かつ未逆探索，2:逆探索済）
	vi status(n, 0);


	// step1: まず順探索（深さ優先）を行い，結果をスタックに格納する．

	// 深さ優先の順探索で見つかった順に頂点を記録するスタック
	stack<int> stk;

	// 順探索用の再帰関数
	function<void(int)> trace = [&](int s) {
		// 状態を順探索済かつ未逆探索（1）にする．
		status[s] = 1;

		repe(t, g[s]) {
			// 未探索の頂点を探索しにいく．
			if (status[t] == 0) {
				trace(t);
			}
		}

		// 先の探索が済んだら自身を記録する（深さ優先探索）
		stk.push(s);
	};

	rep(i, n) {
		// 未探索の頂点を見つけたら探索する．
		if (status[i] == 0) {
			trace(i);
		}
	}


	// step2: 次に逆探索を行い，強連結成分を確定する．

	// 逆探索用の再帰関数
	function<void(int)> trace_rev = [&](int s) {
		// 状態を逆探索済（2）にする．
		status[s] = 2;

		repe(t, g_rev[s]) {
			// 未逆探索の頂点を探索しにいく．
			if (status[t] == 1) {
				trace_rev(t);
			}
		}

		// 先の探索が済んだら自身を強連結成分の一員として記録する．
		scc.rbegin()->push_back(s);
	};

	while (!stk.empty()) {
		auto v = stk.top();
		stk.pop();

		// 新しい強連結成分を見つけたらそれをなぞりに行く．
		if (status[v] == 1) {
			scc.push_back(vi());
			trace_rev(v);
		}
	}
}


//【頂点の縮約】O(|V| + |E| log |V|)
/*
* グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として
* 縮約したグラフを gc に格納する．
*/
void vertex_contraction(const Graph& g, const vvi& p, Graph& gc) {
	int n = sz(g);
	int m = sz(p);

	// id[v] : 頂点 v の属する成分
	vi id(n);
	rep(i, m) {
		repe(v, p[i]) {
			id[v] = i;
		}
	}

	// 多重辺や自己ループを防ぐため一旦辺の集合を set でもつ．
	vector<set<int>> gc_set(m);
	rep(s, n) {
		repe(t, g[s]) {
			gc_set[id[s]].insert(id[t]);
		}
		gc_set[id[s]].erase(id[s]);
	}

	// 結果の格納
	gc = Graph(m);
	rep(s, m) {
		repe(t, gc_set[s]) {
			gc[s].push_back(t);
		}
	}
}


//【有向グラフの閉路分割】O(|V| + |E|) 
/*
* 有向グラフ g をいくつかの単純閉路に分割する．
*
* g : 有向グラフ
* cycles[i] : 検出した i 番目の閉路の頂点番号を順に格納したリスト
*/
bool cycle_partition(const Graph& g_, vvi& cycles) {
	int n = sz(g_);
	cycles.clear();

	// 辺を逆向きにしつつ，削除できるようスタックで辺をもつ
	vector<stack<int>> g(n);
	rep(s, n) {
		repe(t, g_[s]) {
			g[t].push(s);
		}
	}

	// r : 開始頂点，戻り値 : 成功か
	function<bool(int)> find_cycle = [&](int r) {
		stack<int> path; // 途中で通った頂点の列
		vb seen(n);

		int s = r;

		// r に戻ってくるまで
		while (true) {
			// s に来たことを記録
			path.push(s);
			seen[s] = true;

			// 行き止まりになったら失敗
			if (g[s].empty()) {
				return false;
			}

			// t : 次に進む予定の頂点
			int t = g[s].top();
			g[s].pop();

			// 閉路を検出した場合
			if (seen[t]) {
				// 閉路を逆順に記録する．予め逆順にしておいたので正順に記録できる．
				cycles.push_back(vi({ t }));
				while (path.top() != t) {
					int v = path.top();
					path.pop();
					seen[v] = false;

					cycles.rbegin()->push_back(v);
				}
				path.pop();
				seen[t] = false;

				// 開始頂点 r に戻ってきたら終了
				if (t == r) {
					return true;
				}
			}

			// 次の頂点へ進む
			s = t;
		}
	};

	// 各頂点 s について
	rep(s, n) {
		// 既になぞった連結成分に属する頂点なら何もしない．
		if (g[s].empty()) {
			continue;
		}

		// s から始まる閉路を探す．閉路分割に失敗したら false を返す．
		if (!find_cycle(s)) {
			return false;
		}
	}

	return true;
}


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


