// Dica pra ler os comentarios: coloque o editor de texto com "text wrapping".
#include <algorithm>
#include <utility>
#include <tuple>
#include <queue>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std; // facilitando leitura
                                    
int n;                              // quantidade de pessoas/bicicletas.
int l,c;                            // numero de linhas e colunas do mapa da lagoa.
vector<vector<char>> mapa;          // mapa da lagoa.
vector<queue<int>> visit_to_bikes;  // posicao i indica a fila de bicicletas pra quem o visitante i vai pedir, em ordem (primeira da fila eh a primeira bicicleta que ele vai tentar parear).
vector<vector<int>> bike_to_ranks;  // posicao i, j indica a posicao do visitante i no rank da bicicleta j, sendo que o visitante preferido da bicicleta j tem rank 0.
vector<int> bykepair;               // posicao i eh com quem a bicicleta i esta pareada, -1 se nao esta pareada com ninguem

// verifica se eh valido andar pra posicao i,j
bool valid_pos(int i, int j) {
    return i >= 0 && i < l && j >= 0 && j < c && mapa[i][j] != '-';
}

vector<pair<int,int>> movs  = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // formas de andar pelo mapa (nao andamos na diagonal)

int main() {
    // Lendo tamanhos:
    cin >> n;
    cin >> l >> c;
    
    // Lendo o mapa:
    mapa.resize(l,vector<char>(c));
    for (auto &lin: mapa)
        for (auto &c: lin)
            cin >> c;

    // Lendo prioridades dos visitantes:
    visit_to_bikes.resize(n);
    for (int i = 0; i < n; i++) {
        vector<pair<int,int>> tmp(n); // pares -prioridade, bike_id
        for (int j = 0; j < n; j++) {
            cin >> tmp[j].first;
            tmp[j].first *= -1;
            tmp[j].second = j;
        }
        sort(tmp.begin(),tmp.end());// note que isso colocarah os pares com menor primeiro valor antes, e caso o primeiro valor empate, a de menor segundo valor vem primeiro. Como colocamos -prioridade como primeiro valor, as maiores prioridades vem primeiro, e em caso de empate, as bicicletas com menor ID.
        for (auto p : tmp)
            visit_to_bikes[i].push(p.second);
    }

    // Lendo prioridades das bicicletas:
    bike_to_ranks.resize(n, vector<int>(n));
    for (int i = 0; i < l; i++)
        for(int j = 0; j < c; j++)
            if (mapa[i][j] >= '0' && mapa[i][j] <= '9') { // se tem uma bicicleta, rodamos a BFS pra encontrar suas prioridades.
            	vector<pair<int,int>> tmp; // pares {distancia, id do visitante}
            	
                // Rodando BFS pra bicicleta 
                queue<tuple<int,int,int>> BFS_queue; // fila de {i, j, distancia da fonte}
                vector<vector<bool>> visited (l, vector<bool>(c, false)); 
                BFS_queue.push({i, j, 0});
                while (!BFS_queue.empty()) {
                    auto tup = BFS_queue.front();
                    int ni = get<0>(tup), nj = get<1>(tup), dist = get<2>(tup);
                    BFS_queue.pop();

                    if(visited[ni][nj])
                        continue;
                    visited[ni][nj] = true;

                    if (mapa[ni][nj] >= 'a' && mapa[ni][nj] <= 'j') 
                        tmp.push_back({dist, mapa[ni][nj]});

                    for (auto mv : movs)
                        if (valid_pos(ni + mv.first, nj + mv.second))
                            BFS_queue.push({ni + mv.first, nj + mv.second, dist + 1});
                }
                assert((int)tmp.size() == n); // conferindo se todos visitantes foram achados (como o grafo eh conexo sempre deve achar todos visitantes).
                sort(tmp.begin(), tmp.end()); // menores distantes primeiro, e em caso de empate, menor id
                for (int k = 0; k < n; k++)
                    bike_to_ranks[tmp[k].second-'a'][mapa[i][j] - '0'] = k;
            }

    // Rodando algoritmo de Gale-Shapley:
    bykepair.resize(n,-1); // -1 pras bicicletas nao pareadas.
    queue<int> unmatched_visits;
    for (int i = 0; i < n; i++)
        unmatched_visits.push(i);// inicialmente nenhum visitante pareado.
    while (!unmatched_visits.empty()) {
        int another = unmatched_visits.front(); // another eh o visitante nao pareado que vai propor.
        unmatched_visits.pop();
        
        while(!visit_to_bikes[another].empty()) {
            int totry = visit_to_bikes[another].front(); // totry eh a bicicleta pra quem another tenta propor.
            visit_to_bikes[another].pop();
            // Verificando se a bicicleta nao estava pareada ou another estah mais bem rankeada que o antigo par de totry:
            if (bykepair[totry] == -1 || bike_to_ranks[another][totry] < bike_to_ranks[bykepair[totry]][totry]) {
                if (bykepair[totry] != -1)
                    unmatched_visits.push(bykepair[totry]); // se a bicicleta totry ja estava pareada, colocamos seu antigo par de volta na fila.
                bykepair[totry] = another;
                break;
            }
        }
    }
    
    // Colocando pares no formato de resposta {visitante, bicicleta}
    vector<pair<char,char>> ans; // pares {visitante, bicicleta}
    for (int bik = 0; bik < n; bik++) {
        assert(bykepair[bik] != -1); // checando se todos visitantes foram pareados
        ans.push_back({bykepair[bik]+'a', bik+'0'});
    }
    sort(ans.begin(),ans.end());// colocando pras respostas ficarem na ordem alfabetica dos visitantes
    
    // Imprimindo resposta:
    for (auto p : ans)
        cout << p.first << ' '  << p.second << endl;
}
