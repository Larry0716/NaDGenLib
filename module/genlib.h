/**
 * @file genlib.h
 * @author Larry Green (larry0716@163.com)
 * @brief 这是 NaDGenLib 的基础头文件
 * @details 事实上，这并不是非常好的代码，我很抱歉
 * @version 0.0.1
 * @date 2023-03-15
 * @note 感谢洛谷团队的 CYaRon 项目带给我的启发
 * @copyright Copyright (c) 2023
 */
#ifndef LARRY76_NADGENLIB_MODULE_GENLIB_HEADER
#define LARRY76_NADGENLIB_MODULE_GENLIB_HEADER

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <string>
#include <fstream>
#include <iostream>

namespace Generator
{
    using std::string;
    std::ofstream *rout = nullptr;
    static string stdPath;
    static string nowFilename;
    static bool registeredStd = false;

    static void failed_to_execute()
    {
        fprintf(stderr, "\nFatal Error!!! Generator failed to continue executing!!!");
        *((int *)NULL) = 114514;
    }

    static void assertion_failed( const char *file, 
                                  const char *base_file, 
                                  const int line, 
                                  const char *exp, 
                                  const char *reason )
    {
        fprintf(stderr, "Assertion Failed!!\n");
        fprintf(stderr, "-> File: %s\n", file);
        fprintf(stderr, "-> Base File: %s\n", base_file);
        fprintf(stderr, "-> Line: %d\n", line);
        fprintf(stderr, "-> Expression: %s\n", exp);
        fprintf(stderr, "-> Information: %s\n", reason);
        failed_to_execute();
    }

    #ifdef assert
        #undef assert
    #endif
    #define assert(exp) if(!(exp)){ assertion_failed(__FILE__, __BASE_FILE__, __LINE__, #exp, NULL); }

    #define asserti(exp,info) if(!(exp)){ assertion_failed(__FILE__, __BASE_FILE__, __LINE__, #exp, info); }

    static string FormatString(string buffer)
    {
        int i = 0;
        for(i = buffer.length()-1; i >= 0; --i) {
            if(buffer[i] == '.')
                break;
        }
        return buffer.substr(0, i);
    }

    /**
     * @brief 用于将输出重定向
     * @warning 注意，除非使用函数 AutoGenerate，否则在任何情况下，请务必先调用该函数。
     * @warning 注意，使用该头文件时，请不要使用 freopen 等重定向。
     * @param filename 用于表示输入文件名。
    */
    void RedirectToFileFunc(string filename)
    {
        if(rout != nullptr)
            delete rout;
        rout = new std::ofstream(filename);
        asserti(rout->is_open(), "Failed to open input file");
        nowFilename = filename;
    }

    #define RedirectToFile(filename) RedirectToFileFunc(filename);\
        std::ofstream &cout = *rout;

    #define FlushIOStream() std::ofstream &cout = *rout;

    /**
     * @brief 用于注册标算程序
     * @warning 注意，注册过程只需要调用一次
     * @warning 注意，标算程序请不要重定向文件。
     * @warning 注意，如果要在 AutoGenerate 启用生成答案支持，请务必在 AutoGenerate 调用前调用它。
     * @param path 标算程序路径。
    */
    void RegisterStdProgram(string path)
    {
        asserti(!path.empty(), "Invalid std filepath");
        stdPath = path;
        registeredStd = true;
    }

    /**
     * @brief 用于生成文件答案。
     * @warning 注意，调用前请先注册标算。
     * @warning 请不要在 AutoGenerate 的自定义函数中调用它
     * @param ansFilename 输出答案文件名
     * @param inputFilename 输入文件名，默认为 RedirectToFile 所指向的文件。
    */
    void GeneratreAns(string ansFilename, string inputFilename = nowFilename){
        asserti(registeredStd, "No std program registered yet!!!");
        asserti(!inputFilename.empty(), "Invalid input filename!!! Maybe you haven\'t redirect to a file yet?");
        asserti(!ansFilename.empty(), "Invalid ans filename!!!");
        char buffer[2048];
        sprintf(buffer, "%s < %s > %s", stdPath.c_str(), inputFilename.c_str(), ansFilename.c_str());
        int ret = system(buffer);
        asserti(!ret, "Std program terminated unexpectedly");
    }

    /**
     * @brief 用于自动生成输入数据。
     * @param format 输入文件的文件名格式。
     * @param startLabel 表示起始标号。
     * @param endLabel 表示结束标号。
     * @param custom 用于传递用户自定义的生成方案函数。
     * @param enableStd 用于决定是否启用标算程序来用于生成答案，默认为不启用（false）。
     * @warning 请注意，文件名的格式应当包含且只包含一个 %d。
     * @warning 请注意，文件名不会自动补全后缀
     * @warning 请注意，答案文件文件名会有强制后缀.ans
    */
    void AutoGenerate( string format,
                       int startLabel, int endLabel, void (*custom)(),
                       bool enableStd = false )
    {
        char buffer[1024];
        char stdbuffer[1024];
        string stdFormat;
        if(enableStd){
            asserti(registeredStd, "No std program registered yet!!");
            stdFormat = stdPath + " < %s > ";
            stdFormat += FormatString(format);
            stdFormat += ".ans";
        }
        time_t agcs = clock();
        for(int i=startLabel; i<=endLabel; i++) {
            sprintf(buffer, format.c_str(), i);
            RedirectToFileFunc(string(buffer));
            (*custom)();
            rout->close();
            if(enableStd) {
                sprintf(stdbuffer, stdFormat.c_str(), buffer, i);
                int ret = system(stdbuffer);
                asserti(!ret, "Std program terminated unexpectedly");
            }
        }
        time_t agce = clock();
        std::cerr << "AutoGenerate Used Time: " << agce-agcs << "ms." << std::endl; 
    }

    /**
     * @brief 用于自动生成。
     * @param format 输入文件的文件名格式。
     * @param startLabel 表示起始标号。
     * @param endLabel 表示结束标号。
     * @param custom 用于传递用户自定义的生成方案函数。
     * @param enableStd 用于决定是否启用标算程序来用于生成答案，默认为不启用（false）。
     * @warning 请注意，文件名的格式应当包含且只包含一个 %d。
     * @warning 请注意，文件名不会自动补全后缀
     * @warning 请注意，答案文件文件名会有强制后缀.ans
    */
    void AutoGenerate( string format, 
                       int startLabel, int endLabel, void (*custom)(int),
                       bool enableStd = false )
    {
        char buffer[1024];
        char stdbuffer[1024];
        string stdFormat;
        if(enableStd){
            asserti(registeredStd, "No std program registered yet!!");
            stdFormat = stdPath + " < %s > ";
            stdFormat += FormatString(format);
            stdFormat += ".ans";
        }
        time_t agcs = clock();
        for(int i=startLabel; i<=endLabel; i++){
            sprintf(buffer, format.c_str(), i);
            RedirectToFileFunc(string(buffer));
            (*custom)(i);
            rout->close();
            if(enableStd){
                sprintf(buffer, stdFormat.c_str(), buffer, i);
                int ret = system(stdbuffer);
                asserti(!ret, "Std program terminated unexpectedly");
            }
        }
        time_t agce = clock();
        std::cerr << "AutoGenerate Used Time: " << agce-agcs << "ms." << std::endl; 
    }
}

#endif
