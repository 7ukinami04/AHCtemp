// AHC Template Main.cpp from https://github.com/7ukinami04/AHCtemp.git

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

// Parameters
struct Parameters {
    int beam_width = 10;
    int beam_depth = 10;

    double time_limit = 1900.0;
    double start_temp = 1000.0;
    double end_temp = 1.0;
};

// State
struct State {
    int turn = 0;
    int64 score = 0;
    double eval_score = 0.0;

    bool is_finished(const Input& input) const {
        return turn >= input.n;
    }

    void apply(const Action& action) {
        turn++;
    }
};

//Evaluation
int64 calc_score(const Input& input, const State& state) {
    // 実スコア計算（State全体を見てスコア計算したい場合に使用）
    return state.turn;
}

double evaluate(const Input& input, const State& state, const Parameters& params) {
    // 評価用スコア計算（解候補の選択などに使用）
    double score = static_cast<double>(calc_score(input, state));

    // 必要であればヒューリスティックスコアを追加

    return score;
}

// Generate Action
vector<Action> generate_actions(const Input& input, const State& state, const Parameters& params) {
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
struct RNG {
    std::mt19937 mt;

    RNG() : mt(random_device{}()) {}
    RNG(unsigned int seed) : mt(seed) {}

    // [0, n-1] の整数 (n > 0)
    int next_int(int n) {
        return uniform_int_distribution<int>(0, n - 1)(mt);
    }

    // [l, r] の整数 (l <= r)
    int next_int(int l, int r) {
        return uniform_int_distribution<int>(l, r)(mt);
    }

    // [0, 1) の実数
    double next_double() {
        return uniform_real_distribution<double>(0.0, 1.0)(mt);
    }

    // [l, r) の実数
    double next_double(double l, double r) {
        return uniform_real_distribution<double>(l, r)(mt);
    }

    // 確率 p で true
    bool next_bool(double p) {
        return next_double() < p;
    }
};

// Todo: Beam Search Utility
// Beam Search Utility
struct BeamNode {
    State state;
    int history_index = -1;
};

struct BeamHistory {
    int parent_index;
    Action action;
};

struct BeamCandidate {
    int parent_index;
    Action action;
    double score;
};

vector<Action> beam_search(
    const Input& input,
    const State& initial_state,
    const Parameters& params,
    chrono::steady_clock::time_point deadline
) {
    vector<BeamNode> beam;
    vector<BeamHistory> history;

    BeamNode initial_node;
    initial_node.state = initial_state;
    beam.push_back(initial_node);

    double best_score = initial_node.state.eval_score;
    int best_history_index = -1;

    for (int depth = 0; depth < params.beam_depth; depth++) {

        if (chrono::steady_clock::now() >= deadline) {
            break;
        }

        vector<BeamCandidate> candidates;

        // ------------------------------------------------------------
        // 現在のBeamに含まれる全Stateから次のAction候補を作る
        // ------------------------------------------------------------
        for (int parent_index = 0;
             parent_index < (int)beam.size();
             parent_index++) {

            const BeamNode& node = beam[parent_index];

            if (node.state.is_finished(input)) {
                if (node.state.eval_score < best_score) {
                    best_score = node.state.eval_score;
                    best_history_index = node.history_index;
                }
                continue;
            }

            vector<Action> actions = generate_actions(input, node.state, params);

            for (const Action& action : actions) {

                // Actionsに対しスコアの差分(delta)が計算できる場合のみ
                // double delta =
                //     evaluate_action_delta(
                //         input,
                //         node.state,
                //         action,
                //         params
                //     );
                // double new_score = node.state.eval_score + delta;

                // Actionsに対しスコアの差分(delta)が計算できない場合
                // Stateコピー+フル評価が入るため遅い
                State new_state = node.state;
                new_state.apply(action);
                double new_score =  evaluate(input, new_state, params);

                candidates.push_back(
                    BeamCandidate{
                        parent_index,
                        action,
                        new_score
                    }
                );
            }
        }

        if (candidates.empty()) {
            break;
        }

        // ------------------------------------------------------------
        // 評価値の良い候補を上位beam_width個残す
        // ------------------------------------------------------------
        int next_size =
            min(max(0, params.beam_width), (int)candidates.size());

        partial_sort(
            candidates.begin(),
            candidates.begin() + next_size,
            candidates.end(),
            [](const BeamCandidate& a,
               const BeamCandidate& b) {
                return a.score > b.score;
            }
        );

        vector<BeamNode> next_beam;
        next_beam.reserve(next_size);

        for (int i = 0; i < next_size; i++) {

            const BeamCandidate& candidate =
                candidates[i];

            const BeamNode& parent =
                beam[candidate.parent_index];

            BeamNode child;

            // 実際にStateを作るのは上位候補だけ
            child.state = parent.state;
            child.state.apply(candidate.action);

            // 差分評価結果をそのまま保持
            child.state.eval_score =
                candidate.score;

            history.push_back(
                BeamHistory{
                    parent.history_index,
                    candidate.action
                }
            );
            child.history_index = (int)history.size() - 1;

            next_beam.push_back(std::move(child));
        }

        beam = std::move(next_beam);

        // 最良Stateを更新
        for (const BeamNode& node : beam) {

            if (node.state.eval_score < best_score) {
                best_score = node.state.eval_score;
                best_history_index = node.history_index;
            }

        }
    }

    vector<Action> best_actions;
    for (int index = best_history_index; index != -1;
         index = history[index].parent_index) {
        best_actions.push_back(history[index].action);
    }
    reverse(best_actions.begin(), best_actions.end());
    return best_actions;
}
// Todo: Simulated Annealing Utility
// 問題ごとに定義
using Solution = vector<Action>;

Solution make_initial_solution(const Input& input);

Solution make_neighbor(
    const Input& input,
    const Solution& current,
    RNG& rng
);

double evaluate_solution(
    const Input& input,
    const Solution& solution
);

// 共通のSA本体

Solution solve_sa(const Input& input, const Parameters& params, Timer& timer, RNG& rng) {
    Solution current = make_initial_solution(input);
    double current_eval = evaluate_solution(input, current);

    Solution best = current;
    double best_eval = current_eval;

    const double TIME_LIMIT = params.time_limit;

    double elapsed = 0.0;
    double progress = 0.0;
    while (true) {
        elapsed = timer.elapsed();
        if (elapsed > TIME_LIMIT) {
            break;
        }
        progress = elapsed / TIME_LIMIT;

        // 温度を線形に下げる
        double temp = params.start_temp
            + (params.end_temp - params.start_temp) * progress;

        Solution candidate = make_neighbor(input, current, rng);
        double candidate_eval = evaluate_solution(input, candidate);

        double delta = candidate_eval - current_eval;

        // 改善なら採用。
        // 改悪なら温度に応じて確率的に採用。
        if (delta >= 0 || rng.next_double() < exp(delta / temp)) {
            current = std::move(candidate);
            current_eval = candidate_eval;
        }

        if (current_eval > best_eval) {
            best = current;
            best_eval = current_eval;
        }
    }

    return best;
}



int main() {

    using namespace std;

    // 時間計測の開始
    // chrono::system_clock::time_point start, end;
    // start = chrono::system_clock::now();

    Timer timer;
    timer.start_timer();
    const int TIME_LIMIT = 1900; // ms

    RNG rng(12345);  // seed固定

    // 入力
    Input input;

    // 初期解を作成
    State state;

    // 結果作成

    // 解を作成

    //スコアを計算

    // ビームサーチ呼び出し
    Parameters params;
    state.eval_score = evaluate(input, state, params);
    auto deadline = chrono::steady_clock::now() + chrono::milliseconds(TIME_LIMIT);
    vector<Action> best_actions = beam_search(input, state, params, deadline);


    //焼きなまし用パラメータ
    double start_temp = 50;
    double end_temp = 10;
    
    //Solution answer = solve_sa(input, params, timer, rng);

    //ここからループ予定
    int step = 0;
    while(true){
        
        // end = chrono::system_clock::now();
        // double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
        double time = timer.elapsed();
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
    cout << "elapsed time: " << timer.elapsed() << " ms" << endl;

    //最後の出力
    //cout << endl;
    //output_ans(n, out);
}
