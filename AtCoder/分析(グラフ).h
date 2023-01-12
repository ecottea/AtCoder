#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "DAG.h"
// ■■■■■ グラフの性質の分析 ■■■■■


//【連結成分分解】O(|V| + |E|)
/*
* 無向グラフ g を連結成分分解し，連結成分のリストを返す．
*/
vvi connected_component(const Graph& g) {
	// verify : https://atcoder.jp/contests/arc032/tasks/arc032_2

	int n = sz(g);
	vvi ccs;

	vb seen(n);

	function<void(int, int)> dfs = [&](int s, int p) {
		if (seen[s]) return;
		seen[s] = true;

		ccs.rbegin()->push_back(s);

		repe(t, g[s]) {
			if (t == p) continue;

			dfs(t, s);
		}
	};

	// 適当な点を始点として DFS を行う．
	rep(s, n) {
		if (seen[s]) continue;

		ccs.push_back(vi());
		dfs(s, -1);
	}

	return ccs;
}


//【トポロジカルソート】O(|V| + |E|)
/*
* DAG g をトポロジカルソートした結果の i 番目の頂点を seq[i] に格納し seq を返す．
* g が DAG でない場合は空リストを返す．
*
*（葉からの幅優先探索）
*/
vi topological_sort(const Graph& g) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_4_B

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

	vi seq;
	while (!q.empty()) {
		auto s = q.front(); q.pop();

		// 入次数が 0 の頂点を見つけ結果に格納する．
		seq.push_back(s);

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の入次数を更新する．
			in_degree[t]--;

			// 新たに入次数 0 の頂点が生まれたらキューに追加する．
			if (in_degree[t] == 0) {
				q.push(t);
			}
		}
	}

	return sz(seq) == n ? seq : vi();
}


//【強連結成分分解】O(|V| + |E|)
/*
* 有向グラフ g を強連結成分分解し，強連結成分をトポロジカルソート順に格納したリストを返す．
*/
vvi strongly_connected_component(const Graph& g) {
	// 参考 : https://hkawabata.github.io/technical-note/note/Algorithm/graph/scc.html
	// verify : https://judge.yosupo.jp/problem/scc

	int n = sz(g);
	vvi ccs;

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) repe(t, g[s]) g_rev[t].push_back(s);

	// 各頂点の状態（0:未探索，1:順探索済かつ未逆探索，2:逆探索済）
	vi status(n, 0);


	// (step1): まず順探索（深さ優先）を行い，結果をスタックに格納する．

	// 深さ優先の順探索で見つかった順に頂点を記録するスタック
	stack<int> stk;

	// 順探索用の再帰関数
	function<void(int)> trace = [&](int s) {
		// 状態を順探索済かつ未逆探索（1）にする．
		status[s] = 1;

		repe(t, g[s]) {
			// 未探索の頂点を探索しにいく．
			if (status[t] == 0) trace(t);
		}

		// 先の探索が済んだら自身を記録する（深さ優先探索）
		stk.push(s);
	};

	rep(i, n) {
		// 未探索の頂点を見つけたら探索する．
		if (status[i] == 0) trace(i);
	}


	// (step2): 次に逆探索を行い，強連結成分を確定する．

	// 逆探索用の再帰関数
	function<void(int)> trace_rev = [&](int s) {
		// 状態を逆探索済（2）にする．
		status[s] = 2;

		repe(t, g_rev[s]) {
			// 未逆探索の頂点を探索しにいく．
			if (status[t] == 1) trace_rev(t);
		}

		// 先の探索が済んだら自身を強連結成分の一員として記録する．
		ccs.rbegin()->push_back(s);
	};

	while (!stk.empty()) {
		auto v = stk.top();
		stk.pop();

		// 新しい強連結成分を見つけたらそれをなぞりに行く．
		if (status[v] == 1) {
			ccs.push_back(vi());
			trace_rev(v);
		}
	}

	return ccs;
}


//【有向グラフの閉路分割】O(|V| + |E|) 
/*
* 有向グラフ g をいくつかの単純閉路に分割する（失敗したら false を返す）
*
* g : 有向グラフ
* cycles[i] : 検出した i 番目の閉路の頂点番号を順に格納したリスト
*/
bool directed_cycle_partition(const Graph& g_, vvi& cycles) {
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
		if (g[s].empty()) continue;

		// s から始まる閉路を探す．閉路分割に失敗したら false を返す．
		if (!find_cycle(s)) return false;
	}

	return true;
}


//【無向グラフの閉路抽出】O(|V| + |E|)
/*
* 無向グラフ g に単純閉路があれば頂点を順に vs に，辺を順に es に格納し，その長さを返す（無ければ -1）
* vs[0] から出て vs[1] に入る辺を es[0] とする．
*/
template <class E>
int cycle_detection(const vector<vector<E>>& g, vi& vs, vector<E>* es = nullptr) {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	int n = sz(g);

	vb seen(n);
	vs.clear();
	if (es != nullptr) es->clear();

	// s : 注目頂点，p : 親
	// 戻り値 : 検出した閉路の末端（-1: 未検出，-2: 抽出完了）
	function<int(int, int)> dfs = [&](int s, int p) {
		// 既に訪れたことのある頂点に辿り着いたら閉路を検出したことになる．
		if (seen[s]) {
			vs.push_back(s);
			return s;
		}
		seen[s] = true;

		// s から辿れる頂点 t それぞれについて
		repe(t, g[s]) {
			// 親には戻らない（長さ 2 は閉路と認めない）
			if (t == p) continue;

			// t に対して深さ優先探索を行う．
			auto end = dfs(t, s);

			// 閉路が検出できなかったなら何もせず次の t を考える．
			if (end == -1) continue;

			// s が検出した閉路の末端であれば，閉路の記録をここで終わる．
			if (end == s || end == -2) {
				if (es != nullptr && end == s) es->push_back(t);
				return -2;
			}

			// 検出した閉路を逆順に記録していく．
			if (end >= 0) {
				vs.push_back(s);
				if (es != nullptr) es->push_back(t);
			}

			return end;
		}

		return -1;
	};

	// 各頂点 v について
	rep(v, n) {
		// 既になぞった連結成分に属する頂点なら何もしない．
		if (seen[v]) continue;

		// v から深さ優先探索を始める．
		int end = dfs(v, v);

		// 閉路を検出していたら終了．
		if (end != -1) {
			if (es != nullptr) {
				auto e = es->back();
				es->pop_back();
				reverse(all(vs));
				reverse(all(*es));
				es->push_back(e);
			}
			return sz(vs);
		}
	}

	return -1;
}


//【有向グラフの閉路抽出】O(|V| + |E|)
/*
* 有向グラフ g の単純閉路を何か 1 つ見つける．
*
* g : 有向グラフ
* cycle : 検出した閉路の頂点番号を順に格納したリスト（閉路なしなら空リスト）
*
* 利用：【強連結成分分解】
*/
template <class G>
void directed_cycle_detection(const G& g, vi& cycle) {
	// verify : https://judge.yosupo.jp/problem/cycle_detection

	int n = sz(g);
	cycle.clear();

	// 注目している強連結成分に含まれる頂点の集合
	set<int> valid;

	function<void(void)> ikiatari_battari = [&]() {
		vb seen(n);

		// 深さ優先探索用の関数
		// s : 注目頂点
		// 戻り値 : 逆順に検出した閉路の末端（-1: 未検出，-2: 抽出完了）
		function<int(int)> dfs = [&](int s) {
			// 注目している強連結成分に含まれる頂点でなければすぐに帰る．
			if (!valid.count(s)) return -1;

			// 既に訪れたことのある頂点に辿り着いたら閉路を検出したことになる．
			if (seen[s]) {
				cycle.push_back(s);
				return s;
			}
			seen[s] = true;

			// s から辿れる頂点 t それぞれについて
			repe(t, g[s]) {
				// t に対して深さ優先探索を行う．
				auto end = dfs(t);

				// 閉路が検出できなかったなら何もせず次の t を考える．
				if (end == -1) continue;

				// s が検出した閉路の末端であれば，閉路の記録をここで終わる．
				if (end == s || end == -2) return -2;

				// 検出した閉路を逆順に記録していく．
				if (end >= 0) cycle.push_back(s);

				return end;
			}

			return -1;
		};

		// 各頂点 v について
		rep(v, n) {
			// 既になぞった連結成分に属する頂点なら何もしない．
			if (seen[v]) continue;

			// v から深さ優先探索を始める．
			int end = dfs(v);

			// 閉路を検出していたら終了．
			if (end != -1) {
				// 逆順に検出しているので正順に戻す．
				reverse(all(cycle));

				return;
			}
		}
	};

	// まず強連結成分分解する．
	vvi scc;
	strongly_connected_component(g, scc);

	// 各強連結成分 vs について
	repe(vs, scc) {
		// 大きさ 2 以上の強連結成分 vs があれば閉路がある．
		if (sz(vs) > 1) {
			// 通っても良い頂点の集合に vs の頂点を記録する．
			repe(v, vs) valid.insert(v);

			// vs 内なら行き止まりがないので，行きあたりばったりで閉路検出ができる．
			ikiatari_battari();
			return;
		}
	}
}


//【二部グラフ判定】O(|V| + |E|)
/*
* 連結無向グラフが二部グラフかどうか判定する．
* 二部グラフならその彩色例を col に格納する（色は 0, 1 で表す）
*/
bool bipartite_graphQ(const Graph& g, vi& col) {
	// verify : https://atcoder.jp/contests/code-festival-2017-qualb/tasks/code_festival_2017_qualb_c

	int n = sz(g);

	// 頂点の色（0,1 は色を，-1 は未探索を表す）
	col = vi(n, -1);

	// 再帰用の関数
	function<bool(int)> dfs = [&](int s) {
		repe(t, g[s]) {
			// 未彩色の頂点の場合
			if (col[t] == -1) {
				// s と異なる色で t を彩色する．
				col[t] = 1 - col[s];

				// t から先を彩色しにいき，二部グラフでないならすぐに帰る．
				if (!dfs(t)) return false;
			}
			// 彩色済の頂点の場合
			else {
				// s と t が同色だったら二部グラフではないのですぐに帰る．
				if (col[t] == col[s]) return false;
			}
		}

		// ここまで来たなら見た範囲は二部グラフである．
		return true;
	};

	// 0 を始点として再帰関数を呼び出す．
	col[0] = 0;
	return dfs(0);
}


//【二部グラフ判定（非連結）】O(|V| + |E|)
/*
* 無向グラフを連結成分分解した結果を連結成分の頂点リストとして cc[i] に格納し，
* i 番目の連結成分 cc[i] が二部グラフかどうかを b[i] に格納する．
* 二部グラフの部分についてはその彩色例を col に格納する（色は 0, 1 で表す）
*/
template <class G>
void bipartite_graphQ(const G& g, vvi& cc, vb& b, vi& col) {
	// veriy : https://atcoder.jp/contests/arc099/tasks/arc099_c

	int n = sz(g);
	cc.clear(); b.clear();

	// 頂点の色（0,1 は色を，-1 は未探索を表す）
	col = vi(n, -1);

	// 再帰用の関数
	function<bool(int)> dfs = [&](int s) {
		cc.rbegin()->push_back(s);
		bool is_bipartite = true;

		repe(t, g[s]) {
			// 未彩色の頂点の場合
			if (col[t] == -1) {
				// s と異なる色で t を彩色する．
				col[t] = 1 - col[s];

				is_bipartite &= dfs(t);
			}
			// 彩色済の頂点の場合
			else {
				// s と t が同色だったら二部グラフではない．
				if (col[t] == col[s]) is_bipartite = false;
			}
		}

		return is_bipartite;
	};

	// 連結成分に分解しつつ二部グラフかどうか判定する．
	rep(s, n) {
		if (col[s] != -1) continue;

		col[s] = 0;
		cc.push_back(vi());
		b.push_back(dfs(s));
	}
}


//【グラフの関節点と橋】O(|V| + |E|)
/*
* 無向グラフ g の関節点のリストを a に，橋のリストを b に格納する．
*	関節点：その頂点を取り除くとグラフの連結成分が 1 つ増える頂点
*	橋：その辺を取り除くとグラフの連結成分が 1 つ増える辺
*
* a[i] : i 番目に見つけた関節点の頂点番号
* b[i] = {s, e} : i 番目に見つけた橋の始点が s，終点への辺が e
*/
template <class E>
void lowlink(const vector<vector<E>>& g, vi* a = nullptr, vector<pair<int, E>>* b = nullptr) {
	// 参考 : https://algo-logic.info/articulation-points/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_A
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_B

	int n = sz(g);
	if (a != nullptr) a->clear();
	if (b != nullptr) b->clear();

	// e_cnt[s * n + t] : 頂点 s, t を結ぶ辺の本数
	unordered_map<ll, int> e_cnt;
	rep(s, n) repe(t, g[s]) e_cnt[(ll)s * n + t]++;

	// in[s] : DFS で頂点 s を何番目に探索したか
	// low[s] : s から DFS 木を逆走せず後退辺を高々 1 回用いて到達できる頂点 t についての min in[t]
	//			後退辺とは，DFS でなぞられなかった g の辺のことをいう．
	vi in(n), low(n); vb seen(n);

	int time = 0;
	int r; // 暫定的な根

	// in, low を定める再帰用の関数
	function<void(int, int)> dfs = [&](int s, int p) {
		// s を最初に訪れた
		in[s] = time++;
		low[s] = in[s];
		seen[s] = true;

		bool is_ap = false; // 関節点か
		int ccnt = 0; // 子の個数

		repe(t, g[s]) {
			// 親に戻る辺と自己ループは通らない．
			//	自己ループは連結性に影響を与えないので無視できる
			if (t == p || t == s) continue;

			// t を既に訪れていた場合
			if (seen[t]) {
				// s→t または t→s は後退辺なので in[t] で low[s] を更新する．
				//	s→t が後退辺のとき：
				//		t から DFS 木の辺を辿って他の頂点 v に行けたとしても，
				//		必ず in[t] < in[v] となっているので無視できる．
				//	t→s が後退辺のとき：
				//		s→t は DFS 木に対するショートカットとなるので，
				//		必ず in[s] < in[t] となり更新は起こらないので安心．
				chmin(low[s], in[t]);
			}
			// t をまだ訪れていない場合
			else {
				// 再帰的になぞりにいく．
				dfs(t, s);

				// s→t は DFS 木の辺なので low[t] で low[s] を更新する．
				chmin(low[s], low[t]);

				// 橋であれば記録する（ただし多重辺は橋にはなりえない）
				if (in[s] < low[t] && e_cnt[(ll)s * n + t] == 1) {
					if (b != nullptr) b->push_back({ s, t });
				}

				// 関節点かどうかの判定用
				is_ap |= (in[s] <= low[t]);
				ccnt++;
			}
		}

		// 根の場合の例外処理
		if (s == r) is_ap = (ccnt >= 2);

		// 関節点であれば記録する．
		if (is_ap) if (a != nullptr) a->push_back(s);
	};

	// 適当な点を根（始点）として DFS を行う．
	rep(s, n) {
		if (seen[s]) continue;

		r = s;
		dfs(r, -1);
	}
}


//【二辺連結成分分解】O(|V| + |E|)
/*
* 無向グラフ g を二辺連結成分分解し，二辺連結成分の頂点集合のリストを返す．
* 二辺連結成分：任意の 1 辺を取り除いても連結な部分グラフ
*
* 利用：【グラフの関節点と橋】,【連結成分分解】
*/
vvi two_edge_connected_component(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/two_edge_connected_components

	//【方法】
	// 橋を全て除去してから連結成分分解すればよい．

	int n = sz(g);

	vector<pii> b;
	lowlink(g, nullptr, &b);

	unordered_set<ll> bridges;
	repe(e, b) {
		int s, t;
		tie(s, t) = e;

		bridges.insert((ll)s * n + t);
		bridges.insert((ll)t * n + s);
	}

	Graph g2(n);
	rep(s, n) repe(t, g[s]) {
		if (!bridges.count((ll)s * n + t)) {
			g2[s].push_back(t);
		}
	}

	return connected_component(g2);
}


//【通行可能性と壁の連結性の双対性】
/*
* L から R まで通行可能[不可能] ⇔ U 側の壁と D 側の壁が非連結[連結]
* 
* verify : https://atcoder.jp/contests/abc181/tasks/abc181_f
*/


//【DFS 木の性質】
/*
* 無向グラフ g の DFS 木 t に含まれない全ての辺 u-v について，
* u と v は t において先祖と子孫の関係にある．
* 
* verify : https://atcoder.jp/contests/abc251/tasks/abc251_f
*/


//【BFS 木の性質】
/*
* 無向グラフ g の BFS 木 t に含まれない全ての辺 u-v について，
* u と v は t において先祖と子孫の関係にない．
*
* verify : https://atcoder.jp/contests/abc251/tasks/abc251_f
*/


//【連結性を保った頂点消去】
/*
* 連結無向グラフ g について，連結性を保ったまま頂点を消去していくには，
* g の辺を用いて適当な頂点を根とする木を作り，深さ降順に頂点を消去していけばいい．
* 
* verify : https://atcoder.jp/contests/arc119/tasks/arc119_d
*/


