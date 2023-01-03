#pragma once
#include "header.h"
#include "分析(木).h"
// ■■■■■ 木の数え上げ ■■■■■


//【頂点間距離が k である頂点の組の数え上げ】O(n log n)
/*
* 木 g の異なる 2 頂点の組で，距離が k であるものの個数を返す．
*
* 利用：【木の重心分解】
*/
ll count_tree_distance(const Graph& g, int k) {
	// verify : https://csacademy.com/contest/round-58/task/path-inversions/

	if (k <= 0) return 0;

	int n = sz(g);

	// 木 g を重心分解する．
	Centroid_decomposition<Graph> cd(g);
	
	ll res = 0;

	// cnt_all[l] : 長さ l のパスの本数（使い回す）
	vl cnt_all(n);

	// cent : 部分木の重心
	rep(cent, n) {
		// nc : cent に隣接する頂点の個数，c_dep : cent の深さ
		int nc = sz(g[cent]), c_dep = cd[cent].dep;

		// 部分木ごとの長さのリスト
		vvi lens(nc);

		// cent で分割された各部分木の cent に隣接する頂点を根として dfs する．
		rep(i, nc) {
			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int l) {
				// 長さ l のパスの存在を記録する．
				lens[i].emplace_back(l);
				cnt_all[l]++;

				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, l + 1);
				}
			};
			dfs(g[cent][i], -1, 1);
		}
		
		// 2 回目の dfs
		rep(i, nc) {
			// i 番目の部分木の分をロールバック
			repe(l, lens[i]) cnt_all[l]--;

			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int l) {
				// cent を端点にもつ長さ k のパスを数える（後で 2 で割るので 2 本として数える）
				if (l == k) res += 2;

				// 他の部分木内の長さ 1 以上のパスと繋げて長さ k になるパスを数える．
				if (k - l >= 1) res += cnt_all[k - l];

				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, l + 1);
				}
			};
			dfs(g[cent][i], -1, 1);

			// i 番目の部分木の分のロールバックを元に戻す
			repe(l, lens[i]) cnt_all[l]++;
		}

		// cnt_all の初期化（assign() より速い）
		rep(i, nc) repe(l, lens[i]) cnt_all[l]--;
	}

	// 同じパスを 2 通りの向きで数えてしまっているので 2 で割る．
	return res / 2;
}


//【頂点間距離の分布】O(n (log n)^2)
/*
* 木 g の異なる 2 点間の距離の分布を返す．
*
* 利用：【木の重心分解】
*/
vl tree_distance_frequency(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/frequency_table_of_tree_distance

	int n = sz(g);
	Centroid_decomposition<Graph> cd(g);

	vl res(n);

	// cent : 部分木の重心
	rep(cent, n) {
		// nc : cent に隣接する頂点の個数，c_dep : cent の深さ
		int nc = sz(g[cent]), c_dep = cd[cent].dep;

		// cnt[i][j] : cent を始点にもち，cent の i 番目の隣接頂点方向へ向かう長さ j のパスの本数
		vvl cnt(nc, vl(1, 1));

		// cnt_all[j] : cent を始点にもつ長さ j のパスの本数
		vl cnt_all(1, 1);

		// cent で分割された各部分木の cent に隣接する頂点を根として dfs する．
		rep(i, nc) {
			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int len) {
				// 長さ len のパスの存在を記録する
				if (sz(cnt[i]) == len) cnt[i].resize(len + 1);
				cnt[i][len]++;

				if (sz(cnt_all) == len) cnt_all.resize(len + 1);
				cnt_all[len]++;

				// 再帰処理
				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, len + 1);
				}
			};
			dfs(g[cent][i], -1, 1);
		}

		// cent を含むパスを数える．
		auto conv = convolution_ll(cnt_all, cnt_all);
		rep(j, 2 * sz(cnt_all) - 1) res[j] += conv[j];

		rep(i, nc) {
			// cent で折り返すパスを数えすぎているので引く．
			cnt[i][0] = 0;
			auto conv = convolution_ll(cnt[i], cnt[i]);
			rep(j, 2 * sz(cnt[i]) - 1) res[j] -= conv[j];
		}
	}

	rep(j1, n) res[j1] /= 2;

	return res;
}


//【森の連結成分の数】
/*
* 森 g の連結成分の個数は，(g の頂点数) - (g の辺数) に等しい．
* 
* verify : https://atcoder.jp/contests/agc015/tasks/agc015_c
*/


//【誘導部分グラフの連結成分の数】
/*
* 根付き木 g の誘導部分グラフ h の連結成分の個数は，
* 親が h に属さない（または g の根である）ような h の頂点の個数に等しい．
* 
* verify : https://atcoder.jp/contests/abc173/tasks/abc173_f
*/


//【独立集合の数え上げ】
/*
* 木DP.h へ
*/


//【部分木の数え上げ】
/*
* 二乗の木DP.h へ
*/


//【木の誘導部分グラフの数え上げ】
/*
* 二乗の木DP.h へ
*/


//【互いに素なパスの数え上げ】
/*
* 二乗の木DP.h へ
*/


//【先祖-子孫関係にない頂点集合の数え上げ】
/*
* 二乗の木DP.h へ
*/


