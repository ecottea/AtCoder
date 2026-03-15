//【ライブラリの読み込み】
#include <bits/stdc++.h>
using namespace std;

#include <atcoder/all>
using namespace atcoder;


//【出力演算子 << のオーバーロード（デバッグ用）】
template <class T>
inline ostream& operator<<(ostream& os, fenwick_tree<T> ft) {
	// verify : https://judge.yosupo.jp/problem/static_range_sum

	//【注意】
	// fenwicktree.hpp 内で _n を勝手に public に移動している．

	int n = ft._n; 
	for (int i = 0; i < n; i++) {
		os << ft.sum(i, i + 1) << " ";
	}
	return os;
}

template <class S, S(*op)(S, S), S(*e)()>
inline ostream& operator<<(ostream& os, segtree<S, op, e> seg) {
	// verify : https://judge.yosupo.jp/problem/point_set_range_composite

	//【注意】
	// segtree.h 内に埋め込もうとしたら pair の出力のオーバーロードと衝突してエラーを吐いた．

	int n = seg.max_right(0, [](S) { return true; });
	for (int i = 0; i < n; i++) {
		os << seg.get(i) << " ";
	}
	return os;
}

template <class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
inline ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, act, comp, id> seg) {
	int n = seg.max_right(0, [](S) { return true; });
	for (int i = 0; i < n; i++) {
		os << seg.get(i) << " ";
	}
	return os;
}

inline ostream& operator<<(ostream& os, dsu d) {
	// verify : https://judge.yosupo.jp/problem/unionfind

	auto gs = d.groups();
	int K = (int)gs.size();
	for (int k = 0; k < K; k++) {
		for (const auto& v : gs[k]) os << v << " ";
		if (k < K - 1) os << "/ ";
	}
	return os;
}

template <class Cap>
inline ostream& operator<<(ostream& os, mf_graph<Cap> g) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_bp

	for (const auto& e : g.edges()) {
		os << e.from << "->" << e.to << " cap:" << e.cap << " flow:" << e.flow << std::endl;
	}
	return os;
}

template <class Cap, class Cost>
inline ostream& operator<<(ostream& os, mcf_graph<Cap, Cost> g) {
	// verify : https://yukicoder.me/problems/no/1301

	for (const auto& e : g.edges()) {
		os << e.from << "->" << e.to << " ";
		os << "cap:" << e.cap << " ";
		os << "flow:" << e.flow << " ";
		os << "cost:" << e.cost << std::endl;
	}
	return os;
}

