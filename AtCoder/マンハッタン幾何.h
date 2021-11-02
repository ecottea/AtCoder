#pragma once
#include "header.h"
#include "構造(幾何).h"
// ■■■■■ マンハッタン幾何 ■■■■■


//【線分群の交点（軸平行）】O(n log n)
/*
* n 本の閉線分の集合 segs の交点の数を返す．
* segs[i] = {x1, y1, x2, y2} は (x1, y1), (x2, y2) を結ぶ閉線分を表す．
*
* 制約：互いに平行な線分同士が共有点をもつことはない．
* 
*（平面走査）
*/
ll count_intersection(vector<tuple<ll, ll, ll, ll>>& segs) {
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


