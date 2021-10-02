#pragma once
#include "header.h"
// ■■■■■ 区間 ■■■■■



//【区間の結合】O(n log n)
/*
* n 個の半開区間 lr[i] = [l, r) を結合した半開区間の列を res に格納する．
* また結合した後の半開区間の個数を返す．
*/
template <class T>
int interval_union(vector<pair<T, T>>& lr, vector<pair<T, T>>& res) {
	int n = sz(lr);

	sort(all(lr));

	int m = 1;
	res.clear();
	res.push_back(lr[0]);
	repi(i, 1, n - 1) {
		// i 番目の区間の左端が結合中の区間の右端より右だった場合
		if (lr[i].first > res[m - 1].second) {
			// 区間の結合は完了したので，i 番目の区間を結合中の区間として次に進む．
			res.push_back(lr[i]);
			m++;
		}
		// i 番目の区間の左端が結合中の区間の右端より左だった場合（ちょうどを含む）
		else {
			// i 番目の区間を結合中の区間に結合し，右端を更新する．
			chmax(res[m - 1].second, lr[i].second);
		}
	}

	return m;
}


//【スライド最小値】O(n)
/*
* 長さ n の配列 a に対し a[i] からの w 個の最小値を a_min[i] に返す．
*/
template <class T>
void slide_minimum(const vector<T>& a, int w, vector<T>& a_min) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332

	int n = sz(a);
	a_min = vector<T>(n + 1 - w);

	// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
	deque<int> q;

	rep(i, n) {
		// 現在の最小値が注目区間の外に出たらデックの先頭から除去する．
		if (!q.empty() && q.front() <= i - w) {
			q.pop_front();
		}

		// 新しく区間に入る数より大きい数は最小値とはなりえないので
		// デックの末尾から除去する．
		while (!q.empty() && a[q.back()] > a[i]) {
			q.pop_back();
		}

		// 新しく区間に入る数は，今後最小値となる可能性があるので
		// デックの末尾に追加する．
		q.push_back(i);

		// 注目区間の幅が w になっていれば結果を記録する．
		if (i >= w - 1) {
			a_min[i - w + 1] = a[q.front()];
		}
	}
}


