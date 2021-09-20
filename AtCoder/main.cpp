#ifndef HIDDEN_IN_VISUAL_STUDIO // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// 使えるライブラリの読み込み
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
const ll INFL = (ll)2e18;	const int INF = (int)1e9;
const double EPS = 1e-10; // 許容誤差に応じて調整

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

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 入出力用の >>, << のオーバーロード
template <class T, class U> inline istream& operator>> (istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T, class U> inline ostream& operator<< (ostream& os, const pair<T, U>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template <class T, class U, class V> inline istream& operator>> (istream& is, tuple<T, U, V>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t); return is; }
template <class T, class U, class V> inline ostream& operator<< (ostream& os, const tuple<T, U, V>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")"; return os; }
template <class T, class U, class V, class W> inline istream& operator>> (istream& is, tuple<T, U, V, W>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t); return is; }
template <class T, class U, class V, class W> inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")"; return os; }
template <class T> inline istream& operator>> (istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline ostream& operator<< (ostream& os, const vector<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#define popcount (int)__popcnt // 全ビットにおける 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cerr << "\033[1;36m" << (x) << "\033[0m" << endl; // デバッグ出力用
#define dumpel(v) cerr << "\033[1;36m"; repe(x, (v)) {cerr << x << endl;} cerr << "\033[0m";
#define dumpeli(v) cerr << "\033[1;36m"; rep(i, sz(v)) {cerr << i << ": " << v[i] << endl;} cerr << "\033[0m";
#define input_from_file(f) ifstream in(f); cin.rdbuf(in.rdbuf());
// 提出用（GCC）
#else
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(x)
#define dumpel(v)
#define dumpeli(v)
#define input_from_file(f)
#endif

#endif // 折りたたみ用


//-----------------AtCoder 専用-----------------
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>> (istream& is, mint& x) { ll tmp; is >> tmp; x = tmp; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


//【ウェーブレット行列】
/*
* Wavelet_matrix(a) : O(n)
*	辞書を多重集合 a で初期化する．
* 
* get(i) : O(log(max a))
*	昇順で i 番目の要素を返す．
*
* get(l, r, i) : O(log(max a))
*	a[l..r) の中で昇順で i 番目の要素を返す．
*
* count(l, r, v) : O(log(max a))
*	a[l..r) に v が何個あるかを返す．
* 
* position(v, c) : O(log(n) log(max a))
*	昇順で c 番目の v の位置を返す．
* 
* frequency(l, r, c, freq) : O((r - l) log(max a))
*	a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組のリストを freq に格納する．
* 
* sum(l, r) : O((r - l) log(max a))
*	a[l..r) の和を返す．
* 
* intersection(l1, r1, l2, r2, freq) : O(((r1 - l1) + (r2 - l2)) log(max a))
*	a[l1..r1) と a[l2..r2) に共通する要素を求め，
*	その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
*/
struct Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259
	
	int n; // 要素数
	int k; // msb 以下の桁数（1-indexed）
	vvb bs; // bs[j][i] : 第 j + 1 ビットについての安定ソート後の a[i] の第 j ビット
	vvvi bs_acc; // bs[b] : のビット b = 0, 1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	unordered_map<ll, int> id; // 値 → 安定ソートが終わったときの最左位置
	
	// コンストラクタ（何もしない）
	Wavelet_matrix() : n(0) {}

	// 辞書を多重集合 a で初期化する．
	Wavelet_matrix(const vl& t)
		: n(sz(t)), k(msbll(*max_element(all(t))) + 1),
		bs(k, vb(n)), bs_acc(2, vvi(k, vi(n + 1))), num_zeros(k)
	{
		// ビットと組にして安定ソートするためのリスト
		vector<pair<bool, ll>> bt(n);
		rep(i, n) {
			bt[i].second = t[i];
		}

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, k - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[j][i] = bt[i].first = (bt[i].second & (1LL << j));

				// ビット 0, 1 それぞれの個数の累積和を求めておく．
				rep(b, 2) {
					bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				}
				if (bs[j][i]) {
					bs_acc[1][j][i + 1]++;
				}
				else {
					bs_acc[0][j][i + 1]++;
					num_zeros[j]++;
				}
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			stable_sort(all(bt), [](auto const& lhs, auto const& rhs) {
				return lhs.first < rhs.first;
			});
		}

		// 値 → 安定ソートが終わったときの最左位置
		rep(i, n) {
			if (!id.count(bt[i].second)) id[bt[i].second] = i;
		}
	}

	// 昇順で i 番目の要素を返す．
	ll get(int i) {
		ll res = 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			res *= 2;

			// 注目ビットに応じて次の位置を求めつつ，値を更新していく．
			if (bs[j][i]) {
				res++;
				i = num_zeros[j] + bs_acc[1][j][i];
			}
			else {
				i = bs_acc[0][j][i];
			}
		}

		return res;
	}

	// a[l..r) に v が何個あるかを返す．
	int count(int l, int r, ll v) {
		return count_sub(r, v) - count_sub(l, v);
	}

	// a[0..r) に v が何個あるかを返す．
	int count_sub(int r, ll v) {
		// 一つも無ければすぐに 0 を返す．
		if (!id.count(v)) return 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			// 注目ビットに応じて次の位置を求めていく．
			if (v & (1LL << j)) {
				r = num_zeros[j] + bs_acc[1][j][r];
			}
			else {
				r = bs_acc[0][j][r];
			}
		}

		return r - id[v];
	}

	// 昇順で c 番目の v の位置を返す．
	int position(ll v, int c) {
		if (!id.count(v)) return -1;

		int i = id[v] + c;
		rep(j, k) {
			if (v & (1LL << j)) {
				auto it = upper_bound(all(bs_acc[1][j]), i - num_zeros[j]);
				i = distance(bs_acc[1][j].begin(), it) - 1;
			}
			else {
				auto it = upper_bound(all(bs_acc[0][j]), i);
				i = distance(bs_acc[0][j].begin(), it) - 1;
			}
		}

		return i;
	}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	ll get(int l, int r, int i) {
		ll res = 0;

		repir(j, k - 1, 0) {
			res *= 2;

			int cnt0 = bs_acc[0][j][r] - bs_acc[0][j][l];
			if (i >= cnt0) {
				res++;
				l = num_zeros[j] + bs_acc[1][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				i -= cnt0;
			}
			else {
				l = bs_acc[0][j][l];
				r = bs_acc[0][j][r];
			}
		}

		return res;
	}

	// a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組を返す．
	void frequency(int l, int r, int c, vector<pli>& freq) {
		freq.clear();

		priority_queue<tuple<int, int, int, int, ll>> q;
		q.push({ r - l, k - 1, l, r, 0 });

		while (!q.empty()) {
			int w, j;
			ll v;
			tie(w, j, l, r, v) = q.top();
			q.pop();

			if (j == -1) {
				freq.push_back({ v, w });
				if (--c == 0) return;
			}
			else {
				int l1 = num_zeros[j] + bs_acc[1][j][l];
				int r1 = num_zeros[j] + bs_acc[1][j][r];
				int l0 = bs_acc[0][j][l];
				int r0 = bs_acc[0][j][r];

				q.push({ r1 - l1, j - 1, l1, r1, 2 * v + 1 });
				q.push({ r0 - l0, j - 1, l0, r0, 2 * v });
			}
		}
	}

	// a[l..r) の和を返す．
	ll sum(int l, int r) {
		vector<pli> freq;
		frequency(l, r, INF, freq);

		ll res = 0;
		repe(p, freq) {
			res += p.first * p.second;
		}

		return res;
	}

	// a[l1..r1) と a[l2..r2) に共通する要素を求め，
	// その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
	void intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) {
		freq.clear();

		queue<tuple<int, int, int, int, int, ll>> q;
		q.push({ k - 1, l1, r1, l2, r2, 0 });

		while (!q.empty()) {
			int j;
			ll v;
			tie(j, l1, r1, l2, r2, v) = q.front();
			q.pop();

			if (l1 == r1 || l2 == r2) {
				continue;
			}

			if (j == -1) {
				freq.push_back({ v, r1 - l1, r2 - l2 });
			}
			else {
				int l11 = num_zeros[j] + bs_acc[1][j][l1];
				int r11 = num_zeros[j] + bs_acc[1][j][r1];
				int l10 = bs_acc[0][j][l1];
				int r10 = bs_acc[0][j][r1];
				int l21 = num_zeros[j] + bs_acc[1][j][l2];
				int r21 = num_zeros[j] + bs_acc[1][j][r2];
				int l20 = bs_acc[0][j][l2];
				int r20 = bs_acc[0][j][r2];

				q.push({ j - 1, l11, r11, l21, r21, 2 * v + 1 });
				q.push({ j - 1, l10, r10, l20, r20, 2 * v });
			}
		}
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, ll v0, ll v1) {
		return count_rsub(l, r, v1) - count_rsub(l, r, v0);
	}

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, ll v) {
		if (msbll(v) >= k) return r - l;

		int cnt = 0;
		repir(j, k - 1, 0) {
			if (v & (1LL << j)) {
				cnt += bs_acc[0][j][r] - bs_acc[0][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return cnt;
	}
};


int main() {
//	input_from_file("input.txt"); // ファイルから入力

	vl t({ 5, 4, 5, 5, 2, 1, 5, 6, 1, 3, 5, 0 });
	int n = sz(t);

	Wavelet_matrix wm(t);
	dump(wm.n);
	dump(wm.k);
	dumpel(wm.bs);
	dumpel(wm.bs_acc[1]);
	dumpel(wm.bs_acc[0]);
	dump(wm.num_zeros);
	dump(wm.id);

	rep(i, n) {
		dump(wm.get(i));
	}
	dump("dt78fth68tre78ht78her78rtfh8");

	repi(i, 0, n) {
		cout << wm.count(0, i, 5, 6) << " ";
		dump(wm.count(0, i, 5));
	}
	dump("dt78fth68tre78ht78her78rtfh8");

	rep(c, 5) {
		dump(wm.position(5, c));
	}
	dump("dt78fth68tre78ht78her78rtfh8");

	rep(i, 11 - 1) {
		dump(wm.get(1, 11, i));
	}
	dump("dt78fth68tre78ht78her78rtfh8");

	vector<pli> freq;
	wm.frequency(1, 10, 2, freq);
	dump(freq);

	dump(wm.sum(1, 10));

	vector<tuple<ll, int, int>> freq2;
	wm.intersection(0, 6, 6, 11, freq2);
	dump(freq2);
}
