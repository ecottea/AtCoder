//【ライブラリの読み込み】
#include <bits/stdc++.h>
using namespace std;

#include <atcoder/all>
using namespace atcoder;


//【出力演算子 << のオーバーロード（デバッグ用）】
template <class S, S(*op)(S, S), S(*e)()>
inline ostream& operator<< (ostream& os, segtree<S, op, e> seg) {
	int n = seg.max_right(0, [](S) { return true; });
	for (int i = 0; i < n; i++) {
		os << seg.get(i) << " ";
	}
	return os;
}

template <class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
inline ostream& operator<< (ostream& os, lazy_segtree<S, op, e, F, act, comp, id> seg) {
	int n = seg.max_right(0, [](S) { return true; });
	for (int i = 0; i < n; i++) {
		os << seg.get(i) << " ";
	}
	return os;
}

inline ostream& operator<< (ostream& os, dsu d) {
	for (const auto& g : d.groups()) {
		for (const auto& v : g) {
			os << v << " ";
		}
		os << "/ ";
	}
	return os;
}
