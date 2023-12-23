#pragma once
#include "header.h"
// ■■■■■ タイリング ■■■■■


//【ドミノタイリング】O(h w √(h w))
/*
* c[0..h)[0..w) 上に置けるだけドミノ {(i1, j1), (i2, j2)} を配置し，そのリストを返す．
* c[i][j] = ng であるようなマス (i, j) にはドミノを配置できない．
*/
vector<tuple<int, int, int, int>> domino_tiling(const vvc& c, char ng = '#') {
	// verify : https://atcoder.jp/contests/practice2/tasks/practice2_d

	int h = sz(c), w = sz(c[0]);

	int ST = h * w, GL = ST + 1;
	mf_graph<int> g(GL + 1);

	rep(i, h) rep(j, w) {
		if (c[i][j] == ng) continue;

		// ST とマス，マスと GL を繋ぐ
		if ((i + j) % 2 == 0) g.add_edge(ST, i * w + j, 1);
		else g.add_edge(i * w + j, GL, 1);

		// マスと隣のマスを繋ぐ
		if ((i + j) % 2 == 0) {
			if (i > 0 && c[i - 1][j] != ng) g.add_edge(i * w + j, (i - 1) * w + j, 1);
			if (i < h - 1 && c[i + 1][j] != ng) g.add_edge(i * w + j, (i + 1) * w + j, 1);
			if (j > 0 && c[i][j - 1] != ng) g.add_edge(i * w + j, i * w + (j - 1), 1);
			if (j < w - 1 && c[i][j + 1] != ng) g.add_edge(i * w + j, i * w + (j + 1), 1);
		}
	}

	int f = g.flow(ST, GL);

	vector<tuple<int, int, int, int>> res;
	res.reserve(f);

	repe(e, g.edges()) {
		// マスとマスを結びフローが流れている辺以外は無視する．
		if (e.from == ST || e.to == GL || e.flow == 0) continue;

		int i1 = e.from / w, j1 = e.from % w;
		int i2 = e.to / w, j2 = e.to % w;
		res.emplace_back(i1, j1, i2, j2);
	}

	return res;
}


//【ポリオミノの列挙】O(?)（n=8 くらいまで動く）
/*
* n 以下の k について，k-オミノを列挙する．
*
* k-オミノ は k 個の点 {x, y} の集合として表す．
* ただし x, y 各座標の最小値は 0 であるようにする．
*
* polyomino[k] : k-オミノを表す点の集合，の集合
*/
void enumerate_polyominoes(int n, vector<set<set<pii>>>& polyomino) {
	// polyomino[i - 1] : i-オミノ 全て
	// i-オミノ は i 個の点 (x, y) の集合として表す．
	// ただし x, y 各座標の最小値は 0 であるようにする．
	polyomino = vector<set<set<pii>>>(n);

	// 1-オミノ は { (0, 0) } ただ 1 つしか存在しない．
	polyomino[0] = { { {0, 0} } };

	// 大きさ i + 1 について昇順に探していく．
	repi(i, 0, n - 2) {
		// 大きさ i + 1 の各ポリオミノ poly について
		repe(poly, polyomino[i]) {
			// (i+1)-オミノ poly の各点 (x, y) について
			repe(p, poly) {
				int x, y;
				tie(x, y) = p;

				// 点 (x, y) の 4 近傍 (nx, ny) について
				rep(j, 4) {
					int nx = x + DX[j];
					int ny = y + DY[j];
					pii np = { nx, ny };

					// もし (nx, ny) が poly に含まれていたら何もしない．
					if (poly.count(np) > 0) {
						continue;
					}

					// (nx, ny) を追加した (i+2)-オミノ npoly を作る．
					// もし nx, ny が -1 になったら全体を +1 平行移動する．
					set<pii> npoly;
					int add_x = 0, add_y = 0;
					if (nx < 0) {
						add_x = 1;
					}
					if (ny < 0) {
						add_y = 1;
					}
					repe(p, poly) {
						npoly.insert({ p.first + add_x, p.second + add_y });
					}
					npoly.insert({ nx + add_x, ny + add_y });

					// (i+2)-オミノ の集合に npoly を追加する．
					// set を用いているので重複することはない．
					polyomino[i + 1].insert(npoly);
				}
			}
		}
	}

	/* 確認用
	void zikken() {
		cout << fixed << setprecision(12);

		int n;
		cin >> n;

		vector<set<set<pii>>> polyomino;
		enumerate_polyominoes(n, polyomino);

		repe(polys, polyomino[n - 1]) {
			vector<vector<char>> c(n, vector<char>(n, '.'));
			repe(p, polys) {
				int x, y;
				tie(x, y) = p;

				c[x][y] = '#';
			}
			rep(i, n) {
				rep(j, n) {
					cout << c[i][j];
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	*/
}


//【ドミノ＆モノミノのタイリングの列挙】O(?)
/*
* h×w の盤面にドミノ d 個とモノミノ h w - 2 d 個を敷き詰める方法を boards に列挙する．
* i 番目に敷き詰められたタイルを番号 i で表す．
*/
void enumerate_domino_monomino_tiling(int h, int w, int d, vvvi& boards) {
	//verify : https://atcoder.jp/contests/abc196/tasks/abc196_d

	vvi board(h, vi(w, -1));

	// (i, j): 注目位置，a : 1x2 タイルの残り数，b : 1x1 タイルの残り数
	function<void(int, int, int, int, int)> dfs = [&](int i, int j, int a, int b, int id) {
		// 完成していれば記録
		if (i == h) {
			boards.push_back(board);
			return;
		}

		// 右まで走査しきったら 1 つ下の行へ
		if (j == w) {
			dfs(i + 1, 0, a, b, id);
			return;
		}

		// すでにタイルが敷かれていたら 1 つ右のマスへ
		if (board[i][j] >= 0) {
			dfs(i, j + 1, a, b, id);
			return;
		}

		// 1x1 タイルを敷く場合
		if (b > 0) {
			board[i][j] = id;
			dfs(i, j + 1, a, b - 1, id + 1);
			board[i][j] = -1;
		}

		// 1x2 タイルを敷く場合
		if (a > 0 && j < w - 1 && board[i][j + 1] == -1) {
			board[i][j] = board[i][j + 1] = id;
			dfs(i, j + 2, a - 1, b, id + 1);
			board[i][j] = board[i][j + 1] = -1;
		}

		// 2x1 タイルを敷く場合
		if (a > 0 && i < h - 1 && board[i + 1][j] == -1) {
			board[i][j] = board[i + 1][j] = id;
			dfs(i, j + 1, a - 1, b, id + 1);
			board[i][j] = board[i + 1][j] = -1;
		}
	};

	dfs(0, 0, d, h * w - 2 * d, 0);
}


//【T-テトロミノのタイリングの列挙】O(?)
/*
* h×w の盤面に T-テトロミノを敷き詰める方法全てを格納したリストを返す．
* i 番目に敷き詰められたタイルを番号 i で表す．
* 
*（バックトラッキング）
*/
vvvi enumerate_Ttetromino_tiling(int h, int w) {
	// board[i][j] : 盤の位置 (i, j) に置かれているタイルの番号（タイルがなければ -1）
	vvi board(h, vi(w, -1));
	vvvi boards;

	// board[i][j] を返す（盤外なら -2 を返す）
	auto get = [&](int i, int j) {
		if (i < 0 || j < 0 || i >= h || j >= w) return -2;
		return board[i][j];
	};

	// 敷き詰めに使うピースのリスト
	// 各ピースは辞書順最小位置を {0, 0} としたマスの集合で表す．
	vector<vector<pii>> pieces{
		{ {0, 0}, {0, 1}, {0, 2}, {1, 1} }, // ┬
		{ {0, 0}, {1, 0}, {1, 1}, {2, 0} }, // ├
		{ {0, 0}, {1, -1}, {1, 0}, {1, 1} }, // ┴
		{ {0, 0}, {1, -1}, {1, 0}, {2, 0} }  // ┤
	};

	// (i, j): 注目位置
	function<void(int, int, int)> dfs = [&](int i, int j, int id) {
		// 完成していれば記録
		if (i == h) {
			boards.push_back(board);
			return;
		}

		// 右まで走査しきったら 1 つ下の行へ
		if (j == w) {
			dfs(i + 1, 0, id);
			return;
		}

		// すでにタイルが敷かれていたら 1 つ右のマスへ
		if (get(i, j) >= 0) {
			dfs(i, j + 1, id);
			return;
		}

		// 各ピースを置くことができるかをチェックする．
		repe(piece, pieces) {
			bool ok = true;
			for (auto& [di, dj] : piece) {
				if (get(i + di, j + dj) != -1) {
					ok = false;
					break;
				}
			}
			if (!ok) continue;

			for (auto& [di, dj] : piece) board[i + di][j + dj] = id;
			dfs(i, j + 1, id + 1);
			for (auto& [di, dj] : piece) board[i + di][j + dj] = -1;
		}
	};
	dfs(0, 0, 0);

	return boards;
}


//【トロミノのタイリングの数え上げ】O(h 4^w)
/*
* h×w の盤面にトロミノを敷き詰める方法が何通りあるかを返す．
*/
ll count_Tromino_tiling(int h, int w) {
	// マスの数が 3 の倍数でなければ明らかに不可能．
	if (h * w % 3 != 0) return 0;

	// 盤は縦長だとする．
	if (h < w) swap(h, w);

	// 敷き詰めに使うピースのリスト
	// 各ピースは辞書順最小位置を {0, 0} としたマスの集合で表す．
	vector<vector<pii>> pieces{
		{ {0, 0}, { 0, 1 }, { 0, 2 } },
		{ {0, 0}, {0, 1}, {1, 0} },
		{ {0, 0}, {0, 1}, {1, 1} },
		{ {0, 0}, {1, -1}, {1, 0} },
		{ {0, 0}, {1, 0}, {1, 1} },
		{ {0, 0}, {1, 0}, {2, 0} }
	};

	// mat[set] : 盤の上 2 行分の敷き詰めが set のとき，そこから遷移できる次の 2 行分のパターンのリスト
	vvl mat(1LL << (2 * w));

	// set : 盤の上 2 行分の敷き詰めパターン
	repb(set, 2 * w) {
		// board[i][j] : 上 3 行の盤の位置 (i, j) にタイルが置かれているか
		vvb board(3, vb(w));
		rep(x, 2) rep(y, w) if (get(set, x * w + y)) board[x][y] = 1;

		// board[i][j] を返す（盤外なら true を返す）
		auto get_board = [&](int i, int j) {
			if (j < 0 || j >= w) return true;
			return (bool)board[i][j];
		};

		// (0, j): 注目位置
		function<void(int)> dfs = [&](int j) {
			// 1 行目が埋まったら次の 2 行の形状を記録
			if (j == w) {
				int nset = 0;
				rep(y, w) if (board[1][y]) nset |= 1 << y;
				rep(y, w) if (board[2][y]) nset |= 1 << (w + y);
				mat[set].push_back(nset);
				return;
			}

			// すでにタイルが敷かれていたら 1 つ右のマスへ
			if (get_board(0, j)) {
				dfs(j + 1);
				return;
			}

			// 各ピースを置くことができるかをチェックする．
			repe(piece, pieces) {
				bool ok = true;
				for (auto& [di, dj] : piece) {
					if (get_board(0 + di, j + dj)) {
						ok = false;
						break;
					}
				}
				if (!ok) continue;

				for (auto& [di, dj] : piece) board[0 + di][j + dj] = true;
				dfs(j + 1);
				for (auto& [di, dj] : piece) board[0 + di][j + dj] = false;
			}
		};
		dfs(0);
	}

	// dp_i[set] : i 行目までみて，盤の上 2 行のパターンが set である敷き詰め方の数
	vl dp(1LL << (2 * w));
	dp[0] = 1;

	rep(hoge, h) {
		vl ndp(1LL << (2 * w));
		repb(set, 2 * w) repe(nset, mat[set]) ndp[nset] += dp[set];
		dp = move(ndp);
	}

	return dp[0];
}

