#pragma once
#include "header.h"
#include "ゼータ変換.h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■


//【長さ k のパスの数え上げ】
/*
* 隣接行列を k 乗すれば，全頂点対の長さ k のパス数が O(|V|^3 log k) で求まる．
* 
* 辺が少なく単一始点でよいなら，行列ベクトル積を繰り返し O(|E| k) で求まる． 
* verify : https://atcoder.jp/contests/abc244/tasks/abc244_e
*/


//【最短経路の数え上げ】O(|V| + |E|)
/*
* グラフ g に対し，始点 st から各頂点 i への最短距離（到達不能なら INF）を dist[i] に，
* 最短経路の総数を cnt[i] にそれぞれ格納する．
*/
void count_shortest_path(const Graph& g, int st, vm& cnt, vi* dist = nullptr) {
	// verify : https://atcoder.jp/contests/abc211/tasks/abc211_d

	int n = sz(g);

	// cnt[i] : st から i までの最短経路の総数
	cnt.resize(n);
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
}


//【最短経路の数え上げ（コスト付きグラフ）】O(|V| + |E| log|V|)
/*
* 正のコスト付きグラフ g に対し，始点 st から各頂点 i への最短距離（到達不能なら INFL）を
* dist[i] に，最短経路の総数を cnt[i] にそれぞれ格納する．
*/
void count_shortest_path(const WGraph& g, int st, vm& cnt, vl* dist = nullptr) {
	// verify : https://atcoder.jp/contests/arc090/tasks/arc090_c

	int n = sz(g);

	// cnt[i] : st から i までの最短経路の総数
	cnt.resize(n);

	// dist[i] : st から i までの最短距離
	if (dist == nullptr) dist = new vl;
	*dist = vl(n, INFL);

	// 組 (st からの距離, 注目頂点, 直前の頂点) を入れる優先度付きキューを用意する．
	// st からの距離がより小さいものを優先的に取り出す．
	priority_queue_rev<tuple<ll, int, int>> que;
	que.push({ 0, st, -1 });

	while (!que.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		ll d; int s, p;
		tie(d, s, p) = que.top(); que.pop();
		mint c = (p == -1 ? 1 : cnt[p]);

		// 既に最短距離が求まっている場合
		if (d >= (*dist)[s]) {
			// 現時点での最短距離と同じなら個数を加算する．
			if (d == (*dist)[s]) cnt[s] += c;

			continue;
		}

		// 最短距離の決定
		// 優先度付きキューで距離の小さい順に取り出しており，
		// かつコストが非負より三角不等式が成立するので最短の保証がある．
		(*dist)[s] = d;
		cnt[s] = c;

		// そこから移動できるノードについての情報をキューに追加する．
		repe(e, g[s]) que.push({ d + e.cost, e.to, s });
	}
}


//【独立集合の数え上げ】O(2^|V| |V|)
/*
* 無向グラフ g について，頂点集合 set の部分集合のうち
* g の独立集合を成すものの個数を cnt[set] に格納する．
*
*（bit DP）
*/
void count_independent_set(const Graph& g, vm& cnt) {
	// verify : https://judge.yosupo.jp/problem/chromatic_number

	int n = sz(g);
	cnt.resize(1LL << n);

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
}


//【単純パスの数え上げ】O(2^|V| |V|^2)
/*
* グラフ g について単純パス s → t の個数を cnt[s][t] に格納する．
*
*（bit DP）
*/
void count_simple_path(const Graph& g, vvl& cnt) {
	int n = sz(g);

	// dp[s][t][set] : 単純パス s → t で途中 set を通るものの個数
	//		s !∈ set, t ∈ set とする．
	vvvl dp(n, vvl(n, vl(1LL << n)));
	vvvb seen(n, vvb(n, vb(1LL << n)));
	rep(s, n) {
		dp[s][s][0] = 1;
		seen[s][s][0] = true;
	}

	// 単純パス s → t で途中 set を通るものの個数を返す．
	function<ll(int, int, int)> rf = [&](int s, int t, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][t][set]) return dp[s][t][set];
		seen[s][t][set] = true;

		// s から行ける各頂点 v について
		repe(v, g[s]) {
			// v が set に含まれていなければ何もしない．
			if (!(set & (1 << v))) continue;

			// s → v と進む単純パスの個数を加算する．
			dp[s][t][set] += rf(v, t, set - (1 << v));
		}

		return dp[s][t][set];
	};

	// 結果の格納
	cnt = vvl(n, vl(n));
	rep(s, n) {
		rep(t, n) {
			repb(set, n) {
				if ((set & (1 << s)) || !(set & (1 << t))) continue;
				cnt[s][t] += rf(s, t, set);
			}

			// 不動の場合もカウントする．
			if (s == t) cnt[s][t]++;
		}
	}
}


//【トポロジカルソートの数え上げ】O(2^|V| |E|)
/*
* 有向グラフ g をトポロジカルソートする方法が何通りあるかを返す．
*
*（bit DP）
*/
ll count_topological_sort(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc041/tasks/abc041_d

	int n = sz(g);

	// dp[set] : 位置降順で [0..|set|) 番目の頂点までが set に対応する場合の数
	vl dp(1LL << n);
	dp[0] = 1;

	repb(set, n) {
		// s : 次に割り当てる頂点
		rep(s, n) {
			// s が既に割り当て済の頂点なら何もしない．
			if (set & (1 << s)) continue;

			// 位置降順に頂点を対応させていっているので，
			// s → t なる頂点 t は既に選ばれていなければならない．
			bool choosable = true;
			repe(t, g[s]) {
				if (!(set & (1 << t))) {
					choosable = false;
					break;
				}
			}

			if (choosable) {
				dp[set + (1 << s)] += dp[set];
			}
		}
	}

	return dp[(1 << n) - 1];
}


//【部分グラフの数え上げ】O(2^|V| |V|)
/*
* 無向グラフ g について，set を頂点集合とする部分グラフの個数を cnt[set] に格納する．
*
* 利用：【下位集合，添字 or での畳込み】
*/
void count_subgraph(const Graph& g, vm& cnt) {
	// 参考：https://drken1215.hatenablog.com/entry/2021/08/12/132500
	// verify : https://atcoder.jp/contests/abc213/tasks/abc213_g

	//【方法】
	// 明らかに
	//		cnt[set] = 2^(g の辺のうち両端点とも set に属するものの個数)
	// であるから，
	//		g[set] := g の辺のうち両端点とも set に属するものの個数
	// が求まれば良い．
	//
	// そこで，
	//		f[set] := Boole[ |set| = 2 かつそれらが g のある辺の両端点か ]
	// と定めると，
	//		g[set] = Σsub⊂set f[sub]
	// と表される．これは下位集合での高速ゼータ変換で計算可能である．

	int n = sz(g);
	cnt.resize(1LL << n);

	vi f(1LL << n); int cnt_e = 0;
	rep(s, n) {
		int set = 1 << s;

		repe(t, g[s]) {
			f[set + (1 << t)] = 1;
			cnt_e++;
		}
	}
	cnt_e /= 2;

	subset_zeta(f);

	vm pow2(cnt_e + 1);
	pow2[0] = 1;
	rep(i, cnt_e) pow2[i + 1] = pow2[i] * 2;

	repb(set, n) {
		cnt[set] = pow2[f[set]];
	}
}


//【連結部分グラフの数え上げ】O(3^|V| |V|)
/*
* 無向グラフ g について，set を頂点集合とする連結部分グラフの個数を cnt[set] に格納する．
*
* 利用：【部分グラフの数え上げ】,【下位集合の全探索】
*/
void count_connected_subgraph(const Graph& g, vm& cnt) {
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
	cnt.resize(1LL << n);

	vm cnt_all;
	count_subgraph(g, cnt_all);

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
vvm directed_cycle_decomposition(int n, int m) {
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
vvm undirected_path_decomposition(int n, int m, const Factorial_mint& fm) {
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
		res[i - j][j] = fm.binomial(i - j - 1, j - 1) * fm.factorial(i) * fm.factorial_inv(j) * pow2inv[j];
		res[i - j][j] *= fm.factorial_inv(i);
	}
	res[0][0] = 1;

	vm fac(m + 1);
	repi(j, 0, m) fac[j] = fm.factorial_inv(j);
	repi(i, 0, n) {
		res[i] = convolution(res[i], fac);
		res[i].resize(m + 1);
	}

	repir(i, n, 0) repi(j, 0, m) {
		if (j <= i) res[i][j] = res[i - j][j] * fm.factorial(i);
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
vvvm undirected_cycle_decomposition(int n, int m, const Factorial_mint& fm) {
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
		dp2[i][1][i] = fm.factorial(i - 1) * inv2 * fm.factorial_inv(i);
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
	repi(i, 0, n) repi(j, 0, m) repi(k, 0, n) res[i][j][k] *= fm.factorial(i);

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


