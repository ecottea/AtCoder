#pragma once
#include "header.h"
#include "畳込み.h"
#include "二項係数.h"
// ■■■■■ 一般グラフのマッチング ■■■■■


//【貪欲に選べるマッチング】
/*
* コストなし無向グラフ g において以下が成り立つ：
*		マッチング s-t を含む最大マッチングが存在しない
*		⇒ ある s', t' が存在し，マッチング s-s', t-t' が作れる
* 対偶をとれば，
*		s, t いずれかのマッチング相手が他に居なければ s-t を作るのが最善
* が言えるので，貪欲にマッチングを構築することができる．
*
* verify : https://atcoder.jp/contests/agc029/tasks/agc029_b
*/


//【最小コスト完全マッチング】O(2^|V| |V|)
/*
* コスト付きグラフ g の隣接行列 adj を元に，g の最小コスト完全マッチングのコストを返す．
*
*（bit DP）
*/
ll minimum_cost_matching(const vvl& adj) {
	int n = sz(adj);
	if (n % 2 == 1) {
		return -INFL;
	}

	// dp[set] : set に含まれる頂点で作れる完全マッチングの最小コスト
	vl dp(1LL << n, INF);
	vb seen(1LL << n);
	dp[0] = 0;
	seen[0] = true;

	// set : 考慮すべき頂点の集合
	function<ll(int)> rf = [&](int set) {
		// 計算済ならその値を返す．
		if (seen[set]) {
			return dp[set];
		}
		seen[set] = true;

		// s : set で最も番号の小さい頂点
		int s = lsb(set);

		// t : s とペアになる set の頂点
		repi(t, s + 1, n - 1) {
			if (set & (1 << t)) {
				chmin(dp[set], rf(set - (1 << s) - (1 << t)) + adj[s][t]);
			}
		}

		return dp[set];
	};

	// 全頂点に対して最小コストを計算する．
	return rf((1 << n) - 1);
}


//【完全マッチングの存在判定】O(|V|^3)
/*
* 単純無向グラフ G に完全マッチングが存在するとき，辺の重みを乱数で定めたタット行列の
* 行列式は高確率で非 0 になり，存在しなければ必ず 0 になる．
* ここでいうタット行列とは，交代的な隣接行列のこととする．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2347
*/


//【異色頂点間を結ぶ完全マッチングの数え上げ（mod 998244353）】O(n (log n)^2)
/*
* 頂点の色が c[0..n) で与えられる完全グラフ K_n について，
* どのマッチングも異色頂点間を結ぶような完全マッチングの個数を返す．
*
* 利用：【階乗など（法が大きな素数）】,【複数の数列の畳込み】
* 
*（個数ごとの状態系包除原理）
*/
template <class T>
mint count_different_color_matching(const vector<T>& c) {
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


//【パスグラフの大きさ k のマッチングの数え上げ】
/*
* パスグラフ P_n の大きさ k のマッチングは bin(n-k, k) 通り存在する．
*
*（証明）
* P_(n-k) の k 個の頂点を選ぶ方法は bin(n-k, k) 通り存在する．
* 選んだ各頂点 i の右隣りに頂点 i' を追加して i と i' を結び，
* 適当に頂点の番号を振り直せば P_n の大きさ k のマッチングが得られる．
* 明らかに逆操作も可能であり，これは 1:1 対応である．
*
* 参考 : http://oeis.org/A011973
* verify : https://atcoder.jp/contests/agc005/tasks/agc005_d
*/


//【サイクルグラフの大きさ k のマッチングの数え上げ】
/*
* サイクルグラフ C_n(n>=1) の大きさ k のマッチングは n/(n-k) bin(n-k, k) 通り存在する．
*
*（証明）
* マッチングに使われない辺を 1 つ固定（固定の仕方は n 通り）してそこで切り開けば，
* P_n の大きさ k のマッチングを得ることができる．
* マッチングに使われない辺は n-k 本あるので，各マッチングは n-k 回重複して数えられる．
* よって求める場合の数は n/(n-k) bin(n-k, k) となる．
*
* 参考 : http://oeis.org/A034807
* verify : https://atcoder.jp/contests/abc214/tasks/abc214_g
*/


//【完全グラフの完全マッチングの列挙】O((2n-1)!! n)
/*
* 頂点 [0..2n) をもつ完全グラフの完全マッチング全てのリストを返す．
* 完全マッチングは n 個の頂点対のリストとして表す．
*/
vector<vector<pii>> enumerate_perfect_matching(int n) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_d

	vector<vector<pii>> mcs;

	// a[i] : 頂点 i が何番目のマッチングに属しているか（未使用なら -1）
	vi a(2 * n, -1);

	// k : 次に定めるのが何番目のマッチングか
	int k = 0;

	// mc : 作成途中のマッチング
	vector<pii> mc(n);

	// 頂点 i 以降のマッチングを見つける
	function<void(int)> rf = [&](int i) {
		// 全ての頂点をマッチし終えたら結果を格納する．
		if (i == 2 * n) {
			mcs.push_back(mc);
			return;
		}

		// 頂点 i が使用済だった場合は次の頂点へ進む．
		if (a[i] != -1) {
			rf(i + 1);
			return;
		}

		// 頂点 i を k 番目のマッチングの片方に選ぶ．
		a[i] = k;
		mc[k].first = i;

		// j : 頂点 i とマッチさせる頂点
		repi(j, i + 1, 2 * n - 1) {
			// 頂点 j が使用済だった場合は選べない．
			if (a[j] != -1) continue;

			// 頂点 j を頂点 i とマッチさせる．
			a[j] = k;
			mc[k].second = j;
			k++;

			// 次の頂点に進む．
			rf(i + 1);

			// 頂点 j を未使用に戻しておく．
			k--;
			a[j] = -1;
		}

		// 頂点 i を未使用に戻しておく．
		a[i] = -1;

		return;
	};
	rf(0);

	return mcs;
}


//【大きさ k のマッチングの列挙】O(√perm(|V|, 2k) k)
/*
* 無向グラフ g の大きさ k のマッチング全てのリストを返す．
* マッチングは n 個の頂点対のリストとして表す．
*/
vector<vector<pii>> enumerate_matching(const Graph& g, int k) {
	// verify : https://atcoder.jp/contests/arc095/tasks/arc095_c

	int n = sz(g);
	vector<vector<pii>> mcs;

	// used[v] : 頂点 v をマッチングに使用しているか
	int used = 0;

	// mc : 作成途中のマッチング
	vector<pii> mc;

	// 頂点 s 以降のマッチングを見つける
	function<void(int)> rf = [&](int s) {
		// マッチングの大きさが k になったら結果を格納して打ち切る．
		if (sz(mc) == k) {
			mcs.push_back(mc);
			return;
		}

		// 残りの頂点を全て使ってもマッチングの大きさが k に満たない場合は打ち切る．
		if (sz(mc) + (n - s - popcount(used >> s)) / 2 < k) return;

		// 頂点 s を新たなマッチングに使用しない場合
		rf(s + 1);

		// 頂点 s が使用済だった場合はこれで終わり．
		if (used & (1 << s)) return;

		// 頂点 s を j 番目のマッチングの片方に選ぶ．
		used += (1 << s);
		mc.emplace_back(s, -1);

		// t : 頂点 s とマッチさせる頂点
		repe(t, g[s]) {
			// 頂点 t が走査済または使用済だった場合は選べない．
			if (t < s || used & (1 << t)) continue;

			// 頂点 t を頂点 s とマッチさせる．
			used += (1 << t);
			mc.back().second = t;

			// 次の頂点に進む．
			rf(s + 1);

			// 頂点 t を未使用に戻しておく．
			used -= (1 << t);
		}

		// 頂点 s を未使用に戻しておく．
		mc.pop_back();
		used -= (1 << s);

		return;
	};
	rf(0);

	return mcs;
}


