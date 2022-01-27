#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "順列.h"
#include "探索.h"
#include "作図.h"
// ■■■■■ 平面走査 ■■■■■


//【線分群の交点（軸平行）】O(n log n)
/*
* n 本の閉線分の集合 segs の交点の数を返す．
* segs[i] = {x1, y1, x2, y2} は (x1, y1), (x2, y2) を結ぶ閉線分を表す．
*
* 制約：互いに平行な線分同士が共有点をもつことはない．
*/
ll count_intersection(vector<tuple<ll, ll, ll, ll>>& segs) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_6_A

	int n = sz(segs);

	// 各線分が x1 <= x2, y1 <= y2 を満たすようにしつつ，x1 ごとに昇順に記録する．
	// 線分が x 軸平行のとき type = 0，y 軸平行のとき type = 1 とし，
	// 同じ x1 をもつ線分同士については type 0 が先，type 1 が後に並ぶようにする．
	map<ll, set<tuple<int, ll, ll>>> seg;
	rep(i, n) {
		ll x1, y1, x2, y2;
		tie(x1, y1, x2, y2) = segs[i];

		if (x1 != x2) {
			seg[min(x1, x2)].insert({ 0, max(x1, x2), y2 });
		}
		else {
			seg[x1].insert({ 1, min(y1, y2), max(y1, y2) });
		}
	}

	ll res = 0;

	// 走査中の x 座標と交点をもつような x 軸平行な線分の y 座標を記録するテーブル．
	// 二分探索を利用するため vector でなく set を用いている．
	set<ll> sx;

	// x 軸平行な線分の終端まで走査したらそれをテーブルから取り除くために，
	// 終端の x 座標について昇順に取り出すことのできる優先度付きキュー．
	priority_queue<pll, vector<pll>, greater<pll>> q;

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
			int type;
			ll x2_or_y1, y2;
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
				res += distance(it1, it2);
			}
		}
	}

	return res;
}


//【直線群の交点（マンハッタン距離の和の最小化）】O(n log n log(1/EPS))
/*
* n 本の直線 a[i] x + b[i] y = c[i] の交点の多重集合 S について，S の点とのマンハッタン距離の
* 和が最小になる点 (x, y) のうち，両座標が最も小さいものを返す．
*
* 制約：平行な線分の組は存在しない，軸平行な線分は存在しない，交点の座標の絶対値は INF 以下．
*
* 利用：【二分探索（実数）】，【転倒数】
*/
pair<double, double> minimize_manhattansum(const vd& a, const vd& b, const vd& c) {
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
	dump(yi);

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
	static mt19937_64 mt64((int)time(0));
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


