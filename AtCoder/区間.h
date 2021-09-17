#pragma once
#include "header.h"
// ■■■■■ 区間 ■■■■■



//【区間の結合】O(n log n)
/*
* n 個の半開区間 lr[i] = [l, r) を結合した半開区間の列を res に格納する．
* また結合した後の半開区間の個数を返す．
*/
template <class T>
int interval_union(vector<pair<T, T>>& lr, vector<pair<T, T>>& res) {
	int n = sz(lr);

	sort(all(lr));

	int m = 1;
	res.clear();
	res.push_back(lr[0]);
	repi(i, 1, n - 1) {
		// i 番目の区間の左端が結合中の区間の右端より右だった場合
		if (lr[i].first > res[m - 1].second) {
			// 区間の結合は完了したので，i 番目の区間を結合中の区間として次に進む．
			res.push_back(lr[i]);
			m++;
		}
		// i 番目の区間の左端が結合中の区間の右端より左だった場合（ちょうどを含む）
		else {
			// i 番目の区間を結合中の区間に結合し，右端を更新する．
			chmax(res[m - 1].second, lr[i].second);
		}
	}

	return m;
}


//【スライド最小値】O(n)
/*
* 長さ n の配列 a に対し a[i] からの w 個の最小値を a_min[i] に返す．
*/
template <class T>
void slide_minimum(const vector<T>& a, int w, vector<T>& a_min) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332


	int n = sz(a);
	a_min = vector<T>(n + 1 - w);

	// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
	deque<int> q;

	rep(i, n) {
		// 現在の最小値が注目区間の外に出たらデックの先頭から除去する．
		if (!q.empty() && q.front() <= i - w) {
			q.pop_front();
		}

		// 新しく区間に入る数より大きい数は最小値とはなりえないので
		// デックの末尾から除去する．
		while (!q.empty() && a[q.back()] > a[i]) {
			q.pop_back();
		}

		// 新しく区間に入る数は，今後最小値となる可能性があるので
		// デックの末尾に追加する．
		q.push_back(i);

		// 注目区間の幅が w になっていれば結果を記録する．
		if (i >= w - 1) {
			a_min[i - w + 1] = a[q.front()];
		}
	}
}


//【区間スコア和最大化】O((n + m) log n)
/*
* ビット列 [0, n) 上の m 個の区間 [l, r] とそのスコア a が与えられる．
* 区間内に 1 があればスコア a が加算されるとき，スコアの最大値を返す．
* 
*（遅延評価セグメント木で高速化したインライン DP）
*/
ll op2(ll x, ll y) { return max(x, y); } // 区間最大値を得たい
ll e2() { return -INFL; }
ll mapping2(ll f, ll x) { return f + x; } // 区間への加算を行いたい
ll composition2(ll f, ll g) { return f + g; }
ll id2() { return 0; }
ll maximize_interval_score(int n, vi& l, vi& r, vl& a) {
	int m = sz(l);

	// 区間 [l, r] のスコアが a であることを r_to_la[r] ∋ {l, a} で記録する．
	vector<vector<pil>> r_to_la(n);
	rep(i, m) {
		r_to_la[r[i]].push_back({ l[i], a[i] });
	}

	// dp : 区間最大値の計算と区間への加算ができる遅延評価セグメント木．
	// dp[j + 1] : 今まで見てきた区間の中で考えたときの，
	//   最も右の 1 の位置が j であるようなものの中での最高スコア
	//   （j + 1 = 0 は 1 が全くないことを表す．）
	lazy_segtree<ll, op2, e2, ll, mapping2, composition2, id2> dp(n + 1);

	// 1 が全くないときのスコアは 0 である．
	dp.set(0, 0);

	// 区間の右端 r について昇順に見ていく．
	rep(r, n) {
		ll a_sum = 0;
		repe(la, r_to_la[r]) {
			a_sum += la.second;
		}

		// 位置 r を 1 にする場合
		//   r を右端にもつ区間のスコアの和 A が加算される．
		//   よって今までのスコアの最大値 + A が右端位置 r の最高スコアとなる．
		//   区間最大値を必要とするので遅延評価セグメント木が有効．
		dp.set(r + 1, dp.prod(0, r + 1) + a_sum);

		// 位置 r を 0 にする場合
		//   r を右端にもつ各区間 [l, r] とそのスコア a について，
		//   最も右の 1 が [l, r) に含まれている場合は，a が加算される．
		//   区間への加算を必要とするので遅延評価セグメント木が有効．
		repe(la, r_to_la[r]) {
			dp.apply(la.first + 1, r + 1, la.second);
		}
	}

	// 右端の 1 の位置を任意としたときの最高スコアを返す．
	return dp.all_prod();
}


