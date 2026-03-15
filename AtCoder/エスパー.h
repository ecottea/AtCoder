#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ エスパー ■■■■■


//【線形漸化式の発見】O(n^2)
/*
* 与えられた数列 a[0..n) に対し，以下の等式を満たす c[0..m) で m を最小とするものを返す：
*		a[i] = Σj∈[0..m) c[j] a[i-1-j]  (∀i∈[m..n))
*
* 制約 : mint::mod は大きい素数
*/
vm berlekamp_massey(const vm& a) {
	// 参考 : https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
	// verify : https://judge.yosupo.jp/problem/find_linear_recurrence

	vm S(a), C{ 1 }, B{ 1 };
	int N = sz(a), m = 1; mint b = 1;

	rep(n, N) {
		mint d = 0;
		rep(i, sz(C)) d += C[i] * S[n - i];

		if (d == 0) {
			m++;
		}
		else if (2 * (sz(C) - 1) <= n) {
			vm T(C);

			mint coef = d * b.inv();
			C.resize(max(sz(C), sz(B) + m));
			rep(j, sz(B)) C[j + m] -= coef * B[j];

			B = T;
			b = d;
			m = 1;
		}
		else {
			mint coef = d * b.inv();
			C.resize(max(sz(C), sz(B) + m));
			rep(j, sz(B)) C[j + m] -= coef * B[j];

			m++;
		}
	}

	C.erase(C.begin());
	rep(i, sz(C)) C[i] *= -1;

	return C;
}


//【変数係数線形漸化式の発見】O(n L^2 D^2 + N (L D + log(mod)))
/*
* 係数多項式の次数が D 次未満の L 項間漸化式
*	Σi∈[0..L) Σj∈[0..D) c(i,j) (m+i)^j a[m+i] = 0
* の存在を仮定して a[0..n) を延長し a[0..N] にする（失敗したら false を返す）
*
* 制約 : n ≧ L(D+1)-1（ランク落ちしてるとこれでも足りないかも）
*
* 利用：【行列】,【線形方程式】
*/
bool p_recursive(int N, vm& a, int L, int D, vm* coef = nullptr) {
	// verify : https://atcoder.jp/contests/abc222/tasks/abc222_h

	int n = sz(a);

	// 既に十分な長さがある場合はそのままで良い．
	if (N <= n - 1) {
		a.resize(N + 1);
		return true;
	}

	// 式が足りないといつでも非自明解をもってしまって意味がない（とも限らない → LLL）
	if (n < L * (D + 1) - 1) return false;

	// 行列方程式 A x = 0 を解いて一般解の基底 xs を求める．
	Matrix<mint> A(n - L + 1, L * D);
	repi(n0, 0, n - L) {
		rep(i, L) rep(j, D) {
			A[n0][i * D + j] = mint(n0 + i).pow(j) * a[n0 + i];
		}
	}
	vvm xs;
	gauss_jordan_elimination(A, vm(n - L + 1), &xs);

	// 自明解 x = 0 しか存在しない場合は失敗．
	if (xs.empty()) return false;

	a.resize(N + 1);

	// 得られた非自明解 xs.back() から漸化式を復元し，それに基づき a[0..n) を延長する．
	auto& x = xs.back();
	repi(n0, n - L + 1, N - L + 1) {
		mint num = 0;
		rep(i, L - 1) {
			mint pow_n0i = 1;
			rep(j, D) {
				num += x[i * D + j] * pow_n0i * a[n0 + i];
				pow_n0i *= n0 + i;
			}
		}

		mint dnm = 0;
		mint pow_n0L = 1;
		rep(j, D) {
			dnm += x[(L - 1) * D + j] * pow_n0L;
			pow_n0L *= n0 + L - 1;
		}

		// num + dnm * a[n0 + L - 1] = 0 を解く．
		// dnm は規則正しいことが多いので O(log(mod)) が落とせるかも．
		a[n0 + L - 1] = -num / dnm;
	}

	if (coef) *coef = move(x);

	return true;
}


//【変数係数線形漸化式の発見（2 次元）】O(h w L^4 D^4 + H W (L^2 D^2 + log(mod)))
/*
* 係数多項式の次数が D 次未満の L 項間漸化式
*	Σi1,i2∈[0..L) Σj1,j2∈[0..D) c(i1,i2,j1,j2) (m1+i1)^j1 (m2+i2)^j2 a[m1+i1][m2+j2] = 0
* の存在を仮定して a[0..h)[0..w) を延長し a[0..H][0..W] にする（失敗したら false を返す）
*
* 利用：【変数係数線形漸化式の発見】
*/
bool p_recursive(int H, int W, vvm& a, int L, int D) {
	// verify : https://atcoder.jp/contests/arc162/tasks/arc162_f

	int h = sz(a), w = sz(a[0]);

	// 既に十分な大きさがある場合はそのままで良い．
	if (H <= h - 1 && W <= w - 1) {
		a.resize(H + 1);
		repi(i, 0, H) a[i].resize(W + 1);
		return true;
	}

	// cE : 式の数, cV : 未知数の数
	int cE = (h - L + 1) * (w - L + 1);
	int cV = L * D * L * D;

	// 式が足りないといつでも非自明解をもってしまって意味がない（とも限らない → LLL）
	if (h < L || cE < cV) return false;

	// 行列方程式 A x = 0 を解いて一般解の基底 xs を求める．
	Matrix<mint> A(cE, cV);
	repi(n1, 0, h - L) repi(n2, 0, w - L) {
		int n = n1 * (w - L + 1) + n2;
		rep(i1, L) rep(j1, D) rep(i2, L) rep(j2, D) {
			int k = ((i1 * D + j1) * L + i2) * D + j2;
			A[n][k] = mint(n1 + i1).pow(j1) * mint(n2 + i2).pow(j2) * a[n1 + i1][n2 + i2];
		}
	}
	vvm xs;
	gauss_jordan_elimination(A, vm(cE), &xs);

	// 自明解 x = 0 しか存在しない場合は失敗．
	if (xs.empty()) return false;

	a.resize(H + 1);
	repi(i, 0, H) a[i].resize(W + 1);

	// a[0..L)[0..W] を 1 次元 P-recursive を仮定して埋める．（L, D は見直すべき）
	rep(i, L - 1) {
		vm seq(w);
		rep(j, w) seq[j] = a[i][j];

		if (!p_recursive(W, seq, L, D)) return false;
		repi(j, 0, W) a[i][j] = seq[j];
	}

	// a[0..H][0..L) を 1 次元 P-recursive を仮定して埋める．（L, D は見直すべき）
	rep(j, L - 1) {
		vm seq(h);
		rep(i, h) seq[i] = a[i][j];

		if (!p_recursive(H, seq, L, D)) return false;
		repi(i, 0, H) a[i][j] = seq[i];
	}

	// 得られた非自明解 xs.back() から漸化式を復元し，それに基づき a[0..h)[0..w) を延長する．
	auto& x = xs.back();
	repi(n1, 0, H - L + 1) repi(n2, 0, W - L + 1) {
		mint num = 0, dnm = 0;

		rep(i1, L) {
			mint pow_ni1 = 1;
			rep(j1, D) {
				rep(i2, L) {
					mint pow_ni2 = 1;
					rep(j2, D) {
						int k = ((i1 * D + j1) * L + i2) * D + j2;
						mint coef = x[k] * pow_ni1 * pow_ni2;

						if (i1 == L - 1 && i2 == L - 1) dnm += coef;
						else num += coef * a[n1 + i1][n2 + i2];

						pow_ni2 *= n2 + i2;
					}
				}
				pow_ni1 *= n1 + i1;
			}
		}

		// num + dnm * a[n1 + L - 1][n2 + L - 1] = 0（dnm が 0 になるようなら場所を変える）
		a[n1 + L - 1][n2 + L - 1] = -num / dnm;
	}

	return true;
}


//【D-finite】
/*
* 母関数が D-finite ⇔ 数列が P-recursive なので，P-recursive ライブラリを用いれば良い．
*/


//【D-algebraic】
/*
* 母関数の満たす微分方程式を得ることができる場合がある．その後はオンライン畳込み等で処理する．
* 
* 参考 : https://maruoka842.github.io/functionFinder/
* verify : https://mojacoder.app/users/googol_S0/problems/Most_Difficult_Exam_Plus
*/


//【遷移行列推定】
/*
* DP が線形であるという予想のもとで，愚直で得た例から遷移行列を復元できる場合がある．
*
* verify : https://atcoder.jp/contests/abc200/tasks/abc200_f
*/


//【自動スライド bit DP】
/*
* DP が線形であるという予想のもとで，愚直で得た例からスライド bit DP を復元できる場合がある．
*
* verify : https://atcoder.jp/contests/abc200/tasks/abc200_f
*/


//【線形回帰】O(cnt dim min(cnt, dim))
/*
* get_sample() で得た cnt 個のサンプル点を元に線形回帰を行い係数列を返す（失敗すれば空リスト）
* code には，変数不足なら -(不足量)，さもなくば斉次方程式の解空間の次元を格納する．
*
* 利用：【行列】,【線形方程式】
*/
template <class FUNC>
vm linear_regression(const FUNC& get_sample, int cnt, int* code = nullptr) {
	// verify : https://mofecoder.com/contests/oupc2024day2/tasks/oupc2024day2_a

	Matrix<mint> A(cnt, 0); vm b(cnt);

	// 説明変数と目的変数の対を cnt 個用意する．
	rep(i, cnt) {
		auto [vec, ans] = get_sample();
		A[i] = vec;
		b[i] = ans;
	}
	A.m = sz(A.v[0]);

	// 説明変数の数以上にデータがないといつでも非自明解をもってしまって意味がない（とも限らない）
	if (A.n < A.m) {
		if (code) *code = A.n - A.m;
		return vm();
	}

	// 行列方程式 A x = b を解いて特殊解 x を求める．
	vvm xs;
	auto x = gauss_jordan_elimination(A, b, &xs);
	if (code) *code = sz(xs);
	//dumpel(xs);

	// 解が係数列なのでそれを返す（解がなければ説明変数が足りておらず失敗）
	return x;

	/* 雛形
	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<ll> rnd(0, (ll)1e18);

	int sample_num = -1;

	auto get_sample = [&]() {
		++sample_num;

		// テストケースのランダム生成
		int a = (int)(rnd(mt) % 11 - 5);
		int b = (int)(rnd(mt) % 11 - 5);
		string s;
		rep(k, 8) s += "01"[rnd(mt) % 2];

		// 手動で入れたいケースがある場合はここで作成する．
		if (sample_num == -1) {
			;
		}
		// 係数を決め打って自由度を殺したい場合はここに追加する．
		else if (sample_num == -2) {
			// coef[0] = 1 に決め打ち
			vm vec(n + 1); vec[0] = 1;
			mint ans = 1;
			return make_pair(vec, ans);
		}

		// テストケースから説明変数ベクトルを作成
		vm vec{ 1 };
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(min(a, b));

		// 目的変数の計算
		mint ans = naive(a, b, s);

		// 説明変数と目的変数の対を返す
		return make_pair(vec, ans);
	};

	int code;
	auto coef = linear_regression(get_sample, 500, &code);
	dump(coef); dump("code:", code);
	*/
}


//【線形回帰（ラベル毎）】O(cnt dim min(cnt, dim))
/*
* get_sample() で得た cnt 個のサンプル点（L 種のラベル付き）を元に線形回帰を行い，
* 各ラベル lab 毎に係数列をまとめた二次元リストを返す（失敗すれば空リスト）
* code[lab] には，変数不足なら -(不足量)，さもなくば斉次方程式の解空間の次元を格納する．
*
* 利用：【行列】,【線形方程式】
*/
template <class FUNC>
vvm linear_regression(const FUNC& get_sample, int cnt, int L, vi* code = nullptr) {
	// verify : https://atcoder.jp/contests/abc271/tasks/abc271_h

	vector<Matrix<mint>> As(L);
	vvm bs(L);

	// 説明変数と目的変数の対を cnt 個用意する．
	rep(i, cnt) {
		auto [vec, lab, ans] = get_sample();
		As[lab].push_back(vec);
		bs[lab].push_back(ans);
	}

	vvm res(L);
	if (code) code->resize(L);

	rep(lab, L) {
		// サンプルが 1 つもないラベルは無視する．
		if (bs[lab].empty()) {
			if (code) (*code)[lab] = 999;
			continue;
		}

		// 説明変数の数以上にデータがないといつでも非自明解をもってしまって意味がない（とも限らない）
		As[lab].m = sz(As[lab].v[0]);
		if (As[lab].n < As[lab].m) {
			if (code) (*code)[lab] = As[lab].n - As[lab].m;
			continue;
		}

		// 行列方程式 A x = b を解いて特殊解 x を求める．
		vvm xs;
		auto x = gauss_jordan_elimination(As[lab], bs[lab], &xs);
		if (code) (*code)[lab] = sz(xs);

		// 解が係数列なのでそれを返す（解がなければ説明変数が足りておらず失敗）
		res[lab] = x;
	}

	return res;

	/* 雛形
	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<ll> rnd(0, (ll)1e18);

	int sample_num = -1;

	auto get_sample = [&]() {
		++sample_num;

		// テストケースのランダム生成
		int a, b; string s;
		while (1) {
			a = (int)(rnd(mt) % 21 - 10);
			b = (int)(rnd(mt) % 21 - 10);
			if (abs(a) <= 3 && abs(b) <= 3) continue;

			rep(k, 8) s += "01"[rnd(mt) % 2];
			break;
		}

		// 手動で入れたいケースがある場合はここで作成する．
		if (sample_num == -1) {
			;
		}
		// 係数を決め打って自由度を殺したい場合はここに追加する．
		else if (sample_num == -2) {
			// coef[0] = 1 に決め打ち
			vm vec(n + 1); vec[0] = 1;
			mint ans = 1;
			return make_pair(vec, ans);
		}

		// テストケースから説明変数ベクトルを作成
		vm vec{ 1 };
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(min(a, b));

		// テストケースからラベルを作成
		int lab = 0;
		if (a + b > 0 && b == 0) lab = 0;
		else if (a - b > 0 && b > 0) lab = 1;
		else if (a - b == 0 && b > 0) lab = 2;
		else if (a - b < 0 && a > 0) lab = 3;
		else if (a - b < 0 && a == 0) lab = 4;
		else if (a + b > 0 && a < 0) lab = 5;
		else if (a + b == 0 && a < 0) lab = 6;
		else if (a + b < 0 && b > 0) lab = 7;
		else if (a + b < 0 && b == 0) lab = 8;
		else if (a - b < 0 && b < 0) lab = 9;
		else if (a - b == 0 && b < 0) lab = 10;
		else if (a - b > 0 && a < 0) lab = 11;
		else if (a - b > 0 && a == 0) lab = 12;
		else if (a + b < 0 && a > 0) lab = 13;
		else if (a + b == 0 && a > 0) lab = 14;
		else if (a + b > 0 && b < 0) lab = 15;

		lab *= 2;
		lab += a & 1;

		lab *= 2;
		lab += b & 1;

		rep(k, 8) {
			lab *= 2;
			lab += s[k] - '0';
		}

		// 目的変数の計算
		mint ans = naive(a, b, s);

		// 説明変数と目的変数の対を返す
		return make_tuple(vec, lab, ans);
	};

	vi code;
	auto coef = linear_regression(get_sample, 1000000, 16*2*2*(1<<8), &code);
	int L = sz(coef);

	unordered_map<int, int> code_dist;
	repe(c, code) code_dist[c]++;
	dump("code_dist:"); dumpel(code_dist);
	*/
}


//【決定木】
/*
* Decision_tree<CLS>() : O(1)
*	クラス [0..CLS) を分類するための空の決定木を準備する．
*
* int size() : O(1)
*	データ数を返す．
*
* add_data(vi X, int y) : O(1)
*	(特徴ベクトル, クラス) = (X, y) を追加する．
*
* build() : O(n log n) (?)
*	決定木を構築する．
*
* int predict(vi X) : O(log n) (?)
*	特徴ベクトル X の属するクラスを返す．
*
* to_string() : O(n)
*	決定木埋め込み用の文字列を出力する．
*/
template <int CLS>
class Decision_tree {
	// ChatGPT 作

	struct Node {
		int feature = -1;
		int threshold = 0;
		int label = -1;
		Node* left = nullptr, * right = nullptr;
	};

	vvi Xs; vi ys; int DIM;
	Node* rt;

	Node* build_tree(vi& idx) {
		int n = sz(idx);

		// 全部同じクラスなら葉
		bool same = true;
		repi(i, 1, n - 1) if (ys[idx[i]] != ys[idx[0]]) { same = false; break; }
		if (same) {
			Node* leaf = new Node();
			leaf->label = ys[idx[0]];
			return leaf;
		}

		int best_feat = -1;
		int best_thr = 0;
		double best_score = 1e18;

		// 特徴量ごとに候補探索
		rep(feat, DIM) {
			vector<pii> vals;
			vals.reserve(n);
			repe(id, idx) vals.push_back({ Xs[id][feat], ys[id] });
			sort(vals.begin(), vals.end());

			// prefix 集計
			array<int, CLS> left_cnt, right_cnt;
			left_cnt.fill(0); right_cnt.fill(0);
			repe(v, vals) right_cnt[v.second]++;
			int left_size = 0, right_size = n;

			rep(i, n - 1) {
				int cls = vals[i].second;
				left_cnt[cls]++; right_cnt[cls]--;
				left_size++; right_size--;

				if (vals[i].first == vals[i + 1].first) continue;

				auto gini = [](const array<int, CLS>& cnt, int sz) {
					if (sz == 0) return 0.0;
					double g = 1.0;
					rep(c, CLS) {
						double p = (double)cnt[c] / sz;
						g -= p * p;
					}
					return g;
				};

				double score = gini(left_cnt, left_size) * left_size + gini(right_cnt, right_size) * right_size;
				if (score < best_score) {
					best_score = score;
					best_feat = feat;
					best_thr = vals[i + 1].first;
				}
			}
		}

		// 同じ特徴量なのにクラスが別のものがあれば不可能
		Assert(best_feat != -1);

		vi L, R;
		repe(id, idx) {
			if (Xs[id][best_feat] < best_thr) L.push_back(id);
			else R.push_back(id);
		}

		// 毎回ほぼ半分ずつに分かれてくれるなら高速
		Node* node = new Node();
		node->feature = best_feat;
		node->threshold = best_thr;
		node->left = build_tree(L);
		node->right = build_tree(R);
		return node;
	}

	void to_string(Node* node) {
		if (!node) return;

		if (node->label != -1) {
			cout << "return " << node->label << ";";
			return;
		}

		cout << "if(v[" << node->feature << "]<" << node->threshold << ")";
		to_string(node->left);
		cout << "else ";
		to_string(node->right);
	}

public:
	Decision_tree() : DIM(-1), rt(nullptr) {
		// verify : https://atcoder.jp/contests/arc192/tasks/arc192_b
	}

	// データ数を返す．
	int size() {
		return sz(Xs);
	}

	// (特徴ベクトル, クラス) = (X, y) を追加する．
	void add_data(const vi& X, int y) {
		// verify : https://atcoder.jp/contests/arc192/tasks/arc192_b

		Xs.push_back(X);
		ys.push_back(y);
	}

	// 決定木を構築する．
	void build() {
		// verify : https://atcoder.jp/contests/arc192/tasks/arc192_b

		vi idx(sz(Xs));
		iota(all(idx), 0);

		DIM = sz(Xs[0]);

		rt = build_tree(idx);
	}

	// 特徴量ベクトル X の属するクラスを返す．
	int predict(const vi& X) {
		// verify : https://atcoder.jp/contests/arc192/tasks/arc192_b

		Node* node = rt;

		while (1) {
			if (node->label != -1) return node->label;

			if (X[node->feature] < node->threshold) node = node->left;
			else node = node->right;
		}
	}

	// 決定木埋め込み用の文字列を出力する．
	void to_string() {
		// verify : https://atcoder.jp/contests/arc192/tasks/arc192_b

		cout << "int predict(const vi& v){\n";
		to_string(rt);
		cout << "\n}\n";
	}
};


//【ペンパソルバー】
/*
* ペンシルパズル用のソルバーを利用して構築問題の規則的な解を発見できる場合がある．
*/


