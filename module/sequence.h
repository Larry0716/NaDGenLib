/**
 * @file sequence.h
 * @author Larry Green (larry0716@163.com)
 * @brief 这是 NaDGenLib 关于序列部分的头文件
 * @details 事实上，这并不是非常好的代码，我很抱歉
 * @version 0.0.1
 * @date 2023-03-15
 * @note 感谢洛谷团队的 CYaRon 项目带给我的启发
 * @copyright Copyright (c) 2023
 */
#ifndef LARRY76_NADGENLIB_MODULE_SEQUENCE_HEADER
#define LARRY76_NADGENLIB_MODULE_SEQUENCE_HEADER

#include <fstream>
#include <string>

#include "genlib.h"

namespace Generator
{
    using std::string;
    extern std::ofstream *rout;

    enum SEQ_GEN_METHOD{
        METHOD_INTEGER = 1,// 生成整数序列
        METHOD_FLOAT,// 生成浮点数序列
        METHOD_CUSTOM//生成自定义序列
    };

    /**
     * @brief 用于 Sequence 类的配置
    */
    template <typename T = int>
    struct SEQ_GEN_CONF{
        SEQ_GEN_CONF(){}
        /**
         * @brief SEQ_GEN_CONF 的构造函数
         * @param method 生成方式
         * @param length 生成序列长度
         * @param vmin 生成序列的最小值
         * @param vmax 生成序列的最大值
         * @param split 生成序列的分割方式，默认为空格
         * @param ends 生成序列的结束方式，默认为回车
        */
        SEQ_GEN_CONF( SEQ_GEN_METHOD method,
                      unsigned long long length, T vmin, T vmax,
                      string split = " ", string ends = "\n" )
        {
            asserti(method != METHOD_CUSTOM, "Unmatched method");
            this->method = method;
            this->length = length;
            this->vmin = vmin;
            this->vmax = vmax;
            this->split = split;
            this->ends = ends;
        }
        /**
         * @brief SEQ_GEN_CONF 的构造函数
         * @param method 生成方式
         * @param length 生成序列长度
         * @param vmin 生成序列的最小值
         * @param vmax 生成序列的最大值
         * @param eps 生成序列的数据精度
         * @param split 生成序列的分割方式，默认为空格
         * @param ends 生成序列的结束方式，默认为回车
         * @overload
        */
        SEQ_GEN_CONF( SEQ_GEN_METHOD method,
                      unsigned long long length, T vmin, T vmax, unsigned eps,
                      string split = " ", string ends = "\n" )
        {
            asserti(method != METHOD_CUSTOM, "Unmatched method");
            this->method = method;
            this->length = length;
            this->vmin = vmin;
            this->vmax = vmax;
            this->eps = eps;
            this->split = split;
            this->ends = ends;
        }
        /**
         * @brief SEQ_GEN_CONF 的构造函数
         * @param method 生成方式
         * @param length 生成序列长度
         * @param customFunction 自定义生成函数
         * @param split 生成序列的分割方式，默认为空格
         * @param ends 生成序列的结束方式，默认为回车
         * @warning 如果生成函数也需要精度控制，请执行修改 eps。
        */
        SEQ_GEN_CONF( SEQ_GEN_METHOD method,
                      unsigned long long length, T (*customFunction)(int n),
                      string split = " ", string ends = "\n" )
        {
            asserti(method == METHOD_CUSTOM, "Unmatched method");
            this->method = method;
            this->length = length;
            this->customFunction = customFunction;
            this->split = split;
            this->ends = ends;
        }
        SEQ_GEN_METHOD method; // 生成方式，必填
        unsigned long long length; // 生成长度，必填
        T vmin; // 生成最小值，若非提供自定义生成函数，则必填
        T vmax; // 生成最大值，若非提供自定义生成函数，则必填
        unsigned eps = 3; // 生成精度，若非浮点数生成，则忽略，若浮点数生成，则默认精度为 3
        std::string split; // 分割方式，必填
        std::string ends; // 结束方式，必填
        T (*customFunction)(int n) = nullptr; // 自定义生成函数，若确认提供自定义生成函数，则必填
    };

    /**
     * @brief 生成序列
     * @warning 注意，默认生成整数序列
    */
    template <typename T=int>
    class Sequence{
    public:
        /**
         * @brief Sequence 的默认构造函数。
        */
        Sequence(){}
        /**
         * @brief Sequence 的标准构造。
        */
        Sequence(SEQ_GEN_CONF<T> config)
        {
            SetConfig(config);
        }
        /**
         * @brief 设置生成序列规则
         * @param config 规则结构体，详情请参照 SEQ_GEN_CONF 结构体
        */
        void SetConfig( SEQ_GEN_CONF<T> config )
        {
            settedConfig = true;
            asserti(config.length != 0, "Invalid length");
            if(config.method == METHOD_CUSTOM) {
            	isCustomGenerate = true;
                asserti(config.customFunction != nullptr, "Invalid custom generating function\'s Address.");
            } else {
                asserti(config.vmax >= config.vmin, "Invalid range");
            }
            genConfig = config;
        }
        /**
         * @brief 启动生成。
         * @warning 注意，此函数并不会返回任何值，因为他将会直接输出到文件中
        */
        void Generate()
        {
            asserti(settedConfig, "No config setted yet!!");
            switch (genConfig.method) {
                case METHOD_CUSTOM: {
                    CustomGen();
                    break;
                }
                case METHOD_INTEGER: {
                    IntegerGen();
                    break;
                }
                case METHOD_FLOAT: {
                    FloatGen();
                    break;
                }
            }
        }
    protected:
        /**
         * @brief 生成整数序列
        */
        void IntegerGen()
        {
        	std::ofstream &cout = *rout;
            for(register int i = 1; i < int(genConfig.length); ++i) {
                cout << rnd.irand(genConfig.vmin, genConfig.vmax) << genConfig.split;
            }
            cout << rnd.irand(genConfig.vmin, genConfig.vmax) << genConfig.ends;
        }
        /**
         * @brief 生成浮点数序列
        */
        void FloatGen()
        {
        	std::ofstream &cout = *rout;
            for(register int i = 1; i < int(genConfig.length); ++i) {
                cout << std::fixed << std::setprecision(genConfig.eps) 
                     << rnd.frand(genConfig.vmin, genConfig.vmax) 
                     << genConfig.split;
            }
            cout << rnd.frand(genConfig.vmin, genConfig.vmax) << genConfig.ends;
        }
        /**
         * @brief 生成自定义序列
        */
        void CustomGen()
        {
        	std::ofstream &cout = *rout;
            for(register int i = 1; i < int(genConfig.length); ++i) {
                cout << genConfig.customFunction(i) << genConfig.split;
            }
            cout << genConfig.customFunction(genConfig.length) << genConfig.ends;
        }
    private:
        bool settedConfig;
        bool isCustomGenerate;
        SEQ_GEN_CONF<T> genConfig;
        Random rnd;
    };

    /**
     * @brief 生成排列
    */
    class Permutation{
    public:
        /**
         * @brief 排列的构造函数
        */
        Permutation(int length)
        {
            id = new int[length+1];
            for(int i = 0; i <= length; i++)
                id[i] = i;
            shuffle();
        }
        /**
         * @brief 输出生成结果
         * @param split 分割方式，默认为空格
         * @param ends 结束方式，默认为换行
         * @warning 注意，split 和 ends 都应当为字符串
        */
        void Output(string split = " ", string ends = "\n")
        {
            std::ofstream &cout = *rout;
            for(int i = 1; i <= length; ++i)
                cout << id[i] << split;
            cout << ends;
        }
        /**
         * @brief 排列的析构函数。
        */
        ~Permutation()
        {
            delete[] id;
        }
    protected:
        /**
         * @brief 打乱排列
        */
        void shuffle()
        {
            for(int i = 1; i <= length; ++i) {
                int oth = rnd.irand(1, length);
                std::swap(id[i], id[oth]);
            }
        }
    private:
        Random rnd;
        int *id;
        int length;
    };
}

#endif
