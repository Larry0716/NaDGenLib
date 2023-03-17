/**
 * @file random.h
 * @author Larry Green (larry0716@163.com)
 * @brief 这是 NaDGenLib 关于随机值部分的头文件
 * @details 事实上，这并不是非常好的代码，我很抱歉
 * @version 0.0.1
 * @date 2023-03-15
 * @note 感谢洛谷团队的 CYaRon 项目带给我的启发
 * @copyright Copyright (c) 2023
 */
#ifndef LARRY76_NADGENLIB_MODULE_RANDOM_HEADER
#define LARRY76_NADGENLIB_MODULE_RANDOM_HEADER

#include <chrono>
#include <random>
#include <string>

#include "genlib.h"

namespace Generator
{
    using std::string;

    /**
     * @brief 随机生成器，用于生成整数、浮点数随机值和随机字符串。
    */
    class Random {
    public:
        /**
         * @brief Random 类构造函数
        */
        Random()
        {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
            ::srand(seed + (std::random_device{}()) + time(0) * ::rand());
            seed += ::rand();
            rand_int = new std::mt19937_64(seed);
            rand_real = new std::default_random_engine(seed);
        }
        /**
         * @brief 生成指定范围 [l,r] 的整数
         * @param l 生成的最小值
         * @param r 生成的最大值
         * @warning 注意，该生成方式不一定均匀
        */
        inline long long irand(long long l, long long r)
        {
            asserti(r>=l,"Invalid range");
            return (*rand_int)() % (r-l+1) + l;
        }
        /**
         * @brief 生成指定范围 [l,r] 的浮点数
         * @param l 生成的最小值
         * @param r 生成的最大值
        */
        inline long double frand(long double l, long double r)
        {
            asserti(r>=l, "Invalid range");
            static std::uniform_real_distribution<long double> dist(l, r);
            return dist(*rand_real);
        }
        /**
         * @brief 生成指定长度指定字符集的字符串
         * @param length 生成的字符串长度
         * @param charset 字符集，默认是数字加字母大小写
        */
        string srand( unsigned long long length, 
                      string charset="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZMXNCBV" )
        {
            string ans;
            while(length--)
                ans += charset[ irand(0, charset.length()-1) ];
            return ans;
        }
    private:
        std::mt19937_64 *rand_int;
        std::default_random_engine *rand_real;
        unsigned long long seed = 0;
    };
}

#endif