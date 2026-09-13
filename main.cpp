#include <bits/stdc++.h>
#include <chrono>

using namespace std;

typedef long long int int64;
const int inf = 1073741823;
const int64 infl = 1LL << 60;

#define rep(i, x, limit) for (int i = (int)x; i < (int)limit; i++)
#define REP(i, x, limit) for (int i = (int)x; i <= (int)limit; i++)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

//データ定義
typedef struct
{
    int x;
    int y;
    int num = -1;
}Point;

//距離系
int euclidean_distance(Point aa, Point bb){ // 整数値に丸める
    return (int)sqrt((double)(pow(aa.x - bb.x, 2) + pow(aa.y - bb.y, 2)));
}
int manhattan_distance(Point aa, Point bb){ // 整数値に丸める
    return (abs(aa.x - bb.x) + abs(aa.y - bb.y));
}
//便宜上、xが上下方向、yが左右方向
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

//タイマー
struct Timer {
    chrono::steady_clock::time_point start, end;

    Timer() : start(chrono::steady_clock::now()), end(chrono::steady_clock::now()) {}
    void start_timer() {
        start = chrono::steady_clock::now();
    }
    double elapsed() {
        end = chrono::steady_clock::now();
        return static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    }
};

// 入力・出力のデータ構造
// 問題により設計する

//Input
struct Input {
    int n;
    vector<Point> points;
};

//Action 
struct Action {
    int type;
};

//Output
struct Output {
    int n;
    vector<Action> acts;
};

// State
struct State {
    int turn = 0;
    int64 score = 0;

    bool is_finished(const Input& input) {
        return turn >= input.n;
    }

    void apply(const Action& action) {
        turn++;
    }
};

//Evaluation
int64 calc_score(const Input& input, const State& state) {
    // 実スコア計算（State全体を見てスコア計算したい場合に使用）
    return state.score;
}

double evaluate(const Input& input, const State& state) {
    // 評価用スコア計算（解候補の選択などに使用）
    double score = static_cast<double>(state.score);

    // 必要であればヒューリスティックスコアを追加

    return score;
}

// Generate Action
vector<Action> generate_actions(const Input& input, const State& state) {
    vector<Action> actions;
    // 必要に応じてアクションを生成
    return actions;
}


//出力
void output_ans(int n, const Output& out){
    for (int i = 0; i < n; i++){
        cout << out.acts[i].type << endl;
    }
}

// 乱数生成器
std::mt19937 mt{ std::random_device{}() };

int random_number(int n)
{
    std::uniform_int_distribution<int> dist(0, n-1);
    return dist(mt);
}

// Todo: Beam Search Utility

// Todo: Simulated Annealing Utility

int main() {

    using namespace std;

    // 時間計測の開始
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    const int TIME_LIMIT = 1900; // ms

    // 入力
    Input input;

    // 初期解を作成

    // 結果作成

    // 解を作成

    //スコアを計算


    //焼きなまし用パラメータ
    double start_temp = 50;
    double end_temp = 10;
    //ここからループ予定
    int step = 0;
    while(true){
        
        end = chrono::system_clock::now();
        double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
        cerr << "time: " << time << "ms" << endl;
        if(time>TIME_LIMIT){
            break;
        }
        double temp = start_temp + (end_temp - start_temp) * (time) / TIME_LIMIT;
        step++;

        // 解をランダムに変える

        // 今までの状態をコピー
        
        // 候補ごとにスコアを計算する

        // 前回のスコアを超えていなければ採用、そうでなければ差し戻す

        // スコア・パラメータ更新


        // ループ終わり
        break;
    }
    cerr << "step num: " << step << endl;


    //最後の出力
    //cout << endl;
    //output_ans(n, out);
}
