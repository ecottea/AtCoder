#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 二部グラフのマッチング ■■■■■


//【二部グラフの最大マッチング，最小辺被覆，最小点被覆】
/*
* 二部グラフ (S, T) の最大マッチングなどを求める．
*
* Bipartite_matching(int n, int m) : O(|V|)
*	S, T の要素数を n, m で初期化する．
*
* add_edge(int s, int t) : O(1)
*	s∈S と t∈T の間に辺を張る．
*
* int solve() : O( min(|V|^(2/3) (|V| + |E|), (|V| + |E|)^(3/2)) )
*	フローを流し計算を行い，最大マッチングの大きさを返す．
*	戻り値は「|最小点被覆|」，「|V| - |最小辺被覆|」，「|V| - |最大独立集合|」とも解釈できる．
*
* vector<pii> maximum_matching() : O(|E|)
*	最大マッチングに含まれる辺 {s, t} ∈ S×T のリストを返す．
*	制約：solve() の後に呼び出すこと．
*
* vector<pii> minimum_edge_covering() : O(|V| + |E|)
*	最小辺被覆に含まれる辺 {s, t} ∈ S×T のリストを返す．
*	es が辺被覆であるとは，任意の頂点がある e∈es の端点として現れることをいう．
*	制約：孤立点が存在しない．solve() の後に呼び出すこと．
*
* vvi minimum_vertex_covering() : O(|V| + |E|)
*	最小点被覆の例を具体的に求め，S の頂点を vs[0], T の頂点を vs[1] に格納し，vs を返す．
*	vs が点被覆であるとは，任意の辺がある v∈vs を端点にもつことをいう．
*	制約：孤立点が存在しない．solve() の後に呼び出すこと．
*
*（最大流問題）
*/
struct Bipartite_matching {
	// 参考 : https://qiita.com/drken/items/e805e3f514acceb87602
	// 参考 : https://qiita.com/drken/items/7f98315b56c95a6181a4

	int n, m;
	mf_graph<int> g;
	int ST, GL;

	// |S|, |T| を渡して初期化する．
	Bipartite_matching(int n, int m) : n(n), m(m) {
		// verify : https://judge.yosupo.jp/problem/bipartitematching

		g = mf_graph<int>(n + m + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = n + m;
		GL = n + m + 1;
		rep(i, n) g.add_edge(ST, i, 1);
		rep(j, m) g.add_edge(j + n, GL, 1);
	}

	// s∈S と t∈T の間に辺を張る． 
	void add_edge(int s, int t) { 
		// verify : https://judge.yosupo.jp/problem/bipartitematching
		
		g.add_edge(s, t + n, 1);
	}

	// 計算を実行し，最大マッチングの大きさを返す．
	int solve() {
		// verify : https://judge.yosupo.jp/problem/bipartitematching

		return g.flow(ST, GL);
	}

	// 最大マッチングの例を具体的に求める．
	vector<pii> maximum_matching() {
		// verify : https://judge.yosupo.jp/problem/bipartitematching

		vector<pii> es;

		repe(e, g.edges()) {
			// フローが流れている S, T 間の辺がマッチングに対応する．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
		}

		return es;
	}

	// 最小辺被覆の例を具体的に求める．
	vector<pii> minimum_edge_covering() {
		vector<pii> es;

		// マッチングに含まれない S, T の頂点の集合
		unordered_set<int> iso_s, iso_t;
		rep(i, n) if (g.get_edge(i).flow == 0) iso_s.insert(i);
		rep(j, m) if (g.get_edge(j + n).flow == 0) iso_t.insert(j + n);

		repe(e, g.edges()) {
			// マッチングに含まれる S, T の頂点はそのまま結ぶ．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
			// マッチングに含まれない S の頂点は，適当な T の頂点と結んでおく．
			else if (iso_s.count(e.from)) {
				es.push_back({ e.from, e.to - n });
				iso_s.erase(e.from);
			}
			// マッチングに含まれない T の頂点は，適当な S の頂点と結んでおく．
			else if (iso_t.count(e.to)) {
				es.push_back({ e.from, e.to - n });
				iso_t.erase(e.to);
			}
		}

		return es;
	}

	// 最小点被覆の例を具体的に求める．
	vvi minimum_vertex_covering() {
		// verify : https://judge.yosupo.jp/problem/assignment

		vvi vs(2);

		// ar[v] : g の残余グラフで ST から v に到達可能か
		vb ar = g.min_cut(ST);

		// 残余グラフで ST から到達不可能な S の頂点を選ぶ．
		rep(i, n) if (!ar[i]) vs[0].push_back(i);

		// 残余グラフで ST から到達可能な T の頂点を選ぶ．
		rep(j, m) if (ar[n + j]) vs[1].push_back(j);

		return vs;
	}
};


//【二部グラフの最小コスト最大マッチング】
/*
* コスト付き二部グラフ (S, T) の最小コスト最大マッチングを求める．
*
* Minimum_cost_bipartite_matching(int n, int m) : O(|V|)
*	S, T の要素数を n, m で初期化する．
*
* add_edge(int s, int t, ll c) : O(1)
*	s∈S と t∈T の間にコスト c の辺を張る．
*
* pil flow() : O(cost (|V| + |E|) log|V|)
*	最大マッチングの大きさと，そのうちの最小コストの組を返す．
*
* vector<pii> minimul_cost_maximum_matching() : O(|E|)
*	最小コスト最大マッチングに含まれる辺 {s, t} ∈ S×T のリストを返す．
*	制約 : flow() の後に呼び出すこと．
*
*（最小費用流問題）
*/
struct Minimum_cost_bipartite_matching {
	int n, m;
	mcf_graph<int, ll> g;
	int ST, GL;

	// |S|, |T| を渡して初期化する．
	Minimum_cost_bipartite_matching(int n, int m) : n(n), m(m) {
		g = mcf_graph<int, ll>(n + m + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = n + m;
		GL = n + m + 1;
		rep(i, n) g.add_edge(ST, i, 1, 0);
		rep(i, m) g.add_edge(n + i, GL, 1, 0);
	}

	// s∈S と t∈T の間にコスト c の辺を張る． 
	void add_edge(int s, int t, ll c) {
		// verify : https://atcoder.jp/contests/maximum-cup-2013/tasks/maximum_2013_f

		g.add_edge(s, n + t, 1, c);
	}

	// 最大マッチングの大きさと，そのうちの最小コストを返す．
	pil flow() {
		// verify : https://atcoder.jp/contests/maximum-cup-2013/tasks/maximum_2013_f

		return g.flow(ST, GL);
	}

	// 実現例を具体的に求める．
	vector<pii> minimul_cost_maximum_matching() {
		vector<pii> es;
		repe(e, g.edges()) {
			// フローが流れている S, T 間の辺がマッチングに対応する．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
		}
		return es;
	}
};


//【二部グラフの最大コスト最大マッチング】
/*
* 十分大きな定数を INF として，各コストを INF から引いたものに置き換えたグラフを作り，
* 【二部グラフの最小コスト最大マッチング】を利用する．
* 最後に (マッチングの大きさ) * INF から求まった最小コストを引けば良い．
* 
* verify : https://atcoder.jp/contests/abc247/tasks/abc247_g
*/


//【二部グラフの最小コスト辺被覆】
/*
* 【二部グラフの最小コスト最大マッチング】用のグラフをベースに，
* ST からの辺の最小流量を 1，GL への辺の最小流量を 1，容量 INF でコスト 0 の辺 GL → ST を追加
* としたグラフを構築し，一般化最小費用流問題を解けば良い．
* 
* verify : https://atcoder.jp/contests/abc231/tasks/abc231_h
*/


//【完全二部グラフの最小コスト完全マッチング】O(|S|^3 ?)
/*
* S[i], T[j] 間のコストが c[i][j] で与えられるコスト付き完全二部グラフ (S, T) について，
* 完全マッチングのうち最小のコストをもつもののコストを返す．
* また S[i], T[j] がマッチングを成すことを p[i] = j として格納する．
* 
* 利用：【二部グラフの最大マッチング，最小辺被覆，最小点被覆】
*/
ll hungarian(const vvl& c_, vi& p) {
	// 参考 : http://www.bunkyo.ac.jp/~nemoto/lecture/network/2010/matching1_2010.pdf
	// verify : https://judge.yosupo.jp/problem/assignment

	vvl c = c_;
	int n = sz(c);
	p.resize(n);

	while (true) {
		dumpel(c);

		// 各行について行の最小値を引く．
		vl y_min(n, INFL);
		rep(i, n) {
			ll x_min = *min_element(all(c[i]));
			rep(j, n) {
				c[i][j] -= x_min;
				chmin(y_min[j], c[i][j]);
			}
		}

		// 各列について列の最小値を引く．
		rep(i, n) rep(j, n) c[i][j] -= y_min[j];

		// 各行各列に存在する 0 部分の最大マッチング（大きさ k）を求める．
		Bipartite_matching bm(n, n);
		rep(i, n) rep(j, n) if (c[i][j] == 0) bm.add_edge(i, j);
		int k = bm.solve();

		// 完全マッチングが得られたら終了．
		if (k == n) {
			vector<pii> es = bm.maximum_matching();

			ll res = 0;
			repe(e, es) {
				p[e.first] = e.second;
				res += c_[e.first][e.second];
			}

			return res;
		}

		// 最小点被覆を得る（k 本の縦線または横線で全ての 0 を被覆することに対応する．）
		vvi vs = bm.minimum_vertex_covering();

		// 直線が存在しない行および列を得る．
		vvi vs_cp(2);
		vi vs_all(n); iota(all(vs_all), 0);
		rep(t, 2) set_difference(all(vs_all), all(vs[t]), inserter(vs_cp[t], vs_cp[t].end()));

		// 直線で被覆されていない部分の最小値 v_min を求める．
		ll v_min = INFL;
		repe(x, vs_cp[0]) repe(y, vs_cp[1]) chmin(v_min, c[x][y]);

		// 直線で被覆されていない部分から v_min を引く．
		repe(x, vs_cp[0]) repe(y, vs_cp[1]) c[x][y] -= v_min;

		// 両方向の直線で被覆されている部分に v_min を加える．
		repe(x, vs[0]) repe(y, vs[1]) c[x][y] += v_min;
	}

	return -1;
}


//【二部グラフの最小コスト弾性マッチング】O(|S| |T|)
/*
* コスト付き完全二部グラフ (S, T) の最小コスト弾性マッチングのコストを返す．
* またそのようなマッチングを昇順に match に格納する．
*
* c[i][j] : S[i] と T[j] の間にある辺のコスト．
* (i, j) ∈ res : S[i] と T[j] がマッチングしていることを表す．
*
*（二次元 DP）
*/
ll minimum_cost_elastic_matching(vvl& c, vector<pii>* match = nullptr) {
	// verify : https://algo-method.com/tasks/316

	// n = |S|, m = |T|
	int n = sz(c), m = sz(c[0]);

	// dp[i][j] : S[0..i) と T[0..j) の弾性マッチングの最小コスト
	vvl dp(n + 1, vl(m + 1, INFL));
	dp[0][0] = 0;

	// 貰う DP
	rep(i, n) rep(j, m) {
		// S[i] と T[j] は繋ぐしかないのでその分のコストは絶対必要になる．
		// その他に
		//		S[i] に T[j - 1] が繋がる
		//		T[j] に S[i - 1] が繋がる
		//		S[i], T[j] 共に他に繋がる頂点はなし
		// の 3 通りの場合が考えられるので，そのうち最小のものを選ぶ．
		dp[i + 1][j + 1] = min({ dp[i + 1][j], dp[i][j + 1], dp[i][j] }) + c[i][j];
	}

	// DP 復元
	if (match != nullptr) {
		match->clear();
		int i = n - 1, j = m - 1;
		while (i >= 0 && j >= 0) {
			match->push_back({ i, j });

			auto dp_min = min({ dp[i + 1][j], dp[i][j + 1], dp[i][j] });

			if (dp_min == dp[i + 1][j]) j--;
			else if (dp_min == dp[i][j + 1]) i--;
			else { i--; j--; }
		}

		reverse(all(*match));
	}

	return dp[n][m];
}


//【二部グラフの完全マッチングの数え上げ】O(2^n n)
/*
* |S| = |T| = n なる二部グラフ (S, T) の完全マッチングの個数を返す．
* e[i][j] : S[i] と T[j] の間に辺が存在するなら ex, なければ その他の値．
*
*（bit DP）
*/
template <class T>
mint count_bipartite_perfect_matching(const vector<vector<T>>& e, T ex) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035
	// verify : https://atcoder.jp/contests/dp/tasks/dp_o

	int n = sz(e);

	// dp[set] : set ⊂ T と，[0..|set|) ⊂ S との完全マッチングの個数
	vm dp(1LL << n);
	vb seen(1LL << n);

	// メモ化再帰用の関数
	function<mint(int)> rf = [&](int set) {
		// set が空集合の場合は完全マッチングは 1 通り．
		if (!set) return mint(1);

		// dp[set] が計算済ならその値を返す．
		if (seen[set]) return dp[set];
		seen[set] = true;

		// S の |set| 番目の頂点 i を得る．
		int i = popcount(set) - 1;

		// 頂点 i が j ∈ set とマッチしている場合についてループ．
		rep(j, n) {
			// (i, j) に辺がなかったり，j が set に属していなければ何もしない．
			if (e[i][j] != ex || !(set & (1 << j))) continue;

			// i と j がマッチしている場合の数を加算する．
			dp[set] += rf(set - (1 << j));
		}

		return dp[set];
	};

	// set = T として再帰関数に投げる．
	return rf((1 << n) - 1);
}


//【二部グラフの完全マッチングの数え上げとパーマネント】
/*
* 【二部グラフの完全マッチングの数え上げ】の戻り値は
* 0,1 を成分にもつ二部隣接行列 e[0..n)[0..n) のパーマネント perm(e) とも解釈できる．
*
* mod 2 では perm(e) = det(e) であり，det(e) は O(n^3) で計算できる．
* verify : https://atcoder.jp/contests/arc054/tasks/arc054_c
*/


//【二部グラフのマッチングの数え上げ】O(2^|T| |S| |T|)
/*
* 各 set⊂T について，誘導二部グラフ (S, set) の大きさ |set| のマッチングの個数を格納したリストを返す．
* e[i][j] : S[i] と T[j] の間の辺の本数
*/
template <class T>
vector<T> count_bipartite_matching(const vector<vector<T>>& e) {
	int n = sz(e), m = sz(e[0]);

	// dp[i][set] : S[0..i) と T[set] とのマッチングの個数
	vector<vector<T>> dp(n + 1, vector<T>(1LL << m));
	dp[0][0] = 1;

	rep(i, n) repb(set, m) {
		// S[i] とはマッチさせない場合
		dp[i + 1][set] += dp[i][set];

		// S[i] と T[j] をマッチさせる場合
		rep(j, m) {
			if (set & (1 << j)) continue;

			dp[i + 1][set + (1 << j)] += dp[i][set] * e[i][j];
		}
	}

	return dp[n];
}


//【二部グラフのマッチングの数え上げ（大小指定）】O(n^2)
/*
* |S| = |T| = n なる二部グラフ (S, T) の完全マッチングのうち以下の条件を満たすものの個数を返す：
* S[i] が T[i] 以前[自身, 以降] とマッチングすることを許すなら sb[i] の第 0[1, 2] ビットが 1
* T[j] が S[j] 以前[自身, 以降] とマッチングすることを許すなら tb[i] の第 0[1, 2] ビットが 1
*
*（箱根駅伝 DP）
*/
mint count_bipartite_perfect_matching_LEG(const vi& sb, const vi& tb) {
	// 参考 : https://drken1215.hatenablog.com/entry/2019/10/05/173700
	// verify : https://yukicoder.me/problems/no/1001

	int n = sz(sb);

	// dp_i[j] : S[0..i) と T[0..i) までで，マッチングが j 箇所ある場合の数
	vm dp(n + 1);
	dp[0] = 1;

	rep(i, n) {
		vm ndp(n + 1);

		repi(j, 0, n) {
			// S[i] とは T[i] 以前，T[i] とは S[i] 以前をマッチングさせる場合
			if ((sb[i] & 1) && (tb[i] & 1)) {
				if (j + 2 <= n) ndp[j + 2] += dp[j] * (i - j) * (i - j);
			}

			// S[i] とは T[i] 以前，T[i] とは S[i] 以降をマッチングさせる場合
			if ((sb[i] & 1) && (tb[i] & 4)) {
				if (j + 1 <= n) ndp[j + 1] += dp[j] * (i - j);
			}

			// S[i] とは T[i] 以降，T[i] とは S[i] 以前をマッチングさせる場合
			if ((sb[i] & 4) && (tb[i] & 1)) {
				if (j + 1 <= n) ndp[j + 1] += dp[j] * (i - j);
			}

			// S[i] とは T[i] 以降，T[i] とは S[i] 以降をマッチングさせる場合
			if ((sb[i] & 4) && (tb[i] & 4)) {
				ndp[j] += dp[j];
			}

			// S[i] と T[i] とをマッチングさせる場合
			if ((sb[i] & 2) && (tb[i] & 2)) {
				if (j + 1 <= n) ndp[j + 1] += dp[j];
			}
		}

		dp = move(ndp);
	}

	return dp[n];
}


//【木の最大マッチング】O(n)
/*
* 木 g の最大マッチングの大きさを返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_tbm = int;
void merge_tbm(T_tbm& x, const T_tbm& y, int s) { x += y; }
T_tbm e_tbm() { return 0; }
T_tbm leaf_tbm(int s) { return 0; }
T_tbm apply_tbm(const T_tbm& x, int s, int t) { return (T_tbm)(x == 0); }
int tree_maximum_matching(const Graph& g) {
	// verify : https://atcoder.jp/contests/agc014/tasks/agc014_d
	
	vector<T_tbm> dp = tree_getDP_vmerge<T_tbm, merge_tbm, e_tbm, leaf_tbm, apply_tbm>(g, 0);
	
	int res = 0;
	rep(i, sz(g)) res += (int)(dp[i] > 0);
	return res;
}


//【木の最大マッチングの共通部分】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* 頂点 s が木 g の全ての最大マッチングに含まれるかを格納したリストを返す．
*
* 利用：【全方位木 DP】
*/
using T_mmi = bool; // 根を必ず使うか
T_mmi merge_mmi(T_mmi x, T_mmi y, int s) { return x || y; }
T_mmi e_mmi() { return false; }
T_mmi leaf_mmi(int s) { return false; }
T_mmi apply_mmi(T_mmi x, int p, int s) { return !x; }
vb tree_maximum_matching_intersection(Graph& g) {
	// verify : https://atcoder.jp/contests/abc223/tasks/abc223_g

	return rerooting<T_mmi, merge_mmi, e_mmi, leaf_mmi, apply_mmi>(g);
}


//【ホールの結婚定理】
/*
* 二部グラフ (S, T) の最大マッチング M ⊂ E の大きさを求めたい．
* 
* 各 s ∈ S について，f(s) ⊂ T を
*		f(s) = {t ∈ T | (s, t) ∈ E}
*		（s と辺で結ばれた T の頂点集合）
* と定める．さらに各 X ⊂ S について f(X) ⊂ T を
*		f(X) = ∪s∈X f(s)
*		（X の少なくとも 1 つの頂点と辺で結ばれた T の頂点集合）
* と定める．
*
* このとき
*		|S| - |M| = max(|X| - |f(X)| | X ⊂ S)
*		（マッチングに入れなかった S の元の個数の最小値は，
*		  X が f(X) からはみ出る大きさの最大値に等しい．）
* が成り立つ．
* 
* また各 Y ⊂ T について
*		g(Y) = (f(X) ⊂ Y となる X についての |X| の最大値)
* と定めれば，
*		|S| - |M| = max(g(Y) - |Y| | Y ⊂ T)
* が成り立つとも言い換えられる．
* 
* 無条件では
*		|2^S| = 2^|S|, |2^T| = 2^|T|
* なので計算量的に役に立たないが，|f(2^S)| が十分小さく，
* 各 Y ⊂ f(2^S) に対する g(Y) が効率よく計算できるならば役に立つ．
* 
* verify：https://atcoder.jp/contests/arc076/tasks/arc076_d
*/


//【頂点次数が等しい二部グラフの完全マッチング分割】
/*
* 二部グラフ (S, T) で，|S| = |T| かつ全ての頂点の次数が等しい場合，
* (S, T) には必ず完全マッチングが存在し，これを帰納的に適用して辺集合を完全マッチングに分割できる．
* 
* 証明：各頂点の次数を d とする．ホールの結婚定理において，
*		∀X ⊂ S, |X| <= |f(X)|
* を示せば良い．X から出る辺は d |X| 本であり，これは f(X) に入る辺の本数以下である．
* f(X) の各頂点の次数も d なので，f(X) に入る辺の本数が d |X| 本以上となるためには
* f(X) が少なくとも |X| 個の頂点を含んでいなければならず，|X| <= |f(X)| である．
* 
* verify : https://atcoder.jp/contests/agc037/tasks/agc037_d
*/


//【点と区間の最大マッチング】O(n log n + m log m)
/*
* n 個の点 xs[i] と m 個の区間 [ls[j]..rs[j]) がある．
* ls[j] <= xs[i] < rs[j] を満たす組 (i, j) の最大個数を返し，組 (i, j) を ps に格納する．
*/
int point_interval_matching(const vl& xs, const vl& ls, const vl& rs, vector<pii>* ps = nullptr) {
	// verify : https://atcoder.jp/contests/code-festival-2014-morning-easy/tasks/code_festival_morning_easy_d

	int n = sz(xs), m = sz(ls);

	// 点を昇順にソートする．
	multiset<pli> xi;
	rep(i, n) xi.insert({ xs[i], i });

	// 区間を右端昇順にソートする．
	vector<pli> rj(m);
	rep(j, m) rj[j] = { rs[j], j };
	sort(all(rj));

	int res = 0;

	// 区間の右端昇順に点とマッチングさせていく．
	rep(j, m) {
		ll r; int id;
		tie(r, id) = rj[j];
		ll l = ls[id];

		// 区間内の点のうち最も左にあるものを見つける（無ければマッチング失敗）
		auto it = xi.lower_bound({ l, -INF });
		if (it == xi.end() || it->first >= r) continue;

		if (ps != nullptr) ps->push_back({ it->second, id });
		xi.erase(it);
		res++;
	}

	return res;
}


//【合コン最大マッチング】O(n log n + m log m)
/*
* n 人の男性それぞれの年収が inc_m[i], 女性への希望年収が dst_m[i] 以上であるとする．
* m 人の女性それぞれの年収が inc_w[j], 男性への希望年収が dst_w[j] 以上であるとする．
* これらの条件のもとで成立するカップル数の最大値を返し，成立したカップルを男女順に p に格納する．
*/
ll op_gm(ll a, ll b) { return min(a, b); }
ll e_gm() { return INFL; }
int gokon_matching(const vl& inc_m, const vl& dst_m, const vl& inc_w, const vl& dst_w, vector<pii>* p = nullptr)
{
	// verify : https://atcoder.jp/contests/arc046/tasks/arc046_c

	int n = sz(inc_m), m = sz(inc_w);

	// 男女それぞれを年収昇順にソートする．
	vector<pli> incid_m(n), incid_w(m);
	rep(i, n) incid_m[i] = { inc_m[i], i };
	rep(j, m) incid_w[j] = { inc_w[j], j };
	sort(all(incid_m));
	sort(all(incid_w));

	vl im(n), dm(n), iw(m), dw(m); vi idm(n), idw(m);
	rep(i, n) {
		idm[i] = incid_m[i].second;
		im[i] = inc_m[idm[i]];
		dm[i] = dst_m[idm[i]];
	}
	rep(j, m) {
		idw[j] = incid_w[j].second;
		iw[j] = inc_w[idw[j]];
		dw[j] = dst_w[idw[j]];
	}

	// 女性の希望年収
	segtree<ll, op_gm, e_gm> seg(dw);

	int res = 0;

	// 年収の低い男性から順に相手の女性を見つけていく．
	rep(i, n) {
		// 男性 i の希望年収以上の中で最低の年収をもつ女性 j0 を探す．
		int j0 = lbpos(iw, dm[i]);

		// そのような女性が居なければ男性 i とはカップル不成立として次へ．
		if (j0 == m) continue;

		// j : 男性 i の希望年収以上をもつ女性の中で，男 i の年収で満足できる最低年収の女性
		function<bool(ll)> f = [&](ll x) { return x > im[i]; };
		int j = seg.max_right(j0, f);

		// そのような女性が居なければ男性 i とはカップル不成立として次へ．
		if (j == m) continue;

		// 男性 i と女性 j がカップルになったので，浮気防止のため女性 j の要求水準を INFL にしておく．
		seg.set(j, INFL);
		if (p != nullptr) p->push_back({ idm[i], idw[j] });
		res++;
	}

	return res;
}


//【二部グラフの完全マッチングと順列】
/*
* |S| = |T| = n なる二部グラフ (S, T) の完全マッチング M は，
* (S(i), T(p[i])) ∈ M なる [0..n) の順列 p[0..n) と一対一に対応する．
* 
* 参考 : https://drken1215.hatenablog.com/entry/2019/10/05/173700
*/


//【二部グラフの辺彩色】O(|V| |E|)
/*
* 二部グラフ (S, T) を |S| = ns, |T| = nt で辺 (u[i], v[i]) ∈ S×T をもつものと定め，(S, T) の辺彩色数を返す．
* また具体的な辺彩色例における i 番目の辺の色を col[i] に格納する．
*（辺彩色は辺集合のマッチングへの分割ともみなせる．）
*/
int bipartite_edge_chromatic(int ns, int nt, const vi& u, const vi& v, vi& col) {
	// verify : https://codeforces.com/contest/600/problem/F

	int m = sz(u), n = ns + nt;

	// g[v][i] : 頂点 v から辺 i を通って行ける頂点
	vector<unordered_map<int, int>> g(n); vi deg(n);
	rep(i, m) {
		g[u[i]][i] = ns + v[i];
		g[ns + v[i]][i] = u[i];

		deg[u[i]]++;
		deg[ns + v[i]]++;
	}

	// d_max : g の最大次数（用いる色数）
	int d_max = *max_element(all(deg));

	// col[j] : 辺 j に塗られた色（未彩色なら -1）
	col.assign(m, -1);

	// nxt[v][j] : 頂点 v から出る色 j の辺番号（なければ -1）
	vvi nxt(n, vi(d_max, -1));

	// g の全ての辺について
	rep(s, n) repe(it, g[s]) {
		int i, t;
		tie(i, t) = it;

		if (col[i] != -1) continue;

		// ce : 辺 s-t に使える色（無ければ -1）
		// cst[0] : s に隣接する辺には塗られていないが，t に隣接する辺に塗られている色
		// cst[1] : t に隣接する辺には塗られていないが，s に隣接する辺に塗られている色
		int ce = -1; vi cst(2, -1);

		rep(c, d_max) {
			if (nxt[s][c] == -1) {
				if (nxt[t][c] == -1) {
					ce = c;
					break;
				}
				else cst[0] = c;
			}
			else if (nxt[t][c] == -1) cst[1] = c;
		}

		// 辺 s-t に使える色があった場合，暫定的にその色で塗る．
		if (ce != -1) {
			col[i] = ce;
			nxt[s][ce] = i;
			nxt[t][ce] = i;
			continue;
		}

		// 辺 s-t に使える色がなかった場合，t から伸びる色が cst[0], cst[1], ... と
		// 交互に伸びていく極大単純パスを見つけ，その色を反転する．
		int len = 0; vi is; vi vs{ t };
		while (1) {
			int c = cst[len % 2];
			int i = nxt[vs.back()][c];
			if (i == -1) break;

			vs.emplace_back(g[vs.back()][i]);
			is.emplace_back(i);
			len++;
		}

		rep(l, len) {
			int i = is[l];
			int c = cst[(l + 1) % 2];

			col[i] = c;
			nxt[vs[l]][c] = i;
			nxt[vs[l + 1]][c] = i;
		}
		nxt[vs[len]][cst[(len - 1) % 2]] = -1;

		// 辺 s-t を cst[0] で塗る．
		int c = cst[0];
		col[i] = c;
		nxt[s][c] = i;
		nxt[t][c] = i;
	}

	return d_max;
}


