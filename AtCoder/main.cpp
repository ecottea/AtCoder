#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; using ull = unsigned long long; // -2^63 ～ 2^63 = 9 * 10^18（int は -2^31 ～ 2^31 = 2 * 10^9）
using pii = pair<int, int>;	using pll = pair<ll, ll>;	using pil = pair<int, ll>;	using pli = pair<ll, int>;
using vi = vector<int>;		using vvi = vector<vi>;		using vvvi = vector<vvi>;	using vvvvi = vector<vvvi>;
using vl = vector<ll>;		using vvl = vector<vl>;		using vvvl = vector<vvl>;	using vvvvl = vector<vvvl>;
using vb = vector<bool>;	using vvb = vector<vb>;		using vvvb = vector<vvb>;
using vc = vector<char>;	using vvc = vector<vc>;		using vvvc = vector<vvc>;
using vd = vector<double>;	using vvd = vector<vd>;		using vvvd = vector<vvd>;
template <class T> using priority_queue_rev = priority_queue<T, vector<T>, greater<T>>;
using Graph = vvi;

// 定数の定義
const double PI = acos(-1);
const vi DX = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi DY = { 0, 1, 0, -1 };
int INF = 1001001001; ll INFL = 4004004003104004004LL; // (int)INFL = 1010931620;

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(18); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), x))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), x))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define YES(b) {cout << ((b) ? "YES\n" : "NO\n");}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0, set##_ub = 1 << int(d); set < set##_ub; ++set) // d ビット全探索（昇順）
#define repis(i, set) for(int i = lsb(set), bset##i = set; i >= 0; bset##i -= 1 << i, i = lsb(bset##i)) // set の全要素（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了
#define inQ(x, y, u, l, d, r) ((u) <= (x) && (l) <= (y) && (x) < (d) && (y) < (r)) // 矩形内判定

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

#endif // 折りたたみ用


#if __has_include(<atcoder/all>)
#include <atcoder/all>
using namespace atcoder;

#ifdef _MSC_VER
#include "localACL.hpp"
#endif

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

namespace atcoder {
	inline istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
	inline ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
}
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>; using vvvvm = vector<vvvm>;
#endif


#ifdef _MSC_VER // 手元環境（Visual Studio）
#include "local.hpp"
#else // 提出用（gcc）
inline int popcount(int n) { return __builtin_popcount(n); }
inline int popcount(ll n) { return __builtin_popcountll(n); }
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : -1; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : -1; }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define dump(...)
#define dumpel(v)
#define dump_list(v)
#define dump_mat(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif


//【rollback Union-Find】
/*
* Rollback_Union_find(int n) : O(n)
*	非連結で大きさ n の Union-Find を構築する．
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
* vvi groups() : O(n log n)
*	連結成分のリストを返す．
*
* snapshot() : O(1)
*	スナップショットを作成する．
*
* rollback() : O(1)
*	直前に作成したスナップショットの状態まで巻き戻し，スナップショットを破棄する．
*/
class Rollback_Union_find {
	// 参考 : https://snuke.hatenablog.com/entry/2016/07/01/000000

	int n; // 頂点の個数
	int m; // 連結成分の個数

	// parent_or_size[i] : 頂点 i の親または属する集合の大きさ
	//	頂点 i が根でない場合は親の番号（非負）を，
	//	根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// 変更履歴
	stack<pii> history;

public:
	// 非連結で大きさ n の Union-Find を構築する．
	Rollback_Union_find(int n) : n(n), m(n), parent_or_size(n, -1) {
		// verify : https://codeforces.com/gym/100513/problem/A
	}
	Rollback_Union_find() : n(0), m(0) {} // ダミー

	// 頂点 a, b を結合する．
	void merge(int a, int b) {
		// verify : https://codeforces.com/gym/100513/problem/A

		// 頂点 a, b の属する連結成分の根 ra, rb を得る．
		int ra = leader(a);
		int rb = leader(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) return;

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) swap(ra, rb);

		// 変更前の情報を記録しておく．
		history.emplace(ra, parent_or_size[ra]);
		history.emplace(rb, parent_or_size[rb]);

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;

		// 連結成分の数を 1 つ減らす．
		m--;
	}

	// スナップショットを作成する．
	void snapshot() {
		// verify : https://atcoder.jp/contests/abc302/tasks/abc302_h

		history.emplace(INF, m);
	}

	// 直前に作成したスナップショットの状態まで巻き戻す．
	void rollback() {
		// verify : https://atcoder.jp/contests/abc302/tasks/abc302_h

		while (true) {
			auto [i, v] = history.top(); history.pop();
			if (i == INF) {
				m = v;
				break;
			}
			parent_or_size[i] = v;
		}
	}

	// 頂点 a, b が同じ連結成分に属するかを返す．
	bool same(int a, int b) {
		// verify : https://codeforces.com/gym/100513/problem/A

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

		// 経路圧縮はしない．

		return ra;
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
	friend ostream& operator<<(ostream& os, Rollback_Union_find d) {
		repe(g, d.groups()) {
			repe(v, g) os << v << " ";
			os << endl;
		}
		return os;
	}
#endif
};


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");
	
	int h, w;
	cin >> h >> w;

	vvc c(h, vc(w));
	cin >> c;

	int cr = 0;

	rep(i, h) rep(j, w) if (c[i][j] == '.') cr++;

	Rollback_Union_find d(h * w);

	rep(i, h) rep(j, w) {
		if (c[i][j] == '.') continue;

		rep(k, 4) {
			int ni = i + DX[k];
			int nj = j + DY[k];
			if (inQ(ni, nj, 0, 0, h, w) && c[ni][nj] == '#') {
				d.merge(i * w + j, ni * w + nj);
			}
		}
	}

	mint res = 0;

	rep(i, h) rep(j, w) {
		if (c[i][j] == '#') continue;

		d.snapshot();

		rep(k, 4) {
			int ni = i + DX[k];
			int nj = j + DY[k];
			if (inQ(ni, nj, 0, 0, h, w) && c[ni][nj] == '#') {
				d.merge(i * w + j, ni * w + nj);
			}
		}

		res += sz(d) - (cr - 1);

		d.rollback();
	}
	res /= cr;

	cout << res << endl;
}
