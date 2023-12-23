#pragma once
#include "header.h"
// ■■■■■ 列の管理 ■■■■■


//【Mo's algorithm】O(n√q α + q log q)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を格納したリストを返す．
*
* 制約：両端の要素の追加 & 削除が O(α) で可能
* 
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm(const vector<T>& a, const vi& l, const vi& r) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011
	// verify : https://atcoder.jp/contests/abc174/tasks/abc174_f

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k + n 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q+1) + 1 となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int n = sz(a), q = sz(l);
	int sqrt_q = (int)(sqrt(q) + 1e-12);
	int width = max((n + sqrt_q - 1) / sqrt_q, 1);
	vector<S> res(q);

	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		int b = l[j] / width;
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));

	// -------------- ここを実装する（auto の方が速い） ---------------

	// 必要なデータ構造を用意する．
	int m = *max_element(all(a)) + 1;
	vi freq(m);
	S kind = 0;

	// 区間に a[i] を追加し，データ構造を更新する．
	auto insert = [&](int i) {
		if (freq[a[i]] == 0) kind++;
		freq[a[i]]++;
	};

	// 区間から a[i] を削除し，データ構造を更新する．
	auto erase = [&](int i) {
		freq[a[i]]--;
		if (freq[a[i]] == 0) kind--;
	};

	// クエリ j に対し，データ構造を参照して解を求める．
	auto get_sol = [&](int j) {
		return kind;
	};

	// --------------------------------------------------------------

	// lpt[rpt] : 半開区間の左[右] 端の位置
	int lpt = 0, rpt = 0; 

	// クエリを順に処理していく．
	rep(tmp, q) {
		int j = get<2>(lb_sr_j[tmp]);

		// 区間を広げる．
		while (lpt > l[j]) insert(--lpt);
		while (rpt < r[j]) insert(rpt++);

		// 区間を狭める．
		while (lpt < l[j]) erase(lpt++);
		while (rpt > r[j]) erase(--rpt);

		// 区間 [l[j]..r[j]) に対する解を得る．
		res[j] = get_sol(j);
	}

	return res;
}


//【Mo's algorithm（非対称）】O(n√q α + q log q)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を格納したリストを返す．
*
* 制約：両端の要素の追加 & 削除が O(α) で可能
*
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm_asymmetric(const vector<T>& a, const vi& l, const vi& r) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011
	// verify : https://judge.yosupo.jp/problem/static_range_inversions_query

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k + n 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q+1) + 1 となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int n = sz(a), q = sz(l);
	int sqrt_q = (int)(sqrt(q) + 1e-12);
	int width = max((n + sqrt_q - 1) / sqrt_q, 1);
	vector<S> res(q);

	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		int b = l[j] / width;
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));

	// -------------- ここを実装する（auto の方が速い） ---------------

	// 必要なデータ構造を用意する．
	int m = *max_element(all(a)) + 1;
	fenwick_tree<int> ft(m);
	ll inv = 0;

	// 区間の右に a[i] を追加し，データ構造を更新する．
	auto insert_right = [&](int i) {
		inv += ft.sum(a[i] + 1, m);
		ft.add(a[i], 1);
	};

	// 区間の左に a[i] を追加し，データ構造を更新する．
	auto insert_left = [&](int i) {
		inv += ft.sum(0, a[i]);
		ft.add(a[i], 1);
	};

	// 区間の右から a[i] を削除し，データ構造を更新する．
	auto erase_right = [&](int i) {
		inv -= ft.sum(a[i] + 1, m);
		ft.add(a[i], -1);
	};

	// 区間の左から a[i] を削除し，データ構造を更新する．
	auto erase_left = [&](int i) {
		inv -= ft.sum(0, a[i]);
		ft.add(a[i], -1);
	};

	// クエリ j に対し，データ構造を参照して解を求める．
	auto get_sol = [&](int j) {
		return inv;
	};

	// --------------------------------------------------------------

	// lpt[rpt] : 半開区間の左[右] 端の位置
	int lpt = 0, rpt = 0;

	// クエリを順に処理していく．
	rep(tmp, q) {
		int j = get<2>(lb_sr_j[tmp]);

		// 区間を広げる．
		while (lpt > l[j]) insert_left(--lpt);
		while (rpt < r[j]) insert_right(rpt++);

		// 区間を狭める．
		while (lpt < l[j]) erase_left(lpt++);
		while (rpt > r[j]) erase_right(--rpt);

		// 区間 [l[j]..r[j]) に対する解を得る．
		res[j] = get_sol(j);
	}

	return res;
}


//【Mo's algorithm（rollback）】O(n√q α + q log q)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を格納したリストを返す．
*
* 制約：両端の要素の追加が O(α) で可能，snapshot と rollback が O(α) 程度で可能
*
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm_rollback(const vector<T>& a, const vi& l, const vi& r) {
	// 参考 : https://snuke.hatenablog.com/entry/2016/07/01/000000
	// verify : https://codeforces.com/gym/100513/problem/A

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q) となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int n = sz(a), q = sz(l);
	int sqrt_q = (int)(sqrt(q) + 1e-12);
	int width = (n + sqrt_q - 1) / sqrt_q;
	vector<S> res(q);

	// クエリを左端の位置するブロックごとに分け，右端について昇順ソートする．
	vector<vector<pii>> lb_to_rj(sqrt_q);
	vi l_max(sqrt_q, -1); // ブロック内の左端位置の最大値
	rep(j, q) {
		int b = l[j] / width;
		lb_to_rj[b].emplace_back(r[j], j);
		chmax(l_max[b], l[j]);
	}
	rep(b, sqrt_q) sort(all(lb_to_rj[b]));

	// -------------- ここを実装する（auto の方が速い） ----------------

	// 区間を管理するデータ構造を用意する．
	Rollback_Union_find data;
	bool sol = true;

	// データ構造を初期化する．
	auto init = [&]() {
		data = Rollback_Union_find((int)2e5 + 10);
		sol = true;
	};

	// 区間に a[i] を追加し，データ構造 data を更新する．
	auto insert = [&](int i) {
		if (!sol) return;

		auto [u, v] = a[i];
		data.merge(u, v + n);
		data.merge(u + n, v);
		if (data.same(u, v)) sol = false;
	};

	// 現在のデータ構造の状態を一時記憶しておく．
	bool sol_tmp;
	auto snapshot = [&]() {
		data.snapshot();
		sol_tmp = sol;
	};

	// データ構造の状態を一時記憶してあったものに戻す．
	auto rollback = [&]() {
		data.rollback();
		sol = sol_tmp;
	};

	// クエリ j に対し，データ構造を参照して解を求める．
	auto get_sol = [&](int j) {
		return sol;
	};

	// --------------------------------------------------------------

	// b : 左から何番目のブロックか
	rep(b, sqrt_q) {
		// rpt : 半開区間の右端の位置（ブロック b 内の左端位置の最大値で初期化する）
		int rpt = l_max[b];
		if (rpt == -1) continue;

		// データ構造を初期化する．
		init();

		repe(tmp, lb_to_rj[b]) {
			// j : クエリ番号
			int j = tmp.second;

			// 区間の右側を伸ばしデータ構造を更新する．
			while (rpt < r[j]) insert(rpt++);

			// データ構造のスナップショットを作成する．
			snapshot();

			// a[l[j]..l_max[j]) の要素を右から順に追加していく．
			repir(i, min(l_max[b], r[j]) - 1, l[j]) insert(i);

			// a[l[j]..r[j]) に対する解を得る．
			res[j] = get_sol(j);

			// データ構造の状態をスナップショット作成時まで戻す．
			rollback();
		}
	}

	return res;
}


//【Mo's algorithm（マージ）】O(n√q α + q log q)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を格納したリストを返す．
*
* 制約：右端の要素の追加が O(α) で可能，左右に分かれたデータ構造からの解の計算が O(√n) 程度で可能
* 注意：データ構造そのもののマージが高速にできるのならセグメント木で十分．
*
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm_merge(const vector<T>& a, const vi& l, const vi& r) {
	// verify : https://atcoder.jp/contests/pakencamp-2022-day1/tasks/pakencamp_2022_day1_k

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q) となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int n = sz(a), q = sz(l);
	int sqrt_q = (int)(sqrt(q) + 1e-12);
	int width = (n + sqrt_q - 1) / sqrt_q;
	vector<S> res(q);

	// クエリを左端の位置するブロックごとに分け，右端について昇順ソートする．
	vector<vector<pii>> lb_to_rj(sqrt_q);
	vi l_max(sqrt_q, -1); // ブロック内の左端位置の最大値
	rep(j, q) {
		int b = l[j] / width;
		lb_to_rj[b].emplace_back(r[j], j);
		chmax(l_max[b], l[j]);
	}
	rep(b, sqrt_q) sort(all(lb_to_rj[b]));

	// -------------- ここを実装する（auto の方が速い） ---------------

	// 区間の左[右] 側を管理するデータ構造を用意する．
	using D = vl;
	D data_l, data_r;

	// データ構造 data を初期化する．
	auto init = [&](D& data) {
		data.clear();
	};

	// 区間の右に a[i] を追加し，データ構造 data を更新する．
	auto insert = [&](int i, D& data) {
		
	};

	// クエリ j に対し，左[右] 側のデータ構造 data_l[data_r] を参照して解を求める．
	auto get_sol = [&](int j) {

	};

	// --------------------------------------------------------------

	// b : 左から何番目のブロックか
	rep(b, sqrt_q) {
		// rpt : 半開区間の右端の位置（ブロック b 内の左端位置の最大値で初期化する）
		int rpt = l_max[b];

		// 区間の右側に対応するデータ構造を初期化する．
		init(data_r);

		repe(tmp, lb_to_rj[b]) {
			// j : クエリ番号
			int j = tmp.second;

			// 区間の右側に対応するデータ構造を更新する．
			while (rpt < r[j]) insert(rpt++, data_r);

			// 区間の左側に対応するデータ構造を初期化する．
			init(data_l);

			// a[l[j]..l_max[j]) の要素を左から順に追加していく．
			repi(i, l[j], min(l_max[b], r[j]) - 1) insert(i, data_l);

			// a[l[j]..r[j]) に対する解を得る．
			res[j] = get_sol(j);
		}
	}

	return res;
}


//【Implicit Treap（M-可換モノイド）】
/*
* Implicit_treap<S, op, e, F, act, comp, id>() : O(1)
*	空で初期化する．
*	要素は左作用付き可換モノイド (S, op, e, F, act, comp, id) の元とする．
*
* Implicit_treap<S, op, e, F, act, comp, id>(vS a) : O(n log n)
*	配列 a[0..n) で初期化する．
*
* bool empty() : O(1)
*	空かを返す．
*
* int size() : O(1)
*	要素数を返す．
*
* S get(int i) : O(log n)
*	a[i] を返す（なければ e() を返す）
*
* S prod(int l, int r) : O(log n)
*	Σa[l..r) を返す（空なら e() を返す）
*
* apply(int i, F f) : O(log n)
*	a[i] = f( a[i] ) とする．
*
* apply(int l, int r, F f) : O(log n)
*	a[l..r) = f( a[l..r) ) とする（空なら何もしない）
*
* int max_right(int l, function<bool(S)> g) : O(log n)
*	g( Σa[l..r) ) = true となる最大の r を返す．
*   制約：g( e() ) = true かつ g は単調
*
* int min_left(int r, function<bool(S)> g) : O(log n)
*	g( Σa[l..r) ) = true となる最小の l を返す．
*	制約：g( e() ) = true かつ g は単調
*
* insert(int i, S x) : O(log n)
*	a[i] = x を挿入する（元々あった要素は右に移動する）
*
* erase(int i) : O(log n)
*	a[i] の要素を削除し左詰めする（なければ何もしない）
*
* reverse(int l, int r) : O(log n)
*	a[l..r) を左右反転する．
*
* rotate(int l, int m, int r) : O(log n)
*	a[l..r) を，a[m] が先頭にくるよう巡回シフトする．
*
* Implicit_treap split(int key) : O(log n)
*	自身から位置 key 以上の要素を切り出し，切り出して出来た木を返す．
*
* void merge(Implicit_treap IT) : O(log n)
*	自身の右側に IT をマージする．
*
* vS get_all() : O(n)
*	全要素のリストを返す．
*/
template <class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Implicit_treap {
	// 参考 : https://xuzijian629.hatenablog.com/entry/2018/12/08/000452

	inline static bool first_call = true;
	inline static mt19937 rnd;

	struct Node {
		S value;				// 頂点の値
		S acc;					// 部分木のノードの総和
		F lazy;					// 部分木のノードへの遅延作用
		unsigned int priority;	// ランダムに決めた優先度
		int cnt;				// 部分木のノード数
		bool rev;				// 部分木が反転されているか
		Node* l, * r;			// 左右の子へのポインタ
		Node(S value, unsigned int priority) : value(value), acc(e()), lazy(id()), priority(priority),
			cnt(1), rev(false), l(nullptr), r(nullptr) {}
	};

	Node* root;

	// 部分木 t のノード数を返す．
	int cnt(Node* t) {
		return t ? t->cnt : 0;
	}

	// Σ(部分木 t) を返す．
	S acc(Node* t) {
		return t ? t->acc : e();
	}

	// 部分木 t のノード数を更新する．
	void update_cnt(Node* t) {
		if (t) t->cnt = cnt(t->l) + 1 + cnt(t->r);
	}

	// Σ(部分木 t) を更新する．
	void update_acc(Node* t) {
		if (t) t->acc = op(acc(t->l), op(t->value, acc(t->r)));
	}

	// 部分木 t の cnt と acc を更新する（子は更新済であること）
	void pushup(Node* t) {
		update_cnt(t);
		update_acc(t);
	}

	// 遅延評価を適用する．
	void pushdown(Node* t) {
		// 部分木 t の反転フラグが true なら，実際に反転させた上で反転フラグを false にする．
		if (t && t->rev) {
			t->rev = false;
			swap(t->l, t->r);

			// t の子については反転フラグを flip しておくだけにする．
			if (t->l) t->l->rev ^= 1;
			if (t->r) t->r->rev ^= 1;
		}

		// 部分木 t に作用が溜まっていたら，実際に作用させた上で作用を id() にする．
		if (t && t->lazy != id()) {
			// t の子については作用を遅延させておくだけにする（acc だけは更新する）
			if (t->l) {
				t->l->lazy = comp(t->lazy, t->l->lazy);
				t->l->acc = act(t->lazy, t->l->acc);
			}
			if (t->r) {
				t->r->lazy = comp(t->lazy, t->r->lazy);
				t->r->acc = act(t->lazy, t->r->acc);
			}

			t->value = act(t->lazy, t->value);
			t->lazy = id();
		}

		// 部分木 t の cnt と acc を更新する．
		pushup(t);
	}

	// 部分木 t を位置 key 未満[以上] に分割し，それぞれの根へのポインタを l[ r ] に格納する．
	void split(Node* t, int key, Node*& l, Node*& r) {
		// 空なら分割しなくていい．
		if (!t) {
			l = r = nullptr;
			return;
		}

		// t の情報を更新する．
		pushdown(t);

		// 部分木 t 内の自身の位置を得る．
		int implicit_key = cnt(t->l);

		if (key <= implicit_key) {
			// 左の木を分割しその左側を l に採用する．小さくなった右側は t->l に繋ぎ直す．
			split(t->l, key, l, t->l);
			r = t;
		}
		else {
			// 右の木を分割しその右側を r に採用する．小さくなった左側は t->r に繋ぎ直す．
			split(t->r, key - implicit_key - 1, t->r, r);
			l = t;
		}

		// 繋ぎ変えで部分木 t の cnt と acc が壊れたので更新する．
		pushup(t);
	}

	// 部分木 l, r をこの順にマージした部分木を t に格納する．
	void merge(Node*& t, Node* l, Node* r) {
		// l, r の情報を更新する．
		pushdown(l);
		pushdown(r);

		// 片方が空ならもう一方を根とすればよい．
		if (!l) t = r;
		else if (!r) t = l;
		// 優先度が高い方を根とし，もう一方をその子とマージする．
		else if (l->priority > r->priority) {
			merge(l->r, l->r, r);
			t = l;
		}
		else {
			merge(r->l, l, r->l);
			t = r;
		}

		// 部分木 t の cnt と acc を更新する．
		pushup(t);
	}

	int max_right(Node* t, S x, int offset, const function<bool(S)>& g) {
		if (!t) return offset;

		// t の情報を更新する．
		pushdown(t);

		// 左の子の中に答えがあるなら左の子へ
		if (t->l) {
			S nx = op(x, t->l->acc);
			if (!g(nx)) return max_right(t->l, x, offset, g);
			x = nx;
		}

		// 自身が答えならそれを返す．
		S nx = op(x, t->value);
		if (!g(nx)) return offset + cnt(t->l);
		x = nx;

		// 右の子の中に答えがあるなら右の子へ
		if (t->r) {
			S nx = op(x, t->r->acc);
			if (!g(nx)) return max_right(t->r, x, offset + cnt(t->l) + 1, g);
			x = nx;
		}

		// どこにもないなら右端を返す．
		return offset + cnt(t->l) + 1 + cnt(t->r);
	}

	int min_left(Node* t, S x, int offset, const function<bool(S)>& g) {
		if (!t) return offset;

		// t の情報を更新する．
		pushdown(t);

		// 右の子の中に答えがあるなら右の子へ
		if (t->r) {
			S nx = op(t->r->acc, x);
			if (!g(nx)) return min_left(t->r, x, offset + cnt(t->l) + 1, g);
			x = nx;
		}

		// 自身が答えならそれを返す．
		S nx = op(t->value, x);
		if (!g(nx)) return offset + cnt(t->l);
		x = nx;

		// 左の子の中に答えがあるなら左の子へ
		if (t->l) {
			S nx = op(t->l->acc, x);
			if (!g(nx)) return min_left(t->l, x, offset, g);
			x = nx;
		}

		// どこにもないなら左端を返す．
		return offset;
	}

	void get_all(Node* t, vector<S>& seq) {
		if (!t) return;
		pushdown(t);
		get_all(t->l, seq);
		seq.emplace_back(t->value);
		get_all(t->r, seq);
	}

public:
	// 空で初期化する．
	Implicit_treap() : root(nullptr) {
		// verify : https://www.spoj.com/problems/IITWPC4D/

		if (Implicit_treap::first_call) {
			rnd = mt19937((int)time(NULL));
			Implicit_treap::first_call = false;
		}
	}

	// 配列 a[0..n) で初期化する．
	Implicit_treap(const vector<S>& a) : root(nullptr) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		if (Implicit_treap::first_call) {
			rnd = mt19937((int)time(NULL));
			Implicit_treap::first_call = false;
		}

		rep(i, sz(a)) insert(i, a[i]);
	}

	// 要素が空かを返す．
	bool empty() {
		return !(bool)root;
	}

	// 要素数を返す．
	int size() {
		// verify : https://www.spoj.com/problems/IITWPC4D/

		return cnt(root);
	}

	// a[l..r) = f( a[l..r) ) とする（空なら何もしない）
	void apply(int l, int r, F f) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		if (l >= r) return;

		Node* lt, * mt, * rt;

		// [l..r) に対応する部分木 mt を切り出してくる．
		split(root, l, lt, rt);
		split(rt, r - l, mt, rt);

		// mt への作用 f を遅延させる（acc だけは更新する）
		if (mt) {
			mt->lazy = comp(f, mt->lazy);
			mt->acc = act(f, mt->acc);
		}

		// 木を元に戻しておく．
		merge(rt, mt, rt);
		merge(root, lt, rt);
	}

	// a[i] = f( a[i] ) とする（なければ何もしない）
	void apply(int i, F f) {
		apply(i, i + 1, f);
	}

	// Σa[l..r) を返す（空なら e() を返す）
	S prod(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		if (l >= r) return e();

		Node* lt, * mt, * rt;

		// [l..r) に対応する部分木 mt を切り出してくる．
		split(root, l, lt, rt);
		split(rt, r - l, mt, rt);

		// 値は既に acc に格納されている．
		S res = acc(mt);

		// 木を元に戻しておく．
		merge(rt, mt, rt);
		merge(root, lt, rt);

		return res;
	}

	// a[i] を返す（なければ e() を返す）
	S get(int i) {
		// verify : https://www.spoj.com/problems/IITWPC4D/

		return prod(i, i + 1);
	}

	// g( Σa[l..r) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(S)>& g) {
		// verify : https://atcoder.jp/contests/practice2/tasks/practice2_j

		Node* lt, * rt;

		// [l..n) に対応する部分木 rt を切り出してくる．
		split(root, l, lt, rt);

		S x = e();
		int res = max_right(rt, x, l, g);

		// 木を元に戻しておく．
		merge(root, lt, rt);

		return res;
	}

	// g( Σa[l..r) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(S)>& g) {
		Node* lt, * rt;

		// [0..r) に対応する部分木 lt を切り出してくる．
		split(root, r, lt, rt);

		S x = e();
		int res = min_left(lt, x, 0, g);

		// 木を元に戻しておく．
		merge(root, lt, rt);

		return res;
	}

	// a[i] = x を挿入する（元々あった要素は右に移動する）
	void insert(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		Node* lt, * rt;

		// 一旦 i で分割し，x を挟んでからマージする．
		split(root, i, lt, rt);
		merge(lt, lt, new Node(x, rnd()));
		merge(root, lt, rt);
	}

	// a[i] の要素を削除し左詰めする（なければ何もしない）
	void erase(int i) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		Node* lt, * mt, * rt;

		// i の前後で分割し，i だけ除いてマージする．
		split(root, i + 1, lt, rt);
		split(lt, i, lt, mt);
		merge(root, lt, rt);
	}

	// a[l..r) を左右反転する．
	void reverse(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		if (l >= r) return;

		Node* lt, * mt, * rt;

		// [l..r) に対応する部分木 mt を切り出してくる．
		split(root, l, lt, rt);
		split(rt, r - l, mt, rt);

		// 反転フラグを flip する．
		mt->rev ^= 1;

		// 木を元に戻しておく．
		merge(rt, mt, rt);
		merge(root, lt, rt);
	}

	// a[l, r) を，a[m] が先頭にくるよう巡回シフトする．
	void rotate(int l, int m, int r) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/1508

		// 全体を反転
		reverse(l, r);

		// 左右それぞれを反転
		reverse(l, l + r - m);
		reverse(l + r - m, r);
	}

	// 自身から位置 key 以上の要素を切り出し，切り出して出来た木を返す．
	Implicit_treap split(int key) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		Node* l, * r;
		split(root, key, l, r);
		root = l;
		Implicit_treap ret;
		ret.root = r;
		return ret;
	}

	// 自身の右側に IT をマージする．
	void merge(Implicit_treap& IT) {
		// verify : https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

		merge(root, root, IT.root);
	}

	// 全要素のリストを返す．
	vector<S> get_all() {
		// verify : https://www.spoj.com/problems/TWIST/

		vector<S> seq;
		get_all(root, seq);
		return seq;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Implicit_treap IT) {
		os << IT.get_all();
		return os;
	}
#endif
};


//【キュー（モノイド）】
/*
* Queue_SWAG<S, op, e>() : O(1)
*	空のキューで初期化する．要素はモノイド (S, op, e) の元とする．
*
* push(S x) : O(1)
*	キューの末尾に x を追加する．
*
* pop() : ならし O(1)
*	キューの先頭の要素を削除する（空なら何もしない）
*
* S prod() : O(1)
*	キューの要素の先頭から順にとった総積を返す（空なら e() を返す）
*/
template <class S, S(*op)(S, S), S(*e)()>
class Queue_SWAG {
	// 参考 : https://motsu-xe.hatenablog.com/entry/2021/05/13/224016

	// acc_f[b] : キューの先頭[末尾] 側の要素の先頭側から順にとった累積積を格納するスタック
	// raw_b : キューの末尾側の要素を格納するスタック
	stack<S> acc_f, acc_b, raw_b;

	// 末尾側の要素をまとめて先頭側に移動する．
	void move() {
		while (!raw_b.empty()) {
			S l = raw_b.top(); raw_b.pop(); acc_b.pop();
			S r = acc_f.empty() ? e() : acc_f.top();
			acc_f.push(op(l, r));
		}
	}

public:
	// 空のキューで初期化する．
	Queue_SWAG() {
		// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite
	}

	// キューの末尾に x を追加する．
	void push(S x) {
		// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite

		raw_b.push(x);
		S l = acc_b.empty() ? e() : acc_b.top();
		acc_b.push(op(l, x));
	}

	// キューの先頭の要素を削除する．
	void pop() {
		// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite

		if (acc_f.empty()) move();
		if (acc_f.empty()) return;
		acc_f.pop();
	}

	// キューの要素の先頭から順にとった総積を返す．
	S prod() {
		// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite

		S l = acc_f.empty() ? e() : acc_f.top();
		S r = acc_b.empty() ? e() : acc_b.top();
		return op(l, r);
	}
};


//【両端キュー（モノイド）】
/*
* Deque_SWAG<S, op, e>() : O(1)
*	空の両端キューで初期化する．要素はモノイド (S, op, e) の元とする．
*
* push_front(S x) : O(1)
*	両端キューの先頭に x を追加する．
*
* push_back(S x) : O(1)
*	両端キューの末尾に x を追加する．
*
* pop_front() : ならし O(log n) ?
*	両端キューの先頭の要素を削除する（空なら何もしない）
*
* pop_back() : ならし O(log n) ?
*	両端キューの末尾の要素を削除する（空なら何もしない）
*
* S prod() : O(1)
*	両端キューの要素の先頭から順にとった総積を返す（空なら e() を返す）
*/
template <class S, S(*op)(S, S), S(*e)()>
class Deque_SWAG {
	// 参考 : https://motsu-xe.hatenablog.com/entry/2021/05/13/224016

	// acc_f[b] : 両端キューの先頭[末尾] 側の要素の先頭側から順にとった累積積を格納する両端キュー
	// raw : 元の要素を格納する両端キュー
	deque<S> acc_f, acc_b, raw;

	// 末尾側の要素の半分を先頭側へ移動する．
	void move_to_front() {
		// n : 要素数
		int n = sz(raw), hn = (n + 1) / 2;

		// 中央より前を先頭側に移す．
		repir(i, hn - 1, 0) {
			S l = raw[i];
			S r = (i == hn - 1) ? e() : acc_f.front();
			acc_f.push_front(op(l, r));
		}

		// 中央より後を末尾側に残す．
		deque<S> nacc_b;
		repi(i, hn, n - 1) {
			S l = (i == hn) ? e() : nacc_b.back();
			S r = raw[i];
			nacc_b.push_back(op(l, r));
		}
		acc_b = move(nacc_b);
	}

	// 先頭側の要素の半分を末尾側へ移動する．
	void move_to_back() {
		// n : 要素数
		int n = sz(raw), hn = n / 2;

		// 中央より後を末尾側に移す．
		repi(i, hn, n - 1) {
			S l = (i == hn) ? e() : acc_b.back();
			S r = raw[i];
			acc_b.push_back(op(l, r));
		}

		// 中央より前を先頭側に残す．
		deque<S> nacc_f;
		repir(i, hn - 1, 0) {
			S l = raw[i];
			S r = (i == hn - 1) ? e() : nacc_f.front();
			nacc_f.push_front(op(l, r));
		}
		acc_f = move(nacc_f);
	}

public:
	// 空の両端キューで初期化する．
	Deque_SWAG() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite
	}

	// 両端キューの先頭に x を追加する．
	void push_front(S x) {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		S r = acc_f.empty() ? e() : acc_f.front();
		acc_f.push_front(op(x, r));
		raw.push_front(x);
	}

	// 両端キューの末尾に x を追加する．
	void push_back(S x) {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		S l = acc_b.empty() ? e() : acc_b.back();
		acc_b.push_back(op(l, x));
		raw.push_back(x);
	}

	// 両端キューの先頭の要素を削除する．
	void pop_front() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		// 両端キューが空の場合の例外処理
		if (raw.empty()) return;

		// 先頭側が空だった場合は末尾側の要素の半分を移してくる．
		if (acc_f.empty()) move_to_front();

		acc_f.pop_front();
		raw.pop_front();
	}

	// 両端キューの末尾の要素を削除する．
	void pop_back() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		// 両端キューが空の場合の例外処理
		if (raw.empty()) return;

		// 末尾側が空だった場合は先頭側の要素の半分を移してくる．
		if (acc_b.empty()) move_to_back();

		acc_b.pop_back();
		raw.pop_back();
	}

	// 両端キューの要素の先頭から順にとった総積を返す．
	S prod() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		S l = acc_f.empty() ? e() : acc_f.front();
		S r = acc_b.empty() ? e() : acc_b.back();
		return op(l, r);
	}
};


//【両端キュー（群）】
/*
* Deque_group<S, op, e, inv>() : O(1)
*	空の両端キューで初期化する．要素は群 (S, op, e, inv) の元とする．
*
* push_front(S x) : O(1)
*	両端キューの先頭に x を追加する．
*
* push_back(S x) : O(1)
*	両端キューの末尾に x を追加する．
*
* pop_front() : ならし O(log n) ?
*	両端キューの先頭の要素を削除する（空なら何もしない）
*
* pop_back() : ならし O(log n) ?
*	両端キューの末尾の要素を削除する（空なら何もしない）
*
* S prod() : O(1)
*	両端キューの要素の先頭から順にとった総積を返す（空なら e() を返す）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
class Deque_group {
	// acc : 累積積
	S acc;

	// 元の要素を格納する両端キュー
	deque<S> raw;

public:
	// 空の両端キューで初期化する．
	Deque_group() : acc(e()) {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite
	}

	// 両端キューの先頭に x を追加する．
	void push_front(S x) {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		acc = op(x, acc);
		raw.push_front(x);
	}

	// 両端キューの末尾に x を追加する．
	void push_back(S x) {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		acc = op(acc, x);
		raw.push_back(x);
	}

	// 両端キューの先頭の要素を削除する．
	void pop_front() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		if (raw.empty()) return;

		acc = op(inv(raw.front()), acc);
		raw.pop_front();
	}

	// 両端キューの末尾の要素を削除する．
	void pop_back() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		if (raw.empty()) return;

		acc = op(acc, inv(raw.back()));
		raw.pop_back();
	}

	// 両端キューの要素の先頭から順にとった総積を返す．
	S prod() {
		// verify : https://judge.yosupo.jp/problem/deque_operate_all_composite

		return acc;
	}
};


//【バケット平方分割（モノイド）】
/*
* Quadratic_division<S, op, e>(int n) : O(n e)
*	v[0..n) = e() で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Quadratic_division<S, op, e>(vS v) : O(n op)
*	v[0..n) で初期化する．
*
* set(int i, S x) : O(√n op)
*	v[i] = x とする．
*
* S get(int i) : O(1)
*	v[i] を返す．
*
* S prod(int l, int r) : O(√n op)
*	Πv[l..r) を返す．空なら e() を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
struct Quadratic_division {
	using vS = vector<S>;

	int n, w, m; // n : 要素数，w : バケット幅，m : バケット数
	vector<S> v, v_mul;

	// コンストラクタ（e() で初期化）
	Quadratic_division(int n) : n(n) {
		w = (int)(sqrt(n) + 1e-12);
		m = (n + w - 1) / w;

		v = vS(n, e());
		v_mul = vS(m, e());
	}

	// コンストラクタ（配列で初期化）
	Quadratic_division(vector<S>& v_) : Quadratic_division(sz(v_)) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite
		
		v = v_;
		v_mul = vS(m, e());
		rep(i, n) {
			int j = i / w;
			v_mul[j] = op(v_mul[j], v[i]);
		}
	}

	// v[i] = x とする．
	void set(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		// 要素 v[i] の更新
		v[i] = x;

		// v[i] を含むバケットの総積を再計算する．
		int j = i / w, i_min = j * w, i_max = min(i_min + w, n) - 1;
		v_mul[j] = e();
		repi(i, i_min, i_max) v_mul[j] = op(v_mul[j], v[i]);
	}

	// v[i] を返す．
	S get(int i) const { return v[i]; }

	// Πv[l..r) を返す．空なら e() を返す．
	S prod(int l, int r) const {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite
		
		chmax(l, 0); chmin(r, n);
		if (l >= r) return e();

		S res = e();

		int j_min = l / w + 1, j_max = r / w - 1;

		if (j_min <= j_max) {
			repi(i, l, j_min * w - 1) res = op(res, v[i]);
			repi(j, j_min, j_max) res = op(res, v_mul[j]);
			repi(i, (j_max + 1) * w, r - 1) res = op(res, v[i]);
		}
		else {
			repi(i, l, r - 1) res = op(res, v[i]);
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Quadratic_division qd) {
		rep(i, qd.n) {
			os << qd.get(i) << " ";
		}
		return os;
	}
#endif
};


//【バケット平方分割（M-集合）】
/*
* Quadratic_division<S, F, act, comp, id>(int n) : O(n id)
*	v[0..n) = id() で初期化する．
*	要素は M-集合 (S, F, act, comp, id) の作用素集合 F の元とする．
*
* Quadratic_division<S, F, act, comp, id>(vF v) : O(n comp)
*	配列 v[0..n) の要素で初期化する．
*
* set(int i, F f) : O(√n comp) // 何度も呼ぶと遅い
*	v[i] = f とする．
*
* F get(int i) : O(id)
*	v[i] を返す．
*
* S prod(int l, int r, S x) : O(√n act)
*	v[r-1] ... v[l] x を返す．空なら x を返す．
*/
template <class S, class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
struct Quadratic_division_Mset {
	using vF = vector<F>;

	int n, w, m; // n : 要素数，w : バケット幅，m : バケット数
	vF v, v_mul;

	// コンストラクタ（e() で初期化）
	Quadratic_division_Mset(int n_) : n(n_) {
		w = (int)(sqrt(n) + 0.001);
		m = (n + w - 1) / w;

		v = vF(n, id());
		v_mul = vF(m, id());
	}

	// コンストラクタ（配列で初期化）
	Quadratic_division_Mset(const vF& v_) {
		// verify : https://atcoder.jp/contests/arc027/tasks/arc027_4

		n = sz(v_);
		w = (int)(sqrt(n) + 0.001);
		m = (n + w - 1) / w;

		v = v_;
		v_mul = vF(m, id());
		rep(i, n) {
			int j = i / w;
			v_mul[j] = comp(v_mul[j], v[i]);
		}
	}

	// v[i] = x とする．
	void set(int i, F x) {
		// 要素 v[i] の更新
		v[i] = x;

		// v[i] を含むバケットの総積を再計算する．
		int j = i / w, i_min = j * w, i_max = min(i_min + w, n) - 1;
		v_mul[j] = id();
		repi(i, i_min, i_max) v_mul[j] = comp(v_mul[j], v[i]);
	}

	// v[i] を返す．
	F get(int i) const { return v[i]; }

	// v[l..r) x を返す．空なら x を返す．
	S prod(int l, int r, S x) const {
		// verify : https://atcoder.jp/contests/arc027/tasks/arc027_4

		int j_min = l / w + 1, j_max = r / w - 1;

		if (j_min <= j_max) {
			repi(i, l, j_min * w - 1) x = act(v[i], x);
			repi(j, j_min, j_max) x = act(v_mul[j], x);
			repi(i, (j_max + 1) * w, r - 1) x = act(v[i], x);
		}
		else {
			repi(i, l, r - 1) x = act(v[i], x);
		}

		return x;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Quadratic_division_Mset qd) {
		rep(i, qd.n) os << qd.get(i) << " ";
		return os;
	}
#endif
};


//【rollback 配列】
/*
* Rollback_array<T>(int n) : O(n)
*	a[0..n) を T の初期値で初期化する．
*
* Rollback_array<T>(int n, T x) : O(n)
*	a[0..n) を x で初期化する．
*
* Rollback_array<T>(vT a) : O(n)
*	a[0..n) で初期化する．
*
* void set(int i, T x) : O(1)
*	a[i] = x とする．
*
* T get(int i) : O(1)
*	a[i] を返す．
*
* void snapshot() : O(1)
*	スナップショットを作成する．
*
* rollback() : O(1)
*	直前に作成したスナップショットの状態まで巻き戻し，スナップショットを破棄する．
*/
template <class T>
class Rollback_array {
	int n;
	vector<T> a;
	stack<pair<int, T>> history;

public:
	// a[0..n) を T の初期値で初期化する．
	Rollback_array(int n) : n(n), a(n) {}

	// a[0..n) を x で初期化する．
	Rollback_array(int n, T x) : n(n), a(n, x) {
		// verify : https://yukicoder.me/problems/no/2338
	}

	// a[0..n) で初期化する．
	Rollback_array(const vector<T>& a) : n(sz(a)), a(a) {}
	Rollback_array() : n(0) {}

	// a[i] = x とする．
	void set(int i, T x) {
		// verify : https://yukicoder.me/problems/no/2338

		Assert(0 <= i && i < n);

		history.emplace(i, a[i]);
		a[i] = x;
	}

	// a[i] を返す．
	T get(int i) const {
		// verify : https://yukicoder.me/problems/no/2338

		return a[i];
	}

	// スナップショットを作成する．
	void snapshot() {
		// verify : https://yukicoder.me/problems/no/2338

		history.emplace(INF, 0);
	}

	// 直前に作成したスナップショットの状態まで巻き戻す．
	void rollback() {
		// verify : https://yukicoder.me/problems/no/2338

		while (true) {
			auto [i, x] = history.top(); history.pop();
			if (i == INF) break;
			a[i] = x;
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Rollback_array RA) {
		os << RA.a << endl;
		return os;
	}
#endif
};


//【永続スタック】
/*
* Persistent_stack<T>() : O(1)
*	型 T の空スタックで初期化する．履歴番号は 0 とする．
*
* bool empty(int t = n - 1) : O(1)
*	t 番目の履歴が空スタックかを返す．
*
* int size(int t = n - 1) : O(1)
*	t 番目の履歴のスタックの大きさを返す．
*
* T top(int t = n - 1) : O(1)
*	t 番目の履歴のスタックの先頭の値を返す（t = 0 のとき値は未定義）
*
* int push(T val, int t = n - 1) : O(1)
*	t 番目の履歴に対し val を先頭に積んだスタックを最新の履歴として記録し，履歴番号を返す．
*
* int pop(int t = n - 1) : O(1)
*	t 番目の履歴に対し先頭要素を積む前のスタックの履歴番号を返す．
*/
template <class T>
class Persistent_stack {
	// 参考 : https://qiita.com/wotsushi/items/72e7f8cdd674741ffd61

	struct Node {
		T val = -1; // スタックの先頭の値
		int size = 0; // スタックの大きさ
		int prev = -1; // 直前の状態の履歴番号（なければ -1）

		// コンストラクタ
		Node(T val_, int size_, int prev_) : val(val_), size(size_), prev(prev_) {}
		Node() {}

#ifdef _MSC_VER
		// 出力
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(v:" << v.val << ", s:" << v.size << ", p:" << v.prev << ')';
			return os;
		}
#endif
	};

	int n; // 履歴の個数
	vector<Node> stk; // 履歴

public:
	// 型 T の空スタックで初期化する．
	Persistent_stack() : n(1), stk(1) {}

	// t 番目の履歴のスタックの大きさを返す．
	int size(int t) {
		Assert(0 <= t && t < n);
		return stk[t].size;
	}
	int size() { return size(n - 1); }

	// t 番目の履歴が空スタックかを返す．
	bool empty(int t) {
		Assert(0 <= t && t < n);
		return t == 0;
	}
	bool empty() { return empty(n - 1); }

	// t 番目の履歴のスタックの先頭の値を返す（t = 0 のとき値は未定義）
	T top(int t) {
		Assert(0 <= t && t < n);
		return stk[t].val;
	}
	T top() { return top(n - 1); }

	// t 番目の履歴に対し val を先頭に積んだスタックを最新の履歴として記録し，履歴番号を返す．
	int push(T val, int t) {
		Assert(0 <= t && t < n);
		stk.push_back(Node(val, stk[t].size + 1, t));
		return n++;
	}
	int push(T val) { return push(val, n - 1); }

	// t 番目の履歴に対し先頭要素を積む前のスタックの履歴番号を返す．
	int pop(int t) {
		Assert(0 <= t && t < n);
		return stk[t].prev;
	}
	int pop() { return pop(n - 1); }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Persistent_stack& ps) {
		rep(i, ps.n) os << i << ": " << ps.stk[i] << endl;
		return os;
	}
#endif
};


//【永続キュー】
/*
* Persistent_queue<T>() : O(1)
*	型 T の空キューで初期化する．履歴番号は 0 とする．
*
* bool empty(int t) : O(1)
*	t 番目の履歴が空キューかを返す．
*
* int size(int t) : O(1)
*	t 番目の履歴のキューの大きさを返す．
*
* T front(int t) : O(log n)（n : キューの大きさ）
*	t 番目の履歴のキューの先頭の値を返す．
*
* T back(int t) : O(1)
*	t 番目の履歴のキューの末尾の値を返す．
*
* int push(T val, int t) : O(log n)（n : キューの大きさ）
*	t 番目の履歴に対し val を末尾に追加したキューを最新の履歴として記録し，履歴番号を返す．
*
* int pop(int t) : O(1)
*	t 番目の履歴に対し先頭要素を削除したキューを最新の履歴として記録し，履歴番号を返す．
*/
template <class T>
class Persistent_queue {
	struct Node {
		T val = -1; // キューの末尾の値
		vector<Node*> nx; // nx[i] : 2^i 個先のノードへのポインタ（ダブリング用）

		// コンストラクタ（val : 追加する値，p : 直前の末尾ノードへのポインタ）
		Node(T val_, Node* nx0) : val(val_) {
			nx.push_back(nx0);
			int i = 0; Node* p = nx0;
			while (i < sz(p->nx)) {
				nx.push_back(p->nx[i++]);
				p = nx.back();
			}
		}
		Node() {}

		// i 個先のノードへのポインタを返す．（なければ nullptr）
		Node* next(int i) {
			if (i == 0) return this;

			// ダブリングで求める．
			int b = msb(i);
			if (b >= sz(nx)) return nullptr;
			return nx[b]->next(i - (1 << b));
		}
	};

	int n; // 履歴の個数
	vector<Node*> his_p; // 履歴（末尾ノードへのポインタ）
	vi his_len; // 履歴（キューの大きさ）

public:
	// 型 T の空キューで初期化する．
	Persistent_queue() : n(1), his_p(1), his_len(1) {
		his_p[0] = new Node();
	}

	// t 番目の履歴のキューの大きさを返す．
	int size(int t) {
		Assert(0 <= t && t < n);
		return his_len[t];
	}

	// t 番目の履歴が空キューかを返す．
	bool empty(int t) {
		Assert(0 <= t && t < n);
		return size(t) == 0;
	}

	// t 番目の履歴のキューの先頭の値を返す．
	T front(int t) {
		Assert(0 <= t && t < n);
		return his_p[t]->next(his_len[t] - 1)->val;
	}

	// t 番目の履歴のキューの末尾の値を返す．
	T back(int t) {
		Assert(0 <= t && t < n);
		return his_p[t]->val;
	}

	// t 番目の履歴に対し val を末尾に追加したキューを最新の履歴として記録し，履歴番号を返す．
	int push(T val, int t) {
		Assert(0 <= t && t < n);
		Node* p = new Node(val, his_p[t]);
		his_p.push_back(p);
		his_len.push_back(his_len[t] + 1);
		return n++;
	}

	// t 番目の履歴に対し先頭要素を削除したキューを最新の履歴として記録し，履歴番号を返す．
	int pop(int t) {
		Assert(0 <= t && t < n);
		his_p.push_back(his_p[t]);
		his_len.push_back(his_len[t] - 1);
		return n++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Persistent_queue& q) {
		rep(t, q.n) {
			vector<T> seq; Node* p = q.his_p[t];
			rep(i, q.his_len[t]) {
				seq.push_back(p->val);
				p = p->nx[0];
			}
			reverse(all(seq));
			os << t << ": " << seq << endl;
		}
		return os;
	}
#endif
};


//【永続配列】
/*
* Persistent_array<S>(int n) : O(n)
*	v[0..n) = 0 で初期化する．履歴番号は 0 とする．
*
* Persistent_segtree<S>(vS v) : O(n)
*	配列 v[0..n) の要素で初期化する．履歴番号は 0 とする．
*
* int set(int i, S x, int t) : O(log n)
*	t 番目の履歴に対し v[i] = x とした配列を最新の履歴として記録し，履歴番号を返す．
*
* S get(int i, int t) : O(log n)
*	t 番目の履歴の v[i] を返す．
*/
template <class S>
class Persistent_array {
	// 参考 : https://qiita.com/hotman78/items/9c643feae1de087e6fc5

	static const int M = 20; // 子の数

	struct Node {
		int l, r;
		S val; // 葉なら値
		vi ch; // 子

		Node(int l, int r, S val) : l(l), r(r), val(val) {}
	};

	// ノードを貯めておくプール
	vector<Node> pool;

	int n; // 配列の大きさ
	int T; // 履歴の個数
	vi his; // 履歴

	// ノードの新規作成
	int new_node(int l, int r, S val = 0) {
		pool.emplace_back(l, r, val);
		return sz(pool) - 1;
	}

	int init_rf(const vector<S>& v, int l, int r) {
		if (r - l <= 0) return -1;

		// 葉を作る場合
		if (r - l == 1) return new_node(l, r, v[l]);

		int id = new_node(l, r);
		pool[id].ch.resize(M);
		int w = r - l;
		rep(k, M) pool[id].ch[k] = init_rf(v, l + w * k / M, l + w * (k + 1) / M);

		return id;
	}

	int set_rf(int id, int i, S x) {
		// pool[id] が葉の場合
		if (pool[id].ch.empty()) return new_node(pool[id].l, pool[id].r, x);

		int nid = new_node(pool[id].l, pool[id].r);
		pool[nid].ch = pool[id].ch;
		int w = pool[id].r - pool[id].l;
		int k = ((i - pool[id].l + 1) * M - 1) / w;
		pool[nid].ch[k] = set_rf(pool[id].ch[k], i, x);

		return nid;
	}

	S get_rf(int id, int i) const {
		const Node& p = pool[id];

		// p が葉の場合
		if (p.ch.empty()) return p.val;

		int w = p.r - p.l;
		int k = ((i - p.l + 1) * M - 1) / w;
		return get_rf(p.ch[k], i);
	}

	void print_rf(int id, ostream& os) const {
		if (id == -1) return;

		const Node& p = pool[id];

		if (p.ch.empty()) {
			os << p.val << " ";
			return;
		}

		rep(k, M) print_rf(p.ch[k], os);
	}

public:
	// 配列 v[0..n) の要素で初期化する．
	Persistent_array(const vector<S>& v) : n(sz(v)), T(1), his(1) {
		pool.reserve(n * 2);
		his[0] = init_rf(v, 0, n);
	}

	// v[0..n) = 0 で初期化する．
	Persistent_array(int n) : n(n), T(1), his(1) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		vector<S> v(n, 0);
		pool.reserve(n * 2);
		his[0] = init_rf(v, 0, n);
	}
	Persistent_array() : n(0), T(0) {} // ダミー

	// t 番目の履歴に対し v[i] = x とした配列を最新の履歴として記録し，履歴番号を返す．
	int set(int i, S x, int t) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		Assert(0 <= i && i < n);
		Assert(t < T);
		his.push_back(set_rf(his[t], i, x));
		return T++;
	}

	// t 番目の履歴の v[i] を返す．
	S get(int i, int t) const {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		Assert(0 <= i && i < n);
		Assert(t < T);
		return get_rf(his[t], i);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Persistent_array& pa) {
		rep(t, pa.T) {
			os << t << ": ";
			pa.print_rf(pa.his[t], os);
			os << endl;
		}
		return os;
	}
#endif
};


//【二分探索木】
/*
* Binary_search_tree() : O(1)
*	空の二分探索木で初期化する．
*
* insert(key) : 平均 O(log n)／最悪 O(n)
*	key を挿入する．
*
* find(key) : 平均 O(log n)／最悪 O(n)
*	key が存在すれば true, さもなくば false を返す．
*
* erase(key) : 平均 O(log n)／最悪 O(n)
*	key を削除する．
*/
template <class T>
struct Binary_search_tree {
verify: https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_C

	// 二分探索木のノード
	struct Node {
		T key;
		Node* left;
		Node* right;
	};

	int n; // ノード数
	Node* root; // 根へのポインタ

	// コンストラクタ（空の二分探索木で初期化）：O(1)
	Binary_search_tree() : n(0), root(nullptr) {}

	// 要素の挿入：平均 O(log n)／最悪 O(n)
	void insert(T& key) {
		// 挿入すべき位置を二分探索する．
		Node** p = &root;
		while (*p != nullptr) {
			if (key < (*p)->key) {
				p = &((*p)->left);
			}
			else {
				p = &((*p)->right);
			}
		}

		// 新しいノードの作成
		Node* node = new Node;
		node->key = key;
		node->left = nullptr;
		node->right = nullptr;
		*p = node;
		n++;
	}

	// 要素の探索：平均 O(log n)／最悪 O(n)
	bool find(T& key) {
		// key を二分探索する．
		Node* x = root;
		while (x != nullptr) {
			if (key == x->key) {
				return true;
			}

			if (key < x->key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		return false;
	}

	// 要素の削除：平均 O(log n)／最悪 O(n)
	void erase(T& key) {
		// 挿入すべき位置を二分探索する．
		Node** p = &root;
		while (*p != nullptr) {
			// 見つかった場合
			if (key == (*p)->key) {
				// 見つかったノードを x とする．
				Node* x = *p;

				// x が子をもたない場合
				if (x->left == nullptr && x->right == nullptr) {
					*p = nullptr;
				}
				// x が右の子のみをもつ場合
				else if (x->left == nullptr) {
					// 短絡する．
					*p = x->right;
				}
				// x が左の子のみをもつ場合
				else if (x->right == nullptr) {
					// 短絡する．
					*p = x->left;
				}
				// x が左右の子をもつ場合
				else {
					// x の右の子から最小の要素を削除し，
					// その要素へ親からのポインタを繋ぐ．
					*p = delete_min(&(x->right));

					// その要素の左右の子を x の左右の子とする．
					(*p)->right = x->right;
					(*p)->left = x->left;
				}

				delete x;
				n--;

				return;
			}

			// 見つからなかった場合は子を探しに行く．
			if (key < (*p)->key) {
				p = &((*p)->left);
			}
			else {
				p = &((*p)->right);
			}
		}
	}

	// *p が指す部分木の最小の要素を削除し，その要素へのポインタを返す．
	Node* delete_min(Node** p) {
		// 最小の要素は左の子をずっと辿った先にある．
		while ((*p)->left != nullptr) {
			p = &((*p)->left);
		}

		// 最小の要素には左の子は居ないので，
		// 親からのポインタに右の子を繋いで短絡しておく．
		Node* x = *p;
		*p = (*p)->right;

		return x;
	}
};


//【ツリープ】
/*
* Treap() : O(1)
*	空のツリープで初期化する．
*
* insert(T key) : O(log n)
*	key を挿入する．
*
* int count(T key) : O(log n)
*	key が存在すれば 1, さもなくば 0 を返す．
*
* erase(T key) : O(log n)
*	key を削除する．
*/
template <class T>
struct Treap {
	// varify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_D

	// ツリープのノード
	struct Node {
		T key;
		unsigned int priority;
		Node* left;
		Node* right;

		// コンストラクタ
		Node() {}
		Node(const T& k, unsigned int p)
			: key(k), priority(p), left(nullptr), right(nullptr) {}
	};

	int n; // ツリープのノード数
	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器

	// コンストラクタ（空のツリープで初期化）
	Treap() : n(0), root(nullptr), rnd((int)time(0)) {}

	// 要素の挿入
	void insert(const T& key) { insert_rf(root, key, rnd()); n++; }
	void insert_rf(Node*& t, const T& key, unsigned int priority) {
		// 葉に到達したら新しいノードを作って繋ぐ．
		if (t == nullptr) {
			t = new Node(key, priority);
			return;
		}

		// 重複した key は無視する．
		if (key == t->key) {
			return;
		}

		// 左の子への移動
		if (key < t->key) {
			insert_rf(t->left, key, priority);
			if (t->priority < priority) {
				rotate_right(t);
			}
		}
		// 右の子への移動
		else {
			insert_rf(t->right, key, priority);
			if (t->priority < priority) {
				rotate_left(t);
			}
		}
	}

	// 木の右回転
	void rotate_right(Node*& t) {
		Node* s = t->left;
		t->left = s->right;
		s->right = t;
		t = s;
	}

	// 木の左回転
	void rotate_left(Node*& t) {
		Node* s = t->right;
		t->right = s->left;
		s->left = t;
		t = s;
	}

	// 要素の探索
	int count(const T& key) {
		Node** p = &root;
		p = move(p, key);
		return *p == nullptr ? 0 : 1;
	}
	Node** move(Node** p, const T& key) {
		// key を二分探索する．
		while (*p != nullptr) {
			if (key == (*p)->key) {
				return p;
			}

			if (key < (*p)->key) {
				p = &(*p)->left;
			}
			else {
				p = &(*p)->right;
			}
		}
		return p;
	}

	// 要素の削除
	void erase(const T& key) {
		// 削除対象の位置まで移動する．
		Node** p = &root;
		p = move(p, key);

		// 削除対象があれば削除する．
		if (*p != nullptr) {
			erase_rf(*p);
			n--;
		}
	}
	void erase_rf(Node*& t) {
		// 葉の場合
		if (t->left == nullptr && t->right == nullptr) {
			// 単に削除して終了．
			delete t;
			t = nullptr;
		}
		// 右の子のみもつ場合
		else if (t->left == nullptr) {
			// 短絡して終了．
			Node* s = t;
			t = t->right;
			delete s;
		}
		// 左の子のみもつ場合
		else if (t->right == nullptr) {
			// 短絡して終了．
			Node* s = t;
			t = t->left;
			delete s;
		}
		// 左右の子をもつ場合
		else {
			// 優先度の高い方が上にくるよう回転し，再帰する．
			if (t->left->priority < t->right->priority) {
				rotate_left(t);
				erase_rf(t->left);
			}
			else {
				rotate_right(t);
				erase_rf(t->right);
			}
		}
	}
};


