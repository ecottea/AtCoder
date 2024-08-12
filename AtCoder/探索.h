#pragma once
#include "header.h"
// ■■■■■ 探索 ■■■■■


//【めぐる式二分探索】O(log|ok - ng|)
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
* 境界に隣り合うような条件を満たす要素（ok 側）の位置を返す．
* debug_mode = true にして実行すると手元では単調かどうかチェックしながら全探索する．
*/
template <class T, class FUNC>
T meguru_search(T ok, T ng, const FUNC& okQ, bool debug_mode = false) {
	// 参考 : https://twitter.com/meguru_comp/status/697008509376835584
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_a

	Assert(ok != ng);

#ifdef _MSC_VER	
	// 単調かどうか自信がないとき用
	if (debug_mode) {
		T step = ok < ng ? 1 : -1; T res = ok; bool is_ok = true;
		for (T i = ok; i != ng + step; i += step) {
			auto b = okQ(i);
			if (b) {
				if (!is_ok) {
					cout << "not monotony!" << endl;
					for (T i = ok; i != ng + step; i += step) {
						cout << i << " : " << okQ(i) << endl;
					}
					exit(1);
				}
			}
			else {
				if (is_ok) res = i - step;
				is_ok = false;
			}
		}

		return res;
	}
#endif

	// 境界が決定するまで
	while (abs(ok - ng) > 1) {
		// 区間の中間
		T mid = (ok + ng) / 2;

		// 中間が OK かどうかに応じて区間を縮小する．
		if (okQ(mid)) ok = mid;
		else ng = mid;
	}
	return ok;

	/* okQ の定義の雛形
	auto okQ = [&](ll x) {
		return true || false;
	};
	*/
}


//【二分探索（実数）】O(log(|ok - ng| / EPS))
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
*/
template <class T, class FUNC>
T bin_search(T ok, T ng, const FUNC& okQ, double EPS = 1e-12) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2020/04/29/155009
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ck

	int L = max((int)log2(abs(ok - ng) / EPS), 1);

	rep(hoge, L) {
		// 区間の中間
		T mid = (ok + ng) * 0.5;

		// 相対誤差を小さくしたい場合はこちらを使う．
		//T mid = sqrt(ok * ng);

		// 中間が OK かどうかに応じて区間を縮小する．
		if (okQ(mid)) ok = mid;
		else ng = mid;
	}
	return (ok + ng) * 0.5;

	/* okQ の定義の雛形
	auto okQ = [&](double x) {
		return true || false;
	};
	*/
}


//【三分探索（最大値）】O(log(r - l))
/*
* 階差の符号変化が + → 0 → - である関数 f(x) の開区間 (l..r) における最大値を与える x を返す．
* debug_mode = true にして実行すると手元では単峰かどうかチェックしながら全探索する．
*/
template <class T, class FUNC>
T ternary_search_max(T l, T r, const FUNC& f, bool debug_mode = false) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_f

	Assert(r - l >= 2);

#ifdef _MSC_VER	
	// 単峰かどうか自信がないとき用
	if (debug_mode) {
		if (r - l == 2) return l + 1;

		auto p_val = f(l + 1); int p_sgn = 1;
		auto val_max = p_val; T i_max = l + 1;

		for (T i = l + 2; i < r; i++) {
			auto val = f(i);
			if (chmax(val_max, val)) i_max = i;

			int sgn = (val > p_val) - (val < p_val);
			if (p_sgn < sgn) {
				cout << "not unimodal!" << endl;
				for (T i = l + 1; i < r; i++) cout << f(i) << " \n"[i == r - 1];
				exit(1);
			}
			p_val = val;
			p_sgn = sgn;
		}

		return i_max;
	}
#endif

	while (r - l > 2) {
		T s = l + r;
		T m1 = s / 2 - (s % 2 < 0);
		T m2 = m1 + 1;

		if (f(m1) < f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	using T = ll;
	auto f = [&](T x) {
		return x;
	};
	*/
}


//【三分探索（最小値）】O(log(r - l))
/*
* 階差の符号変化が - → 0 → + である関数 f(x) の開区間 (l..r) における最小値を与える x を返す．
* debug_mode = true にして実行すると手元では単峰かどうかチェックしながら全探索する．
*/
template <class T, class FUNC>
T ternary_search_min(T l, T r, const FUNC& f, bool debug_mode = false) {
	// verify : https://atcoder.jp/contests/abc279/tasks/abc279_d

#ifdef _MSC_VER	
	// 単峰かどうか自信がないとき用
	if (debug_mode) {
		if (r - l == 2) return l + 1;

		auto p_val = f(l + 1); int p_sgn = -1;
		auto val_min = p_val; T i_min = l + 1;

		for (T i = l + 2; i < r; i++) {
			auto val = f(i);
			if (chmin(val_min, val)) i_min = i;

			int sgn = (val > p_val) - (val < p_val);
			if (p_sgn > sgn) {
				cout << "not unimodal!" << endl;
				for (T i = l + 1; i < r; i++) cout << f(i) << " \n"[i == r - 1];
				exit(1);
			}
			p_val = val;
			p_sgn = sgn;
		}

		return i_min;
	}
#endif

	while (r - l > 2) {
		T s = l + r;
		T m1 = s / 2 - (s % 2 < 0);
		T m2 = m1 + 1;

		if (f(m1) > f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	using T = ll;
	auto f = [&](T x) {
		return x;
	};
	*/
}


//【ランダム三分探索（最大値）】O(log(r - l))
/*
* 階差の符号変化が + → 0 → - である関数 f(x) の開区間 (l..r) における最大値を与える x を返す．
* 上に単峰でなくても運が良ければ正しい x を返す．
*/
template <class FUNC>
ll random_ternary_search_max(ll l, ll r, const FUNC& f) {
	// verify : https://atcoder.jp/contests/abc342/tasks/abc342_f

	static bool first_call = true;

	static mt19937 mt;
	static uniform_int_distribution<ll> rnd;
	if (first_call) {
		first_call = false;
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(0, INFL);
	}

	while (r - l > 2) {
		ll m1 = l + 1 + rnd(mt) % (r - l - 1);
		ll m2 = l + 1 + rnd(mt) % (r - l - 1);
		if (m1 == m2) continue;
		if (m1 > m2) swap(m1, m2);

		if (f(m1) < f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	auto f = [&](ll x) {
		return x;
	};
	*/
}


//【ランダム三分探索（最小値）】O(log(r - l))
/*
* 階差の符号変化が - → 0 → + である関数 f(x) の開区間 (l..r) における最小値を与える x を返す．
* 下に単峰でなくても運が良ければ正しい x を返す．
*/
template <class FUNC>
ll random_ternary_search_min(ll l, ll r, const FUNC& f) {
	// verify : https://yukicoder.me/problems/no/2627

	static bool first_call = true;

	static mt19937 mt;
	static uniform_int_distribution<ll> rnd;
	if (first_call) {
		first_call = false;
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(0, INFL);
	}

	while (r - l > 2) {
		ll m1 = l + 1 + rnd(mt) % (r - l - 1);
		ll m2 = l + 1 + rnd(mt) % (r - l - 1);
		if (m1 == m2) continue;
		if (m1 > m2) swap(m1, m2);

		if (f(m1) > f(m2)) l = m1; // 上に凸に対応したかったらここの不等号を逆にする．
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	auto f = [&](ll x) {
		return x;
	};
	*/
}


//【フィボナッチ探索】
/*
* Fibonacci_search(ll w) : O(log w)
*	最大で幅 w の開区間まで扱えるよう初期化する．
*
* ll search_max(ll l, ll r, function<ll(ll)> f) : O(log(r - l))
*	開区間 (l..r) で上に単峰な関数 f の最大値が f(i) であるとし，組 {i, f(i)} を返す．
*
* ll search_min(ll l, ll r, function<ll(ll)> f) : O(log(r - l))
*	開区間 (l..r) で下に単峰な関数 f の最小値が f(i) であるとし，組 {i, f(i)} を返す．
*/
class Fibonacci_search {
	int n;
	vl fib;

	pll search(ll left, ll right, const function<ll(ll)>& f, ll sgn) const {
		Assert(right - left >= 2);

		// 最大値の候補が 1 つしかない場合の例外処理（f(i) が不要なら省略可）
		if (right - left == 2) return make_pair(left + 1, f(left + 1));

		// 符号変化の条件を満たすよう範囲外の値を定めておく．
		auto F = [&](ll x) {
			if (x >= right) return -INFL - (x - right); // たぶん大丈夫だけどオーバーフローに注意
			else return sgn * f(x);
		};

		// l, m1, m2, r の順で区間を φ : 1 : φ に内分する点を得る．
		int i = n;
		ll l = left;
		ll r = l + fib[i];
		ll m1 = l + fib[i - 2];
		ll m2 = l + fib[i - 1];
		i -= 3;

		// 内分点における関数値の計算
		ll v1 = F(m1);
		ll v2 = F(m2);

		// 候補が内分点のみになるまで
		while (i > 0) {
			// 左の内分点での値の方が大きければ，次の区間は左側をとる．
			if (v1 > v2) {
				// 右の内分点を新たに右端とする．
				r = m2;

				// 左の内分点を新たに右の内分点とする．
				m2 = m1;
				v2 = v1;

				// 左の内分点を新たに計算する．
				m1 = l + fib[i];
				v1 = F(m1);
			}
			// 右の内分点での値の方が大きければ，次の区間は右側をとる．
			else {
				// 左の内分点を新たに左端とする．
				l = m1;

				// 右の内分点を新たに左の内分点とする．
				m1 = m2;
				v1 = v2;

				// 右の内分点を新たに計算する．
				m2 = r - fib[i];
				v2 = F(m2);
			}
			i--;
		}

		// 最後の候補を比較し，大きかった方の番号と値を返す．
		return (v1 > v2) ? make_pair(m1, sgn * v1) : make_pair(m2, sgn * v2);
	}

public:
	Fibonacci_search(ll w) : n(1), fib({ 1, 1 }) {
		// verify : https://yukicoder.me/problems/no/2627

		// 利用する範囲のフィボナッチ数列を準備する．
		while (fib[n] < w) {
			fib.push_back(fib[n] + fib[n - 1]);
			n++;
		}
	}

	// 開区間 (l..r) で上に単峰な関数 f の最大値が f(i) であるとし，組 {i, f(i)} を返す．
	pll search_max(ll l, ll r, const function<ll(ll)>& f) const {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ba

		return search(l, r, f, 1);
	}

	// 開区間 (l..r) で下に単峰な関数 f の最小値が f(i) であるとし，組 {i, f(i)} を返す．
	pll search_min(ll l, ll r, const function<ll(ll)>& f) const {
		// verify : https://atcoder.jp/contests/abc330/tasks/abc330_c

		return search(l, r, f, -1);
	}

	/* f の定義の雛形
	auto f = [&](ll x) {
		return x;
	};
	*/
};


//【黄金分割探索（実数，最大値）】O(log((r - l) / EPS))
/*
* 全域で上に単峰な関数 f(x) の開区間 (l..r) における最大値を与える x を返す．
*/
template <class FUNC>
double golden_search_max(double l, double r, const FUNC& f, double EPS = 1e-12) {
	constexpr double phi = 1.61803398875; // 黄金数

	int L = max((int)(log((r - l) / EPS) / log(phi)), 1);

	// l, m1, m2, r の順で区間を φ : 1 : φ に内分する点
	double m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
	double m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);

	// 内分点における関数値の計算
	double v1 = f(m1);
	double v2 = f(m2);

	// 絶対誤差か相対誤差が EPS 以下になるまで
	rep(hoge, L) {
		// 左の内分点での値の方が大きければ，次の区間は左側をとる．
		if (v1 > v2) {
			// 右の内分点を新たに右端とする．
			r = m2;

			// 左の内分点を新たに右の内分点とする．
			m2 = m1;
			v2 = v1;

			// 左の内分点を新たに計算する．
			m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
			v1 = f(m1);
		}
		// 右の内分点での値の方が大きければ，次の区間は右側をとる．
		else {
			// 左の内分点を新たに左端とする．
			l = m1;

			// 右の内分点を新たに左の内分点とする．
			m1 = m2;
			v1 = v2;

			// 右の内分点を新たに計算する．
			m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);
			v2 = f(m2);
		}
	}

	// 最後の候補を比較し，大きかった方の x を返す．
	return (v1 > v2) ? m1 : m2;

	/* f の定義の雛形
	auto f = [&](double x) {
		return x;
	};
	*/
}


//【黄金分割探索（実数，最小値）】O(log((r - l) / EPS))
/*
* 全域で下に単峰な関数 f(x) の開区間 (l..r) における最小値を与える x を返す．
*/
template <class D, class FUNC>
D golden_search_min(D l, D r, const FUNC& f, D EPS = 1e-12) {
	// verify : https://atcoder.jp/contests/arc049/tasks/arc049_b

	constexpr D phi = 1.618033988749895; // 黄金数

	int L = max((int)(log((r - l) / EPS) / log(phi)), 1);

	// l, m1, m2, r の順で区間を φ : 1 : φ に内分する点
	D m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
	D m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);

	// 内分点における関数値の計算
	D v1 = f(m1);
	D v2 = f(m2);

	// 絶対誤差か相対誤差が EPS 以下になるまで
	rep(hoge, L) {
		// 左の内分点での値の方が小さければ，次の区間は左側をとる．
		if (v1 < v2) {
			// 右の内分点を新たに右端とする．
			r = m2;

			// 左の内分点を新たに右の内分点とする．
			m2 = m1;
			v2 = v1;

			// 左の内分点を新たに計算する．
			m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
			v1 = f(m1);
		}
		// 右の内分点での値の方が小さければ，次の区間は右側をとる．
		else {
			// 左の内分点を新たに左端とする．
			l = m1;

			// 右の内分点を新たに左の内分点とする．
			m1 = m2;
			v1 = v2;

			// 右の内分点を新たに計算する．
			m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);
			v2 = f(m2);
		}
	}

	// 最後の候補を比較し，小さかった方の x を返す．
	return (v1 < v2) ? m1 : m2;

	/* f の定義の雛形
	auto f = [&](double x) {
		return x;
	};
	*/
}


//【ランダム三分探索（実数，最小値）】O(log((r - l) / EPS))
/*
* 全域で下に単峰な関数 f(x) の開区間 (l..r) における最小値を与える x を返す．
* 下に単峰じゃなくても運が良ければ正しい x を返す．
*/
template <class D, class FUNC>
D random_ternary_search_min(D l, D r, const FUNC& f, D EPS = 1e-12) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_f

	static bool first_call = true;

	static mt19937 mt;
	static uniform_real_distribution<D> rnd(0, 1);
	if (first_call) {
		mt.seed((int)time(NULL));
		first_call = false;
	}

	D m1 = l, m2 = r;

	// 絶対誤差か相対誤差が EPS 以下になるまで
	while (r - l > EPS && r - l > EPS * (r + l) / 2) {
		m1 = l + (r - l) * rnd(mt);
		m2 = l + (r - l) * rnd(mt);
		if (m1 > m2) swap(m1, m2);

		// 左の内分点での値の方が小さければ，次の区間は左側をとる．
		if (f(m1) < f(m2)) {
			r = m2;
		}
		// 右の内分点での値の方が小さければ，次の区間は右側をとる．
		else {
			l = m1;
		}
	}

	// 最後の候補を比較し，小さかった方の x を返す．
	return (f(m1) < f(m2)) ? m1 : m2;

	/* f の定義の雛形
	auto f = [&](double x) {
		return x;
	};
	*/
}


//【並列二分探索】O(O(okQ) log max|ok[i] - ng[i]|)
/*
* i=[0..q) について，条件を満たす要素 ok[i] と満たさない要素 ng[i] の
* 境界を二分探索し，ok[i][ng[i]] を境界に接する OK[NG] 側の要素に変更する．
* okQ は，okQ(mid, res) で呼び出すと mid[i] が条件を満たすかが res[i] に格納されるとする．
*/
void parallel_binary_search(vi& ok, vi& ng, const function<void(const vi&, vb&)>& okQ) {
	// 参考 : https://betrue12.hateblo.jp/entry/2019/08/14/152227
	// verify : https://yukicoder.me/problems/no/1982

	//【使い所】
	// 解が単調性を持っていることが分かっているが，ランダムアクセスができず，
	// 愚直に二分探索を繰り返すと O(N Q) がかかってしまう場合．（Union-Find など）
	// どうせ線形探索に O(N) かかるのなら Q 個のクエリをまとめて処理できるので，
	// 線形探索の回数を O(log Q) に抑えることで全体 O(N log Q) を実現する．
	//
	// 永続データ構造の代案として使えたりする．
	
	int q = sz(ok); // クエリの数

	vi mid(q); vb res(q);

	while (true) {
		bool update = false; // 更新が起こったか

		// それぞれのクエリの ok と ng の中央値を mid に格納する．
		rep(i, q) {
			if (abs(ok[i] - ng[i]) <= 1) continue;
			update = true;

			mid[i] = (ok[i] + ng[i]) / 2;
		}

		// 更新が起こらなかったなら探索終了
		if (!update) break;

		// mid に対して一括で ok か ng かを判定する．
		okQ(mid, res);

		// 判定結果に応じて ok または ng を更新する．
		rep(i, q) {
			if (res[i]) ok[i] = mid[i];
			else ng[i] = mid[i];
		}		
	}

	/* okQ の定義の雛形
	function<void(const vi&, vb&)> okQ = [&](const vi& mid, vb& res) {
		// mid の値ごとに処理するため，mid → id を作る．
		vvi mid_to_id(T);
		rep(id, q) mid_to_id[mid[id]].push_back(id);

		// 必要なデータ構造の準備をここに書く：


		// シミュレーションを行う
		rep(t, T) {
			// 時刻 t での処理をここに書く：


			// mid = t のものに対する判定
			repe(id, mid_to_id[t]) res[id] = (seg.get(I[id]) >= x[id]);
		}
	};
	*/
}


//【二分探索（最小値固定）】O(n (log n)^2)
/*
* 与えられた列 a[0..n) に対し，各 m∈[0..n) について，m を含む区間 [l..r) で
* argmin a[l..r) = m（最左優先）かつ is_ok(m, l, r) = true となるものの個数のリストを返す．
* max_flag = true とすると最大値固定とする．
*
* 制約：is_ok(m,・,・) は単調
*/
template <class T, class FUNC>
vl bin_search_fixed_min(const vector<T>& a, const FUNC& okQ, bool max_flag = false) {
	// verify : https://atcoder.jp/contests/abc282/tasks/abc282_h

	int n = sz(a);

	// prv[M] : a[M] 以下の数の前の位置（なければ -1）
	// nxt[M] : a[M] より真に大きい数の次の位置（なければ n）
	vi prv(n, -1), nxt(n, n);

	if (!max_flag) {
		// 自身以下の数の前の位置を一括計算する．
		stack<pair<int, T>> st;
		repir(i, n - 1, 0) {
			while (!st.empty() && st.top().second >= a[i]) {
				prv[st.top().first] = i;
				st.pop();
			}
			st.push({ i, a[i] });
		}

		// 自身より真に小さい数の次の位置を一括計算する．
		stack<pair<int, T>> st2;
		rep(i, n) {
			while (!st2.empty() && st2.top().second > a[i]) {
				nxt[st2.top().first] = i;
				st2.pop();
			}
			st2.push({ i, a[i] });
		}
	}
	else {
		// 自身以上の数の前の位置を一括計算する．
		stack<pair<int, T>> st;
		repir(i, n - 1, 0) {
			while (!st.empty() && st.top().second <= a[i]) {
				prv[st.top().first] = i;
				st.pop();
			}
			st.push({ i, a[i] });
		}

		// 自身より真に大きい数の次の位置を一括計算する．
		stack<pair<int, T>> st2;
		rep(i, n) {
			while (!st2.empty() && st2.top().second < a[i]) {
				nxt[st2.top().first] = i;
				st2.pop();
			}
			st2.push({ i, a[i] });
		}
	}

	vl res(n);

	rep(M, n) {
		// 最小値が a[M] である極大半開区間 [L..M..R) をとる．
		int L = prv[M] + 1;
		int R = nxt[M];

		// M の左側と右側とで要素の少ない方を決め打ちする（マージテクと同じ計算量になる）
		if (M - L + 1 < R - M) {
			repi(l, L, M) {
				int r_ok = M, r_ng = R;
				if (okQ(M, l, r_ng)) swap(r_ok, r_ng); // これが誤りならどうせ答えは 0

				while (abs(r_ok - r_ng) > 1) {
					int r_mid = (r_ok + r_ng) / 2;

					if (okQ(M, l, r_mid)) r_ok = r_mid;
					else r_ng = r_mid;
				}
				res[M] += r_ok < r_ng ? r_ok - M : R - r_ok + 1;
			}
		}
		else {
			repi(r, M + 1, R) {
				int l_ok = M + 1, l_ng = L;
				if (okQ(M, l_ng, r)) swap(l_ok, l_ng); // これが誤りならどうせ答えは 0

				while (abs(l_ok - l_ng) > 1) {
					int l_mid = (l_ok + l_ng) / 2;

					if (okQ(M, l_mid, r)) l_ok = l_mid;
					else l_ng = l_mid;
				}
				res[M] += l_ok > l_ng ? M - l_ok + 1 : l_ok - L + 1;
			}
		}
	}

	return res;

	/* okQ の定義の雛形
	auto okQ = [&](int M, int L, int R) {
		return true;
	};
	*/
}


//【幅優先探索（動的）】O(n + m)（遅い）
/*
* st から到達可能な頂点 t のリストを返す．nxt(s) は s の次に訪れることのできる頂点のリストを返す．
* 探索は lim [ms] だけ続ける．
*/
template <class T>
set<T> get_reachable_set(T st, const function<vector<T>(T)>& nxt, int lim = (int)1e9) {
	// verify : https://atcoder.jp/contests/agc045/tasks/agc045_c

	auto start = chrono::system_clock::now();

	set<T> vs; // st から到達可能な頂点のリスト
	vs.insert(st);

	queue<T> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点 s を得る．
		auto s = que.front(); que.pop();

		repe(t, nxt(s)) {
			// t が発見済みの頂点なら何もしない．
			if (vs.count(t)) continue;

			// t に到達したことを記録する．
			vs.insert(t);

			// 未探索の頂点として t を追加する．
			que.push(t);
		}

		auto now = chrono::system_clock::now();
		auto msec = chrono::duration_cast<chrono::milliseconds>(now - start).count();
		if (msec >= lim) break;
	}

	return vs;

	/* nxt の定義の雛形
	using T = ll;
	function<vector<T>(T)> nxt = [&](T s) {
		vector<T> res;

		return res;
	};
	*/
}


//【幅優先探索（動的，ハッシュ）】O(n + m)（遅い）
/*
* st から到達可能な頂点 t のリストを返す．nxt(s) は s の次に訪れることのできる頂点のリストを返す．
* HASH はハッシュ関数 size_t operator()(const T& p) の定義された関数オブジェクトとする．
* 探索は lim [ms] だけ続ける．
*/
template <class T, class HASH>
unordered_set<T, HASH> get_reachable_set_hashed(T st, const function<vector<T>(T)>& nxt, int lim = (int)1e9) {
	auto start = chrono::system_clock::now();

	unordered_set<T, HASH> vs; // st から到達可能な頂点のリスト
	vs.insert(st);

	queue<T> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点 s を得る．
		auto s = que.front(); que.pop();

		repe(t, nxt(s)) {
			// t が発見済みの頂点なら何もしない．
			if (vs.count(t)) continue;

			// t に到達したことを記録する．
			vs.insert(t);

			// 未探索の頂点として t を追加する．
			que.push(t);
		}

		auto now = chrono::system_clock::now();
		auto msec = chrono::duration_cast<chrono::milliseconds>(now - start).count();
		if (msec >= lim) break;
	}

	return vs;

	/* nxt の定義の雛形
	using T = ll;
	function<vector<T>(T)> nxt = [&](T s) {
		vector<T> res;

		return res;
	};
	*/
}


//【山登り法】O(?)（遅い）
/*
* 初期解 st から始めて，スコアが改善される限り近傍への移動を続け，到達した局所解を返す．
* neib(s) は解 s の近傍解のリストを返す．calc(s) は解 s のスコアを返す．
*/
template <class T>
T hill_climbing(T st, const function<vector<T>(T)>& neib, const function<ll(T)>& calc) {
	ll sc = calc(st);

	while (true) {
		ll sc_max(sc);

		repe(nst, neib(st)) {
			ll nsc = calc(nst);

			if (chmax(sc_max, nsc)) st = nst;
		}

		if (sc_max == sc) break;
		sc = sc_max;
	}

	return st;

	/* neib と calc の定義の雛形
	using T = vl;
	function<vector<T>(T)> neib = [&](T s) {
		vector<T> res;

		return res;
	};
	function<ll(T)> calc = [&](T s) {
		ll sc = 0;

		return sc;
	};
	*/
}


//【最適化問題 → 判定問題】
/*
*	(最適化問題) 条件 P(x) を満たすような最小の x を求めよ
* について，P(x) を満たす x の範囲が [lb..∞) の形であるとする．また，
*	(判定問題) x が条件 P(x) を満たすか判定せよ
* であれば O(α) で解けるとする．
*
* このとき，条件を満たすか否かの境界を二分探索で求めることにより，
* 元の (最適化問題) を O(α log(∞-lb)) で解くことができる．
*
* verify : https://atcoder.jp/contests/typical90/tasks/typical90_a
*/


//【最大値の最小化】
/*
*	(最適化問題)「hogehoge な n 個の値の最大値」の最小値を求めよ
* に対して【最適化問題 → 判定問題】を適用すると，
*	(判定問題) hogehoge な n 個の値の最大値を x 以下にできるか判定せよ
* に帰着する．これは
*	(判定問題 2) hogehoge な n 個の値全てを x 以下にできるか判定せよ
* と等価であり，n 個の値を独立に考えられ扱いやすくなる．
*
* verify : https://atcoder.jp/contests/typical90/tasks/typical90_a
*/


//【昇順 k 番目の要素】
/*
*	(求値問題) 昇順で k 番目（1-indexed）の hogehoge な要素を求めよ
* の答えは，
*	(判定問題) 昇順で k 番目（1-indexed）の hogehoge な要素は x 以下か
* を導入すると，
*	(最適化問題) 先の判定問題の答えを yes とする最小の x を求めよ
* と等価となる．さらに (判定問題) は
*	(判定問題 2) x 以下の hogehoge な要素は k 個以上か
* と等価であるから，hogehoge な要素を x 以下か否かで 0-1 変数化することができる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2270
*/


//【濃度の最大化】O(n log((max(p) - min(p)) / EPS))
/*
* 質量 a[0..n) で濃度が p[0..n) の液体を k 個混ぜ合わせたときの最大濃度を返す．
*
* 利用：【二分探索（実数）】
*/
template <class D>
D maximize_concentration(const vector<D>& a, const vector<D>& p, int k, double EPS = 1e-12) {
	// verify : https://atcoder.jp/contests/abc034/tasks/abc034_d

	//【方法】
	// 目標とする濃度 c を決め打ちして二分探索すると，それが達成できるということは
	//		(Σi a[i] p[i]) / (Σi a[i]) ≧ c
	//		⇔ (Σi a[i] p[i]) ≧ c (Σi a[i])
	//		⇔ (Σi a[i] p[i]) - (Σi c a[i]) ≧ 0
	//		⇔ Σi a[i] (p[i] - c) ≧ 0
	// と同値変形できる．
	// 
	// したがって，左辺の値を最大化し，0 以上となるかを考えれば良い．
	// a[i] (p[i] - c) の値を降順に並べて始め k 個の和を取れば左辺を最大化できる．

	int n = sz(a);

	// 濃度 c が達成できるか
	function<bool(D)> okQ = [&](D c) {
		// d[i] : i 番目の液体の溶質の量と濃度 c だったときとの差
		vector<D> d(n);
		rep(i, n) d[i] = a[i] * (p[i] - c);

		// 溶質の量が濃度 c に比べて多い順に並べる．
		sort(all(d), greater<D>());

		// 貪欲に k 個の液体を選ぶ．
		D sum = accumulate(d.begin(), d.begin() + k, D(0));

		// 溶質が足りているなら濃度 c を達成可能．
		return sum >= 0;
	};

	// 達成可能か不可能かの境目となる濃度を得る．
	D p_min = *min_element(all(p));
	D p_max = *max_element(all(p));
	D res = bin_search(p_min, p_max, okQ, EPS);

	return res;
}


//【互いに異なるカードの選択】O(n log n)
/*
* n 種のカードが各 c[i] 枚ある．互いに異なる k 枚のカードを最大何回抽出できるかを返す．
*
* 利用：【めぐる式二分探索】
*/
ll all_different_select(vl c, ll k) {
	// verify : https://atcoder.jp/contests/abc227/tasks/abc227_d

	int n = sz(c);

	// 枚数昇順にソートする．
	sort(all(c));

	// 累積枚数
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + c[i];

	// x 回抽出できるか
	function<bool(ll)> okQ = [&](ll x) {
		// x 枚以上あるカードについては，そのうち x 枚しか使えないので，
		// そのようなカードの種類数 * x で枚数が求まる．
		// それ以外については全て使えるので下からの累積和で枚数が求まる．
		int i = lbpos(c, x);
		ll sel = acc[i] + (n - i) * x;

		// 使えるカードの枚数 sel が使いたいカードの枚数 k * x 以上なら抽出可能．
		return sel / k >= x;
	};

	ll res = meguru_search(0LL, accumulate(all(c), 0LL) / k + 1, okQ);

	return res;
}


