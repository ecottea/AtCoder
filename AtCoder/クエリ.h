#pragma once
#include "header.h"
// ■■■■■ クエリ処理 ■■■■■



//【法を m とした和クエリ】
/*
* Mod_sum_query(a) : O(n)
*	配列 a で初期化する．
*
* sum(m) : O(max(a) log(n) / m)
*	a[0..n) mod m の和を返す．
*/
struct Mod_sum_query {
	vi a;    // ★ a でなくバケツで累積和を持てば O(log n) を落とせる．
	int n;
	ll asum; // a[0..n) の和


	// コンストラクタ（何もしない）
	Mod_sum_query() : n(0), asum(0) {}

	// 配列 a で初期化
	Mod_sum_query(const vi& a_) : a(a_), n(sz(a)), asum(0LL) {
		sort(all(a));
		rep(i, n) asum += a[i];
	}

	// a[0..n) mod m の和を返す．
	ll sum_mod(int m) {
		ll res = asum;

		for (ll v = m; v <= a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			res -= (ll)m * distance(lower_bound(all(a), v), a.end());
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll sum_lack(int m) {
		// sum : 1-indexed での a[0..n) mod m の和
		ll sum = asum;

		for (ll v = m; v < a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			sum -= (ll)m * distance(lower_bound(all(a), v + 1), a.end());
		}

		// 不足分を返す．
		return (ll)m * n - sum;
	}
};


//【Convex-Hull Trick（挿入単調／クエリ単調）】
/*
* insert(l) : n 回で O(n)
*	l = {a, b} が表す直線 y = a x + b を追加する．
*	呼び出す際の挿入する直線の傾き a は降順でなくてはならない．
*
* min(x) : q 回で O(q)
*	a x + b の最小値を返す．
*	呼び出す際の x 座標は昇順でなくてはならない．
*/
struct Convex_hull_trick {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331

	int n; // 記録している直線の本数
	deque<pll> lines; // 直線を傾き狭義降順に記録したデック

	Convex_hull_trick() : n(0) {}

	void insert(pll l) {
		ll a3, b3;
		tie(a3, b3) = l;

		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[n - 2];
			tie(a2, b2) = lines[n - 1];

			// y = a2 x + b2 が必要な直線なら消去をここまでにする．
			// 積をとってしまっているのでオーバーフローに注意．
			if ((a1 - a2) * (b3 - b2) > (a2 - a3) * (b2 - b1)) {
				break;
			}
			lines.pop_back();
			n--;
		}
		lines.push_back(l);
		n++;
	}

	ll min(ll x) {
		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[0];
			tie(a2, b2) = lines[1];

			if (a1 * x + b1 < a2 * x + b2) {
				break;
			}
			lines.pop_front();
			n--;
		}

		ll a, b;
		tie(a, b) = lines[0];
		return a * x + b;
	}
};


//【kd 木】
/*
* KDTree() : O(1)
*	空で初期化する．
*
* KDTree(ps) : O(n log n) // TODO 遅いので作り直す
*	点群 ps で初期化する．
*
* insert(p) : O(log n) // TODO 点の分布によっては木の形が悪くなるので作り直す
*	点 p を挿入する．
*
* search(p0, p1, res) : O(log n + cnt)
*	半開長方形 R = [p0, p1) 内の点を res に格納する．
*	p0 = {x0, y0}, p1 = {x1, y1} としたとき，R = [x0, x1) × [y0, y1) である．
*/
template <class T, class S>
struct KDTree {
	// 参考 : http://www.prefield.com/algorithm/geometry/kdtree.html

	using Pnt = pair<T, T>;

	// kd-木のノード
	struct Node {
		Pnt p;
		S val;
		Node* left, * right;

		// コンストラクタ
		Node() {}
		Node(const Pnt& p_, const S& val_) : p(p_), val(val_),
			left(nullptr), right(nullptr) {}
	};

	int n; // 要素数
	Node* root; // 根へのポインタ


	// 空で初期化
	KDTree() : n(0), root(nullptr) {}

	// 配列で初期化
	KDTree(vector<pair<Pnt, S>>& a) : n(sz(a)), root(nullptr) {
		mt19937 rnd((int)time(0));
		shuffle(all(a), rnd);
		rep(i, n) {
			insert_rf(root, true, a[i].first, a[i].second);
		}
	}

	// 点の比較（x_axis = true なら x 座標で，false なら y 座標で比較する）
	bool less(bool x_axis, const Pnt& p, const Pnt& q) {
		return x_axis ? p.first < q.first : p.second < q.second;
	}

	// 点の挿入
	void insert(const Pnt& p, const S& val) {
		insert_rf(root, true, p, val);
		n++;
	}

	// t : 挿入位置，dim : 比較に使う次元，p : 挿入する点
	void insert_rf(Node*& t, bool x_axis, const Pnt& p, const S& val) {
		// 葉に辿り着いたら新しいノードを挿入する．
		if (t == nullptr) {
			t = new Node(p, val);
			return;
		}

		// p の座標がいまのノードの座標より小さいか大きいかで場合分けし挿入位置を探る．
		if (less(x_axis, p, t->p)) {
			insert_rf(t->left, !x_axis, p, val);
		}
		else {
			insert_rf(t->right, !x_axis, p, val);
		}
	}

	// 点の探索
	void search(const Pnt& p0, const Pnt& p1, vector<Node*>& res) {
		res.clear();
		search_rf(root, true, p0, p1, res);
	}

	void search_rf(Node* t, bool x_axis, const Pnt& p0, const Pnt& p1, vector<Node*>& res) {
		// 葉に辿り着いたらすぐに帰る．
		if (t == nullptr) {
			return;
		}

		// 領域内なら点を記録する．
		if (p0.first <= t->p.first && t->p.first < p1.first
			&& p0.second <= t->p.second && t->p.second < p1.second) {
			res.push_back(t);
		}

		// t->p < p0 でない限り左の子を調べにいく．
		if (!less(x_axis, t->p, p0)) {
			search_rf(t->left, !x_axis, p0, p1, res);
		}

		// p1 <= t->p でない限り右の子を調べにいく．
		if (less(x_axis, t->p, p1)) {
			search_rf(t->right, !x_axis, p0, p1, res);
		}
	}
};


//【kd トライ】
/*
* KDTrie(ps) : O(n log n)
*	点と値の組の集合 ps[i] = {{x[i], y[i]}, val[i]} で初期化する．
*
* sum(p1, p2) : O(log n)
*	半開長方形 R = [p1, p2) 内の点の値の和を返す．
*	p1 = {x1, y1}, p2 = {x2, y2} としたとき，R = [x1, x2) × [y1, y2) である．
*/
struct KDTrie {
	struct Node {
		vl p1, p2; // 半開長方形 R = [p1, p2) に対応するノード
		ll val;
		Node* left, * right;

		// コンストラクタ
		Node() {}
		Node(vl p1_, vl p2_, ll val_) : p1(p1_), p2(p2_), val(val_),
			left(nullptr), right(nullptr) {}
	};

	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器

	// 空で初期化
	KDTrie() : root(nullptr) {}

	// 点と値の集合で初期化
	KDTrie(vector<pair<vl, ll>>& a) : root(nullptr), rnd((int)time(0)) {
		split(root, { -INFL, -INFL }, { INFL, INFL }, a, 0, sz(a), 0);
	}

	ll at(vector<pair<vl, ll>>& a, int i, int d) {
		return a[i].first[d];
	}

	// ノード *t に a[i0..i1) を割り当て分割する．
	ll split(Node*& t, vl p1, vl p2, vector<pair<vl, ll>>& a, int i0, int i1, int d) {
		// 空なら何もしない．
		if (i0 == i1) {
			return 0;
		}

		// 要素が一つだけなら葉として格納して帰る．
		if (i0 + 1 == i1) {
			t = new Node(a[i0].first, { a[i0].first[0] + 1, a[i0].first[1] + 1 },
				a[i0].second);
			return a[i0].second;
		}

		// 要素をランダムに 2 k + 1 個選択し，それらの座標の中央値 median を得る．
		const int k = 10;
		vl cands(2 * k + 1);
		rep(i, 2 * k + 1) { cands[i] = at(a, rnd() % (i1 - i0) + i0, d); }
		sort(all(cands));
		ll median = cands[k];

		// median を閾値として用い，それ未満のものを左，以上のものを右に移動する．
		int i = i0; // i : a[i0, i) が median 未満の要素と確定
		repi(j, i0, i1 - 1) { // j : a[i, j) が median 以上の要素と確定
			// j の位置に median 未満の要素がある場合
			if (at(a, j, d) < median) {
				// 最も左の x 以上の要素と交換する．
				swap(a[i], a[j]);
				i++;
			}
		}

		// median で分割して左右の子を作りに行く．
		t = new Node(p1, p2, 0);
		vl pm(2);
		pm[d] = median;
		pm[1 - d] = p2[1 - d];
		t->val += split(t->left, p1, pm, a, i0, i, 1 - d);
		pm[1 - d] = p1[1 - d];
		t->val += split(t->right, pm, p2, a, i, i1, 1 - d);

		return t->val;
	}

	// 半開長方形 R = [p1, p2) 内の点の値の和を返す．
	ll sum(const vl& p1, const vl& p2) {
		return sum_rf(root, p1, p2, 0);
	}

	ll sum_rf(Node* t, const vl& p1, const vl& p2, int d) {
		// 木が空の場合
		if (t == nullptr) return 0;

		// 一部も範囲に入っていない場合
		if (p2[d] <= t->p1[d] || t->p2[d] <= p1[d]) return 0;

		// 完全に範囲に入っている場合
		if (p1[0] <= t->p1[0] && t->p2[0] <= p2[0]
			&& p1[1] <= t->p1[1] && t->p2[1] <= p2[1]) {
			return t->val;
		}

		ll val = 0;
		val += sum_rf(t->left, p1, p2, 1 - d);
		val += sum_rf(t->right, p1, p2, 1 - d);

		return val;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const KDTrie& kd) {
		kd.print_rf(os, kd.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "val:" << t->val << ", p1:" << t->p1 << ", p2:" << t->p2 << ", left:";
		t->left != nullptr ? os << "(" << t->left->p1 << "," << t->left->p2 << ")" : os << "-";
		os << ", right:";
		t->right != nullptr ? os << "(" << t->right->p1 << "," << t->right->p2 << ")" : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
};


