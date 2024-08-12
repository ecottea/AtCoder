#pragma once
#include "header.h"
#include "ヒープ.h"
// ■■■■■ 区間 DP ■■■■■


//【連鎖行列積問題】O(n^3)
/*
* 行数 d[i]，列数 d[i + 1] の n 個の行列 M[i] の積を計算する場合の最小乗算回数を返す．
*
*（区間 DP）
*/
ll matrix_chain_multiplication(vl& d) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_10_B

	int n = sz(d) - 1;

	// dp[i][j] : M[i..j] の積を計算する場合の最小乗算回数．
	vvl dp(n, vl(n, INFL));
	rep(i, n) {
		dp[i][i] = 0;
	}

	// 区間幅 w = j - i について昇順に貰う DP
	repi(w, 1, n - 1) {
		rep(i, n) {
			int j = i + w;
			if (j >= n) break;
			
			// M[i, k] * M[k + 1, j] と計算する場合
			repi(k, i, j - 1) {
				chmin(dp[i][j], dp[i][k] + dp[k + 1][j] + d[i] * d[k + 1] * d[j + 1]);
			}
		}
	}

	return dp[0][n - 1];
}


//【スライム融合】O(n^3)
/*
* 大きさ c[i] のスライム n 匹が一列に並んでいる．隣り合うスライムは合体できる．
* 合体時に大きさの和だけのコストを払うとき，全部を合体させるための最小コストを返す．
* 
*（区間 DP）
*/
ll merge_slimes(const vl& c) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_n

	//【備考】
	// 最適二分探索木の最小コストを求める問題とも解釈できる（実は O(n log n) で解ける）

	int n = sz(c);

	// acc[i] : 累積和 Σc[0..i)
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + c[i];

	// dp[l][r] : スライム [l..r) を合体させるための最小コスト
	vvl dp(n, vl(n + 1, INFL));
	rep(i, n) dp[i][i + 1] = 0;

	// 区間幅 w の小さい順に貰う DP
	repi(w, 2, n) {
		repi(l, 0, n - w) {
			int r = l + w;

			// k : [l..k) と [k..r) を最後に合体する
			repi(k, l, r - 1) {
				// [l..k) および [k..r) それぞれの合体にかかる最小コスト
				chmin(dp[l][r], dp[l][k] + dp[k][r]);
			}

			// [l..k) と [k..r) の合体にかかるコスト（k に依らない）を加算する．
			// この O(n^3) の区間 DP においてはここは何でもよい．
			dp[l][r] += acc[r] - acc[l];
		}
	}

	return dp[0][n];
}


//【最適二分探索木のコスト】O(n^2)
/*
* 探索頻度が p[i] である n 個の要素を葉に持つ順序付き二分探索木を作るときの
* コスト Σi p[i] depth[i] の最小値を返す．（depth は深さで，根の深さを 0 とする）
*
*（Monge DP）
*/
ll optimal_binary_tree_monge(const vl& p) {
	// 参考 : https://atcoder.jp/contests/atc002/tasks/atc002_c

	//【方法】
	// 発想は O(n^3) の区間 DP と同じで，最初バラバラの葉をマージしていくと考える．
	// p[l..r) ← p[l..k) + p[k..r) が最善となる k の範囲を考える．
	// 
	// p[l..r-1) ← p[l..k0) + p[k0..r-1) が最善であるならば，p[l..r) は
	// これの右が 1 つ伸びたものなので，最善の分割位置が左にずれることはない．
	// p[l+1..r) ← p[l+1..k1) + p[k1..r) が最善であるならば，p[l..r) は
	// これの左が 1 つ伸びたものなので，最善の分割位置が右にずれることはない．
	// 両者より，調べるべき k の範囲は k0 ≦ k ≦ k1 に限られる．

	//【Monge 性】
	// 区間に対する非負重み関数を f とする．f が Monge 性をもつとは，
	//		f(a∪b) + f(a∩b) ≧ f(a) + f(b)　（∀a,b : 区間）
	// を満たすことをいい，その場合は同様の区間 DP の高速化が可能となる．
	//
	// 今回の重み関数は f(a) = Σi∈a p[i] であり，Monge 性をもつ．

	int n = sz(p);

	// acc[i] : 累積和 Σp[0..i)
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + p[i];

	// dp[l][r] : 部分木 [l..r) をマージする最小コスト
	vvl dp(n + 1, vl(n + 1, INFL));
	rep(i, n) dp[i][i + 1] = 0;

	// sp[l][r] : [l..k) と [k..r) を最後にマージすることを表す．
	vvi sp(n + 1, vi(n + 1, -1));
	rep(i, n) sp[i][i + 1] = i + 1;

	// 区間幅 w の小さい順に貰う DP
	repi(w, 2, n) {
		repi(l, 0, n - w) {
			int r = l + w;

			// k : [l..k) と [k..r) を最後にマージする
			// 調べる k の範囲を 1 つ短い区間の分割位置の間に限定している．
			repi(k, sp[l][r - 1], sp[l + 1][r]) {
				// [l..k) および [k..r) それぞれのマージにかかる最小コスト
				if (chmin(dp[l][r], dp[l][k] + dp[k][r])) {
					sp[l][r] = k;
				}
			}

			// [l..k) と [k..r) のマージにかかるコスト（k に依らない）を加算する．
			dp[l][r] += acc[r] - acc[l];
		}
	}

	return dp[0][n];
}


//【最適二分探索木のコスト】O(n log n)
/*
* 探索頻度が p[i] である n 個の要素を葉に持つ順序付き二分探索木を作るときの
* コスト Σi p[i] depth[i] の最小値を返す．（depth は深さで，根の深さを 0 とする）
*
* 利用：【併合可能ヒープ（昇順）】
*/
ll hu_tucker(vl p) {
	// 参考 : https://ei1333.github.io/luzhiled/snippets/dp/hu-tucker.html
	// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

	//【方法】
	// 発想はハフマン符号と同じで，和の小さい葉を優先的にマージしていく．
	// 順序を保つ必要があるので，基本的には隣接する葉しかマージできない．
	// ただし葉と中間ノードを区別し，中間ノードを飛び越えるマージは許す．

	int n = sz(p);

	vector<Skew_heap_rev<ll>> heap(n - 1); // heap[i] : 葉 i, nxt[i] とマージできる中間ノード
	vi prv(n, -1), nxt(n, -1); // 双方向リストでの前[次]の葉の番号
	vl cost(n - 1); // 葉 i または中間ノード i を含むマージの最小コスト
	priority_queue_rev<pli> q; // マージコスト昇順に (コスト, 番号) を記録
	repi(i, 0, n - 2) {
		prv[i] = i - 1;
		nxt[i] = i + 1;
		cost[i] = p[i] + p[i + 1];
		q.push({ cost[i], i });
	}

	ll res = 0;

	repi(k, 0, n - 2) {
		// 次にマージするのは葉 i または中間ノード i で，そのコストは c
		ll c = 0; int i;
		do {
			tie(c, i) = q.top();
			q.pop();
		} while (nxt[i] == -1 || cost[i] != c); // 以前から状況が変化していたら無視
		res += c;

		// mi[ mni ] : 葉 i[ nxt[i] ] がマージに使われたか
		bool mi = false, mni = false;

		// 葉 i と中間ノード i をマージする場合
		if (!heap[i].empty() && p[i] + heap[i].top() == c) {
			heap[i].pop();
			mi = true;
		}
		// 葉 i と葉 nxt[i] をマージする場合
		else if (p[i] + p[nxt[i]] == c) {
			mi = mni = true;
		}
		else {
			ll c2 = heap[i].top();
			heap[i].pop();

			// 中間ノード i と中間ノード i をマージする場合
			if (!heap[i].empty() && heap[i].top() + c2 == c) {
				heap[i].pop();
			}
			// 中間ノード i と葉 nxt[i] をマージする場合
			else {
				mni = true;
			}
		}

		heap[i].push(c);
		if (mi) p[i] = INFL;
		if (mni) p[nxt[i]] = INFL;

		// 葉 i をマージに使ったため左の中間ノード j = prv[i] と併合する．
		if (mi && i > 0) {
			int j = prv[i];
			heap[j].merge(heap[i]); heap[i] = Skew_heap_rev<ll>();
			nxt[j] = nxt[i];
			nxt[i] = -1;
			prv[nxt[j]] = j;
			i = j;
		}

		// 葉 nxt[i] をマージに使ったため右の中間ノード j = nxt[i] と併合する．
		if (mni && nxt[i] < n - 1) {
			int j = nxt[i];
			heap[i].merge(heap[j]); heap[j] = Skew_heap_rev<ll>();
			nxt[i] = nxt[j];
			nxt[j] = -1;
			prv[nxt[i]] = i;
		}

		// 葉 i と葉 nxt[i] をマージする場合のコスト
		cost[i] = p[i] + p[nxt[i]];

		// 葉 i または葉 nxt[i] と中間ノード i をマージする場合の最小コスト
		c = heap[i].top();
		heap[i].pop();
		chmin(cost[i], min(p[i], p[nxt[i]]) + c);

		// 中間ノード i と中間ノード i をマージする場合の最小コスト
		if (!heap[i].empty()) chmin(cost[i], c + heap[i].top());

		heap[i].push(c); // 2 つ目の確認のため pop してしまった 1 つ目を戻しておく

		// これらのうち最小のもののみを記録する．
		q.push({ cost[i], i });
	}

	return res;
}


//【ダルマ落とし】O(n^3)
/*
* 重さ w[i] のブロックが塔状に積まれている．重さの差が d 以下の 2 ブロックを
* 同時に叩き出せるとき，取り除けるブロックの最大個数を返す．
* 
*（区間 DP）
*/
int daruma_otoshi(const vi& w, int d) {
	int n = sz(w);

	// dp[l][r] : ブロック [l..r) から叩き出せる最大ブロック数
	vvi dp(n + 1, vi(n + 1));

	// 区間幅 width の小さい順に貰う DP
	repi(width, 2, n) {
		repi(l, 0, n - width) {
			int r = l + width;

			// ブロック l を叩き出さない場合
			dp[l][r] = dp[l + 1][r];

			// ブロック l と l + (2 k + 1) を同時に叩き出す場合
			for (int k = 0; l + (2 * k + 1) < r; k++) {
				// w[l], w[l + (2 k + 1)] の差が d 以下でないといけない．
				if (abs(w[l] - w[l + (2 * k + 1)]) > d) continue;

				// ブロック l と l + (2 k + 1) の間が全て叩き出せないといけない．
				if (dp[l + 1][l + (2 * k + 1)] != 2 * k) continue;

				chmax(dp[l][r], (2 * k + 2) + dp[l + (2 * k + 2)][r]);
			}
		}
	}

	return dp[0][n];
}


//【iwi】O(n^3)
/*
* 'i' と 'w' からなる文字列 s に対し，連続する "iwi" を取り除くことを繰り返す．
* 取り除ける最大文字数を返す．
* 
*（区間 DP）
*/
int iwi(const string& s) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_iwi

	//【備考】
	// もし "iwi" でなく "fox" なら，取り除けるなら取り除くというのが最善になるので，
	// 3 つまで掘れるスタックを用いた線形走査で O(n) が達成できる．
	// verify : https://atcoder.jp/contests/arc108/tasks/arc108_b

	int n = sz(s);

	// dp[l][r] : s[l..r) から取り除ける文字数
	vvi dp(n + 1, vi(n + 1));

	// 区間幅 w の小さい順に貰う DP
	repi(w, 3, n) {
		repi(l, 0, n - w) {
			int r = l + w;

			// s[l] を取り除かない場合
			chmax(dp[l][r], dp[l + 1][r]);

			// s[r - 1] を取り除かない場合
			chmax(dp[l][r], dp[l][r - 1]);

			// 以降は s[l], s[r - 1] を取り除く場合のみを考えるので，
			// その文字が 'w' の場合は取り除けないから無視してよい．
			if (s[l] == 'w' || s[r - 1] == 'w') continue;

			// s[l] と s[r - 1] を別々に取り除く場合
			// 分ける場所 k を全通り試せば良い．
			repi(k, l + 1, r - 1) {
				chmax(dp[l][r], dp[l][k] + dp[k][r]);
			}

			// s[l] と s[r - 1] を同時に取り除く場合
			// 同時に消される s[k] = 'w' の可能性を全通り試せば良い．
			for (int k = l + 1; k < r; k += 3) {
				if (s[k] == 'w'
					&& dp[l + 1][k] == k - (l + 1)
					&& dp[k + 1][r - 1] == (r - 1) - (k + 1)) {
					chmax(dp[l][r], r - l);
				}
			}
		}
	}

	return dp[0][n];
}


