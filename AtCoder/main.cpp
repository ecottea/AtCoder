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
#define popcount (int)__popcnt // 全ビット中の 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cout << "\033[1;36m" << (x) << "\033[0m" << endl;
#define dumps(x) cout << "\033[1;36m" << (x) << "\033[0m ";
#define dumpel(a) { int i = 0; cout << "\033[1;36m"; repe(x, a) {cout << i++ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream isTMP(f); cin.rdbuf(isTMP.rdbuf());
#define output_to_file(f) ofstream osTMP(f); cout.rdbuf(osTMP.rdbuf());
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

template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


//【多項式，形式的冪級数】
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
* f.pow(k) : O(n k log n log k) ?
*	多項式 f の k 乗を返す．
*	制約：T = ll, modint998244353
*
* f.inv(d) : O(n log n)
*	多項式 f の x^d を法とする乗法逆元を返す．
*	制約：f(0) ≠ 0, T = modint998244353
*
* f / g : O(n log n) 
*	多項式 f を g で割った商を返す．
*	制約：T = modint998244353
*
* f % g : O(n log n) 
*	多項式 f を g で割った余りを返す．
*	制約：T = modint998244353
*
* d % f : O(n log n log d) ?
*	多項式 x^d を f で割った余りを返す．
*	制約：T = modint998244353
*
* assign(v) : O(n)
*	多項式 f の不定元 x に v を代入した値を返す．
*
* deg() : O(1)
*	多項式 f の次数を返す．
* 
* shift(d) : O(n)
*	係数列を d だけシフトした多項式を返す．
*	（d >= 0 なら x^d の乗算，d < 0 なら x^(-d) で割った商と等価）
*/
template <class T> struct Polynomial {
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
	T const& operator[](int i) const { return c[i]; }
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
	Polynomial pow(ll k) const {
		Polynomial res(1), pow2 = *this;
		while (k > 0) {
			if ((k & 1) != 0) res *= pow2;
			pow2 *= pow2;
			k /= 2;
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

	// 係数のシフト
	Polynomial shift(int d) const {
		Polynomial f = *this;
		if (d > 0) {
			f.n += d;
			vector<T> zeros(d);
			f.c.insert(f.c.begin(), zeros.begin(), zeros.end());
		}
		else if (d < 0) {
			f.n -= d;
			if (f.n <= 0) {
				f.c.clear();
				f.n = 0;
			}
			else {
				f.c.erase(f.c.begin(), f.c.begin() - d);
			}
		}
		return f;
	}

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
template <> inline Polynomial<ll>& Polynomial<ll>::operator*=(const Polynomial<ll>& g) {
	c = convolution_ll(c, g.c);
	n = sz(c);
	return *this;
}
template <> inline Polynomial<modint998244353>& Polynomial<modint998244353>::operator*=(const Polynomial<modint998244353>& g) {
	c = convolution(c, g.c);
	n = sz(c);
	return *this;
}

// 乗法逆元
template <> Polynomial<modint998244353> Polynomial<modint998244353>::inv(int d) const {
	// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

	//【方法】
	// f^(-1) mod x^d を求めることは，
	//		f g = 1 (mod x^d)
	// なる g を求めることである．
	// この d の部分を 1, 2, 4, ..., 2^i と倍々にして求めていく．
	//
	// d = 1 のときについては
	//		g = f[0]^(-1) (mod x^1)
	// である．
	//
	// 次に，
	//		g = h (mod x^k)
	// が求まっているとして
	//		g mod x^(2 k)
	// を求める．最初の式を変形していくことで
	//		g - h = 0 (mod x^k)
	//		⇒ (g - h)^2 = 0 (mod x^(2 k))
	//		⇔ g^2 - 2 g h + h^2 = 0 (mod x^(2 k))
	//		⇒ f g^2 - 2 f g h + f h^2 = 0 (mod x^(2 k))
	//		⇔ g - 2 h + f h^2 = 0 (mod x^(2 k)) (f g = 1 (mod x^d) より)
	//		⇔ g = (2 - f h) h (mod x^(2 k))
	// を得る．
	//
	// この手順を d <= 2^i となる i まで繰り返し，d 次以上の項を削除すればよい．

	Polynomial<modint998244353> g(c[0].inv());
	for (int k = 1; k < d; k *= 2) {
		g = (modint998244353(2) - *this * g) * g;
		g.resize(2 * k);
	}

	return g.resize(d);
}

// 商
template <> Polynomial<modint998244353> Polynomial<modint998244353>::operator/(const Polynomial<modint998244353>& g) const {
	// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

	//【方法】
	// f(x) = g(x) q(x) + r(x) となる q(x) を求める．
	// f の次数は n - 1, g の次数は m - 1 とする．(n >= m)
	// 従って q の次数は n - m，r の次数は m - 2 となる．
	// 
	// f^R で f の係数列を逆順にした多項式を表す．すなわち
	//		f^R(x) := f(1/x) x^(n-1)
	// である．他の多項式も同様とする．
	//
	// 最初の式で x → 1/x と置き換えると，
	//		f(1/x) = g(1/x) q(1/x) + r(1/x)
	//		⇔ f(1/x) x^(n-1) = g(1/x) q(1/x) x^(n-1) + r(1/x) x^(n-1)
	//		⇔ f(1/x) x^(n-1) = g(1/x) x^(m-1) q(1/x) x^(n-m) + r(1/x) x^(m-2) x^(n-m+1)
	//		⇔ f^R(x) = g^R(x) q^R(x) + r^R(x) x^(n-m+1)
	//		⇒ f^R(x) = g^R(x) q^R(x) (mod x^(n-m+1))
	// 	    ⇒ q^R(x) = f^R(x) / g^R(x)  (mod x^(n-m+1))
	// を得る．
	// 	   
	// これで q を mod x^(n-m+1) で正しく求めることができることになるが，
	// q の次数は n - m であったから，q 自身を正しく求めることができた．

	if (n >= g.n) {
		return ((this->rev() * g.rev().inv(n - g.n + 1)).resize(n - g.n + 1)).rev();
	}
	else {
		return Polynomial<modint998244353>();
	}
}

// x^d % f を返す．
Polynomial<modint998244353> operator%(ll d, const Polynomial<modint998244353>& f) {
	Polynomial<modint998244353> res(1), pow2({ 0, 1 });
	while (d > 0) {
		if (d & 1) {
			res = (res * pow2) % f;
		}
		pow2 = (pow2 * pow2) % f;
		d /= 2;
	}
	return res;
}


//【微分】O(n)
/*
* f'(x) を返す．
*/


//【指数関数】O(n log n)
/*
* exp(f(x)) mod x^d を返す．
* 
* 制約：f(0) = 0
*/
Polynomial<modint998244353> exp(const Polynomial<modint998244353>& f, int d) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

	//【方法】
	// g(x) = exp(f(x)) とおき，方程式
	//		log g(x) = f(x)
	// に対してニュートン法を用いる．
	// 
	// f(0) = 0 なので，mod x^1 では
	//		log(1) ≡ f(x) mod x^1
	// が成り立つ．
	//
	// mod x^k で
	//		log h(x) ≡ f(x) mod x^k
	// が成り立っていると仮定すると，ニュートン法より
	//		g = h - (log h - f) / (log h)'
	//   ⇔ g = h (f + 1 - log h)
	// と置くと
	//		log g(x) ≡ f(x) mod x^(2 k)
	// が成り立つ．
	//
	// これを繰り返せば所望の g が求まる．

	// ニュートン法で log g = f なる g を見つける．
	Polynomial<modint998244353> g(1);
	for (int k = 1; k < d; k *= 2) {
		g = g * (f + 1 - log(g, 2 * k));
		g.resize(2 * k);
	}

	return g;
}

int main() {
	cout << fixed << setprecision(12);
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, m;
	cin >> n >> m;

	vm a(n);
	cin >> a;

	Polynomial<mint> A(a);
	bool find;
	auto B = sqrt(A, n, find);
	if (!find) {
		cout << -1 << endl;
		return 0;
	}

	rep(i, n) {
		cout << B[i];
		cout << (i < n - 1 ? " " : "\n");
	}
}
