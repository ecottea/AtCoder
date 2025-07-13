#pragma once
#include "header.h"
#include "包含.h"
#include "二項係数.h"
#include "列挙(分割).h"
#include "群論.h"
#include "bit全探索.h"
#include "全域木.h"
#include "最短路.h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■


//【長さ k のパスの数え上げ】
/*
* 隣接行列を k 乗すれば，全頂点対の長さ k のパス数が O(n^3 log k) で求まる．
* 
* 辺が少なく単一始点でよいなら，行列ベクトル積を繰り返し O(m k) で求まる． 
* verify : https://atcoder.jp/contests/abc244/tasks/abc244_e
*/


//【最短経路の数え上げ】O(n + m)
/*
* 有向グラフ g に対し，始点 st から各頂点 i への最短経路数を格納したリストを返す．
* また必要ならそのときの最短距離（到達不能なら INF）を dist[i] に格納する．
* 
*（幅優先探索）
*/
vm count_shortest_path(const Graph& g, int st, vi* dist = nullptr) {
	// verify : https://atcoder.jp/contests/abc211/tasks/abc211_d

	int n = sz(g);

	// cnt[i] : st から i までの最短経路の総数
	vm cnt(n);
	cnt[st] = 1;

	// dist[i] : st から i までの最短距離
	if (dist == nullptr) dist = new vi;
	*dist = vi(n, INF);
	(*dist)[st] = 0;

	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = que.front(); que.pop();

		repe(t, g[s]) {
			// t が探索済の頂点の場合
			if ((*dist)[t] != INF) {
				// 現時点での最短距離と同じなら個数を加算する．
				if ((*dist)[t] == (*dist)[s] + 1) cnt[t] += cnt[s];

				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			(*dist)[t] = (*dist)[s] + 1;
			cnt[t] = cnt[s];

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	return cnt;
}


//【最短経路の数え上げ（重み付きグラフ）】O(n + m log n)
/*
* 正の重み付き有向グラフ g に対し，始点 st から各頂点 i への最短経路数を格納したリストを返す．
* また必要ならそのときの最短距離（到達不能なら INFL）を dist[i] に格納する．
*
*（ダイクストラ法）
*/
vm count_shortest_path(const WGraph& g, int st, vl* dist = nullptr) {
	// verify : https://atcoder.jp/contests/arc090/tasks/arc090_c

	//【注意】
	// コスト 0 の辺のみでできた閉路があると，そこを通る最短経路数は無限個になる．
	// 閉路さえなければコスト 0 の辺があっても大丈夫？

	int n = sz(g);

	// dist[i] : st から i までの最短距離
	if (dist == nullptr) dist = new vl;
	*dist = vl(n, INFL);
	(*dist)[st] = 0;

	// cnt[i] : st から i までの最短経路の総数
	vm cnt(n);
	cnt[st] = 1;

	// 組 (st からの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pli> q;
	q.push({ 0, st });

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto [c, s] = q.top(); q.pop();

		// すでにより短い距離に更新されていたなら何もしない（忘れると O(n^2)）
		if ((*dist)[s] < c) continue;

		// より短い距離で辿り着けるなら距離を更新し，その先も探索する．
		repe(e, g[s]) {
			ll ndist = (*dist)[s] + e.cost;
			if (ndist == (*dist)[e.to]) {
				cnt[e.to] += cnt[s];
			}
			else if (ndist < (*dist)[e.to]) {
				(*dist)[e.to] = ndist;
				cnt[e.to] = cnt[s];
				q.push({ (*dist)[e.to], e.to });
			}
		}
	}

	return cnt;
}


//【最短経路の数え上げ（補グラフ）】O(n + m)
/*
* 与えられた無向グラフ g の補グラフに対し，始点 ST から各頂点 i への最短距離のリストと，
* 最短距離を実現する経路の総数のリストの組を返す．（距離は到達不能なら INF）
*
* 利用：【幅優先探索（補グラフ）】
*/
pair<vi, vm> count_shortest_path_complement(const Graph& g, int ST) {
	// verify : https://atcoder.jp/contests/abc319/tasks/abc319_g

	int n = sz(g);

	// dist[s] : ST から s までの最短距離（到達不能なら INF）
	auto dist = complement_BFS(g, ST);

	// d2s[d] : ST からの距離が d である頂点のリスト
	vvi d2s(n);
	rep(s, n) if (dist[s] != INF) d2s[dist[s]].push_back(s);

	// cnt[s] : ST から s までの距離が dist[s] である経路の総数
	vm cnt(n);
	cnt[ST] = 1;

	rep(d, n - 1) {
		mint cnt_all;
		repe(s, d2s[d]) cnt_all += cnt[s];

		repe(t, d2s[d + 1]) {
			// ひとまず全部足しておく．
			cnt[t] = cnt_all;

			// そこから禁止された辺を通る経路数を引く．
			repe(s, g[t]) if (dist[s] == d) cnt[t] -= cnt[s];
		}
	}

	return { dist, cnt };
}


//【独立集合の数え上げ】O(2^n n)
/*
* 与えられた無向グラフ g に対し，各頂点集合 set⊂[0..n) について，
* set の部分集合のうち g の独立集合を成すものの個数を格納したリストを返す．
*/
template <class T>
vector<T> count_independent_set(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/chromatic_number

	int n = sz(g);
	vector<T> cnt(1LL << n);

	repb(set, n) {
		if (set == 0) {
			cnt[set] = 1;
			continue;
		}

		// s : set に含まれる頂点
		int s = msb(set);

		// s を含まない独立集合の個数を数える．
		int sub = set - (1 << s);
		cnt[set] = cnt[sub];

		// s を含む独立集合の個数を数える．
		repe(t, g[s]) {
			// s と辺で結ばれた頂点は選ぶことができない．
			sub &= ~(1 << t);
		}
		cnt[set] += cnt[sub];
	}

	return cnt;
}


//【k-彩色の数え上げ】O(2^n n^3)
/*
* 各 k∈[0..n] について，無向グラフ g を k 色で彩色する方法の数を格納したリストを返す．
*/
template <class T>
vector<T> count_chromatic(const Graph& g) {
	int N = sz(g);

	if (N == 0) return vector<T>{ 1 };

	// is_ind[set] : set が独立集合か
	vi is_ind(1LL << N, 1);

	// 辺の両端からなる 2 点集合 {s, t} は独立集合ではない．
	rep(s, N) repe(t, g[s]) {
		int set = (1 << s) + (1 << t);
		is_ind[set] = 0;
	}

	// 独立集合でない集合を部分集合にもつ集合は独立集合ではない．
	repb(set, N) rep(i, N) {
		if (set & (1 << i)) {
			int sub = set - (1 << i);
			is_ind[set] = is_ind[set] & is_ind[sub];
		}
	}

	// is_ind にランク（集合の要素数）の情報を付加して f とする．
	vector<vector<T>> f(1LL << N, vector<T>(N + 1));
	repb(set, N) {
		int r = popcount(set);
		f[set][r] = is_ind[set];
	}

	// f のランク付き下位ゼータ変換
	rep(i, N) repb(set, N) repi(r, 0, N) {
		if (!(set & (1 << i))) f[set + (1 << i)][r] += f[set][r];
	}

	// f_pow : f^k
	vector<vector<T>> f_pow(1LL << N, vector<T>(N + 1));
	repb(set, N) f_pow[set][0] = 1;

	vector<T> cnt(N + 1);
	repi(k, 1, N) {
		// 各点積 h = f^(k-1) * f（ただしランクが N より大きい項は無視する）
		vector<vector<T>> h(1LL << N, vector<T>(N + 1));
		repb(set, N) repi(r, 0, N) repi(rf, 0, r) h[set][r] += f_pow[set][rf] * f[set][r - rf];
		f_pow = move(h);

		// f_pow のランク付き下位メビウス変換（最大元のランク N の項のみ）
		repb(set, N) cnt[k] += ((N - popcount(set)) & 1 ? -1 : 1) * f_pow[set][N];
	}

	return cnt;
}


//【k-彩色の数え上げ（辺が少）】
/*
* 無向グラフ G=(V,E) の k-彩色の数を P(G, k) と表すとき，∀e∈E について，
*	P(G, k) = P(G-e, k) - P(G/e, k)
* が成り立つ（G-e は G から辺 e を取り除いたグラフ，G/e は G の辺 e を縮約したグラフ）
*
* また G が n 頂点の木であれば，P(G, k) = k (k-1)^(n-1) である．
*
* verify : https://atcoder.jp/contests/abc294/tasks/abc294_h
*/


//【彩色多項式】O(2^N N^2)
/*
* 無向グラフ g の彩色多項式 f(z) を返す（f(k) は g を k 色で彩色する場合の数を表す）
*
* 利用：【独立集合判定】,【SPS 累乗の係数列挙】,【ラグランジュ補間（多項式復元）】
*/
MFPS chromatic_polynomial(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/chromatic_polynomial

	int n = sz(g);

	auto ind = independent_setQ(g);

	vm s(1LL << n);
	repb(set, n) s[set] = (int)ind[set];

	vm id(1LL << n);
	id[0] = 1;
	auto y = coefficients_of_power_sps(id, s, n + 1); // f(k) のリスト

	vm x(n + 1);
	iota(all(x), mint(0));

	return lagrange_interpolation(x, y);
}


//【無向グラフのハミルトン閉路の数え上げ】O(2^n n^2)
/*
* 重み付き対称隣接行列 c[0..n)[0..n) が表す無向グラフ g に対し，各頂点集合 set について，
* 誘導部分グラフ g[set] のハミルトン閉路の個数を並べたリストを返す．
*
*（bit DP）
*/
vm count_hamiltonian_cycle(const vvm& c) {
	// verify : https://atcoder.jp/contests/abc411/tasks/abc411_g

	int n = sz(c);
	if (n == 0) return vm{ 0 };
	if (n == 1) return vm{ 0, 0 };

	vm res(1LL << n, 0);

	// v : set に含まれる番号最大の頂点
	rep(v, n) {
		// dp[s][set] : 頂点 s から頂点 v までの set⊂[0..v] を通る単純パスの個数
		vvm dp(v + 1, vm(1LL << (v + 1), 0));
		dp[v][1LL << v] = 1;

		// 貰う DP
		repi(set, 1 << v, (1 << (v + 1)) - 1) {
			// s, t ∈ set なる辺 s→t をチェックする．
			repis(s, set) repis(t, set - (1 << s)) {
				dp[s][set] += c[s][t] * dp[t][set - (1 << s)];
			}
		}

		// g[set] のハミルトン路 s→v に辺 v→s を追加して g[set] のハミルトン閉路を得る．
		repi(set, 1 << v, (1 << (v + 1)) - 1) repis(s, set) {
			res[set] += dp[s][set] * c[v][s];
		}
	}

	mint inv2 = mint(2).inv();

	// 長さ 2 の閉路は，同じ辺を往復してしまっているので数え直す．
	rep(u, n) rep(v, n) {
		if (v == u) continue;

		int set = (1 << u) + (1 << v);
		res[set] = c[u][v] * (c[u][v] - 1) * inv2;
	}

	// 長さ 3 以上の閉路は，回る向き 2 通りで重複カウントしているので 2 で割る．
	repb(set, n) {
		int pc = popcount(set);
		if (pc <= 2) continue;

		res[set] *= inv2;
	}

	return res;
}


//【単純パス，単純サイクルの数え上げ】O(2^n n (n+m))
/*
* 有向グラフ g について，set を通る単純パス s→t の個数を cnt[s][t][set] に格納し，cnt を返す．
* また cflag = true なら set を通る単純サイクル s→s の個数も cnt[s][s][set] に格納する．
*
*（bit DP）
*/
vvvm count_simple_path_and_cycle(const Graph& g, bool cflag = true) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_house

	int n = sz(g);

	// dp[s][t][set] : set を通る s→t 単純パスの個数
	vvvm dp(n, vvm(n, vm(1LL << n)));

	// 貰う DP で単純パスを数え上げる．
	repb(set, n) rep(s, n) rep(t, n) {
		// s→t パスなので s, t を通らないことはありえない．
		if (!getb(set, s) || !getb(set, t)) continue;

		// set = {s} = {t} の場合は不動の 1 通りのみである．
		if (s == t && set == (1 << s)) {
			dp[s][t][set] = 1;
			continue;
		}

		// 各辺 s-v について
		repe(v, g[s]) {
			// v が set に含まれていなければ s→v は通れない．
			if (!getb(set, v)) continue;

			// s→v と進む単純パスを加算する．
			dp[s][t][set] += dp[v][t][set - (1 << s)];
		}
	}

	// 単純サイクルを数え上げる．
	if (cflag) {
		mint inv2 = mint(2).inv();
		repb(set, n) rep(s, n) {
			// s→s サイクルなので s を通らないことはありえない．
			if (!getb(set, s)) continue;

			// set には少なくとも 3 点含まれていなければならない．
			if (popcount(set) < 3) continue;

			// 各辺 s-v について
			repe(v, g[s]) {
				// v が set に含まれていなければ s→v は通れない．
				if (!getb(set, v)) continue;

				// s→v と進む単純パスを加算する（s は終点でもあるので set から除かない）
				dp[s][s][set] += dp[v][s][set];
			}

			// 2 通りの向きを数えてしまっているので 2 で割る．
			dp[s][s][set] *= inv2;
		}
	}

	return dp;
}


//【トポロジカルソートの数え上げ】O(2^n m)
/*
* 有向グラフ g をトポロジカルソートする方法が何通りあるかを返す．
*/
ll count_topological_sort(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc041/tasks/abc041_d

	//【方法】
	// トポロジカルソートされた列の後ろから順にどの頂点を割り当てるかを決めていく．
	// 割り当て済の頂点の集合を覚えておく bit DP を用いる．

	int n = sz(g);

	// dp[set] : トポロジカルソートされた列の後ろから |set| 個の頂点が set である場合の数
	vl dp(1LL << n);
	dp[0] = 1;

	repb(set, n) {
		// s : 次に割り当てる頂点
		rep(s, n) {
			// s が既に割り当て済の頂点なら何もしない．
			if (set & (1 << s)) continue;

			// トポロジカルソートされた列の後ろ順に頂点を対応させていっているので，
			// s→t なる全ての頂点 t は既に選ばれていなければならない．
			bool choosable = true;
			repe(t, g[s]) {
				if (!(set & (1 << t))) {
					choosable = false;
					break;
				}
			}

			if (choosable) dp[set + (1 << s)] += dp[set];
		}
	}

	return dp[(1 << n) - 1];
}


//【DAG の数え上げ】O(3^n)
/*
* 無向グラフ g の各辺に向きを付けて DAG にする方法が何通りあるかを返す．
*
* 利用：【下位集合の全探索】
*/
mint count_DAG(const Graph& g) {
	//【方法】
	// dp[set] を 誘導部分グラフ g[set] を DAG にする方法の数と定める．
	// g[set] の入次数 0 の頂点集合 sub⊂set を決め打つ（sub が G の独立集合であることが必要）
	// sub に接続する辺の向きは全て決まり，あとは g[set-sub] を DAG にすれば良い．
	// ただし set={s}, {t} の中には set={s,t} とした場合が含まれるなどするので，
	// (-1)^(|set|+1) を係数として乗じて包除しながら計算していく必要がある．

	int n = sz(g);

	// is_ind[set] : set が g の独立集合か
	vb is_ind(1LL << n);
	is_ind[0] = true;

	repb(set, n) {
		if (set == 0) continue;

		// s : set に属する番号最大の頂点
		int s = msb(set);

		// set - s が独立集合でなければ明らかに set も独立集合でない．
		if (!is_ind[set - (1 << s)]) continue;

		// s に隣接するどの頂点 t も set に属していないなら set は独立集合．
		bool ok = true;
		repe(t, g[s]) if (getb(set, t)) {
			ok = false;
			break;
		}
		is_ind[set] = ok;
	}

	// dp[set] : 誘導部分グラフ g[set] を DAG にする方法の数
	vm dp(1LL << n);
	dp[0] = 1;

	// SoS-bit DP
	repb(set, n) {
		repbs(sub, set) {
			if (sub == 0 || !is_ind[sub]) continue;

			dp[set] += dp[set - sub] * (popcount(sub) & 1 ? 1 : -1);
		}
	}

	return dp[(1 << n) - 1];
}


//【部分グラフの数え上げ】O(2^n n)
/*
* 与えられた無向グラフ g に対し，各頂点集合 set⊂[0..n) について，
* set を頂点集合とする部分グラフの個数を格納したリストを返す．
*
* 利用：【下位ゼータ変換】
*/
vm count_subgraph(const Graph& g) {
	// 参考：https://drken1215.hatenablog.com/entry/2021/08/12/132500
	// verify : https://atcoder.jp/contests/abc213/tasks/abc213_g

	//【方法】
	// 頂点集合 set に対する答えは明らかに
	//		2^(g の辺のうち両端点とも set に属するものの個数)
	// であるから，指数部分
	//		g[set] := g の辺のうち両端点とも set に属するものの個数
	// が求まれば良い．
	//
	// そこで，
	//		f[set] := Boole[ |set| = 2 かつそれらが g のある辺の両端点か ]
	// と定めると，
	//		g[set] = Σsub⊂set f[sub]
	// と表される．これは下位集合での高速ゼータ変換で計算可能である．

	int n = sz(g);
	vm cnt(1LL << n);

	vi f(1LL << n); int cnt_e = 0;
	rep(s, n) {
		int set = 1 << s;

		repe(t, g[s]) {
			f[set + (1 << t)] = 1;
			cnt_e++;
		}
	}
	cnt_e /= 2;

	set_subzeta(f);

	vm pow2(cnt_e + 1);
	pow2[0] = 1;
	rep(i, cnt_e) pow2[i + 1] = pow2[i] * 2;

	repb(set, n) {
		cnt[set] = pow2[f[set]];
	}

	return cnt;
}


//【連結部分グラフの数え上げ】O(3^n n)
/*
* 与えられた無向グラフ g に対し，各頂点集合 set⊂[0..n) について，
* set を頂点集合とする連結部分グラフの個数を格納したリストを返す．
*
* 利用：【部分グラフの数え上げ】,【下位集合の全探索】
*/
vm count_connected_subgraph(const Graph& g) {
	// 参考 : https://drken1215.hatenablog.com/entry/2021/08/12/132500
	// verify : https://atcoder.jp/contests/abc213/tasks/abc213_g

	//【方法】
	// まず
	//		all[set] := set を頂点集合とする部分グラフの個数
	// を求めておく．
	// 
	// cnt[set] について考える代わりに，連結でないものの個数 all[set] - cnt[set] を考える．
	// set に属する頂点 v を 1 つ固定し，v と連結な頂点集合 sub で場合分けして和をとることにより，
	//		all[set] - cnt[set] = Σv∈sub⊂set cnt[sub] all[set - sub]
	// と表される．

	int n = sz(g);
	vm cnt(1LL << n);

	vm cnt_all = count_subgraph(g);

	repb(set, n) {
		if (set == 0) {
			cnt[set] = 1;
			continue;
		}

		int v = lsb(set);

		mint sum = 0;
		repbs(sub, set) {
			if (!(sub & (1 << v))) continue;
			if (sub == set) continue;

			sum += cnt[sub] * cnt_all[set - sub];
		}

		cnt[set] = cnt_all[set] - sum;
	}

	return cnt;
}


//【有向オイラー閉路の数え上げ（重み付き）】O(n^3)
/*
* 重み付き有向グラフ g のオイラー閉路の個数を返す（辺の重みは辺の本数と解釈する．）
*
* 制約 : fm は m! まで計算可能
*
* 利用：【有向全域木の数え上げ（重み付き）】
*/
mint best_theorem(const WGraph& g, Factorial_mint& fm) {
	// 参考 : https://atcoder.jp/contests/abc336/editorial/9060
	// verify : https://judge.yosupo.jp/problem/counting_eulerian_circuits

	int n = sz(g);

	// 入次数と出次数を調べる．
	vl in_deg(n), out_deg(n);
	rep(s, n) repe(t, g[s]) {
		in_deg[t] += t.cost;
		out_deg[s] += t.cost;
	}

	// 入次数と出次数が異なる頂点があればオイラー閉路は存在しない．
	rep(s, n) if (in_deg[s] != out_deg[s]) return 0;

	// vs : 非孤立点，v_id[s] : s が何番目の頂点か
	vi vs; vi v_id(n, -1);
	rep(s, n) if (in_deg[s] > 0) {
		v_id[s] = sz(vs);
		vs.push_back(s);
	}
	int n2 = sz(vs);

	// 辺が 0 本の場合は空のオイラー閉路を 1 つもつと考える．
	if (n2 == 0) return 1;

	// g2 : 孤立点を除去した有向グラフ
	WGraph g2(n2);
	rep(s, n) repe(t, g[s]) g2[v_id[s]].emplace_back(v_id[t], t.cost);

	// g2 が連結でなければオイラー閉路は存在しない．
	dsu d(n2);
	rep(s, n2) repe(t, g2[s]) d.merge(s, t);
	if (d.size(0) != n2) return 0;

	// 有向行列木定理で 0 を根とする有向全域木の個数を求める．
	mint res = directed_matrix_tree_theorem(g2, 0);

	// これに各頂点の (入次数)-1 を掛けたものが求めるオイラー閉路の個数となる．
	repe(s, vs) res *= fm.fact((int)in_deg[s] - 1);

	return res;
}


//【有向オイラー路の数え上げ】
/*
* 始点 ST と終点 GL を指定されたオイラー路（ST=GL も可）の個数は，
* 辺 GL→ST を追加したグラフのオイラー閉路の個数に等しい．
* 
* verify : https://atcoder.jp/contests/agc051/tasks/agc051_d
*/


//【無向オイラー閉路の数え上げ】
/*
* 無向グラフのオイラー閉路を多項式時間で数え上げる方法は見つかっていない．
* もし辺で結ばれている点対が (n-1)+k 個ならば，適当な k 個の対の間の辺の向き付けを決め打ち
* 有向グラフに変換することで O(m^k n^3) を達成できる．
* 
* verify : https://atcoder.jp/contests/agc051/tasks/agc051_d
*/


//【入力定数個のグラフ数え上げ】
/*
* 参考 : https://maspypy.com/%E3%82%B0%E3%83%A9%E3%83%95%E6%95%B0%E3%81%88%E4%B8%8A%E3%81%92
*/


//【ラベルなし単純グラフの数え上げ】O(n の分割数)（n=50 くらいまで動く）
/*
* n 頂点のラベルなし単純グラフの個数を返す．
*
* 利用：【自然数の分割の列挙（値が k 以下）】,【置換の数え上げ（型指定）】
*/
mint count_unlabeled_simple_graph(int n) {
	Factorial_mint fm(n);

	// 型 p をもつ置換の頂点集合 [0..n) への作用から誘導される辺集合への作用の軌道の個数を返す．
	auto edges = [](const vi& p) {
		int k = sz(p), res = 0;

		// 1 つの巡回置換が作用する頂点集合の中を結ぶ辺についての軌道の個数を数える．
		//（正 p[i] 角形の対角線の長さが何通りあるかを数えていると思えば分かりやすい．）
		rep(i, k) res += p[i] / 2;

		// 異なる巡回置換が作用する頂点集合の間を結ぶ辺についての軌道の個数を数える．
		rep(i, k) repi(j, i + 1, k - 1) res += gcd(p[i], p[j]);

		return res;
	};

	// [0..n) 上の置換の型を列挙する．
	auto ips = integer_partitions_val(n, n);

	mint res = 0;

	// ポリアの数え上げ定理を用いて数え上げる．
	repe(p, ips) {
		// c : 型 p をもつ置換の個数
		auto c = count_permutation_type(p, fm);

		// ec : 型 p をもつ置換の辺集合上の軌道の個数．
		auto ec = edges(p);

		// 辺の有無は 2 色による彩色とみなせる．
		res += c * mint(2).pow(ec);
	}

	// |S_n| で割って平均をとる．
	res *= fm.fact_inv(n);

	return res;
}


//【有向パスによる頂点分割の数え上げ】
/*
* 頂点 [0..n) を k 個の長さ 0 以上の有向パスに分割する方法の数は，
* bin(n-1, k-1) * n! / k! で与えられる．
*
* 証明：頂点 [0..n) を使って 1 つの有向パス P を作る方法の数は n! である．
* P の辺 n-1 本から k-1 本を選んで（bin(n-1,k-1) 通り）切断することで
* P を k 個の長さ 0 以上の有向パス p[0..k) へ分割できる．
* p[0..k) の順列の数 k! だけ重複して数えているのでそれで割る．
*/


//【有向パスによる頂点分割の数え上げ（長さ 1 以上）】
/*
* 頂点 [0..n) を k 個の長さ 1 以上の有向パスに分割する方法の数は，
* bin(n-k-1, k-1) * n! / k! で与えられる．
*
* 証明：頂点 [0..n) を使って 1 つの有向パス P を作る方法の数は n! である．
* P の辺 n-1 本から切断箇所が連続しないように k-1 本を選んで（bin(n-k-1, k-1) 通り）
* 切断することで，P を k 個の長さ 1 以上の有向パス p[0..k) へ分割できる．
* p[0..k) の順列の数 k! だけ重複して数えているのでそれで割る．
*/


//【有向サイクルによる頂点分割の数え上げ】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，頂点 [0..i) を
* j 個の長さ 1 以上の有向サイクルに分割する方法の数を S1[i][j] に格納し S1 を返す．
*/
vvm count_directed_cycle_decomposition(int n, int m) {
	//【方法】
	// 頂点の有向サイクルへの分割は，置換の巡回置換の積への分解と等価である．
	// よって S1[i][j] は第 1 種スターリング数に等しい．

	// S1[i][j] : 頂点 [0..i) を j 個の長さ 1 以上の有向サイクルに分割する方法の数
	vvm S1(n + 1, vm(m + 1));
	S1[0][0] = 1;

	// 貰う DP
	repi(i, 1, n) repi(j, 1, m) {
		// 頂点 i-1 を，既存の有向サイクルのある頂点の直後に挿入する場合
		S1[i][j] += S1[i - 1][j] * (i - 1);

		// 頂点 i-1 を，単独で長さ 1 の有向サイクルとする場合
		S1[i][j] += S1[i - 1][j - 1];
	}

	return S1;
}


//【無向パスによる頂点分割の数え上げ（mod 998244353）】O(n m log m)
/*
* 各 i∈[0..n], j∈[0..m] について，頂点 [0..i) を
* j 個の長さ 0 以上の無向パスに分割する方法の数を res[i][j] に格納し res を返す．
*
* 制約：fm は max(n, m)! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
vvm count_undirected_path_decomposition(int n, int m, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/2135

	//【方法】
	// 頂点 [0..i) を j 個の長さ 1 以上の無向パスに分割する方法の数であれば
	// 有向パスの場合の結果より bin(i-j-1, j-1) * i! / (2^j j!) で与えられる．
	// これは 頂点 [0..i+k) を j+k 個の長さ 0 以上の無向パスに分割する方法としても
	// bin(i+k, k) = (i+k)! / (i! k!) 倍されたうえで数えられる．
	// よって最初に i 行目を 1/i! 倍，最後に i 行目を i! 倍することにし，
	// 斜め方向に {1/i!} と畳込みを行えば良い．

	vm pow2inv(m + 1);
	pow2inv[0] = 1;
	pow2inv[1] = mint(2).inv();
	repi(j, 2, m) pow2inv[j] = pow2inv[j - 1] * pow2inv[1];

	vvm res(n + 1, vm(m + 1));
	repi(i, 0, n) repi(j, 1, min(i - 1, m)) {
		res[i - j][j] = fm.bin(i - j - 1, j - 1) * fm.fact(i) * fm.fact_inv(j) * pow2inv[j];
		res[i - j][j] *= fm.fact_inv(i);
	}
	res[0][0] = 1;

	vm fac(m + 1);
	repi(j, 0, m) fac[j] = fm.fact_inv(j);
	repi(i, 0, n) {
		res[i] = convolution(res[i], fac);
		res[i].resize(m + 1);
	}

	repir(i, n, 0) repi(j, 0, m) {
		if (j <= i) res[i][j] = res[i - j][j] * fm.fact(i);
		else res[i][j] = 0;
	}

	return res;
}


//【無向サイクルによる頂点分割の数え上げ】O(n^2 m)
/*
* 各 i∈[0..n], j∈[0..m], k∈[0..n] について，頂点 [0..i) を j 個の無向サイクルに分割する方法のうち，
* サイクルの長さの最小値が k であるようなものの数を res[i][j][k] に格納し res を返す．
*
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
vvvm count_undirected_cycle_decomposition(int n, int m, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/2135

	//【方法】
	// dp[i][j][k] を，頂点 [1..i] を j 個の無向サイクルに分割する方法のうち，
	// サイクルの長さの最小値が k であるようなものの数と定める．
	// i≧2，j≧2 とし，頂点 i の属する無向サイクルの長さ l≦k で場合分けを行い dp[i][j][k] を計算する．
	//
	// (i) l=1 のとき
	// 他の無向サイクルの長さの最小値は何でも良いので
	//		dp[i][j][1] += Σdp[i-1][j-1][1..n]
	//
	// (ii) l=2 のとき
	// i 以外のもう 1 つの頂点の選び方は i-1 通りある．
	// k=1 のときは，他の無向サイクルの長さの最小値が 1 でなくてはならないので
	//		dp[i][j][1] += (i-1) dp[i-2][j-1][1]
	// k=2 のときは，他の無向サイクルの長さの最小値は 2 以上なら何でも良いので
	//		dp[i][j][2] += (i-1) Σdp[i-2][j-1][2..n]
	//
	// (iii) l≧3 のとき
	// i 以外の l-1 個の頂点の選び方は bin(i-1, l-1) 通りある．
	// さらにサイクル内の頂点の並び方が，長さ l の数珠順列で (l-1)!/2 通りある．
	// k<l のときは，他の無向サイクルの長さの最小値が k でなくてはならないので
	//		dp[i][j][k] += (l-1)!/2 bin(i-1, l-1) dp[i-l][j-1][k]
	// k=l のときは，他の無向サイクルの長さの最小値は l 以上なら何でも良いので
	//		dp[i][j][l] += (l-1)!/2 bin(i-1, l-1) Σdp[i-l][j-1][l..n]
	// 
	// (iii) の k<l のときは，l≧max(3,k+1) についてまとめて行うと
	//		dp[i][j][k] += Σl∈[max(3,k+1)..n] (l-1)!/2 bin(i-1, l-1) dp[i-l][j-1][k]
	// となる．このままでは累積和による高速化ができないので，dp の代わりに
	//		dp2[i][j][k] = (1/i!) dp[i][j][k]
	// とおく．
	//
	// dp2 を用いれば，それぞれの遷移式は
	//		dp2[i][j][1] += (1/i) Σdp2[i-1][j-1][1..n]
	//		dp2[i][j][1] += (1/i) dp2[i-2][j-1][1]
	//		dp2[i][j][2] += (1/i) Σdp2[i-2][j-1][2..n]
	//		dp2[i][j][k] += (1/2i) Σdp2[0..i-max(3,k+1)][j-1][k]
	//		dp2[i][j][l] += (1/2i) Σdp2[i-l][j-1][l..n]
	// と書き直せ，累積和を用いて高速化が可能になる．

	mint inv2 = 1 / mint(2);

	// dp2[i][j][k] : (1/i!) * (頂点 [1..i] を j 個の無向サイクルに分割する方法のうち，
	//		サイクルの長さの最小値が k であるようなものの数)
	vvvm dp2(n + 1, vvm(m + 1, vm(n + 1)));

	// dp2 の i に関する累積和
	vvvm acc2i(n + 2, vvm(m + 1, vm(n + 1)));

	// dp2 の k に関する累積和
	vvvm acc2k(n + 1, vvm(m + 1, vm(n + 2)));

	dp2[0][0][0] = 1;
	dp2[1][1][1] = 1;
	dp2[2][1][2] = inv2;
	repi(i, 3, n) {
		dp2[i][1][i] = fm.fact(i - 1) * inv2 * fm.fact_inv(i);
	}
	repi(i, 0, n) repi(k, 0, n) {
		acc2i[i + 1][1][k] = acc2i[i][1][k] + dp2[i][1][k];
		acc2k[i][1][k + 1] = acc2k[i][1][k] + dp2[i][1][k];
	}

	repi(i, 2, n) repi(j, 2, m) {
		// i の属する無向サイクルの長さが 1 のとき
		dp2[i][j][1] += (acc2k[i - 1][j - 1][n + 1] - acc2k[i - 1][j - 1][1]) * fm.inv(i);

		// i の属する無向サイクルの長さが 2 のとき
		dp2[i][j][1] += dp2[i - 2][j - 1][1] * fm.inv(i);
		dp2[i][j][2] += (acc2k[i - 2][j - 1][n + 1] - acc2k[i - 2][j - 1][2]) * fm.inv(i);

		// i の属する無向サイクルの長さが 3 以上のとき
		repi(k, 0, n - 1) {
			int l_min = max(3, k + 1);
			if (i - l_min + 1 < 0) break;

			dp2[i][j][k] += acc2i[i - l_min + 1][j - 1][k] * fm.inv(i) * inv2;
		}
		repi(l, 3, n) {
			if (i - l < 0) break;

			dp2[i][j][l] += (acc2k[i - l][j - 1][n + 1] - acc2k[i - l][j - 1][l]) * fm.inv(i) * inv2;
		}

		// 累積和の更新
		repi(k, 0, n) {
			acc2i[i + 1][j][k] = acc2i[i][j][k] + dp2[i][j][k];
			acc2k[i][j][k + 1] = acc2k[i][j][k] + dp2[i][j][k];
		}
	}

	auto res(dp2);
	repi(i, 0, n) repi(j, 0, m) repi(k, 0, n) res[i][j][k] *= fm.fact(i);

	return res;
}


//【全域木に関する数え上げ】
/*
* 全域木.h へ
*/


//【DAG 上のパスの数え上げ】
/*
* DAG.h へ
*/


//【マッチングの数え上げ】
/*
* マッチング(二部).h，マッチング(一般).h へ
*/


