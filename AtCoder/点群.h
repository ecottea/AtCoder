#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "分析(文字列).h"
#include "順列.h"
#include "探索.h"
#include "作図.h"
// ■■■■■ 点群，線分群，直線群など ■■■■■


//【点群の合同判定】O(|s| log|s|)　
/*
* 点の集合 s, t が同じ向きで合同かどうかを返す．
*
* 利用：【偏角ソート】,【部分文字列判定】
*/
template <typename T>
bool congruenceQ(vector<Point<T>> s, vector<Point<T>> t) {
	// verify : https://atcoder.jp/contests/abc207/tasks/abc207_d

	int n = sz(s);

	// 点の数が違うならもちろん合同ではない．
	if (sz(t) != n) {
		return false;
	}

	// 原点中心に n 倍拡大した上で重心を求める．
	Point<T> gs, gt;
	rep(i, n) {
		gs += s[i];
		s[i] *= n;

		gt += t[i];
		t[i] *= n;
	}

	// 重心が原点にくるよう平行移動を行う．
	rep(i, n) {
		s[i] -= gs;
		t[i] -= gt;
	}

	// 原点の周りで偏角ソートを行う．
	Point<T> o;
	argument_sort(s, o);
	argument_sort(t, o);

	// 原点に点が存在した場合の例外処理
	// o は便宜上偏角最大としていたので，存在するなら配列の末尾にある．
	if (*s.rbegin() == o) {
		if (*t.rbegin() == o) {
			// 両者が原点を含んでいたなら，それらを取り除く．
			s.pop_back();
			t.pop_back();
			n--;
		}
		else {
			// s のみが原点を含んでいたなら一致しない．
			return false;
		}
	}
	else {
		if (*t.rbegin() == o) {
			// t のみが原点を含んでいたなら一致しない．
			return false;
		}
	}

	// 1 点のみだった場合の例外処理（もちろん合同）
	if (s.empty()) {
		return true;
	}

	// 原点回りの三角形の辺の長さと夾角の大きさの情報を格納したリスト
	// 実際には長さの代わりに 2 乗ノルム，夾角の大きさの代わりに余弦と正弦，
	// さらにその代わりに内積と外積を用いる．
	// （内積だけだと，例えば対称移動させた直角三角形で WA してしまう．）
	vector<ll> tri_s, tri_t;
	s.push_back(s[0]);
	t.push_back(t[0]);
	rep(i, n) {
		tri_s.push_back(s[i].sqnorm());
		tri_s.push_back(s[i].dot(s[i + 1]));
		tri_s.push_back(s[i].cross(s[i + 1]));

		tri_t.push_back(t[i].sqnorm());
		tri_t.push_back(t[i].dot(t[i + 1]));
		tri_t.push_back(t[i].cross(t[i + 1]));
	}

	// tri_t をコピーし 2 倍に延長することで周期境界を扱いやすくする．
	rep(i, 3 * n) {
		tri_t.push_back(tri_t[i]);
	}

	// tri_s を 2 * tri_t が連続部分列として含んでいるかをチェックする．
	int res = knuth_morris_pratt(tri_t, tri_s);

	return res >= 0;
}


//【点群の相似判定】O(|s| log|s|)
/*
* 点の集合 s, t が同じ向きで相似であれば，s から t への倍率を返す（相似でなければ -1 を返す）
*
* 利用：【偏角ソート】,【部分文字列判定】
*/
template <typename T> double similarityQ(vector<Point<T>> s, vector<Point<T>> t) {
	int n = sz(s);

	// 点の数が違うならもちろん相似ではない．
	if (sz(t) != n) {
		return false;
	}

	// 原点中心に n 倍拡大した上で重心を求める．
	Point<T> gs, gt;
	rep(i, n) {
		gs += s[i];
		s[i] *= n;

		gt += t[i];
		t[i] *= n;
	}

	// 重心が原点にくるよう平行移動を行う．
	rep(i, n) {
		s[i] -= gs;
		t[i] -= gt;
	}

	// 原点の周りで偏角ソートを行う．
	Point<T> o;
	argument_sort(s, o);
	argument_sort(t, o);

	// 原点に点が存在した場合の例外処理
	// o は便宜上偏角最大としていたので，存在するなら配列の末尾にある．
	if (*s.rbegin() == o) {
		if (*t.rbegin() == o) {
			// 両者が原点を含んでいたなら，それらを取り除く．
			s.pop_back();
			t.pop_back();
			n--;
		}
		else {
			// s のみが原点を含んでいたなら相似ではない．
			return -1;
		}
	}
	else {
		if (*t.rbegin() == o) {
			// t のみが原点を含んでいたなら相似ではない．
			return -1;
		}
	}

	// 1 点のみだった場合の例外処理（もちろん相似）
	if (s.empty()) {
		return 1;
	}

	// 原点回りの夾角の大きさの情報を格納したリスト
	// 実際には夾角の大きさの代わりに余弦と正弦，さらにその代わりに内積と外積の比を用いる．
	vl tri_s, tri_t;
	s.push_back(s[0]);
	t.push_back(t[0]);
	rep(i, n) {
		ll in_s = s[i].dot(s[i + 1]);
		ll out_s = s[i].cross(s[i + 1]);
		ll g_s = gcd(in_s, out_s);
		tri_s.push_back(in_s / g_s);
		tri_s.push_back(out_s / g_s);

		ll in_t = t[i].dot(t[i + 1]);
		ll out_t = t[i].cross(t[i + 1]);
		ll g_t = gcd(in_t, out_t);
		tri_t.push_back(in_t / g_t);
		tri_t.push_back(out_t / g_t);
	}

	// tri_t をコピーし 2 倍に延長することで周期境界を扱いやすくする．
	rep(i, 2 * n) {
		tri_t.push_back(tri_t[i]);
	}

	// 2 * tri_t が tri_s を連続部分列として含んでいるかをチェックする．
	vi pos;
	knuth_morris_pratt(tri_t, tri_s, pos);

	double res = -1;
	if (sz(pos) != 0) {
		int i = pos[0] / 2;
		res = t[i].norm() / s[0].norm();
	}

	return res;
}


//【凸包】O(n log n)
/*
* 点群 p[0..n) の凸包の頂点を反時計回りに ch に格納する．
* p[0] は x 座標最小（同じものがあれば y 座標最小）の点とする．
*/
template <typename T> void convex_hull(vector<Point<T>> p, Polygon<T>& ch) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_A

	int n = sz(p);

	// x 座標を優先して昇順ソート（x 座標が同じなら y 座標昇順）
	sort(all(p));

	// 凸包を成す頂点
	ch.clear();

	// まず x 座標昇順に見ていき，凸包の y 座標の小さい側を得る．
	int pt = 0;
	rep(i, n) {
		// 凸でない限り直前の点を除去することを繰り返す．
		// 凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch[pt - 1] - ch[pt - 2]).cross(p[i] - ch[pt - 2]) < 0) {
			ch.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch.push_back(p[i]);
		pt++;
	}

	// 次に x 座標降順に見ていき，凸包の y 座標の大きい側を得る．
	repir(i, n - 2, 0) {
		// 凸でない限り直前の点を除去することを繰り返す．
		// 凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch[pt - 1] - ch[pt - 2]).cross(p[i] - ch[pt - 2]) < 0) {
			ch.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch.push_back(p[i]);
		pt++;
	}

	// p[0] が重複してしまっているので取り除く．
	ch.pop_back();
}


//【上からの凸包】O(n log n)
/*
* n 個の点 (x[i], y[i]) の上からの凸包の x 座標について昇順で j 番目の頂点の座標を p[j] に格納する．
* また p の大きさを返す．strict = false とすると広義凸包，upper = false とすると下からの凸包を格納する．
*/
template <class T>
int upper_convex_hull(const vector<T>& x, const vector<T>& y, vector<pair<T, T>>& p,
	bool strict = false, bool upper = true)
{
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_A

	int n = sz(x);
	p.clear();

	// x 座標について昇順に並べる．
	map<T, T> x_to_y;
	rep(i, n) {
		if (x_to_y.count(x[i])) {
			if (upper) chmax(x_to_y[x[i]], y[i]);
			else chmin(x_to_y[x[i]], y[i]);
		}
		else x_to_y[x[i]] = y[i];
	}

	// 3 点 P, Q, R のの部分が上に凸かを返す．
	//		P, Q, R 部分が上に凸
	//		⇔ 直線 PQ の傾き > 直線 PR の傾き
	//		⇔ (Q[y] - P[y]) / (Q[x] - P[x]) > (R[y] - P[y]) / (R[x] - P[x])
	//		⇔ (Q[y] - P[y]) (R[x] - P[x]) > (R[y] - P[y]) (Q[x] - P[x])
	using PT = pair<T, T>;
	function<bool(const PT&, const PT&, const PT&)> convexQ = [&](const PT& P, const PT& Q, const PT& R) {
		T left = (Q.second - P.second) * (R.first - P.first);
		T right = (R.second - P.second) * (Q.first - P.first);

		if (!strict && left == right) return true;

		return upper ? left > right : left < right;
	};

	int pt = 0;
	repe(xy, x_to_y) {
		// 凸包の直前の 2 点を P, Q，今見ている点を R とし，
		// P, Q, R 部分が上に凸でない限り Q を除去することを繰り返す．
		while (pt >= 2 && !convexQ(p[pt - 2], p[pt - 1], xy)) {
			p.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		p.push_back(xy);
		pt++;
	}

	return pt;
}


//【線分群の交点（軸平行）】O(n log n)
/*
* 2 点 (x1[i], y1[i]), (x2[i], y2[i]) を結ぶ n 本の閉線分の共有点の数を返す．
*
* 制約：互いに平行な線分同士が共有点をもつことはない．
*/
ll count_intersection(const vl& x1, const vl& y1, const vl& x2, const vl& y2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_6_A

	int n = sz(x1);

	// 各線分が x1 <= x2, y1 <= y2 を満たすようにしつつ，x1 ごとに昇順に記録する．
	// 線分が x 軸平行のとき type = 0，y 軸平行のとき type = 1 とし，
	// 同じ x1 をもつ線分同士については type 0 が先，type 1 が後に並ぶようにする．
	map<ll, set<tuple<int, ll, ll>>> seg;
	rep(i, n) {
		if (x1[i] != x2[i]) {
			seg[min(x1[i], x2[i])].insert({ 0, max(x1[i], x2[i]), y2[i] });
		}
		else {
			seg[x1[i]].insert({ 1, min(y1[i], y2[i]), max(y1[i], y2[i]) });
		}
	}

	ll res = 0;

	// 走査中の x 座標と交点をもつような x 軸平行な線分の y 座標を記録するテーブル．
	// 二分探索を利用するため vector でなく set を用いている．
	set<ll> sx;

	// x 軸平行な線分の終端まで走査したらそれをテーブルから取り除くために，
	// 終端の x 座標について昇順に取り出すことのできる優先度付きキュー．
	priority_queue_rev<pll> q;

	// x 座標について昇順に走査していく．
	// 実際に幅の分だけ走査するのではなく，x1 の値となっているものだけを走査する．
	repe(p, seg) {
		// x 軸平行な線分のうち x2 が走査位置の手前であるものをテーブルから取り除く．
		while (!q.empty() && (q.top().first < p.first)) {
			sx.erase(q.top().second);
			q.pop();
		}

		// x1 が同じものを順に見ていく．
		// まずは type 0 が処理され，その後 type 1 が処理される．
		repe(s, p.second) {
			int type; ll x2_or_y1, y2;
			tie(type, x2_or_y1, y2) = s;

			// x 軸平行な線分の場合
			if (type == 0) {
				// 走査中の x 軸平行な線分のテーブルに追加する．
				sx.insert(y2);

				// 終端まで走査したときにテーブルから削除するためのキューに登録する．
				q.push({ x2_or_y1, y2 });
			}
			// y 軸平行な線分の場合
			else {
				// 走査中の x 軸平行な線分のテーブルから，
				// y 座標が y1 以上 y2 以下のものを数える．
				auto it1 = sx.lower_bound(x2_or_y1);
				auto it2 = sx.upper_bound(y2);
				res += (ll)distance(it1, it2);
			}
		}
	}

	return res;
}


//【最短バイトニックツアー】O(n^2)
/*
* x 座標の互いに異なる n 個の点列 p について，
*	x 座標最小の点 →(x 座標昇順)→ x 座標最大の点 →(x 座標降順)→ x 座標最小の点
* の順に点を結ぶ経路の最短長を返す．
*/
template <class T> double minimum_bitonic_tour(vector<Point<T>>& p) {
	int n = sz(p);

	sort(all(p));

	// dp[i][j] : i から j までの最短バイトニック経路長（i < j）
	vvd dp(n, vd(n, numeric_limits<double>::max()));
	dp[0][1] = (p[1] - p[0]).norm();
	repi(j, 1, n - 1) {
		repi(i, 0, j - 1) {
			// i, j が隣り合う頂点でない場合
			if (j - i >= 2) {
				// j からは j-1 にしか繋げない．
				dp[i][j] = dp[i][j - 1] + (p[j] - p[j - 1]).norm();
				continue;
			}

			// k : j をどこに繋ぐか
			repi(k, 0, i - 1) {
				chmin(dp[i][j], dp[k][i] + (p[k] - p[j]).norm());
			}
		}
	}

	double res = numeric_limits<double>::max();
	repi(i, 0, n - 2) {
		chmin(res, dp[i][n - 1] + (p[i] - p[n - 1]).norm());
	}

	return res;
}


//【直線群の交点からのマンハッタン距離の和の最小化】O(n log(n) log(1/EPS))
/*
* n 本の直線 a[i] x + b[i] y = c[i] の交点の多重集合 S について，S の点との
* マンハッタン距離の和が最小になる点 (x, y) のうち，両座標が最も小さいものを返す．
*
* 制約：平行な線分の組は存在しない，軸平行な線分は存在しない，交点の座標の絶対値は INF 以下．
*
* 利用：【二分探索（実数）】,【転倒数】
*/
pair<double, double> minimize_manhattan_sum(const vd& a, const vd& b, const vd& c) {
	// verify : https://atcoder.jp/contests/tenka1-2017/tasks/tenka1_2017_e

	int n = sz(a);

	// np : 交点の個数
	ll np = (ll)n * (n - 1) / 2;

	// X 座標が十分小さいところでの Y 座標についての昇順に直線をソートする．
	vector<pair<double, int>> yi(n);
	rep(i, n) {
		double x = (double)(-INF);
		double y = (c[i] - a[i] * x) / b[i];
		yi[i] = { y, i };
	}
	sort(all(yi));

	// マンハッタン距離最小を与えるのは中央値であることに注意する．
	// (-INF, x] の範囲に交点が (np - 1) / 2 個以下しかないか
	function<bool(double)> okQx = [&](double x) {
		// y[j] : j 番目の直線の X = x における Y 座標
		vd y(n);
		rep(j, n) {
			int i = yi[j].second;
			y[j] = (c[i] - a[i] * x) / b[i];
		}

		// Y 座標の転倒数が交点の個数となる．
		ll inv = inversion_number(y);

		return inv <= (np - 1) / 2;
	};

	// res_x : (-INF, x] の範囲の交点が (np - 1) / 2 個以下となるかならないかの境界
	double res_x = binary_search((double)(-INF), (double)(INF), okQx);

	// Y 座標についても同様のことを行う．
	vector<pair<double, int>> xi(n);
	rep(i, n) {
		double y = (double)(-INF);
		double x = (c[i] - b[i] * y) / a[i];
		xi[i] = { x, i };
	}
	sort(all(xi));

	function<bool(double)> okQy = [&](double y) {
		vd x(n);
		rep(j, n) {
			int i = xi[j].second;
			x[j] = (c[i] - b[i] * y) / a[i];
		}

		ll inv = inversion_number(x);

		return inv <= (np - 1) / 2;
	};

	double res_y = binary_search((double)(-INF), (double)(INF), okQy);

	return { res_x, res_y };
}


//【一定距離以内の点対の列挙】O(?)
/*
* n 個の点 (x[i], y[i]) からなる対で距離 d 以内にあるものを res に格納する．
* 点対は添字の対 (i, j)（i < j）として表し，辞書順に並べるものとする．
* 全探索できる点対の個数の上限を lim に与える．
* 
* 利用：【回転】
*/
void enumerate_point_pair(const vl& x, const vl& y, ll d, vector<pii>& res, ll lim = (ll)1e8) {
	// 参考 : https://wata-orz.hatenadiary.org/entry/20111218/1324226179
	// verify : https://atcoder.jp/contests/abc234/tasks/abc234_h
	
	int n = sz(x);

	// 乱数生成器
	static mt19937_64 mt64((int)time(NULL));
	uniform_real_distribution<double> rnd(0., 2 * PI);

	while (true) {
		// 点群をランダムな角度で回転させ，x 座標昇順にソートする．
		vector<pair<Point<double>, int>> pi(n);

		const Point<double> o(0, 0);
		double th = rnd(mt64);
		rep(i, n) {
			Point<double> p(x[i], y[i]);
			pi[i] = { rotate(p, o, th), i };
		}
		sort(all(pi));

		int l, r;

		// sum : x 座標の差が d 以内である点対の個数
		ll sum = 0;

		// 尺取法で x 座標の差が d 以内である点対を数える．
		l = 0, r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d + EPS) {
				sum += r - l;
				r++;
			}
			else {
				l++;
			}
		}

		// 調べるべき点対が多すぎるなら回転角をランダムに決め直す．
		if (sum > lim) continue;

		res.clear();

		// 尺取法で x 座標の差が d 以内である点対を列挙し，精密に距離を求める．
		l = 0; r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d + EPS) {
				repi(i, l, r - 1) {
					int jr = pi[r].second;
					int ji = pi[i].second;
					if (ji > jr) swap(ji, jr);

					Point<ll> e(x[ji] - x[jr], y[ji] - y[jr]);
					if (e.sqnorm() <= d * d) {
						res.push_back({ ji, jr });
					}
				}

				r++;
			}
			else {
				l++;
			}
		}
		sort(all(res));

		return;
	}
}


//【最近点対】O(?)（TODO : 嘘解法なのでやり直す）
/*
* n 個の点 (x[i], y[i]) について，最も近い 2 点の距離を返す．
* またその点対が i 番目と j 番目であることを ps = {i, j} として格納する．
* 全探索できる点対の個数の上限を lim に与える．
*
* 利用：【回転】
*/
template <class T>
double recent_point_pair(const vector<T>& x, const vector<T>& y, pii* ps = nullptr, ll lim = (ll)1e8) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_5_A
	
	int n = sz(x);

	// 乱数生成器
	static mt19937_64 mt64((int)time(NULL));
	uniform_real_distribution<double> rnd(0., 2 * PI);

	// x 座標の差が d 以内の点対に絞って精密に距離を求める．
	double d = 1;

	while (true) {
		// 点群をランダムな角度で回転させ，x 座標昇順にソートする．
		vector<pair<Point<double>, int>> pi(n);

		const Point<double> o(0, 0);
		double th = rnd(mt64);
		rep(i, n) {
			Point<double> p(x[i], y[i]);
			pi[i] = { rotate(p, o, th), i };
		}
		sort(all(pi));

		int l, r;

		// sum : x 座標の差が d 以内である点対の個数
		ll sum = 0;

		// 尺取法で x 座標の差が d 以内である点対を数える．
		l = 0, r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				sum += r - l;
				r++;
			}
			else {
				l++;
			}
		}

		// 該当する点対が存在しないなら，d を大きくし，回転角をランダムに決め直す．
		if (sum == 0) {
			d *= 2;
			continue;
		}

		// 調べるべき点対が多すぎるなら，d を小さくし，回転角をランダムに決め直す．
		if (sum > lim) {
			d /= 2;
			continue;
		}

		// 尺取法で x 座標の差が d 以内である点対を列挙し，精密に距離を求める．
		l = 0; r = 0;
		double res = (double)INFL;

		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				repi(i, l, r - 1) {
					int jr = pi[r].second;
					int ji = pi[i].second;
					if (ji > jr) swap(ji, jr);

					Point<double> e(x[ji] - x[jr], y[ji] - y[jr]);
					if (chmin(res, e.norm())) {
						if (ps != nullptr) {
							*ps = { ji, jr };
						}
					}
				}

				r++;
			}
			else {
				l++;
			}
		}

		// 最短距離が d を超えていたなら，d を大きくし，回転角をランダムに決め直す．
		if (res > d) {
			d *= 2;
			continue;
		}

		return res;
	}
}


//【2 点間マンハッタン距離の最大値】O(n)
/*
* n 個の点 (x[i], y[i]) から選んだ 2 点のマンハッタン距離の最大値を返す．
*/
ll maximize_manhattan_distance(const vl& x, const vl& y) {
	// verify : https://atcoder.jp/contests/abc178/tasks/abc178_e

	//【方法】
	// 点 (x1, y1) より点 (x2, y2) が右下にある場合，マンハッタン距離は
	//		(x2 - x1) + (y2 - y1) = (x2 + y2) - (x1 + y1)
	// とも表される．点 (x1, y1) より点 (x2, y2) が左下にある場合，マンハッタン距離は
	//		(x2 - x1) - (y2 - y1) = (x2 - y2) - (x1 - y1)
	// とも表される．2 点の位置関係はこれらのうちいずれかなので，
	//		max(x[i] + y[i]) - min(x[i] + y[i])
	//		max(x[i] - y[i]) - min(x[i] - y[i])
	// のうち大きい方が答えである．

	int n = sz(x);

	ll s_max = -INFL, s_min = INFL, d_max = -INFL, d_min = INFL;
	rep(i, n) {
		chmax(s_max, x[i] + y[i]);
		chmin(s_min, x[i] + y[i]);
		chmax(d_max, x[i] - y[i]);
		chmin(d_min, x[i] - y[i]);
	}

	ll res = max(s_max - s_min, d_max - d_min);

	return res;
}


//【2 点間距離の最大値と円被覆】
/*
* 距離がユークリッド距離，マンハッタン距離，チェス盤距離のいずれかであるとする．
* 点群 S 内の任意の 2 点の距離が 2r 以下のとき，S は半径 r の閉円盤で被覆される． 
* 
* verify : https://atcoder.jp/contests/joi2010ho/tasks/joi2010ho_d
*/

