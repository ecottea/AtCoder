#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "変換(グラフ).h"
#include "DAG.h"
// ■■■■■ グラフの性質の分析 ■■■■■


//【連結成分分解】O(n + m)
/*
* 無向グラフ g を連結成分分解し，連結成分の頂点のリストを返す．
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


//【連結成分分解（補グラフ）】O(n + m)
/*
* 無向グラフ g の補グラフを連結成分分解し，連結成分のリストを返す．
*/
vvi connected_component_complement(const Graph& g) {
	// 参考 : https://atcoder.jp/contests/abc319/editorial/7120
	// verify : https://judge.yosupo.jp/problem/connected_components_of_complement_graph

	int n = sz(g);

	// rem : 未探索の頂点のリスト
	list<int> rem;
	rep(s, n) rem.push_back(s);

	// fb[s] : 注目頂点と s との間の移動が禁止されているか
	vb fb(n);

	// q : BFS 用のキュー
	queue<int> q;

	// ccs : 連結成分のリスト
	vvi ccs;

	// 未探索の頂点が残っている限り連結成分の抽出を続ける．
	while (!rem.empty()) {
		auto it = rem.begin();

		// ST : 連結成分抽出の始点
		int ST = *it;

		// cc : 抽出中の連結成分
		vi cc{ ST };

		// 探索待ちの頂点に ST を追加する．
		q.push(ST);

		// 未探索の頂点から ST を削除する．
		rem.erase(it);

		while (!q.empty()) {
			// s : 注目頂点
			auto s = q.front(); q.pop();

			// s に隣接する頂点の移動禁止フラグを立てる．
			repe(t, g[s]) fb[t] = true;

			// t : 未探索の頂点
			for (auto it = rem.begin(); it != rem.end(); ) {
				int t = *it;

				// t への移動が禁止されていれば何もしない
				if (fb[t]) {
					it++;
					continue;
				}

				// 連結成分に t を追加する．
				cc.push_back(t);

				// 探索待ちの頂点に t を追加する．
				q.push(t);

				// 未探索の頂点から t を削除する．
				it = rem.erase(it);
			}

			// s に隣接する頂点の移動禁止フラグを折る．
			repe(t, g[s]) fb[t] = false;
		}

		ccs.push_back(move(cc));
	}

	return ccs;
}


//【トポロジカルソート】O(n + m)
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
	rep(i, n) repe(t, g[i]) in_degree[t]++;

	// 入次数が 0 の頂点から順に取り除いていく．
	queue<int> q;
	rep(i, n) if (in_degree[i] == 0) q.push(i);

	vi seq;
	seq.reserve(n);

	while (!q.empty()) {
		auto s = q.front(); q.pop();

		// 入次数が 0 の頂点を見つけ結果に格納する．
		seq.push_back(s);

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の入次数を更新する．
			in_degree[t]--;

			// 新たに入次数 0 の頂点が生まれたらキューに追加する．
			if (in_degree[t] == 0) q.push(t);
		}
	}

	return sz(seq) == n ? seq : vi();
}


//【強連結成分分解】O(n + m)
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


//【閉路抽出（無向グラフ）】O(n + m)
/*
* 無向グラフ g に（長さ 3 以上の）単純閉路があれば，
* 頂点を順に vs に，辺を順に es に格納し，その長さを返す（無ければ -1）
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


//【閉路抽出（有向グラフ）】O(n + m)
/*
* 有向グラフ g から極大個数の単純閉路を抽出し，各閉路に含まれる頂点のリストを返す．
*/
template <class G>
vvi directed_cycles_detection(G g) {
	// verify : https://yukicoder.me/problems/no/1865

	int n = sz(g);

	vvi cycles;

	// 0 : 未探索，1 : 探索済，2 : 閉路の端点
	vi seen(n);

	// 0 : 探索モード，2 : 閉路抽出モード
	int tp = 0;

	function<void(int)> dfs = [&](int s) {
		seen[s] = 1;

		while (!g[s].empty()) {
			// 同じ辺を二度見ないように削除しておく．
			auto t = g[s].back(); g[s].pop_back();

			// 自己ループは単独の閉路として記録する．
			if (t == s) {
				cycles.push_back(vi{ t });
				continue;
			}

			// 探索済の頂点にたどり着いたなら閉路を検出できた．
			if (seen[t] == 1) {
				// t が閉路の端点であることを覚えておく．
				seen[t] = 2;

				// 閉路抽出モードに移行する．
				tp = 2;
				cycles.push_back(vi());
				cycles.back().push_back(t);
				break;
			}

			dfs(t);

			// 閉路抽出モードなら，探索は一休みして閉路を抽出する．
			if (tp == 2) {
				cycles.back().push_back(t);

				// 閉路の端点まで抽出しきったのなら探索モードに移行する．
				if (seen[s] == 2) {
					reverse(all(cycles.back()));
					seen[s] = 1;
					tp = 0;
					continue;
				}
				break;
			}
		}

		// 頂点に関しては二度以上見る必要があるのでバックトラッキングする．
		seen[s] = 0;
	};

	// 各頂点 s を通る閉路をできるかぎり抽出する．
	rep(s, n) while (!g[s].empty()) dfs(s);

	repea(c, cycles) c.push_back(c[0]);

	return cycles;
}


//【閉路抽出（有向グラフ，参照付き）】O(n + m)
/*
* 有向グラフ g から極大個数の単純閉路を抽出し，各閉路に含まれる辺のリストを返す．
*/
vvi directed_cycles_detection(IGraph g) {
	// verify : https://yukicoder.me/problems/no/2464

	int n = sz(g);

	vvi cycles;

	// 0 : 未探索，1 : 探索済，2 : 閉路の端点
	vi seen(n);

	// 0 : 探索モード，2 : 閉路抽出モード
	int tp = 0;

	function<void(int)> dfs = [&](int s) {
		seen[s] = 1;

		while (!g[s].empty()) {
			// 同じ辺を二度見ないように削除しておく．
			auto t = g[s].back(); g[s].pop_back();

			// 自己ループは単独の閉路として記録する．
			if (t == s) {
				cycles.push_back(vi{ t.id });
				continue;
			}

			// 探索済の頂点にたどり着いたなら閉路を検出できた．
			if (seen[t.to] == 1) {
				// t が閉路の端点であることを覚えておく．
				seen[t.to] = 2;

				// 閉路抽出モードに移行する．
				tp = 2;
				cycles.push_back(vi());
				cycles.back().push_back(t.id);
				break;
			}

			dfs(t.to);

			// 閉路抽出モードなら，探索は一休みして閉路を抽出する．
			if (tp == 2) {
				cycles.back().push_back(t.id);

				// 閉路の端点まで抽出しきったのなら探索モードに移行する．
				if (seen[s] == 2) {
					reverse(all(cycles.back()));
					seen[s] = 1;
					tp = 0;
					continue;
				}
				break;
			}
		}

		// 頂点に関しては二度以上見る必要があるのでバックトラッキングする．
		seen[s] = 0;
	};

	// 各頂点 s を通る閉路をできるかぎり抽出する．
	rep(s, n) while (!g[s].empty()) dfs(s);

	return cycles;
}


//【オイラー路（無向グラフ，参照付き）】O(n + m) 
/*
* 参照付き無向グラフ g のオイラー路を返す．（なければ空リスト）
* オイラー路は {頂点リスト, 辺番号リスト} の形で返す．
*/
pair<vi, vi> eulerian_trail(IGraph g) {
	// 参考 : https://twitter.com/snuke_/status/1268592276697042944
	// verify : https://judge.yosupo.jp/problem/eulerian_trail_undirected

	int n = sz(g), m = 0;

	// deg[s] : 頂点 s の次数
	vi deg(n);

	rep(s, n) repe(t, g[s]) {
		// 辺を二重に数えてしまうことを防ぐ．
		if (s <= t) {
			deg[s]++;
			deg[t]++;
			chmax(m, t.id);
		}
	}
	m++;

	// st : 始点
	int st = -1; int odd_cnt = 0;

	rep(s, n) {
		if (deg[s] & 1) {
			// 奇点がオイラー路の始点となる．
			st = s;
			odd_cnt++;
		}
	}

	// 奇点が 4 つ以上あるならオイラー路は存在しない．
	if (odd_cnt >= 4) return { vi(), vi() };

	// 始点に任意性がある場合，孤立点でない適当な頂点を始点にする．
	if (st == -1) {
		rep(s, n) if (deg[s] > 0) {
			st = s;
			break;
		}
	}

	// 孤立点しかない（辺が 0 本）の場合，頂点 0 で不動とする．
	if (st == -1) return { vi{0}, vi() };

	vi vs, es; vb seen(m);

	function<void(int)> dfs = [&](int s) {
		while (!g[s].empty()) {
			// e : 未探索の辺
			auto e = g[s].back();

			// 同じ辺を二度通らないよう削除しておく．
			g[s].pop_back();

			// 逆向きに通ったことがあるなら通らない．
			if (seen[e.id]) continue;
			seen[e.id] = true;

			dfs(e.to);

			// 帰りがけ順に通った頂点や辺を記録しておく．
			vs.push_back(e.to);
			es.push_back(e.id);
		}
	};

	// st を始点として DFS を行う．
	dfs(st);

	// 通っていない辺が残っているならば，非連結なのでオイラー路は存在しない．
	rep(s, n) if (!g[s].empty()) return { vi(), vi() };

	vs.push_back(st);

	reverse(all(vs));
	reverse(all(es));

	return { vs, es };
}


//【オイラー路（有向グラフ）】O(n + m) 
/*
* 有向グラフ g のオイラー路を成す頂点のリストを返す．（なければ空リスト）
*/
vi directed_eulerian_trail(Graph g) {
	// verify : https://atcoder.jp/contests/abc227/tasks/abc227_h
	// 参考 : https://twitter.com/snuke_/status/1268592276697042944

	int n = sz(g);

	// in_deg[s] : 頂点 s の入次数，out_deg[s] : 頂点 s の出次数
	vi in_deg(n), out_deg(n);

	rep(s, n) repe(t, g[s]) {
		out_deg[s]++;
		in_deg[t]++;
	}

	// st : 始点
	int st = -1;

	rep(s, n) {
		int dif = out_deg[s] - in_deg[s];

		// 入次数と出次数の差が 2 以上の頂点があればオイラー路は存在しない．
		if (dif >= 2) return vi();

		if (dif == 1) {
			// 入次数と出次数の差が 1 の頂点が複数あればオイラー路は存在しない．
			if (st != -1) return vi();

			// 入次数と出次数の差が 1 の頂点がオイラー路の始点となる．
			st = s;
		}
	}

	// 始点に任意性がある場合，孤立点でない適当な頂点を始点にする．
	if (st == -1) {
		rep(s, n) if (out_deg[s] > 0) {
			st = s;
			break;
		}
	}

	// 孤立点しかない（辺が 0 本）場合，頂点 0 で不動とする．
	if (st == -1) return vi{ 0 };

	vi vs, es;

	function<void(int)> dfs = [&](int s) {
		while (!g[s].empty()) {
			// t : 未探索の辺の先にある頂点
			auto t = g[s].back();

			// 同じ辺を二度通らないよう削除しておく．
			g[s].pop_back();

			dfs(t);

			// 帰りがけ順に通った頂点を記録しておく．
			vs.push_back(t);
		}
	};

	// st を始点として DFS を行う．
	dfs(st);

	// 通っていない辺が残っているならば，非連結なのでオイラー路は存在しない．
	rep(s, n) if (!g[s].empty()) return vi();

	vs.push_back(st);

	reverse(all(vs));

	return vs;
}


//【オイラー路（有向グラフ，参照付き）】O(n + m) 
/*
* 参照付き有向グラフ g のオイラー路を返す．（なければ空リスト）
* オイラー路は {頂点リスト, 辺番号リスト} の形で返す．
*/
pair<vi, vi> directed_eulerian_trail(IGraph g) {
	// 参考 : https://twitter.com/snuke_/status/1268592276697042944
	// verify : https://judge.yosupo.jp/problem/eulerian_trail_directed

	int n = sz(g);

	// in_deg[s] : 頂点 s の入次数，out_deg[s] : 頂点 s の出次数
	vi in_deg(n), out_deg(n);

	rep(s, n) repe(t, g[s]) {
		out_deg[s]++;
		in_deg[t]++;
	}

	// st : 始点
	int st = -1;

	rep(s, n) {
		int dif = out_deg[s] - in_deg[s];

		// 入次数と出次数の差が 2 以上の頂点があればオイラー路は存在しない．
		if (dif >= 2) return { vi(), vi() };

		if (dif == 1) {
			// 入次数と出次数の差が 1 の頂点が複数あればオイラー路は存在しない．
			if (st != -1) return { vi(), vi() };

			// 入次数と出次数の差が 1 の頂点がオイラー路の始点となる．
			st = s;
		}
	}

	// 始点に任意性がある場合，孤立点でない適当な頂点を始点にする．
	if (st == -1) {
		rep(s, n) if (out_deg[s] > 0) {
			st = s;
			break;
		}
	}

	// 孤立点しかない（辺が 0 本）場合，頂点 0 で不動とする．
	if (st == -1) return { vi{ 0 }, vi() };

	vi vs, es;

	function<void(int)> dfs = [&](int s) {
		while (!g[s].empty()) {
			// e : 未探索の辺
			auto e = g[s].back();

			// 同じ辺を二度通らないよう削除しておく．
			g[s].pop_back();

			dfs(e.to);

			// 帰りがけ順に通った頂点や辺を記録しておく．
			vs.push_back(e.to);
			es.push_back(e.id);
		}
	};

	// st を始点として DFS を行う．
	dfs(st);

	// 通っていない辺が残っているならば，非連結なのでオイラー路は存在しない．
	rep(s, n) if (!g[s].empty()) return { vi(), vi() };

	vs.push_back(st);

	reverse(all(vs));
	reverse(all(es));

	return { vs, es };
}


//【二部グラフ判定（連結）】O(n + m)
/*
* 連結無向グラフが二部グラフかどうか判定する．
* 二部グラフならその彩色例を col に格納する（色は 0, 1 で表す）
*/
template <class G>
bool bipartite_graphQ(const G& g, vi& col) {
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


//【二部グラフ判定（非連結）】O(n + m)
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


//【二部グラフ判定（補グラフ，非連結）】O(n + m)
/*
* 無向グラフ g の補グラフが二部グラフかどうか判定し彩色例を返す（色は 0, 1 で表す）
* g が二部グラフでないときは空リストを返す．
*/
vi bipartite_graphQ_complement(const Graph& g) {
	// verify : https://atcoder.jp/contests/agc067/tasks/agc067_a

	int n = sz(g);

	// rem : 未探索の頂点のリスト
	list<int> rem;
	rep(s, n) rem.push_back(s);

	// fb[s] : 注目頂点と s との間の移動が禁止されているか
	vb fb(n);

	// q : BFS 用のキュー
	queue<int> q;

	// col : g の頂点の彩色
	vi col(n, -1);

	// 未探索の頂点が残っている限り連結成分の抽出を続ける．
	while (!rem.empty()) {
		auto it = rem.begin();

		// ST : 連結成分内の探索の始点
		int ST = *it;
		col[ST] = 0;

		// 探索待ちの頂点に ST を追加する．
		q.push(ST);

		// 未探索の頂点から ST を削除する．
		rem.erase(it);

		while (!q.empty()) {
			// s : 注目頂点
			auto s = q.front(); q.pop();

			// s に隣接する頂点の移動禁止フラグを立てる．
			repe(t, g[s]) fb[t] = true;

			// t : 未探索の頂点
			for (auto it = rem.begin(); it != rem.end(); ) {
				int t = *it;
				
				// t への移動が禁止されていれば何もしない
				if (fb[t]) {
					it++;
					continue;
				}

				// ST から t までの距離 (mod 2) が確定する．
				col[t] = col[s] ^ 1;

				// 探索待ちの頂点に t を追加する．
				q.push(t);

				// 未探索の頂点から t を削除する．
				it = rem.erase(it);
			}

			// s に隣接する頂点の移動禁止フラグを折る．
			repe(t, g[s]) fb[t] = false;
		}
	}

	// cnt_v[c] : 色 c の頂点数
	vl cnt_v(2);
	rep(i, n) cnt_v[col[i]]++;

	// cnt_e2[c] : 色 c の頂点どうしを結ぶ辺の数 * 2
	vl cnt_e(2);
	rep(s, n) repe(t, g[s]) {
		if (col[s] == col[t]) cnt_e[col[s]]++;
	}

	// 同色頂点間を結ぶ辺全てが g に存在すれば OK
	rep(c, 2) {
		if (cnt_e[c] != cnt_v[c] * (cnt_v[c] - 1)) {
			col.clear();
		}
	}

	return col;
}


//【lowlink】
/*
* Lowlink(IGraph g) : O(n + m)
*	参照付き無向グラフ g（多重辺可，自己ループ可）で初期化する．
*
* bool articulation_pointQ(int s) : O(1)
*	頂点 s が関節点かを返す．
*	関節点：その頂点を取り除くとグラフの連結成分が増える頂点
*
* vi get_articulation_points() : O(n)
*	g の関節点の昇順リストを返す．
*
* bool bridgeQ(int j) : O(1)
*	辺 j が橋かを返す．
*	橋：その辺を取り除くとグラフの連結成分が増える辺
*
* vi get_bridges() : O(m)
*	g の橋の番号の昇順リストを返す．
*
* vvi get_two_edge_connected_components() : O(n + m)
*	g を二重辺連結成分分解し，二重辺連結成分の頂点集合のリストを返す．
*	二重辺連結成分：任意の 1 辺を取り除いても連結な部分グラフ
*
* vvi get_two_vertex_connected_components() : O(n + m)
*	g を二重頂点連結成分分解し，二重頂点連結成分の辺集合の番号のリストを返す．
*	二重頂点連結成分：任意の 1 頂点を取り除いても連結な極大部分グラフ
*	制約：g は自己ループをもたない．
*	注意：孤立点のみからなる二重頂点連結成分は辺をもたないので検出されない．
*
* bool connectedQ(int s, int t) : O(1)
*	頂点 s, t が連結かを返す．
*
* bool separated_by_bridgeQ(int s, int j, int t) : O(1)
*	頂点 s, t 間に橋 j が存在するかを返す．
*/
class Lowlink {
	// 参考 : https://kntychance.hatenablog.jp/entry/2022/09/16/161858

	int n, m;
	IGraph g;

	// in[s] : DFS で頂点 s を初めて探索した時刻（連結成分間の移動には INF 時間かかる）
	vl in;

	// low[s] : s から DFS 木を逆走せず後退辺を高々 1 回用いて到達できる頂点 t についての min in[t]
	//	後退辺とは，DFS でなぞられなかった g の辺のことをいう．
	vl low;

	// in_e[j] : DFS で辺 j を行きがけに探索した時刻
	// out_e[j] : DFS で辺 j を帰りがけに探索した時刻
	vl in_e, out_e;

	// is_ap[s] : 頂点 s が関節点か
	vb is_ap;

	// is_bg[j] : 辺 j が橋か
	vb is_bg;

public:
	// 参照付き無向グラフ g（多重辺可，自己ループ可）で初期化する．
	Lowlink(const IGraph& g) : n(sz(g)), m(-1), g(g), in(n, -1), low(n), is_ap(n) {
		// verify : https://atcoder.jp/contests/abc301/tasks/abc301_h

		rep(s, n) repe(t, g[s]) chmax(m, t.id);
		m++;
		in_e.resize(m), out_e.resize(m);
		is_bg.assign(m, false);

		int rt; // 走査中の連結成分の根
		ll time = 0; // 現在時刻

		// DFS 木をなぞる．
		function<void(int, int)> dfs = [&](int s, int id) {
			// s を最初に訪れた
			in[s] = low[s] = time++;

			int child_cnt = 0; // DFS 木における子の個数

			repe(t, g[s]) {
				// 戻る辺と自己ループは通らない．
				//（自己ループは連結性に影響を与えないので無視できる）
				if (t.id == id || t == s) continue;

				// t を既に訪れていた場合
				if (in[t] != -1) {
					// s→t が後退辺のとき：
					//		t から DFS 木の辺を辿って他の頂点 v に行けたとしても
					//		必ず in[t] < in[v] となっているので low[s] = in[t] で確定．
					// t→s が後退辺のとき：
					// 		s→t は DFS 木に対するショートカットとなるので
					//		必ず low[s] ≦ in[s] < in[t] となるから更新不要．
					// chmin を使えば両方同時に対応可能である．
					chmin(low[s], in[t]);
				}
				// t をまだ訪れていない場合
				else {
					// 再帰的になぞりにいく．
					in_e[t.id] = time++;
					dfs(t, t.id);
					out_e[t.id] = time++;

					// s→t は DFS 木の辺なので low[t] で low[s] を更新する．
					chmin(low[s], low[t]);

					// s→t を渡ってしまうと DFS 木の s の先祖に帰れないなら s-t は橋である．
					if (in[s] < low[t]) is_bg[t.id] = true;

					// t から DFS 木の s の真の先祖に帰れないなら s は t にとって関節点である．
					//（ただし s が根の場合は後で例外処理する．）
					is_ap[s] = is_ap[s] || (in[s] <= low[t]);
					child_cnt++;
				}
			}

			// s が根の場合，子が 2 つ以上ないと関節点にはなり得ない．
			if (s == rt) is_ap[s] = (child_cnt >= 2);
			};

		// 各連結成分ごとに適当な点を始点（根）として DFS を行い，in と　low を求める．
		rep(s, n) {
			if (in[s] != -1) continue;

			// 連結成分を跨ぐには INF 時間かかる
			time = (time / INF + 1) * INF;

			// s を根として DFS し，同連結成分内の in と low を定める．
			rt = s;
			dfs(s, -1);
		}
	}

	// 頂点 s が関節点かを返す．
	bool articulation_pointQ(int s) {
		return is_ap[s];
	}

	// g の関節点の昇順リストを返す．
	vi get_articulation_points() {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_A

		vi aps;
		rep(s, n) if (is_ap[s]) aps.push_back(s);
		return aps;
	}

	// 辺 j が橋かを返す．
	bool bridgeQ(int j) {
		// verify : https://atcoder.jp/contests/abc375/tasks/abc375_g

		return is_bg[j];
	}

	// g の橋の番号の昇順リストを返す．
	vi get_bridges() {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_B

		vi bgs;
		rep(j, m) if (is_bg[j]) bgs.push_back(j);
		return bgs;
	}

	// g を二重辺連結成分分解し，二重辺連結成分の頂点集合のリストを返す．
	vvi get_two_edge_connected_components() {
		// verify : https://judge.yosupo.jp/problem/two_edge_connected_components

		// ccs : 連結成分の頂点のリスト
		vvi ccs; vb seen(n);

		function<void(int, int)> dfs = [&](int s, int p) {
			ccs.back().push_back(s);
			seen[s] = true;

			repe(t, g[s]) {
				// 親には戻らない．探索済の頂点には進まない．橋は渡らない．
				if (t == p || seen[t] || is_bg[t.id]) continue;

				dfs(t, s);
			}
			};

		rep(s, n) {
			if (seen[s]) continue;

			ccs.push_back(vi());
			dfs(s, -1);
		}

		return ccs;
	}

	// g を二重頂点連結成分分解し，二重頂点連結成分の辺集合の番号のリストを返す．
	vvi get_two_vertex_connected_components() {
		// verify : https://judge.yosupo.jp/problem/biconnected_components

		// ccs : 連結成分の辺のリスト
		vvi ccs; vb seen(m);

		// s : 注目頂点
		// ap : 探索を開始した関節点または根
		// k : 何番目の二重頂点連結成分を検出中か
		function<void(int, int, int)> dfs = [&](int s, int ap, int k) {
			repe(t, g[s]) {
				// 探索済の辺には進まない．
				if (seen[t.id]) continue;
				seen[t.id] = true;

				// s が t にとっての関節点であるような辺 s-t の先は別の二重頂点連結成分である．
				if (in[s] <= low[t]) {
					ccs.push_back(vi{ t.id });
					dfs(t, s, sz(ccs) - 1);
					continue;
				}

				// 辺を記録する．
				ccs[k].push_back(t.id);

				// 後退辺でなければ先を探索する．
				if (in[s] < in[t]) dfs(t, ap, k);
			}
			};

		rep(s, n) repe(t, g[s]) {
			if (seen[t.id]) continue;
			seen[t.id] = true;

			ccs.push_back(vi{ t.id });
			dfs(t, s, sz(ccs) - 1);
		}

		return ccs;
	}

	bool connectedQ(int s, int t) {
		return in[s] / INF == in[t] / INF;
	}

	bool separated_by_bridgeQ(int s, int j, int t) {
		// verify : https://atcoder.jp/contests/abc301/tasks/abc301_h

		if (!connectedQ(s, t)) return false;
		if (!is_bg[j]) return false;

		bool bs = in_e[j] < in[s] && in[s] < out_e[j];
		bool bt = in_e[j] < in[t] && in[t] < out_e[j];
		return bs != bt;
	}
};


//【向き付けによる強連結化】
/*
* 橋をもたない連結無向グラフ g に対し，DFS 木の辺に下向き，後退辺に上向きに向き付けることで，
* g を強連結な有向グラフにすることができる．
* 
* verify : https://atcoder.jp/contests/arc143/tasks/arc143_d
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


//【グラフのデカルト木】O(n + m α(n))
/*
* 与えられた無向グラフ g に対し，以下の規則で構築される n-1 を根とする有向根付き木 T を返す：
*	頂点 p に隣接する p 未満の頂点のみからなる各連結成分 S に対し，S 内の番号最大の頂点 s を p の子とする．
*
* 性質：
*	g で s 以下の頂点のみからなるパス s-t が存在する ⇔ T で t は s の子孫
*	特に g で隣接する 2 頂点は T で先祖-子孫の関係にある．
*/
Graph graph_cartesian_tree(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/2588

	int n = sz(g);

	Graph g2(n);

	dsu d(n);

	// v_max[l] : l をリーダーとする連結成分内の最大頂点番号
	vi v_max(n); iota(all(v_max), 0);

	rep(s, n) {
		repe(t, g[s]) {
			if (t > s) continue;

			// 既に s と連結済なら何もしない．
			if (d.same(s, t)) continue;

			// s の子を t を含む連結成分内の最大頂点とする．
			g2[s].push_back(v_max[d.leader(t)]);

			// s と t を連結する．
			d.merge(s, t);
		}

		// s を含む連結成分内の最大頂点は s である．
		v_max[d.leader(s)] = s;
	}

	return g2;
}


//【誘導部分グラフの抽出（密度保証）】O(n^2 (n + m))
/*
* 無向グラフ g に対し，誘導部分グラフ g[vs] の密度が d_num/d_dnm より大きい vs を返す（なければ空リスト）
* 密度：(辺の数)/(頂点の数)
*/
vi goldberg(const Graph& g, ll d_num, ll d_dnm) {
	// 参考 : http://dopal.cs.uec.ac.jp/okamotoy/lect/2021/gn/lect09.pdf

	int n = sz(g);

	// m : g の辺数
	ll m = 0;
	rep(s, n) m += sz(g[s]);
	m /= 2;

	int ST = n, GL = ST + 1;
	mf_graph<ll> g2(GL + 1);

	rep(s, n) {
		// 辺 ST→v の容量を m とする．
		g2.add_edge(ST, s, m * d_dnm);

		// 辺 v→GL の容量を m + 2 (密度保証) - deg(v) とする．
		g2.add_edge(s, GL, m * d_dnm + 2 * d_num - sz(g[s]) * d_dnm);

		// 辺 s→t の容量を 1 とする．
		repe(t, g[s]) g2.add_edge(s, t, d_dnm);
	}

	// グラフ g2 の最小カットを求める
	auto cap = g2.flow(ST, GL);

	// 最小カットの容量が m n 以上なら密度保証を満たす誘導部分グラフは存在しない．
	//（等号のときは，密度 d_num/d_dnm のグラフを見つけた or {ST} を見つけてしまった）
	if (cap >= m * n * d_dnm) return vi();

	// さもなくば，最小カットにおいて ST にある頂点集合が条件を満たす．
	vi res;
	auto b = g2.min_cut(ST);
	rep(s, n) if (b[s]) res.push_back(s);

	return res;
}


//【到達可能性】O((n + m) q / 64)
/*
* 与えられた有向グラフ g に対し，各 j∈[0..q) について，
* 頂点 u[j] から頂点 v[j] に到達可能かを格納したリストを返す．
*
* 利用：【強連結成分分解】，【頂点の縮約】，【到達可能性（DAG）】
*/
vb reachability(const Graph& g, const vi& u, const vi& v) {
	// verify : https://atcoder.jp/contests/abc204/tasks/abc204_c

	int n = sz(g), q = sz(u);
	vb res(q);

	// g を強連結成分分解する．
	auto ccs = strongly_connected_component(g);

	// id[s] : g の頂点 s が何番目の強連結成分に属しているか
	vi id(n);
	rep(i, sz(ccs)) repe(s, ccs[i]) id[s] = i;

	// u2[j], v2[j] : u[j], v[j] の属する強連結成分の番号
	vi u2(q), v2(q);
	rep(j, q) {
		u2[j] = id[u[j]];
		v2[j] = id[v[j]];
	}

	// g2 : g の強連結成分を縮約した DAG
	auto g2 = vertex_contraction(g, ccs);

	// DAG g2 において到達可能性を調べれば良い．
	return reachability_DAG(g2, u2, v2);
}


//【通行可能性と壁の連結性の双対性】
/*
* L から R まで通行可能[不可能] ⇔ U 側の壁と D 側の壁が非連結[連結]
* 
* verify : https://atcoder.jp/contests/abc181/tasks/abc181_f
*/


//【連結性を保った頂点消去】
/*
* 連結無向グラフ g について，連結性を保ったまま頂点を消去していくには，
* g の辺を用いて適当な頂点を根とする木を作り，深さ降順に頂点を消去していけばいい．
* 
* verify : https://atcoder.jp/contests/arc119/tasks/arc119_d
*/


//【閉路長の倍数条件】
/*
* 強連結な有向グラフ g について，以下の 2 条件は同値である：
*	(1) 頂点 ST を含む全ての閉路の長さが D の倍数
*	(2) 任意の辺 e:s→t について，dist[ST][s] + e.cost ≡ dist[ST][t] (mod D)
* 
* (1)⇒(2) の証明：
*	ST を根とする g の最短路木を T とすると，任意の e∈T について (2) が成り立つことに注意する．
*	まず ST を終点とする辺 e:s→ST を 1 つ固定する．
*	T の辺のみからなる ST から s へのパスをとり，これと e を繋ぐことで ST を通る閉路 C が得られる．
*	(1) より C の長さは D の倍数であり，T の辺は全て (2) を満たすので e も (2) を満たす．
*	次に C 上のある点 t2 と t2 を終点とする辺 e2:s2→t2 を 1 つ固定する．
*	T の辺のみからなる ST から s2 へのパスを取り，これと e2 を繋ぎ，さらに C の辺のみからなる
*	t2 から ST へのパスを繋ぐことで，ST を通る閉路 C2 が得られる．
*	(1) より C2 の長さは D の倍数であり，T, C の辺は全て (2) を満たすので e2 も (2) を満たす．
*	g は強連結なので，同様の手順を繰り返すことで全ての辺が (2) を満たすことが示せる．
* 
* (2)⇒(1) の証明：
*	ST から ST への任意の閉路 C について，(2) より
*		dist[ST][ST] + Σe∈C e.cost ≡ dist[ST][ST] (mod D)
*	が成り立つ．これは閉路長が D の倍数であることを意味している．
* 
* verify : https://atcoder.jp/contests/abc306/tasks/abc306_g
*/


//【独立集合判定】O(2^N N)
/*
* グラフ g の各頂点集合 set⊂[0..N) が独立集合かを格納したリストを返す．
*/
vb independent_setQ(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/chromatic_polynomial

	int n = sz(g);

	// ind[set] : set⊂[0..n) が独立集合か
	vb ind(1LL << n, true);

	// 辺の両端からなる 2 点集合 {s, t} は独立集合ではない．
	rep(s, n) repe(t, g[s]) {
		int set = (1 << s) | (1 << t);
		ind[set] = false;
	}

	// 独立集合でない集合を部分集合にもつ集合は独立集合ではない．
	repb(set, n) repis(i, set) {
		int sub = set - (1 << i);
		ind[set] = ind[set] && ind[sub];
	}

	return ind;
}


//【パスグラフ判定】O(n + m)
/*
* 無向グラフ g がパスグラフかを返す（空グラフ，1 点 0 辺のグラフもパスグラフと認める）
*/
template <class G>
bool path_graphQ(const G& g) {
	// verify : https://atcoder.jp/contests/abc287/tasks/abc287_c

	int n = sz(g);

	// 空グラフもパスグラフと認めることにする．
	if (n == 0) return true;

	// 1 頂点からなるグラフに対する例外処理
	if (n == 1) {
		if (sz(g[0]) == 0) return true;
		return false;
	}

	// c1 : 次数 1 の頂点の個数，c2 : 次数 2 の頂点の個数
	int c1 = 0, c2 = 0;
	rep(i, n) {
		int deg = sz(g[i]);
		c1 += (deg == 1);
		c2 += (deg == 2);
	}

	if (c1 != 2 || c2 != n - 2) return false;

	// 連結性の確認
	dsu d(n);
	rep(s, n) repe(t, g[s]) d.merge(s, t);

	return d.size(0) == n;
}


//【ウニグラフ判定】O(n + m)
/*
* 無向グラフ g がウニグラフかを返す．
*/
template <class G>
bool uni_graphQ(const G& g) {
	// verify : https://atcoder.jp/contests/abc225/tasks/abc225_b

	int n = sz(g);

	// 空グラフもウニグラフと認めることにする．
	if (n == 0) return true;

	// 1 頂点からなるグラフに対する例外処理
	if (n == 1) {
		if (sz(g[0]) == 0) return true;
		return false;
	}

	// c1 : 次数 1 の頂点の個数，cn1 : 次数 n-1 の頂点の個数
	int c1 = 0, cn1 = 0;
	rep(i, n) {
		int deg = sz(g[i]);
		c1 += (deg == 1);
		cn1 += (deg == n - 1);
	}

	if (n == 2) {
		if (c1 != 2) return false;
	}
	else {
		if (c1 != n - 1 || cn1 != 1) return false;
	}

	// 連結性の確認
	dsu d(n);
	rep(s, n) repe(t, g[s]) d.merge(s, t);

	return d.size(0) == n;
}


