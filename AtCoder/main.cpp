#ifndef HIDDEN_IN_VISUAL_STUDIO // 折りたたみ用

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
const double PI = 3.14159265359;
const double DEG = PI / 180.; // θ [deg] = θ * DEG [rad]
const vi dx4 = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi dy4 = { 0, 1, 0, -1 };
const vi dx8 = { 1, 1, 0, -1, -1, -1, 0, 1 }; // 8 近傍
const vi dy8 = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int INF = 1001001001; const ll INFL = 2002002002002002002LL;
const double EPS = 1e-10; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define distance (int)distance
#define Yes(b) {cout << ((b) ? "Yes" : "No") << endl;}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define repit(it, a) for(auto it = (a).begin(); it != (a).end(); ++it) // イテレータを回す（昇順）
#define repitr(it, a) for(auto it = (a).rbegin(); it != (a).rend(); ++it) // イテレータを回す（降順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 演算子オーバーロード
template <class T, class U> inline istream& operator>> (istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T, class U> inline ostream& operator<< (ostream& os, const pair<T, U>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template <class T, class U, class V> inline istream& operator>> (istream& is, tuple<T, U, V>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t); return is; }
template <class T, class U, class V> inline ostream& operator<< (ostream& os, const tuple<T, U, V>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")"; return os; }
template <class T, class U, class V, class W> inline istream& operator>> (istream& is, tuple<T, U, V, W>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t); return is; }
template <class T, class U, class V, class W> inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")"; return os; }
template <class T> inline istream& operator>> (istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline ostream& operator<< (ostream& os, const vector<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const list<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T, greater<T>>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue_rev<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline vector<T>& operator--(vector<T>& v) { rep(_i_, sz(v)) --v[_i_]; return v; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#define popcount (int)__popcnt // 全ビット中の 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cout << "\033[1;36m" << (x) << "\033[0m" << endl;
#define dumps(x) cout << "\033[1;36m" << (x) << "\033[0m ";
#define dumpel(a) { int _i_ = -1; cout << "\033[1;36m"; repe(x, a) {cout << ++_i_ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream isTMP(f); cin.rdbuf(isTMP.rdbuf());
#define output_to_file(f) ofstream osTMP(f); cout.rdbuf(osTMP.rdbuf());
// 提出用（gcc）
#else
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(x)
#define dumps(x)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


////-----------------AtCoder 専用-----------------
//#include <atcoder/all>
//using namespace atcoder;
//
//using mint = modint1000000007;
////using mint = modint998244353;
////using mint = modint; // mint::set_mod(m);
//
//template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
//template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
//istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
//ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
//using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
////----------------------------------------------


//【コスト付きグラフの入力（コストは別）】O(|V| + |E|)
/*
* 始点 終点 コストの組からなる入力を受け取り，n 頂点 m 辺のグラフを構成する．
* 辺へのコストの割り当ては別で記録する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* c : 辺 (s, t) のコストを c[s][t] に格納する
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, Graph& g, vector<unordered_map<int, ll>>& c,
	bool directed = false, bool one_indexed = true) {
	g = Graph(n);
	c = vector<unordered_map<int, ll>>(n);
	rep(i, m) {
		int a, b; ll x;
		cin >> a >> b >> x;

		if (one_indexed) { a--; b--; }

		g[a].push_back(b);
		c[a][b] = x;
		if (!directed) {
			g[b].push_back(a);
			c[b][a] = x;
		}
	}
}


//【クリークの列挙】O(2^(1.4√|E|) |V|)
/*
* 無向グラフ g の i 番目に見つけたクリークを cs[i] に頂点の列として列挙する．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*/
void enumerate_clique(const Graph& g, vvi& cs) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2306

	int n = sz(g);
	cs.clear();

	// 隣接行列 adj，各頂点の次数 deg，総次数 deg_sum，
	// 最小次数 deg_min，次数最小頂点の番号 i_min を得る．
	vvb adj(n, vb(n));
	vi deg(n);
	int deg_sum = 0, deg_min = INF, i_min = -1;
	rep(s, n) {
		deg[s] = sz(g[s]);
		deg_sum += deg[s];
		repe(t, g[s]) adj[s][t] = true;

		if (chmin(deg_min, deg[s])) i_min = s;
	}

	// 考慮すべき頂点のリスト
	vi v(n);
	iota(all(v), 0);

	// 素朴な方法で最大クリークを求める O(2^n n)
	function<void()> naive = [&]() {
		// 全ての部分集合 set について
		repb(set, n) {
			bool sum = 0;

			// n 頂点それぞれについて
			rep(i, n) {
				// set に選んでいないなら無関係
				if (!(set & (1 << i))) {
					continue;
				}

				// i 番目以降の頂点について
				repi(j, i + 1, n - 1) {
					// set に選んでいないなら無関係 
					if (!(set & (1 << j))) {
						continue;
					}

					// 辺 (v[i], v[j]) がなければクリークでない．
					if (!adj[v[i]][v[j]]) {
						goto NEXT_LOOP;
					}
				}
			}

			// クリークが見つかったので記録する．
			cs.push_back(vi());
			rep(i, n) {
				if (set & (1 << i)) {
					cs.rbegin()->push_back(v[i]);
				}
			}

		NEXT_LOOP:;
		}
	};

	int res = 1;
	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min >= deg_sum) {
			naive();
			return;
		}

		// 次数最小の頂点 v[i_min] の隣接点の番号の集合を得る．
		// 同時に v[i_min] に出入りする辺を削除したことにし，各頂点の次数 deg を更新する．
		vi ia;
		rep(i, n) {
			if (adj[v[i_min]][v[i]]) {
				ia.push_back(i);

				deg[v[i]]--;
			}
		}
		int d = sz(ia);

		// まず v[i_min] を含む最大クリークの大きさ res を求める．
		// v[i_min] の隣接点の部分集合 sub すべてについて
		repb(sub, d) {
			rep(i, d) {
				// sub に選んでいないなら無関係
				if (!(sub & (1 << i))) {
					continue;
				}

				repi(j, i + 1, d - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) {
						continue;
					}

					// sub がクリークでなければ何もしない．
					if (!adj[v[ia[i]]][v[ia[j]]]) {
						goto LOOP_END;
					}
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなるので記録する．
			cs.push_back(vi({ v[i_min] }));
			rep(i, d) {
				if (sub & (1 << i)) {
					cs.rbegin()->push_back(v[ia[i]]);
				}
			}

		LOOP_END:;
		}

		// v[i_min] を含む最大クリークの大きさは求まったので，
		// 以降は v[i_min] を含まないクリークだけを考えれば良い．
		// 頂点 v[i_min] と v[n-1] を交換して n を減らすことで v[i_min] を除去する．
		swap(v[i_min], v[n - 1]);
		n--;

		// 総次数 deg_sum，最小次数 deg_min，次数最小頂点の番号 i_min を得る．
		deg_sum = 0;
		deg_min = INF;
		rep(i, n) {
			if (chmin(deg_min, deg[v[i]])) {
				i_min = i;
			}
			deg_sum += deg[v[i]];
		}
	}
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, m;
	cin >> n >> m;

	Graph g;
	vector<unordered_map<int, ll>> c;
	read_graph(n, m, g, c);
	dumpel(g);
	dumpel(c);

	vvi cs;
	enumerate_clique(g, cs);
	dumpel(cs);

	ll res = 0;
	repe(cq, cs) {
		int k = sz(cq);
		if (k <= 1) continue;

		ll sc = 0;
		rep(i, k) {
			ll sat = INFL;
			rep(j, k) {
				if (i == j) continue;

				chmin(sat, c[cq[i]][cq[j]]);
			}
			sc += sat;
		}

		chmax(res, sc);
	}

	cout << res << endl;
}
