#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include <unordered_map>
using ll = long long;
using namespace std;
#define endl "\n"
#define f(i, a, b) for (int i = a; i < b; i++)





void PrintDebugInfo(
    const vector<vector<vector<int>>>& path,
    const vector<vector<int>>& dist,
    const vector<vector<int>>& minfuelforpath,
    const int a[],
    int T
) {
    cout << "---- Node Types (a[i]) ----" << endl;
    for (int i = 0; i < T; i++) {
        cout << "Node " << i << " type: ";
        if (a[i] == 0) cout << "None";
        else if (a[i] == 1) cout << "Hub";
        else if (a[i] == 2) cout << "House";
        else if (a[i] == 3) cout << "Fuel";
        cout << endl;
    }

    cout << "\n---- Paths ----" << endl;
    for (int src = 0; src < T; src++) {
        for (int dest = 0; dest < T; dest++) {
            if (!path[src][dest].empty()) {
                cout << "Path from " << src << " to " << dest << ": ";
                for (int node : path[src][dest]) {
                    cout << node << " ";
                }
                cout << endl;
            }
        }
    }

    cout << "\n---- Distances ----" << endl;
    for (int src = 0; src < T; src++) {
        cout << "From source " << src << ": ";
        for (int dest = 0; dest < T; dest++) {
            if (dist[src][dest] == INF) cout << "INF ";
            else cout << dist[src][dest] << " ";
        }
        cout << endl;
    }

    cout << "\n---- Min Fuel Needed per Path ----" << endl;
    for (int src = 0; src < T; src++) {
        cout << "From source " << src << ": ";
        for (int dest = 0; dest < T; dest++) {
            cout << minfuelforpath[src][dest] << " ";
        }
        cout << endl;
    }
}








void Minfuelforpathfunction(int source,int a[],vector<vector<int>>& path,vector<int> &minfuelforpath,vector<vector<int>> d){
    for(int i=0;i<path.size();i++){
        if(a[path[i][0]]==3){
                break;
            }
        for(int j=1;j<path[i].size();j++){
            minfuelforpath[i]+=d[path[i][j-1]][path[i][j]];
            if(a[path[i][j]]==3){
                break;
            }
        }
    }
}






void DijkstraAlgo(
    int source,
    const vector<vector<pair<int, int>>>& adj,
    vector<int>& dist,
    vector<vector<int>>& path,
    int n,
    int a[],
    int maxfuel,
    vector<int>& fuel
) {
    vector<int> parent(n, -1);

    dist[source] = 0;
    fuel[source] = maxfuel;

    // Priority queue: (distance, node, fuel_left)
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    pq.push({0, source, maxfuel});

    while (!pq.empty()) {
        auto [d, u, f] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& [v, w] : adj[u]) {
            if (w > f) continue; // Not enough fuel to go to v

            int new_fuel = f - w;
            if (a[v] == 3) new_fuel = maxfuel; // Refuel at v

            int new_dist = dist[u] + w;

            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                fuel[v] = new_fuel;
                parent[v] = u;
                pq.push({new_dist, v, new_fuel});
            }
        }
    }

    // Reconstruct paths from source to all nodes
    path.assign(n, vector<int>());
    for (int v = 0; v < n; ++v) {
        if (dist[v] == INF) continue;
        for (int curr = v; curr != -1; curr = parent[curr]) {
            path[v].push_back(curr);
        }
        reverse(path[v].begin(), path[v].end());
    }
}








void solve() {
    
    int N, T, M, K, F;
    cin>>N>>T>>M>>K>>F;
    vector<int> hubnode(N), housenode(N), fuelnode(K);
    vector<vector<pair<int, int > > > graph(T);
    int a[T]; // for knowing a given a value if 0 it is nothing if 1 it is hub if 2 it is house if 3 fuel;
    map <int,int>hubofhouse;
    vector<vector<int>> d(T,vector<int>(T));

    for(int i=0;i<N;i++){   
        cin>>hubnode[i];
        a[hubnode[i]]=1;
    }
    for(int i=0;i<N;i++){
        cin>>housenode[i];
        a[housenode[i]]=2;
        hubofhouse[housenode[i]]=hubnode[i];
    }
    for(int i=0;i<K;i++){
        cin>>fuelnode[i];
        a[fuelnode[i]]=3;
    }
    for(int i=0;i<M;i++){
        int a,b,c;
        cin>>a>>b>>c;
        graph[a].push_back({b,c});
        d[a][b]=c;
        d[b][a]=c;
        graph[b].push_back({a, c});
    }


    vector<vector<int>> dist(T, vector<int>(T, INF));
    vector<vector<vector<int>>> path(T, vector<vector<int>>(T));
    vector<vector<int>> fuel(T, vector<int>(T, -1));
    vector<vector<int>> minfuelforpath(T, vector<int>(T, 0));
    
    

    for(int i=0;i<N;i++){
        DijkstraAlgo(hubnode[i],graph,dist[hubnode[i]],path[hubnode[i]],T,a,F,fuel[i]);
        DijkstraAlgo(housenode[i],graph,dist[housenode[i]],path[housenode[i]],T,a,F,fuel[i]);

    }
    for(int i=0;i<K;i++){
        DijkstraAlgo(fuelnode[i],graph,dist[fuelnode[i]],path[fuelnode[i]],T,a,F,fuel[i]);
    }
    for(int i=0;i<N;i++){
        Minfuelforpathfunction(hubnode[i],a,path[hubnode[i]],minfuelforpath[hubnode[i]],d);
        Minfuelforpathfunction(housenode[i],a,path[housenode[i]],minfuelforpath[housenode[i]],d);
    }

    for(int i=0;i<K;i++){
        Minfuelforpathfunction(fuelnode[i],a,path[fuelnode[i]],minfuelforpath[fuelnode[i]],d);
    }
    // PrintDebugInfo(path, dist, minfuelforpath, a, T);




   // for every hub , house, the nearest fuel station, the nearst house ,the nearest hub;
    vector<vector<vector<pair<int,int>>>>hubarr(T,vector<vector<pair<int,int>>>(3));
    vector<vector<vector<pair<int,int>>>>housearr(T,vector<vector<pair<int,int>>>(3));
    vector<vector<vector<pair<int,int>>>>fuelarr(T,vector<vector<pair<int,int>>>(3));



    for(int i=0;i<N;i++){
      // store in this  hubarr[hubnode[i]]
      // hub 0 house 1 fuel 2
      for(int j=0;j<N;j++){
        if(j!=i){
            hubarr[hubnode[i]][0].push_back({dist[hubnode[i]][hubnode[j]],hubnode[j]});
        }
      }
      for(int j=0;j<N;j++){
            hubarr[hubnode[i]][1].push_back({dist[hubnode[i]][housenode[j]],housenode[j]}); 
      }
       for(int j=0;j<K;j++){
            hubarr[hubnode[i]][2].push_back({dist[hubnode[i]][fuelnode[j]],fuelnode[j]}); 
      }

    }

    for(int i=0;i<N;i++){
      for(int j=0;j<N;j++){
        if(j!=i){
            housearr[housenode[i]][1].push_back({dist[housenode[i]][housenode[j]],housenode[j]});
        }
      }
      for(int j=0;j<N;j++){
            housearr[housenode[i]][0].push_back({dist[housenode[i]][hubnode[j]],hubnode[j]});
      }
       for(int j=0;j<K;j++){
            housearr[housenode[i]][2].push_back({dist[housenode[i]][fuelnode[j]],fuelnode[j]}); 
      }

    }

    for(int i=0;i<K;i++){
      for(int j=0;j<K;j++){
        if(j!=i){
            fuelarr[fuelnode[i]][2].push_back({dist[fuelnode[i]][fuelnode[j]],fuelnode[j]});
        }
      }
      for(int j=0;j<N;j++){
            fuelarr[fuelnode[i]][0].push_back({dist[fuelnode[i]][hubnode[j]],hubnode[j]});
      }
       for(int j=0;j<N;j++){
            fuelarr[fuelnode[i]][1].push_back({dist[fuelnode[i]][housenode[j]],housenode[j]});
      }

    }   


    // Sorting in decending order
    for (int i = 0; i < N; i++) {

    sort(hubarr[hubnode[i]][0].begin(), hubarr[hubnode[i]][0].end(), 
         greater<pair<int, int>>());
    sort(hubarr[hubnode[i]][1].begin(), hubarr[hubnode[i]][1].end(), 
         greater<pair<int, int>>());
    sort(hubarr[hubnode[i]][2].begin(), hubarr[hubnode[i]][2].end(), 
         greater<pair<int, int>>());
    
    sort(housearr[housenode[i]][0].begin(), housearr[housenode[i]][0].end(), 
         greater<pair<int, int>>());
    sort(housearr[housenode[i]][1].begin(), housearr[housenode[i]][1].end(), 
         greater<pair<int, int>>());
    sort(housearr[housenode[i]][2].begin(), housearr[housenode[i]][2].end(), 
         greater<pair<int, int>>());
    
    }

    for( int i = 0;i < K; i++){
        sort(fuelarr[fuelnode[i]][0].begin(), fuelarr[fuelnode[i]][0].end(), 
         greater<pair<int, int>>());

        sort(fuelarr[fuelnode[i]][1].begin(), fuelarr[fuelnode[i]][1].end(), 
         greater<pair<int, int>>());

        sort(fuelarr[fuelnode[i]][2].begin(), fuelarr[fuelnode[i]][2].end(), 
         greater<pair<int, int>>());
    }




    vector<pair<int,vector<int>>>route;
    vector<vector< tuple< tuple<int,int,int>, pair<int,int> , pair<int, vector<int>>>> > parent(N);
    vector<vector<tuple<vector<bool>,vector<bool>,int>>>ofparent(N);
    



    for(int u=0;u<N;u++){
        


        int startnode=hubnode[u];
        int currentfuel = F;
        int housecnt=0;
        int currdist=0;


        vector<int>currroute;
        vector<bool> house_visited(T, false);
        vector<bool> hub_visited(T, false);


        int currnode=startnode;
        int phouse;
        int phub;
        int pfuel;
        bool pathwillnotwork=false;

        int decision=-1;
        bool usingparent;


        while(housecnt < N){
            
            pair<int,int>p4;
            usingparent=false;
            if (pathwillnotwork) {
                if(parent[u].empty()){
                    pathwillnotwork=true;
                    break;
                }
                


            pathwillnotwork=false;
            auto [p1, p2, p3] = parent[u].back(); 
            auto [p5,p6,p7] =ofparent[u].back();
            currnode = get<0>(p1);
            currdist = get<1>(p1);
            decision = get<2>(p1);
            currentfuel = p3.first;
            currroute = p3.second;
            

            usingparent=false;
            if((p4.first<=0&&p4.second<0)||(p4.second<=0&&p4.first<0)){
                pathwillnotwork=true;
                break;
            }
            p4.first=p2.first;
            p4.second=p2.second;
            hub_visited=p5;
            house_visited=p6;
            housecnt=p7;
            usingparent = true;
            parent[u].pop_back();
            ofparent[u].pop_back();
        }

            // IDEA HERE IS IF THE ROUTE IS NOT FEASIBLE WITH THAT FUEL THEN GO TO THE NEAREST FUEL STATION AND COME AGAIN TO THE DESTINATION
            
            if(!usingparent){
                currroute.push_back(currnode);
                
            if(a[currnode]==1){
                
                hub_visited[currnode]=true;
                phouse=N-1;
                phub=N-2;
                pfuel=K-1;


                while(true){
                    if(phouse>=0){

                    int temp=hubarr[currnode][1][phouse].second;
                    int temp1=hubarr[currnode][1][phouse].first;


                    if(hub_visited[hubofhouse[temp]]&&!house_visited[temp]){
                        if(minfuelforpath[currnode][temp]<=currentfuel){
                                parent[u].push_back({{currnode,currdist, 1}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=temp1;


                            f(i,1,path[currnode][temp].size()-1){
                                currroute.push_back(path[currnode][temp][i]);
                                currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                if(a[path[currnode][temp][i]]==3){
                                    currentfuel=F;
                                }
                            }
                            
                            currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                            currnode=temp;
                            
                            break;
                            }
                            else{
                                
                                int temp2=temp;
                                temp=hubarr[currnode][2][pfuel].second;


                                if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                    
                                    parent[u].push_back({{currnode,currdist, 1}, {phouse, phub}, {currentfuel, currroute}});
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                    currdist+=hubarr[currnode][2][pfuel].first;


                                    f(i,1,path[currnode][temp].size()){
                                        currroute.push_back(path[currnode][temp][i]);
                                    }

                                    currentfuel=F;

                                    f(i,1,path[temp][temp2].size()-1){
                                        currroute.push_back(path[temp][temp2][i]);
                                        currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        if(a[path[temp][temp2][i]]==3){
                                            currentfuel=F;
                                        }
                                    }


                                currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currnode=temp2;


                                break;
                                }
                            }
                            
                        }}
                   
                        phouse--;

                        if(phub>=0){

                        int temp=hubarr[currnode][0][phub].second;
                        int temp1=hubarr[currnode][0][phub].first;

                        if(!hub_visited[temp]){


                            if(minfuelforpath[currnode][temp]<=currentfuel){

                                parent[u].push_back({{currnode,currdist, 2}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=temp1;

                                f(i,1,path[currnode][temp].size()-1){

                                currroute.push_back(path[currnode][temp][i]);
                                currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                if(a[path[currnode][temp][i]]==3){
                                    currentfuel=F;
                                }

                                }

                            currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                            currnode=temp;

                            break;
                            }
                            else{
                               
                                int temp2=temp;
                                temp=hubarr[currnode][2][pfuel].second;

                            if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                
                                
                                parent[u].push_back({{currnode,currdist, 2}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=hubarr[currnode][2][pfuel].first;

                                f(i,1,path[currnode][temp].size()){
                                    currroute.push_back(path[currnode][temp][i]);
                                }

                                currentfuel=F;

                                f(i,1,path[temp][temp2].size()-1){

                                    currroute.push_back(path[temp][temp2][i]);
                                    currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                    currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);

                                    if(a[path[temp][temp2][i]]==3){
                                        currentfuel=F;
                                    }
                                }

                                currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currnode=temp2;


                                break;
                            }
                        }
                    
                    }
                }

                                    phub--;

                                    if(phouse<0&&phub<0){
                                        pathwillnotwork=true;
                                        break;
                                    }
                
            }
            }
            else{
                
                
                    house_visited[currnode]=true;
                    housecnt++;
                    if(housecnt==N){
                        break;
                    }
                    phouse=N-2;
                    phub=N-1;
                    pfuel=K-1;

                    while(true){
                    
                    
                     if(phouse>=0){
                            int temp=housearr[currnode][1][phouse].second;
                            int temp1=housearr[currnode][1][phouse].first;

                        if(hub_visited[hubofhouse[temp]]&&!house_visited[temp]){

                            if(minfuelforpath[currnode][temp]<=currentfuel){

                                parent[u].push_back({{currnode,currdist, 3}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=temp1;

                                f(i,1,path[currnode][temp].size()-1){
                                    currroute.push_back(path[currnode][temp][i]);
                                    currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                    if(a[path[currnode][temp][i]]==3){
                                        currentfuel=F;
                                    }
                                }

                                currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                                currnode=temp;
                                break;

                            }
                            else{
                                
                                int temp2=temp;
                                temp=housearr[currnode][2][pfuel].second;

                                if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                    
                                    parent[u].push_back({{currnode,currdist, 3}, {phouse, phub}, {currentfuel, currroute}});
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                    currdist+=housearr[currnode][2][pfuel].first;

                                    f(i,1,path[currnode][temp].size()){
                                        currroute.push_back(path[currnode][temp][i]);
                                    }

                                    currentfuel=F;

                                    f(i,1,path[temp][temp2].size()-1){
                                        currroute.push_back(path[temp][temp2][i]);
                                        currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        if(a[path[temp][temp2][i]]==3){
                                            currentfuel=F;
                                        }
                                    }

                                currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                currnode=temp2;
                                break;


                                }
                            }
                            
                        }}
                        phouse--;
                            if(phub>=0){
                                int temp=housearr[currnode][0][phub].second;
                                int temp1=housearr[currnode][0][phub].first;

                                if(!hub_visited[temp]){

                                    if(minfuelforpath[currnode][temp]<=currentfuel){

                                        parent[u].push_back({{currnode,currdist, 4}, {phouse, phub}, {currentfuel, currroute}});
                                        ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                        currdist+=temp1;

                                        f(i,1,path[currnode][temp].size()-1){
                                        currroute.push_back(path[currnode][temp][i]);
                                        currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];

                                        if(a[path[currnode][temp][i]]==3){
                                            currentfuel=F;
                                        }
                                        }

                                        currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                                        currnode=temp;
                                        break;

                                    }
                                    else{
                                        
                                        int temp2=temp;
                                        temp=housearr[currnode][2][pfuel].second;

                                    if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                        parent[u].push_back({{currnode,currdist, 4}, {phouse, phub}, {currentfuel, currroute}});
                                        ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                        currdist+=housearr[currnode][2][pfuel].second;

                                        f(i,1,path[currnode][temp].size()){
                                            currroute.push_back(path[currnode][temp][i]);
                                        }

                                        currentfuel=F;

                                        f(i,1,path[temp][temp2].size()-1){
                                            currroute.push_back(path[temp][temp2][i]);
                                            currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                            currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                            if(a[path[temp][temp2][i]]==3){
                                                currentfuel=F;
                                            }
                                        }

                                        currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                        currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                        currnode=temp2;
                                        break;

                                    }
                                }
                            
                            
                        }   
                        }
                                    phub--;
                                    if(phouse<0&&phub<0){
                                        pathwillnotwork=true;
                                        break;
                                    }
                    }
            }
            }
            else{
                
                if(a[currnode]==1){
                    phouse=p4.first;
                    phub=p4.second;
                    pfuel=K-1;
                    bool dec=false;

                    if(decision==1||decision==3){
                        dec=true;
                    }
                    else{
                        phub--;
                    }

                    int cnt=0;
                while(true){
                    
                    
                    if(!dec || cnt>0){

                    if(phouse>=0){

                        int temp=hubarr[currnode][1][phouse].second;
                        int temp1=hubarr[currnode][1][phouse].first;
                    
                    
                        if(hub_visited[hubofhouse[temp]]&&!house_visited[temp]){

                            if(minfuelforpath[currnode][temp]<=currentfuel){

                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                 parent[u].push_back({{currnode,currdist, 1}, {phouse, phub}, {currentfuel, currroute}});
                                currdist+=temp1;

                                f(i,1,path[currnode][temp].size()-1){
                                    currroute.push_back(path[currnode][temp][i]);
                                    currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                    if(a[path[currnode][temp][i]]==3){
                                        currentfuel=F;
                                    }
                                }
                            

                            currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                            currnode=temp;
                            
                            break;
                            }
                            else{
                               
                                
                                int temp2=temp;
                                temp=hubarr[currnode][2][pfuel].second;
                                if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                   
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                     parent[u].push_back({{currnode,currdist, 1}, {phouse, phub}, {currentfuel, currroute}});
                                    currdist+=hubarr[currnode][2][pfuel].first;

                                    f(i,1,path[currnode][temp].size()){
                                        currroute.push_back(path[currnode][temp][i]);
                                    }

                                    currentfuel=F;

                                    f(i,1,path[temp][temp2].size()-1){
                                        currroute.push_back(path[temp][temp2][i]);
                                        currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        if(a[path[temp][temp2][i]]==3){
                                            currentfuel=F;
                                        }
                                    }

                                    currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currnode=temp2;
                               
                                break;
                                }
                            }
                            cnt++;
                        }
                        } }
                        phouse--;
                        if(phub>=0){

                        int temp=hubarr[currnode][0][phub].second;
                        int temp1=hubarr[currnode][0][phub].first;
                        
                        
                        if(!hub_visited[temp]){
                            
                            if(minfuelforpath[currnode][temp]<=currentfuel){
                                parent[u].push_back({{currnode,currdist, 2}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=temp1;
                                f(i,1,path[currnode][temp].size()-1){
                                currroute.push_back(path[currnode][temp][i]);
                                currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                if(a[path[currnode][temp][i]]==3){
                                    currentfuel=F;
                                }
                                }
                                currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                                currnode=temp;
                            
                            break;
                            }
                            else{
                                
                                int temp2=temp;
                                temp=hubarr[currnode][2][pfuel].second;
                                if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){

                                    parent[u].push_back({{currnode,currdist, 2}, {phouse, phub}, {currentfuel, currroute}});
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                    currdist+=hubarr[currnode][2][pfuel].first;

                                    f(i,1,path[currnode][temp].size()){
                                        currroute.push_back(path[currnode][temp][i]);
                                    }

                                    currentfuel=F;

                                    f(i,1,path[temp][temp2].size()-1){
                                        currroute.push_back(path[temp][temp2][i]);
                                        currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        if(a[path[temp][temp2][i]]==3){
                                            currentfuel=F;
                                        }
                                    }

                                    currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currnode=temp2;
                                    break;

                                }
                        }
                    
                }
                }
                                    phub--;
                                    if(phouse<0&&phub<0){
                                        pathwillnotwork=true;
                                        break;
                                    }
                
                }
            }
            else{

                    if(housecnt==N){
                        break;
                    }
                    phouse=p4.first;
                    phub=p4.second;
                    pfuel=K-1;
                    bool dec=false;

                    if(decision==1||decision==3){
                        dec=true;
                    }
                    else{
                        phub--;
                    }

                    int cnt=0;
                    while(true){
                    if(!dec || cnt>0){

                    if(phouse>=0){
                        int temp=housearr[currnode][1][phouse].second;
                        int temp1=housearr[currnode][1][phouse].first;

                        if(hub_visited[hubofhouse[temp]]&&!house_visited[temp]){

                            if(minfuelforpath[currnode][temp]<=currentfuel){

                                parent[u].push_back({{currnode,currdist, 3}, {phouse, phub}, {currentfuel, currroute}});
                                ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                currdist+=temp1;

                                f(i,1,path[currnode][temp].size()-1){
                                    currroute.push_back(path[currnode][temp][i]);
                                    currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                    if(a[path[currnode][temp][i]]==3){
                                        currentfuel=F;
                                    }
                                }
                                currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                                currnode=temp;
                                break;

                            }
                            else{
                                
                                int temp2=temp;
                                temp=housearr[currnode][2][pfuel].second;

                                if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){
                                    
                                    parent[u].push_back({{currnode,currdist, 3}, {phouse, phub}, {currentfuel, currroute}});
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                    currdist+=housearr[currnode][2][pfuel].first;

                                    f(i,1,path[currnode][temp].size()){
                                        currroute.push_back(path[currnode][temp][i]);
                                    }

                                    currentfuel=F;

                                    f(i,1,path[temp][temp2].size()-1){
                                        currroute.push_back(path[temp][temp2][i]);
                                        currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                        if(a[path[temp][temp2][i]]==3){
                                            currentfuel=F;
                                        }
                                    }

                                    currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                    currnode=temp2;
                                    break;


                                }
                            }

                            cnt++;

                        }
                        }
                    }

                        phouse--;

                        if(phub>=0){
                            int temp=housearr[currnode][0][phub].second;
                            int temp1=housearr[currnode][0][phub].first;

                            if(!hub_visited[temp]){
                                if(minfuelforpath[currnode][temp]<=currentfuel){

                                    parent[u].push_back({{currnode,currdist, 4}, {phouse, phub}, {currentfuel, currroute}});
                                    ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                    currdist+=temp1;

                                    f(i,1,path[currnode][temp].size()-1){

                                    currroute.push_back(path[currnode][temp][i]);
                                    currentfuel-=d[path[currnode][temp][i-1]][path[currnode][temp][i]];
                                    if(a[path[currnode][temp][i]]==3){
                                        currentfuel=F;
                                    }

                                    }

                                    currentfuel-=d[path[currnode][temp][path[currnode][temp].size()-2]][path[currnode][temp][path[currnode][temp].size()-1]];
                                    currnode=temp;
                                    break;

                                }
                                else{
                                    
                                    int temp2=temp;
                                    int temp=housearr[currnode][2][pfuel].second;

                                    if(minfuelforpath[currnode][temp]<=currentfuel&&minfuelforpath[temp][temp2]<=F){

                                        parent[u].push_back({{currnode,currdist, 4}, {phouse, phub}, {currentfuel, currroute}});
                                        ofparent[u].push_back({hub_visited,house_visited,housecnt});
                                        currdist+=housearr[currnode][2][pfuel].second;

                                        f(i,1,path[currnode][temp].size()){
                                            currroute.push_back(path[currnode][temp][i]);
                                        }

                                        currentfuel=F;

                                        f(i,1,path[temp][temp2].size()-1){
                                            currroute.push_back(path[temp][temp2][i]);
                                            currentfuel-=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                            currdist+=(d[path[temp][temp2][i-1]][path[temp][temp2][i]]);
                                            if(a[path[temp][temp2][i]]==3){
                                                currentfuel=F;
                                            }
                                        }

                                        currdist+=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                        currentfuel-=(d[path[temp][temp2][path[temp][temp2].size()-2]][path[temp][temp2][path[temp][temp2].size()-1]]);
                                        currnode=temp2;
                                        break;

                                    }
                            }
                        
                        
                        }
                    }
                                    phub--;
                                    if(phouse<0&&phub<0){
                                        pathwillnotwork=true;
                                        break;
                                    }
                }
            }



            }
            
        }
        
        
        
            if(!pathwillnotwork){
                route.push_back({currdist,currroute});
            }

   }


    sort(route.begin(), route.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;  // sort by first int ascending
    });




    if (!route.empty()) {
        cout << route[0].second.size() << endl;
        for (int i = 0; i < route[0].second.size(); i++) {
            cout << route[0].second[i] << ' ';
        }
        cout << endl;
    } else {
        cout << "No valid route found." << endl;
    }


    
}

int main() {
    solve();

    return 0;
}