#ifndef LARRY76_NADGENLIB_GENLIB_HEADER
#define LARRY76_NADGENLIB_GENLIB_HEADER

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>

#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "module/genlib.h"
#include "module/random.h"
#include "module/sequence.h"

namespace Generator{
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

    class DaisyMap{
        public:
            DaisyMap(int perimeter,int dep){
                this->realsize = perimeter*dep + 2;
                ver = new int[this->realsize];
                this->dep = dep;
                this->perimeter = perimeter;
                for(int i=1;i<this->realsize;i++)
                    ver[i] = i;
            }
            int GetVertexNum(){
                return this->realsize-1;
            }
            void Output(){
                shuffle();
                std::ofstream &fout = *(rout);
                for(int i=0;i<dep-1;i++){
                    for(int j=2;j<=perimeter+1;j++){
                        fout << ver[i*perimeter+j] << ' ' << ver[(i+1)*perimeter+j] << '\n';
                    }
                }
                for(int i=2;i<=perimeter+1;i++){
                    fout<<ver[1]<<' '<<ver[i]<<'\n';
                }
            }
            void Output(int wmin,int wmax){
                shuffle();
                std::ofstream &fout = *(rout);
                for(int i=0;i<dep-1;i++)
                    for(int j=2;j<=perimeter+1;j++)
                        fout << ver[i*perimeter+j] << ' ' << ver[(i+1)*perimeter+j] << ' ' << rnd.irand(wmin,wmax)<< '\n';
                
                for(int i=2;i<=perimeter+1;i++)
                    fout<<ver[1]<<' '<<ver[i]<<' '<<rnd.irand(wmin,wmax)<<'\n';
            }
        protected:
            void shuffle(){
                for(int i=1;i<realsize;i++){
                    int oth = rnd.irand(1,realsize-1);
                    swap(ver[i],ver[oth]);
                }
            }
            inline void swap(int &x,int &y){
                int buf = x;
                x = y;
                y = buf;
            }
        private:
            int *ver;
            int realsize,perimeter,dep;
            Random rnd;
    };

    class DaisyChain{
        public:
            DaisyChain(int n){
                this->n = n;
                this->central = 1;
                ver = new int[n+1];
                for(int i=1;i<=n;i++)
                    ver[i] = i;
                shuffle();
            }
            DaisyChain(int n,int central){
                this->n = n;
                this->central = central;
                ver = new int[n+1];
                for(int i=1;i<=n;i++)
                    ver[i] = i;
                shuffle();
            }

            inline int GetStart(){
                return ver[central];
            }

            void Output(){
                std::ofstream &fout = *(rout);

                for(int i=2;i<=n;i++)
                    fout<< ver[central] <<' '<< ver[i] <<'\n';

                for(int i=n;i>=2;--i)
                    fout<< ver[i] <<' '<< ver[i-1] << '\n';
            }

            void Output(int wmin, int wmax){
                std::ofstream &fout = *(rout);

                for(int i=2;i<=n;i++)
                    fout<< ver[central] <<' '<< ver[i] << rnd.irand(wmin,wmax) <<'\n';

                for(int i=n;i>=1;--i)
                    fout<< ver[i-1] <<' '<< ver[i] << rnd.irand(wmin,wmax) << '\n';
            }
        protected:
            void shuffle(){
                for(int i=1;i<=n;i++){
                    int oth = rnd.irand(1,n);
                    swap(ver[i],ver[oth]);
                }
            }

            inline void swap(int &x,int &y){
                int temp = x;
                x = y;
                y = temp;
            }
        private:
            int *ver;
            int n,central;
            Random rnd;
    };

    class AntiSPFA{
        public:
            AntiSPFA(int n,int m){
                this->realsize = n*m+1;
                this->n = n;
                this->m = m;
                ver = new int*[n+1];
                for(int i=0;i<=n;i++)
                    ver[i] = new int[m+1];
                for(int i=1;i<=n;i++){
                    for(int j=1;j<=m;j++){
                        ver[i][j] = (i-1)*m+j;
                    }
                }
                shuffle();
            }

            inline int GetStart(){
                return ver[1][1];
            }

            inline void ReShuffle(){
                shuffle();
            }

            inline int GetEdgeCount(){
                return n*(m-1)*2+(n-1)*m*2+m-1;
            }

            inline int GetVertexCount(){
                return n*m;
            }

            void Output(){
                std::ofstream &fout = *(rout);
                for(int i=1;i<=n;i++)
                    for(int j=1;j<m;j++){
                        fout<<ver[i][j]<<' '<<ver[i][j+1]<<'\n';
                        fout<<ver[i][j+1]<<' '<<ver[i][j]<<'\n';
                    }
                
                for(int i=1;i<=m;i++)
                    for(int j=1;j<n;j++){
                        fout<<ver[j][i]<<' '<<ver[j+1][i]<<'\n';
                        fout<<ver[j+1][i]<<' '<<ver[j][i]<<'\n';
                    }

                for(int i=1;i<n;i++)
                    for(int j=1;j<m;j++)
                        fout<<ver[i][j]<<' '<<ver[i+1][j+1]<<'\n';
            }

            void Output(int wmin,int wmax){
                std::ofstream &fout = *(rout);
                for(int i=1;i<=n;i++)
                    for(int j=1;j<m;j++){
                        fout<<ver[i][j]<<' '<<ver[i][j+1]<<' '<<rnd.irand(wmin,wmax)<<'\n';
                        fout<<ver[i][j+1]<<' '<<ver[i][j]<<' '<<rnd.irand(wmin,wmax)<<'\n';
                    }
                
                for(int i=1;i<=m;i++)
                    for(int j=1;j<n;j++){
                        fout<<ver[j][i]<<' '<<ver[j+1][i]<<' '<<1<<'\n';
                        fout<<ver[j+1][i]<<' '<<ver[j][i]<<' '<<1<<'\n';
                    }

                for(int i=1;i<n;i++)
                    for(int j=1;j<m;j++)
                        fout<<ver[i][j]<<' '<<ver[i+1][j+1]<<' '<<rnd.irand(wmin,wmax)<<'\n';
            }

            void AutoMode(int wmin,int wmax){
                std::ofstream &fout = *(rout);
                fout<<this->GetVertexCount()<<' '<<this->GetEdgeCount()<<' '<<this->GetStart()<<'\n';
                this->Output(wmin,wmax);
            }
        protected:
            void shuffle(){
                for(int i=1;i<=n;i++){
                    for(int j=1;j<=m;j++){
                        int oth = rnd.irand(1,m);
                        swap(ver[i][j],ver[i][oth]);
                        oth = rnd.irand(1,n);
                        swap(ver[i][j],ver[oth][j]);
                    }
                }
            }

            inline void swap(int &x,int &y){
                int temp = y;
                y = x;
                x = temp;
            }
            
        private:
            int **ver;
            int realsize,m,n;
            Random rnd;
    };

    class DAG{
        public:
            DAG(int n,int m){
                if(m<n-1)
                    throw "Invalid edge count.";
                ver = new int[n+1];
                head = new int[n*(n-1)>>1];
                next = new int[n*(n-1)>>1];
                ver = new int[n+1];
                tot = 0;
                this->n = n;
                this->m = m;
            }

            void Output(){
                for(int i=1;i<=n;i++)
                    for(int j=i;j<=n;j++)
                        add(i,j);

                while(m){
                    
                }
            }
        protected:
            void add(int u,int v){
                ver[++tot] = v;
                next[tot] = head[u];
                head[u] = tot;
            }
        private:
            int *head,*next,*ver;
            int tot;
            int n,m;
    };
};

#endif
