#pragma once
#include "header.h"
// ■■■■■ Union-Find ■■■■■


//【Union-Find】
/*
* Union_find(int n) : O(n)
*	非連結で大きさ n の Union-Find を構築する．
*
* merge(int a, int b) : O(α(n))
*	頂点 a と頂点 b を統合する．
*
* bool same(int a, int b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* int leader(int a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* int size(int a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* vvi groups() : O(n α(n))
*	連結成分のリストを返す．
*/
struct Union_find {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// 非連結で大きさ n の Union-Find を構築する．
	Union_find(int n_) : n(n_), m(n), parent_or_size(n, -1) {}
	Union_find() : n(0), m(0) {} // ダミー

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://judge.yosupo.jp/problem/unionfind

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(ra, rb);
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// verify : https://judge.yosupo.jp/problem/unionfind

		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// verify : https://atcoder.jp/contests/abc049/tasks/arc065_b

		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		return ra;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// verify : https://codeforces.com/contest/764/problem/C

		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		// verify : https://atcoder.jp/contests/abc229/tasks/abc229_e

		return m;
	}

	// 連結成分のリストを返す．
	vvi groups() {
		vvi res(m);

		vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].push_back(a);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Union_find d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【ポテンシャル Union-Find】
/*
* Potential_union_find<T>(int n) : O(n)
*	非連結で大きさ n の重み付き Union-Find を構築する．
*
* bool set_diff(int a, int b, T d) : O(α(n))
*	v[b] - v[a] = d という関係を追加する．失敗は false を返す．
*
* bool same(int a, int b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* T get_diff(int a, int b) : O(α(n))
*	v[b] - v[a] を返す．（差が未確定なら戻り値は未定義とする）
*
* int leader(int a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* int size(int a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* vv<piT> groups() : O(n α(n))
*	連結成分の (頂点番号, ポテンシャル) の組のリストを返す．
*/
template <class T>
struct Potential_union_find {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または集合の大きさ
	// 頂点 i が根でない場合は親の番号（非負）を，
	// 根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// pot[i] : 根からみた頂点 i との差（ポテンシャル）
	vector<T> pot;

	// 非連結で大きさ n の重み付き Union-Find を構築する．
	Potential_union_find(int n_) : n(n_), m(n), parent_or_size(n, -1), pot(n) {}
	Potential_union_find() : n(0), m(0) {}

	// 頂点 a, b 間の差 v[b] - v[a] を設定する．
	bool set_diff(int a, int b, T d) {
		// verify : https://atcoder.jp/contests/code-festival-2016-quala/tasks/codefestival_2016_qualA_d

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		// 既にある関係と整合しているかを返す．
		if (ra == rb) return pot[b] - pot[a] == d;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(a, b);
			swap(ra, rb);
			d *= -1;
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
		pot[rb] = pot[a] - pot[b] + d;

		// 連結成分の数を 1 つ減らす．
		m--;

		return true;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// verify : https://yukicoder.me/problems/no/2251

		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// v[b] - v[a] を返す．
	T get_diff(int a, int b) {
		// verify : https://yukicoder.me/problems/no/2251

		// 差が確定していると仮定し，根からの差の差として計算する．
		return pot[b] - pot[a];
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		// 再帰的な処理が回り，pa の親は ra になっていることに注意．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		pot[a] += pot[pa];
		return ra;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		// verify : https://yukicoder.me/problems/no/2251

		return m;
	}

	// 連結成分の (頂点番号, ポテンシャル) の組のリストを返す．
	vector<vector<pair<int, T>>> groups() {
		// verify : https://atcoder.jp/contests/code-festival-2016-quala/tasks/codefestival_2016_qualA_d

		vector<vector<pair<int, T>>> res(m);

		vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].emplace_back(a, pot[a]);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Potential_union_find d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【部分永続 Union-Find】
/*
* 永続データ構造.h へ
*/


//【永続 Union-Find】
/*
* 永続データ構造.h へ
*/


//【Union-Find（偶奇判定付き）】
/*
* Parity_union_find(int n) : O(n)
*	非連結な頂点 [0..n) で初期化する．
*
* merge_even(int a, int b) : O(α(n))
*	頂点 a と頂点 b を長さ 2 の辺で結ぶ．（統合とみなせる）
*
* merge_odd(int a, int b) : O(α(n))
*	頂点 a と頂点 b を長さ 1 の辺で結ぶ．
*
* bool same_even(int a, int b) : O(α(n))
*	頂点 a, b 間の距離が偶数かを返す．（非連結なら false を返す）
*
* bool same_odd(int a, int b) : O(α(n))
*	頂点 a, b 間の距離が奇数かを返す．（非連結なら false を返す）
*/
struct Parity_union_find {
	int n;
	dsu d;

	// コンストラクタ（初期化なし，大きさ n で初期化）
	Parity_union_find() : n(0) {}
	Parity_union_find(int n_) : n(n_), d(2 * n) {}

	// 頂点 a と頂点 b を 1 つに統合する．
	void merge_even(int a, int b) {
		// verify : https://atcoder.jp/contests/arc036/tasks/arc036_d

		d.merge(a, b);
		d.merge(a + n, b + n);
	}

	// 頂点 a と頂点 b を辺で結ぶ．
	void merge_odd(int a, int b) {
		// verify : https://atcoder.jp/contests/arc036/tasks/arc036_d

		d.merge(a, b + n);
		d.merge(a + n, b);
	}

	// 頂点 a, b 間を偶数回の移動で行き来できるかを返す．
	bool same_even(int a, int b) {
		// verify : https://atcoder.jp/contests/arc036/tasks/arc036_d

		return d.same(a, b);
	}

	// 頂点 a, b 間を奇数回の移動で行き来できるかを返す．
	bool same_odd(int a, int b) {
		// verify : https://atcoder.jp/contests/abc126/tasks/abc126_d

		return d.same(a, b + n);
	}
};


//【総和 Union-Find】
/*
* Union_find_sum<S>(vS v) : O(n)
*	値 v[0..n) をもった n 頂点からなる非連結の総和 Union-Find を構築する．
*
* merge(int a, int b) : O(α(n))
*	頂点 a と頂点 b を統合する．
*
* bool same(int a, int b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* int leader(int a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* S sum(int a) : O(α(n))
*	頂点 a の属する連結成分の和を返す．
*
* int size(int a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* vvi groups() : O(n α(n))
*	連結成分のリストを返す．
*/
template <class S>
struct Union_find_sum {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// val[i] : 頂点 i の属する集合の和
	vector<S> val;

	// 値 a[0..n) をもった n 頂点からなる非連結の Union-Find を構築する．
	Union_find_sum(const vector<S>& v) : n(sz(v)), m(n), parent_or_size(n, -1), val(v) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_d
	}
	Union_find_sum() : n(0), m(0) {}

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_d

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) swap(ra, rb);

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
		val[ra] += val[rb];

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		return ra;
	}

	// 頂点 a の属する連結成分の和を返す．
	S sum(int a) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_d

		// a の根を調べ，そこに記録されている和の情報を返す．
		return val[leader(a)];
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		return m;
	}

	// 連結成分のリストを返す．
	vvi groups() {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_d

		vvi res(m);

		vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].push_back(a);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Union_find_sum d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << "sum: " << d.sum(g[0]) << endl;
		}
		return os;
	}
#endif
};


//【総和 Union-Find（可換モノイド）】
/*
* Union_find_sum<S, op, o>(vS v) : O(n)
*	値 v[0..n) をもった n 頂点からなる非連結の総和 Union-Find を構築する．
*	要素は可換モノイド <S, op, o> の元とする．
*
* merge(int a, int b) : O(α(n))
*	頂点 a と頂点 b を統合する．
*
* bool same(int a, int b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* int leader(int a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* S sum(int a) : O(α(n))
*	頂点 a の属する連結成分の和を返す．
*
* int size(int a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* vvi groups() : O(n α(n))
*	連結成分のリストを返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
struct Union_find_sum_amonoid {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// val[i] : 頂点 i の属する集合の和
	vector<S> val;

	// 値 a[0..n) をもった n 頂点からなる非連結の Union-Find を構築する．
	Union_find_sum_amonoid(const vector<S>& v) : n(sz(v)), m(n), parent_or_size(n, -1), val(v) {
		// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_e
	}
	Union_find_sum_amonoid() : n(0), m(0) {}

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_e

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) swap(ra, rb);

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
		val[ra] = op(val[ra], val[rb]);

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_e

		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_e

		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		return ra;
	}

	// 頂点 a の属する連結成分の和を返す．
	S sum(int a) {
		// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_e

		// a の根を調べ，そこに記録されている和の情報を返す．
		return val[leader(a)];
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		return m;
	}

	// 連結成分のリストを返す．
	vvi groups() {
		vvi res(m);

		vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].push_back(a);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Union_find_sum_amonoid d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << "sum: " << d.sum(g[0]) << endl;
		}
		return os;
	}
#endif
};


//【加算 Union-Find】
/*
* Union_find_add(int n) : O(n)
*	非連結で大きさ n の加算 Union-Find を値 0 で初期化する．
*
* merge(int a, int b) : O(log n)
*	頂点 a と頂点 b を統合する．
*
* bool same(int a, int b) : O(log n)
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* int leader(int a) : O(log n)
*	頂点 a の属する連結成分の親を返す．
*
* int size(int a) : O(log n)
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* void add(int a, ll val) : O(log n)
*	頂点 a を含む連結成分全体に val を加算する．
*
* ll get(int a) : O(log n)
*	頂点 a の値を返す．
*
* vvi groups() : O(n log n)
*	連結成分のリストを返す．
*/
struct Union_find_add {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	vl lazy; // 遅延させている値

	// 非連結で大きさ n の Union-Find を構築する．
	Union_find_add(int n_) : n(n_), m(n), parent_or_size(n, -1), lazy(n) {}
	Union_find_add() : n(0), m(0) {} // ダミー

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://yukicoder.me/problems/no/1054

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(ra, rb);
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		lazy[rb] -= lazy[ra];
		parent_or_size[rb] = ra;

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa);

		// 経路短絡はしない．
		return ra;
	}

	void add(int a, ll val) {
		// verify : https://yukicoder.me/problems/no/1054

		// a の根に遅延評価をセットする．
		int ra = leader(a);
		lazy[ra] += val;
	}

	ll get(int a) {
		// verify : https://yukicoder.me/problems/no/1054

		// a から根までの遅延評価を集める．
		ll res = 0;
		while (parent_or_size[a] >= 0) {
			res += lazy[a];
			a = parent_or_size[a];
		}
		res += lazy[a];
		return res;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		return m;
	}

	// 連結成分のリストを返す．
	vvi groups() {
		vvi res(m);

		vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].push_back(a);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Union_find_add d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【連想 Union-Find】
/*
* 頂点の統合と連結判定を行う．定数倍が悪いので注意．
*
* Union_find_set<T>() : O(1)
*	空の Union-Find を構築する．
*	制約：T はハッシュ化可能（だめなら map を使えばいいが log が付く）
*
* merge(T a, T b) : O(α(n))
*	頂点 a と頂点 b を統合する．
*
* bool same(T a, T b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* T leader(T a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* int size(T a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* int size() : O(1)
*	連結成分の個数を返す．
*
* vvT groups() : O(n α(n))
*	連結成分のリストを返す．
*/
template <class T>
class Union_find_set {
	int m; // 連結成分の個数

	// parent[c] : 子 v の親（v が子でなければ v 自身）
	unordered_map<T, T> c_to_p;

	// size[r] : 根 r の属する集合の大きさ
	unordered_map<T, int> r_to_sz;

	// 頂点 a が存在しなければ単独ノードとして追加する．
	void insert(T a) {
		if (c_to_p.count(a)) return;

		c_to_p[a] = a;
		r_to_sz[a] = 1;
		m++;
	}

public:
	// 空の Union-Find を構築する．
	Union_find_set() : m(0) {}

	// 頂点 a, b を結合する．
	void merge(T a, T b) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_c

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		T ra = leader(a);
		T rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (r_to_sz[ra] < r_to_sz[rb]) swap(ra, rb);

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		r_to_sz[ra] += r_to_sz[rb];
		r_to_sz.erase(rb);
		c_to_p[rb] = ra;

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(T a, T b) {
		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// 頂点 a の属する連結成分の根を返す．
	T leader(T a) {
		// verify : https://atcoder.jp/contests/abc279/tasks/abc279_f

		insert(a);

		// a が根であれば自分自身を返す．
		if (r_to_sz.count(a)) return a;

		// a が根でなければ，a の親の根 ra を求める．
		T ra = leader(c_to_p[a]);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		c_to_p[a] = ra;
		return ra;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(T a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return r_to_sz[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		return m;
	}

	// 連結成分のリストを返す．
	vector<vector<T>> groups() {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_c

		vector<vector<T>> res(m);

		unordered_map<T, int> r_to_i; int i = 0;
		repe(tmp, r_to_sz) r_to_i[tmp.first] = i++;

		repe(tmp, c_to_p) {
			T r = leader(tmp.first);
			res[r_to_i[r]].push_back(tmp.first);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Union_find_set d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << endl;
		}
		return os;
	}
#endif
};


