#include <algorithm>
#include <utility>
#include <tuple>
#include <queue>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std; // tirar

int n;
int l,c;
vector<vector<char>> mapa;
vector<queue<int>> visit_to_bikes; // posicao i indica a fila de bicicletas pra quem i vai pedir
vector<vector<int>> bike_to_ranks; // posicao i,j indica a posicao do visitante i no rank da bicicleta j, sendo 0 o preferido
vector<int> bykepair; // posicao i eh com quem a bicicleta i ta pareada

bool valid_pos(int i, int j) {
    return i >= 0 && i < l && j >= 0 && j < c && mapa[i][j] != '-';
}

vector<pair<int,int>> movs  = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};


int main() {
    cin >> n;
    
    cin >> l >> c;
    
    mapa.resize(l,vector<char>(c));
    
    for (auto &lin: mapa)
        for (auto &c: lin)
            cin >> c;
    
    visit_to_bikes.resize(n);
    for (int i = 0; i < n; i++) {
        vector<pair<int,int>> tmp(n); // -prioridade, bike_id
        for (int j = 0; j < n; j++) {
            cin >> tmp[j].first;
            tmp[j].first *= -1;
            tmp[j].second = j;
        }
        sort(tmp.begin(),tmp.end());

        for (auto p : tmp)
            visit_to_bikes[i].push(p.second);
    }
    
    bike_to_ranks.resize(n, vector<int>(n));
    for (int i = 0; i < l; i++)
        for(int j = 0; j < c; j++)
            if (mapa[i][j] >= '0' && mapa[i][j] <= '9') {
            	vector<pair<int,int>> tmp; // distancia, id do visitante

                // Rodando BFS pra bicicleta 
                queue<tuple<int,int,int>> BFS_queue; // i, j, dist da fonte
                vector<vector<bool>> visited (l, vector<bool>(c, false)); 
                BFS_queue.push({i, j, 0});
                
                while (!BFS_queue.empty()) {
                    auto tup = BFS_queue.front();
                    BFS_queue.pop();
                    
                    
                    int ni = get<0>(tup), nj = get<1>(tup), dist = get<2>(tup);
                    if(visited[ni][nj])
                        continue;
                    visited[ni][nj] = true;
                    
                    if (mapa[ni][nj] >= 'a' && mapa[ni][nj] <= 'j') 
                        tmp.push_back({dist, mapa[ni][nj]});
                    
                    
                    for (auto mv : movs)
                        if (valid_pos(ni + mv.first, nj + mv.second))
                            BFS_queue.push({ni + mv.first, nj + mv.second, dist + 1});
                }

                assert((int)tmp.size() == n);

                sort(tmp.begin(), tmp.end());
                for (int k = 0; k < n; k++)
                    bike_to_ranks[tmp[k].second-'a'][mapa[i][j] - '0'] = k;
                
            }
            
            
    bykepair.resize(n,-1);
    queue<int> unmatched_visits;
    for (int i = 0; i < n; i++)
        unmatched_visits.push(i);
    while (!unmatched_visits.empty()) {
        int another = unmatched_visits.front();
        unmatched_visits.pop();
        
        while(!visit_to_bikes[another].empty()) {
            int totry = visit_to_bikes[another].front();
            visit_to_bikes[another].pop();

            if (bykepair[totry] == -1 || bike_to_ranks[another][totry] < bike_to_ranks[bykepair[totry]][totry]) {
                if (bykepair[totry] != -1)
                    unmatched_visits.push(bykepair[totry]);
                bykepair[totry] = another;
                break;
            }
        }
    }
  
    vector<pair<char,char>> ans;
    for (int bik = 0; bik < n; bik++) {
        assert(bykepair[bik] != -1);
        ans.push_back({bykepair[bik]+'a', bik+'0'});
    }
    sort(ans.begin(),ans.end());
    
    for (auto p : ans)
        cout << p.first << ' '  << p.second << endl;
}
