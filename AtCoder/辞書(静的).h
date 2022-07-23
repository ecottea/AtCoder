#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 静的辞書 ■■■■■


//【ウェーブレット行列】
/*
* Wavelet_matrix(vl a) : O(n log n log A)
*	非負整数列 a[0..n) で初期化する．（A = max(a) とおく．）
*
* ll get(int i) : O(log(max a))
*	昇順で i 番目の要素を返す．
*
* ll get(int l, int r, int i) : O(log A))
*	a[l..r) の中で昇順で i 番目の要素を返す．
*
* int count(int l, int r, ll v) : O(log A)
*	a[l..r) に v が何個あるかを返す．
*
* int count(int l, int r, ll v0, ll v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*
* int position(ll v, int c) : O(log n log A)
*	昇順で c 番目の v の位置を返す．
*
* frequency(int l, int r, int c, vector<pli>& freq) : O(min(r - l, A) log A)
*	a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組のリストを freq に格納する．
*
* ll sum(int l, int r) : O(1)
*	a[l..r) の和を返す．
*
* ll sum(int l, int r, ll v0, ll v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) : O(min((r1 - l1) + (r2 - l2), A) log A)
*	a[l1..r1) と a[l2..r2) に共通する要素を求め，
*	その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
*/
class Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	int k; // msb 以下の桁数（1-indexed）
	vvb bs; // bs[j][i] : 第 j + 1 ビットについての安定ソート後の a[i] の第 j ビット
	vvvi bs_acc; // bs[b] : のビット b = 0, 1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	unordered_map<ll, int> id; // 値 → 安定ソートが終わったときの最左位置
	vvl acc; // acc[j] : 第 j ビットについての安定ソート後の a の累積和

	// a[0..r) に v が何個あるかを返す．
	int count_sub(int r, ll v) {
		// 一つも無ければすぐに 0 を返す．
		if (!id.count(v)) return 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			// 注目ビットに応じて次の位置を求めていく．
			if (v & (1LL << j)) {
				r = num_zeros[j] + bs_acc[1][j][r];
			}
			else {
				r = bs_acc[0][j][r];
			}
		}

		return r - id[v];
	}

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, ll v) {
		if (msb(v) >= k) return r - l;

		int cnt = 0;
		repir(j, k - 1, 0) {
			if (v & (1LL << j)) {
				cnt += bs_acc[0][j][r] - bs_acc[0][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return cnt;
	}

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	ll sum_rsub(int l, int r, ll v) {
		if (msb(v) >= k) return acc[k][r] - acc[k][l];

		ll res = 0;
		repir(j, k - 1, 0) {
			if (v & (1LL << j)) {
				res += acc[j][bs_acc[0][j][r]] - acc[j][bs_acc[0][j][l]];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return res;
	}

public:
	// 非負整数列 t で初期化する．
	Wavelet_matrix(const vl& t)
		: n(sz(t)), k(msb(*max_element(all(t))) + 1),
		bs(k, vb(n)), bs_acc(2, vvi(k, vi(n + 1))), num_zeros(k), acc(k + 1, vl(n + 1))
	{
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		// ビットと組にして安定ソートするためのリスト
		vector<pair<bool, ll>> bt(n);
		rep(i, n) bt[i].second = t[i];

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, k - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[j][i] = bt[i].first = (bt[i].second & (1LL << j));

				// ビット 0, 1 それぞれの個数の累積和を求めておく．
				rep(b, 2) bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				if (bs[j][i]) {
					bs_acc[1][j][i + 1]++;
				}
				else {
					bs_acc[0][j][i + 1]++;
					num_zeros[j]++;
				}

				// 要素の累積和の計算
				acc[j + 1][i + 1] = acc[j + 1][i] + bt[i].second;
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			stable_sort(all(bt), [](auto const& lhs, auto const& rhs) {
				return lhs.first < rhs.first;
				});
		}

		rep(i, n) {
			// 値 → 安定ソートが終わったときの最左位置
			if (!id.count(bt[i].second)) id[bt[i].second] = i;

			// 要素の累積和の計算
			acc[0][i + 1] = acc[0][i] + bt[i].second;
		}
	}

	Wavelet_matrix() : n(0), k(0) {} // ダミー

	// 昇順で i 番目の要素を返す．
	ll get(int i) {
		ll res = 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			res *= 2;

			// 注目ビットに応じて次の位置を求めつつ，値を更新していく．
			if (bs[j][i]) {
				res++;
				i = num_zeros[j] + bs_acc[1][j][i];
			}
			else {
				i = bs_acc[0][j][i];
			}
		}

		return res;
	}

	// a[l..r) に v が何個あるかを返す．
	int count(int l, int r, ll v) {
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		return count_sub(r, v) - count_sub(l, v);
	}

	// 昇順で c 番目の v の位置を返す．
	int position(ll v, int c) {
		if (!id.count(v)) return -1;

		int i = id[v] + c;
		rep(j, k) {
			if (v & (1LL << j)) {
				i = ubpos(bs_acc[1][j], i - num_zeros[j]) - 1;
			}
			else {
				i = ubpos(bs_acc[0][j], i - num_zeros[j]) - 1;
			}
		}

		return i;
	}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	ll get(int l, int r, int i) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

		ll res = 0;

		repir(j, k - 1, 0) {
			res *= 2;

			int cnt0 = bs_acc[0][j][r] - bs_acc[0][j][l];
			if (i >= cnt0) {
				res++;
				l = num_zeros[j] + bs_acc[1][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				i -= cnt0;
			}
			else {
				l = bs_acc[0][j][l];
				r = bs_acc[0][j][r];
			}
		}

		return res;
	}

	// a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組を返す．
	void frequency(int l, int r, int c, vector<pli>& freq) {
		freq.clear();

		priority_queue<tuple<int, int, int, int, ll>> q;
		q.push({ r - l, k - 1, l, r, 0 });

		// 出現頻度の高い値が多ければ c に応じて早めに打ち切られるが，
		// そうでなければ最悪 a[l..r) を調べ尽くしてしまう．
		while (!q.empty()) {
			int w, j;
			ll v;
			tie(w, j, l, r, v) = q.top();
			q.pop();

			if (j == -1) {
				freq.push_back({ v, w });
				if (--c == 0) return;
			}
			else {
				int l1 = num_zeros[j] + bs_acc[1][j][l];
				int r1 = num_zeros[j] + bs_acc[1][j][r];
				int l0 = bs_acc[0][j][l];
				int r0 = bs_acc[0][j][r];

				q.push({ r1 - l1, j - 1, l1, r1, 2 * v + 1 });
				q.push({ r0 - l0, j - 1, l0, r0, 2 * v });
			}
		}
	}

	// a[l..r) の和を返す．
	ll sum(int l, int r) {
		return acc[k][r] - acc[k][l];
	}

	// a[l1..r1) と a[l2..r2) に共通する要素を求め，
	// その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
	void intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) {
		freq.clear();

		queue<tuple<int, int, int, int, int, ll>> q;
		q.push({ k - 1, l1, r1, l2, r2, 0 });

		while (!q.empty()) {
			int j; ll v;
			tie(j, l1, r1, l2, r2, v) = q.front(); q.pop();

			if (l1 == r1 || l2 == r2) continue;

			if (j == -1) {
				freq.push_back({ v, r1 - l1, r2 - l2 });
			}
			else {
				int l11 = num_zeros[j] + bs_acc[1][j][l1];
				int r11 = num_zeros[j] + bs_acc[1][j][r1];
				int l10 = bs_acc[0][j][l1];
				int r10 = bs_acc[0][j][r1];
				int l21 = num_zeros[j] + bs_acc[1][j][l2];
				int r21 = num_zeros[j] + bs_acc[1][j][r2];
				int l20 = bs_acc[0][j][l2];
				int r20 = bs_acc[0][j][r2];

				q.push({ j - 1, l11, r11, l21, r21, 2 * v + 1 });
				q.push({ j - 1, l10, r10, l20, r20, 2 * v });
			}
		}
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, ll v0, ll v1) {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_c

		chmax(v0, 0LL); chmin(v1, (1LL << k) - 1);
		if (v0 >= v1) return 0;

		return count_rsub(l, r, v1) - count_rsub(l, r, v0);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	ll sum(int l, int r, ll v0, ll v1) {
		// verify : https://yukicoder.me/problems/no/924

		chmax(v0, 0LL); chmin(v1, (1LL << k) - 1);
		if (v0 >= v1) return 0;

		return sum_rsub(l, r, v1) - sum_rsub(l, r, v0);
	}
};


//【部分文字列辞書】
/*
* Substring_dictionary(s) : O(n)
*	文字列 s[0..n) の部分文字列（空文字列は除く）で初期化する．
*
* ll size() : O(1)
*	部分文字列の個数を返す．
*
* string get(ll i) : O(|sub| + log n)
*	辞書順で i 番目の部分文字列 sub を返す．（0-indexed, なければ "" を返す）
*/
struct Substring_dictionary {
	int n;
	string s;

	// sa[i] : s の接尾辞 s[j..n) のうち辞書順 i 番目のものの先頭位置 j（0-indexed）
	// la[i] : s[sa[i]..n) と s[sa[i+1]..n) の最長共通接頭辞の長さ
	vi sa, la;

	// cnt[i] : s の部分文字列のうち，s[sa[i]..n) 以下のものの個数
	vl cnt;

	// コンストラクタ
	Substring_dictionary() : n(0) {}
	Substring_dictionary(const string& s_) : n(sz(s_)), s(s_), cnt(n) {
		sa = suffix_array(s);
		la = lcp_array(s, sa);

		cnt[0] = n - sa[0];
		repi(i, 1, n - 1) {
			cnt[i] = cnt[i - 1] + (n - sa[i]) - la[i - 1];
		}
	}

	// 部分文字列の個数を返す．
	ll size() { return cnt[n - 1]; }

	// i 番目の部分文字列を返す．
	string get(ll i) {
		i++; // 1-indexed に直す

		// i 番目の部分文字列がどの接尾辞 s[sa[k]..n) の接頭辞かを探す．O(log n)
		auto it = lower_bound(all(cnt), i);
		if (it == cnt.end()) return "";
		int k = (int)distance(cnt.begin(), it);

		// i から cnt[k] に足りない分だけ後ろの文字を削ったものが求める部分文字列．
		// c++ は s.substr(i, w) : s[i..i+w) なので注意．
		return s.substr(sa[k], n - (*it - i) - sa[k]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Substring_dictionary& sd) {
		cout << sd.n << endl << sd.s << endl << sd.sa << endl << sd.la << endl
			<< sd.cnt << endl;
		return os;
	}
#endif
};


//【組の和の辞書】
/*
* Outer_sum_dictionary(vl a, vl b) : O(n log n + m log m)
*	S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* ll lower_bound(ll v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* ll upper_bound(ll v) : O(n log m)
*	S の v 以下の要素の個数を返す．
*
* ll get(ll i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目の要素を返す．
*
* ll sum(ll i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目未満の要素の和を返す．
*
* 利用：【めぐる式二分探索】
*/
struct Outer_sum_dictionary {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_e

	int n, m;
	vl a, b, acc_b;

	Outer_sum_dictionary() : n(0), m(0) {}

	// S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_sum_dictionary(const vl& a_, const vl& b_)
		: n(sz(a_)), m(sz(b_)), a(a_), b(b_)
	{
		sort(all(a)); sort(all(b));

		acc_b = vl(m + 1);
		rep(j, m) {
			acc_b[j + 1] = acc_b[j] + b[j];
		}
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		ll cnt = 0;
		rep(i, n) cnt += lbpos(b, v - a[i]);
		return cnt;
	}

	// S の v 以下の要素の個数を返す．
	ll upper_bound(ll v) {
		ll cnt = 0;
		rep(i, n) cnt += ubpos(b, v - a[i]);			
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(a[0] + b[0] - 1, a[n - 1] + b[m - 1] + 1, okQ);
	}

	// S の i 番目未満の要素の和を返す．
	ll sum(ll i) {
		// v : i 番目の要素 
		ll v = get(i);

		// sum : v 未満の要素の和, cnt : v 未満の要素の個数
		ll sum = 0, cnt = 0;
		rep(i, n) {
			int d = lbpos(b, v - a[i]);
			sum += a[i] * d + acc_b[d];
			cnt += d;
		}

		// 残り i - cnt 個の要素はちょうど v であるからその分を加算する．
		sum += v * (i - cnt);

		return sum;
	}
};


//【組の積の辞書】
/*
* Outer_mul_dictionary(vl a, vl b) : O(n log n + m log m)
*	S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* ll lower_bound(ll v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* ll get(ll i) : O(n log m log(INFL))
*	S の i 番目の要素を返す．
*
* 利用：【めぐる式二分探索】
*/
struct Outer_mul_dictionary {
	// verify : https://atcoder.jp/contests/arc037/tasks/arc037_c

	// n, m : a, b の要素数
	// np, mp : a, b の 正の要素数
	// nz, mz : a, b の 0 の要素数
	// nn, mn : a, b の 負の要素数
	int n, np, nz, nn, m, mp, mz, mn;

	// ap, bp : a, b の正の要素を昇順に格納したリスト
	// an, bn : a, b の負の要素の 絶対値 を昇順に格納したリスト
	vl ap, an, bp, bn;

	Outer_mul_dictionary() : n(0), np(0), nz(0), nn(0), m(0), mp(0), mz(0), mn(0) {}

	// S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_mul_dictionary(const vl& a, const vl& b) {
		np = nz = nn = 0;
		repe(x, a) {
			if (x > 0) {
				ap.push_back(x);
				np++;
			}
			else if (x < 0) {
				an.push_back(-x);
				nn++;
			}
			else {
				nz++;
			}
		}
		sort(all(ap)); sort(all(an));
		n = np + nz + nn;

		mp = mz = mn = 0;
		repe(x, b) {
			if (x > 0) {
				bp.push_back(x);
				mp++;
			}
			else if (x < 0) {
				bn.push_back(-x);
				mn++;
			}
			else {
				mz++;
			}
		}
		sort(all(bp)); sort(all(bn));
		m = mp + mz + mn;
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		ll cnt = 0;
		if (v > 0) {
			cnt += (ll)m * n - (ll)np * mp - (ll)nn * mn;
			repe(x, ap) cnt += lbpos(bp, (v + x - 1) / x);
			repe(x, an) cnt += lbpos(bn, (v + x - 1) / x); 
		}
		else if (v < 0) {
			repe(x, ap) {
				auto it = std::upper_bound(all(bn), -v / x);
				cnt += (ll)distance(it, bn.end());
			}
			repe(x, an) {
				auto it = std::upper_bound(all(bp), -v / x);
				cnt += (ll)distance(it, bp.end());
			}
		}
		else {
			cnt += (ll)np * mn + (ll)nn * mp;
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(-INFL, INFL, okQ);
	}
};


//【k-D 木（集合）】
/*
* KDTree<S>(vvS p) : O(n log n)
*	n 個の点 p[i] で初期化する．
*	制約 : 点の座標は互いに異なる．
*
* T search(vS p1, vS p2, vi& res) : O(n^(1 - 1/dim) + |res|)
*	半開直方体 R = [p1, p2) 内の点の番号を res に格納する．
*/
template <class S> struct KDTree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C

	using vS = vector<S>; using vvS = vector<vS>; using vvSi = vector<pair<vS, int>>;
	const S S_MIN = numeric_limits<S>::lowest(), S_MAX = numeric_limits<S>::max();

	struct Node {
		vS p1, p2; // 半開直方体 R = [p1, p2) に対応するノードであることを表す
		int id; // R 内の点の注目座標に関する中央値に位置する点の番号
		Node* left, * right;

		Node() {}
		Node(vS p1_, vS p2_, int id_) : p1(p1_), p2(p2_), id(id_), left(nullptr), right(nullptr) {}
	};

	vvS p; // 点群
	Node* root; // 根へのポインタ
	int dim; // 次元

	// n 個の点 p[i] で初期化する．
	KDTree(vvS& p_) : p(p_), root(nullptr), dim(sz(p[0])) {
		int n = sz(p);

		vvSi pi(n);
		rep(i, n) pi[i] = { p[i], i };

		split(root, vS(dim, S_MIN), vS(dim, S_MAX), pi, 0, n, 0);
	}

	// ノード *t に点 p[i0..i1) を割り当て分割する．
	void split(Node*& t, vS p1, vS p2, vvSi& pi, int i0, int i1, int d) {
		// 空なら何もしない．
		if (i0 >= i1) return;

		// 要素が一つだけなら葉として格納して帰る．
		if (i0 + 1 == i1) {
			vS p0_inc = pi[i0].first;
			rep(i, dim) p0_inc[i]++;

			t = new Node(pi[i0].first, p0_inc, pi[i0].second);
			return;
		}

		// 中央値を得る．
		vector<pair<S, int>> cds;
		repi(i, i0, i1 - 1) cds.push_back({ pi[i].first[d], pi[i].second });
		sort(all(cds));
		S med = cds[sz(cds) / 2].first;

		// med を閾値として用い，それ未満のものを左，以上のものを右に移動する．
		int i = i0; // i : pi[i0, i) が med 未満の座標と確定
		repi(j, i0, i1 - 1) { // j : pi[i, j) が med 以上の座標と確定
			// j の位置に med 未満の座標がある場合
			if (pi[j].first[d] < med) {
				// 最も左の med 以上の座標と交換する．
				swap(pi[i], pi[j]);
				i++;
			}
		}

		// pi[i] に座標がちょうど med のものをもってくる．
		repi(j, i, i1 - 1) {
			if (pi[j].first[d] == med) {
				swap(pi[i], pi[j]);
				break;
			}
		}

		// med で分割して左右の子を作りに行く．
		t = new Node(p1, p2, 0);
		t->id = pi[i].second;
		vS p1m = p1, p2m = p2;
		p1m[d] = med; p2m[d] = med;
		split(t->left, p1, p2m, pi, i0, i, (d + 1) % dim);
		split(t->right, p1m, p2, pi, i + 1, i1, (d + 1) % dim);
	}

	// 半開直方体 R = [p1, p2) 内の点の番号を res に格納する．
	void search(const vS& p1, const vS& p2, vi& res) { return search_rf(root, p1, p2, 0, res); }

	void search_rf(Node* t, const vS& p1, const vS& p2, int d, vi& res) {
		// 木が空の場合
		if (t == nullptr) return;

		// 点が探索範囲に入っている場合は点を記録する
		bool in_flag = true;
		rep(i, dim) {
			if (p[t->id][i] < p1[i] || p2[i] <= p[t->id][i]) in_flag = false;
		}
		if (in_flag) res.push_back(t->id);

		// 探索範囲の左端が記録範囲の中央値以下なら左の子を調べにいく．
		if (p1[d] <= p[t->id][d]) {
			search_rf(t->left, p1, p2, (d + 1) % dim, res);
		}

		// 探索範囲の右端が記録範囲の中央値以上なら右の子を調べにいく．
		if (p2[d] >= p[t->id][d]) {
			search_rf(t->right, p1, p2, (d + 1) % dim, res);
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const KDTree& kd) {
		kd.print_rf(os, kd.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "id:" << t->id << ", p1:" << t->p1 << ", p2:" << t->p2 << ", left:";
		t->left != nullptr ? os << "(" << t->left->p1 << "," << t->left->p2 << ")" : os << "-";
		os << ", right:";
		t->right != nullptr ? os << "(" << t->right->p1 << "," << t->right->p2 << ")" : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


//【k-D トライ（写像）】
/*
* KDTrie(vvS p, vT val) : O(n log n)
*	n 個の点 p[i] に値 val[i] を持たせて初期化する．
*	制約 : 点の座標は互いに異なる．
*
* T sum(vS p1, vS p2) : O(n^(1 - 1/dim))
*	半開直方体 R = [p1, p2) 内の点の値の和を返す．
*/
template <class S, class T> struct KDTrie {
	// verify : https://atcoder.jp/contests/abc075/tasks/abc075_d

	using vS = vector<S>; using vvS = vector<vS>; using vT = vector<T>;
	const S S_MIN = numeric_limits<S>::lowest(), S_MAX = numeric_limits<S>::max();

	struct Node {
		vS p1, p2; // 半開直方体 R = [p1, p2) に対応するノードであることを表す
		T val;
		Node *left, *right;

		Node() {}
		Node(vS p1_, vS p2_, T val_) : p1(p1_), p2(p2_), val(val_), left(nullptr), right(nullptr) {}
	};

	Node* root; // 根へのポインタ
	int dim; // 次元

	// n 個の点 p[i] に値 val[i] を持たせて初期化する．
	KDTrie(vvS& p, vT& val) : root(nullptr), dim(sz(p[0])) {
		int n = sz(p);
		split(root, vS(dim, S_MIN), vS(dim, S_MAX), p, val, 0, n, 0);
	}

	// ノード *t に点 p[i0..i1) と値 val[i0..i1) を割り当て分割する．
	T split(Node*& t, vS p1, vS p2, vvS& p, vT& val, int i0, int i1, int d) {
		// 空なら何もしない．
		if (i0 == i1) return 0;

		// 要素が一つだけなら葉として格納して帰る．
		if (i0 + 1 == i1) {
			vS p0_inc = p[i0];
			rep(i, dim) p0_inc[i]++;

			t = new Node(p[i0], p0_inc, val[i0]);
			return val[i0];
		}

		// 中央値を得る．
		vS cds;
		repi(i, i0, i1 - 1) cds.push_back(p[i][d]);
		uniq(cds);
		S med = cds[sz(cds) / 2];

		// med を閾値として用い，それ未満のものを左，以上のものを右に移動する．
		int i = i0; // i : p[i0, i) が med 未満の座標と確定
		repi(j, i0, i1 - 1) { // j : p[i, j) が med 以上の座標と確定
			// j の位置に med 未満の座標がある場合
			if (p[j][d] < med) {
				// 最も左の med 以上の座標と交換する．
				swap(p[i], p[j]); swap(val[i], val[j]);
				i++;
			}
		}

		// med で分割して左右の子を作りに行く．
		t = new Node(p1, p2, 0);
		vS p1m = p1, p2m = p2;
		p1m[d] = med; p2m[d] = med;
		t->val += split(t->left, p1, p2m, p, val, i0, i, (d + 1) % dim);
		t->val += split(t->right, p1m, p2, p, val, i, i1, (d + 1) % dim);

		return t->val;
	}

	// 半開直方体 R = [p1, p2) 内の点の値の和を返す．
	T sum(const vS& p1, const vS& p2) { return sum_rf(root, p1, p2, 0); }

	T sum_rf(Node* t, const vS& p1, const vS& p2, int d) {
		// 木が空の場合
		if (t == nullptr) return 0;

		// 一部も範囲に入っていない場合
		if (p2[d] <= t->p1[d] || t->p2[d] <= p1[d]) return 0;

		// 完全に範囲に入っている場合
		bool in_flag = true;
		rep(i, dim) {
			if (t->p1[i] < p1[i] || p2[i] < t->p2[i]) in_flag = false;
		}
		if (in_flag) return t->val;

		T val = 0;
		val += sum_rf(t->left, p1, p2, (d + 1) % dim);
		val += sum_rf(t->right, p1, p2, (d + 1) % dim);

		return val;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const KDTrie& kd) {
		kd.print_rf(os, kd.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "val:" << t->val << ", p1:" << t->p1 << ", p2:" << t->p2 << ", left:";
		t->left != nullptr ? os << "(" << t->left->p1 << "," << t->left->p2 << ")" : os << "-";
		os << ", right:";
		t->right != nullptr ? os << "(" << t->right->p1 << "," << t->right->p2 << ")" : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


