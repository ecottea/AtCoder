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


//【区間スケジューリング問題】O(n log n)
/*
* 期間 [l[i], r[i]) に着手すべき n 個の仕事を請け負える最大個数を返す．
* 
* なお戻り値は「全ての区間 [l[i], r[i]) を切断するための最小切断回数」にも一致する．
*
*（貪欲法）
*/
int interval_scheduling(const vl& l, const vl& r) {
	// varify : https://algo-method.com/tasks/363

	int n = sz(l);

	// 締め切りの早い順にソートする．
	vector<pll> rl(n);
	rep(i, n) rl[i] = { r[i], l[i] };
	sort(all(rl));

	int res = 0;

	ll t = -INFL; // 現在

	// 締め切りの早い順に仕事を見ていく．
	rep(i, n) {
		ll l, r;
		tie(r, l) = rl[i];

		// 仕事の開始日が現在以降の場合はその仕事を請ける．
		if (t <= l) {
			t = r;
			res++;
		}
	}

	return res;
}


//【区間スケジューリング問題（期間自由，報酬最大化）】O(n log n + n max(r))
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


//【区間ネスト数最大化】O(n log n)
/*
* n 個の閉区間 [l[i]..r[i]] の最大ネスト数を返す．
*
*（最長増加部分列）
*/
using S14 = int;
S14 op14(S14 x, S14 y) { return max(x, y); }
S14 e14() { return 0; }
using RMQ = segtree<S14, op14, e14>;
int maximize_interval_nest(const vl& l, vl r) {
	int n = sz(l);

	//【方法】
	// 左端について昇順にソートすれば，右端が真に減少しているときネストできる．
	// すなわち右端のみについての最長減少部分列問題に帰着する．
	// 
	// ただし左端が揃っているときネストできないようにするため，
	// 左端が等しいものの右端については昇順にソートしておく．
	//
	// さらに範囲が広いと困るので座標圧縮も並行して行う．

	// 左端昇順，次いで右端昇順にソートする
	vector<pll> lr(n);
	rep(i, n) lr[i] = { l[i], r[i] };
	sort(all(lr));

	// 右端を座標圧縮する準備
	uniq(r);
	int m = sz(r);

	// dp_i[j] : r[0..i) までで右端が j である最長減少部分列の長さ
	RMQ dp(m);

	rep(i, n) {
		int j = distance(r.begin(), lower_bound(all(r), lr[i].second));

		// j を右端にもてるのは，それまでの右端が j より大きいもののみ．
		// よってその中での最長減少部分列の長さを求め，それに 1 を加える．
		int len = dp.prod(j + 1, m) + 1;

		// j を右端とするより長いものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		if (len > dp.get(j)) {
			dp.set(j, len);
		}
	}

	// 右端の値を任意としたときの最長減少部分列の長さを得る．
	return dp.all_prod();
}


//【区間のオーバーラップ】O(n^2)
/*
* n 個の幅 1 の区間を順に並べる．区間 i, j が重なるとスコア a[i][j] が
* 得られるときの総スコアの最大値を返す．
* 
*（累積和，累積 max で高速化したインライン DP）
*/
ll interval_overlapping(const vvl& a) {
	int n = sz(a);

	// dp_r[l] : 区間 r までで，区間 r が区間 l 以降とのみ重なるときのスコア
	vl dp(n);

	repi(r, 1, n - 1) {
		// acc[l] : 区間 r が区間 l 以降と重なることで得られるスコアの和
		vl acc(r + 1);
		repir(l, r - 1, 0) {
			acc[l] = acc[l + 1] + a[l][r];
		}

		// s_max[l] : 区間 r - 1 が区間 l と重なる場合のスコアの最大値
		vl s_max(r + 1);
		s_max[0] = dp[0];
		repi(i, 1, r) {
			s_max[i] = max(s_max[i - 1], dp[i]);
		}

		repir(l, r, 0) {
			// 区間 r - 1 が区間 l と重なっていれば，区間 r を区間 l 以降と重ねられる．
			dp[l] = acc[l] + s_max[l];
		}
	}

	return *max_element(all(dp));
}


//【区間のピン留め】O(n + m log n)
/*
* [0, n) 上の m 個の区間 lr[i] = [l[i], r[i]] とスコア a[i] が与えられる．
* 自由にピンを配置するとき，ピン留めされた区間のスコアの和の最大値を返す．
*
*（遅延評価セグメント木で高速化したインライン DP）
*/
using S2 = ll;
S2 op2(S2 x, S2 y) { return max(x, y); }
S2 e2() { return -INFL; }
using F2 = ll;
S2 mapping2(F2 f, S2 x) { return f + x; }
F2 composition2(F2 f, F2 g) { return f + g; }
F2 id2() { return 0; }
ll interval_pinning(const vector<pii>& lr, const vl& a) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106

	int n = 0;
	int m = sz(lr);

	// 区間 [l, r] のスコアが a であることを r_to_la[r] ∋ {l, a} で記録する．
	map<int, vector<pil>> r_to_la;
	rep(i, m) {
		int l, r;
		tie(l, r) = lr[i];

		r_to_la[r].push_back({ l, a[i] });
		chmax(n, r);
	}
	n++;

	// dp_i[j + 1] : 今まで見てきた区間の中で考えたときの，
	//   最も右のピンの位置が j であるようなものの中での最高スコア
	//  （j + 1 = 0 はピンが全くないことを表す．）
	lazy_segtree<S2, op2, e2, F2, mapping2, composition2, id2> dp(n + 1);

	// 1 が全くないときのスコアは 0 である．
	dp.set(0, 0);

	// 区間の右端 r について昇順に見ていく．
	rep(r, n) {
		ll a_sum = 0;
		repe(la, r_to_la[r]) {
			a_sum += la.second;
		}

		// 位置 r にピンを打つ場合
		//   r を右端にもつ区間のスコアの和 A が加算される．
		//   よって今までのスコアの最大値 + A が右端位置 r の最高スコアとなる．
		//   区間最大値を必要とするので遅延評価セグメント木が有効．
		dp.set(r + 1, dp.prod(0, r + 1) + a_sum);

		// 位置 r にピンを打たない場合
		//   r を右端にもつ各区間 [l, r] とそのスコア a について，
		//   最も右のピンが [l, r) に含まれている場合は，a が加算される．
		//   区間への加算を必要とするので遅延評価セグメント木が有効．
		repe(la, r_to_la[r]) {
			dp.apply(la.first + 1, r + 1, la.second);
		}
	}

	// 右端のピンの位置を任意としたときの最高スコアを返す．
	return dp.all_prod();
}


