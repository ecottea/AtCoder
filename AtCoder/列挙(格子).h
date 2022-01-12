#pragma once
#include "header.h"
// ■■■■■ 格子の列挙 ■■■■■


//【ポリオミノの列挙】O(?)
/*
* n 以下の k について，k-オミノを列挙する．（n <= 8 くらいが限界かも）
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
					int nx = x + dx4[j];
					int ny = y + dy4[j];
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

	/* デバッグ出力用
	int main() {
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

