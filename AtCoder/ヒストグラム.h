#pragma once
#include "header.h"
// ■■■■■ ヒストグラム ■■■■■


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


//【ヒストグラム内最大長方形】O(n)
/*
* 長さ n のヒストグラム hist に含まれる長方形の面積の最大値を返す．
*/
template <class T>
ll largest_rectangle_in_histogram(vector<T>& hist) {
	// 参考：http://algorithms.blog55.fc2.com/blog-entry-132.html

	int n = sz(hist);

	ll res = 0;

	// 面積未確定の長方形を記憶しておくためのスタック
	// 要素 (l, h) は，左端位置が l，高さが h であることを表す．
	stack<pair<int, T>> st;

	// hist の末尾に番兵を仕込んでおき，左から順に見ていく．
	hist.push_back(0);
	rep(i, n + 1) {
		// 位置 i で高さ hist[i] を実現する長方形の左端位置
		int left = i;

		// スタックに記録されている長方形を順に見ていく．
		while (!st.empty()) {
			int l;
			T h;
			tie(l, h) = st.top();

			// 注目位置以下の高さをもつ長方形についてはまだ面積を増やせる．
			if (h <= hist[i]) {
				break;
			}

			// 注目位置より高い左端をもつ長方形はこれ以上伸ばせないので面積確定．
			chmax(res, (ll)h * (i - l));
			left = l;
			st.pop();
		}

		// 面積未確定の長方形として高さ hist[i] をもつものを追加する．
		st.push({ left, hist[i] });
	}
	hist.pop_back();

	return res;
}


