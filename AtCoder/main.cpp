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
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#define popcount (int)__popcnt // 全ビットにおける 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cout << "\033[1;36m" << (x) << "\033[0m" << endl;
#define dumps(x) cout << "\033[1;36m" << (x) << "\033[0m ";
#define dumpel(a) { int i = 0; cout << "\033[1;36m"; repe(x, a) {cout << i++ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream _in_(f); cin.rdbuf(_in_.rdbuf());
#define output_to_file(f) ofstream _out_(f); cout.rdbuf(_out_.rdbuf());
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
#define dumps(x)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


//-----------------AtCoder 専用-----------------
#include <atcoder/all>
using namespace atcoder;

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


// ビット全探索
void naive_solve(int n, const vi& c) {
	vm num(n + 1), dnm(n + 1);

	repb(s, n) {
		int k = popcount(s);

		set<int> col;
		rep(i, n) {
			if (s & (1 << i)) {
				col.insert(c[i]);
			}
		}

		num[k] += sz(col);
		dnm[k]++;
	}

	repi(i, 1, n) {
		cout << num[i] / dnm[i] << endl;
	}
}


//【階乗と二項係数（mint利用）】
/*
* 十分大きな素数を法として，階乗，その逆数，二項係数を計算する．
*
* factorial_mint(n) : O(n)
*	n! までの階乗とその逆数を前計算する．
*
* factorial(n) : O(1)
*	n! を返す．
*
* factorial_inv(n) : O(1)
*	n! の逆元を返す．
*
* permutation(n, r) : O(1)
*	順列の数 nPr を返す．
*
* binomial(n, r) : O(1)
*	二項係数 nCr を返す．
*
* multinomial(r) : O(|r|)
*	多項係数 nC[r] を返す．（n = Σr）
*/
struct factorial_mint {
	// 階乗とその逆数の値を保持するテーブル
	vm fac;
	vm fac_inv;

	// n! までの階乗とその逆数を前計算しておく．O(n)
	factorial_mint(int n) {
		fac = vector<mint>(n + 1);
		fac[0] = 1;
		repi(i, 1, n) {
			fac[i] = fac[i - 1] * i;
		}

		fac_inv = vector<mint>(n + 1);
		fac_inv[n] = fac[n].inv();
		repir(i, n - 1, 1) {
			fac_inv[i] = fac_inv[i + 1] * (i + 1);
		}
		fac_inv[0] = 1;
	}

	// n! を返す．O(1)
	mint factorial(int n) {
		return fac[n];
	}

	// (n!)^(-1) を返す．O(1)
	mint factorial_inv(int n) {
		return fac_inv[n];
	}

	// 順列の数 nPr を返す．O(1)
	mint permutation(int n, int r) {
		if (r < 0 || n - r < 0) {
			return 0;
		}
		return fac[n] * fac_inv[n - r];
	}

	// 二項係数 nCr を返す．O(1)
	mint binomial(int n, int r) {
		if (r < 0 || n - r < 0) {
			return 0;
		}
		return fac[n] * fac_inv[r] * fac_inv[n - r];
	}

	// 多項係数 nC[r] を返す．O(|r|)
	mint multinomial(const vi& r) {
		int len = sz(r);

		int sum = 0;
		rep(i, len) {
			sum += r[i];
		}

		mint res = fac[sum];
		repe(ri, r) {
			res *= fac_inv[ri];
		}

		return res;
	}
};


// 遅い DP
void inline_dp_solve(int n, const vi& c) {
	map<int, int> cnt_;
	rep(i, n) cnt_[c[i]]++;

	vi cnt;
	repe(p, cnt_) cnt.push_back(p.second);
	sort(all(cnt));

	factorial_mint fm(n);

	vector<pair<mint, mint>> dp(n + 1);
	dp[0] = { 0, 1 };
	dump(dp);
	
	int cnt_sum = 0;
	repe(k, cnt) {
		auto dp_ = dp;

		repi(i, 0, cnt_sum) {
			repi(j, 1, k) {
				if (i + j > n) break;

				dp_[i + j].first += fm.binomial(k, j) * (dp[i].first + dp[i].second);
				dp_[i + j].second += fm.binomial(k, j) * dp[i].second;
			}
		}

		cnt_sum += k;
		dp = dp_;
		dump(dp);
	}
	
	repi(i, 1, n) {
		cout << dp[i].first / dp[i].second << endl;
	}
}


//【行列】
/*
* 行列を表す構造体
*
* Matrix(m, n) : O(m n)
*	m * n 零行列で初期化する．
*
* Matrix(n) : O(n^2)
*	n * n 単位行列で初期化する．
*
* Matrix(a) : O(m n)
*	配列 a の要素で初期化する．
*
* A + B : O(m n)
*	m * n 行列 A, B の和を返す．+= も使用可．
*
* A - B : O(m n)
*	m * n 行列 A, B の差を返す．-= も使用可．
*
* c * A ／ A * c : O(m n)
*	m * n 行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * x : O(m n)
*	m * n 行列 A と n 次元列ベクトル x の積を返す．
*
* x * A : O(m n)
*	m 次元行ベクトル x と m * n 行列 A の積を返す．
*
* A * B : O(l m n)
*	l * m 行列 A と m * n 行列 B の積を返す．
*
* pow(d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class T>
struct Matrix {
	int m, n; // 行列のサイズ（m 行 n 列）
	vector<vector<T>> v; // 行列の成分


	// コンストラクタ（初期化なし）
	Matrix() {}

	// 零行列で初期化
	Matrix(const int& m_, const int& n_) : m(m_), n(n_), v(m_, vector<T>(n_)) {}

	// 単位行列で初期化
	Matrix(const int& n_) : m(n_), n(n_), v(n_, vector<T>(n_)) {
		rep(i, n) v[i][i] = 1;
	}

	// 二次元配列で初期化
	Matrix(const vector<vector<T>>& a) : m(sz(a)), n(sz(a[0])), v(a) {}

	// 代入
	Matrix(const Matrix& b) = default;
	Matrix& operator=(const Matrix& b) = default;

	// 入力
	friend istream& operator>>(istream& is, Matrix& a) {
		rep(i, a.m) rep(j, a.n) is >> a.v[i][j];
		return is;
	}

	// アクセス
	vector<T>& operator[](int i) { return v[i]; }

	// 比較
	bool operator==(const Matrix& b) const {
		return m == b.m && n == b.n && v == b.v;
	}
	bool operator!=(const Matrix& b) const { return !(*this == b); }

	// 加算，減算，スカラー倍
	Matrix& operator+=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] += b.v[i][j];
		return *this;
	}
	Matrix& operator-=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] -= b.v[i][j];
		return *this;
	}
	Matrix& operator*=(const T& c) {
		rep(i, m) rep(j, n) v[i][j] *= c;
		return *this;
	}
	Matrix operator+(const Matrix& b) const { Matrix a = *this; return a += b; }
	Matrix operator-(const Matrix& b) const { Matrix a = *this; return a -= b; }
	Matrix operator*(const T& c) const { Matrix a = *this; return a *= c; }
	friend Matrix operator*(const T& c, const Matrix<T>& a) { return a * c; }

	// 行列ベクトル積 : O(m n)
	vector<T> operator*(const vector<T>& x) const {
		vector<T> y(m);
		rep(i, m) rep(j, n)	y[i] += v[i][j] * x[j];
		return y;
	}

	// ベクトル行列積 : O(m n)
	friend vector<T> operator*(const vector<T>& x, const Matrix& a) {
		vector<T> y(a.n);
		rep(i, a.m) rep(j, a.n) y[j] += x[i] * a.v[i][j];
		return y;
	}

	// 積：O(n^3)
	Matrix operator*(const Matrix& b) const {
		Matrix res(m, b.n);
		rep(i, res.m) rep(j, res.n) rep(k, n) res.v[i][j] += v[i][k] * b.v[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix& b) { *this = *this * b; return *this; }

	// 累乗：O(n^3 log d)
	Matrix pow(ll d) const {
		Matrix res(n), pow2 = *this;
		while (d > 0) {
			if ((d & 1) != 0) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Matrix& a) {
		rep(i, a.m) {
			rep(j, a.n) os << a.v[i][j] << ' ';
			os << endl;
		}
		return os;
	}
};


//【多項式】
/*
* 多項式を表す構造体
*
* Polynomial() : O(1)
*	零多項式 f = 0 で初期化する．
*
* Polynomial(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* Polynomial(c) : O(|c|)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
*
* c + f ／ f + c : O(1)
*	多項式 f と定数 c の和を返す．+= も使用可．
*
* f - c : O(1)
*	多項式 f と定数 c の差を返す．-= も使用可．
*
* c - f : O(n)
*	定数 c と多項式 f の差を返す．
*
* f + g : O(n)
*	多項式 f, g の和を返す．+= も使用可．
*
* -f : O(n)
*	多項式 f の加法逆元を返す．
*
* f - g : O(n)
*	多項式 f, g の差を返す．-= も使用可．
*
* c * f ／ f * c : O(n)
*	多項式 f と定数 c の積を返す．*= も使用可．
*
* f / c : O(n)
*	多項式 f を定数 c で割った多項式を返す．/= も使用可．
*
* f * g : O((n + m) log(n + m))
*	多項式 f, g の積を返す．*= も使用可．（deg g = m）
*	制約：T = ll, modint998244353
*
* f.pow(d) : O(n d log n log d) ?
*	多項式 f の d 乗を返す．
*	制約：T = ll, modint998244353
*
* f.inv(d) : O((n + d) log(n + d)) ?
*	多項式 f の x^d を法とする乗法逆元を返す．
*	制約：T = modint998244353
*
* f / g : O(n log n) ?
*	多項式 f を g で割った商を返す．
*	制約：T = modint998244353
*
* f % g : O(n log n) ?
*	多項式 f を g で割った余りを返す．
*	制約：T = modint998244353
*
* d % f : O(n log n log d) ?
*	多項式 x^d を f で割った余りを返す．
*	制約：T = modint998244353
*
* assign(x) : O(n)
*	多項式 f の不定元に x を代入した値を返す．
*
* deg() : O(1)
*	多項式 f の次数を返す．
*/
template <class T>
struct Polynomial {
	int n; // 多項式の係数の個数（次数 + 1）
	vector<T> c; // 多項式の係数列


	// コンストラクタ（零多項式，定数多項式，係数列で初期化）
	Polynomial() : n(0) {}
	Polynomial(const T& c0) : n(1), c({ c0 }) {}
	Polynomial(const vector<T>& c_) : n(sz(c_)), c(c_) {}

	// 代入
	Polynomial(const Polynomial& f) = default;
	Polynomial& operator=(const Polynomial& f) = default;
	Polynomial& operator=(const T& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	T& operator[](int i) { return c[i]; }

	// 加算
	Polynomial& operator+=(const Polynomial& g) {
		if (n >= g.n) {
			rep(i, g.n) c[i] += g.c[i];
		}
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	Polynomial operator+(const Polynomial& g) const { Polynomial h = *this; h += g; return h; }

	// 定数加算
	Polynomial& operator+=(const T& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	Polynomial operator+(const T& sc) const { Polynomial h = *this; h += sc; return h; }
	friend Polynomial operator+(const T& sc, const Polynomial& f) { return f + sc; }

	// 加法逆元
	Polynomial operator-() { Polynomial h = *this; h *= -1; return h; }

	// 減算
	Polynomial& operator-=(const Polynomial& g) {
		if (n >= g.n) {
			rep(i, g.n) c[i] -= g.c[i];
		}
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	Polynomial operator-(const Polynomial& g) const { Polynomial h = *this; h -= g; return h; }

	// 定数減算
	Polynomial& operator-=(const T& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] -= sc; }
		return *this;
	}
	Polynomial operator-(const T& sc) const { Polynomial h = *this; h -= sc; return h; }
	friend Polynomial operator-(const T& sc, const Polynomial& f) { return -(f - sc); }

	// 定数倍
	Polynomial& operator*=(const T& sc) {
		rep(i, n) c[i] *= sc;
		return *this;
	}
	Polynomial operator*(const T& sc) const { Polynomial h = *this; h *= sc; return h; }
	friend Polynomial operator*(const T& sc, const Polynomial& f) { return f * sc; }

	// 右からの定数除算
	Polynomial& operator/=(const T& sc) {
		rep(i, n) c[i] /= sc;
		return *this;
	}
	Polynomial operator/(const T& sc) const { Polynomial h = *this; h /= sc; return h; }

	// 積
	Polynomial& operator*=(const Polynomial& g);
	Polynomial operator*(const Polynomial& g) const { Polynomial h = *this; h *= g; return h; }

	// 累乗
	Polynomial pow(ll d) const {
		Polynomial res(1), pow2 = *this;
		while (d > 0) {
			if ((d & 1) != 0) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 乗法逆元
	Polynomial inv(int d) const;

	// 商
	Polynomial operator/(const Polynomial& g) const;
	Polynomial& operator/=(const Polynomial& g) { return *this = *this / g; }

	// 余り
	Polynomial operator%(const Polynomial& g) const {
		return (*this - (*this / g) * g).resize(g.n - 1);
	}
	Polynomial& operator%=(const Polynomial& g) { return *this = *this % g; }

	// 係数反転
	Polynomial rev() const {
		Polynomial h = *this;
		reverse(all(h.c));
		return h;
	}

	// 不要な高次項の除去
	Polynomial& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == 0) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去
	Polynomial& resize(int d) {
		// x^d 以上の項を除去する．
		n = d;
		c.resize(d);
		return *this;
	}

	// 不定元への代入
	T assign(const T& x) const {
		T val = 0;
		repir(i, n - 1, 0) {
			val = val * x + c[i];
		}
		return val;
	}

	// 次数
	int deg() const { return n - 1; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Polynomial& f) {
		if (f.n == 0)
			os << 0;
		else {
			rep(i, f.n) {
				os << f.c[i] << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};

// 積
template <>
inline Polynomial<ll>& Polynomial<ll>::operator*=(const Polynomial<ll>& g) {
	c = convolution_ll(c, g.c);
	n = sz(c);
	return *this;
}
template <>
inline Polynomial<modint998244353>& Polynomial<modint998244353>::operator*=(const Polynomial<modint998244353>& g) {
	c = convolution(c, g.c);
	n = sz(c);
	return *this;
}


int main() {
	//	input_from_file("input.txt");
	//	output_to_file("output.txt");

	int n;
	cin >> n;

	vi c(n);
	cin >> c;

//	naive_solve(n, c);
//	inline_dp_solve(n, c);

	// 色 → その色のキャンディの個数
	map<int, int> cnt;
	rep(i, n) cnt[c[i]]++;
	dump(cnt);

	// キャンディの個数 → その個数だけあるキャンディが何種類あるか
	map<int, int> kind;
	repe(p, cnt) kind[p.second]++;
	dump(kind);

	// 母関数の計算
	Matrix<Polynomial<mint>> gf(2);
	dump(gf);
	repe(p, kind) {
		dump(p);
		Polynomial<mint> f({ 1, 1 });
		f = f.pow(p.first);
		dump(f);

		Matrix<Polynomial<mint>> r({ {f, f - 1}, {mint(0), f} });
		dump(r);
		gf *= r.pow(p.second);
		dump(r.pow(p.second));
		dump(gf);
	}

	Polynomial<mint> num(gf[0][1]), dnm(gf[1][1]);
	dump(num);
	dump(dnm);
	repi(i, 1, n) {
		cout << num[i] / dnm[i] << endl;
	}
}