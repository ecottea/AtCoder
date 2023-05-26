#pragma once
#include "header.h"
#include "ビット全探索.h"
#include "ヒープ.h"
#include "構造(木).h"
// ■■■■■ 最適化（集合の分割） ■■■■■


//【分割の和スコアの最大値】O(3^n)
/*
* [0..n) の分割 π = 凵k S_k に対するスコアが Σk a[S_k] で与えられるとする．
* 各 set⊂[0..n) について，全ての分割のスコアの最大値を格納したリストを返す．
*/
template <class T>
vector<T> set_partition_sum_score_max(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/apc001/tasks/apc001_f

	int N = sz(a);

	vector<T> dp(N);
	dp[0] = 0;

	// SoS bit DP
	rep(set, N) {
		if (set == 0) continue;

		// set1 : set から特定の要素 x を取り除いた集合
		int set1 = (set - 1) & set;

		// sub ⊂ set1 を全探索する（set - sub が x を含む）
		for (int sub = set1, tmp = 1; tmp > 0; tmp = sub, sub = (sub - 1) & set1) {
			chmax(dp[set], dp[sub] + a[set - sub]);
		}
	}

	return dp;
}


//【直径最小化クラスタリング】O(3^n k)
/*
* i と j の距離が dist[i][j] で与えられる n 点を，クラスタ直径の最大値が最小になるように
* k 個のクラスタに分割したときの直径の大きさを返す．
*
*（SoS bit DP）
*
* 利用：【下位集合の全探索】
*/
ll minimize_diameter_clustering(const vvl& dist, int k) {
	int n = sz(dist);

	// dp[set][c] : 点集合 set を c 個に分割するときの最小直径
	vvl dp(1LL << n, vl(k + 1LL, INFL));

	// 点集合 set の直径 dp[set][1] を前計算しておく．O(2^n n^2)
	repb(set, n) {
		dp[set][1] = 0;

		// set に含まれる 2 点の組全てを調べ，その距離の最大値をとる．
		rep(i, n) {
			rep(j, i) {
				if ((set & (1 << i)) && (set & (1 << j))) {
					chmax(dp[set][1], dist[i][j]);
				}
			}
		}
	}

	// SoS bit DP
	repb(set, n) {
		if (set == 0) continue;

		// set1 : set から 1 つ目の要素を取り除いた集合
		int set1 = (set - 1) & set;

		// set の 1 つ目の要素を含まない部分集合 sub を全探索する．
		repbs(sub, set1) {
			repi(c, 2, k) {
				chmin(dp[set][c], max(dp[sub][1], dp[set - sub][c - 1]));
			}
		}
	}

	return dp[(1LL << n) - 1][k];
}


//【スコア和最大化クラスタリング】O(3^n)
/*
* i と j が同クラスタに居るとスコア sc[i][j] が得られる条件で，
* n 点をいくつかのクラスタに分割して得られるスコアの最大値を返す．
*
*（SoS bit DP）
*
* 利用：【下位集合の全探索】
*/
ll maximize_sum_clustering(const vvl& sc) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_u

	int n = sz(sc);

	// dp[set] : 部分集合 set での得点の最大値
	vl dp(1LL << n);

	// SoS bit DP
	repb(set, n) {
		// set 全体を 1 つのクラスタとする場合
		rep(i, n) {
			if (!(set & (1 << i))) continue;

			rep(j, i) if (set & (1 << j)) dp[set] += sc[i][j];
		}

		// set の部分集合 sub を全探索する．
		repbs(sub, set) chmax(dp[set], dp[sub] + dp[set - sub]);
	}

	return dp[(1LL << n) - 1];
}


//【組の差の和の最小化】
/*
* 互いに異なる要素からなる集合 a[0..2n) を組に分割する．各組のスコアを 2 つの要素の差，
* 分割のスコアを各組のスコアの総和と定めるとき，スコア最大の分割は次にようにして得られる：
*	a[0..2n) をソートし，a[0..n) から 1 つ，a[n..2n) から 1 つ要素を選び組にする．
*
* verify : https://atcoder.jp/contests/arc120/tasks/arc120_d
*/


//【スコア和最大化 3 彩色】O(n log n)
/*
* 玉 i∈[0..n) を色 A[B, C] で彩色すると非負スコア a[i][b[i], c[i]] が得られるとする．
* x[y, z] 個（n=x+y+z）の玉を A[B, C] で彩色するときの最大スコアを返す．
*
* 利用：【多重集合の和（大小分離）】
*/
ll maximize_sum_coloring3(int x, int y, int z, const vl& a, const vl& b, const vl& c) {
	// verify : https://atcoder.jp/contests/agc018/tasks/agc018_c

	//【解法】
	// 全て C で彩色した場合とのスコアの差分を
	//		d[i] := a[i] - c[i], e[i] := b[i] - c[i]
	// とおくと，d から x 個，e から y 個を重複せず選びスコア和を最大化する問題となる．
	//
	// e について降順ソートすると，i < j のとき，d[i] と e[i] を共に選ばずかつ e[j] を選ぶくらいなら，
	// d[j] と e[j] を共に選ばずかつ e[i] を選んでもスコアが悪くなることはない．
	// よって e を y 個選び尽くすまでは d も e も選ばないという選択肢は考える必要がない．
	//
	// e を最後に選ぶのが e[i] だとする．
	// [0..i) については，e か d かの 2 択なので，全て e を選んだ場合とのスコアの差分を
	//		f[i] := d[i] - e[i]
	// とおき，f[i] を降順に i-y 個選ぶのが最善となる．
	// (i..n) については，d か 選ばないかの 2 択なので，d を降順に x-(i-y) 個選ぶのが最善となる．

	int n = x + y + z;

	ll c_sum = accumulate(all(c), 0LL);

	vector<pll> ed(n);
	rep(i, n) ed[i] = { b[i] - c[i], a[i] - c[i] };
	sort(all(ed), greater<pll>());

	vl d(n), e(n);
	rep(i, n) tie(e[i], d[i]) = ed[i];

	ll e_acc = 0;
	Separated_multiset_sum<ll> F, D;
	rep(i, y) {
		e_acc += e[i];
		F.insert_l(d[i] - e[i]);
	}
	repi(i, y, n - 1) {
		if (D.size_h() < x) D.insert_h(d[i]);
		else D.insert_l(d[i]);
	}

	ll de_sum = e_acc + D.sum_h();

	repi(i, y, n - 1) {
		e_acc += e[i];
		F.insert_h(d[i] - e[i]);
		D.erase_h(d[i]);

		chmax(de_sum, e_acc + F.sum_h() + D.sum_h());

		if (D.size_h() == 0) break;
	}

	return de_sum + c_sum;
}


//【ハフマン符号木】O(n log n)
/*
* n 種類の文字 i の出現頻度が p[i] > 0 であるときのハフマン符号木を構築し返す．
*
*（頻度が低い順に貪欲法）
*
* 利用：【二分木】
*/
template <class T>
Binary_Tree huffman_tree(const vector<T>& p) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_15_D

	int n = sz(p);

	// 出現頻度の低い順に文字を取り出す優先度付きキュー
	priority_queue_rev<pair<T, int>> q;

	// s[i] の左の子が l[i]，右の子が r[i] であることの記録用
	vi s, l, r;

	// ハフマン木の葉となる文字たちをキューに追加する．
	rep(i, n) {
		q.push({ p[i], i });

		// 葉であることを記録する．
		s.push_back(i);
		l.push_back(-1);
		r.push_back(-1);
	}

	// 出現頻度の低い文字 2 つを組にして部分木を作り，
	// それらを合わせた分の出現頻度をもつ新たな文字とみなすことを繰り返す．
	int i = n;
	while (sz(q) > 1) {
		// 出現頻度の低い文字 2 つを得る．
		T p1, p2; int i1, i2;
		tie(p1, i1) = q.top(); q.pop();
		tie(p2, i2) = q.top(); q.pop();

		// それらを合わせた文字を表すノードを作り，キューに追加する．
		q.push({ p1 + p2, i });

		// 親子関係を記録する．
		s.push_back(i);
		l.push_back(i1);
		r.push_back(i2);

		i++;
	}

	// ハフマン木を構築する．
	return Binary_Tree(s, l, r);
}


