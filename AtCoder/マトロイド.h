#pragma once
#include "header.h"
// ■■■■■ マトロイド ■■■■■


//【マトロイド】
/*
* 有限集合 M とその部分集合族 I が以下の条件を満たすとき，(M, I) をマトロイドという：
*		(1) 空集合 {} ∈ I
*		(2) ∀Y∈I, ∀X⊂Y, X∈I
*		(3) ∀X, Y∈I, |X|>|Y| ⇒ ∃x∈X＼Y, Y∪{x}∈I
* また I に属する集合を「独立集合」，属さない集合を「従属集合」という．
* 極大な独立集合のことを「基」という．
*/


//【ベクトルマトロイド】
/*
* M をベクトルの集合，I を一次独立なベクトルの集合からなる族とすれば，
* (M, I) はマトロイドとなる．これをベクトルマトロイドという．
* (M, I) の基は，span(M) の基底を意味する．
*/


//【グラフマトロイド】
/*
* 無向グラフ G = (V, E) に対し，M = E とし，I を森からなる集合族とすると，
* (M, I) はマトロイドとなる．これをグラフマトロイドという．
* (M, I) の基は，全域森を意味する．
*/


//【マトロイド上の最適化問題】
/*
* マトロイド (M, I) の各 x∈M にコスト c[x] が与えられているとする．
* S⊂M のコストを Σx∈S c[x] と定めるとき，コスト最小の基を求めたい．
*
* これは，次のような貪欲アルゴリズムで求められる：
*	M の元をコスト昇順にソートしておく．また X←{} とする．
*	各 x∈M について，X∪{x}∈I なら X←X∪{x} とすることを繰り返す．
*
*（グラフマトロイド上でこれを行うのが最小全域森を求めるクラスカル法である．）
*/


//【最小コスト基底（ビットベクトル，全空間）】O(m n min(m, n))
/*
* n 次元ビットベクトル v[0..m) がコスト c[0..m) をもつとし，
* F2^n の最小コスト基底のコストを返す（基底が構築できない場合は INFL を返す．）
*
*（呼び出すとき minimum_cost_base<N> としないと gcc でエラーになるので注意．）
*/
template <int N>
ll minimum_cost_base(vector<bitset<N>> v, int n, const vl& c) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_f

	int m = sz(v);

	// c[i] 昇順に添え字を並べる．
	vector<pli> ci(m);
	rep(i, m) ci[i] = { c[i], i };
	sort(all(ci));

	// id[i] : c の昇順で i 番目の要素が c[id[i]] であることを表す．
	vi id(m);
	rep(i, m) id[i] = ci[i].second;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	rep(j, n) rmd.push_back(j);

	ll res = 0;

	rep(i, m) {
		// i 番目のベクトルを走査し 1 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) {
			if (v[id[i]][*it] == 1) break;
		}

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;

		int j = *it;
		rmd.erase(it);

		// コストを加算
		res += c[id[i]];

		// 第 j 成分で見つかったのなら 第 j 成分が 1 である他のベクトルと XOR をとる．
		pivots.push_back({ i, j });
		repi(i2, i + 1, m - 1) {
			if (v[id[i2]][j]) v[id[i2]] ^= v[id[i]];
		}
	}

	return rmd.empty() ? res : INFL;
}


//【最小コスト基底（ビットベクトル，Span）】O(m n min(m, n))
/*
* ビットベクトル v[0..m) がコスト c[0..m) をもつとし，Span(v) の最小コスト基底のコストを返す．
*
*（呼び出すとき minimum_cost_base<N> としないと gcc でエラーになるので注意．）
*/
template <int N>
ll minimum_cost_base(vector<bitset<N>> v, const vl& c) {
	int m = sz(v);

	// c[i] 昇順に添え字を並べる．
	vector<pli> ci(m);
	rep(i, m) ci[i] = { c[i], i };
	sort(all(ci));

	// id[i] : c の昇順で i 番目の要素が c[id[i]] であることを表す．
	vi id(m);
	rep(i, m) id[i] = ci[i].second;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	rep(j, N) rmd.push_back(j);

	ll res = 0;

	rep(i, m) {
		// i 番目のベクトルを走査し 1 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) {
			if (v[id[i]][*it] == 1) break;
		}

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;

		int j = *it;
		rmd.erase(it);

		// コストを加算
		res += c[id[i]];

		// 第 j 成分で見つかったのなら 第 j 成分が 1 である他のベクトルと XOR をとる．
		pivots.push_back({ i, j });
		repi(i2, i + 1, m - 1) {
			if (v[id[i2]][j]) v[id[i2]] ^= v[id[i]];
		}
	}

	return res;
}


