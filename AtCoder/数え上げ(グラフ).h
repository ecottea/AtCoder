#pragma once
#include "header.h"
#include "ゼータメビウス変換.h"
#include "畳込み.h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■


//【長さ k のパスの数え上げ】
/*
* 隣接行列を k 乗すれば，全頂点対の長さ k のパス数が O(|V|^3 log k) で求まる．
* 辺が少なく単一始点でよいなら，行列ベクトル積を繰り返し O(|E| k) で求まる．
* 
* verify : https://atcoder.jp/contests/abc244/tasks/abc244_e
*/


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
* 利用：【部分グラフの数え上げ】，【下位集合の全探索】
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


//【異色頂点間を結ぶ完全マッチングの数え上げ】
/*
* 頂点の色が c[0..n) で与えられる完全グラフ K_n について，
* どのマッチングも異色頂点間を結ぶような完全マッチングの個数を返す．
*
* 利用：【階乗など（法が大きな素数）】，【複数の数列の畳込み】
*/
template <class T> mint count_different_color_matching(const vector<T>& c) {
	// verify : https://atcoder.jp/contests/abl/tasks/abl_f

	//【方法】
	// K_n の同色頂点を結ぶ辺集合 E についての状態系包除原理を用いる．
	// 各 es⊂E について，es に属する辺を全て含む K_n の完全マッチングの個数を c[es] とすれば，
	// 求める場合の数は
	//		Σes⊂E (-1)^|es| c[es]
	//			c[es] = (n - 2 |es|)!! （es に端点を共有する辺が含まれない場合）
	//			c[es] = 0			   （そうでない場合）
	// と求められる．
	//
	// es⊂E の選び方は多すぎるので，先の式の通りに計算を行うことはできない．
	// 代わりに |es|=j であり，端点を共有する辺を含まないような es⊂E が何通りあるかを考える．
	// これが b[j] 通りであると分かれば，求める場合の数は
	//		Σj=[0..n/2] (-1)^j b[j] (n - 2 j)!!
	// と表される．
	//	
	// b[j] は K_n の同色頂点間を結ぶことだけが許される大きさ j のマッチングの数である．
	// 各色の頂点数が cnt[0..m) であるとすると，
	// b[j] は K_cnt[0] + ... + K_cnt[m-1] の大きさ j のマッチングの数である．
	// これらのグラフ間には共通する頂点は存在しないので，畳込みを使うことができる．

	int n = sz(c);
	if (n % 2 == 1) return 0;

	unordered_map<T, int> cnt;
	rep(i, n) cnt[c[i]]++;

	// df[i] : (2i-1)!!
	vm df(n / 2 + 1);
	df[0] = 1;
	repi(i, 1, n / 2) df[i] = df[i - 1] * (2 * i - 1);

	Factorial_mint fm(n);

	// a[i][j] : i 番目の色の頂点について，その中で大きさ j のマッチングを作る場合の数
	vvm a;
	repe(p, cnt) {
		int ni = p.second;
		a.push_back(vm(ni / 2 + 1));

		repi(j, 0, ni / 2) {
			a.back()[j] = fm.binomial(ni, 2 * j) * df[j];
		}
	}

	// b[j] : 同色の頂点間を結ぶ大きさ j のマッチングを作る場合の数
	vm b = multi_convoluion(a);
	b.resize(n / 2 + 1);

	mint res = 0;
	repi(j, 0, n / 2) {
		res += (j % 2 == 0 ? 1 : -1) * b[j] * df[n / 2 - j];
	}

	return res;
}


