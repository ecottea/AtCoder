#pragma once
#include "header.h"
// ■■■■■ 列に対する汎用性のある前処理 ■■■■■


//【自身より小さい数の次の位置】O(n log n)
/*
* a[0..n) で，i < j かつ a[i] > a[j] なる最小の j（なければ n）を pos[i] に格納し pos を返す．
* eq = true とすると「自身以下の数の次の位置」を格納する．
*/
template <class T>
vi next_less_position(const vector<T>& a, bool eq = false) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_3_C

	int n = sz(a);
	vi nxt_les(n, n);

	priority_queue<pair<T, int>> q;

	rep(i, n) {
		while (!q.empty() && q.top().first >= a[i]) {
			if (!eq && q.top().first == a[i]) break;

			nxt_les[q.top().second] = i;
			q.pop();
		}

		q.push({ a[i], i });
	}

	return nxt_les;
}


//【自身より大きい数の次の位置】O(n log n)
/*
* a[0..n) で，i < j かつ a[i] < a[j] なる最小の j（なければ n）を pos[i] に格納し pos を返す．
* eq = true とすると「自身以上の数の次の位置」を格納する．
*/
template <class T>
vi next_greater_position(const vector<T>& a, bool eq = false) {
	// verify : https://atcoder.jp/contests/code-festival-2014-qualb/tasks/code_festival_qualB_d

	int n = sz(a);
	vi nxt_grt(n, n);

	priority_queue_rev<pair<T, int>> q;

	rep(i, n) {
		while (!q.empty() && q.top().first <= a[i]) {
			if (!eq && q.top().first == a[i]) break;

			nxt_grt[q.top().second] = i;
			q.pop();
		}

		q.push({ a[i], i });
	}

	return nxt_grt;
}


//【自身より小さい数の前の位置】O(n log n)
/*
* a[0..n) で，j < i かつ a[j] < a[i] なる最大の j（なければ -1）を pps[i] に格納し pos を返す．
* eq = true とすると「自身以下の数の前の位置」を格納する．
*/
template <class T>
vi prev_less_position(const vector<T>& a, bool eq = false) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_3_C

	int n = sz(a);
	vi prv_les(n, -1);

	priority_queue<pair<T, int>> q;

	repir(i, n - 1, 0) {
		while (!q.empty() && q.top().first >= a[i]) {
			if (!eq && q.top().first == a[i]) break;

			prv_les[q.top().second] = i;
			q.pop();
		}

		q.push({ a[i], i });
	}

	return prv_les;
}


//【自身より大きい数の前の位置】O(n log n)
/*
* a[0..n) で，j < i かつ a[j] > a[i] なる最大の j（なければ -1）を pos[i] に格納し pos を返す．
* eq = true とすると「自身以上の数の前の位置」を格納する．
*/
template <class T>
vi prev_greater_position(const vector<T>& a, bool eq = false) {
	// verify : https://atcoder.jp/contests/code-festival-2014-qualb/tasks/code_festival_qualB_d

	int n = sz(a);
	vi prv_grt(n, -1);

	priority_queue_rev<pair<T, int>> q;

	repir(i, n - 1, 0) {
		while (!q.empty() && q.top().first <= a[i]) {
			if (!eq && q.top().first == a[i]) break;

			prv_grt[q.top().second] = i;
			q.pop();
		}

		q.push({ a[i], i });
	}

	return prv_grt;
}


//【自身より大きい数の次以降 k 個の位置】O(k n log n)
/*
* a[0..n) で，i < j かつ a[i] < a[j] なる j を昇順に k 個まで pos[i] に格納し pos を返す．
* eq = true とすると「自身以上の数の次から k 個の位置」を格納する．
*/
template <class T>
vvi next_greater_positions(const vector<T>& a, int k, bool eq = false) {
	// verify : https://atcoder.jp/contests/abc140/tasks/abc140_e

	int n = sz(a);
	vvi nxt_grt(n);

	// 奥の要素にアクセスできる優先度付きキューとして set を用いる．
	set<pair<T, int>> q;

	rep(i, n) {
		auto it = q.begin();
		while (it != q.end()) {
			if (eq && it->first > a[i]) break;
			if (!eq && it->first >= a[i]) break;

			nxt_grt[it->second].push_back(i);

			if (sz(nxt_grt[it->second]) == k) it = q.erase(it);
			else it++;
		}

		q.insert({ a[i], i });
	}

	return nxt_grt;
}


//【自身と同じ数の次の位置】O(n)
/*
* a[0..n) で，i < j かつ a[i] = a[j] なる最小の j（なければ n）を pos[i] に格納し pos を返す． 
*/
template <class T>
vi next_equal(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc036/tasks/agc036_b

	int n = sz(a);

	// nxt[i] : j > i かつ a[j] = a[i] となる最小の j（なければ n）
	vi nxt(n, n);

	// num_to_pos[x] : 値 x が最後に現れた位置（右から走査する）
	unordered_map<T, int> num_to_pos;

	repir(i, n - 1, 0) {
		if (num_to_pos.count(a[i])) {
			nxt[i] = num_to_pos[a[i]];
		}
		num_to_pos[a[i]] = i;
	}

	return nxt;
}


//【自身と同じ数の前の位置】O(n)
/*
* a[0..n) で，j < i かつ a[j] = a[i] なる最大の j（なければ -1）を pps[i] に格納し pos を返す．
*/
template <class T> 
vi prev_equal(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc031/tasks/agc031_b

	int n = sz(a);

	// prv[i] : j < i かつ a[j] = a[i] となる最大の j（なければ -1）
	vi prv(n, -1);

	// num_to_pos[x] : 値 x が最後に現れた位置（左から走査する）
	unordered_map<T, int> num_to_pos;

	rep(i, n) {
		if (num_to_pos.count(a[i])) {
			prv[i] = num_to_pos[a[i]];
		}
		num_to_pos[a[i]] = i;
	}

	return prv;
}


//【自身より 1 つ大きい数の次の位置】O(n)
/*
* a[0..n) で，i < j かつ a[i]+1 = a[j] なる最小の j（なければ n）を pos[i] に格納し pos を返す．
*/
template <class T>
vi next_greater1(const vector<T>& a, vi& nxt) {
	int n = sz(a);

	// nxt[i] : j > i かつ a[j] = a[i] となる最小の j（なければ n）
	vi nxt(n, n);

	// num_to_pos[x] : 値 x が最後に現れた位置（右から走査する）
	unordered_map<T, int> num_to_pos;

	repir(i, n - 1, 0) {
		if (num_to_pos.count(a[i] + 1)) {
			nxt[i] = num_to_pos[a[i] + 1];
		}
		num_to_pos[a[i]] = i;
	}

	return nxt;
}


//【指定値以上の数の次の位置】O(n m)
/*
* [0..m) に値をもつ数列 a[0..n) について，a[i..n) で j 以上の値が現れる
* 最左位置（なければ n）を pos[i][j] に格納し pos を返す．
*/
vvi next_greater_position(int m, const vi& a) {
	// verify : https://atcoder.jp/contests/agc058/tasks/agc058_b

	int n = sz(a);

	// nxt_grt[i][j] : a[i..n) で j 以上の値が現れる最左位置（無いなら n）
	vvi nxt_grt(n + 1, vi(m, n));

	repir(i, n - 1, 0) {
		repi(j, 0, a[i]) nxt_grt[i][j] = i;
		repi(j, a[i] + 1, m - 1) nxt_grt[i][j] = nxt_grt[i + 1][j];
	}

	return nxt_grt;
}


//【デカルト木】
/*
* Cartesian_tree(vT a, smaller = true) : O(n)
*	a[0..n) の最小要素の位置を根とするデカルト木を構築する．
*	根から順に小さい要素での区間の分割を表す（同じ要素は左のものほど小さいとする．）
*	smaller = false とすると，大小関係を逆転して木の構築を行う．
*/
template <class T>
struct Cartesian_tree {
	struct Node {
		T val; // 区間の最小値
		int l, r; // 区間 [l..r) に対応するノードであることを表す．
		int p = -1; // 親（なければ -1）
		int lc = -1; // 左の子（なければ -1）
		int rc = -1; // 右の子（なければ -1）

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "[" << v.l << "," << v.r << "):" << v.val
				<< ", lc:" << v.lc << ", rc:" << v.rc << ", p:" << v.p;
			return os;
		}
#endif
	};

	int n; // 頂点の数
	int rt; // 根
	vector<Node> v; // 頂点

	// 数列 a[0..n) で初期化する．
	Cartesian_tree(const vector<T>& a, bool smaller = true) : n(sz(a)), rt(0), v(n) {
		// verify : https://judge.yosupo.jp/problem/cartesian_tree

		// 木の構造を決定する．
		repi(i, 1, n - 1) {
			// pt : i-1 の祖先で値が a[i] 以下であるもののうち最も深いもの（なければ -1）
			int pt = i - 1;
			while (pt != -1 && (smaller ? a[pt] > a[i] : a[pt] < a[i])) pt = v[pt].p;

			// pt の右の子を i，i の左の子を pt の元の右の子とする．
			if (pt != -1) {
				v[i].p = pt;
				if (v[pt].rc != -1) v[v[pt].rc].p = i;
				v[i].lc = v[pt].rc;
				v[pt].rc = i;
			}
			// pt がなければ i を根とする．
			else {
				v[i].lc = rt;
				v[rt].p = i;
				rt = i;
			}
		}

		// ノードの情報を決定する．
		function<void(int, int, int)> dfs = [&](int s, int l, int r) {
			v[s].val = a[s];
			v[s].l = l;
			v[s].r = r;

			if (v[s].lc != -1) dfs(v[s].lc, l, s);
			if (v[s].rc != -1) dfs(v[s].rc, s + 1, r);
		};
		dfs(rt, 0, n);
	}
	Cartesian_tree() : n(0), rt(-1) {} // ダミー

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Cartesian_tree& ct) {
		rep(i, sz(ct)) os << i << ": " << ct[i] << endl;
		return os;
	}
#endif
};


//【中央値で部分列に分解】O(n log n)
/*
* 数列 a[0..n) の中央値を med とし，med 未満[以上] の要素からなる a の部分列を la[ ua ] に格納する．
* ただし med = min(a) のときは代わりに med ← min(a)+1 を用いる（min(a) = max(a) なら false を返す）
* また l_pos[ u_pos ] には la[ ua ] の要素の元の位置を格納する．
*/
template <class T>
bool median_separate(const vector<T>& a, vector<T>& la, vector<T>& ua, vi& l_pos, vi& u_pos) {
	// verify : https://atcoder.jp/contests/chokudai_s001/tasks/chokudai_S001_j

	int n = sz(a);
	la.clear(); ua.clear(); l_pos.clear(); u_pos.clear();

	auto sa(a);
	sort(all(sa));

	T a_min = sa[0];
	T a_max = sa[n - 1];
	if (a_min == a_max) return false;

	T med = sa[(n + 1) / 2];
	if (med == a_min) med++;

	rep(i, n) {
		if (a[i] < med) {
			la.emplace_back(a[i]);
			l_pos.emplace_back(i);
		}
		else {
			ua.emplace_back(a[i]);
			u_pos.emplace_back(i);
		}
	}

	return true;
}


