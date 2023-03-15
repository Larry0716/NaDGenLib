#ifndef LARRY76_NADGENLIB_MODULE_GRAPH_HEADER
#define LARRY76_NADGENLIB_MODULE_GRAPH_HEADER

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>
#include <map>

#include "random.h"
#include "genlib.h"

namespace Generator
{
    using std::vector;
    using std::map;
    using std::string;

    typedef std::pair<int,int> pii;
    /**
     * @brief 图的基类，用于存储图。
    */
    class Graph{
        public:
            /**
             * @brief 图的构造函数
             * @param verCount 图的点数
             * @param undirectedMap 无向图开关，默认为有向图（false）
             * @param weightedMap 带权图开关，默认为不带权（false）
             * @param muiltiedgeCheck 重边检查开关，默认为关闭（false）
             * @param loopCheck 自环检查开关，默认为关闭（false）
            */
            Graph(int verCount,
                  bool undirectedMap = false, bool weightedMap = false, 
                  bool muiltiedgeCheck = false, bool loopCheck = false)
            {
                ++verCount;
                head = new int[verCount];
                id = new int[verCount];
                --verCount;
                vertexCount = verCount;
                undirectedMapSwitch = undirectedMap;
                muiltiedgeCheckSwitch = muiltiedgeCheck;
                weightedMapSwitch = weightedMap;
                loopCheckSwitch = loopCheck;
                clear();
            }
            /**
             * @brief 用于清空图
             * @warning 注意，清空图并不意味着清空了其占用的内存空间。
            */
            void clear()
            {
                for(register int i = 0; i < vertexCount; ++i)
                    id[i] = i;
                memset( head, -1, vertexCount * sizeof(int) );
                ver.clear();
                next.clear();
                edge.clear();
            }
            /**
             * @brief 添加一条边
             * @param from 边的起点
             * @param to 边的终点
            */
            void add_edge(int from, int to)
            {
                asserti(!weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
                if(!multiedgeChecker(from, to))
                    return ;
                if(!loopChecker(from, to))
                    return ;
                __add_edge(from, to);
                if(undirectedMapSwitch)
                    __add_edge(to, from);
            }
            /**
             * @brief 添加一条带权边
             * @param from 边的起点
             * @param to 边的终点
             * @param weight 边的权
            */
            void add_edge(int from, int to, int weight)
            {
                asserti(weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
                if(!multiedgeChecker(from, to))
                    return ;
                if(!loopChecker(from, to))
                    return ;
                __add_edge(from, to, weight);
                if(undirectedMapSwitch)
                    __add_edge(to, from, weight);
            }
            void output(bool shuffleOutput = true){
                std::ofstream &cout = *rout;
                if(shuffleOutput)
                    shuffle();
                for(int i = 0; i <= vertexCount; ++i){
                    for(int j = head[i]; ~j; j = next[j]){
                        cout << id[i] << ' ' << id[ver[j]];
                        if(weightedMapSwitch)
                            cout << ' ' << edge[j];
                        cout << std::endl;
                    }
                }
            }
        protected:
            void shuffle(){
                Random rnd;
                for(int i = 1; i <= vertexCount; ++i){
                    int oth = rnd.irand(1,n);
                    std::swap(id[i],id[oth]);
                }
            }
            inline void __add_edge(int from, int to, int weight)
            {
                ++tot;
                ver.push_back(to);
                edge.push_back(weight);
                next.push_back(head[from]);
                head[from] = next[tot];
            }
            inline void __add_edge(int from, int to)
            {
                ++tot;
                ver.push_back(to);
                next.push_back(head[from]);
                head[from] = next[tot];
            }
            /**
             * @brief 重边检查器
             * @return true 则为检查通过，false 则为检查失败
             * @note 若检查失败，则会输出日志
            */
            inline bool multiedgeChecker(int from, int to)
            {
                if(!muiltiedgeCheckSwitch)
                    return true;
                if(from>to)
                    std::swap(from,to);
                if(hashtable[{from, to}]) {
                    fprintf(stderr, "Edge <%d,%d> was unable to pass muiltiedge check. Ignored.\n", from, to);
                    return false;
                }
                hashtable[{from, to}] = true;
                return true;
            }
            inline bool loopChecker(int from, int to)
            {
                if(!loopCheckSwitch)
                    return true;
                if(from == to){
                    fprintf(stderr, "Edge <%d,%d> was unable to pass loop check. Ignored.\n", from, to);
                    return false;
                }
                return true;
            }
        private:
            bool loopCheckSwitch;
            bool muiltiedgeCheckSwitch;
            bool undirectedMapSwitch;
            bool weightedMapSwitch;
            int vertexCount;
            int tot = 0;
            int *id;
            int *head;
            vector<int>next;
            vector<int>edge;
            vector<int>ver;
            map<pii,bool> hashtable;
    };
    /**
     * @brief 生成一颗无根树
    */
    class NoRootTree{
        public:
            NoRootTree(int n){
                ver = new int[n+1];
                this->n = n;
                for(int i=1;i<=n;i++)
                    ver[i] = i;
            }
            void Output(){
                shuffle();
                std::ofstream &fout = *(rout);
                for(int i=2;i<n;i++){
                    int fa = rnd.irand(1,i-1);
                    fout<<ver[i]<<' '<<ver[fa]<<'\n';
                }
            }
            void Output(int wmin,int wmax){
                shuffle();
                std::ofstream &fout = *(rout);
                for(int i=2;i<n-1;i++){
                    int fa = rnd.irand(1,i-1);
                    fout<<ver[i]<<' '<<ver[fa]<<' '<<rnd.irand(wmin,wmax)<<'\n';
                }
            }
        protected:
            void shuffle(){
                for(int i=1;i<=n;i++){
                    int oth = rnd.irand(1,n);
                    swap(ver[i],ver[oth]);
                }
            }
            inline void swap(int &x,int &y){
                int buf = x;
                x = y;
                y = buf;
            }
        private:
            Random rnd;
            int *ver;
            int n;
    };
}

#endif