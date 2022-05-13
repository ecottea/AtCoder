#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; // -2^63 ～ 2^63 = 9 * 10^18（int は -2^31 ～ 2^31 = 2 * 10^9）
using pii = pair<int, int>;	using pll = pair<ll, ll>;	using pil = pair<int, ll>;	using pli = pair<ll, int>;
using vi = vector<int>;		using vvi = vector<vi>;		using vvvi = vector<vvi>;
using vl = vector<ll>;		using vvl = vector<vl>;		using vvvl = vector<vvl>;
using vb = vector<bool>;	using vvb = vector<vb>;		using vvvb = vector<vvb>;
using vc = vector<char>;	using vvc = vector<vc>;		using vvvc = vector<vvc>;
using vd = vector<double>;	using vvd = vector<vd>;		using vvvd = vector<vvd>;
template <class T> using priority_queue_rev = priority_queue<T, vector<T>, greater<T>>;
using Graph = vvi;

// 定数の定義
const double PI = acos(-1);
const vi dx4 = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi dy4 = { 0, 1, 0, -1 };
const vi dx8 = { 1, 1, 0, -1, -1, -1, 0, 1 }; // 8 近傍
const vi dy8 = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int INF = 1001001001; const ll INFL = 4004004004004004004LL;
const double EPS = 1e-12; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define distance (int)distance
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 演算子オーバーロード
template <class T, class U> inline istream& operator>>(istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T> inline istream& operator>>(istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline vector<T>& operator--(vector<T>& v) { repea(x, v) --x; return v; }
template <class T> inline vector<T>& operator++(vector<T>& v) { repea(x, v) ++x; return v; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#include "local.hpp"
// 提出用（gcc）
#else
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(...)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


//--------------AtCoder 専用--------------
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------


//【遅延評価 binary trie】
/*
* Lazy_binary_trie<T>() : O(1)
*   型 T の整数を扱えるよう空で初期化する．
* 
* ll size() : O(1)
*   要素数を返す．
* 
* bool empty() : O(1)
*   要素が 0 個かを返す．
* 
* ll count(T val) : O(B)
*   要素 val の個数を返す．
* 
* insert(T val, ll cnt = 1) : O(B)
*   値 val を cnt 個追加する．
* 
* erase(T val, ll cnt = 1) : O(B)
*   値 val を cnt 個削除する．
* 
* xor_all(T mask): O(1) 
*   全要素に対して mask との XOR をとる．
* 
* T max_element(T mask = 0) : O(B)
*   mask との XOR をとったときの最大要素を返す．
* 
* T min_element(T mask = 0) : O(B)
*   mask との XOR をとったときの最小要素を返す．
* 
* T get(ll i) : O(B)
*   昇順で i 番目（0-indexed）の要素を返す．
* 
* ll lower_bound(T val) : O(B)
*   val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed）
* 
* ll upper_bound(T val) : O(B)
*   val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed）
*/
template<class T> class Lazy_binary_trie {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/06/022654

    struct Node {
        ll cnt; // 部分木のもつ要素の個数
        T lazy; // XOR 待ちの値
        Node* ch[2]; // 左右の子へのポインタ

        Node() : cnt(0), lazy(0), ch{ nullptr, nullptr } {}
    };

    Node* root; // 根へのポインタ
    int B; // 何ビット整数を扱うか
    T mask_all; // 全要素にかけられた XOR マスク
    
    Node* insert_sub(Node* t, T val, ll cnt, int b) {
        // まだノードがなければ作成する．
        if (t == nullptr) t = new Node;

        // 個数を増やす．
        t->cnt += cnt;

        // 自身が葉ならすぐに帰る．
        if (b < 0) return t;

        // 下位ビットに対応するノードに加算しにいく．
        T f = (val >> b) & T(1);
        t->ch[f] = insert_sub(t->ch[f], val, cnt, b - 1);

        // 自身へのポインタを親に返す．
        return t;
    }

    Node* erase_sub(Node* t, T val, ll cnt, int b) {
        // 存在しない要素を削除しようとすればエラーを返す．
        assert(t != nullptr && t->cnt >= cnt);

        // 個数を減らす．
        t->cnt -= cnt;

        // 要素が 0 個になったならノードを削除する．
        if (t->cnt == 0) {
            delete t;
            return nullptr;
        }

        // 自身が葉ならすぐに帰る．
        if (b < 0) return t;

        // 下位ビットに対応するノードに減算しにいく．
        T f = (val >> b) & T(1);
        t->ch[f] = erase_sub(t->ch[f], val, cnt, b - 1);

        // 自身へのポインタを親に返す．
        return t;
    }

    T min_element_sub(Node* t, T mask, int b) {
        assert(t != nullptr);

        // 葉なら 0 を返す．
        if (b < 0) return 0;

        // 下位ビットに対応するノードの最小値を求めにいく．
        T f = (mask >> b) & T(1);
        if (t->ch[f] == nullptr) f ^= T(1);
        T val = min_element_sub(t->ch[f], mask, b - 1);

        // 自身のビットを設定する．
        val |= f << b;

        return val;
    }

    T get_sub(Node* t, ll k, int b) {
        // 葉なら 0 を返す．
        if (b < 0) return 0;

        // 左の部分木に含まれる要素の個数をみて適切な子に探索しにいく．
        ll lk = (t->ch[0] != nullptr ? t->ch[0]->cnt : 0);
        T val;
        if (k < lk) val = get_sub(t->ch[0], k, b - 1);
        else val = get_sub(t->ch[1], k - lk, b - 1) | (1LL << b);

        return val;
    }

    ll lower_bound_sub(Node* t, T val, int b) {
        // 葉であるかまたはノードがなければ 0 を返す．
        if (t == nullptr || b < 0) return 0;

        // val の第 b ビットをみて適切な子に探索しにいく．
        T f = (val >> b) & T(1);
        ll res = 0;
        if (f == 1 && t->ch[0] != nullptr) res += t->ch[0]->cnt;
        res += lower_bound_sub(t->ch[f], val, b - 1);

        return res;
    }

public:
    // 空で初期化する． : O(1)
    Lazy_binary_trie() : root(nullptr), B((int)sizeof(T) * 8), mask_all(0) {}

    // 要素数を返す． : O(1)
    ll size() const { 
        return root != nullptr ? root->cnt : 0;
    }

    // 要素が 0 個かを返す． : O(1)
    bool empty() const {
        return root == nullptr;
    }

    // 値 val を cnt[=1] 個追加する． : O(B)
    void insert(T val, ll cnt = 1) {
        // verify : https://atcoder.jp/contests/arc033/tasks/arc033_3

        root = insert_sub(root, val ^ mask_all, cnt, B - 1);
    }

    // 値 val を cnt[=1] 個削除する． : O(B)
    void erase(T val, ll cnt = 1) {
        // verify : https://atcoder.jp/contests/arc033/tasks/arc033_3

        root = erase_sub(root, val ^ mask_all, cnt, B - 1);
    }

    // 全要素について mask との XOR をとる． : O(1) 
    void xor_all(T mask) {
        mask_all ^= mask;
    }

    // mask[=0] との XOR をとったときの最大要素を返す． : O(B)
    T max_element(T mask = 0) {
        return min_element_sub(root, ~(mask ^ mask_all), B - 1);
    }

    // mask[=0] との XOR をとったときの最小要素を返す． : O(B)
    T min_element(T mask = 0) {
        return min_element_sub(root, mask ^ mask_all, B - 1);
    }

    // 昇順で i 番目（0-indexed）の要素を返す． : O(B)
    T get(ll i) {
        // verify : https://atcoder.jp/contests/arc033/tasks/arc033_3

        assert(0 <= i && i < size());
        return get_sub(root, i, B - 1);
    }
 
    // val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
    ll lower_bound(T val) {
        return lower_bound_sub(root, val, B - 1);
    }

    // val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
    ll upper_bound(T val) {
        return lower_bound_sub(root, val + 1, B - 1);
    }

    // 要素 val の個数を返す． : O(B)
    ll count(T val) {
        if (!root) return 0;
        Node* t = root;
        for (int i = B - 1; i >= 0; i--) {
            eval(t, i);
            t = t->ch[(val >> i) & 1LL];
            if (!t) return 0;
        }
        return t->cnt;
    }
}; 


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

    Lazy_binary_trie<int> lbt;

    int q;
    cin >> q;

    rep(hoge, q) {
        int t, x;
        cin >> t >> x;

        if (t == 1) {
            lbt.insert(x);
        }
        else {
            int v = lbt.get(x - 1);
            cout << v << endl;

            lbt.erase(v);
        }
    }
}
