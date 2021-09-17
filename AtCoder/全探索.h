#pragma once
#include "header.h"
// ■■■■■ 全探索 ■■■■■


// 【中間集合の全探索】
/*
* 大きさ d の全体集合 Ω のうち，部分集合 set⊂Ω を含む
* 中間集合 mid⊂Ω を昇順に全探索する．
*/
#define repbm(mid, set, d) for(int mid = set; mid < (1 << int(d)); mid = (mid + 1) | set)


// 【部分集合の全探索】
/*
* 全体集合 Ω の部分集合 set⊂Ω について，set の部分集合 sub⊂set を降順に全探索する．
*/
#define repbs(sub, set) for(int sub = set, bsub = 1; bsub > 0; bsub = sub, sub = (sub - 1) & set) 


// 【部分集合の全探索（大きさ固定）】
/*
* 大きさ d の全体集合 Ω のうち，大きさ k の部分集合 set⊂Ω を昇順に全探索する．
*/
#define repbc(set, k, d) for(int set = (1 << k) - 1, lb, nx; set < (1 << n); lb = set & -set, nx = set + lb, set = (((set & ~nx) / lb) >> 1) | nx)


//【広義単調増加列の列挙】O(binomial(n + m - 1, n))
/*
* 0 <= a[0] <= a[1] <= ... <= a[n-1] < m なる列 a を列挙する
*
* 戻り値 : 列が何通りあるか（二項係数 binomial(n + m - 1, n)）
*/
int weakly_increase_sequence(int n, int m) {
	vi a(n);
	int res = 0;

	// len : 列の長さ
	function<void(int)> rf = [&](int len) {
		// 列の長さが n の場合
		if (len == n) {
			// 完成しているので出力
			cout << a << endl;

			res++;
			return;
		}

		int i0 = (len > 0 ? a[len - 1] : 0);
		repi(i, i0, m - 1) {
			a[len++] = i;
			rf(len);
			len--;
		}
	};

	rf(0);

	return res;
}


//【2 選択和数え上げ問題（ビット全探索）】O(2^n n)
/*
* 長さ N の列 a, b と目標値 sum が与えられる．
* 各 i ごとに a[i] か b[i] のいずれかを選択して得られる和が
* ちょうど sum になるものの個数を返す．
*/
template <class T>
int count_2select_sum(vector<T>& a, vector<T>& b, T sum) {
	int n = sz(a);

	int cnt = 0;

	// set の i ビット目が a[i] と b[i] のどちらを選択するかを表す．
	repb(bit, n) {
		// 選択されたものの和を計算する．
		T v = 0;
		rep(i, n) {
			// set の i ビット目が 1 であれば a[i] を選択する．
			if (bit & (1 << i)) {
				v += a[i];
			}
			// set の i ビット目が 1 であれば b[i] を選択する．
			else {
				v += b[i];
			}
		}

		// 和が目標に一致していればカウントする．
		if (v == sum) {
			cnt++;
		}
	}

	return cnt;
}


//【3 選択和数え上げ問題（3 進全探索）】O(3^n n)
/*
* 長さ N の列 a, b, c と目標値 sum が与えられる．
* 各 i ごとに a[i], b[i], c[i] のいずれかを選択して得られる和が
* ちょうど sum になるものの個数を返す．
*/
template <class T>
int count_3select_sum(vector<T>& a, vector<T>& b, vector<T>& c, T sum) {
	int n = sz(a);

	int cnt = 0;

	// tit が 3 進数表記されているものと考える．
	// tit の i 桁目が a[i], b[i], c[i] のいずれを選択するかを表す．
	rep(tit, pow(3, n)) {
		// 選択されたものの和を計算する．
		T v = 0;
		rep(i, n) {
			// tit の i 桁目を抜き出す．
			switch ((tit / pow(3, i)) % 3) {
			case 0:
				v += a[i];
				break;
			case 1:
				v += b[i];
				break;
			case 2:
				v += c[i];
				break;
			default:;
			}
		}

		// 和が目標に一致していればカウントする．
		if (v == sum) {
			cnt++;
		}
	}

	return cnt;
}


//【1 の連続しないビット列の列挙】O(1.6^n)
/*
* 長さ n のビット列のうち 1 が連続しないものを全出力する．
*
* 戻り値 : 列が何通りあるか（フィボナッチ数 F_(n+1)）
*/
int noncontinuous_bitsequence(int n) {
	ll b = 0;
	int res = 0;

	function<void(int, int)> rf = [&](int len, int d) {
		// 残りの長さが 0 の場合
		if (len == 0) {
			// ビット列が完成しているので出力
			rep(i, n) {
				if (b & (1LL << i)) {
					cout << '1';
				}
				else {
					cout << '0';
				}
			}
			cout << endl;

			res++;
			return;
		}

		// 直前の桁が 0 である場合は 1 を使える．
		if (d == 0) {
			b = b * 2 + 1;
			rf(len - 1, 1);
			b = (b - 1) / 2;
		}
		// 0 を使う．
		b *= 2;
		rf(len - 1, 0);
		b /= 2;

		return;
	};

	rf(n, 0);

	return res;
}


//【括弧列の列挙】O(4^n n^(-3/2))
/*
* 長さ 2 n の括弧列を辞書順に全出力する．
*
* 戻り値 : 括弧列が何通りあるか（カタラン数 C_n）
*/
int parenthesis_sequence(int n) {
	string s = "";
	int res = 0;

	function<void(int, int)> rf = [&](int l, int r) {
		// 左右の括弧を使い切っている場合
		if (l == 0 && r == 0) {
			// 長さ 2 n の括弧列が完成しているので出力する．
			cout << s << endl;

			res++;
			return;
		}

		// 左括弧をまだ使える場合
		if (l >= 1) {
			s += '(';
			rf(l - 1, r);
			s.pop_back();
		}
		// 左括弧が十分あり右括弧を使える場合
		if (r > l) {
			s += ')';
			rf(l, r - 1);
			s.pop_back();
		}
		return;
	};

	rf(n, n);

	return res;
}


//【自然数の分割の列挙】O(?)
/*
* 自然数 n を k 以下の数に分割する方法を全出力する．
*
* 戻り値 : 分割の個数（k = n なら分割数 p(n)）
*/
int integer_partitions(int n, int k) {
	map<int, int> ip;
	int res = 0;

	// 再帰用の関数
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			// 分割の出力
			repe(p, ip) {
				rep(i, p.second) {
					cout << p.first << " ";
				}
			}
			cout << endl;

			res++;
			return;
		}

		// 分割に使える数がもうない場合
		if (k == 0) {
			return;
		}

		// n が k 以上のときは，n を k と n-k に分割できる．
		if (n >= k) {
			ip[k]++;
			rf(n - k, k);
			ip[k]--;
			if (ip[k] == 0) {
				ip.erase(k);
			}
		}

		// これ以上 n の分割に k を使わない場合
		rf(n, k - 1);
	};

	rf(n, k);

	return res;
}


//【集合の分割の列挙】O((n / log n)^n)
/*
* 大きさ n の集合の分割を全出力する．
*
* 戻り値 : 分割の個数（ベル数 B_n）
*/
int set_partitions(int n) {
	vector<int> sets;
	int res = 0;

	function<void(int)> rf = [&](int e) {
		// 全ての要素を集合に割り振り終わった場合
		if (e == n) {
			// 分割を出力する．
			repe(set, sets) {
				rep(i, n) {
					if (set & (1 << i)) {
						cout << 1;
					}
					else {
						cout << 0;
					}
				}
				cout << " ";
			}
			cout << endl;

			res++;
			return;
		}

		// 要素 e を割り当てる集合 set それぞれについて
		rep(i, sz(sets)) {
			sets[i] += 1 << e;
			rf(e + 1);
			sets[i] -= 1 << e;
		}

		// 要素 e を単独で新たな集合とする場合
		sets.push_back({ 1 << e });
		rf(e + 1);
		sets.pop_back();

		return;
	};

	rf(0);

	return res;
}


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

