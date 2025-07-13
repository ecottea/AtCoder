#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "有理数.h"
// ■■■■■ ゲーム ■■■■■


//【局面の勝敗】O(?)（遅いので実験用）
/*
* 先手番での初期局面 p_ini から遷移可能な局面とその勝敗を {{手番, 局面}, 勝者} で表したリストを返す．
* nxt(t, p, nps) を呼ぶと，t=1:先手番[t=0:後手番] での局面 p から遷移可能な局面のリストを nps に格納する．
* ただし nps が空の場合は，先手勝ちなら 1，後手勝ちなら 0 を返すようにする．
*/
template <class T, class FUNC>
map<pair<int, T>, int> decide_WL(const T& p_ini, FUNC& nxt) {
	// verify : https://atcoder.jp/contests/abc380/tasks/abc380_f

	map<pair<int, T>, int> res; // これをグローバル変数にすれば再利用可能

	// t=1:先手番[t=0:後手番] で局面 p であるときの勝敗を返す．
	function<int(int, const T&)> dfs = [&](int t, const T& p) {
		// 既に勝敗が確定済ならその結果を返す．
		if (res.count({ t, p })) return res[{t, p}];

		// 局面 p から遷移可能な局面の集合 nps を得る．
		vector<T> nps;
		int wl = nxt(t, p, nps);

		// p から遷移可能な局面が無い場合は決着．
		if (nps.empty()) {
			res[{t, p}] = wl;
			return wl;
		}

		// 遷移先に自分勝ちの局面が全く無ければ相手勝ち
		res[{t, p}] = 1 - t;

		// 遷移先に自分勝ちの局面が 1 つでもあれば自分勝ち
		repe(np, nps) {
			if (dfs(1 - t, np) == t) {
				res[{t, p}] = t;

				// 最善手も知りたい場合はこれを利用する．
				//map<pair<int, T>, vector<T>> best_hands; // グローバルで宣言
				//best_hands[{t, p}].push_back(np);
	
				break; // 初期局面から遷移可能な全局面を調べたいならコメントアウトする．
			}
		}

		return res[{t, p}];
	};

	dfs(1, p_ini);

	return res;

	/* nxt の定義の雛形
	using T = tuple<ll, ll, vl>;
	auto nxt = [&](int t, const T& p, vector<T>& nps) {
		auto [l, r, a] = p;

		return 0;
	};
	*/
}


//【局面の勝敗（引き分けあり）】O(?)（遅いので実験用）
/*
* 先手番での初期局面 p_ini から遷移可能な局面とその勝敗を {{手番, 局面}, 勝者} で表したリストを返す．
* nxt(t, p, nps) を呼ぶと，t=1:先手番[t=0:後手番] での局面 p から遷移可能な局面のリストを nps に格納する．
* ただし nps が空の場合は，先手勝ちなら 1，後手勝ちなら 0，引き分けなら -1 を返すようにする．
*/
template <class T>
map<pair<int, T>, int> decide_WLD(const T& p_ini, function<int(int, const T&, vector<T>&)>& nxt) {
	// verify : https://www.codechef.com/problems/DISPDOM

	map<pair<int, T>, int> res;

	// t=1:先手番[t=0:後手番] で局面 p であるときの勝敗を返す．
	function<int(int, const T&)> dfs = [&](int t, const T& p) {
		// 既に勝敗が確定済ならその結果を返す．
		if (res.count({ t, p })) return res[{t, p}];

		// 局面 p から遷移可能な局面の集合 nps を得る．
		vector<T> nps;
		int wl = nxt(t, p, nps);

		// p から遷移可能な局面が無い場合は決着．
		if (nps.empty()) {
			res[{t, p}] = wl;
			return wl;
		}

		// 遷移先に自分勝ちの局面が 1 つでもあれば自分勝ち
		bool drawable = false;
		repe(np, nps) {
			int ret = dfs(1 - t, np);
			if (ret == t) return res[{t, p}] = t;
			if (ret == -1) drawable = true;
		}

		// そうでなく，遷移先に引き分けの局面が 1 つでもあれば引き分け
		if (drawable) return res[{t, p}] = -1;

		// そうでなければ相手勝ち
		return res[{t, p}] = 1 - t;
	};

	dfs(1, p_ini);

	return res;

	/* nxt の定義の雛形
	using T = tuple<ll, ll, vl>;
	function<int(int, const T&, vector<T>&)> nxt = [&](int t, const T& p, vector<T>& nps) {
		auto [l, r, a] = p;

		return 0;
	};
	*/
}


//【局面のスコア】O(?)（遅いので実験用）
/*
* 先手番での初期局面 p_ini から遷移可能な局面とその最終スコアを {{手番, 局面}, スコア} で表したリストを返す．
* ただし先手はスコアの最大化を目指し，後手はスコアの最小化を目指すものとする．
* nxt(t, p, nps) を呼ぶと，t=True:先手番[t=False:後手番] での局面 p から遷移可能な局面のリストを nps に格納する．
* ただし nps が空の場合はスコアを返す．
*/
template <class T>
map<pair<bool, T>, ll> decide_final_score(const T& p_ini, function<ll(bool, const T&, vector<T>&)>& nxt) {
	map<pair<bool, T>, ll> res;

	// t=True:先手番[t=False:後手番] で局面 p であるときの勝敗を返す．
	function<ll(bool, const T&)> dfs = [&](bool t, const T& p) {
		// 既に最終スコアが確定済ならその結果を返す．
		if (res.count({ t, p })) return res[{t, p}];

		// 局面 p から遷移可能な局面の集合 nps を得る．
		vector<T> nps;
		ll sc = nxt(t, p, nps);

		// p から遷移可能な局面が無い場合は決着．
		if (nps.empty()) return res[{t, p}] = sc;

		// 先手番の場合
		if (t) {
			// 遷移先の中のスコアの最大値を現局面のスコアとする．
			sc = -INFL;
			repe(np, nps) chmax(sc, dfs(!t, np));
		}
		// 後手番の場合
		else {
			// 遷移先の中のスコアの最小値を現局面のスコアとする．
			sc = INFL;
			repe(np, nps) chmin(sc, dfs(!t, np));
		}

		return res[{t, p}] = sc;
	};

	dfs(true, p_ini);

	return res;

	/* nxt の定義の雛形
	using T = tuple<ll, vi>;
	function<ll(bool, const T&, vector<T>&)> nxt = [&](bool t, const T& p, vector<T>& nps) {
		auto [sc, a] = p;

		return 0LL;
	};
	*/
}


//【局面のゲーム値（二進有理数）】O(?)（遅いので実験用）
/*
* 局面 p のゲーム値（分母分子の型が T の二進有理数）を返す．
* nxt(p, lnps, rnps) を呼ぶと，局面 p から左[右] の手番で遷移可能な局面のリストを lnps[rnps] に格納する．
*
* 制約：p からはゲーム値が数でない局面に遷移しない．
* 
* 利用：【有理数】
*/
template <class P, class T = ll>
Frac<T> calc_advantage(const P& p, function<void(const P&, vector<P>&, vector<P>&)>& nxt) {
	// verify : https://atcoder.jp/contests/abc229/tasks/abc229_h

	map<P, Frac<T>> dp;

	// 局面 p のゲーム値を返す．
	function<Frac<T>(const P&)> dfs = [&](const P& p) {
		// ゲーム値が確定済ならその結果を返す．
		if (dp.count(p)) return dp[p];

		// 局面 p から遷移可能な局面の集合 lnps, rnps を得る．
		vector<P> lnps, rnps;
		nxt(p, lnps, rnps);

		// 左右それぞれの最も優位な選択肢のみを残す．
		Frac<T> l_max(-(T)INF), r_min((T)INF);
		repe(np, lnps) chmax(l_max, dfs(np));
		repe(np, rnps) chmin(r_min, dfs(np));

		// ゲーム値が数でない局面に遷移してはならない．
		Assert(l_max < r_min);

		// {負 | 正} = 0
		if (l_max < T(0) && T(0) < r_min) return dp[p] = Frac<T>(T(0));

		// {負1 | 負2} = -{正2 | 正1}
		T sign = 1;
		if (l_max < T(0)) {
			l_max *= T(-1);
			r_min *= T(-1);
			swap(l_max, r_min);
			sign = -1;
		}

		// {正1 | 正2} のゲーム値は，開区間内の誕生日が最小の数
		together(l_max, r_min);
		int B = msb(r_min.dnm);
		repir(b, B, 0) {
			T num = ((l_max.num >> b) + 1) << b;
			if (num < r_min.num) {
				Frac<T> res(sign * num, r_min.dnm);
				res.reduction();
				return dp[p] = res;
			}
		}

		Frac<T> res(sign * (l_max.num + r_min.num), 2 * r_min.dnm);
		return dp[p] = res;
		};

	return dfs(p);

	/* nxt の定義の雛形
	using P = vi;
	function<void(const P&, vector<P>&, vector<P>&)> nxt = [&](const P& p, vector<P>& lnps, vector<P>& rnps) {

	};
	*/
}


//【重み付き DAG 上のコマ移動ゲーム】O(n + m)　
/*
* ゲームのルール：
* 重み付き DAG g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かし，辺の重み分の得点を得る．
* コマが移動不可能になったとき，自分がより高い点数を得ていることを目指す．
*
* res[v] = {f, l} : v にコマがある状態から最善を尽くしたときの f:先手[l:後手] の点数
*/
void weighted_DAG_game(const WGraph& g, vector<pll>& res) {
	int n = sz(g);

	vb seen(n);
	res = vector<pll>(n);

	function<void(int)> dfs = [&](int s) {
		// s の情報を計算済だったら何もしない．
		if (seen[s]) return;
		seen[s] = true;

		// コマが移動不能になったら終了．
		if (g[s].empty()) {
			res[s] = { 0, 0 };
			return;
		}

		// s から出ている辺 e の情報を元に s の情報を得る．
		res[s] = { -INFL, INFL };
		repe(e, g[s]) {
			// e を辿った場合の結果を求める．
			dfs(e.to);

			// e を辿る方が利得が大きいなら更新する．
			if (res[s].first < res[e.to].second + e.cost) {
				res[s].first = res[e.to].second + e.cost;
				res[s].second = res[e.to].first;
			}
		}
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【重み付き有向グラフ上のコマ移動ゲーム】O(n + m log n)
/*
* ゲームのルール：
* 重み付き有向グラフ（閉路可）g のある頂点 v にコマが置かれている．
* 左と右は交互にコマを辺で繋がれた頂点のいずれかへ動かし，
* 通った辺の重みの総和を左は最小化，右は最大化する（終了せず +∞ になることも認める．）
* 
* コマが v にある状態から，左[右] が先手だった場合の最終スコアを sc_l[sc_r] に格納する．
*
*（後退解析）
*/
void weighted_directed_graph_game(const WGraph& g, vl& sc_l, vl& sc_h) {
	// verify : https://atcoder.jp/contests/abc261/tasks/abc261_h

	int n = sz(g);
	sc_l.resize(n);
	sc_h.resize(n);

	// g_rev : g の辺の向きを逆にし，頂点を倍にしたグラフ（偶数：左先手，奇数：右先手）
	WGraph g_rev(2 * n);
	rep(s, n) repe(e, g[s]) {
		g_rev[2 * e.to].push_back({ 2 * s + 1, e.cost });
		g_rev[2 * e.to + 1].push_back({ 2 * s, e.cost });
	}

	vl sc(2 * n, INFL);

	// rem[i] : まだ調べていない局面 i から遷移可能な局面がいくつあるか
	vi rem(2 * n);

	// 終局までのスコア昇順に局面を取り出す優先度付きキュー
	priority_queue_rev<pli> q;

	rep(t, n) {
		// 遷移可能な局面数の記録
		rem[2 * t] = rem[2 * t + 1] = sz(g[t]);

		// 遷移不能な頂点ならどちらが先手でもスコアは 0
		if (sz(g[t]) == 0) {
			sc[2 * t] = sc[2 * t + 1] = 0;

			repe(e, g_rev[2 * t]) q.push({ e.cost, e.to });
			repe(e, g_rev[2 * t + 1]) q.push({ e.cost, e.to });
		}
	}

	// 後退解析を行う．スコアが有限で確定する局面がある限り処理を行う．
	while (!q.empty()) {
		ll c; int t;
		tie(c, t) = q.top(); q.pop();

		rem[t]--;

		// 左が先手の場合（スコア最小化）
		if (t % 2 == 0) {
			// スコア昇順に局面を取り出しているので，最初に見たものが最善
			if (c >= sc[t]) continue;

			sc[t] = c;
			repe(e, g_rev[t]) q.push({ c + e.cost, e.to });
		}
		// 右が先手の場合（スコア最大化）
		else {
			// スコア昇順に局面を取り出しているので，最後に見たものが最善
			if (rem[t] == 0) {
				sc[t] = c;
				repe(e, g_rev[t]) q.push({ c + e.cost, e.to });
			}
		}
	}
	// 後退解析が終わってもスコアが有限で確定していない局面は全てスコア +∞ になる．

	rep(i, n) {
		sc_l[i] = sc[2 * i];
		sc_h[i] = sc[2 * i + 1];
	}
}


//【無向グラフ上のコマ移動ゲーム（始点任意）】
/*
* ゲームのルール：
* 無向グラフ g が与えられる．まず先手がコマを好きな頂点に配置する．
* その後は後手から始めてコマを未訪問の頂点に移動させていき，移動不可能になった方を負けとする．
* 
* このゲームの勝利条件は以下の通り：
*	後手必勝 ⇔ g に完全マッチングが存在する．
* 
* 参考 : https://37zigen.com/game-on-graph/
*/


//【無向グラフ上のコマ移動ゲーム（始点固定）】
/*
* ゲームのルール：
* 無向グラフ g の頂点 v にコマが置かれている．
* 先手から始めてコマを未訪問の頂点に移動させていき，移動不可能になった方を負けとする．
*
* このゲームの勝利条件は以下の通り：
*	後手必勝 ⇔ v を含まない g の最大マッチングが存在する．
*
* verify : https://yukicoder.me/problems/no/2476
*/


//【ハッケンブッシュ紐のゲーム値】
/*
* 左[右] が着手可能な辺の色を +1[-1] なる値で表す．根から順に
*	(k 個の c0), -c0, c2, c3, c4, ...
* と辺が繋がった局面のゲーム値は以下の式で与えられる 2 進有理数となる：
*	c0 k - c0 2^(-1) + c2 2^(-2) + c3 2^(-3) + c4 2^(-4) + ...
* 
* 参考 :『Winning Ways for Your Mathematical Plays, Volume 1』p.78
* verify : https://projecteuler.net/problem=895
*/


//【賢者ゲーム】
/*
* 賢者ゲームにおいては，
*	・各ターン i における賢者の思考（自分からみて候補が一意か）
*	・インターバル i→i+1 における共通認識の更新（候補の削減）
* に分けて考察すると良い．
* 
* verify : https://atcoder.jp/contests/wtf22-day2-open/tasks/wtf22_day2_a
*/


//【カードめくり】O(n^2 log n)
/*
* n 枚のカードからなる山札があり，上から i 枚目には a[i] が書かれている．
* 先手が x，後手が y と書かれたカードを持った状態から以下のゲームを行う：
*	山札から任意に [1..k] 枚カードを引き，最後のカードを新たに手札とする．
*	スコアは最終的な両者の手札が p, q だったとき sc(p, q) で与えられる．
* 先手はスコアの最大化，後手はスコアの最小化を目指すときの最終スコアを返す．
*/
using S3 = ll;
S3 op3(S3 a, S3 b) { return max(a, b); }
S3 e3() { return -INFL; }
using S4 = ll;
S4 op4(S4 a, S4 b) { return min(a, b); }
S4 e4() { return INFL; }
ll card_flipping_game(const vl& a, ll x, ll y, int k, function<ll(ll, ll)>& sc) {
	int n = sz(a);

	// dpx[i][j] : 手札が a[i-1], a[j-1] の状態で先手番のときの最終スコア 
	using RmQ = segtree<S4, op4, e4>;
	vector<RmQ> dpx(n + 1, RmQ(n + 1));
	repi(i, 1, n - 1) dpx[i].set(n, sc(a[i - 1], a[n - 1]));
	dpx[0].set(n, sc(x, a[n - 1]));

	// dpy[j][i] : 手札が a[i-1], a[j-1] の状態で後手番のときの最終スコア 
	using RMQ = segtree<S3, op3, e3>;
	vector<RMQ> dpy(n + 1, RMQ(n + 1));
	repi(j, 1, n - 1) dpy[j].set(n, sc(a[n - 1], a[j - 1]));
	dpy[0].set(n, sc(a[n - 1], y));

	// 全探索しているが，無駄な探索を削れば O(n k log n) にできる．
	repir(i, n - 1, 0) {
		repir(j, n - 1, 0) {
			// 先手番の可能性がある場合
			if (i == 0 || i < j) {
				ll x2 = dpy[j].prod(i + 1, min(i + k + 1, n + 1));
				dpx[i].set(j, x2);
			}

			// 後手番の可能性がある場合
			if (j == 0 || j < i) {
				ll y2 = dpx[i].prod(j + 1, min(j + k + 1, n + 1));
				dpy[j].set(i, y2);
			}
		}
	}

	return dpx[0].get(0);
}


