#pragma once
#include "header.h"
// ■■■■■ 列 ■■■■■



//【ヒストグラムの横切り】
/*
* ヒストグラム hist を横切りしたときの (高さ, 長さ) の組を hls に格納する．
* ただし高さが狭義単調増加かつ長さ狭義単調減少となる部分だけを格納する．
*
* hls[i] = {h, l} : 低い方から i 番目の高さが h，長さが l であることを表す．
*
* 戻り値 : hls の要素数
*/
int cut_histogram_horizontal(const vl& hist, vector<pli>& hls) {
	int n = sz(hist);

	// ヒストグラムをソートしておく．
	vl hist_sorted = hist;
	sort(all(hist_sorted));

	hls.clear();

	// 直前に見ていた高さ
	ll bh = -1;

	rep(i, n) {
		// 直前に見ていた高さより高い場合
		if (bh < hist_sorted[i]) {
			// 高さと長さの組を格納する．
			hls.push_back({ hist_sorted[i], n - i });

			bh = hist_sorted[i];
		}
	}

	return sz(hls);
}


