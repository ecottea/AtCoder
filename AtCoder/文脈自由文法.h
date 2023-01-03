#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
// ■■■■■ 文脈自由文法 ■■■■■


//【文脈自由文法】
/*
* 出発記号を 0，変数を [0..n)，終端記号を (-m..0) で表すものとする．
* 生成規則の本体は記号を順に並べたリスト vi で表し，
* 頭部 A をもつ生成規則の集合は，vvvi[A] に生成規則のリストを並べて表す．
* また語は終端記号を並べたリスト vi で表す．
*
* 例（2値文字列上の回文）：
*	P → ε
*	P → a
*	P → b
*	P → aPa
*	P → bPb
* は，P を 0 に，a, b を -1, -2 に対応させれば
*	vvvi{{{}, {-1}, {-2}, {-1, 0, -1}, {-2, 0, -2}}}
* と表される．
*
* また回文 aba は vi{-1, -2, -1} と表される．
*/
using CFG = vvvi;


//【チョムスキー標準形への変換】O(|cfg|^2)
/*
* 文脈自由文法 cfg をチョムスキー標準形に変換したものを cnf に格納する．
* すなわち cnf の本体は「変数 2 つ」または「終端記号 1 つ」からなり，無用な記号をもたない．
*
* 制約：cfg は ε-規則を含まなず，無用な記号をもたない．
*
* 利用：【幅優先探索】
*/
void to_chomsky(const CFG& cfg, CFG& cnf) {
	// n : cfg の変数の数, m : chg の終端記号の数
	int n = sz(cfg), m = -INF;

	int n2 = n; CFG cfg2(n);

	// 本体の長さを 2 以下にする．
	rep(i, n) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repe(body, cfg[i]) {
			chmax(m, -*min_element(all(body)));

			// 本体の長さが 2 以下ならそのまま
			if (sz(body) <= 2) {
				cfg2[i].push_back(body);
				continue;
			}

			// 本体の長さが 2 を超えていたら，新たな変数を導入して長さ 2 に分割する．
			int l = sz(body);
			cfg2.resize(n2 + l - 2);
			cfg2[i].push_back(vi{ body[0], n2++ });
			repi(j, 1, l - 3) cfg2[n2 - 1].push_back(vi{ body[j], n2++ });
			cfg2[n2 - 1].push_back(vi{ body[l - 2], body[l - 1] });
		}
	}

	// 長さ 2 の本体から終端記号を除去する．
	vi to_var(m + 1, -1); Graph g(n);
	rep(i, n2) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repea(body, cfg2[i]) {
			// 本体の長さが 1 以下ならそのまま
			if (sz(body) == 1) {
				if (body[0] >= 0) g[i].push_back(body[0]);
				continue;
			}

			// 本体の長さが 2 のとき，終端記号を含むなら対応する変数に置き換える．
			rep(k, 2) if (body[k] < 0) {
				int x = -body[k];
				if (to_var[x] == -1) {
					to_var[x] = n2++;
					cfg2.push_back(vvi{ {-x} });
				}
				body[k] = to_var[x];
			}
		}
	}

	// 単位規則を除去する．
	vvi dist(n);
	rep(i, n) dist[i] = breadth_first_search(g, i);

	cnf = CFG(n2);
	rep(i, n) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repea(body, cfg2[i]) {
			// 単位規則なら除去する．
			if (sz(body) == 1 && body[0] >= 0) continue;

			// i への単位規則をもつ j 全てに本体をコピーする．
			rep(j, n) if (dist[j][i] < INF) {
				cnf[j].push_back(body);
			}
		}
	}
	repi(i, n, n2 - 1) cnf[i] = cfg2[i];
}


//【チョムスキー標準形への所属検査】O(n^3 |cnf|)
/*
* 語 w[0..n) がチョムスキー標準形 cnf で生成される言語に属するかを返す．
*
* 制約：cnf はチョムスキー標準形である．
*/
bool chomsky_wordQ(const vi& w, const CFG& cnf) {
	// 参考：『オートマトン 言語理論 計算論I』(J.ホッブクロフトほか) pp.327-

	// n : 語 w の長さ，m : cnf の変数の数
	int n = sz(w), m = sz(cnf);

	// dp[i][j] : 語 w[i..j] を導出できる変数の集合
	vector<vector<unordered_set<int>>> dp(n, vector<unordered_set<int>>(n));

	// cnf において終端記号の導出を無視したもの
	vector<vector<pii>> cnf_v(m);

	rep(j, m) repea(body, cnf[j]) {
		// 語 w[i] を導出できるのは，本体が終端記号 1 つからなる生成規則のみである．
		if (sz(body) == 1) {
			int x = body[0];

			rep(i, n) if (w[i] == x) dp[i][i].insert(j);
		}
		else {
			cnf_v[j].emplace_back(body[0], body[1]);
		}
	}

	// len : 注目している部分文字列 w[i1..i2] の長さ
	repi(len, 2, n) repi(i1, 0, n - len) {
		int i2 = i1 + len - 1;

		// 前半 w[i1..im] と後半 w[im+1..i2] に分けて導出できるかを調べる
		repi(im, i1, i2 - 1) {
			rep(j, m) repea(body, cnf_v[j]) {
				int jl, jr;
				tie(jl, jr) = body;

				if (dp[i1][im].count(jl) && dp[im + 1][i2].count(jr)) {
					dp[i1][i2].insert(j);
				}
			}
		}
	}

	return dp[0][n - 1].count(0);
}


//【コスト付き文脈自由文法】
/*
* 出発記号を 0，変数を [0..n)，終端記号を (-m..0) で表すものとする．
* 生成規則の本体は記号を順に並べたリストとコストの対 pair<vi, ll> で表し，
* 頭部 A をもつ生成規則の集合は，CFG[A] に生成規則のリストを並べて表す．
* また語は終端記号を並べたリスト vi で表す．
*/
using WCFG = vector<vector<pair<vi, ll>>>;


//【コスト付きチョムスキー標準形への変換】O(|cfg|^2)
/*
* コスト付き文脈自由文法 cfg をコスト付きチョムスキー標準形に変換したものを cnf に格納する．
* すなわち cnf の本体は「変数 2 つ」または「終端記号 1 つ」からなり，無用な記号をもたない．
*
* 制約：cfg は ε-規則を含まなず，無用な記号をもたない．cfg のコストは 0 か 1
*
* 利用：【01-BFS】
*/
void to_chomsky(const WCFG& cfg, WCFG& cnf) {
	// verify : https://atcoder.jp/contests/abc261/tasks/abc261_g

	// n : cfg の変数の数, m : chg の終端記号の数
	int n = sz(cfg), m = -INF;

	int n2 = n; WCFG cfg2(n);

	// 本体の長さを 2 以下にする．
	rep(i, n) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repe(body_cost, cfg[i]) {
			const vi& body = body_cost.first;
			ll cost = body_cost.second;

			chmax(m, -*min_element(all(body)));

			// 本体の長さが 2 以下ならそのまま
			if (sz(body) <= 2) {
				cfg2[i].push_back(body_cost);
				continue;
			}

			// 本体の長さが 2 を超えていたら，新たな変数を導入して長さ 2 に分割する．
			int l = sz(body);
			cfg2.resize(n2 + l - 2);
			cfg2[i].push_back(make_pair(vi{ body[0], n2++ }, cost));
			repi(j, 1, l - 3) cfg2[n2 - 1].push_back(make_pair(vi{ body[j], n2++ }, 0LL));
			cfg2[n2 - 1].push_back(make_pair(vi{ body[l - 2], body[l - 1] }, 0LL));
		}
	}

	// 長さ 2 の本体から終端記号を除去する．
	vi to_var(m + 1, -1); WGraph g(n);
	rep(i, n2) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repea(body_cost, cfg2[i]) {
			vi& body = body_cost.first;
			ll cost = body_cost.second;

			// 本体の長さが 1 以下ならそのまま
			if (sz(body) == 1) {
				if (body[0] >= 0) g[i].push_back({ body[0], cost });
				continue;
			}

			// 本体の長さが 2 のとき，終端記号を含むなら対応する変数に置き換える．
			rep(k, 2) if (body[k] < 0) {
				int x = -body[k];
				if (to_var[x] == -1) {
					to_var[x] = n2++;
					cfg2.push_back(vector<pair<vi, ll>>{ make_pair(vi{ -x }, 0LL) });
				}
				body[k] = to_var[x];
			}
		}
	}

	// 単位規則を除去する．
	vvi dist(n);
	rep(i, n) binary_BFS(g, i, dist[i]);

	cnf = WCFG(n2);
	rep(i, n) {
		// body : 変数 i を頭部にもつ生成規則の本体
		repea(body_cost, cfg2[i]) {
			const vi& body = body_cost.first;
			ll cost = body_cost.second;

			// 単位規則なら除去する．
			if (sz(body) == 1 && body[0] >= 0) continue;

			// i への単位規則をもつ j 全てに本体をコピーする．
			rep(j, n) if (dist[j][i] < INF) {
				cnf[j].push_back(make_pair(body, cost + dist[j][i]));
			}
		}
	}
	repi(i, n, n2 - 1) cnf[i] = cfg2[i];
}


//【コスト付きチョムスキー標準形での導出コスト】O(n^3 |cnf|)
/*
* コスト付きチョムスキー標準形 cnf で語 w[0..n) を導出するのにかかる最小コストを返す（不可能なら INFL）
*
* 制約：cnf はチョムスキー標準形である．
*/
ll chomsky_derivate_distance(const vi& w, const WCFG& cnf) {
	// 参考：『オートマトン 言語理論 計算論I』(J.ホッブクロフトほか) pp.327-
	// verify : https://atcoder.jp/contests/abc261/tasks/abc261_g

	// n : 語 w の長さ，m : cnf の変数の数
	int n = sz(w), m = sz(cnf);

	// dp[i][j] : 語 w[i..j] を導出するための最小コスト
	vector<vector<unordered_map<int, ll>>> dp(n, vector<unordered_map<int, ll>>(n));

	// cnf において終端記号の導出を無視したもの
	vector<vector<pair<pii, ll>>> cnf_v(m);

	rep(j, m) repea(body_cost, cnf[j]) {
		const vi& body = body_cost.first;
		ll cost = body_cost.second;

		// 語 w[i] を導出できるのは，本体が終端記号 1 つからなる生成規則のみである．
		if (sz(body) == 1) {
			int x = body[0];

			rep(i, n) if (w[i] == x) {
				if (dp[i][i].count(j)) chmin(dp[i][i][j], cost);
				else dp[i][i][j] = cost;
			}
		}
		else {
			cnf_v[j].push_back(make_pair(make_pair(body[0], body[1]), cost));
		}
	}

	// len : 注目している部分文字列 w[i1..i2] の長さ
	repi(len, 2, n) repi(i1, 0, n - len) {
		int i2 = i1 + len - 1;

		// 前半 w[i1..im] と後半 w[im+1..i2] に分けて導出できるかを調べる
		repi(im, i1, i2 - 1) {
			rep(j, m) repea(body_cost, cnf_v[j]) {
				int jl, jr;
				tie(jl, jr) = body_cost.first;
				ll cost = body_cost.second;

				if (dp[i1][im].count(jl) && dp[im + 1][i2].count(jr)) {
					ll ncost = cost + dp[i1][im][jl] + dp[im + 1][i2][jr];
					if (dp[i1][i2].count(j)) chmin(dp[i1][i2][j], ncost);
					else dp[i1][i2][j] = ncost;
				}
			}
		}
	}

	if (dp[0][n - 1].count(0)) return dp[0][n - 1][0];
	else return INFL;
}


