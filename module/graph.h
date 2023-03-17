/**
 * @file graph.h
 * @author Larry Green (larry0716@163.com)
 * @brief 这是 NaDGenLib 关于图论部分的头文件
 * @details 事实上，这并不是非常好的代码，我很抱歉
 * @version 0.0.1
 * @date 2023-03-15
 * @note 感谢洛谷团队的 CYaRon 项目带给我的启发
 * @copyright Copyright (c) 2023
 */
#ifndef LARRY76_NADGENLIB_MODULE_GRAPH_HEADER
#define LARRY76_NADGENLIB_MODULE_GRAPH_HEADER

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "random.h"
#include "genlib.h"

namespace Generator
{
    using std::vector;
    using std::map;
    using std::string;

    using std::endl;
    using std::flush;

    typedef std::pair<int,int> pii;

    /**
     * @brief 图的基类，用于存储图和实现图中的一些功能。
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
        Graph( int verCount,
               bool undirectedMap = false, bool weightedMap = false, 
               bool muiltiedgeCheck = false, bool loopCheck = false )
        {
            asserti(verCount, "Invaild vertex count.");
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
        ~Graph()
        {
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
        inline bool add_edge(int from, int to)
        {
            asserti(!weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
            if(!multiedgeChecker(from, to))
                return false;
            if(!loopChecker(from, to))
                return false;
            __add_edge(from, to);
            return true;
        }
        /**
         * @brief 添加一条带权边
         * @param from 边的起点
         * @param to 边的终点
         * @param weight 边的权
        */
        inline bool add_edge(int from, int to, int weight)
        {
            asserti(weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
            if(!multiedgeChecker(from, to))
                return false;
            if(!loopChecker(from, to))
                return false;
            __add_edge(from, to, weight);
            return true;
        }
        /**
         * @brief 输出图的接口
         * @param shuffleOutput 是否启用乱序节点输出
        */
        inline void Output(bool shuffleOutput = true)
        {
            _output(shuffleOutput);
        }
        /**
         * @brief 获取图的边数的接口
        */
        inline int GetEdgeCount()
        {
            return _GetEdgeCount();
        }
    protected:
        int *id;
        /**
         * @brief 获取边数的实现
        */
        inline int _GetEdgeCount()
        {
            return edgeContainer.size();
        }
        /**
         * @brief 输出图的实现
         * @param shuffleOutput 打乱点序输出，默认为启用（true）
        */
        void _output(bool shuffleOutput = true)
        {
            std::ofstream &cout = *rout;
            if(shuffleOutput)
                shuffle();
            for(auto c : edgeContainer) {
                if(weightedMapSwitch)
                    cout << id[c.from] << ' ' 
                         << id[c.to] << ' ' 
                         << c.weight << "\n";
                else
                    cout << id[c.from] << ' ' 
                         << id[c.to] << "\n";
            }
            cout<<flush;
        }
        /**
         * @brief 打乱点序
        */
        void shuffle()
        {
            Random rnd;
            for(int i = 1; i <= vertexCount; ++i) {
                int oth = rnd.irand(1, vertexCount);
                std::swap(id[i], id[oth]);
            }
            std::shuffle(edgeContainer.begin(), edgeContainer.end(), std::mt19937_64(time(0)));
        }
        /**
         * @brief 加一条带权边的实现
        */
        inline void __add_edge(int from, int to, int weight)
        {
            edgeContainer.push_back( (EDGE){from, to, weight} );
        }
        /**
         * @brief 加一条无权边的实现
        */
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
        /**
         * @brief 立即为 vector 分配内存
         * @param size 分配的大小
        */
        inline void alloc(size_t size)
        {
            edgeContainer.resize(size);
        }
    private:
        bool loopCheckSwitch;
        bool muiltiedgeCheckSwitch;
        bool undirectedMapSwitch;
        bool weightedMapSwitch;
        int vertexCount;
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
            :Graph(verCount, true, true, false, true)
        {
            vertexCount = verCount;
            weightedTreeSwitch = true;
            this->vmin = vmin;
            this->vmax = vmax;
            alloc(verCount);
            clear();
        }
        /**
         * @brief NoRootTree 的无边权构造函数
         * @param verCount 点数
         * @warning 注意，该构造函数与其另一个重载有本质不同
        */
        NoRootTree(int verCount)
            :Graph(verCount, true, false, false, true)
        {
            vertexCount = verCount;
            weightedTreeSwitch = false;
            alloc(verCount);
            clear();
        }
        /**
         * @brief 启用随机种类生成
        */
        inline void Generate()
        {
            clear();
            int mode = rnd.irand(1, 100);
            _Generate(mode);
        }
        /**
         * @brief 启用特定种类生成
        */
        inline void SpecificGenerate(NRT_GEN_CONF tgc)
        {
            clear();
            _Generate(tgc);
        }
        /**
         * @brief 输出生成结果
        */
        inline void Output(bool shuffleOutput = true)
        {
            _output(shuffleOutput);
        }
    protected:
        /**
         * @brief 加一条边（自动生成边权）
         * @param from 边的起点
         * @param to 边的终点
        */
        inline void add(int from, int to)
        {
            if(weightedTreeSwitch)
                add_edge(from, to, rnd.irand(vmin, vmax));
            else
                add_edge(from, to);
        }
        /**
         * @brief 生成方式选择子
        */
        inline void _Generate(int mode)
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
        /**
         * @brief 生成一朵菊花
         * @param center 中心节点编号
         * @param size 菊花大小
         * @param labelbegin 边缘点的起始编号
        */
        void Daisy(int center = 1, unsigned size = 0, int labelbegin = 2)
        {
            if(!size)
                size = vertexCount;
            for(int i = labelbegin; i < labelbegin+int(size); ++i)
                add(center, i);
        }
        /**
         * @brief 生成一条链
         * @param begin 链的起始节点
         * @param length 链的长度
         * @param labelbegin 链的第二节点编号
         * @warning 注意，labelbegin 与 begin 没有关系
        */
        void Chain(int begin = 1, unsigned length = 0, int labelbegin = 2)
        {
            if(!length)
                length = vertexCount;
            add(begin, labelbegin);
            for(int i = labelbegin; i < labelbegin + int(length) - 1; ++i) {
                add(i, i+1);
            }
        }
        /**
         * @brief 生成链套菊花
         * @param ChainProbability 生成链的概率
         * @param DaisyProbability 生成菊花的概率
         * @warning 注意，这两者的概率相加必须要等于 1
         * @warning 注意，链套菊花不等同于菊花链
        */
        void ChainWithDaisy( double ChainProbability=0.5, 
                             double DaisyProbability=0.5 )
        {
            asserti(ChainProbability + DaisyProbability == 1, "PANIC!!! Probability sum wasn't equal to 1.");
            int bsize = sqrt(vertexCount);
            int p = 0;
            double mode = 0;
            register int i;
            for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                mode = rnd.frand(0, 1);
                p = rnd.irand(1, i-1);
                if(mode <= ChainProbability)
                    Chain(p, bsize, i);
                else
                    Daisy(p, bsize, i);
            }
            if(i-1 < vertexCount) {
                mode = rnd.frand(0, 1);
                p = rnd.irand(1, i-1);
                if(mode <= ChainProbability)
                    Chain(p, vertexCount-i+1, i);
                else
                    Daisy(p, vertexCount-i+1, i);
            }
        }
        /**
         * @brief 生成链套链
        */
        void ChainWithChain()
        {
            int bsize = sqrt(vertexCount);
            int p = 0;
            register int i;
            for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                p = rnd.irand(1, i-1);
                Chain(p, bsize, i);
            }
            if(i-1 < vertexCount) {
                p = rnd.irand(1, i-1);
                Chain(p, vertexCount-i+1, i);
            }
        }
        /**
         * @brief 生成菊花套菊花
        */
        void DaisyWithDaisy()
        {
            int bsize = sqrt(vertexCount);
            int p = 0;
            register int i;
            for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
                p = rnd.irand(1, i-1);
                Daisy(p, bsize, i);
            }
            if(i-1 < vertexCount) {
                p = rnd.irand(1, i-1);
                Daisy(p, vertexCount-i+1, i);
            }
        }
        /**
         * @brief 生成一颗随机构造树
         * @param beginlabel 表示起始节点编号
         * @param endlabel 表示终止节点编号
        */
        void RandomTree(int beginlabel = 1, int endlabel = 0)
        {
            if(!endlabel)
                endlabel = vertexCount;
            int from;
            for(register int i = beginlabel+1; i <= endlabel; i++) {
                from = rnd.irand(beginlabel, i-1);
                add(from, i);
            }
        }
        /**
         * @brief 生成一颗完全 K 叉树
        */
        void CompleteKBTree()
        {
            int k = rnd.irand(2, 5);
            for(register int i = 2; i <= vertexCount; ++i) {
                if(weightedTreeSwitch)
                    add_edge(i, ((k - 2) + i) / k, rnd.irand(vmin, vmax));
                else
                    add_edge(i, ((k - 2) + i) / k);
            }
        }
        /**
         * @brief 生成随机构造树套随机构造树
        */
        void TreeOverTree()
        {
            int bsize = sqrt(vertexCount);
            register int i;
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

    /**
     * @brief 生成菊花链
    */
    class DaisyChain : protected Graph{
    public:
        /**
         * @brief 无权 DaisyChain 的构造函数
         * @param verCount 菊花链的节点数
        */
        DaisyChain(int verCount)
            :Graph(verCount, false, false, true, true)
        {
            vertexCount = verCount;
            weightedDaisyMapSwitch = false;
        }
        /**
         * @brief 带权 DaisyChain 的构造函数
         * @param vmin 边权最小值
         * @param vmax 边权最大值
        */
        DaisyChain(int verCount, int vmin, int vmax)
            :Graph(verCount, false, true, true, true)
        {
            vertexCount = verCount;
            weightedDaisyMapSwitch = true;
            vmax = vmax;
            vmin = vmin;
        }
        /**
         * @brief 启动生成
        */
        void Generate(){
            clear();
            for(int i = 2; i <= vertexCount ;++i)
                add(central, i);

            for(int i = vertexCount; i >= 2; --i)
                add(i, i-1);
        }
        /**
         * @brief 输出生成结果
        */
        inline void Output(bool shuffleOutput = true){
            _output(shuffleOutput);
        }
    protected:
        /**
         * @brief 加一条边（自动生成边权）
         * @param from 边的起点
         * @param to 边的终点
        */
        inline void add(int from, int to)
        {
            if(weightedDaisyMapSwitch)
                add_edge(from, to, rnd.irand(vmin, vmax));
            else
                add_edge(from, to);
        }
    private:
        bool weightedDaisyMapSwitch;
        int vertexCount;
        int vmin, vmax;
        int central = 1;
        Random rnd;
    };

    /**
     * @brief 生成强对抗 SPFA 图（网格图）
    */
    class AntiSPFA : protected Graph{
    public:
        /**
         * @brief AntiSPFA 构造函数
         * @param n 网格图 n 行
         * @param m 网格图 m 列
        */
        AntiSPFA(int n, int m)
            :Graph(n*m, false, true, true, true)
        {
            this->n = n;
            this->m = m;
            this->vmin = vmin;
            this->vmax = vmax;
        }
        /**
         * @brief 获取起点
        */
        inline int GetStart()
        {
            return id[1];
        }
        /**
         * @brief 获取边数
        */
        inline int GetEdgeCount()
        {
            return _GetEdgeCount();
        }
        /**
         * @brief 启动生成
         * @param vmin 边权最小值
         * @param vmax 边权最大值
        */
        void Generate(int vmin, int vmax)
        {
            clear();
            shuffle();
            for(int i = 1; i <= n; ++i){
                for(int j = 1; j < m; ++j){
                    add_edge(getIndex(i, j), getIndex(i, j+1), rnd.irand(vmin, vmax));
                    add_edge(getIndex(i, j+1), getIndex(i, j), rnd.irand(vmin, vmax));
                }
            }
            for(int i = 1; i <= m; ++i){
                for(int j = 1; j < n; ++j){
                    add_edge(getIndex(j, i), getIndex(j+1, i), 1);
                    add_edge(getIndex(j+1, i), getIndex(j, i), 1);
                }
            }
            for(int i = 1; i < n; ++i)
                for(int j = 1; j < m; ++j)
                    add_edge(getIndex(i, j), getIndex(i+1, j+1), rnd.irand(vmin, vmax));
        }
        /**
         * @brief 启用自动生成模式
         * @param vmin 边权最小值
         * @param vmax 边权最大值
        */
        inline void AutoMode(int vmin, int vmax)
        {
            std::ofstream &cout = *rout;
            Generate(vmin, vmax);
            cout << n*m << ' ' << GetEdgeCount() << ' ' << GetStart() << endl;
            Output();
        }
        /**
         * @brief 输出生成结果
        */
        inline void Output()
        {
            _output(false);
        }
    protected:
        /**
         * @brief 获取二维网格图节点在一维数组中的实际存储位置\
         * @param i 第 i 行
         * @param j 第 j 列
        */
        inline int getIndex(int i, int j)
        {
            return (i-1)*m+j;
        }
    private:
        int n,m;
        int vmin, vmax;
        Random rnd;
    };

    /**
     * @brief 生成随机图
     * @note 允许图不连通，但不允许自环和重边
    */
    class RandomGraph : protected Graph{
    public:
        /**
         * @brief 随机无权图的构造函数
         * @param verCount 随机图点数
         * @param edgeCount 随机图边数
         * @param undirectedMap 无向图开关，默认为有向图（false）
         * @warning 该生成器并非高性能生成器，当边数较大时可能会出现严重的性能下降
        */
        RandomGraph(int verCount, int edgeCount, bool undirectedMap = false)
            :Graph(verCount, undirectedMap, false, true, true), 
             vertexCount(verCount), edgeCount(edgeCount),
             weightedMapSwitch(false)
        { }
        /**
         * @brief 随机带权图的构造函数
         * @param verCount 随机图点数
         * @param edgeCoucnt 随机图边数
         * @param vmin 边权最小值
         * @param vmax 边权最大值
         * @param undirectedMap 无向图开关，默认为有向图（false）
         * @warning 该生成器并非高性能生成器，当边数较大时可能会出现严重的性能下降
        */
        RandomGraph( int verCount, int edgeCount, int vmin, int vmax, 
                     bool undirectedMap = false)
            :Graph(verCount, undirectedMap, true, true, true),
             vertexCount(verCount), edgeCount(edgeCount),
             vmin(vmin), vmax(vmax), 
             weightedMapSwitch(true)
        { }
        /**
         * @brief 启用生成
        */
        void Generate()
        {
            while(edgeCount){
                bool u = rnd.irand(1, vertexCount);
                bool v = rnd.irand(1, vertexCount);
                edgeCount -= add(u, v);
            }
        }
        /**
         * @brief 输出生成结果
         * @param shuffleOutput 打乱节点输出，默认为打乱（true）
        */
        inline void Output(bool shuffleOutput = true)
        {
            _output(shuffleOutput);
        }
    protected:
        /**
         * @brief 加一条边（自动生成边权）
         * @param from 边的起点
         * @param to 边的终点
        */
        inline bool add(int from, int to)
        {
            if(weightedMapSwitch)
                return add_edge(from, to, rnd.irand(vmin, vmax));
            else
                return add_edge(from, to);
        }
    private:
        Random rnd;
        int vertexCount;
        int edgeCount;
        int vmin, vmax;
        bool weightedMapSwitch;
    };

    /**
     * @brief 生成一张有向无环图。
    */
    class DAG : protected Graph{
    public:
        /**
         * @brief 无权有向无环图的构造函数
         * @param verCount 有向无环图的点数
         * @param edgeCount 有向无环图的边数
        */
        DAG(int verCount, int edgeCount)
            :Graph(verCount, false, false, true, true), 
             vertexCount(verCount), edgeCount(edgeCount), 
             weightedMapSwitch(false)
        {
            asserti(edgeCount >= verCount - 1, "Invalid edge count.");
        }
        /**
         * @brief 带权有向无环图的构造函数
         * @param verCount 有向无环图的点数
         * @param edgeCount 有向无环图的边数
         * @param vmin 边权最小值
         * @param vmax 边权最大值
        */
        DAG(int verCount, int edgeCount, int vmin, int vmax)
            :Graph(verCount, false, true, true, true),
             vertexCount(verCount), edgeCount(edgeCount), 
             vmin(vmin), vmax(vmax),
             weightedMapSwitch(true)
        {
            asserti(edgeCount >= verCount - 1, "Invalid edge count.");
        }
        /**
         * @brief 启用生成
        */
        void Generate()
        {
            for(int i = 2; i <= vertexCount; i++) {
                int from = rnd.irand(1, i-1);
                add(from, i);
            }
            edgeCount -= vertexCount - 1;
            if(!edgeCount)
                return ;
            for(int i = 1; i <= vertexCount; ++i) {
                for(int j = i+1; j <= vertexCount; ++j) {
                    if(!edgeCount)
                        return;
                    edgeCount -= add(i, j);
                }
            }
        }
        /**
         * @brief 输出生成结果
         * @param shuffleOutput 打乱节点输出，默认为打乱（true）
        */
        inline void Output(bool shuffleOutput = true)
        {
            _output(shuffleOutput);
        }
    protected:
        /**
         * @brief 加一条边（自动生成边权）
         * @param from 边的起点
         * @param to 边的终点
        */
        bool add(int from, int to)
        {
            if(weightedMapSwitch)
                return add_edge(from, to, rnd.irand(vmin, vmax));
            else
                return add_edge(from, to);
        }
    private:
        int vertexCount;
        int edgeCount;
        int vmin, vmax;
        bool weightedMapSwitch;
        Random rnd;
    };
    /**
     * @brief 生成一棵仙人掌
     * @note 因为本人尚菜，所以该生成器只能指定点数了/kk
     * @warning 请开发组注意，该方法可能会产生 BUG
    */
    class Cactus{
    public:
        /**
         * @brief 无权 Cactus 的构造函数
         * @param verCount 节点数
        */
        Cactus(int verCount)
        {
            vertexCount = verCount;
            weightedMapSwitch = false;
            alloc(verCount);
            clear();         
        }
        /**
         * @brief 带权 Cactus 的构造函数
         * @param verCount 节点数
         * @param vmin 边权最小值
         * @param vmax 边权最大值
        */
        Cactus(int verCount, int vmin, int vmax)
            :vmin(vmin), vmax(vmax)
        {   
            vertexCount = verCount;
            weightedMapSwitch = true;
            alloc(verCount+1);
            clear();
        }
        /**
         * @brief Cactus 的析构函数
        */
        ~Cactus(){
            delete[] head;
            delete[] ver;
            delete[] next;
            delete[] edge;
            delete[] top;
            delete[] siz;
            delete[] fa;
            delete[] hson;
            delete[] id;
        }
        /**
         * @brief 清空 Cactus
        */
        void clear(){
            tot = 0;
            memset(top,-1,sizeof(int)*(vertexCount<<2));
            memset(next, 0, sizeof(int)*(vertexCount<<2));
            memset(head, 0, sizeof(int)*vertexCount);
            memset(ver, 0, sizeof(int)*(vertexCount<<2));
            memset(edge, 0, sizeof(int)*(vertexCount<<2));
            memset(siz, 0, sizeof(int)*(vertexCount<<2));
            memset(fa, 0, sizeof(int)*(vertexCount<<2));
            memset(hson, 0, sizeof(int)*(vertexCount<<2));
            for(int i = 1; i <= vertexCount; ++i)
                id[i] = i;
        }
        /**
         * @brief 启用生成
        */
        void Generate(){
            for(int i = 2; i <= vertexCount; i++) {
                int from = rnd.irand(1, i-1);
                add(from, i);
            }
            TreeDecomposition();
            for(int i = 1; i <= vertexCount; ++i){
                if(top[i] != -1)
                    add(i, top[i]);
            }
        }
        /**
         * @brief 输出生成结果
         * @param shuffleOutput 打乱节点输出，默认为打乱（true）
        */
        void Output(bool shuffleOutput = true)
        {
            _output(shuffleOutput);
        }
    protected:
        void alloc(int verCount){
            head = new int[verCount];
            next = new int[verCount<<2];
            ver = new int[verCount<<2];
            edge = new int[verCount<<2];
            top = new int[verCount<<2];
            siz = new int[verCount<<2];
            fa = new int[verCount<<2];
            hson = new int[verCount<<2];
            id = new int[verCount];
        }
        /**
         * @brief 树链剖分
        */
        void TreeDecomposition()
        {
            dfs1(1, 1);
            dfs2(1, 1);
        }
        /**
         * @brief 树链剖分预处理
         * @param u 当前节点
         * @param f 当前节点的父节点
        */
        void dfs1(int u, int f)
        {
            siz[u] = 1;
            fa[u] = f;
            int maxsize = -1;
            for(int i = head[u]; i; i = next[i]){
                int v = ver[i];
                if(v == f)
                    continue;
                dfs1(v, u);
                siz[u] += siz[v];
                if(maxsize < siz[v]){
                    hson[u] = v;
                    maxsize = siz[v];
                }
            }
        }
        /**
         * @brief 树链剖分
         * @param u 当前节点
         * @param t 链顶
        */
        void dfs2(int u, int t)
        {
            if(!hson[u]) {
                top[u] = t;
                return;
            }
            dfs2(hson[u], t);
            for(int i = head[u]; i; i = next[i]) {
                int v = ver[i];
                if(v != hson[u] && v != fa[u])
                    dfs2(v, v);
            }
        }
        /**
         * @brief 加一条边（自动生成边权）
         * @param from 边的起点
         * @param to 边的终点
        */
        void add(int from, int to)
        {
            if(weightedMapSwitch)
                add_edge(from, to, rnd.irand(vmin, vmax));
            else
                add_edge(from, to);
        }
        /**
         * @brief 添加一条无向无权边
         * @param from 边的起点
         * @param to 边的终点
        */
        void add_edge(int from, int to)
        {
            _add_edge(from, to);
            _add_edge(to, from);
        }
        /**
         * @brief 添加一条无向带权边
         * @param from 边的起点
         * @param to 边的终点
         * @param weight 边权
        */
        void add_edge(int from, int to, int weight)
        {
            _add_edge(from, to, weight);
            _add_edge(to, from, weight);
        }
        /**
         * @brief 添加一条有向无权边
         * @param from 边的起点
         * @param to 边的终点
        */
        void _add_edge(int from, int to)
        {
            ver[++tot] = to;
            next[tot] = head[from];
            head[from] = tot;
        }
        /**
         * @brief 添加一条有向带权边
         * @param from 边的起点
         * @param to 边的终点
         * @param weight 边权
        */
        void _add_edge(int from, int to, int weight)
        {
            ver[++tot] = to;
            edge[tot] = weight;
            next[tot] = head[from];
            head[from] = tot;
        }
        /**
         * @brief 打乱节点编号
        */
        void shuffle()
        {
            Random rnd;
            for(int i = 1; i <= vertexCount; ++i) {
                int oth = rnd.irand(1, vertexCount);
                std::swap(id[i], id[oth]);
            }
        }
        /**
         * @brief 输出图的实现
        */
        void _output(bool shuffleOutput = true)
        {
            std::ofstream &cout = *rout;
            if(shuffleOutput)
                shuffle();
            for(int i = 1; i <= vertexCount; ++i) {
                for(int j = head[i]; j; j = next[j]) {
                    int v = ver[j];
                    if(hashtable[{ std::min(i, v), std::max(i, v) }])
                        continue;
                    cout << id[v] << ' ' << id[i];
                    if(weightedMapSwitch)
                        cout << ' ' << edge[i];
                    cout << '\n';
                    hashtable[{ std::min(i, v), std::max(i, v) }] = true;
                }
            }
            cout<<flush;
        }
    private:
        bool weightedMapSwitch;
        int vertexCount;
        int *head, *next, *ver, *edge;
        int *siz, *hson, *top, *fa;
        int *id;
        int vmin, vmax;
        int tot = 0;
        Random rnd;
        map<pii,bool> hashtable;
    };
}
#endif