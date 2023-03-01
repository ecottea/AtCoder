#pragma once
#include "header.h"
// ■■■■■ 永続データ構造 ■■■■■


//【部分永続 Union-Find】
/*
* 各時刻での履歴を残しながら頂点の統合と連結判定を行う．
* 時刻とは，それまでに呼ばれた merge() の回数を意味する．
*
* Partially_persistent_union_find(int n) : O(n)
*	非連結で大きさ n の部分永続 Union-Find を構築する．
*
* merge(int a, int b) : O(log n)
*	頂点 a と頂点 b を統合する．
*
* bool same(int a, int b, int t) : O(log n)
*	時刻 t に頂点 a と頂点 b が同じ連結成分に属していたかを返す．
*
* int leader(int a, int t) : O(log n)
*	時刻 t に頂点 a が属していた連結成分の親を返す．
*
* int size(int a, int t) : O(log n)
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

	// 非連結で大きさ n の部分永続 Union-Find を構築する．
	Partially_persistent_union_find(int n_)
		: n(n_), now(0), parent(n), rank(n, 1), time(n, INF), num(n)
	{
		rep(i, n) {
			parent[i] = i;
			num[i].push_back({ 0, 1 });
		}
	}
	Partially_persistent_union_find() : n(0), now(0) {} // ダミー

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


//【永続セグメント木（モノイド）】
/*
* Persistent_segtree<S, op, e>(int n) : O(n)
*	v[0..n) = e() で初期化する．履歴番号は 0 とする．
*	要素はモノイド (S, op, e) の元とする．
*
* Persistent_segtree<S, op, e>(vS v) : O(n)
*	配列 v[0..n) の要素で初期化する．履歴番号は 0 とする．
*
* int set(int i, S x, int t) : O(log n)
*	t 番目の履歴に対し v[i] = x とした配列を最新の履歴として記録し，履歴番号を返す．
*
* S get(int i, int t) : O(log n)
*	t 番目の履歴の v[i] を返す．
*
* S prod(int l, int r, int t) : O(log n)
*	t 番目の履歴の Πv[l..r) を返す．
*
* S all_prod(int t) : O(1)
*	t 番目の履歴の Πv[0..n) を返す．
*
* int max_right(int l, function<bool(S)> f, int t) : O(log n)
*	t 番目の履歴について，f(Πv[l..r)) = true となる最大の r を返す．
*	制約：f(e()) = true，f は単調
*
* int min_left(int r, function<bool(S)> f, int t) : O(log n)
*	t 番目の履歴について，f(Πv[l..r)) = true となる最小の l を返す．
*	制約：f(e()) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*e)()>
class Persistent_segtree {
	struct Node {
		int l, r;
		S val; // Πv[l..r) の値
		Node* lp, * rp; // 左右の子へのポインタ

		Node(int l_, int r_, S val_ = e(), Node* lp_ = nullptr, Node* rp_ = nullptr)
			: l(l_), r(r_), val(val_), lp(lp_), rp(rp_) {}
	};

	int n; // 配列の大きさ
	int T; // 履歴の個数
	vector<Node*> his; // 履歴へのポインタ

	Node* init_rf(const vector<S>& v, int l, int r) {
		// 葉を作る場合
		if (r - l == 1) {
			Node* p = new Node(l, r, v[l]);
			return p;
		}

		Node* p = new Node(l, r);
		int m = (l + r) / 2;
		p->lp = init_rf(v, l, m);
		p->rp = init_rf(v, m, r);
		p->val = op(p->lp->val, p->rp->val);

		return p;
	}

	Node* set_rf(Node* p, int i, S x) {
		// p が葉の場合
		if (p->r - p->l == 1) {
			Node* np = new Node(p->l, p->r, x);
			return np;
		}

		Node* np = new Node(p->l, p->r);
		int m = (p->l + p->r) / 2;
		if (i < m) {
			np->lp = set_rf(p->lp, i, x);
			np->rp = p->rp;
		}
		else {
			np->lp = p->lp;
			np->rp = set_rf(p->rp, i, x);
		}
		np->val = op(np->lp->val, np->rp->val);

		return np;
	}

	S get_rf(Node* p, int i) const {
		// p が葉の場合
		if (p->r - p->l == 1) return p->val;

		int m = (p->l + p->r) / 2;
		if (i < m) return get_rf(p->lp, i);
		else  return get_rf(p->rp, i);
	}

	S prod_rf(Node* p, int l, int r) const {
		// 範囲外なら単位元 e() を返す．
		if (p->r <= l || r <= p->l) return e();

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= p->l && p->r <= r) return p->val;

		// 一部の範囲のみを含むなら子を見に行く．
		S vl = prod_rf(p->lp, l, r);
		S vr = prod_rf(p->rp, l, r);
		return op(vl, vr);
	}

	int max_right_rf(Node* p, int l, S& x, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (p->r <= l) return n;

		// f( Πv[p->l..p->r) ) = true の場合
		if (f(op(x, p->val))) {
			x = op(x, p->val);
			return n;
		}

		// p が葉の場合，これがちょうど条件を満たさなくなる値なのでその位置を返す．
		if (p->r - p->l == 1) return p->l;

		// 左の部分木から見に行く．境界が見つかったらそれを返す．
		int pos = max_right_rf(p->lp, l, x, f);
		if (pos != n) return pos;

		// 見つからなかったら右の部分木も見に行き，結果を返す．
		return max_right_rf(p->rp, l, x, f);
	}

	int min_left_rf(Node* p, int r, S& x, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (r <= p->l) return -1;

		// f( Πv[p->l..p->r) ) = true の場合
		if (f(op(p->val, x))) {
			x = op(p->val, x);
			return -1;
		}

		// p が葉の場合，これがちょうど条件を満たさなくなる値なのでその位置を返す．
		if (p->r - p->l == 1) return p->l;

		// 右の部分木から見に行く．境界が見つかったらそれを返す．
		int pos = min_left_rf(p->rp, r, x, f);
		if (pos != -1) return pos;

		// 見つからなかったら左の部分木も見に行き，結果を返す．
		return min_left_rf(p->lp, r, x, f);
	}

	void print_rf(Node* p, ostream& os) const {
		if (p->r - p->l == 1) {
			os << p->val << " ";
			return;
		}

		print_rf(p->lp, os);
		print_rf(p->rp, os);
	}

public:
	// 配列 v[0..n) の要素で初期化する．
	Persistent_segtree(const vector<S>& v) : n(sz(v)), T(1), his(1) {
		his[0] = init_rf(v, 0, n);
	}

	// v[0..n) = e() で初期化する．
	Persistent_segtree(int n_) : n(n_), T(1), his(1) {
		// verify : https://atcoder.jp/contests/abc165/tasks/abc165_f

		vector<S> v(n, e());
		his[0] = init_rf(v, 0, n);
	}

	Persistent_segtree() : n(0), T(0) {} // ダミー

	// t 番目の履歴に対し v[i] = x とした配列を最新の履歴として記録し，履歴番号を返す．
	int set(int i, S x, int t) {
		// verify : https://atcoder.jp/contests/abc165/tasks/abc165_f

		Assert(0 <= i && i < n);
		Assert(t < T);
		his.push_back(set_rf(his[t], i, x));
		return T++;
	}

	// t 番目の履歴の v[i] を返す．
	S get(int i, int t) const {
		Assert(0 <= i && i < n);
		Assert(t < T);
		return get_rf(his[t], i);
	}

	// t 番目の履歴の Πv[l..r) を返す．
	S prod(int l, int r, int t) const {
		// verify : https://atcoder.jp/contests/abc165/tasks/abc165_f

		Assert(0 <= l && r <= n);
		Assert(t < T);
		if (l >= r) return e();
		return prod_rf(his[t], l, r);
	}

	// t 番目の履歴の Πv[0..n) を返す．
	S all_prod(int t) const {
		// verify : https://atcoder.jp/contests/abc165/tasks/abc165_f

		Assert(t < T);
		return prod(0, n, t);
	}

	// t 番目の履歴について，f(Πv[l..r)) = true となる最大の r を返す．
	int max_right(int l, const function<bool(S)>& f, int t) const {
		// verify : https://atcoder.jp/contests/practice2/tasks/practice2_j

		S x(e());
		return max_right_rf(his[t], l, x, f);
	}

	// t 番目の履歴について，f(Πv[l..r)) = true となる最小の l を返す．
	int min_left(int r, const function<bool(S)>& f, int t) const {
		S x(e());
		return min_left_rf(his[t], r, x, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Persistent_segtree& seg) {
		rep(t, seg.T) {
			os << t << ": ";
			seg.print_rf(seg.his[t], os);
			os << endl;
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
		vector<Node*> cp; // 子へのポインタ

		Node(int l_, int r_, S val_ = 0) : l(l_), r(r_), val(val_) {}
	};

	int n; // 配列の大きさ
	int T; // 履歴の個数
	vector<Node*> his; // 履歴へのポインタ

	Node* init_rf(const vector<S>& v, int l, int r) {
		if (r - l <= 0) return nullptr;

		// 葉を作る場合
		if (r - l == 1) {
			Node* p = new Node(l, r, v[l]);
			return p;
		}

		Node* p = new Node(l, r);
		p->cp = vector<Node*>(M);
		int w = r - l;
		rep(k, M) p->cp[k] = init_rf(v, l + w * k / M, l + w * (k + 1) / M);

		return p;
	}

	Node* set_rf(Node* p, int i, S x) {
		// p が葉の場合
		if (p->cp.empty()) {
			Node* np = new Node(p->l, p->r, x);
			return np;
		}

		Node* np = new Node(p->l, p->r);
		np->cp = p->cp;
		int w = p->r - p->l;
		int k = ((i - p->l + 1) * M - 1) / w;
		np->cp[k] = set_rf(p->cp[k], i, x);

		return np;
	}

	S get_rf(Node* p, int i) const {
		// p が葉の場合
		if (p->cp.empty()) return p->val;

		int w = p->r - p->l;
		int k = ((i - p->l + 1) * M - 1) / w;
		return get_rf(p->cp[k], i);
	}

	void print_rf(Node* p, ostream& os) const {
		if (p == nullptr) return;

		if (p->cp.empty()) {
			os << p->val << " ";
			return;
		}

		rep(k, M) print_rf(p->cp[k], os);
	}

public:
	// 配列 v[0..n) の要素で初期化する．
	Persistent_array(const vector<S>& v) : n(sz(v)), T(1), his(1) {
		his[0] = init_rf(v, 0, n);
	}

	// v[0..n) = 0 で初期化する．
	Persistent_array(int n_) : n(n_), T(1), his(1) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		vector<S> v(n, 0);
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


//【永続 Union-Find】
/*
* Persistent_union_find(int n) : O(n)
*	非連結で大きさ n の Union-Find を構築する．履歴番号は 0 とする．
*
* int merge(int a, int b, int t) : O(log n)
*	t 番目の履歴の頂点 a, b を結合して最新の履歴として記録し，履歴番号を返す．
*	a, b が元々連結だった場合は何もしないが，履歴としては記録する．
*
* bool same(int a, int b, int t) : O(log n)
*	t 番目の履歴の頂点 a と頂点 b が同じ連結成分に属するかを返す．
*
* int leader(int a, int t) : O(log n)
*	t 番目の履歴の頂点 a の属する連結成分の親を返す．
*
* int size(int a, int t) : O(log n)
*	t 番目の履歴の頂点 a の属する連結成分の大きさを返す．
*
* int size(int t) : O(1)
*	t 番目の履歴の連結成分の個数を返す．
*
* vvi groups(int t) : O(n log n)
*	t 番目の履歴の連結成分のリストを返す．
* 
* 利用：【永続配列】
*/
struct Persistent_union_find {
	// 参考 : https://qiita.com/hotman78/items/9c643feae1de087e6fc5

	int n; // 頂点の個数
	int T; // 履歴の個数
	vi ms; // 連結成分の個数
	vi times; // セグ木の何番目の履歴と対応するか

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	Persistent_array<int> parent_or_size;

	// 非連結で大きさ n の Union-Find を構築する．履歴番号は 0 とする．
	Persistent_union_find(int n_) : n(n_), T(1), ms(1), times(1) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		vi minus1(n, -1);
		parent_or_size = Persistent_array<int>(minus1);
		ms[0] = n;
		times[0] = 0;
	}

	Persistent_union_find() : n(0), T(0) {} // ダミー

	// t 番目の履歴の頂点 a, b を結合して最新の履歴として記録し，履歴番号を返す．
	int merge(int a, int b, int t) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		Assert(0 <= t && t < T);

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a, t);
		int rb = leader(b, t);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) {
			ms.push_back(ms[t]);
			times.push_back(times[t]);
			return T++;
		}

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		int sa = -parent_or_size.get(ra, times[t]);
		int sb = -parent_or_size.get(rb, times[t]);
		if (sa < sb) swap(ra, rb);

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		int nt2 = parent_or_size.set(ra, -(sa + sb), times[t]);
		nt2 = parent_or_size.set(rb, ra, nt2);
		times.push_back(nt2);

		// 連結成分の数を 1 つ減らす．
		ms.push_back(ms[t] - 1);

		return T++;
	}

	// t 番目の履歴の頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b, int t) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		Assert(0 <= t && t < T);

		// 根が同じなら連結である．
		return leader(a, t) == leader(b, t);
	}

	// t 番目の履歴の頂点 a の属する連結成分の根を返す．
	int leader(int a, int t) {
		// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

		Assert(0 <= t && t < T);

		// a が根であれば自分自身を返す．
		int pa = parent_or_size.get(a, times[t]);
		if (pa < 0) return a;

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = leader(pa, t);

		return ra;
	}

	// t 番目の履歴の頂点 a の属する連結成分の大きさを返す．
	int size(int a, int t) {
		Assert(0 <= t && t < T);

		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size.get(leader(a, t), times[t]);
	}

	// t 番目の履歴の連結成分の個数を返す．
	int size(int t) {
		Assert(0 <= t && t < T);

		return ms[t];
	}

	// t 番目の履歴の連結成分のリストを返す．
	vvi groups(int t) {
		Assert(0 <= t && t < T);

		vvi res(ms[t]); vi r_to_i(n, -1); int i = 0;
		rep(a, n) {
			int r = leader(a, t);
			if (r_to_i[r] == -1) r_to_i[r] = i++;
			res[r_to_i[r]].push_back(a);
		}

		return res;
	}
};


