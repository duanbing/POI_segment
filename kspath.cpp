#include "util.h"
#include "kspath.h"
#include <algorithm>
//double kspath_t::astar_node_t::g_dst[MAX_WORD_PER_SENTENCE] = {0};

//void kspath_t::reset(){
//	for(int i=0;i<=nvertex;++i) m_graph[i].clear(),m_rgraph[i].clear();
//	memset(m_vst,0,sizeof(m_vst));
//}

kspath_t::kspath_t(int row,int col,edge_t edge[][MAX_WORD_PER_SENTENCE],int _topk){
	nvertex = col;
	src = 0;
	dest = col-1;
	for(int i=0;i<row;i++) for(int j=0;j<col;++j) {
		if(  ZERO(edge[i][j].cost - INF) < 0) {
			m_graph[i].push_back(vertex_t(j,edge[i][j].cost));
			//m_rgraph[j].push_back(vertex_t(i,edge[i][j].cost));
		}
	}
//	memset(m_vst,0,sizeof(m_vst));
	topk = _topk;
	m_nkpath = 0;
}
kspath_t::~kspath_t() {

}

void kspath_t::cal_all_path(int u,int p,int *x,int *ix,double *w) {
	
	if(u == dest) { //find a path
		m_kpath[m_nkpath]=vector<int>(x,x+(*ix));
		m_kw[m_nkpath] = *w;
		++m_nkpath;
		return ;
	}
	for(int i=0,sz=m_graph[u].size();i<sz;++i) {
		int v = m_graph[u][i].u;
		if(v != p) {
			x[(*ix)]=v;
			*w += m_graph[u][i].c;
			(*ix) += 1;
			cal_all_path(v,u,x,ix,w);
			(*ix)-=1;
			*w -= m_graph[u][i].c;
		}
	}
}

double kspath_t::m_kw[] = {0};

bool kspath_t::cmp(int i,int j){
	return ZERO(m_kw[i] - m_kw[j]) < 0;
}

void kspath_t::cal_kth_path(){
	int x[100],ix=0;
	x[ix++]=0;
	double w=0;
	cal_all_path(src,-1,x,&ix,&w);
	
	for(int i=0;i<m_nkpath;++i) idx[i] = i;

	sort(idx,idx+m_nkpath,cmp);

	if(topk < m_nkpath) m_nkpath = topk; 
}

//
//void kspath_t::spfa() {
//	
//	int s = dest;
//
//	for(int i=0;i<=nvertex;++i) astar_node_t::g_dst[i] = INF,m_vst[i]=false;
//	astar_node_t::g_dst[s] = 0;
//	queue<int>q;	
//
//	q.push(s);
//	while(!q.empty()) {
//		int u = q.front(); q.pop();
//		m_vst[u] = 0;
//		for(int i=0,size=m_rgraph[u].size();i<size;++i) {
//			vertex_t nd = m_rgraph[u][i];
//			int v = nd.u;
//			if(astar_node_t::g_dst[v] > astar_node_t::g_dst[u] + nd.c) {
//				astar_node_t::g_dst[v] = astar_node_t::g_dst[u] + nd.c;
//				if(!m_vst[v]){ m_vst[v]=true; q.push(v); }
//			}
//		} 
//	}
//}
//
//void kspath_t::log_path(int *path) {
//	int i=0;
//	while(-1!=path[dest]) {
//		//m_kpath[m_nkpath][i++]
//	}
//
//}
//
//double kspath_t::astar() {
//	int s = src;
//	int pathtmp[MAX_WORD_PER_SENTENCE];
//	double dst[MAX_WORD_PER_SENTENCE] ;
//	memset(pathtmp ,-1 ,sizeof(pathtmp));
//	fill(dst,dst+MAX_WORD_PER_SENTENCE,1e8);
//	priority_queue<astar_node_t> q;
//	if(astar_node_t::g_dst[s] == INF) return -1;
//	int cnt[MAX_WORD_PER_SENTENCE];
//	//printf("%d\n",INF);	
//	memset(cnt,0,sizeof(cnt));
//	q.push(astar_node_t(s,0));
//	dst[s] = 0;
//	while(!q.empty()) {
//		astar_node_t a= q.top(); q.pop();
//		int u = a.u;
//		double len = a.dis;
//		++ cnt[u];
//		if(u == dest) {  //¼ÇÂ¼Â·¾¶
//			log_path(pathtmp);
//			++m_nkpath;	
//		}
//		if(cnt[dest] == topk) 
//			return len;
//		for(int i=0,size=m_graph[u].size();i<size;++i) {
//			vertex_t tmp = m_graph[u][i];
//			int v = tmp.u;
//			if( ZERO(dst[v] - len - tmp.c) > 0) {
//				dst[v] = len + tmp.c;
//				pathtmp[v] = u;
//			} 
//			q.push(astar_node_t(v,len + tmp.c));
//		}
//	}
//
//	return -1;
//}