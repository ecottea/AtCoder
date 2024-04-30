#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 不偏ゲーム ■■■■■


//【局面のニム値】O(?)（遅いので実験用）
/*
* 初期局面 p から遷移可能な局面とそのニム値の組のリストを返す．
* nxt(p, nps) を呼ぶと，p から遷移可能な局面のリストを nps に格納するものとする．
*/
template <class T>
map<T, int> calc_nimber(const T& p, function<void(const T&, vector<T>&)>& nxt) {
	map<T, int> nim; // これをグローバル変数にすれば再利用可能

	function<int(const T&)> calc_nimber = [&](const T& p) {
		if (nim.count(p)) return nim[p];

		vector<T> nps;
		nxt(p, nps);

		vi next_nimbers;
		repe(np, nps) {
			next_nimbers.push_back(calc_nimber(np));
		}
		uniq(next_nimbers);

		int i = 0;
		while (i < sz(next_nimbers) && next_nimbers[i] == i) i++;
		nim[p] = i;

		return nim[p];
	};
	calc_nimber(p);

	return nim;

	/* nxt の定義の雛形
	using T = vi;
	function<void(const T&, vector<T>&)> nxt = [&](const T& p, vector<T>& nps) {

	};
	*/
}


//【最善手】O(?)（遅いので実験用）
/*
* 初期局面 p から遷移可能な各局面 x について，
* 組 (x, x のニム値, x から最善手を選んで遷移できる局面のリスト) のリストを返す．
* nxt(p, nps) を呼ぶと，p から遷移可能な局面のリストを nps に格納するものとする．
*/
template <class T>
vector<tuple<T, int, vector<T>>> best_move(const T& p, function<void(const T&, vector<T>&)>& nxt) {
	// veirfy : https://mojacoder.app/users/tatyam/problems/yet-another-min-nim
	
	map<T, int> nim; // これをグローバル変数にすれば再利用可能

	vector<tuple<T, int, vector<T>>> best;

	function<int(const T&)> calc_nimber = [&](const T& p) {
		if (nim.count(p)) return nim[p];

		vector<T> nps;
		nxt(p, nps);
		uniq(nps);
		vector<T> win;

		vi next_nimbers;
		repe(np, nps) {
			int nimber = calc_nimber(np);
			if (nimber == 0) win.push_back(np);
			next_nimbers.push_back(nimber);
		}
		uniq(next_nimbers);
		uniq(win);

		int i = 0;
		while (i < sz(next_nimbers) && next_nimbers[i] == i) i++;
		nim[p] = i;

		if (win.empty()) best.emplace_back(p, 0, move(nps));
		else best.emplace_back(p, i, move(win));

		return i;
	};
	calc_nimber(p);

	return best;

	/* nxt の定義の雛形
	using T = vi;
	function<void(const T&, vector<T>&)> nxt = [&](const T& p, vector<T>& nps) {

	};
	*/
}


//【ニム値の多重集合】
/*
* Mex_multiset(int n) : O(n)
*	ニム値 [0..n) を記録可能な多重集合を空で初期化する．
*
* Mex_multiset(int n, vi a) : O(n + |a|)
*	ニム値 [0..n) を記録可能な多重集合を a で初期化する（a[i] ≧ n な要素は無視する）
*
* ll count(int v) : O(1)
*	ニム値 v の局面数を返す．
*
* insert(int v, ll k = 1) : O(log n)
*	ニム値 v の局面を k 個追加する（v ≧ n なら無視する）
*
* erase(int v, ll k = 1) : O(log n)
*	ニム値 v の局面を k 個まで削除する（v ≧ n なら無視する）
*
* int mex() : O(log n)
*	記録されている局面の mex を返す．
*	制約 : 結果は n 以下
*/
ll opms(ll x, ll y) { return min(x, y); }
ll ems() { return INFL; }
struct Mex_multiset {
	int n;

	// cnt[v] : ニム値 v の局面数
	using SEG = segtree<ll, opms, ems>;
	SEG cnt;

	// ニム値 [0..n) を記録可能な多重集合を空で初期化する．
	Mex_multiset(int n) : n(n) {
		// verify : https://codeforces.com/contest/1905/problem/D

		vl ini(n);
		cnt = SEG(ini);
	}

	// ニム値 [0..n) を記録可能な多重集合を a で初期化する．
	Mex_multiset(int n, const vi& a) : n(n) {
		// verify : https://atcoder.jp/contests/abc330/tasks/abc330_e

		vl ini(n);
		repe(x, a) if (x < n) ini[x]++;

		cnt = SEG(ini);
	}
	Mex_multiset() : n(0) {}

	// ニム値 v の局面数を返す．
	ll count(int v) {
		Assert(v >= 0);
		if (v >= n) return 0;
		return cnt.get(v);
	}

	// ニム値 v の局面を k 個追加する（v ≧ n なら無視する）
	void insert(int v, ll k = 1) {
		// verify : https://atcoder.jp/contests/abc330/tasks/abc330_e

		Assert(v >= 0);
		if (v >= n) return;
		cnt.set(v, cnt.get(v) + k);
	}

	// ニム値 v の局面を k 個まで削除する（v ≧ n なら無視する）
	void erase(int v, ll k = 1) {
		// verify : https://atcoder.jp/contests/abc330/tasks/abc330_e

		Assert(v >= 0);
		if (v >= n) return;
		cnt.set(v, max(cnt.get(v) - k, 0LL));
	}

	// 記録されている局面の mex を返す．
	int mex() {
		// verify : https://atcoder.jp/contests/abc330/tasks/abc330_e

		auto f = [&](ll x) { return x > 0; };
		return cnt.max_right(0, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Mex_multiset& MS) {
		rep(v, MS.n) rep(hoge, MS.cnt.get(v)) os << v << " ";
		return os;
	}
#endif
};


//【最小除外数】O(n log n)
/*
* mex a[0..n) を返す．
*/
int mex(vi a) {
	// verify : https://atcoder.jp/contests/abc349/tasks/abc349_g

	uniq(a);
	a.push_back(INF);

	rep(i, sz(a)) if (i != a[i]) return i;

	return -1;
}


//【ニム値の上界】
/*
* 局面間の遷移の総数を M とすると，局面のニム値は高々 O(√M) である．
* 
*（証明）ニム値 i の局面が現れるには，ニム値 [0..i) の局面が必要である．
* 帰納的に考えて，少なくとも i(i+1)/2 本の局面間の遷移が必要である．
* 
* verify : https://atcoder.jp/contests/agc043/tasks/agc043_c
*/


//【区間 mex】O(n log n)
/*
* 状態 i のニム値 nimber[i] が次式で与えられる場合のニム値を一括計算し nimber を返す．
*	nimber[i] = get{ nimber[j] | j ∈ [i-c[i]..i) }（直前 c[i] 個の mex）
*/
int op_rm(int a, int b) { return min(a, b); }
int e_rm() { return INF; }
vi range_mex(const vi& c) {
	int n = sz(c);
	vi nimber(n);

	// seg[v] : 今まで見てきた中でニム値 v が最後に現れた局面（なければ -1）
	vi ini(n, -1);
	ini[0] = 0;
	segtree<int, op_rm, e_rm> seg(ini);

	repi(i, 1, n - 1) {
		// [i-c[i]..i) よりも前にしか現れていない最小のニム値 v を得る．
		int v = seg.max_right(0, [&](int x) { return x >= i - c[i]; });

		// 最小除外数規則より v が局面 i のニム値である．
		nimber[i] = v;

		// ニム値 v が最後に現れた局面が i であることを記録する．
		seg.set(v, i);
	}

	return nimber;
}


//【減算ニム】O(n m)
/*
* 山から取り除ける石の個数が c[0..m) に限られるルールのニムについて，
* 各 i∈[0..n] 個の石からなる山のニム値を格納したリストを返す．
*/
vi subtraction_nim(const vi& c, int n) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_k

	int m = sz(c);
	vi nimber(n + 1);

	nimber[0] = 0;
	repi(i, 1, n) {
		// bucket[i] : ニム値 i をもつ局面に遷移可能か
		vb bucket(m + 1);

		// 局面 i から遷移可能な局面のニム値を記録する．
		rep(j, m) if (i - c[j] >= 0) bucket[nimber[i - c[j]]] = true;

		// 記録された局面のニム値の mex を求める．
		nimber[i] = 0;
		while (bucket[nimber[i]]) nimber[i]++;
	}

	return nimber;
}


//【K 以下減算ニム】
/*
* 山から取り除ける石の個数が K 以下に限られるルールのニムについて，
* i 個の石からなる山のニム値は i mod (K+1) である．
* 
* verify : https://atcoder.jp/contests/arc168/tasks/arc168_b
*/


//【約数減算ニム】
/*
* 山から取り除ける石の個数が n の約数に限られるルールのニムについて，
* n の約数でない最小の正整数を k とすると，i 個の石からなる山のニム値は i mod k である．
* 
* verify : https://codeforces.com/contest/1844/problem/D
*/


//【累乗減算ニム】
/*
* 山から取り除ける石の個数が k の累乗に限られるルールのニムについて，
* i 個の石からなる山のニム値は，k が奇数のときは明らかに i % 2 で，k が偶数のときは
*	i % 2  (i mod (k+1) < k)
*	2      (i mod (k+1) = k)
* である．
* 
* verify : https://atcoder.jp/contests/nikkei2019-ex/tasks/nikkei2019ex_h
*/


//【Octal game】
/*
* 山に対して行える操作が以下のように定められるニムを Octal game という：
*	各 i>1 について，石を i 個とるときに山を なくす/そのまま/分割する の可否を定める
* 
* Octal game には Guy-Smith periodicity theorem が知られており，
* ニム値の列に十分長いパターンが 2 度繰り返し現れたら，それ以降は周期的になる．
* 
* 参考 : https://fibonacci-freak.hatenablog.com/entry/2017/09/04/132443
* verify : https://yukicoder.me/problems/no/2285
*/


//【逆形ニム】O(n)
/*
* 初期状態で各山の石の数が a[0..n) であるような逆形ニムが先手必勝かを返す．
*/
template <class T>
bool misere_nim(const vector<T>& a) {
	// 参考 : https://www.forcia.com/blog/002362.html
	// verify : https://onlinejudge.u-aizu.ac.jp/services/room.html#RUPC2024Day1/problems/H

	//【方法】
	// 逆形ニム（手詰まりになった方が勝ちとするニム）の勝敗は次のようにして決定できる：
	//	1. 全ての山の石の数が 1 のとき，山が偶数個なら先手勝ち，奇数個なら後手勝ち
	//	2. 石の数が 2 以上の山がただ 1 つ存在するとき，先手勝ち
	//	3. 石の数の総 XOR が 0 でないとき先手勝ち，0 のとき後手勝ち

	//【証明】
	// 1. と 2. は明らか．
	// 3. のときは通常のニムと同じ手順で進めると，途中で必ず 2. の局面を経由する．
	// そのとき 2. での石の数の総 XOR は非 0 であり，先手勝ちという結果と一致するので問題ない．

	int n = sz(a);

	int cnt1 = 0, cnt2 = 0;
	rep(i, n) {
		if (a[i] == 1) cnt1++;
		else if (a[i] >= 2) cnt2++;
	}

	if (cnt2 == 0) return cnt1 % 2 == 0;
	if (cnt2 == 1) return true;

	ll g = 0;
	rep(i, n) g ^= a[i];

	return g != 0;
}


//【DAG 上のコマ移動ゲーム】O((n + m) log n)　
/*
* DAG g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点へ動かし，先に移動不可能になった方が負けとする．
* コマが各 v∈[0..n) にある状態のニム値を格納したリストを返す．
*
* 利用：【最小除外数】
*/
vi DAG_game(const Graph& g) {
	// verify : https://atcoder.jp/contests/agc043/tasks/agc043_c

	int n = sz(g);
		
	vi nimber(n); vb seen(n);

	function<int(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return nimber[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		Mex nx;
		repe(t, g[s]) nx.insert(dfs(t));
		nimber[s] = nx.get();

		return nimber[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);

	return nimber;
}


//【有向グラフ上のコマ移動ゲーム】O(n + m)
/*
* 有向グラフ（閉路可）g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点へ動かし，先に移動不可能になった方が負けとする．
* コマが各 v∈[0..n) にある状態からの結果（1:先手勝ち，0:後手勝ち，-1:引き分け）を格納したリストを返す．
*
*（後退解析）
*/
vi directed_graph_game(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc209/tasks/abc209_e

	int n = sz(g);

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) repe(t, g[s]) g_rev[t].push_back(s);

	// res[i] : 先手番で局面 i のときの勝敗（1:勝ち，0:負け，-1:引き分け）
	const int WIN = 1, LOSE = 0, DRAW = -1;
	vi res(n, DRAW);

	// rem[i] : まだ調べていない局面 i から遷移可能な局面がいくつあるか
	vi rem(n);

	// 勝敗が確定済の局面を記録しておくキュー
	queue<int> q;

	rep(i, n) {
		// 遷移可能な局面数の記録
		rem[i] = sz(g[i]);

		// 遷移不能な頂点なら負けに決定
		if (rem[i] == 0) {
			res[i] = LOSE;
			q.push(i);
		}
	}

	// 後退解析を行う．勝敗が確定する局面がある限り処理を行う．
	while (!q.empty()) {
		// 勝敗が確定している局面 t を得る．
		auto t = q.front(); q.pop();

		// t に遷移できる各局面 s について処理を行う．
		repe(s, g_rev[t]) {
			// 既に勝ちが決まっている局面なら何もしない．
			if (res[s] == WIN) continue;

			// 相手に負け局面を押し付けられるなら他の局面を調べるまでもなく勝ち．
			if (res[t] == LOSE) {
				res[s] = WIN;
				q.push(s);
			}
			else {
				// s から遷移可能な局面を 1 つ調べ終えた．
				rem[s]--;

				// もし全て調べ終えたなら，相手に勝ち局面を渡すしかないので負け．
				if (rem[s] == 0) {
					res[s] = LOSE;
					q.push(s);
				}
			}
		}
	}

	// 後退解析が終わっても勝敗が決定されていない局面は全て引き分け．
	return res;
}


//【コマ移動ゲームの後手必勝局面 → 独立集合】
/*
* コマ移動ゲームにおいて，後手必勝局面の集合は独立集合を成す．
* 
*（証明）後手必勝局面に遷移できる局面は必ず先手必勝になる．
* 
* verify : https://atcoder.jp/contests/agc043/tasks/agc043_c
*/


//【木の辺の切断ゲーム】O(n)
/*
* r を根とする木 g について，交互に辺を切断して着手不能に陥ったほうが負けのゲームを行う．
* 各 s について部分木 s でゲームを開始した場合のニム値を格納したリストを返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
// verify : https://atcoder.jp/contests/agc017/tasks/agc017_d
void merge_gct(int& x, const int& y, int s) { x ^= y; }
int leaf_gct(int s) { return 0; }
int apply_gct(const int& x, int s, int t) { return x + 1; }
vi tree_cut_game(const Graph& g, int r) {
	return tree_getDP_vmerge<int, merge_gct, leaf_gct, apply_gct>(g, r);
}


//【ニム積】
/*
* Nim_product() : O(65536)
*	初期化を行う．
*
* ull prod(ull x, ull y) : O(216)
*	x と y のニム積を返す．
*
* ull pow(ull x, ull n) : O(216 log n)
*	n 個の x のニム積を返す．
*
* ull inv(ull x) : O(216 * 64)
*	x のニム積逆元を返す．
*/
class Nim_product {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2020/04/07/195850
	// 参考 :『ON NUMBERS AND GAMES』(John H. Conway)  (pp.52-53)

	// p[i][j] : i と j のニム積
	vector<vector<ull>> p;

	// a と b のニム積を返す（ただし a, b < 2^16）
	ull prod16(ull a, ull b) {
		constexpr ull mask = (1ULL << 8) - 1;
		ull ah = a >> 8, al = a & mask;
		ull bh = b >> 8, bl = b & mask;

		ull val = (p[ah][bh] ^ p[al][bh] ^ p[ah][bl]) << 8;
		val ^= p[p[ah][bh]][1LL << 7];
		val ^= p[al][bl];
		return val;
	}

	// a と b のニム積を返す（ただし a, b < 2^32）
	ull prod32(ull a, ull b) {
		constexpr ull mask = (1ULL << 16) - 1;
		ull ah = a >> 16, al = a & mask;
		ull bh = b >> 16, bl = b & mask;

		ull val = (prod16(ah, bh) ^ prod16(al, bh) ^ prod16(ah, bl)) << 16;
		val ^= prod16(prod16(ah, bh), 1ULL << 15);
		val ^= prod16(al, bl);
		return val;
	}

	// a と b のニム積を返す（ただし a, b < 2^64）
	ull prod64(ull a, ull b) {
		constexpr ull mask = (1ULL << 32) - 1;
		ull ah = a >> 32, al = a & mask;
		ull bh = b >> 32, bl = b & mask;

		ull val = (prod32(ah, bh) ^ prod32(al, bh) ^ prod32(ah, bl)) << 32;
		val ^= prod32(prod32(ah, bh), 1ULL << 31);
		val ^= prod32(al, bl);
		return val;
	}

public:
	Nim_product() : p(256, vector<ull>(256)) {
		// verify : https://judge.yosupo.jp/problem/nim_product_64

		p[1][1] = 1;

		// [0..256) と [0..256) とのニム積を前計算する．
		int pow2 = 2;
		rep(k, 3) {
			int K = 1 << k;
			repi(a, pow2, pow2 * pow2 - 1) rep(b, pow2 * pow2) {
				int ah = a >> K, al = a & (pow2 - 1);
				int bh = b >> K, bl = b & (pow2 - 1);

				ull val = (p[ah][bh] ^ p[al][bh] ^ p[ah][bl]) << K;
				val ^= p[p[ah][bh]][1LL << (K - 1)];
				val ^= p[al][bl];
				p[a][b] = val;
			}
			rep(a, pow2) repi(b, pow2, pow2 * pow2 - 1) p[a][b] = p[b][a];

			pow2 *= pow2;
		}
	}

	// x と y のニム積を返す．
	ull prod(ull x, ull y) {
		// verify : https://judge.yosupo.jp/problem/nim_product_64

		if (x < (1ULL << 8) && y < (1ULL << 8)) return p[x][y];
		else if (x < (1ULL << 16) && y < (1ULL << 16)) return prod16(x, y);
		else if (x < (1ULL << 32) && y < (1ULL << 32)) return prod32(x, y);
		else return prod64(x, y);
	}

	// n 個の x のニム積を返す．
	ull pow(ull x, ull n) {
		ull res = 1, pow2 = x;
		while (n > 0) {
			if ((n & 1) != 0) res = prod(res, pow2);
			pow2 = prod(pow2, pow2);
			n /= 2;
		}
		return res;
	}

	// x のニム積逆元を返す．
	ull inv(ull x) {
		Assert(x > 0);

		if (x < (1ULL << 1)) return 1;
		if (x < (1ULL << 2)) return 5ULL - x;
		if (x < (1ULL << 4)) return pow(x, (1ULL << 4) - 2);
		if (x < (1ULL << 8)) return pow(x, (1ULL << 8) - 2);
		if (x < (1ULL << 16)) return pow(x, (1ULL << 16) - 2);
		if (x < (1ULL << 32)) return pow(x, (1ULL << 32) - 2);
		return pow(x, ~0ULL - 1);
	}
};


//【最小除外数（旧）】
/*
* Mex() : O(1)
*	空で初期化する．
*
* insert(int v) : O(log n)
*	ニム値 v をもつ局面を 1 つ追加する．
*
* erase(int v) : O(log n)
*	ニム値 v をもつ局面を 1 つ削除する．
*
* int get() : O(log n)
*	現在記録されている局面のニム値の mex を返す．
*/
struct Mex {
	// lrs : 連続したニム値をもつ閉区間 [l, r] の集合
	set<pii> lrs;

	// cnt[v] : ニム値 v をもつ局面の数
	unordered_map<int, int> cnt;

	// コンストラクタ（空で初期化）
	Mex() {}

	// ニム値 v をもつ局面を 1 つ追加する．
	void insert(int v) {
		// verify : https://atcoder.jp/contests/abc194/tasks/abc194_e

		// ニム値 v の局面数を 1 増やす．
		cnt[v]++;

		// 既にニム値 v の局面があったならば区間に変更はない．
		if (cnt[v] > 1) return;

		// v がその左右の区間と結合するかを調べる．
		bool ljoin = false, rjoin = false;
		auto it = lrs.upper_bound({ v, v });
		if (it != lrs.begin() && prev(it)->second == v - 1) ljoin = true;
		if (it != lrs.end() && it->first == v + 1) rjoin = true;

		// 区間の結合の仕方に応じて区間を削除，追加する．
		if (ljoin) {
			if (rjoin) {
				pii lr = { prev(it)->first, it->second };
				it = lrs.erase(it);
				lrs.erase(prev(it));
				lrs.insert(lr);
			}
			else {
				pii lr = { prev(it)->first, v };
				lrs.erase(prev(it));
				lrs.insert(lr);
			}
		}
		else {
			if (rjoin) {
				pii lr = { v, it->second };
				lrs.erase(it);
				lrs.insert(lr);
			}
			else {
				lrs.insert({ v, v });
			}
		}
	}

	// ニム値 v をもつ局面を 1 つ削除する．
	void erase(int v) {
		// verify : https://atcoder.jp/contests/abc194/tasks/abc194_e

		// ニム値 v をもつ局面がなければ何もしない．
		if (cnt[v] == 0) return;

		// ニム値 v の局面数を 1 減らす．
		cnt[v]--;

		// まだニム値 v の局面があるならば区間に変更はない．
		if (cnt[v] >= 1) return;

		// v でその左右の区間が分断されるかに応じて区間を削除，追加する．
		auto it = prev(lrs.upper_bound({ v, INF }));
		int l, r;
		tie(l, r) = *it;
		lrs.erase(it);
		if (l < v) lrs.insert({ l, v - 1 });
		if (r > v) lrs.insert({ v + 1, r });
	}

	// 現在記録されている局面のニム値の最小除外数を返す．
	int get() {
		// verify : https://atcoder.jp/contests/abc194/tasks/abc194_e

		if (lrs.empty() || lrs.begin()->first > 0) return 0;
		return lrs.begin()->second + 1;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Mex nm) {
		vi res;
		repe(p, nm.cnt) rep(hoge, p.second) res.push_back(p.first);
		sort(all(res));
		repe(v, res) os << v << " ";
		return os;
	}
#endif
};


