#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 根付き木 ■■■■■



//【根付き木のオイラーツアー】O(|V|)
/*
* 根付き木 rt のオイラーツアーを求める．
*
* in[s] : 最初に頂点 s を訪れた時刻（根なら 0）
* out[s] : 最後に頂点 s から離れた時刻（根なら 2 |V| - 1）
* pos[t] : 時刻 t に訪れた頂点の番号（長さ 2 |V| - 1）
*/
template <class TREE>
void euler_tour(TREE& rt, vi& in, vi& out, vi& pos) {
	int n = sz(rt.v);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);

	// 再帰用の関数
	function<void(int)> rf = [&](int s) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		for (auto t : rt.v[s].child) {
			rf(t);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r);
}


//【根付き木の HL 分解】O(|V|)
/*
* 根付き木 rt の HL 分解を行う．
*
* in[s] : 最重頂点優先の行きがけ順で頂点 s を何番目になぞるか
* pos[i] : 最重頂点優先の行きがけ順で i 番目になぞる頂点
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void heavy_light_decomposition(TREE& rt, vi& in, vi& pos, vi& top) {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n = (int)rt.v.size();

	int step = 0;
	in = vi(n);
	pos = vi(n);
	top = vi(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = step;
		pos[step++] = s;
		top[s] = p;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		for (auto t : rt.v[s].child) {
			if (chmax(w_max, rt.v[t].weight)) {
				v_max = t;
			}
		}

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) {
			rf(v_max, p);
		}

		// 残りの頂点をなぞる．
		for (auto t : rt.v[s].child) {
			if (t == v_max) {
				continue;
			}

			rf(t, t);
		}

		return;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


//【根付き木の HL 分解／オイラーツアー】O(|V|)
/*
* 根付き木 rt の HL 分解を行いつつオイラーツアーを得る．
*
* in[s] : 最重頂点優先で頂点 s に初めて入る時刻（根なら 0）
* out[s] : 最重頂点優先で頂点 s から最後にでる時刻（根なら 2 |V| - 1）
* pos[t] : 最重頂点優先で時刻 t で居る頂点（長さ 2 |V| - 1）
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void hld_and_et(TREE& rt, vi& in, vi& out, vi& pos, vi& top) {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n = (int)rt.v.size();

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);
	top = vi(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		pos[time++] = s;
		top[s] = p;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		for (auto t : rt.v[s].child) {
			if (chmax(w_max, rt.v[t].weight)) {
				v_max = t;
			}
		}

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) {
			rf(v_max, p);
			pos[time++] = s;
		}

		// 残りの頂点をなぞる．
		for (auto t : rt.v[s].child) {
			if (t == v_max) {
				continue;
			}

			rf(t, t);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


