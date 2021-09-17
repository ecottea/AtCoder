#pragma once
#include "header.h"
// ■■■■■ ソート ■■■■■



//【クイックソート】O(n log n)
/*
* 長さ n の配列 a に対してクイックソートを行う．
*/
template <class T>
void quick_sort(vector<T>& a) {
	int n = sz(a);

	// a[p, r] を a[r] をピボットとして分割する．
	function<int(int, int)> partition = [&](int p, int r) {
		// 分割の基準となる数（ピボット）
		T x = a[r];

		// i : a[p, i] は x 以下の要素と確定
		int i = p - 1;

		// j : a(i, j] は x より大きい要素と確定
		repi(j, p, r - 1) {
			// j の位置に x 以下の要素がある場合
			if (a[j] <= x) {
				// 最も左の x より大きい要素と交換する．
				i++;
				swap(a[i], a[j]);
			}
		}

		// ピボットを仕切りとしての位置に移動する．
		swap(a[i + 1], a[r]);

		return i + 1;
	};

	// 再帰用の関数
	function<void(int, int)> quick_sort_rf = [&](int p, int r) {
		if (p >= r) {
			return;
		}

		// 右端の要素をピボットとして分割し，そのそれぞれをソートする．
		int q = partition(p, r);
		quick_sort_rf(p, q - 1);
		quick_sort_rf(q + 1, r);
	};

	quick_sort_rf(0, n - 1);
}


//【計数ソート】O(n + k)
/*
* 各要素が k 未満の非負整数である長さ n の配列 a に対して計数ソートを行う．
*/
void counting_sort(vector<int>& a, int k) {
	int n = sz(a);

	// 結果の格納用
	vector<int> res(n);

	// 各要素の出現数の記録用
	vector<int> cnt(k);

	// cnt[j] : 要素 j の出現数
	rep(i, n) {
		cnt[a[i]]++;
	}

	// cnt[j] : j 以下の要素の出現数
	rep(j, k - 1) {
		cnt[j + 1] += cnt[j];
	}

	// 安定ソートとなるように降順に正しい位置にはめていく．
	repir(i, n - 1, 0) {
		res[cnt[a[i]] - 1] = a[i];
		cnt[a[i]]--;
	}

	swap(a, res);
}


//【マージソート】O(log n)
/*
* 長さ n の配列 a に対してマージソートを行う．
*/
template <class T>
void merge_sort(vector<T>& a) {
	// 型 T における最大値
	const T T_INF = numeric_limits<T>::max();

	int n = sz(a);

	// 二つの配列を統合する．
	function<void(int, int, int)> merge = [&](int l, int m, int r) {
		vector<T> left{ a.begin() + l, a.begin() + m };
		vector<T> right{ a.begin() + m, a.begin() + r };
		left.push_back(T_INF);
		right.push_back(T_INF);

		int i = 0, j = 0;
		repi(k, l, r - 1) {
			if (left[i] <= right[j]) {
				a[k] = left[i];
				i++;
			}
			else {
				a[k] = right[j];
				j++;
			}
		}
	};

	// 再帰用の関数
	function<void(int, int)> merge_sort_rf = [&](int l, int r) {
		if (r - l > 1) {
			int m = (l + r) / 2;
			merge_sort_rf(l, m);
			merge_sort_rf(m, r);
			merge(l, m, r);
		}
	};

	merge_sort_rf(0, n);
}


//【シェルソート】O(n^1.25)
/*
* 長さ n の配列 a に対してシェルソートを行う．
*/
template <class T>
void shell_sort(vector<T>& a) {
	int n = sz(a);

	// 適切な増分の列を得る．
	vector<int> gap;
	int g = 1;
	do {
		gap.push_back(g);
		g = 3 * g + 1;
	} while (g < n);
	reverse(all(gap));
	int m = sz(gap);

	// 増分を g とした挿入ソート
	function<void(int)> isort = [&](int g) {
		repi(i, g, n - 1) {
			T v = a[i];

			int j = i - g;
			while (j >= 0 && a[j] > v) {
				a[j + g] = a[j];
				j -= g;
			}

			a[j + g] = v;
		}
	};

	// 増分を減らしながら挿入ソートを繰り返す．
	rep(i, m) {
		isort(gap[i]);
	}
}


//【選択ソート】O(n^2)
/*
* 長さ n の配列 a に対して選択ソートを行う．
*/
template <class T>
void selection_sort(vector<T>& a) {
	int n = sz(a);

	// 左から順にそこに居るべき要素を決定していく．
	rep(i, n) {
		// 残る要素の中の最小のものの位置
		int min_j = i;

		// 残りの要素を順に調べる．
		repi(j, i + 1, n - 1) {
			// より小さい要素が見つかれば位置を記憶する．
			if (a[j] < a[min_j]) {
				min_j = j;
			}
		}

		// 注目要素と最小要素とを交換する．
		swap(a[i], a[min_j]);
	}
}


//【挿入ソート】O(n^2)
/*
* 長さ n の配列 a に対して挿入ソートを行う．
*/
template <class T>
void insertion_sort(vector<T>& a) {
	int n = sz(a);

	// 未ソートの要素を昇順に見ていく．
	repi(i, 1, n - 1) {
		// 注目要素
		T v = a[i];

		// 注目要素が存在すべき位置を調べつつ，
		// 要素を 1 つずつずらして挿入のための隙間をつくる．
		int j = i - 1;
		while (j >= 0 && a[j] > v) {
			a[j + 1] = a[j];
			j--;
		}

		// 要素を挿入する．
		a[j + 1] = v;
	}
}


//【バブルソート】O(n^2)
/*
* 長さ n の配列 a に対してバブルソートを行う．
* 要素の交換を行った回数（a の転倒数）を返す．
*/
template <class T>
int bubble_sort(vector<T>& a) {
	int n = sz(a);

	int cnt = 0;
	while (true) {
		// 要素の交換を行ったかどうか
		bool swapped = false;

		repir(j, n - 1, 1) {
			// 隣接要素の大小関係が逆転していたら交換する．
			if (a[j - 1] > a[j]) {
				swap(a[j - 1], a[j]);
				cnt++;
				swapped = true;
			}
		}

		// 要素の交換が行われなくなったら終了する．
		if (!swapped) {
			break;
		}
	}

	return cnt;
}

