#include <cstdio>
#include <vector>

using namespace std;

const int N = 110, inf = 1000000010;


struct Edge
{
	// b = adjacent vertex, c = cost per unit flow, u = capacity, f = current flow, back = id of back edge in adjacent vertex
	int b, c, u, f, back;
};

class MCMF
{
	private:
		// s=source, t=target, n=number of vertices
		int s, t, n;
		vector<Edge> g[N];

	public:

		MCMF(int _s, int _t, int _n)
		{
			s = _s;
			t = _t;
			n = _n;
		}

		// a = 1st vertex, b= 2nd vertex, c = cost per unit, u = capacity
		void addEdge(int a, int b, int c, int u)
		{
			Edge e1 = {b, c, u, 0, (int)g[b].size()};// forward edge
			Edge e2 = {a, -c, 0, 0, (int)g[a].size()};// back edge(cost is negative on removing flow thru back edge, cost is corr. reduced)
			g[a].push_back(e1);
			g[b].push_back(e2);
		}
		// O(MaxFlow*E*V)
		int minCostMaxFlow()
		{
			int flow = 0;
			int cost = 0;
			int *state = new int[n];
			int *from = new int[n];
			int *from_edge = new int[n];
			int *d = new int[n];
			int *q = new int[n];
			int qh, qt;
			
			while (true)
			{
				qh = 0, qt = 0;
				for (int i = 0; i < n; i++) state[i] = 2, d[i] = inf;
				fill(from, from + n, -1);// from[i]= state fom where i was reached
				state[s] = 1;
				// state[x]=0(not visited) state[x]=1(in Q) state[x]=2(visited)
				q[qh++] = s;// add state src(0) to Q
				d[s] = 0;// update distance of 1st vertex to 1
				// SPFA algorithm for negative edges(worst case complexity: O(EV))
				while (qh != qt)// Q becomes empty
				{
					int v = q[qt++];// pop current vertex
					qt %= n;//update tail index
					state[v] = 0; // make state: v as visited
					// if current outflow(of adjacent vertex) < Total outflow capacity(of adjacent vertex)
					// && cost_distance[adj. vertex] > cost_distance[curent vertex] + cost of edge 
					for (int i = 0; i < g[v].size(); i++) if (g[v][i].f < g[v][i].u && d[g[v][i].b] > d[v] + g[v][i].c)
					{
						int to = g[v][i].b;// to = adjacent vertex
						d[to] = d[v] + g[v][i].c;// setting cost_distance of adjacent vertex
						from[to] = v; // setting parent of adjacent vertex to v
						from_edge[to] = i;// setting which edge to to is selected
						if (state[to] == 1) continue;// to is already in Q. So just update distance
						if (!state[to])//re-insert vertex at tail....already processed vertex
						{
							qt--;
							if (qt == -1) qt = n - 1;
							q[qt] = to;
							state[to] = 1;
						} else
						{// new vertex
							state[to] = 1;
							q[qh++] = to;
							qh %= n;
						}
					}
				}
				if (d[t] == inf) break;
				int it = t;
				int addflow = inf;
				while (it != s)
				{
					// find minimum add flow in shortest path found
					addflow = min(addflow, g[from[it]][from_edge[it]].u - g[from[it]][from_edge[it]].f);
					it = from[it];
				}
				it = t;
				while (it != s)
				{
					// edge = from --> to
					g[from[it]][from_edge[it]].f += addflow;//add to current flow of from vertex of current edge
					// in following step by decrementing from current flow, residual capacity(u-f) in back edge is increased.
					g[it][g[from[it]][from_edge[it]].back].f -= addflow; // subtract from current flow in back edge of to vertex
					cost += g[from[it]][from_edge[it]].c * addflow;// add to total cost(min flow * cost_per_unit)
					it = from[it];
				}
				flow += addflow;
			}
			return cost;
		} 
};
int n, m, q;

int main()
{
	scanf("%d %d %d", &n, &m, &q);// number of 1st,2nd column vertices in bipartite graph, number of queries
	MCMF t(0, n+m+1, n + m+2);
	for (int i = 1; i <= n; i++) 
	{
		t.addEdge(0, i, 0, 1);
	}
	for (int i = 1; i <= m; i++) 
	{
		t.addEdge(n+i, n+m+1, 0, 1);
	}
	for (int i = 1; i <= q; i++)
	{
		int x, y, z;
		scanf("%d %d %d", &x, &y, &z);
		t.addEdge(x, y, -z, 1);
	}
	// maximum flow, return minimum cost => t.minCostMaxFlow
	printf("%d\n", -t.minCostMaxFlow());
	return 0;
}




