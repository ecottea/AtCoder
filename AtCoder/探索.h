#pragma once
#include "header.h"
// ■■■■■ 探索 ■■■■■


//【めぐる式二分探索】O(log|ok - ng|)
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
* 境界に隣り合うような条件を満たす要素（ok 側）の位置を返す．
*/
template <class T>
T meguru_search(T ok, T ng, const function<bool(T)>& okQ) {
	// 参考 : https://twitter.com/meguru_comp/status/697008509376835584
	// verify : https://atcoder.jp/contests/abc023/tasks/abc023_d

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
	using T = ll;
	function<bool(T)> okQ = [&](T x) {
		return true || false;
	};
	*/
}


//【二分探索（実数）】O(log(|ok - ng| / EPS))
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
*/
template <class T>
T binary_search(T ok, T ng, const function<bool(T)>& okQ) {
	// verify : https://atcoder.jp/contests/abc189/tasks/abc189_f

	// 誤差 EPS で境界が決定するまで
	while (true) {
		// 区間の中間
		T mid = (ok + ng) / 2;
		//double mid = sqrt(ok * ng); // 相対誤差を小さくする場合

		// 絶対誤差か相対誤差が EPS 以下なら終了する．
		T err = abs(ok - ng);
		if (err <= EPS || err <= abs(mid) * EPS) {
			break;
		}

		// 中間が OK かどうかに応じて区間を縮小する．
		if (okQ(mid)) ok = mid;
		else ng = mid;
	}
	return (ok + ng) / 2;

	/* okQ の定義の雛形
	function<bool(double)> okQ = [&](double x) {
		return true || false;
	};
	*/
}


//【三分探索（上に凸）】O(log(r - l))
/*
* 階差の符号変化が + → 0 → - である関数 f(x) の開区間 (l, r) における最大値を与える x を返す．
*/
template <class T>
ll ternary_search_uc(ll l, ll r, const function<T(ll)>& f) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_f

	while (r - l > 2) {
		ll s = l + r;
		ll m1 = s / 2 - (s % 2 < 0);
		ll m2 = m1 + 1;

		if (f(m1) < f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	function<ll(ll)> f = [&](ll x) {
		return x;
	};
	*/
}


//【三分探索（下に凸）】O(log(r - l))
/*
* 階差の符号変化が - → 0 → + である関数 f(x) の開区間 (l, r) における最小値を与える x を返す．
*/
template <class T>
ll ternary_search_lc(ll l, ll r, const function<T(ll)>& f) {
	// verify : https://atcoder.jp/contests/abc279/tasks/abc279_d

	while (r - l > 2) {
		ll s = l + r;
		ll m1 = s / 2 - (s % 2 < 0);
		ll m2 = m1 + 1;

		if (f(m1) > f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	function<ll(ll)> f = [&](ll x) {
		return x;
	};
	*/
}


//【ランダム三分探索（下に凸）】O(log(r - l))
/*
* 階差の符号変化が - → 0 → + である関数 f(x) の開区間 (l, r) における最小値を与える x を返す．
* そうでなくても運が良ければ正しい x を返す．
*/
template <class T>
ll random_ternary_search_lc(ll l, ll r, const function<T(ll)>& f) {
	static bool first_call = true;

	mt19937 mt;
	uniform_int_distribution<ll> rnd;
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

		if (f(m1) > f(m2)) l = m1;
		else r = m2;
	}
	return l + 1;

	/* f の定義の雛形
	function<ll(ll)> f = [&](ll x) {
		return x;
	};
	*/
}


//【フィボナッチ探索】
/*
* Fibonacci_search(ll w) : O(log w)
*	最大で幅 w の開区間まで扱えるよう初期化する．
*
* ll search(ll left, ll right, function<ll(ll)> f, bool up = true) : O(log(right - left))
*	関数 f(i) の開区間 (left, right) における最大[小]値を与える i を返す．
*	up = true なら f の階差の符号変化は + → 0 → - で，返すのは最大値となる．
*	up = false なら f の階差の符号変化は - → 0 → + で，返すのは最小値となる．
*/
struct Fibonacci_search {
	int n;
	vl fib;

	Fibonacci_search(ll w) : n(1), fib({ 1, 1 }) {
		// 利用する範囲のフィボナッチ数列を準備する．
		while (fib[n] < w) {
			fib.push_back(fib[n] + fib[n - 1]);
			n++;
		}
	}

	ll search(ll left, ll right, const function<ll(ll)>& f_, bool up = true) const {
		function<ll(ll)> f = [&](ll x) {
			// 符号変化の条件を満たすよう範囲外の値を定めておく．
			ll val;
			if (x <= left) {
				val = -INFL - (left - x);
			}
			else if (x >= right) {
				// たぶん大丈夫だけどオーバーフローに注意
				val = -INFL - (x - right);
			}
			else {
				val = (up ? f_(x) : -f_(x));
			}

			return val;
		};

		// l, m1, m2, r の順で区間を φ: 1 :φ に内分する点を得る．
		int i = n;
		ll l = left;
		ll r = l + fib[i];
		ll m1 = l + fib[i - 2];
		ll m2 = l + fib[i - 1];
		i -= 3;

		// 内分点における関数値の計算
		ll v1 = f(m1);
		ll v2 = f(m2);

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
				m2 = r - fib[i];
				v2 = f(m2);
			}
			i--;
		}

		// 最後の候補を比較し，大きかった方の番号を返す．
		return (v1 > v2) ? m1 : m2;
	}
};


//【黄金分割探索（実数，上に凸）】O(log((right - left) / EPS))
/*
* 全域で狭義に上に凸な関数 f(x) の開区間 (left, right) における最大値を与える x を返す．
*/
double golden_search_uc(double left, double right, const function<double(double)>& f) {
	const double phi = (1 + sqrt(5)) / 2;

	// l, m1, m2, r の順で区間を φ: 1 :φ に内分する点
	double l = left;
	double r = right;
	double m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
	double m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);

	// 内分点における関数値の計算
	double v1 = f(m1);
	double v2 = f(m2);

	// 絶対誤差か相対誤差が EPS 以下になるまで
	while (r - l > EPS && r - l > EPS * (r + l) / 2) {
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
	function<double(double)> f = [&](double x) {
		return x;
	};
	*/
}


//【黄金分割探索（実数，下に凸）】O(log((right - left) / EPS))
/*
* 全域で狭義に下に凸な関数 f(x) の開区間 (left, right) における最小値を与える x を返す．
*/
double golden_search_lc(double left, double right, const function<double(double)>& f) {
	// verify : https://atcoder.jp/contests/arc049/tasks/arc049_b
	
	const double phi = (1 + sqrt(5)) / 2;

	// l, m1, m2, r の順で区間を φ: 1 :φ に内分する点
	double l = left;
	double r = right;
	double m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
	double m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);

	// 内分点における関数値の計算
	double v1 = f(m1);
	double v2 = f(m2);

	// 絶対誤差か相対誤差が EPS 以下になるまで
	while (r - l > EPS && r - l > EPS * (r + l) / 2) {
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
	function<double(double)> f = [&](double x) {
		return x;
	};
	*/
}


//【ランダム三分探索（実数，下に凸）】O(log((r - l) / EPS))
/*
* 全域で狭義に下に凸な関数 f(x) の開区間 (l, r) における最小値を与える x を返す．
* 下に凸じゃなくても運が良ければ正しい x を返す．
*/
double random_ternary_search_lc(double l, double r, const function<double(double)>& f) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_f

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_real_distribution<> rnd(0, 1);

	double m1 = l, m2 = r;

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
	function<double(double)> f = [&](double x) {
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
		vvi mid_to_id(k);
		rep(id, q) mid_to_id[mid[id]].push_back(id);

		// 必要なデータ構造の準備をここに書く：


		// シミュレーションを行う
		rep(t, k) {
			// 時刻 t での処理をここに書く：


			// mid = t のものに対する判定
			repe(id, mid_to_id[t]) res[id] = (seg.get(I[id]) >= x[id]);
		}
	};
	*/
}


//【幅優先探索（動的）】O(|V| + |E|)（遅い）
/*
* st から到達可能な頂点 t のリストを返す．nxt(s) は s の次に訪れることのできる頂点のリストを返す．
*/
template <class T>
set<T> get_reachable_set(T st, const function<vector<T>(T)>& nxt) {
	// verify : https://atcoder.jp/contests/agc045/tasks/agc045_c

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
*	(最適化問題) 先の判定問題の答えを真とする最小の x を求めよ
* と等価となる．さらに (判定問題) は
*	(判定問題 2) x 以下の hogehoge な要素は k 個以上か
* と等価であるから，hogehoge な要素を x 以下か否かで 0-1 変数化することができる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2270
*/


//【濃度の最大化】O(n log(1/EPS))
/*
* 質量 a[0..n) で濃度が p[0..n) の液体を k 個混ぜ合わせたときの最大濃度を返す．
*
* 利用：【二分探索（実数）】
*/
double maximize_concentration(const vd& a, const vd& p, int k) {
	//verify : https://atcoder.jp/contests/abc034/tasks/abc034_d

	//【方法】
	// 目標とする濃度 c を決め打ちして二分探索すると，それが達成できるということは
	//		(Σi a[i] p[i]) / (Σi a[i]) >= c
	//		⇔ (Σi a[i] p[i]) >= c (Σi a[i])
	//		⇔ (Σi a[i] p[i]) - (Σi c a[i]) >= 0
	//		⇔ Σi a[i] (p[i] - c) >= 0
	// と同値変形できる．
	// 
	// したがって，左辺の値を最大化し，0 以上となるかを考えれば良い．
	// a[i] (p[i] - c) の値を降順に並べて始め k 個の和を取れば左辺を最大化できる．

	int n = sz(a);

	// 濃度 c が達成できるか
	function<bool(double)> okQ = [&](double c) {
		// d[i] : i 番目の液体の溶質の量と濃度 c だったときとの差
		vd d(n);

		rep(i, n) d[i] = a[i] * (p[i] - c);

		// 溶質の量が濃度 c に比べて多い順に並べる．
		sort(all(d), greater<double>());

		// 貪欲に k 個の液体を選ぶ．
		double sum = accumulate(d.begin(), d.begin() + k, 0.);

		// 溶質が足りているなら濃度 c を達成可能．
		return sum >= 0;
	};

	// 達成可能か不可能かの境目となる濃度を得る．
	double res = binary_search(0., 100., okQ);

	return res;
}


//【互いに異なるカードの選択】O(n log n)
/*
* n 種のカードが各 c[i] 枚ある．互いに異なる k 枚のカードを最大何回抽出できるかを返す．
*
* 利用：【めぐる式二分探索】
*/
ll all_different_select(vl c, ll k) {
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


