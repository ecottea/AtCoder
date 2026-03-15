#pragma once
#include "header.h"
#include "数論変換.h"
#include "二項係数.h"
#include "構造(グラフ).h"
#include "行列.h"
// ■■■■■ 一般グラフのマッチング ■■■■■


//【貪欲に選べるマッチング】
/*
* 重みなし無向グラフ g において以下が成り立つ：
*		マッチング s-t を含む最大マッチングが存在しない
*		⇒ ある s', t' が存在し，マッチング s-s', t-t' が作れる
* 対偶をとれば，
*		s, t いずれかのマッチング相手が他に居なければ s-t を作るのが最善
* が言えるので，貪欲にマッチングを構築することができる．
*
* verify : https://atcoder.jp/contests/agc029/tasks/agc029_b
*/


//【最大マッチング】O(2^n n)
/*
* 与えられた無向グラフ g の最大マッチングの大きさを返す．
*
*（bit DP）
*/
int maximum_matching(const Graph& g) {
	int n = sz(g);

	// adj[s][t] : g が辺 s-t をもつか
	vvb adj(n, vb(n));
	rep(s, n) repe(t, g[s]) if (s != t) adj[s][t] = true;

	// dp[set] : 誘導部分グラフ g[set] に完全マッチングが存在するか
	vb dp(1LL << n);
	dp[0] = 1;

	// 貰う DP
	repb(set, n) {
		if (set == 0) continue;

		// s : set で最も番号の小さい頂点
		int s = lsb(set);

		// t : s とペアになる set の頂点
		int set2 = set - (1 << s);
		repis(t, set2) {
			if (!adj[s][t]) continue;
			dp[set] = dp[set] || dp[set2 - (1 << t)];
		}
	}

	int res = 0;
	repb(set, n) if (dp[set]) chmax(res, popcount(set));

	return res / 2;
}


//【最大マッチング（高速）】O(n^3)
/*
* 与えられた無向グラフ g の最大マッチングの大きさを返す．
*
* 利用：【階段行列】
*/
int maximum_matching_fast(const Graph& g) {
	// 参考 : https://kopricky.github.io/code/Academic/maximum_matching_memo.html
	
	int n = sz(g);

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<int> rnd(1, 998244352);

	// A : 重みを乱数で決めたタット行列
	Matrix<mint> A(n, n);
	rep(s, n) repe(t, g[s]) if (s < t) {
		mint w = rnd(mt);
		A[s][t] = w;
		A[t][s] = -w;
	}

	// g の最大マッチングの大きさは高確率で rank(A)/2 に等しい．
	auto rnk = reduced_row_echelon_form(A);

	return rnk / 2;
}


//【最大マッチング（高速，復元）】O(n^3)
/*
* 与えられた無向グラフ g の最大マッチングを返す．
*
* 利用：【転置】,【階段行列】,【逆行列】
*/
vector<pii> maximum_matching_reconst_fast(const Graph& g) {
	// 参考 : https://kopricky.github.io/code/Academic/maximum_matching_memo.html
	// verify : https://judge.yosupo.jp/problem/general_matching

	int n = sz(g);

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<int> rnd(1, 998244352);

	// A : 重みを乱数で決めたタット行列
	Matrix<mint> A(n, n);
	rep(s, n) repe(t, g[s]) if (s < t) {
		mint w = rnd(mt);
		A[s][t] = w;
		A[t][s] = -w;
	}

	// A を階段行列に変形しピボット位置のリストを得る．
	//	g の最大マッチングの大きさは高確率で rank(A)/2 に等しい．
	//	またそのときマッチングに使われる頂点はピボット位置の列番号に対応する．
	vector<pii> piv;
	auto A_tmp(A);
	n = reduced_row_echelon_form(A_tmp, &piv);

	// p : 最大マッチングに使われる頂点のリスト
	vi p;
	rep(i, n) p.push_back(piv[i].second);

	// T : タット行列 A の p に対応する部分
	Matrix<mint> T(n, n);
	rep(i, n) rep(j, n) T[i][j] = A[p[i]][p[j]];

	// T_inv : T の逆行列 T^(-1)
	auto T_inv = inverse_matrix(T);

	// res : マッチングのリスト
	vector<pii> res;

	// 頂点を選び尽くすまで反復する．
	while (!p.empty()) {
		// 頂点 p[n-1] と頂点 p[k] をマッチさせることができるかを順に調べていく．
		rep(k, n - 1) {
			// 辺が無いならもちろんダメ．
			if (T[k][n - 1] == 0) continue;

			//      T = [A, -C^T; C, D] とブロック分けする（A:2x2）
			// T^(-1) = [P, -Q^T; Q, R] とブロック分けする（P:2x2）

			// P が正則でない場合（⇔ P が零行列の場合）はダメ．
			if (T_inv[k][n - 1] == 0) continue;

			Matrix<mint> P(2, 2);
			P[0][0] = T_inv[k][k];
			P[0][1] = T_inv[k][n - 1];
			P[1][0] = T_inv[n - 1][k];
			P[1][1] = T_inv[n - 1][n - 1];
			auto P_inv = inverse_matrix(P);

			Matrix<mint> Q(n - 2, 2);
			rep(i, k) {
				Q[i][0] = T_inv[i][k];
				Q[i][1] = T_inv[i][n - 1];
			}
			repi(i, k + 1, n - 2) {
				Q[i - 1][0] = T_inv[i][k];
				Q[i - 1][1] = T_inv[i][n - 1];
			}

			Matrix<mint> R(n - 2, n - 2);
			rep(i, k) rep(j, k) R[i][j] = T_inv[i][j];
			repi(i, k + 1, n - 2) rep(j, k) R[i - 1][j] = T_inv[i][j];
			rep(i, k) repi(j, k + 1, n - 2) R[i][j - 1] = T_inv[i][j];
			repi(i, k + 1, n - 2) repi(j, k + 1, n - 2) R[i - 1][j - 1] = T_inv[i][j];

			// D^(-1) = R + Q P^(-1) Q^T なので，これで T^(-1) を更新する．
			T_inv = R + Q * P_inv * transpose(Q);

			// T を更新する．
			repi(i, k + 1, n - 2) rep(j, k) T[i - 1][j] = T[i][j];
			rep(i, k) repi(j, k + 1, n - 2) T[i][j - 1] = T[i][j];
			repi(i, k + 1, n - 2) repi(j, k + 1, n - 2) T[i - 1][j - 1] = T[i][j];
			T.resize(n - 2, n - 2);

			// p[k] と p[n-1] のマッチングを記録する．
			res.push_back({ p[k], p[n - 1] });

			// p を更新する．
			repi(i, k + 1, n - 2) p[i - 1] = p[i];
			p.resize(n - 2);

			// サイズを小さくして次の反復へ．
			n -= 2;
			break;
		}
	}

	return res;
}


//【functional graph の最大マッチング】O(n)
/*
* 与えられた f[0..n) について，辺 i→f[i] をもつグラフの最大マッチングの大きさを返す．
*/
int functional_graph_maximum_matching(const vi& f) {
	// verify : https://projecteuler.net/problem=871

	int n = sz(f);

	int res = 0;

	vb seen(n);

	// dp[s] : その頂点をマッチングに使うか
	vb dp(n);

	// in_deg[s] : 頂点 s の入次数
	vi in_deg(n);
	rep(s, n) in_deg[f[s]]++;

	// 入次数 0 の頂点を入れておくキュー
	queue<int> q;
	rep(s, n) if (in_deg[s] == 0) q.push(s);

	// 入次数 0 の頂点から順にマッチングに利用するか決めていく．
	while (!q.empty()) {
		int s = q.front(); q.pop();
		seen[s] = true;

		int t = f[s];

		// s をマッチングに使用していない場合，貪欲に s-t をマッチして損はしない．
		if (!dp[t] && !dp[s]) {
			dp[s] = true;
			dp[t] = true;
			res++;
		}

		// 辺 s→t を除去し，t の入次数を更新する．
		in_deg[t]--;
		if (in_deg[t] == 0) q.push(t);
	}

	// 頂点 st を含むサイクルを調べる．
	rep(st, n) {
		// 非サイクルまたはチェック済のサイクルなら何もしない．
		if (seen[st]) continue;

		// 自己ループはマッチングに使えない．
		if (f[st] == st) {
			seen[st] = true;
			continue;
		}

		// マッチングに使用できない頂点 s0 から調べ始める．
		int s0 = st;
		do {
			if (dp[s0]) break;
			s0 = f[s0];
		} while (s0 != st);

		int s = s0;
		do {
			seen[s] = true;
			int t = f[s];

			// サイクル内にマッチングに使用しない頂点が 2 つ連続している場合，貪欲にマッチさせる．
			if (!dp[s] && !dp[t]) {
				dp[s] = true;
				dp[t] = true;
				res++;
			}

			s = t;
		} while (s != s0);
	}

	return res;
}


//【最小コスト完全マッチング（隣接行列）】O(1.618^n n)
/*
* 重み付き対称隣接行列 c[0..n)[0..n) が表す無向グラフ g の完全マッチングの最小コストを返す．
*
*（bit DP）
*/
template <class T>
T minimum_cost_matching(const vector<vector<T>>& c) {
	//【方法】
	// 完全マッチングのみを考えるので，全頂点がいずれかの頂点と組になる．
	// そこで組を作るときに番号最小の頂点を優先的に選ぶことにすれば，
	// 例えば状態 0000 → 0011, 0101, 1001 としか遷移せず 0110, 1010, 1100 は無視できる．

	int n = sz(c);

	unordered_map<int, T> dp;
	dp[0] = 0;

	// メモ化再帰で貰う DP することで計算不要な状態を自動的に無視できる．
	function<int(T)> rf = [&](int set) {
		auto it = dp.find(set);
		if (it != dp.end()) return it->second;

		// s : set で最も番号の小さい頂点
		int s = lsb(set);

		// t : s とペアになる set の頂点
		T val = (T)INFL; int set2 = set - (1 << s);
		repis(t, set2) {
			chmin(val, rf(set2 - (1 << t)) + c[s][t]);
		}

		return dp[set] = val;
	};

	return rf((1 << n) - 1);
}


//【最小コストマッチング】O(2^n n)
/*
* 与えられた重み付き無向グラフ g に対し，各頂点集合 set⊂[0..n) について，
* 誘導部分グラフ g[set] の完全マッチングの最小コストを格納したリストを返す．
*
*（bit DP）
*/
vl minimum_cost_matching(const WGraph& g) {
	// verify : https://atcoder.jp/contests/abc318/tasks/abc318_d

	int n = sz(g);

	vvl adj(n, vl(n, INFL));
	rep(s, n) repe(t, g[s]) adj[s][t] = t.cost;

	// dp[set] : 誘導部分グラフ g[set] の完全マッチングの最小コスト
	vl dp(1LL << n, INFL);
	dp[0] = 0;

	// 貰う DP
	repb(set, n) {
		if (set == 0) continue;

		// s : set で最も番号の小さい頂点
		int s = lsb(set);

		// t : s とペアになる set の頂点
		int set2 = set - (1 << s);
		repis(t, set2) {
			chmin(dp[set], dp[set2 - (1 << t)] + adj[s][t]);
		}
	}

	return dp;
}


//【最小コスト完全マッチング（01 コスト）】O(n^3)
/*
* 与えられた重み付き無向グラフ g のコスト最小完全マッチングのコストを返す（なければ INF）
*
* 制約 : fm は n! まで計算可能
*
* 利用：【行列式（1 次多項式）】
*/
int minimum_cost_perfect_matching_01_fast(const WGraph& g, const Factorial_mint& fm) {
	// 参考 : https://atcoder.jp/contests/abc412/editorial/13380
	// verify : https://atcoder.jp/contests/abc412/tasks/abc412_g

	int n = sz(g);

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<int> rnd(1, 998244352);

	// zA+B : 重みを乱数で決めたタット行列（z:コスト）
	Matrix<mint> A(n, n), B(n, n);
	rep(s, n) repe(t, g[s]) if (s < t) {
		mint w = rnd(mt);
		if (t.cost) {
			A[s][t] = w;
			A[t][s] = -w;
		}
		else {
			B[s][t] = w;
			B[t][s] = -w;
		}
	}

	auto f = determinant_FPS_1deg(A, B, fm);
	
	rep(i, sz(f)) if (f[i] != 0) return i / 2;

	return INF;
}


//【完全グラフの完全マッチングの数え上げ（異色頂点間，mod 998244353）】O(n (log n)^2)
/*
* 頂点の色が c[0..n) で与えられる完全グラフ K_n について，
* どの辺も異色頂点間を結ぶような完全マッチングの個数を返す．
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
			a.back()[j] = fm.bin(ni, 2 * j) * df[j];
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


//【完全マッチングの数え上げとハフニアン】O(2^(n/2) n^2)
/*
* 無向グラフ g の完全マッチングの個数を求めたければ，
* その 0-1 隣接行列に対して行列.h の【ハフニアン】を利用すればよい．
*/


//【パスグラフのマッチングの数え上げ（大きさ毎）】
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


//【サイクルグラフのマッチングの数え上げ（大きさ毎）】
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


//【完全グラフのマッチングの列挙】O((2n-1)!! n)
/*
* n 頂点の完全グラフのマッチング全てのリストを返す．
*/
vector<vector<pii>> enumerate_perfect_matching(int n) {
	vector<vector<pii>> mcs;

	// used[v] : 頂点 v をマッチングに使用しているか
	int used = 0;

	// mc : 作成途中のマッチング
	vector<pii> mc;

	// 頂点 s 以降のマッチングを見つける
	function<void(int)> rf = [&](int s) {
		// マッチングを記録する．
		if (s == n) {
			mcs.push_back(mc);
			return;
		}

		// 頂点 s を新たなマッチングに使用しない場合
		rf(s + 1);

		// 頂点 s が使用済だった場合はこれで終わり．
		if (getb(used, s)) return;

		// 頂点 s を j 番目のマッチングの片方に選ぶ．
		used ^= (1 << s);
		mc.emplace_back(s, -1);

		// t : 頂点 s とマッチさせる頂点
		repi(t, s + 1, n - 1) {
			// 頂点 t が使用済だった場合は選べない．
			if (getb(used, t)) continue;

			// 頂点 t を頂点 s とマッチさせる．
			used ^= (1 << t);
			mc.back().second = t;

			// 次の頂点に進む．
			rf(s + 1);

			// 頂点 t を未使用に戻しておく．
			used ^= (1 << t);
		}

		// 頂点 s を未使用に戻しておく．
		mc.pop_back();
		used ^= (1 << s);

		return;
	};
	rf(0);

	return mcs;
}


//【完全グラフの完全マッチングの列挙】O((2n-1)!! n)
/*
* 頂点 v[0..2n) をもつ完全グラフの完全マッチング全てのリストを返す．
* 完全マッチングは n 個の頂点対のリストとして表す．
*/
template <class T>
vector<vector<pair<T, T>>> enumerate_perfect_matching(const vector<T>& v) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_d

	int n = sz(v) / 2;

	vector<vector<pair<T, T>>> mcs;

	// p[i] : 頂点 i が何番目のマッチングに属しているか（未使用なら -1）
	vi p(2 * n, -1);

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
		if (p[i] != -1) {
			rf(i + 1);
			return;
		}

		// 頂点 i を k 番目のマッチングの片方に選ぶ．
		p[i] = k;
		mc[k].first = v[i];

		// j : 頂点 i とマッチさせる頂点
		repi(j, i + 1, 2 * n - 1) {
			// 頂点 j が使用済だった場合は選べない．
			if (p[j] != -1) continue;

			// 頂点 j を頂点 i とマッチさせる．
			p[j] = k;
			mc[k].second = v[j];
			k++;

			// 次の頂点に進む．
			rf(i + 1);

			// 頂点 j を未使用に戻しておく．
			k--;
			p[j] = -1;
		}

		// 頂点 i を未使用に戻しておく．
		p[i] = -1;

		return;
	};
	rf(0);

	return mcs;
}


//【マッチングの列挙】O((2n-1)!! n)
/*
* 無向グラフ g のマッチング全てのリストを返す．
*/
vector<vector<pii>> enumerate_matching(const Graph& g) {
	// verify : https://atcoder.jp/contests/arc095/tasks/arc095_c

	int n = sz(g);
	vector<vector<pii>> mcs;

	// used[v] : 頂点 v をマッチングに使用しているか
	int used = 0;

	// mc : 作成途中のマッチング
	vector<pii> mc;

	// 頂点 s 以降のマッチングを見つける
	function<void(int)> rf = [&](int s) {
		// マッチングを記録する．
		if (s == n) {
			mcs.push_back(mc);
			return;
		}

		// 頂点 s を新たなマッチングに使用しない場合
		rf(s + 1);

		// 頂点 s が使用済だった場合はこれで終わり．
		if (getb(used, s)) return;

		// 頂点 s を j 番目のマッチングの片方に選ぶ．
		used ^= (1 << s);
		mc.emplace_back(s, -1);

		// t : 頂点 s とマッチさせる頂点
		repe(t, g[s]) {
			// 頂点 t が走査済または使用済だった場合は選べない．
			if (t < s || getb(used, t)) continue;

			// 頂点 t を頂点 s とマッチさせる．
			used ^= (1 << t);
			mc.back().second = t;

			// 次の頂点に進む．
			rf(s + 1);

			// 頂点 t を未使用に戻しておく．
			used ^= (1 << t);
		}

		// 頂点 s を未使用に戻しておく．
		mc.pop_back();
		used ^= (1 << s);

		return;
	};
	rf(0);

	return mcs;
}


//【マッチングの列挙（大きさ指定）】O(√perm(n, 2k) k)
/*
* 無向グラフ g の大きさ k のマッチング全てのリストを返す．
* マッチングは k 個の頂点対のリストとして表す．
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


