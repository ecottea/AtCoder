#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; // -2^63 ～ 2^63 = 9 * 10^18（int は -2^31 ～ 2^31 = 2 * 10^9）
using pii = pair<int, int>;	using pll = pair<ll, ll>;	using pil = pair<int, ll>;	using pli = pair<ll, int>;
using vi = vector<int>;		using vvi = vector<vi>;		using vvvi = vector<vvi>;
using vl = vector<ll>;		using vvl = vector<vl>;		using vvvl = vector<vvl>;
using vb = vector<bool>;	using vvb = vector<vb>;		using vvvb = vector<vvb>;
using vc = vector<char>;	using vvc = vector<vc>;		using vvvc = vector<vvc>;
using vd = vector<double>;	using vvd = vector<vd>;		using vvvd = vector<vvd>;
template <class T> using priority_queue_rev = priority_queue<T, vector<T>, greater<T>>;
using Graph = vvi;

// 定数の定義
const double PI = acos(-1);
const vi DX = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi DY = { 0, 1, 0, -1 };
const int INF = 1001001001; const ll INFL = 4004004004004004004LL;
const double EPS = 1e-12; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), x))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), x))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define YES(b) {cout << ((b) ? "YES\n" : "NO\n");}
#define rep(i, n) for(int i = 0, i##_len = ll(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 演算子オーバーロード
template <class T, class U> inline istream& operator>>(istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T> inline istream& operator>>(istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline vector<T>& operator--(vector<T>& v) { repea(x, v) --x; return v; }
template <class T> inline vector<T>& operator++(vector<T>& v) { repea(x, v) ++x; return v; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#include "local.hpp"
// 提出用（gcc）
#else
inline int popcount(int n) { return __builtin_popcount(n); }
inline int popcount(ll n) { return __builtin_popcountll(n); }
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : -1; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : -1; }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define gcd __gcd
#define dump(...)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


//--------------AtCoder 専用--------------
#include <atcoder/all>
using namespace atcoder;

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
//----------------------------------------


//【コスト付きグラフの辺】
/*
* to : 行き先の頂点番号
* cost : 辺のコスト
*/
struct WEdge {
	int to; // 行き先の頂点番号
	ll cost; // 辺のコスト

	// コストなしグラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const WEdge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}
#endif
};


//【コスト付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using WGraph = vector<vector<WEdge>>;


//【コスト付きグラフの入力】O(|V| + |E|)
/*
* 始点 終点 コストの組からなる入力を受け取り，n 頂点 m 辺のコスト付きグラフを構成する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* undirected : 無向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, WGraph& g, bool undirected = true, bool one_indexed = true) {
	g = WGraph(n);
	rep(i, m) {
		int a, b; ll c;
		cin >> a >> b >> c;

		if (one_indexed) { a--; b--; }

		g[a].push_back({ b, c });
		if (undirected) g[b].push_back({ a, c });
	}
}


//【併合可能遅延ヒープ（全順序モノイド）】
/*
* Lazy_skew_heap<S, op, e, cmp, inf>() : O(1)
*	降順に取り出されるヒープを空で初期化する
*	要素は全順序モノイド (S, op, e, cmp, inf) の元とする．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* int size() : O(1)
*	ヒープの大きさを返す．
*
* S top() : O(1)
*	ヒープ内の最大値を返す．
*
* push(S val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最大値を削除する．
*
* merge(Lazy_skew_heap& hp) : O(log n)
*	ヒープ hp を自身に併合する．
* 
* apply(S val) : O(1)
*	ヒープ内の全要素に val を左から掛ける．
*/
template <class S, S(*op)(S, S), S(*e)(), bool(*cmp)(S, S), S(*inf)()>
struct Lazy_skew_heap {
	// 参考 : https://joisino.hatenablog.com/entry/2017/01/11/230141

	struct Node {
		Node* l, * r;
		T v, lazy;

		Node(T v_) : l(nullptr), r(nullptr), v(v_), lazy(e()) {}

		void eval() {
			if (l != nullptr) 
		}

		friend Node* meld(Node* a, Node* b) {
			if (a == nullptr) return b;
			if (b == nullptr) return a;

			// a >= b となるよう並び替える
			if (cmp(a->v, b->v)) swap(a, b);

			// b の方が小さいので，a の左の子とマージしておけば大小関係は保たれる．
			a->l = meld(a->l, b);

			// このままだと毎回左の子が成長していってまずいので，左右の子を交換する．
			swap(a->l, a->r);

			return a;
		}
	};

	Node* root;
	int n;

	// 空で初期化
	Skew_heap() : root(nullptr), n(0) {}

	// ヒープが空かを返す．
	bool empty() const { return root == nullptr; }

	// ヒープの大きさを返す．
	int size() const { return n; }

	// ヒープに値 val を追加する．
	void push(T val) {
		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最大値を返す．
	T top() const { return root->v; }

	// ヒープ内の最大値を削除する．
	void pop() {
		Node* p = root;
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Lazy_skew_heap& hp) {
		n += hp.n;
		root = meld(root, hp.root);
		hp.root = nullptr;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Lazy_skew_heap& q) {
		q.print_rf(os, q.root);
		return os;
	}
	void print_rf(ostream& os, const Node* pt) const {
		if (pt == nullptr) return;
		os << pt->v << " "; print_rf(os, pt->l);  print_rf(os, pt->r);
	}
#endif
};


//【加算 - 降順 順序モノイド】
using S902 = ll;
S902 op902(S902 a, S902 b) { return a + b; }
S902 e902() { return 0; }
bool cmp902(S902 a, S902 b) { return a >= b; }
S902 inf902() { return -INFL; }
#define Add_descending_omonoid S902, op902, e902, cmp902, inf902


//【有向最小全域木】O(|E| log |V|)
/*
* コスト付き有向グラフ g の r を根とする有向最小全域木のコストを返す（なければ -1）
*/
ll directed_minimum_spanning_tree(const WGraph& g, int r) {
	// 参考 : https://joisino.hatenablog.com/entry/2017/01/11/230141

	int n = sz(g);

	const int ROOT = 2, PATH = 1, FREE = 0;
	vi seen(n, FREE);
	seen[r] = ROOT;

	dsu uf(n);

	// qs[t] : t に入ってくる辺をコスト昇順に記録したキュー
	vector<Skew_heap_rev<pli>> qs(n);
	rep(s, n) repe(e, g[s]) qs[e.to].push({ e.cost, s });

	ll res = 0;

	function<bool(int)> dfs = [&](int t) {
		// 根 r と繋がったら終了．
		if (seen[t] == ROOT) return true;
		seen[t] = PATH;

		// 自己ループを無視する．
		int lt = uf.leader(t);
		while (!qs[lt].empty() && uf.same(qs[lt].top().second, lt)) qs[lt].pop();
		
		// t に入ってくる辺が無ければ失敗．
		if (qs[t].empty()) return false;

		// t に入ってくるコスト c が最小の辺 s→t を得る．
		ll c; int s;
		tie(c, s) = qs[t].top();

		// s→t を暫定的に使う辺に追加する．
		res += c;

		// s→t を含めてもサイクルが出来なかったなら s の処理へ．
		if (seen[s] != PATH) return dfs(s);
	};

	rep(t, n) {
		if (seen[t]) continue;

		dfs(t);
	}
}

int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, m, r;
	cin >> n >> m >> r;

	WGraph g;
	read_graph(n, m, g, false, false);


}
