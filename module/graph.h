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
                _output(shuffleOutput);
            }
        protected:
            void _output(bool shuffleOutput = true){
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
            void shuffle()
            {
                Random rnd;
                for(int i = 1; i <= vertexCount; ++i) {
                    int oth = rnd.irand(1, vertexCount);
                    std::swap(id[i], id[oth]);
                }
            }
            inline void __add_edge(int from, int to, int weight)
            {
                edgeContainer.push_back( (EDGE){from, to, weight} );
            }
            inline void __add_edge(int from, int to)
            {
                edgeContainer.push_back( (EDGE){from, to, 1} );
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
            /**
             * @brief 自环检查器
             * @return true 则为检查通过，false 则为检查失败
             * @note 若检查失败，则会输出日志
            */
            inline bool loopChecker(int from, int to)
            {
                if(!loopCheckSwitch)
                    return true;
                if(from == to) {
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
            struct EDGE {
                int from, to, weight;
            };
            vector<EDGE> edgeContainer;
            map<pii,bool> hashtable;
    };
    /**
     * @brief 无根树生成方式
    */
    enum NRT_GEN_CONF{
        METHOD_DAISY=5, //生成一朵菊花
        METHOD_CHAIN=10, //生成一条链
        METHOD_CHAIN_WITH_CHAIN=25, //生成链套链
        METHOD_DAISY_WITH_DAISY=40, //生成菊花套菊花
        METHOD_CHAIN_WITH_DAISY=60, //生成链套菊花
        METHOD_COMP_KB_TREE=75, //生成完全K叉树
        METHOD_RND_TREE=90, //生成随机树
        METHOD_TREE_OVER_TREE=100 //生成随机树套随机树
    };
    /**
     * @brief 生成一颗无根树
    */
    class NoRootTree : protected Graph{
        public:
            /**
             * @brief NoRootTree 的带边权构造函数
             * @param verCount 点数
             * @param vmin 边权最小值
             * @param vmax 边权最大值
             * @warning 注意，该构造函数与其另一个重载有本质不同
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
            /**
             * @brief NoRootTree 的无边权构造函数
             * @param verCount 点数
             * @warning 注意，该构造函数与其另一个重载有本质不同
            */
            NoRootTree(int verCount)
                :Graph(verCount, true, false, true, true)
            {
                vertexCount = verCount;
                weightedTreeSwitch = false;
                clear();
            }
            /**
             * @brief 启用随机种类生成
            */
            void Generate()
            {
                clear();
                int mode = rnd.irand(1, 100);
                _Generate(mode);
            }
            /**
             * @brief 启用特定种类生成
            */
            void SpecificGenerate(NRT_GEN_CONF tgc)
            {
                clear();
                _Generate(tgc);
            }
            /**
             * @brief 输出生成结果
            */
            void output(bool shuffleOutput = true)
            {
                this->_output(shuffleOutput);
            }
        protected:
            /**
             * @brief 生成方式选择子
            */
            void _Generate(int mode)
            {
                if(mode<=5)
                    Daisy();
                else if(mode <= 10)
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
                else
                    TreeOverTree();
            }
            void Daisy(int center = 1, unsigned size = 0, int labelbegin = 1)
            {
                if(!size)
                    size = vertexCount;
                for(int i = labelbegin; i < labelbegin+int(size); ++i) {
                    if(weightedTreeSwitch)
                        add_edge(center, i, rnd.irand(vmin, vmax));
                    else
                        add_edge(center, i);
                }
            }
            void Chain(int begin = 1, unsigned length = 0, int labelbegin = 1)
            {
                if(!length)
                    length = vertexCount;
                add_edge(begin, labelbegin);
                for(int i = labelbegin; i < labelbegin + int(length) - 1; ++i) {
                    if(weightedTreeSwitch)
                        add_edge(i, i+1, rnd.irand(vmin, vmax));
                    else
                        add_edge(i, i+1);
                }
            }
            void ChainWithDaisy(double chainProbabilty=0.5, double DaisyProbabilty=0.5)
            {
                asserti(chainProbabilty + DaisyProbabilty == 1, "PANIC!!! Probabilty sum was equal to 1.");
                int bsize = sqrt(vertexCount);
                int i = 1;
                for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                    double mode = rnd.frand(0, 1);
                    int p = rnd.irand(1, i-1);
                    if(mode <= chainProbabilty)
                        Chain(p, bsize, i);
                    else
                        Daisy(p, bsize, i);
                }
                if(i-1 < vertexCount) {
                    double mode = rnd.frand(0, 1);
                    int p = rnd.irand(1, i-1);
                    if(mode <= chainProbabilty)
                        Chain(p, vertexCount-i+1, i);
                    else
                        Daisy(p, vertexCount-i+1, i);
                }
            }
            void ChainWithChain()
            {
                int bsize = sqrt(vertexCount);
                int i = 1;
                for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                    int p = rnd.irand(1, i-1);
                    Chain(p, bsize, i);
                }
                
                if(i-1 < vertexCount) {
                    int p = rnd.irand(1, i-1);
                    Chain(p, vertexCount-i+1, i);
                }
            }
            void DaisyWithDaisy()
            {
                int bsize = sqrt(vertexCount);
                int i = 0;
                for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                    int p = rnd.irand(1, i-1);
                    Daisy(p, bsize, i);
                }
                if(i-1 < vertexCount) {
                    int p = rnd.irand(1, i-1);
                    Daisy(p, vertexCount-i+1, i);
                }
            }
            void RandomTree(int beginlabel = 1, int endlabel = 0)
            {
                if(!endlabel)
                    endlabel = vertexCount;
                for(int i = beginlabel+1; i <= endlabel; i++) {
                    int from = rnd.irand(beginlabel, i-1);
                    if(weightedTreeSwitch)
                        add_edge(from, i, rnd.irand(vmin, vmax));
                    else
                        add_edge(from, i);
                }
            }
            void CompleteKBTree()
            {
                int k = rnd.irand(2, 5);
                for(int i = 2; i <= vertexCount; ++i) {
                    if(weightedTreeSwitch)
                        add_edge(i, ((k - 2) + i) / k, rnd.irand(vmin, vmax));
                    else
                        add_edge(i, ((k - 2) + i) / k);
                }
            }
            void TreeOverTree()
            {
                int bsize = sqrt(vertexCount);
                int i = 0;
                for(i = 1; i+bsize <= vertexCount; i += bsize)
                    RandomTree(i, i+bsize);
                if(i < vertexCount)
                    RandomTree(i);
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