#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V){ this->V=V; adj.resize(V); }

    void addEdge(int u,int v){ adj[u].push_back(v); adj[v].push_back(u); }

    void parallelBFS(int s){
        vector<bool> vis(V,0);
        queue<int> q;
        vis[s]=1; q.push(s);

        cout<<"\nParallel BFS Traversal: ";
        while(!q.empty()){
            int n=q.size();

            #pragma omp parallel for
            for(int i=0;i<n;i++){
                int x=-1;

                #pragma omp critical
                if(!q.empty()){ x=q.front(); q.pop(); cout<<x<<" "; }

                if(x!=-1){
                    for(int y:adj[x]){
                        if(!vis[y]){
                            #pragma omp critical
                            if(!vis[y]) vis[y]=1,q.push(y);
                        }
                    }
                }
            }
        }
        cout<<endl;
    }

    void dfsUtil(int x, vector<bool>&vis){
        bool done;

        #pragma omp critical
        { done=vis[x]; if(!vis[x]) vis[x]=1, cout<<x<<" "; }

        if(done) return;

        #pragma omp parallel for
        for(int i=0;i<adj[x].size();i++){
            int y=adj[x][i];
            if(!vis[y]){
                #pragma omp task
                dfsUtil(y,vis);
            }
        }
    }

    void parallelDFS(int s){
        vector<bool> vis(V,0);
        cout<<"\nParallel DFS Traversal: ";

        #pragma omp parallel
        #pragma omp single
        dfsUtil(s,vis);

        cout<<endl;
    }
};

int main(){
    int V,E,u,v,s;
    cout<<"Enter number of vertices: "; cin>>V;

    Graph g(V);

    cout<<"Enter number of edges: "; cin>>E;
    cout<<"Enter edges (u v):\n";
    for(int i=0;i<E;i++) cin>>u>>v, g.addEdge(u,v);

    cout<<"Enter starting vertex: "; cin>>s;

    g.parallelBFS(s);
    g.parallelDFS(s);
}
