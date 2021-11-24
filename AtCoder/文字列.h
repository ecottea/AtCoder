#pragma once
#include "header.h"
#include "二分木.h"
#include "二項係数.h"
// ■■■■■ 文字列 ■■■■■


//【ローリングハッシュ】
/*
* 文字列 s の部分文字列 s[l, r) のハッシュ値を計算する．
* ハッシュ値は Σi=0..r-l (s[l + i] + SHIFT) * BASE ^ i (mod MOD)
*
* rolling_hash(s) : O(|s|)
*	文字列を s として初期化する．
*
* get(l, r) : O(1)
*	部分文字列 s[l, r) のハッシュ値を返す．
*/
template <class STR, int MOD, int BASE, int SHIFT>
struct rolling_hash_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;

	const mint B = BASE; // 適当な基数
	const mint invB = B.inv(); // 基数の逆数
	const mint S = SHIFT; // 適当なシフト

	// 文字列とその長さ
	STR s;
	int n;

	// v[i] : s[0, i) のハッシュ値
	vm v;

	// pow_invB[i] : B^(-i)
	vm pow_invB;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_sub(const STR& s_) : s(s_), n(sz(s_)), v(n + 1), pow_invB(n) {
		// ハッシュ値計算用の B の累乗
		pow_invB[0] = 1;
		rep(i, n - 1) {
			pow_invB[i + 1] = pow_invB[i] * invB;
		}

		// s[0, i) のハッシュ値の計算
		mint powB = 1;
		rep(i, n) {
			v[i + 1] = v[i] + (s[i] + S) * powB;
			powB *= B;
		}
	}

	// s[l, r) のハッシュ値の取得
	int get(int l, int r) {
		return ((v[r] - v[l]) * pow_invB[l]).val();
	}
};
template <class STR> // STR は例えば string, vector<int>
struct rolling_hash {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_sub<STR, 1000000007, 100007, 17> rh1;
	rolling_hash_sub<STR, 998244353, 99991, 91> rh2;

	// コンストラクタ（文字列 s で初期化）
	rolling_hash(const STR& s) : rh1(s), rh2(s) {}

	// s[l, r) のハッシュ値の取得
	ll get(int l, int r) {
		return (ll(rh1.get(l, r)) << 32) + ll(rh2.get(l, r));
	}
};


//【二次元ローリングハッシュ】
/*
* 二次元配列 a の部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を計算する．
* ハッシュ値は次の式により計算する：
*   Σi=0..x2-x1 j=0..y2-y1 (a[x1 + i][y1 + j] + SHIFT) * BASE_X ^ i * BASE_Y ^ j (mod MOD)
*
* rolling_hash(a) : O(|w| |h|)
*	二次元配列を a として初期化する．
*
* get(x1, y1, x2, y2) : O(1)
*	部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
*/
template <class T, int MOD, int BASE_X, int BASE_Y, int SHIFT>
struct rolling_hash_2d_sub {
	using mint = static_modint<MOD>;
	using vm = vector<mint>;
	using vvm = vector<vm>;

	const mint BX = BASE_X; // 適当な基数
	const mint invBX = BX.inv(); // 基数の逆数
	const mint BY = BASE_Y;
	const mint invBY = BY.inv();
	const mint S = SHIFT; // 適当なシフト

	// 二次元配列とその大きさ
	vector<vector<T>> a;
	int h, w;

	// v[i][j] : 長方形領域 [0, i) * [0, j) のハッシュ値
	vvm v;

	// ハッシュ値計算用の B の累乗
	vm pow_BX, pow_BY, pow_invBX, pow_invBY;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_2d_sub(vector<vector<T>>& a_) :
		a(a_), h(sz(a)), w(sz(a[0])), v(h + 1, vector<mint>(w + 1)),
		pow_BX(h), pow_BY(w), pow_invBX(h), pow_invBY(w) {

		// ハッシュ値計算用の B の累乗の前計算
		pow_BX[0] = pow_BY[0] = pow_invBX[0] = pow_invBY[0] = 1;
		rep(i, h - 1) {
			pow_BX[i + 1] = pow_BX[i] * BX;
			pow_invBX[i + 1] = pow_invBX[i] * invBX;
		}
		rep(j, w - 1) {
			pow_BY[j + 1] = pow_BY[j] * BY;
			pow_invBY[j + 1] = pow_invBY[j] * invBY;
		}

		// 長方形領域 [0, i) * [0, j) のハッシュ値の計算
		rep(i, h) {
			rep(j, w) {
				v[i + 1][j + 1] = v[i + 1][j] + v[i][j + 1] - v[i][j]
					+ (a[i][j] + S) * pow_BX[i] * pow_BY[j];
			}
		}
	}

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	int get(int x1, int y1, int x2, int y2) {
		return ((v[x2][y2] - v[x1][y2] - v[x2][y1] + v[x1][y1])
			* pow_invBX[x1] * pow_invBY[y1]).val();
	}
};
template <class T> // T は例えば int, ll, char
struct rolling_hash_2d {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_2d_sub<T, 1000000007, 100007, 26627, 17> rh1;
	rolling_hash_2d_sub<T, 998244353, 99991, 54401, 91> rh2;

	// コンストラクタ（二次元配列 a で初期化）
	rolling_hash_2d(vector<vector<T>>& a) : rh1(a), rh2(a) {}

	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	ll get(int x1, int y1, int x2, int y2) {
		return (ll(rh1.get(x1, y1, x2, y2)) << 32) + ll(rh2.get(x1, y1, x2, y2));
	}
};


//【クヌース・モリス・プラット法】O(|s| + |w|)
/*
* s の部分文字列として w が含まれているかどうか調べ，
* 最初に見つかった場所の先頭位置を返す．（見つからなかったら -1 を返す．）
*/
template <class STR> // STR は例えば string, vector<int>
int knuth_morris_pratt(const STR& s, const STR& w) {
	int n = sz(s);
	int m = sz(w);

	// 部分マッチテーブル tbl の作成
	// s[i] から照合を開始して s[i + j] != w[j] となった場合，
	// 次に照合を開始すべき位置が s[i + j - tbl[j]] であるように構築する．
	vi tbl(m);
	tbl[0] = -1;
	tbl[1] = 0;
	int i = 2; // いま tbl[i] を計算中であることを表す．
	int j = 0; // いま w[j] まで見ていることを表す．
	while (i < m) {
		// サブ文字列が w の先頭と一致し続けている場合
		if (w[i - 1] == w[j]) {
			// 一致した長さの分だけバックトラッキングしなければならない．
			tbl[i] = j + 1;

			// それぞれ 1 文字先を見に行く．
			i++;
			j++;
		}
		// サブ文字列と w の先頭との一致が終わった場合？
		else if (j > 0) {
			// 次のサブ文字列を走査するため j を戻す．？
			j = tbl[j];
		}
		// ？
		else {
			tbl[i] = 0;
			i++;
		}
	}

	// 連続部分列を探す．
	i = 0; // いま s[i] から始まる連続部分列を見ていることを表す．
	j = 0; // いま w[j] まで見ていることを表す．
	while (i + j < n) {
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致した場合．
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見．
			if (j == m) {
				return i;
			}
		}
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致しなかった場合．
		else {
			// 部分マッチテーブルに従い i, j を再設定する．
			// i = i + j としたいが tbl[j] だけのバックトラッキングが入る．
			i = i + j - tbl[j];

			// その代わり w との照合を tbl[j] だけ進んだところから始められる．
			if (j > 0) {
				j = tbl[j];
			}
		}
	}

	return -1;
}


//【レーベンシュタイン距離】O(|s| |t|)
/*
* 文字列 s から文字列 t へのレーベンシュタイン距離を返す．
* また必要なら削除や挿入の位置を '-' で表した文字列を s2, t2 に格納する．
*
* dist = {fit, ins, del, sub} :
*	順に，一致，挿入，削除，置換 1 回あたりの距離
*
*（二次元 DP）
*/
template <class T>
ll levenshtein_distance(const vector<T>& s, const vector<T>& t,
	const tuple<ll, ll, ll, ll>& dist = { 0, 1, 1, 1 },
	vector<T>* s2 = nullptr, vector<T>* t2 = nullptr) {

	int n = sz(s);
	int m = sz(t);

	ll fit, ins, del, sub;
	tie(fit, ins, del, sub) = dist;

	// dp[i][j] : s[0..i) から t[0..j) への距離
	vvl dp(n + 1, vl(m + 1, INF));
	dp[0][0] = 0;

	// 貰う DP
	repi(i, 0, n) {
		repi(j, 0, m) {
			// 以下の 3 通りのうちの最大のものを選ぶ：
			//   s[i - 1] と t[j - 1] を対にする．
			//   s[i - 1] を削除する．
			//   t[j - 1] を挿入する．
			if (i > 0 && j > 0) {
				chmin(dp[i][j], dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub));
			}
			if (i > 0) {
				chmin(dp[i][j], dp[i - 1][j] + del);
			}
			if (j > 0) {
				chmin(dp[i][j], dp[i][j - 1] + ins);
			}
		}
	}

	// DP 復元を行う
	if (s2 != nullptr) {
		s2->clear();
		t2->clear();

		int i = n, j = m;
		while (i > 0 || j > 0) {
			if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub)) {
				s2->push_back(s[--i]);
				t2->push_back(t[--j]);
			}
			else if (i > 0 && dp[i][j] == dp[i - 1][j] + del) {
				s2->push_back(s[--i]);
				t2->push_back('-');
			}
			else {
				s2->push_back('-');
				t2->push_back(t[--j]);
			}
		}
		reverse(all(*s2));
		reverse(all(*t2));
	}

	return dp[n][m];
}


//【同じ文字の連続しない文字列の数え上げ】O(n Σcnt[i])
/*
* n 種の各文字 i を cnt[i] 個ずつ含む文字列で，同じ文字が隣り合わないものの個数を返す．
*/
mint count_noncontinuous_sequence(const vi& cnt_) {
	// 0 個の文字は無視する．
	vi cnt;
	repe(c, cnt_) {
		if (c > 0) cnt.push_back(c);
	}
	int n = sz(cnt);
	factorial_mint fm(accumulate(all(cnt), 0));

	// dp[i][j] : 文字 [0..i) で同じ文字の隣接が j 箇所ある文字列の個数
	vvm dp(n + 1);
	dp[0] = vm({ 1, 0 });

	int len = 2; // 文字列の長さ（両端の番兵含む）

	// i : 次に挿入する文字の種類
	rep(i, n) {
		dp[i + 1] = vm(len + cnt[i]);

		// j : 同じ文字の隣接箇所の個数
		rep(j, len - 1) {
			// k : 文字 i をいくつの固まりに分けるか
			repi(k, 1, cnt[i]) {
				// l : 同じ文字の間にいくつ固まりを挿入するか
				repi(l, 0, min(k, j)) {
					int nj = j + (cnt[i] - k) - l;

					mint add = dp[i][j];

					// cnt[i] 個の文字を順序込みで k 個に分ける方法の数
					//	まず文字を k 個減らしておき，重複組合せの考え方を用いて
					//	○ cnt[i] - k 個と ｜ k - 1 個の並べ方を数えれば良い．
					add *= fm.binomial(cnt[i] - 1, k - 1);

					// k 個の固まりをどこに挿入するか
					//	順序は先に定めたので，後は挿入位置だけを考えれば良い．
					//	同じ文字の間が j 箇所中 l 箇所，
					//	異なる文字の間が残り len - 1 - j 箇所中 k - l 箇所．
					add *= fm.binomial(j, l) * fm.binomial(len - 1 - j, k - l);

					dp[i + 1][nj] += add;
				}
			}
		}
		len += cnt[i];
	}

	return dp[n][0];
}


//【ビット列の連結】O(4^S S k log(n))（ただし S = max(|s[i]|)）
/*
* n 個の 0-1 文字列 s[i] を好きに並べて得られる長さ k の文字列の個数を返す．
*/
mint count_string_concat(const vector<string>& s_, int k) {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/27/181249

	int n = sz(s_);

	// m : 文字の長さの最大値
	int m = 0;
	rep(i, n) chmax(m, sz(s_[i]));

	// 扱いやすいようにビット列に変換し，長さごとに記録しておく．
	vector<set<int>> s(m + 1);
	rep(i, n) {
		int seq = 0, len = sz(s_[i]);
		rep(j, len) {
			seq = seq * 2 + (s_[i][j] - '0');
		}
		s[len].insert(seq);
	}

	// dp[i][seq][set] : 長さ i で直前の m 文字が seq であるもので，
	//	文字列 [0..i-j) が s を並べて得られるような j の集合が set であるものの個数
	vvvm dp(k + 1, vvm(1 << m, vm(1 << m)));
	dp[0][0][1] = 1;
	int mask = (1 << m) - 1;

	// i : 文字列の長さ
	rep(i, k) {
		// seq : 文字列（直前 m 文字のみ）
		repb(seq, m) {
			// b : seq に追加する文字
			repi(b, 0, 1) {
				// nseq : seq の末尾に b を追加した文字列
				int nseq = ((seq << 1) & mask) + b;

				// set : 文字列 [0..i-j) が s を並べて得られるような j の集合
				repb(set, m) {
					// cut_flag : 文字列 [0..i+1) が s を並べて得られるか
					int cut_flag = 0;

					// j : 文字列 [0..i-j) が s を並べて得られる
					rep(j, m) {
						if (!(set & (1 << j))) continue;

						// 文字列 nseq[i-j..i+1) が s に含まれるか
						if (s[j + 1].count(nseq & ((1 << (j + 1)) - 1))) {
							cut_flag = 1;
							break;
						}
					}

					// nset : 文字列 [0..i+1-j) が s を並べて得られるような j の集合
					int nset = ((set << 1) & mask) + cut_flag;

					dp[i + 1][nseq][nset] += dp[i][seq][set];
				}
			}
		}
	}

	mint res = 0;
	repb(seq, m) {
		repb(set, m) {
			if (set & 1) {
				res += dp[k][seq][set];
			}
		}
	}

	return res;
}


