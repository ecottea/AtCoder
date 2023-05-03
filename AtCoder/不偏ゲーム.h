#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 不偏ゲーム ■■■■■


//【局面のニム値】O(?)（遅いので実験用）
/*
* 初期局面 p から遷移可能な局面とそのニム値のリストを返す．
* nxt(p, nps) を呼ぶと，p から遷移可能な局面のリストを nps に格納するものとする．
*/
template <class T>
map<T, int> calc_nimber(const T& p, function<void(const T&, vector<T>&)>& nxt) {
	map<T, int> nim;

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


//【最小除外数】
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


//【個数制限付きニム】O(n m)
/*
* 山から取り除ける石の個数が c[0..m) に限られるルールのニムについて，
* 各 i∈[0..n] 個の石からなる山のニム値を格納したリストを返す．
*/
vi selection_nim(const vi& c, int n) {
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

	
//【DAG 上のコマ移動ゲーム】O((|V| + |E|) log|V|)　
/*
* DAG g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点へ動かし，先に移動不可能になった方が負けとする．
* コマが各 v∈[0..n) にある状態のニム値を格納したリストを返す．
*
* 利用：【最小除外数】
*/
vi DAG_game(const Graph& g) {
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


//【有向グラフ上のコマ移動ゲーム】O(|V| + |E|)
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


//【木の辺の切断ゲーム】O(n)
/*
* r を根とする木 g について，交互に辺を切断して着手不能に陥ったほうが負けのゲームを行う．
* 各 s について部分木 s でゲームを開始した場合のニム値を格納したリストを返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
// verify : https://atcoder.jp/contests/agc017/tasks/agc017_d
void merge_gct(int& x, const int& y, int s) { x ^= y; }
int e_gct() { return 0; }
int leaf_gct(int s) { return 0; }
int apply_gct(const int& x, int s, int t) { return x + 1; }
vi tree_cut_game(const Graph& g, int r) {
	return tree_getDP_vmerge<int, merge_gct, e_gct, leaf_gct, apply_gct>(g, r);
}


//【ニム積】
/*
* Nim_product() : O(64^2 * log(64)^2)
*	初期化を行う．
*
* ull prod(ull x, ull y) : O(64^2)
*	x と y のニム積を返す．
*
* ull pow(ull x, ull n) : O(64^2 log n)
*	n 個の x のニム積を返す．
*
* ull inv(ull x) : O(64^3)
*	x のニム積逆元を返す．
*/
class Nim_product {
	// 参考 :『ON NUMBERS AND GAMES』(John H. Conway)  (pp.52-53)

	using ull = unsigned long long;

	// p[i][j] : 2^i と 2^j のニム積
	vector<vector<ull>> p;

public:
	Nim_product() : p(64, vector<ull>(64)) {
		// verify : https://judge.yosupo.jp/problem/nim_product_64

		rep(i, 64) p[0][i] = p[i][0] = 1ULL << i;

		repi(i, 1, 63) repi(j, 1, 63) {
			repir(b, 5, 0) {
				if ((i & (1 << b)) && (j & (1 << b))) {
					int i2 = i - (1 << b);
					int j2 = j - (1 << b);
					ull p2 = p[i2][j2];

					p[i][j] = p2 << (1LL << b);
					rep(k, 1LL << b) if (p2 & (1ULL << k)) p[i][j] ^= p[(1LL << b) - 1][k];
					break;
				}
				else if (i & (1 << b)) {
					int i2 = i - (1 << b);
					ull p2 = p[i2][j];

					p[i][j] = p2 << (1LL << b);
					break;
				}
				else if (j & (1 << b)) {
					int j2 = j - (1 << b);
					ull p2 = p[i][j2];

					p[i][j] = p2 << (1LL << b);
					break;
				}
			}
		}
	}

	ull prod(ull x, ull y) {
		// verify : https://judge.yosupo.jp/problem/nim_product_64

		ull res = 0;
		rep(i, 64) {
			if (!(x & (1ULL << i))) continue;
			rep(j, 64) {
				if (!(y & (1ULL << j))) continue;
				res ^= p[i][j];
			}
		}
		return res;
	}

	ull pow(ull x, ull n) {
		ull res = 1, pow2 = x;
		while (n > 0) {
			if ((n & 1) != 0) res = prod(res, pow2);
			pow2 = prod(pow2, pow2);
			n /= 2;
		}
		return res;
	}

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


