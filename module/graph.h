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
    using std::endl;

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
                id = new int[ verCount + 1 ];
                vertexCount = verCount;
                undirectedMapSwitch = undirectedMap;
                muiltiedgeCheckSwitch = muiltiedgeCheck;
                weightedMapSwitch = weightedMap;
                loopCheckSwitch = loopCheck;
                clear();
            }
            /**
             * @brief Graph 类的析构函数，用于防止 Graph 类引发内存泄漏
            */
            ~Graph(){
                delete[] id;
            }
            /**
             * @brief 用于清空图
             * @warning 注意，清空图并不意味着清空了其占用的内存空间。
            */
            void clear()
            {
                for(register int i = 0; i <= vertexCount; ++i)
                    id[i] = i;
                hashtable.clear();
                edgeContainer.clear();
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
            }
            /**
             * @brief 输出图
             * @param shuffleOutput 是否启用乱序节点输出
            */
            void output(bool shuffleOutput = true)
            {
                std::ofstream &cout = *rout;
                if(shuffleOutput)
                    shuffle();
                for(auto c : edgeContainer) {
                    if(weightedMapSwitch)
                        cout << id[c.from] << ' ' << id[c.to] << ' ' << c.weight << endl;
                    else
                        cout << id[c.from] << ' ' << id[c.to] << endl;
                }
            }
        protected:
            void shuffle()
            {
                Random rnd;
                for(int i = 1; i <= vertexCount; ++i) {
                    int oth = rnd.irand(1,vertexCount);
                    std::swap(id[i],id[oth]);
                }
            }
            inline void __add_edge(int from, int to, int weight)
            {
                edgeContainer.push_back((EDGE){from,to,weight});
            }
            inline void __add_edge(int from, int to)
            {
                edgeContainer.push_back((EDGE){from,to,1});
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
                if(from > to && undirectedMapSwitch)
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
            int *id;
            struct EDGE{
                int from, to, weight;
            };
            vector<EDGE> edgeContainer;
            map<pii,bool> hashtable;
    };
    /**
     * @brief 生成一颗无根树
    */
    class NoRootTree : public Graph{
        public:
            /**
             * @brief NoRootTree 的带边权构造函数
             * @param verCount 点数
             * @param vmin 边权最小值
             * @param vmax 边权最大值
             * @warning 注意，该构造函数与其另一个重载有本质不同
             * @warning 
            */
            NoRootTree(int verCount, int vmin, int vmax)
                :Graph(verCount, true, true, true, true)
            {
                vertexCount = verCount;
                weightedTreeSwitch = true;
                this->vmin = vmin;
                this->vmax = vmax;
                clear();
            }
            NoRootTree(int verCount)
                :Graph(verCount, true, false, true, true)
            {
                vertexCount = verCount;
                weightedTreeSwitch = false;
                clear();
            }
            void Generate(){
                clear();
                int mode = rnd.irand(1,120);
                _Generate(mode);
            }
        protected:
            void _Generate(int mode){
                if(mode<=500)
                    Daisy();
                else if(mode <= 100)
                    Chain();
                else if(mode <= 25)
                    ChainWithChain();
                else if(mode <= 40)
                    DaisyWithDaisy();
                else if(mode <= 60)
                    ChainWithDaisy();
                else if(mode <= 75)
                    CompleteKBTree();
                else if(mode <= 90)
                    RandomTree();
                else if(mode <= 105)
                    InCompleteKBTree();
                else
                    TreeOverTree();
            }
            void Daisy(){
                int center = 1;
                for(int i=2;i<=vertexCount;++i){
                    if(weightedTreeSwitch)
                        add_edge(center, i, rnd.irand(vmin,vmax));
                    else
                        add_edge(center, i);
                }
            }
            void Chain(){
                for(int i=1;i<vertexCount;++i){
                    if(weightedTreeSwitch)
                        add_edge(i,i+1,rnd.irand(vmin,vmax));
                    else
                        add_edge(i,i+1);
                }
            }
            void ChainWithDaisy(){
            }
            void ChainWithChain(){
            }
            void DaisyWithDaisy(){
            }
            void RandomTree(){
            }
            void CompleteKBTree(){
            }
            void InCompleteKBTree(){
            }
            void TreeOverTree(){
            }
            
        private:
            bool weightedTreeSwitch;
            int vertexCount;
            int tot = 0;
            int vmin, vmax;
            Random rnd;
    };
}

#endif