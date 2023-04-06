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
int INF = 1001001001; ll INFL = 4004004004004004004LL;
double EPS = 1e-15;

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(18); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), x))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), x))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
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
template <class T> inline T get(T set, int i) { return (set >> i) & T(1); }

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
#define dump_list(v)
#define dump_list2D(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif

#endif // 折りたたみ用


#if __has_include(<atcoder/all>)
#include <atcoder/all>
using namespace atcoder;

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
#endif


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

		vi ini(n, -1);
		parent_or_size = Persistent_array<int>(ini);
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


int main() {
	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, q;
	cin >> n >> q;

	Persistent_union_find uf(n);

	vi i_to_t(q + 1);

	rep(i, q) {
		int type, k, u, v;
		cin >> type >> k >> u >> v;

		int t = i_to_t[k + 1];

		if (type == 0) {
			t = uf.merge(u, v, t);
			i_to_t[i + 1] = t;
		}
		else {
			cout << uf.same(u, v, t) << endl;
		}
	}
}
