#pragma once
#include "header.h"
#include "ハッシュ.h"
#include "木クエリ.h"
// ■■■■■ 座標圧縮 ■■■■■


//【座標圧縮】O(n log n)
/*
* a[0..n) を座標圧縮した結果を a_cp[0..n) に格納し，その値域の大きさを返す．
* また xs[j] に圧縮された座標 j に対応する元の座標を格納する．
*
* a に重複する要素がなければ，a_cp[i] は a[i] が昇順で何番目かを表し，
* xs[j] は昇順で j 番目の要素が何かを表す．
*/
template <class T>
int coordinate_compression(const vector<T>& a, vi& a_cp, vector<T>* xs = nullptr) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_o

	int n = sz(a);
	if (xs == nullptr) xs = new vector<T>;

	// *xs : a の x 座標のユニークな昇順列
	*xs = a;
	uniq(*xs);

	// a[i] が xs において何番目かを求める．
	a_cp.resize(n);
	rep(i, n) a_cp[i] = lbpos(*xs, a[i]);

	return sz(*xs);
}


//【座標圧縮（区間）】O(n log n)
/*
* n 個の半開区間 [x1[i], x2[i]) を座標圧縮した結果を x1_cp[i], x2_cp[i] に格納する．
* また xs[i] に圧縮された座標 i に対応する元の座標を格納する．
* 戻り値として x 座標の数を返す．
*/
template <class T>
int coordinate_compression_interval(const vector<T>& x1, const vector<T>& x2,
	vi& x1_cp, vi& x2_cp, vector<T>* xs = nullptr)
{
	// verify : https://atcoder.jp/contests/abc188/tasks/abc188_d

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;

	// x 座標だけを抜き出す．
	xs->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		xs->push_back(x2[i]);
	}

	// *xs : 区間端の x 座標のユニークな昇順列
	uniq(*xs);

	// 各区間の端の座標が xs において何番目かを求める．
	x1_cp.resize(n); x2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
	}

	return sz(*xs);
}


//【座標圧縮（長方形）】O(n log n)
/*
* n 個の半開長方形 [x1[i]..x2[i])×[y1[i]..y2[i]) を座標圧縮した結果を
* x1_cp[i], y1_cp[i], x2_cp[i], y2_cp[i] に格納する．
* また xs[i], ys[j] に圧縮された座標 i, j に対応する元の座標を格納する．
* 戻り値として，(x 座標の数, y 座標の数) を返す．
*/
template <class T>
pii coordinate_compression_rectangle(
	const vector<T>& x1, const vector<T>& x2, const vector<T>& y1, const vector<T>& y2,
	vi& x1_cp, vi& x2_cp, vi& y1_cp, vi& y2_cp,
	vector<T>* xs = nullptr, vector<T>* ys = nullptr)
{
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_4_A

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;
	if (ys == nullptr) ys = new vector<T>;

	// x, y それぞれの座標だけを抜き出す．
	xs->clear();
	ys->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		ys->push_back(y1[i]);
		xs->push_back(x2[i]);
		ys->push_back(y2[i]);
	}

	// *xs[*ys] : 区間端の x[y] 座標のユニークな昇順列
	uniq(*xs); uniq(*ys);

	// 各長方形の角の座標が xs, ys において何番目かを求める．
	x1_cp.resize(n); y1_cp.resize(n);
	x2_cp.resize(n); y2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		y1_cp[i] = lbpos(*ys, y1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
		y2_cp[i] = lbpos(*ys, y2[i]);
	}

	return { sz(*xs), sz(*ys) };
}


//【座標圧縮（直方体）】O(n log n)
/*
* n 個の半開直方体 [x1[i]..x2[i])×[y1[i]..y2[i])×[z1[i]..z2[i]) を座標圧縮した結果を
* x1_cp[i], y1_cp[i], z1_cp[i], x2_cp[i], y2_cp[i], z2_cp[i] に格納する．
* また xs[i], ys[j], zs[k] に圧縮された座標 i, j, k に対応する元の座標を格納する．
* 戻り値として，(x 座標の数, y 座標の数, z 座標の数) を返す．
*/
template <class T>
tuple<int, int, int> coordinate_compression_rectangular(
	const vector<T>& x1, const vector<T>& x2, const vector<T>& y1,
	const vector<T>& y2, const vector<T>& z1, const vector<T>& z2,
	vi& x1_cp, vi& x2_cp, vi& y1_cp, vi& y2_cp, vi& z1_cp, vi& z2_cp,
	vector<T>* xs = nullptr, vector<T>* ys = nullptr, vector<T>* zs = nullptr)
{
	// verify : https://atcoder.jp/contests/joi2013yo/tasks/joi2013yo_e

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;
	if (ys == nullptr) ys = new vector<T>;
	if (zs == nullptr) zs = new vector<T>;

	// x, y, z それぞれの座標だけを抜き出す．
	xs->clear();
	ys->clear();
	zs->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		ys->push_back(y1[i]);
		zs->push_back(z1[i]);
		xs->push_back(x2[i]);
		ys->push_back(y2[i]);
		zs->push_back(z2[i]);
	}

	// *xs[*ys, *zs] : 区間端の x[y, z] 座標のユニークな昇順列
	uniq(*xs); uniq(*ys); uniq(*zs);

	// 各直方体の角の座標が xs, ys, zs において何番目かを求める．
	x1_cp.resize(n); y1_cp.resize(n); z1_cp.resize(n);
	x2_cp.resize(n); y2_cp.resize(n); z2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		y1_cp[i] = lbpos(*ys, y1[i]);
		z1_cp[i] = lbpos(*zs, z1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
		y2_cp[i] = lbpos(*ys, y2[i]);
		z2_cp[i] = lbpos(*zs, z2[i]);
	}

	return { sz(*xs), sz(*ys), sz(*zs) };
}


//【座標圧縮（全順序集合）】O(n log n)
/*
* a[0..n) を全順序 cmp を用いて座標圧縮した結果を a_cp[0..n) に格納し，その値域の大きさを返す．
* また xs[j] に圧縮された座標 j に対応する元の座標を格納する．
*/
template <class T>
int coordinate_compression(const vector<T>& a, vi& a_cp, const function<bool(T, T)>& cmp, vector<T>* xs = nullptr) {
	// verify : https://atcoder.jp/contests/tenka1-2014-quala/tasks/tenka1_2014_qualA_d

	int n = sz(a);
	a_cp.resize(n);
	if (xs == nullptr) xs = new vector<T>;

	// *xs : a のユニークな昇順列
	*xs = a;
	sort(all(*xs), cmp);
	xs->erase(unique(all(*xs)), xs->end());

	// a[i] が xs において何番目かを求める．
	rep(i, n) a_cp[i] = (int)distance(xs->begin(), lower_bound(all(*xs), a[i], cmp));

	return sz(*xs);
}


//【格子 DAG の座標圧縮（真に増加）】O(n log n)
/*
* DAG G = (V, E) を，V = [0..n)，E は以下の規則で定まるものとする：
*	辺 i→j をもつ ⇔ x[i] < x[j] かつ y[i] < y[j]
* 各頂点間の移動可能性が G と等しい DAG を返す（頂点 [0..n) は G と対応する．）
*/
template <class T>
Graph lattice_DAG_compression(const vector<T>& x, const vector<T>& y) {
	// verify : https://atcoder.jp/contests/arc165/tasks/arc165_f

	int n = sz(x);

	map<T, vi> x_to_is;
	rep(i, n) x_to_is[x[i]].emplace_back(i);

	vvi k_to_is;
	repea(tmp, x_to_is) k_to_is.emplace_back(move(tmp.second));
	int K = sz(k_to_is);

	Graph g(n); int id = n;

	function<void(int, int)> rf = [&](int l, int r) {
		if (r - l == 1) return;

		int m = (l + r) / 2;

		vector<tuple<T, int, int>> yik;
		repi(k, l, m - 1) repe(i, k_to_is[k]) yik.emplace_back(2 * y[i] + 1, i, k);
		repi(k, m, r - 1) repe(i, k_to_is[k]) yik.emplace_back(2 * y[i], i, k);
		sort(all(yik));

		int pid = -1;

		for (auto& [y, i, k] : yik) {
			g.push_back(vi());

			if (pid != -1) g[pid].push_back(id);
			pid = id;

			if (k < m) g[i].push_back(id);
			else g[id].push_back(i);

			id++;
		}

		rf(l, m);
		rf(m, r);
	};
	rf(0, K);

	return g;
}


//【木の座標圧縮】
/*
* Auxiliary_tree(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g で初期化する．
*
* Graph create(vi vs, vi& id) : O(k (log k + log n))  (k = |vs|)
*	頂点集合 vs とそれらの LCA からなる座標圧縮された木 gc（根は 0）を構築して返す．
*	vs[i] は gc[id[i]] と対応する．
*
* 利用：【オイラーツアー】
*/
struct Auxiliary_tree {
	// 参考 : https://tjkendev.github.io/procon-library/python/graph/auxiliary_tree.html

	Euler_tour<Graph> ET;

public:
	Auxiliary_tree(const Graph& g, int rt) : ET(g, rt) {
		// verify : https://atcoder.jp/contests/arc086/tasks/arc086_c
	}

	// 頂点集合 vs とそれらの LCA からなる座標圧縮された木 g（根は 0）を構築して返す．
	// vs[i] は g[id[i]] と対応する．
	Graph create(const vi& vs, vi& id) {
		// verify : https://atcoder.jp/contests/arc086/tasks/arc086_c

		int k = sz(vs);
		id.resize(k);

		vector<pii> vis(k);
		rep(i, k) vis[i] = { vs[i], i };

		// 頂点集合 vs をオイラーツアーの行きがけ順にソートする．
		sort(all(vis), [&](pii a, pii b) { return ET.get_in(a.first) < ET.get_in(b.first); });

		// 行きがけ順で隣り合う 2 頂点の LCA は必要なので頂点集合に追加する．
		rep(i, k - 1) vis.emplace_back(ET.lca(vis[i].first, vis[i + 1].first), -1);

		// LCA も含めた頂点集合 vs をオイラーツアーの行きがけ順にソートし重複を除去する．
		sort(all(vis), [&](pii a, pii b) { return ET.get_in(a.first) < ET.get_in(b.first); });
		auto it = unique(all(vis));
		vis.erase(it, vis.end());
		k = sz(vis);

		// 元の頂点と座標圧縮された木の頂点との対応を調べる．
		rep(i, k) if (vis[i].second != -1) id[vis[i].second] = i;

		Graph g(k); stack<int> stk;
		rep(si, k) {
			// v = vs[si] とし，スタックトップが v の先祖になるまで走査済の頂点をポップする．
			while (!stk.empty() && ET.get_out(vis[stk.top()].first) < ET.get_in(vis[si].first)) stk.pop();

			// v に先祖が居ればそれは直近の先祖であるから辺で繋ぐ．
			if (!stk.empty()) g[stk.top()].push_back(si);

			stk.push(si);
		}

		return g;
	}
};


//【二分木の座標圧縮】
/*
* a = pos[i] は，根 0 から左に a[0] 回，右に a[1] 回，左に a[2] 回，右に a[3] 回 ... と
* 辿ったところに木 T の i 番目の頂点があることを表すものとする．
* 木 T を座標圧縮し，辺の本数を重みとした 0 を根とする重み付き二分木 Tc を構築し，その頂点数を返す．
* Tc の頂点 v の左[右] の {子の頂点番号, 子への辺の重み} の組を l[v][ r[v] ] に格納する．
* また T の i 番目の頂点が Tc のどの頂点と対応するかを vs[i] に格納する．
*
* 利用：【ローリングハッシュ（列）】
*/
int coordinate_compression_binary_tree(const vvl& pos, vector<pil>& l, vector<pil>& r, vi& vs) {
	// verify : https://atcoder.jp/contests/abc273/tasks/abc273_h

	int n = sz(pos);
	vs.resize(n);

	unordered_map<ll, int> hash_to_id; int id = 0;
	vector<vector<pli>> line; vi dir;

	hash_to_id[0] = id++;
	line.emplace_back();
	dir.emplace_back();

	rep(i, n) {
		int m = sz(pos[i]);
		Rolling_hash<vl> h(pos[i]);

		int t = -1;
		rep(j, m) {
			ll hash_t = h.get(0, j + 1);
			if (!hash_to_id.count(hash_t)) {
				hash_to_id[hash_t] = id++;
				line.emplace_back();
				dir.emplace_back();
			}

			int s = hash_to_id[h.get(0, j)];
			t = hash_to_id[hash_t];
			line[s].emplace_back(pos[i][j], t);
			dir[s] = j % 2;
		}

		vs[i] = t;
	}
	dump(hash_to_id); dump(id); dumpel(line); dump(dir);

	l.assign(id, { -1, -1 });
	r.assign(id, { -1, -1 });

	rep(i, id) {
		uniq(line[i]);
		int m = sz(line[i]);

		int s = i; ll dist_s = 0;
		rep(j, m) {
			int t; ll dist_t;
			tie(dist_t, t) = line[i][j];

			if (dir[i]) r[s] = { t, dist_t - dist_s };
			else l[s] = { t, dist_t - dist_s };

			s = t;
			dist_s = dist_t;
		}
	}
	dump(l); dump(r);

	ll hash0 = Rolling_hash<vl>(vl{ 0 }).get(0, 1);
	if (!hash_to_id.count(hash0)) return id;
	int del = hash_to_id[hash0];

	r[0] = r[del];
	l[0] = l[del];

	swap(l[id - 1], l[del]); l.pop_back();
	swap(r[id - 1], r[del]); r.pop_back();
	id--;

	rep(i, id) {
		if (l[i].first == id) l[i].first = del;
		if (r[i].first == id) r[i].first = del;
	}
	rep(i, n) {
		if (vs[i] == del) vs[i] = 0;
		if (vs[i] == id) vs[i] = del;
	}

	return id;
}


