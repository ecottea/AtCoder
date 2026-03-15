#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; using ull = unsigned long long; // -2^63 ～ 2^63 = 9e18（int は -2^31 ～ 2^31 = 2e9）
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
int DX[4] = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
int DY[4] = { 0, 1, 0, -1 };
int INF = 1001001001; ll INFL = 4004004003094073385LL; // (int)INFL = INF, (int)(-INFL) = -INF;

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(18); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), (x)))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), (x)))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0, set##_ub = 1 << int(d); set < set##_ub; ++set) // d ビット全探索（昇順）
#define repis(i, set) for(int i = lsb(set), bset##i = set; i < 32; bset##i -= 1 << i, i = lsb(bset##i)) // set の全要素（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了
#define inQ(x, y, u, l, d, r) ((u) <= (x) && (l) <= (y) && (x) < (d) && (y) < (r)) // 半開矩形内判定

// 汎用関数の定義
template <class T> inline ll powi(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）
template <class T> inline T getb(T set, int i) { return (set >> i) & T(1); }
template <class T> inline T smod(T n, T m) { n %= m; if (n < 0) n += m; return n; } // 非負mod

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

using mint = modint998244353;
//using mint = static_modint<(int)1e9+7>;
//using mint = modint; // mint::set_mod(m);

using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>; using vvvvm = vector<vvvm>; using pim = pair<int, mint>;
#endif


#ifdef _MSC_VER // 手元環境（Visual Studio）
#include "local.hpp"
#else // 提出用（gcc）
int mute_dump = 0;
int frac_print = 0;
#if __has_include(<atcoder/all>)
namespace atcoder {
	inline istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
	inline ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
}
#endif
inline int popcount(int n) { return __builtin_popcount(n); }
inline int popcount(ll n) { return __builtin_popcountll(n); }
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : 32; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : 64; }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define dump(...)
#define dumpel(v)
#define dump_math(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) { vc MLE(1<<30); EXIT(MLE.back()); } } // RE の代わりに MLE を出す
#endif


//【ウェーブレット行列】
/*
* Wavelet_matrix<T>(vT a) : O(n log n)
*	整数列 a[0..n)（負値も可）で初期化する．
*
* T get(int l, int r, int i) : O(log n)
*	a[l..r) の中で昇順で i 番目の要素を返す（なければ INFL）
*
* int count(int l, int r, T v) : O(log n)
*	a[l..r) の中で (-∞..v) に値をもつ要素の個数を返す．
*
* int count(int l, int r, T v0, T v1) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*
* ll sum(int l, int r, T v) : O(log n)
*	a[l..r) の中で (-∞..v) に値をもつ要素の和を返す．
*
* ll sum(int l, int r, T v0, T v1) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* T ascending_sum(int l, int r, int i) : O(log n)
*	a[l..r) の中で昇順 i 個の要素の和を返す．（i 個なければ INFL）
*
* ll abs_sum(int l, int r, T v) : O(log n)
*	Σi∈[l..r) |a[i] - v| を返す．
*/
template <class T>
class Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	// n : 要素数
	int n;

	// K : 座圧後のビット数の最大値
	int K;

	// cnt0[k][i] : 第 k+1 ビットでの安定ソート後の t[0..i) 内の第 k ビットの 0 の個数
	vvi cnt0;

	// acc[k][i] : 第 k ビットでの安定ソート後の Σa[0..i)
	vector<vector<T>> acc;

	// 座圧前の値のユニークな昇順列
	vector<T> val;

	// t[l..r) の中で [0..ord) に値をもつ要素の個数を返す．
	int count_sub(int l, int r, int ord) {
		int res = 0;

		repir(k, K - 1, 0) {
			if (getb(ord, k)) {
				res += cnt0[k][r] - cnt0[k][l];

				l += cnt0[k][n] - cnt0[k][l];
				r += cnt0[k][n] - cnt0[k][r];
			}
			else {
				l = cnt0[k][l];
				r = cnt0[k][r];
			}
		}

		return res;
	}

	// [l..r) の中で [0..ord) に値をもつ要素の座圧前の値の和を返す．
	T sum_sub(int l, int r, int ord) {
		T res = T(0);

		repir(k, K - 1, 0) {
			if (getb(ord, k)) {
				res += acc[k][cnt0[k][r]] - acc[k][cnt0[k][l]];

				l += cnt0[k][n] - cnt0[k][l];
				r += cnt0[k][n] - cnt0[k][r];
			}
			else {
				l = cnt0[k][l];
				r = cnt0[k][r];
			}
		}

		return res;
	}

public:
	// 整数列 a[0..n) で初期化する．
	Wavelet_matrix(const vector<T>& a) : n(sz(a)), val(a) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

		// a[0..n) を座標圧縮して t[0..n) にする．
		uniq(val);
		val.push_back((T)INFL + 1); // 番兵

		vi t(n);
		rep(i, n) t[i] = lbpos(val, a[i]);

		K = msb(sz(val) - 1) + 1;

		cnt0.assign(K, vi(n + 1));
		acc.assign(K, vector<T>(n + 1));

		// k : 注目ビット位置（第 k+1 ビットでの安定ソート済）
		repir(k, K - 1, 0) {
			rep(i, n) {
				// 第 k ビットの 0 の累積個数を求める．
				cnt0[k][i + 1] += cnt0[k][i] + (1 - getb(t[i], k));
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			vi nt0, nt1;
			nt0.reserve(cnt0[k][n]);
			nt1.reserve(n - cnt0[k][n]);

			rep(i, n) {
				if (getb(t[i], k)) nt1.push_back(t[i]);
				else nt0.push_back(t[i]);
			}
			t.clear();
			repe(x, nt0) t.push_back(x);
			repe(x, nt1) t.push_back(x);

			rep(i, n) {
				// 座圧前の値の累積和を求める．
				acc[k][i + 1] = acc[k][i] + val[t[i]];
			}
		}

	}
	Wavelet_matrix() : n(0), K(0) {}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	T get(int l, int r, int i) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

		chmax(l, 0); chmin(r, n);
		if (i >= r - l) return T(INFL);

		// ord : 求める要素の座圧後の値（uniq(a) において昇順何番目か）
		int ord = 0;

		repir(k, K - 1, 0) {
			ord <<= 1;

			// c0 : 第 k+1 ビットでの安定ソート後の t[l..r) 内の第 k ビットの 0 の個数
			int c0 = cnt0[k][r] - cnt0[k][l];

			if (i >= c0) {
				// 第 k ビットは 1 に確定
				ord++;

				l += cnt0[k][n] - cnt0[k][l];
				r += cnt0[k][n] - cnt0[k][r];

				i -= c0;
			}
			else {
				// 第 k ビットは 0 に確定
				l = cnt0[k][l];
				r = cnt0[k][r];
			}
		}

		return val[ord];
	}

	// a[l..r) の中で (-∞..v) に値をもつ要素の個数を返す．
	int count(int l, int r, T v) {
		// verify : https://judge.yosupo.jp/problem/static_range_sum_with_upper_bound

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		int ord = lbpos(val, v);

		return count_sub(l, r, ord);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, T v0, T v1) {
		// verify : https://atcoder.jp/contests/abc396/tasks/abc396_f

		chmax(l, 0); chmin(r, n);
		if (l >= r || v0 >= v1) return 0;

		int ord0 = lbpos(val, v0);
		int ord1 = lbpos(val, v1);

		return count_sub(l, r, ord1) - count_sub(l, r, ord0);
	}

	// a[l..r) の中で (-∞..v) に値をもつ要素の和を返す．
	T sum(int l, int r, T v) {
		// verify : https://judge.yosupo.jp/problem/static_range_sum_with_upper_bound

		chmax(l, 0); chmin(r, n);;
		if (l >= r) return 0;

		int ord = lbpos(val, v);

		return sum_sub(l, r, ord);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	T sum(int l, int r, T v0, T v1) {
		chmax(l, 0); chmin(r, n);;
		if (l >= r || v0 >= v1) return 0;

		int ord0 = lbpos(val, v0);
		int ord1 = lbpos(val, v1);

		return sum_sub(l, r, ord1) - sum_sub(l, r, ord0);
	}

	// a[l..r) の中で昇順 i 個の要素の和を返す．（i 個なければ INFL）
	T ascending_sum(int l, int r, int i) {
		// verify : https://atcoder.jp/contests/abc281/tasks/abc281_e

		if (i == 0) return T(0);
		i--;

		chmax(l, 0); chmin(r, n);
		if (i >= r - l) return T(INFL);

		T res = T(0); int ord = 0;

		repir(k, K - 1, 0) {
			ord <<= 1;

			// c0 : 第 k+1 ビットでの安定ソート後の t[l..r) 内の第 k ビットの 0 の個数
			int c0 = cnt0[k][r] - cnt0[k][l];

			if (i >= c0) {
				// 第 k ビットは 1 に確定
				ord++;
				res += acc[k][cnt0[k][r]] - acc[k][cnt0[k][l]];

				l += cnt0[k][n] - cnt0[k][l];
				r += cnt0[k][n] - cnt0[k][r];

				i -= c0;
			}
			else {
				// 第 k ビットは 0 に確定
				l = cnt0[k][l];
				r = cnt0[k][r];
			}
		}

		res += (i + 1) * val[ord];

		return res;
	}

	// Σi∈[l..r) |a[i] - v| を返す．
	T abs_sum(int l, int r, T v) {
		// verify : https://yukicoder.me/problems/no/2169

		chmax(l, 0); chmin(r, n);
		if (l >= r) return T(0);

		int ord = lbpos(val, v);

		T res = sum_sub(l, r, (1 << K) - 1);
		res -= (r - l) * v;
		res -= 2 * sum_sub(l, r, ord);
		res += 2 * count_sub(l, r, ord) * v;

		return res;
	}
};


//【切り上げ（余り指定）】O(1)
/*
* x 以上の整数で mod m で k に等しい最小のものを返す．
*/
template <class T>
T ceil_mod(T x, T m, T k) {
	// verify: https://atcoder.jp/contests/abc334/tasks/abc334_b
	Assert(m > 0);
	return x + smod(k - x, m);
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");
	
	int n, m;
	cin >> n >> m;

	vi a(n);
	cin >> a;
	--a;

	vi cnt(m);
	rep(i, n) cnt[a[i]]++;
	dump(cnt);

	vvi C(n + 1);
	rep(j, m) C[cnt[j]].push_back(j);
	dump(C);

	vi seq;
	repi(i, 0, n) repe(x, C[i]) seq.push_back(x);
	dump(seq);

	Wavelet_matrix Seq(seq);

	vl acc(n + 2);
	rep(i, n + 1) acc[i + 1] = acc[i] + sz(C[i]);
	dump(acc);

	vl Acc(n + 3);
	rep(i, n + 2) Acc[i + 1] = Acc[i] + acc[i];
	dump(Acc);

	int q;
	cin >> q;

	rep(hoge, q) {
		dump("--------------------------------------");

		ll x;
		cin >> x;
		--x;

		if (x < n) {
			cout << a[x] + 1 << "\n";
		}
		else {
			x -= n;

			if (x > Acc[n + 2]) {
				x = ceil_mod<ll>(Acc[n + 2], m, x);
			}
			dump("x:", x);

			int k = ubpos(Acc, x) - 1;
			dump("k:", k);

			ll r = x - Acc[k];
			dump("r:", r);

			int res = Seq.get(0, acc[k], r);
			Assert(res != INF);

			cout << res + 1 << "\n";
		}
	}
}
