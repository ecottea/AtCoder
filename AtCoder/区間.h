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


//【区間スケジューリング問題】O(n)
/*
* 期間 [l[i], r[i]) に着手すべき n 個の仕事について，
* 最も多くの仕事をこなす場合のこなした仕事の個数を返す．
* 
*（貪欲法）
*/
int interval_scheduling(const vl& l, const vl& r) {
	int n = sz(l);

	// 締め切りの早い順にソートする．
	vector<pll> rl(n);
	rep(i, n) rl[i] = { r[i], l[i] };
	sort(all(rl));

	int res = 0;

	ll t = 0; // 現在

	// 締め切りの早い順に仕事を見ていく．
	rep(i, n) {
		int l, r;
		tie(r, l) = rl[i];

		// 仕事の開始日が現在以降の場合はその仕事を請ける．
		if (t <= l) {
			t = r;
			res++;
		}
	}

	return res;
}


//【区間スケジューリング問題（期間自由，報酬最大化）】O(n max(r))
/*
* 締め切りが r[i]，所要日数が w[i]，報酬が a[i] の n 個の仕事について，
* 得られる最大報酬を返す．
*
*（貪欲 DP）
*/
ll maximize_floating_interval_scheduling(const vi& r, const vi& w, const vl& a) {
	int n = sz(r);

	// 締め切りの早い順にソートする．
	vector<tuple<int, int, ll>> rwa(n);
	rep(i, n) rwa[i] = { r[i], w[i], a[i] };
	sort(all(rwa));

	int m = *max_element(all(r));

	// dp[i][j] : i 日目までに仕事 [0..j) で得られる最大報酬
	vvl dp(m + 1, vl(n + 1));

	repi(i, 1, m) {
		repi(j, 1, n) {
			// i 日目には何もしない場合
			dp[i][j] = dp[i - 1][j];

			// 仕事 j - 1 には手を付けない場合
			chmax(dp[i][j], dp[i][j - 1]);

			// 仕事 j - 1 を受ける場合
			int r, w; ll a;
			tie(r, w, a) = rwa[j - 1];
			if (w <= i && i <= r) {
				chmax(dp[i][j], dp[i - w][j - 1] + a);
			}
		}
	}

	return dp[m][n];
}


//【発電計画問題】O(n^2)
/*
* 時刻 [l..r) に発電機をオンにすると c[l][r] の電力が得られるときの最大電力を返す．
* ただし [l..m) と [m..r) に同時に発電機をオンにすることはできない．
*
*（左端を固定した DP）
*/
ll unit_commitment_problem(const vvl& c) {
	int n = sz(c);

	// dp[r] : 時刻 [0..r) に得られる最大電力
	vl dp(n + 1);

	repi(r, 1, n) {
		// 時刻 [r-1..r) に発電機を動かさない場合
		dp[r] = dp[r - 1];

		// 時刻 [r-1..r) に発電機を動かす場合
		chmax(dp[r], c[0][r]);
		repi(l, 1, r - 1) {
			chmax(dp[r], dp[l - 1] + c[l][r]);
		}
	}

	return dp[n];
}


//【区間スコア和最大化】O((n + m) log n)
/*
* ビット列 [0, n) 上の m 個の区間 [l, r] とそのスコア a が与えられる．
* 区間内に 1 があればスコア a が加算されるとき，スコアの最大値を返す．
*
*（遅延評価セグメント木で高速化したインライン DP）
*/
ll op2(ll x, ll y) { return max(x, y); }
ll e2() { return -INFL; }
ll mapping2(ll f, ll x) { return f + x; }
ll composition2(ll f, ll g) { return f + g; }
ll id2() { return 0; }
ll maximize_interval_score(int n, const vi& l, const vi& r, const vl& a) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106

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


