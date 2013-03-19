#ifndef K_S_PATH_H
#define  K_S_PATH_H

#include "util.h"
#include <queue>
using namespace std;


struct vertex_t {
		int u;
		double c;
		vertex_t(){}
		vertex_t(int _u,double _c):u(_u),c(_c){}
};





class kspath_t {
public:
	

	vector<int> m_kpath[TOP_K];
	static double m_kw[TOP_K];
	int m_nkpath;
	
	//bool m_vst[MAX_WORD_PER_SENTENCE];
	vector<vertex_t>m_graph[MAX_WORD_PER_SENTENCE];
	//vector<vertex_t>m_rgraph[MAX_WORD_PER_SENTENCE];
	int nvertex;
	int src,dest,topk;
	int idx[100]; //路径的权值排名
	/*struct astar_node_t{
		int u;
		double dis;

		static double g_dst[MAX_WORD_PER_SENTENCE];
		bool operator<(const astar_node_t& node) const {
			return  ZERO(dis + g_dst[u] - node.dis - g_dst[node.u])>0;
		}
		
		astar_node_t(){}
		astar_node_t(int _u,double _d):u(_u),dis(_d) {}
	};*/

	void cal_kth_path();

	kspath_t(int row,int col,edge_t m_graph[][MAX_WORD_PER_SENTENCE],int );
	~kspath_t();

	/*void spfa();
	double astar();
	void reset();
	void log_path(int*);*/
	
private:
	void cal_all_path(int u,int p,int *x,int* ,double*);
	static bool cmp(int ,int);
};

#endif
