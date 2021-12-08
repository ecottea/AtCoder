#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 組合せゲーム ■■■■■


//【最小除外数（mex）】
/*
* Nimber() : O(1)
*	空で初期化する．
*
* insert(v) : O(log n)
*	ニム値 v をもつ局面を 1 つ追加する．
*
* erase(v) : O(log n)
*	ニム値 v をもつ局面を 1 つ削除する．
*
* mex() : O(log n)
*	現在記録されている局面のニム値の mex を返す．
*/
struct Nimber {
	// lrs : 連続したニム値をもつ閉区間 [l, r] の集合
	set<pii> lrs;

	// cnt[v] : ニム値 v をもつ局面の数
	map<int, int> cnt;

	// コンストラクタ（空で初期化）
	Nimber() {}

	// ニム値 v をもつ局面を 1 つ追加する．
	void insert(int v) {
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
	int mex() {
		if (lrs.empty() || lrs.begin()->first > 0) return 0;
		return lrs.begin()->second + 1;
	}
};


//【区間 mex】O(n log n)
/*
* 状態 i のニム値 nimber[i] が次式で与えられる場合のニム値を一括計算する．
*	nimber[i] = mex{ nimber[j] | j ∈ [i-c[i]..i) }（直前 c[i] 個の mex）
*/
int op4(int a, int b) { return min(a, b); }
int e4() { return INF; }
void range_mex(const vi& c, vi& nimber) {
	int n = sz(c);
	nimber = vi(n);

	// seg[v] : 今まで見てきた中でニム値 v が最後に現れた局面（なければ -1）
	vi ini(n, -1);
	ini[0] = 0;
	segtree<int, op4, e4> seg(ini);

	repi(i, 1, n - 1) {
		// [i-c[i]..i) よりも前にしか現れていない最小のニム値 v を得る．
		int v = seg.max_right(0, [&](int x) { return x >= i - c[i]; });

		// 最小除外数規則より v が局面 i のニム値である．
		nimber[i] = v;

		// ニム値 v が最後に現れた局面が i であることを記録する．
		seg.set(v, i);
	}
}


//【個数制限付きニム】O(n m)
/*
* 山から取り除ける石の個数が c[0..m) に限られるルールのニムについて，
* i（<= n）個の石からなる山のニム値を nimber[i] に格納する．
*/
void selection_nim(const vi& c, int n, vi& nimber) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_k

	int m = sz(c);
	nimber.resize(n + 1);

	nimber[0] = 0;
	repi(i, 1, n) {
		// bucket[i] : ニム値 i をもつ局面に遷移可能か
		vb bucket(m + 1);

		// 局面 i から遷移可能な局面のニム値を記録する．
		rep(j, m) {
			if (i - c[j] >= 0) bucket[nimber[i - c[j]]] = true;
		}

		// 記録された局面のニム値の mex を求める．
		nimber[i] = 0;
		while (bucket[nimber[i]]) nimber[i]++;
	}
}

	
// 【正規型不偏ゲーム】O((|V| + |E|) log|V|)　
/*
* ゲームのルール：
* DAG g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かす．
* 先に移動不可能になった方が負けとする．
*
* nimber[v] : v にコマがある状態のニム値
*
* 利用：【最小除外数（mex）】
*/
void impartial_game(Graph& g, vi& nimber) {
	int n = sz(g);

	vb seen(n);
	nimber = vi(n);

	function<int(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return nimber[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		Nimber nx;
		repe(t, g[s]) nx.insert(dfs(t));
		nimber[s] = nx.mex();

		return nimber[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


// 【非有限正規型不偏ゲーム】O(|V| + |E|)
/*
* ゲームのルール：
* 有向グラフ（閉路可） g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かす．
* 先に移動不可能になった方が負けとする．
*
* res[v] : v にコマがある状態からの結果（1:先手勝ち，0:後手勝ち，-1:引き分け）
*
*（後退解析）
*/
void cyclic_impartial_game(Graph& g, vi& res) {
	// verify : https://atcoder.jp/contests/abc209/tasks/abc209_e

	int n = sz(g);

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) {
		repe(t, g[s]) {
			g_rev[t].push_back(s);
		}
	}

	// res[i] : 先手番で局面 i のときの勝敗（1:勝ち，0:負け，-1:引き分け）
	const int WIN = 1, LOSE = 0, DRAW = -1;
	res = vi(n, DRAW);

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
}


