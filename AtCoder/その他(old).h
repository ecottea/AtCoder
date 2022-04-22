#pragma once
#include "header.h"


//【集合の分割の列挙】O((n / log n)^n)
/*
* n 点集合の分割を全出力する．
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


//【並列二分探索】
template <typename T>
void parallel_binary_search(vector<T>& oks, vector<T>& ngs,
	function<void(unordered_map<T, vector<pair<int, bool>>>&)>& okQs)
{
	int q = sz(oks);

	while (true) {
		unordered_map<T, vector<pair<int, bool>>> mid_to_ib;

		bool update = false;
		rep(i, q) {
			if (abs(oks[i] - ngs[i]) == 1) continue;
			update = true;

			T mid = (oks[i] + ngs[i]) / 2;
			mid_to_ib[mid].push_back({ i, false });
		}
		if (!update) break;

		okQs(mid_to_ib);
		dumpel(mid_to_ib);

		repe(tmp, mid_to_ib) {
			T mid = tmp.first;

			repe(ib, tmp.second) {
				int i; bool b;
				tie(i, b) = ib;

				if (b) oks[i] = mid;
				else ngs[i] = mid;
			}
		}
	}
}


//【0-1 ナップサック問題（個数が小）】O(2^(N/2) N)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，重さ W 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
*
*（半分全列挙，グレイコード）
*/
ll knapsack01_problem_gray(const vl& v, vl& w, ll W) {
	// 参考：https://qiita.com/keymoon/items/6cf46473b5421bfe1d48
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_H

	// せっかくグレイコードを使っても，二分探索を使用しているせいで計算量は改善しない．
	// 重さがちょうど W という問題なら代わりにハッシュが使えるので計算量が改善する．

	int N = sz(v); // 品物の個数

	// 前後半それぞれの品物の個数
	int N_a = N / 2;
	int N_b = (N + 1) / 2;

	// 前半の品物の部分集合の重さと価値
	ll W_a = 0, V_a = 0;

	// 前半の品物の部分集合の重さに価値を対応付けるリスト
	map<ll, ll> W_a_to_V_a;

	// i = 0 に対応する処理
	W_a_to_V_a[0] = 0;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << N_a) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			W_a += w[change_index];
			V_a += v[change_index];
		}
		else {
			W_a -= w[change_index];
			V_a -= v[change_index];
		}

		// 重さが上限以下ならばリストに追加
		if (W_a <= W) {
			W_a_to_V_a[W_a] = max(W_a_to_V_a[W_a], V_a);
		}
	}

	// リストを再利用し，その重さ以下での最大価値に更新しておく．
	ll max_V_a = 0;
	for (auto& p : W_a_to_V_a) {
		p.second = max(p.second, max_V_a);
		max_V_a = p.second;
	}

	// 後半の品物の部分集合の重さと価値
	ll W_b = 0, V_b = 0;

	// i = 0 に対応する処理（v は全体での最大価値）
	ll V = W_a_to_V_a.rbegin()->second;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << N_b) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			W_b += w[N_a + change_index];
			V_b += v[N_a + change_index];
		}
		else {
			W_b -= w[N_a + change_index];
			V_b -= v[N_a + change_index];
		}

		// 重さが上限以下ならば前半のリストと照合し最高価値で更新する．
		if (W_b <= W) {
			// 価値が w - wb 以下である前半の部分集合の最大価値を見つける．
			auto it = W_a_to_V_a.upper_bound(W - W_b);
			it--;

			V = max(V, it->second + V_b);
		}
	}

	return V;
}




