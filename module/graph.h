#ifndef LARRY76_NADGENLIB_MODULE_GRAPH_HEADER
#define LARRY76_NADGENLIB_MODULE_GRAPH_HEADER

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <vector>

#include "random.h"
#include "genlib.h"

namespace Generator
{
    using std::vector;
    using std::unordered_map;
    using std::string;
    class Graph{
        public:
            Graph(){}
            Graph(){
                
            }
            void add_edge(int from, int to,){
                ++tot;
                ver.push_back(to);
                next.push_back(head[from]);
                head[from] = next[tot];
            }
        private:
            vector<int>head;
            vector<int>next;
            vector<int>edge;
            vector<int>ver;
            struct EDGE{
                int from,to;
            }
            unordered_map<>
            int tot = 1;
    }
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