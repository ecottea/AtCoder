#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ ■■■■■



//【幅優先探索】O(|E|)
/*
* グラフ g に対し，始点を start として幅優先探索を行い，
* start から各頂点 i への最短経路長を dist[i] に格納する．
* i が start から到達不能な頂点の場合は dist[i] = -1 となる．
*/
void breadth_first_search(Graph& g, int start, vi& dist) {
	int n = sz(g);
	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[start] = 0;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(start);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) {
				// 発見済みの頂点なので何もしない．
				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
}


//【深さ優先探索】O(|E|)
/*
* グラフ g に対し，始点を s として深さ優先探索を行い，
* 行きがけ順に pre(i)，帰りがけ順に post(i) を実行する．
*/
template <class G>
void depth_first_search(G& g, int s, function<void(int)>& pre, function<void(int)>& post) {
	int n = sz(g);

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// 頂点を訪れたことを記録
		seen[s] = true;

		// 行きがけ順の処理
		pre(s);

		for (auto t : g[s]) {
			// 未探索の頂点を探索しにいく．
			if (!seen[t]) {
				dfs(t);
			}
		}

		// 帰りがけ順の処理
		post(s);

		//visited[s] = false; // これをつけるとバックトラッキングになる．
	};

	// s を始点として再帰関数を呼び出す．
	dfs(s);
}


//【トポロジカルソート】O(|V| + |E|)
/*
* 有向グラフ g をトポロジカルソートした結果を seq に返す．
* g[i] は頂点 i からの有向辺をもつ頂点のリスト．
* g が閉路を持っている場合は失敗し，seq.size() < g.size() となる．
*/
void topological_sort(Graph& g, vi& seq) {
	int n = (int)g.size();

	// 入次数を求めておく．
	vi in_degree(n);
	rep(i, n) {
		for (auto t : g[i]) {
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

		for (auto t : g[v]) {
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
void strongly_connected_component_decomposition(Graph& g, Graph& scc) {
	int n = (int)g.size();

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) {
		for (auto t : g[s]) {
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

		for (auto t : g[s]) {
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

		for (auto t : g_rev[s]) {
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


