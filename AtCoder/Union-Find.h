#pragma once
#include "header.h"
// ■■■■■ Union-Find ■■■■■


//【Union-Find】
/*
* 頂点の統合と連結判定を行う．
*
* Union_find(n) : O(n)
*	非連結で大きさ n の Union-Find を構築する．
*
* merge(a, b) : O(α(n))
*	頂点 a と頂点 b を統合する．
*
* same(a, b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* leader(a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
*
* size(a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*
* size() : O(1)
*	連結成分の個数を返す．
*/
struct Union_find {
	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または集合の大きさ
	// 頂点 i が根でない場合は親の番号（非負）を，
	// 根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// コンストラクタ（初期化なし，大きさ n で初期化）
	Union_find() : n(0), m(0) {}
	Union_find(int n_) : n(n_), m(n), parent_or_size(n, -1) {}

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://judge.yosupo.jp/problem/unionfind

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) {
			return;
		}

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
		if (pa < 0) {
			return a;
		}

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		return ra;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}

	// 連結成分の個数を返す．
	int size() {
		//verify : https://atcoder.jp/contests/abc229/tasks/abc229_e

		return m;
	}
};


//【重み付き Union-Find】
/*
* 頂点間の差の指定と差の計算を行う．
*
* Weighted_union_find(n) : O(n)
*	非連結で大きさ n の重み付き Union-Find を構築する．
* 
* set_diff(a, b, d) : O(α(n))
*	v[b] - v[a] = d という関係を追加する．失敗は false を返す．
* 
* same(a, b) : O(α(n))
*	頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* get_diff(a, b) : O(α(n))
*	v[b] - v[a] を返す．（差が未確定なら戻り値は未定義とする）
* 
* leader(a) : O(α(n))
*	頂点 a の属する連結成分の親を返す．
* 
* size(a) : O(α(n))
*	頂点 a の属する連結成分の大きさを返す．
*/
template <class T> struct Weighted_union_find {
	int n; // 頂点の個数

	// parent_or_size[i] : 頂点 i の親または集合の大きさ
	// 頂点 i が根でない場合は親の番号（非負）を，
	// 根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// diff[i] : 親からみた頂点 i との差
	vector<T> diff;

	// コンストラクタ（初期化なし）
	Weighted_union_find() : n(0) {}

	// コンストラクタ（大きさ n で初期化）
	Weighted_union_find(int n_) : n(n_), parent_or_size(n, -1), diff(n) {}

	// 頂点 a, b 間の差 v[b] - v[a] を設定する．
	bool set_diff(int a, int b, T d) {
		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		// 既にある関係と整合しているかを返す．
		if (ra == rb) {
			return diff[b] - diff[a] == d;
		}

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(a, b);
			swap(ra, rb);
			d *= -1;
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
		diff[rb] = diff[a] - diff[b] + d;

		return true;
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// 根が同じなら連結である．
		return leader(a) == leader(b);
	}

	// v[b] - v[a] を返す．
	T get_diff(int a, int b) {
		// 差が確定していると仮定し，根からの差の差として計算する．
		return diff[b] - diff[a];
	}

	// 頂点 a の属する連結成分の根を返す．
	int leader(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) {
			return a;
		}

		// a が根でなければ，a の親 pa の根 ra を求める．
		// 再帰的な処理が回り，pa の親は ra になっていることに注意．
		int ra = leader(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		diff[a] += diff[pa];
		return ra;
	}

	// 頂点 a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[leader(a)];
	}
};


//【部分永続 Union-Find】
/*
* 各時刻での履歴を残しながら頂点の統合と連結判定を行う．
* 時刻とは，それまでに呼ばれた merge() の回数を意味する．
*
* Partially_persistent_union_find(n) : O(n)
*	非連結で大きさ n の部分永続 Union-Find を構築する．
*
* merge(a, b) : O(log n)
*	頂点 a と頂点 b を統合する．
*
* same(a, b, t) : O(log n)
*	時刻 t に頂点 a と頂点 b が同じ連結成分に属していたかを返す．
*
* leader(a, t) : O(log n)
*	時刻 t に頂点 a が属していた連結成分の親を返す．
*
* size(a, t) : O(log n)
*	時刻 t に頂点 a が属していた連結成分の大きさを返す．
*/
struct Partially_persistent_union_find {
	// 参考 : https://misteer.hatenablog.com/entry/persistentUF

	// 頂点の個数
	int n;

	// 現在時刻（過去に何回 merge() が呼ばれたか）
	int now;

	// parent[i] : 頂点 i の親（i が根なら i 自身）
	vi parent;

	// rank[i] : 頂点 i を根とする木の高さ（最も遠い葉までの距離）
	vi rank;

	// time[i] : 頂点 i の親が更新された時刻（i が親なら INF）
	vi time;

	// num[i] : 頂点 i を含む連結成分の時刻 t における頂点数が c であったことを
	//	(t, c) の形で t について狭義昇順に記録したリスト
	//	ただし全てではなく，「統合された側の根」にのみ情報を記録する．
	vector<vector<pii>> num;

	// コンストラクタ（初期化なし，大きさ n で初期化）
	Partially_persistent_union_find() : n(0), now(0) {}
	Partially_persistent_union_find(int n_)
		: n(n_), now(0), parent(n), rank(n, 1), time(n, INF), num(n)
	{
		rep(i, n) {
			parent[i] = i;
			num[i].push_back({ 0, 1 });
		}
	}

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017-open/tasks/code_thanks_festival_2017_h

		// 現在時刻を進める．
		now++;

		// ra[rb] : 頂点 a[b] の属する連結成分の根
		int ra = leader(a, now);
		int rb = leader(b, now);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい[小さい] 連結成分の根を改めて ra[rb] とする．
		if (rank[ra] < rank[rb]) swap(ra, rb);

		// rb を根とする連結成分を ra を根とする連結成分に統合する．
		num[ra].push_back({ now, size(ra, now) + size(rb, now) });
		parent[rb] = ra;
		if (rank[ra] == rank[rb]) rank[ra]++;
		time[rb] = now;
	}

	// 時刻 t に頂点 a, b が同じ連結成分に属していたかを返す．
	bool same(int a, int b, int t) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017-open/tasks/code_thanks_festival_2017_h

		// 根が同じなら連結である．
		return leader(a, t) == leader(b, t);
	}

	// 時刻 t に頂点 a が属する連結成分の根を返す．
	int leader(int a, int t) {
		// 頂点 a の親が更新される以前の場合
		if (t < time[a]) {
			// a 自身が根である．
			return a;
		}
		// 頂点 a の親が更新された以降の場合
		else {
			// a の親の親を再帰的に探しに行く．
			// 通常の Union-Find であれば経路圧縮を行うが，履歴をたどるには
			// 一度更新された親はそのままであってほしいので今回は行わない．
			return leader(parent[a], t);
		}
	}

	// 時刻 t に頂点 a が属する連結成分の大きさを返す．
	int size(int a, int t) {
		// ra : a の属する連結成分の根
		int ra = leader(a, t);

		// 時刻 t またはその直前の情報を得る．
		auto it = lower_bound(all(num[ra]), make_pair(t, INF));
		return prev(it)->second;
	}
};


//【Union-Find（偶奇判定付き）】
/*
* 頂点の統合と偶奇性を考慮した連結判定を行う．
*
* Parity_union_find(n) : O(n)
*	非連結な頂点 [0..n) で初期化する．
*
* merge_even(a, b) : O(α(n))
*	頂点 a と頂点 b を長さ 2 の辺で結ぶ．（統合とみなせる）
*
* merge_odd(a, b) : O(α(n))
*	頂点 a と頂点 b を長さ 1 の辺で結ぶ．
*
* same_even(a, b) : O(α(n))
*	頂点 a, b 間の距離が偶数かを返す．（非連結なら false を返す）
*
* same_odd(a, b) : O(α(n))
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


