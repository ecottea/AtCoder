#pragma once
#include "header.h"
#include "座標圧縮.h"
#include "ダブリング.h"
#include "列の管理.h"
// ■■■■■ 区間に関する最適化問題 ■■■■■


//【尺取り法（群）】O(n)
/*
* 群 (S, op, e, inv) の元を要素とする与えられた列 a[0..n) について，
* 各 l∈[0..n] について f(Πa[l..r)) = true となる最大の r≦n を max_right[l] に，
* 各 r∈[0..n] について f(Πa[l..r)) = true となる最小の l≧0 を min_left[r] にそれぞれ格納する．
*
* 制約：f(e()) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void two_pointers(const vector<S>& a, const function<bool(S)>& f, vi& max_right, vi& min_left) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_d

	int n = sz(a);
	max_right.resize(n + 1); min_left.resize(n + 1);

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// val : Πa[l..r)
	S val = e();

	while (true) {
		// f( Πa[l..r) ) = true の場合
		if (f(val)) {
			// いまの l は固定された r に対して最小の l となっている．
			min_left[r] = l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			val = op(val, a[r++]);
		}
		// f( Πa[l..r) ) = false の場合
		else {
			// いまの r は固定された l に対して最大の r より 1 だけ大きい．
			max_right[l] = r - 1;

			// 左を 1 つ進める．
			val = op(inv(a[l++]), val);
		}
	}

	// いま f( Πa[l..n) ) = true なので，l をより大きくしても true となる．
	for (; l <= n; l++) max_right[l] = n;
}


//【尺取り法（モノイド）】O(n)
/*
* モノイド (S, op, e) の元を要素とする与えられた列 a[0..n) について，
* 各 l∈[0..n] について f(Πa[l..r)) = true となる最大の r≦n を max_right[l] に，
* 各 r∈[0..n] について f(Πa[l..r)) = true となる最小の l≧0 を min_left[r] にそれぞれ格納する．
*
* 制約：f(e()) = true，f は単調
*
* 利用：【キュー（モノイド）】
*/
template <class S, S(*op)(S, S), S(*e)()>
void two_pointers(const vector<S>& a, const function<bool(S)>& f, vi& max_right, vi& min_left) {
	// verify : https://yukicoder.me/problems/no/1036

	int n = sz(a);
	max_right.resize(n + 1); min_left.resize(n + 1);

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// a[l..r) の要素を入れるキュー
	Queue_SWAG<S, op, e> q;

	while (true) {
		// f( Πa[l..r) ) = true の場合
		if (f(q.prod())) {
			// いまの l は固定された r に対して最小の l となっている．
			min_left[r] = l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			q.push(a[r++]);
		}
		// f( Πa[l..r) ) = false の場合
		else {
			// いまの r は固定された l に対して最大の r より 1 だけ大きい．
			max_right[l] = r - 1;

			// 左を 1 つ進める．
			q.pop(); l++;
		}
	}

	// いま f( Πa[l..n) ) = true なので，l をより大きくしても true となる．
	for (; l <= n; l++) max_right[l] = n;
}


//【区間スケジューリング問題の双対】
/*
* 与えられた区間の集合 S について，
* 互いに共通部分をもたないように選べる区間 I ∈ S の最大個数と，
* 全ての区間が少なくとも 1 つの点を含むような点群 V の大きさの最小値は一致する．
*/


//【区間スケジューリング問題】O(n log n)
/*
* 期間 [l[i], r[i]) に着手すべき n 個の仕事を請け負える最大個数を返す．
*/
template <class T>
int interval_scheduling(const vector<T>& l, const vector<T>& r) {
	// varify : https://atcoder.jp/contests/typical-algorithm/tasks/typical_algorithm_b

	int n = sz(l);
	if (n == 0) return 0;

	// 締め切りの早い順にソートする．
	vector<pair<T, T>> rl(n);
	rep(i, n) rl[i] = { r[i], l[i] };
	sort(all(rl));

	int res = 0;

	T t = numeric_limits<T>::lowest(); // 現在時刻

	// 締め切りの早い順に仕事を見ていく．
	rep(i, n) {
		T l, r;
		tie(r, l) = rl[i];

		// 仕事の開始日が現在以降の場合はその仕事を請ける．
		if (t <= l) {
			t = r;
			res++;
		}
	}

	return res;
}


//【区間スケジューリング問題（報酬最大化）】O(n log n)
/*
* 着手期間が [l[i], r[i])，報酬が a[i] の n 個の仕事について，得られる最大報酬を返す．
*/
template <class S, class T>
T interval_scheduling(const vector<S>& l, const vector<S>& r, const vector<T>& a) {
	// verify : https://atcoder.jp/contests/code-formula-2014-final/tasks/code_formula_2014_final_d

	//【方法】
	// 仕事を頂点とし，その次に請け負える仕事への有向辺をもつグラフを考えれば，
	// DAG 上のスコア最大パスを求める問題に帰着するので，後ろから DP すれば良い．
	//
	// 左右逆に見れば，右端でソートして前から貰う DP をしていることに相当する．

	int n = sz(l);

	// 仕事を左端昇順にソートする．
	vector<tuple<S, S, T>> lra(n);
	rep(i, n) lra[i] = { l[i], r[i], a[i] };
	sort(all(lra));

	vector<S> l2(n), r2(n); vector<T> a2(n);
	rep(i, n) tie(l2[i], r2[i], a2[i]) = lra[i];

	// dp[i] : 仕事 [i..n) の中で得られる最大報酬
	vector<T> dp(n + 1);
	dp[n] = 0;

	// 貰う DP
	repir(i, n - 1, 0) {
		// 仕事 i を請ける場合
		auto it = lower_bound(all(l2), r2[i]);
		if (it == l2.end()) {
			dp[i] = a2[i];
		}
		else {
			int j = (int)distance(l2.begin(), it);
			dp[i] = a2[i] + dp[j];
		}

		// 仕事 i を請けない場合
		chmax(dp[i], dp[i + 1]);
	}

	return dp[0];
}


//【区間スケジューリング問題（期間自由，報酬最大化）】O(n log n + n max(r))
/*
* 締め切りが r[i]，所要日数が w[i]，報酬が a[i] の n 個の仕事について，
* 得られる最大報酬を返す．
*/
ll maximize_floating_interval_scheduling(const vi& r, const vi& w, const vl& a) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_k

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


//【巡回区間スケジューリング問題】O(n log n)
/*
* 一年が m 日であるとし，毎年期間 [l[i], r[i]) に着手すべき n 個の仕事を請け負える最大個数を返す．
*
* 利用：【写像の合成】
*/
template <class T>
int cyclic_interval_scheduling(T m, const vector<T>& l, const vector<T>& r) {
	// verify : https://atcoder.jp/contests/jag2013summer-warmingup/tasks/icpc2013summer_warmingUp_a

	int n = sz(l);
	int res = 0;
	if (n == 0) return res;

	// 締め切りの早い順にソートする．
	vector<pair<T, T>> rl;
	rep(i, n) {
		// 一年中休めない仕事は例外処理する．
		if (l[i] == r[i]) {
			res = 1;
			continue;
		}

		T nr = r[i] + (T)(l[i] > r[i]) * m;
		rl.emplace_back(nr, l[i]);
		rl.emplace_back(nr + m, l[i] + m);
	}
	sort(all(rl));

	n = sz(rl) / 2;
	if (n == 0) return res;

	// nx[i] : 仕事 i をこなしたあと，最も早く次の仕事をこなす場合に請け負う仕事
	vi nx(2 * n, 2 * n - 1); int pt = 0;
	rep(i, 2 * n) while (rl[pt].first <= rl[i].second) nx[pt++] = i;

	// 最初に請け負う仕事 i を決め打ちし，ダブリングを用いて請け負える最大仕事数を求める．
	Map_composite mc(nx, n);
	rep(i, n) {
		function<bool(int)> okQ = [&](int x) {
			return rl[x].first <= rl[i].second + m;
		};
		chmax(res, 1 + (int)mc.max_right(i, okQ));
	}

	return res;
}


//【区間被覆問題（貰う DP）】O(n log n + m + n log m)
/*
* 非負コスト c[i] が与えられた [0..m) 上の n 個の区間 [l[i], r[i]) がある．
* 全区間 [0..m) を被覆するために必要な最小コストを返す．
*/
ll op_icgt(ll x, ll y) { return min(x, y); }
ll e_icgt() { return INFL; }
ll interval_covering_getDP(int m, const vi& l, const vi& r, const vl& c) {
	// verify : https://atcoder.jp/contests/arc026/tasks/arc026_3

	int n = sz(l);

	// 右端の小さい順にソートする．
	vector<tuple<int, int, ll>> rlc(n);
	rep(i, n) rlc[i] = { r[i], l[i], c[i] };
	sort(all(rlc));

	// seg_i[j] : [0..i) 番目の区間で [0..j) を被覆するための最小コスト
	segtree<ll, op_icgt, e_icgt> seg(m + 1);
	seg.set(0, 0);

	// 貰う DP
	rep(i, n) {
		int r, l; ll c;
		tie(r, l, c) = rlc[i];

		// cost : 今までの区間で [0..l) を被覆するための最小コスト
		ll cost = seg.prod(l, m + 1);

		// これに c 加えれば [0..r) を被覆することができる．
		seg.set(r, min(cost + c, seg.get(r)));
		dump(seg);
	}

	return seg.get(m);
}


//【区間被覆問題（配る DP）】O(n log n + m + n log m)
/*
* 非負コスト c[i] が与えられた [0..m) 上の n 個の区間 [l[i], r[i]) がある．
* 全区間 [0..m) を被覆するために必要な最小コストを返す．
*/
ll op_icgv(ll x, ll y) { return min(x, y); }
ll e_icgv() { return INFL; }
ll act_icgv(ll f, ll x) { return min(f, x); }
ll comp_icgv(ll f, ll g) { return min(f, g); }
ll id_icgv() { return INFL; }
ll interval_covering_giveDP(int m, const vi& l, const vi& r, const vl& c) {
	// verify : https://atcoder.jp/contests/arc026/tasks/arc026_3

	int n = sz(l);

	// 左端の小さい順にソートする．
	vector<tuple<int, int, ll>> lrc(n);
	rep(i, n) lrc[i] = { l[i], r[i], c[i] };
	sort(all(lrc));

	// seg_i[j] : [0..i) 番目の区間で [0..j) を被覆するための最小コスト
	lazy_segtree<ll, op_icgv, e_icgv, ll, act_icgv, comp_icgv, id_icgv> seg(m + 1);
	seg.set(0, 0);

	// 配る DP
	rep(i, n) {
		int l, r; ll c;
		tie(l, r, c) = lrc[i];

		// cost : 今までの区間で [0..l) を被覆するための最小コスト
		ll cost = seg.get(l);

		// これに c 加えれば [0..r) を被覆することができる．
		seg.apply(0, r + 1, cost + c);
	}

	return seg.get(m);
}


//【最長共通区間】O(n log n)
/*
* 左端 l[0..n)，右端 r[0..n) をもつ n 個の実数上の区間について，
* 異なる 2 つの区間の共通部分の長さの最大値を返す（なければ 0）
* またそれを実現する区間 (l[i1], r[i1]), (l[i2], r[i2]) の例を i1, i2 に格納する（なければ -1）
*/
ll maximize_interval_intersection(const vl& l, const vl& r, int* i1 = nullptr, int* i2 = nullptr) {
	// verify : https://atcoder.jp/contests/arc119/tasks/arc119_e

	int n = sz(l);

	// 区間を左端昇順にソートする．
	vector<pll> lr(n);
	rep(i, n) lr[i] = { l[i], r[i] };
	sort(all(lr));

	ll res = 0, r_max = 0; int ri_max = -1; pii i12{ -1, -1 };

	// 区間 (l[i], r[i]) と今までみた区間との共通部分を考える．
	rep(i, n) {
		auto [l, r] = lr[i];

		// l[i] は今までみた区間のどの左端よりも右にあるので，
		// その中で右端が最大のものと組にするのが最善である．
		if (chmax(res, min(r, r_max) - l)) i12 = { ri_max, i };

		// 右端の最大値を更新する．
		if (chmax(r_max, r)) ri_max = i;
	}

	if (i1 != nullptr) tie(*i1, *i2) = i12;

	return res;
}


//【発電計画問題】O(n^2)
/*
* 時刻 [l..r) に発電機をオンにすると c[l][r] の電力が得られるときの最大電力を返す．
* ただし [l..m) と [m..r) に同時に発電機をオンにすることはできない．
*
*（左端を固定した DP）
*/
ll unit_commitment_problem(const vvl& c) {
	// verify : https://algo-method.com/tasks/317

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


//【区間の和の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) Σa[i..j)
* また最大値を与える (i, j) を ids に格納する．
*/
ll maximize_interval_sum(const vl& a, pii* ids = nullptr) {
	// verify : https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_d

	//【方法】
	// 累積和 acc[i] = Σa[0..i) を導入すれば，求める値は以下のように表される：
	//		max_(i < j) (acc[j] - acc[i])
	// すなわち【組の差の最大化】に帰着する．

	int n = sz(a);

	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	ll res = -INFL, acc_min = acc[0]; int i_min = 0;
	if (ids == nullptr) ids = new pii;

	repi(i, 1, n) {
		if (chmax(res, acc[i] - acc_min)) *ids = { i_min, i };
		if (chmin(acc_min, acc[i])) i_min = i;
	}

	return res;
}


//【区間ネスト数最大化】O(n log n)
/*
* n 個の閉区間 [l[i]..r[i]] の最大狭義ネスト数を返す．
*
*（最長増加部分列）
*/
int op_minest(int x, int y) { return max(x, y); }
int e_minest() { return 0; }
int maximize_interval_nest(const vl& l, vl r) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_target

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
	segtree<int, op_minest, e_minest> dp(m);

	rep(i, n) {
		// 右端の座標圧縮
		int j = lbpos(r, lr[i].second);

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
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_cat

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
ll op_ip(ll x, ll y) { return max(x, y); }
ll e_ip() { return -INFL; }
ll mapping_ip(ll f, ll x) { return f + x; }
ll composition_ip(ll f, ll g) { return f + g; }
ll id_ip() { return 0; }
ll interval_pinning(const vector<pii>& lr, const vl& a) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106
	// verify : https://atcoder.jp/contests/dp/tasks/dp_w

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
	lazy_segtree<ll, op_ip, e_ip, ll, mapping_ip, composition_ip, id_ip> dp(n + 1);

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


//【和が s 以下の最大区間長（要素が非負）】
/*
* 尺取法.h へ
*/


//【和が s 以上の最小区間長（要素が非負）】
/*
* 尺取法.h へ
*/


