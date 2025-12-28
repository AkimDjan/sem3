// Боггл (англ. Boggle) — настольная игра, в которую играют обычно два человека, 
// составляя слова из имеющихся у них букв и записывая их на бумаге.
// Дана доска Боггл N × N. И список из M слов, проверить какие из них действительно
// есть на данной доске и вывести их на экран, сохранив порядок.

// Формат ввода
// В первой строке записано одно натуральное число N - размер исходной доски. 
// В следующих N строках через пробел записана последовательность букв, длиной N - исходное поле Боггл.
// В следующей строке записано число M - число слов, которые необходимо проверить. 
// В следующей строке через пробел записано M слов.

// Формат вывода
// Слова через пробел, которые встречаются на данном поле Боггл, необходимо сохранить
// их относительный исходный порядок.

// Пример
// Ввод
// 4
// M S E F
// R A T D
// L O N E
// K A F B
// 4
// START NOTE SAND STONED
// Вывод
// NOTE SAND STONED

// Идея - Все слова из списка помещаются в бор (trie), после чего по доске Боггл запускается
// DFS из каждой клетки, одновременно двигаясь по бору и отсекая все пути, которые не являются 
// префиксами ни одного слова

#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>


using namespace std;

struct Node {
    int next[26];
    vector<int> ids; 
    bool leaf;
    Node() {
        fill(next, next + 26, -1);
        leaf = false;
    }
};

int N;
vector<string> board;
vector<string> words;
vector<Node> trie;
vector<bool> found;
vector<vector<bool>> used;

void add_word(const string &s, int id) {
    int v = 0;
    for (char ch : s) {
        int c = ch - 'A';
        if (trie[v].next[c] == -1) {
            trie[v].next[c] = (int)trie.size();
            trie.emplace_back();
        }
        v = trie[v].next[c];
    }
    trie[v].leaf = true;
    trie[v].ids.push_back(id);
}

int dx[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

void dfs(int x, int y, int node) {
    int c = board[x][y] - 'A';
    int nxt = trie[node].next[c];
    if (nxt == -1) return; 

    node = nxt;
    used[x][y] = true;

    if (trie[node].leaf) {
        for (int id : trie[node].ids) {
            found[id] = true;
        }
    }

    for (int dir = 0; dir < 8; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (0 <= nx && nx < N && 0 <= ny && ny < N && !used[nx][ny]) {
            dfs(nx, ny, node);
        }
    }

    used[x][y] = false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N)) {
        return 0;
    }

    board.assign(N, string(N, ' '));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char ch;
            cin >> ch; 
            board[i][j] = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        }
    }

    int M;
    cin >> M;
    words.resize(M);
    for (int i = 0; i < M; ++i) {
        cin >> words[i];
    }

    trie.clear();
    trie.emplace_back();         
    found.assign(M, false);
    
    for (int i = 0; i < M; ++i) {
        for (char &ch : words[i]) ch = toupper((unsigned char)ch);
        add_word(words[i], i);
    }

    used.assign(N, vector<bool>(N, false));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dfs(i, j, 0);
        }
    }
    
    bool first = true;
    for (int i = 0; i < M; ++i) {
        if (found[i]) {
            if (!first) cout << ' ';
            cout << words[i];
            first = false;
        }
    }
    cout << '\n';

    return 0;
}
