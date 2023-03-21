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
#include <algorithm>
#include <vector>
#include <map>

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
	 * @brief ���ڽ�����ض���
	 * @warning ע�⣬����ʹ�ú��� AutoGenerate���������κ�����£�������ȵ��øú�����
	 * @warning ע�⣬ʹ�ø�ͷ�ļ�ʱ���벻Ҫʹ�� freopen ���ض���
	 * @param filename ���ڱ�ʾ�����ļ�����
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
	 * @brief ����ע��������
	 * @warning ע�⣬ע�����ֻ��Ҫ����һ��
	 * @warning ע�⣬��������벻Ҫ�ض����ļ���
	 * @warning ע�⣬���Ҫ�� AutoGenerate �������ɴ�֧�֣�������� AutoGenerate ����ǰ��������
	 * @param path �������·����
	*/
	void RegisterStdProgram(string path)
	{
		asserti(!path.empty(), "Invalid std filepath");
		stdPath = path;
		registeredStd = true;
	}

	/**
	 * @brief ���������ļ��𰸡�
	 * @warning ע�⣬����ǰ����ע����㡣
	 * @warning �벻Ҫ�� AutoGenerate ���Զ��庯���е�����
	 * @param ansFilename ������ļ���
	 * @param inputFilename �����ļ�����Ĭ��Ϊ RedirectToFile ��ָ����ļ���
	*/
	void GeneratreAns(string ansFilename, string inputFilename = nowFilename)
	{
		asserti(registeredStd, "No std program registered yet!!!");
		asserti(!inputFilename.empty(), "Invalid input filename!!! Maybe you haven\'t redirect to a file yet?");
		asserti(!ansFilename.empty(), "Invalid ans filename!!!");
		char buffer[2048];
		sprintf(buffer, "%s < %s > %s", stdPath.c_str(), inputFilename.c_str(), ansFilename.c_str());
		int ret = system(buffer);
		asserti(!ret, "Std program terminated unexpectedly");
	}

	/**
	 * @brief �����Զ������������ݡ�
	 * @param format �����ļ����ļ�����ʽ��
	 * @param startLabel ��ʾ��ʼ��š�
	 * @param endLabel ��ʾ������š�
	 * @param custom ���ڴ����û��Զ�������ɷ���������
	 * @param enableStd ���ھ����Ƿ����ñ���������������ɴ𰸣�Ĭ��Ϊ�����ã�false����
	 * @warning ��ע�⣬�ļ����ĸ�ʽӦ��������ֻ����һ�� %d��
	 * @warning ��ע�⣬�ļ��������Զ���ȫ��׺
	 * @warning ��ע�⣬���ļ��ļ�������ǿ�ƺ�׺.ans
	*/
	void AutoGenerate( string format,
	                   int startLabel, int endLabel, void (*custom)(),
	                   bool enableStd = false )
	{
		char buffer[1024];
		char stdbuffer[1024];
		string stdFormat;
		if(enableStd) {
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
	 * @brief �����Զ����ɡ�
	 * @param format �����ļ����ļ�����ʽ��
	 * @param startLabel ��ʾ��ʼ��š�
	 * @param endLabel ��ʾ������š�
	 * @param custom ���ڴ����û��Զ�������ɷ���������
	 * @param enableStd ���ھ����Ƿ����ñ���������������ɴ𰸣�Ĭ��Ϊ�����ã�false����
	 * @warning ��ע�⣬�ļ����ĸ�ʽӦ��������ֻ����һ�� %d��
	 * @warning ��ע�⣬�ļ��������Զ���ȫ��׺
	 * @warning ��ע�⣬���ļ��ļ�������ǿ�ƺ�׺.ans
	*/
	void AutoGenerate( string format,
	                   int startLabel, int endLabel, void (*custom)(int nowLabel),
	                   bool enableStd = false )
	{
		char buffer[1024];
		char stdbuffer[1024];
		string stdFormat;
		if(enableStd) {
			asserti(registeredStd, "No std program registered yet!!");
			stdFormat = stdPath + " < %s > ";
			stdFormat += FormatString(format);
			stdFormat += ".ans";
		}
		time_t agcs = clock();
		for(int i=startLabel; i<=endLabel; i++) {
			sprintf(buffer, format.c_str(), i);
			RedirectToFileFunc(string(buffer));
			(*custom)(i);
			rout->close();
			if(enableStd) {
				sprintf(buffer, stdFormat.c_str(), buffer, i);
				int ret = system(stdbuffer);
				asserti(!ret, "Std program terminated unexpectedly");
			}
		}
		time_t agce = clock();
		std::cerr << "AutoGenerate Used Time: " << agce-agcs << "ms." << std::endl;
	}
}

namespace Generator
{
    using std::string;

    /**
     * @brief ����������������������������������ֵ������ַ�����
    */
    class Random {
    public:
        /**
         * @brief Random �๹�캯��
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
         * @brief ����ָ����Χ [l,r] ������
         * @param l ���ɵ���Сֵ
         * @param r ���ɵ����ֵ
         * @warning ע�⣬�����ɷ�ʽ��һ������
        */
        inline long long irand(long long l, long long r)
        {
            asserti(r>=l,"Invalid range");
            return (*rand_int)() % (r-l+1) + l;
        }
        /**
         * @brief ����ָ����Χ [l,r] �ĸ�����
         * @param l ���ɵ���Сֵ
         * @param r ���ɵ����ֵ
        */
        inline long double frand(long double l, long double r)
        {
            asserti(r>=l, "Invalid range");
            static std::uniform_real_distribution<long double> dist(l, r);
            return dist(*rand_real);
        }
        /**
         * @brief ����ָ������ָ���ַ������ַ���
         * @param length ���ɵ��ַ�������
         * @param charset �ַ�����Ĭ�������ּ���ĸ��Сд
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

    /**
     * @brief ����ָ����Χ [l,r] ������
     * @param l ���ɵ���Сֵ
     * @param r ���ɵ����ֵ
     * @warning ע�⣬�����ɷ�ʽ��һ������
     * @warning ���ʹ�øú��������� Random ���������򲻱�֤ÿ�������¼���Ϊ����
    */
    inline long long irand(long long l, long long r)
    {
        static Random rnd;
        return rnd.irand(l, r);
    }

    /**
     * @brief ����ָ����Χ [l,r] �ĸ�����
     * @param l ���ɵ���Сֵ
     * @param r ���ɵ����ֵ
     * @warning ���ʹ�øú��������� Random ���������򲻱�֤ÿ�������¼���Ϊ����
    */
    inline long double frand(long double l, long double r)
    {
        static Random rnd;
        return rnd.frand(l, r);
    }

    /**
     * @brief ����ָ������ָ���ַ������ַ���
     * @param length ���ɵ��ַ�������
     * @param charset �ַ�����Ĭ�������ּ���ĸ��Сд
     * @warning ���ʹ�øú��������� Random ���������򲻱�֤ÿ�������¼���Ϊ����
    */
    inline string srand( unsigned long long length, 
                         string charset="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZMXNCBV" )
    {
        static Random rnd;
        return rnd.srand(length, charset);
    }
}

namespace Generator
{
	using std::string;
	extern std::ofstream *rout;

	enum SEQ_GEN_METHOD {
	    METHOD_INTEGER = 1,// ������������
	    METHOD_FLOAT,// ���ɸ���������
	    METHOD_CUSTOM//�����Զ�������
	};

	/**
	 * @brief ���� Sequence �������
	*/
	template <typename T = int>
	struct SEQ_GEN_CONF {
		SEQ_GEN_CONF() {}
		/**
		 * @brief SEQ_GEN_CONF �Ĺ��캯��
		 * @param method ���ɷ�ʽ
		 * @param length �������г���
		 * @param vmin �������е���Сֵ
		 * @param vmax �������е����ֵ
		 * @param split �������еķָʽ��Ĭ��Ϊ�ո�
		 * @param ends �������еĽ�����ʽ��Ĭ��Ϊ�س�
		*/
		SEQ_GEN_CONF( SEQ_GEN_METHOD method,
		              unsigned long long length, T vmin, T vmax,
		              string split = " ", string ends = "\n" ) {
			asserti(method != METHOD_CUSTOM, "Unmatched method");
			this->method = method;
			this->length = length;
			this->vmin = vmin;
			this->vmax = vmax;
			this->split = split;
			this->ends = ends;
		}
		/**
		 * @brief SEQ_GEN_CONF �Ĺ��캯��
		 * @param method ���ɷ�ʽ
		 * @param length �������г���
		 * @param vmin �������е���Сֵ
		 * @param vmax �������е����ֵ
		 * @param eps �������е����ݾ���
		 * @param split �������еķָʽ��Ĭ��Ϊ�ո�
		 * @param ends �������еĽ�����ʽ��Ĭ��Ϊ�س�
		 * @overload
		*/
		SEQ_GEN_CONF( SEQ_GEN_METHOD method,
		              unsigned long long length, T vmin, T vmax, unsigned eps,
		              string split = " ", string ends = "\n" ) {
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
		 * @brief SEQ_GEN_CONF �Ĺ��캯��
		 * @param method ���ɷ�ʽ
		 * @param length �������г���
		 * @param customFunction �Զ������ɺ���
		 * @param split �������еķָʽ��Ĭ��Ϊ�ո�
		 * @param ends �������еĽ�����ʽ��Ĭ��Ϊ�س�
		 * @warning ������ɺ���Ҳ��Ҫ���ȿ��ƣ���ִ���޸� eps��
		*/
		SEQ_GEN_CONF( SEQ_GEN_METHOD method,
		              unsigned long long length, T (*customFunction)(int n),
		              string split = " ", string ends = "\n" ) {
			asserti(method == METHOD_CUSTOM, "Unmatched method");
			this->method = method;
			this->length = length;
			this->customFunction = customFunction;
			this->split = split;
			this->ends = ends;
		}
		SEQ_GEN_METHOD method; // ���ɷ�ʽ������
		unsigned long long length; // ���ɳ��ȣ�����
		T vmin; // ������Сֵ�������ṩ�Զ������ɺ����������
		T vmax; // �������ֵ�������ṩ�Զ������ɺ����������
		unsigned eps = 3; // ���ɾ��ȣ����Ǹ��������ɣ�����ԣ������������ɣ���Ĭ�Ͼ���Ϊ 3
		std::string split; // �ָʽ������
		std::string ends; // ������ʽ������
		T (*customFunction)(int n) = nullptr; // �Զ������ɺ�������ȷ���ṩ�Զ������ɺ����������
	};

	/**
	 * @brief ��������
	 * @warning ע�⣬Ĭ��������������
	*/
	template <typename T=int>
	class Sequence
	{
		public:
			/**
			 * @brief Sequence ��Ĭ�Ϲ��캯����
			*/
			Sequence() {}
			/**
			 * @brief Sequence �ı�׼���졣
			*/
			Sequence(SEQ_GEN_CONF<T> config) {
				SetConfig(config);
			}
			/**
			 * @brief �����������й���
			 * @param config ����ṹ�壬��������� SEQ_GEN_CONF �ṹ��
			*/
			void SetConfig( SEQ_GEN_CONF<T> config ) {
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
			 * @brief �������ɡ�
			 * @warning ע�⣬�˺��������᷵���κ�ֵ����Ϊ������ֱ��������ļ���
			*/
			void Generate() {
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
			 * @brief ������������
			*/
			void IntegerGen() {
				std::ofstream &cout = *rout;
				for(register int i = 1; i < int(genConfig.length); ++i) {
					cout << rnd.irand(genConfig.vmin, genConfig.vmax) << genConfig.split;
				}
				cout << rnd.irand(genConfig.vmin, genConfig.vmax) << genConfig.ends;
			}
			/**
			 * @brief ���ɸ���������
			*/
			void FloatGen() {
				std::ofstream &cout = *rout;
				for(register int i = 1; i < int(genConfig.length); ++i) {
					cout << std::fixed << std::setprecision(genConfig.eps)
					     << rnd.frand(genConfig.vmin, genConfig.vmax)
					     << genConfig.split;
				}
				cout << rnd.frand(genConfig.vmin, genConfig.vmax) << genConfig.ends;
			}
			/**
			 * @brief �����Զ�������
			*/
			void CustomGen() {
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
	 * @brief ��������
	*/
	class Permutation
	{
		public:
			/**
			 * @brief ���еĹ��캯��
			*/
			Permutation(int length) {
				id = new int[length+1];
				for(int i = 0; i <= length; i++)
					id[i] = i;
				shuffle();
			}
			/**
			 * @brief ������ɽ��
			 * @param split �ָʽ��Ĭ��Ϊ�ո�
			 * @param ends ������ʽ��Ĭ��Ϊ����
			 * @warning ע�⣬split �� ends ��Ӧ��Ϊ�ַ���
			*/
			void Output(string split = " ", string ends = "\n") {
				std::ofstream &cout = *rout;
				for(int i = 1; i <= length; ++i)
					cout << id[i] << split;
				cout << ends;
			}
			/**
			 * @brief ���е�����������
			*/
			~Permutation() {
				delete[] id;
			}
		protected:
			/**
			 * @brief ��������
			*/
			void shuffle() {
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

namespace Generator
{
	using std::vector;
	using std::map;
	using std::string;
	using std::endl;

	typedef std::pair<int,int> pii;

	/**
	 * @brief ͼ�Ļ��࣬���ڴ洢ͼ��ʵ��ͼ�е�һЩ���ܡ�
	*/
	class Graph
	{
		public:
			/**
			 * @brief ͼ�Ĺ��캯��
			 * @param verCount ͼ�ĵ���
			 * @param undirectedMap ����ͼ���أ�Ĭ��Ϊ����ͼ��false��
			 * @param weightedMap ��Ȩͼ���أ�Ĭ��Ϊ����Ȩ��false��
			 * @param muiltiedgeCheck �ر߼�鿪�أ�Ĭ��Ϊ�رգ�false��
			 * @param loopCheck �Ի���鿪�أ�Ĭ��Ϊ�رգ�false��
			*/
			Graph( int verCount,
			       bool undirectedMap = false, bool weightedMap = false,
			       bool muiltiedgeCheck = false, bool loopCheck = false ) {
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
			 * @brief Graph ����������������ڷ�ֹ Graph �������ڴ�й©
			*/
			~Graph() {
				delete[] id;
			}
			/**
			 * @brief �������ͼ
			 * @warning ע�⣬���ͼ������ζ���������ռ�õ��ڴ�ռ䡣
			*/
			void clear() {
				for(register int i = 0; i <= vertexCount; ++i)
					id[i] = i;
				hashtable.clear();
				edgeContainer.clear();
			}
			/**
			 * @brief ���һ����
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			bool add_edge(int from, int to) {
				asserti(!weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
				if(!multiedgeChecker(from, to))
					return false;
				if(!loopChecker(from, to))
					return false;
				__add_edge(from, to);
				return true;
			}
			/**
			 * @brief ���һ����Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			 * @param weight �ߵ�Ȩ
			*/
			bool add_edge(int from, int to, int weight) {
				asserti(weightedMapSwitch, "PANIC!! The operation doesn't match the setting.");
				if(!multiedgeChecker(from, to))
					return false;
				if(!loopChecker(from, to))
					return false;
				__add_edge(from, to, weight);
				return true;
			}
			/**
			 * @brief ���ͼ�Ľӿ�
			 * @param shuffleOutput �Ƿ���������ڵ����
			*/
			void Output(bool shuffleOutput = true) {
				_output(shuffleOutput);
			}
			/**
			 * @brief ��ȡͼ�ı����Ľӿ�
			*/
			inline int GetEdgeCount() {
				return _GetEdgeCount();
			}
		protected:
			int *id;
			/**
			 * @brief ��ȡ������ʵ��
			*/
			inline int _GetEdgeCount() {
				return edgeContainer.size();
			}
			/**
			 * @brief ���ͼ��ʵ��
			 * @param shuffleOutput ���ҵ��������Ĭ��Ϊ���ã�true��
			*/
			void _output(bool shuffleOutput = true) {
				std::ofstream &cout = *rout;
				if(shuffleOutput)
					shuffle();
				for(auto c : edgeContainer) {
					if(weightedMapSwitch)
						cout << id[c.from] << ' '
						     << id[c.to] << ' '
						     << c.weight << endl;
					else
						cout << id[c.from] << ' '
						     << id[c.to] << endl;
				}
			}
			/**
			 * @brief ���ҵ���
			*/
			void shuffle() {
				Random rnd;
				for(int i = 1; i <= vertexCount; ++i) {
					int oth = rnd.irand(1, vertexCount);
					std::swap(id[i], id[oth]);
				}
				std::shuffle(edgeContainer.begin(), edgeContainer.end(), std::mt19937_64(time(0)));
			}
			/**
			 * @brief ��һ����Ȩ�ߵ�ʵ��
			*/
			inline void __add_edge(int from, int to, int weight) {
				edgeContainer.push_back( (EDGE) {
					from, to, weight
				} );
			}
			/**
			 * @brief ��һ����Ȩ�ߵ�ʵ��
			*/
			inline void __add_edge(int from, int to) {
				edgeContainer.push_back( (EDGE) {
					from, to, 1
				} );
			}
			/**
			 * @brief �ر߼����
			 * @return true ��Ϊ���ͨ����false ��Ϊ���ʧ��
			 * @note �����ʧ�ܣ���������־
			*/
			inline bool multiedgeChecker(int from, int to) {
				if(!muiltiedgeCheckSwitch)
					return true;
				if(from > to && undirectedMapSwitch)
					std::swap(from,to);
				if(hashtable[ {from, to}]) {
					fprintf(stderr, "Edge <%d,%d> was unable to pass muiltiedge check. Ignored.\n", from, to);
					return false;
				}
				hashtable[ {from, to}] = true;
				return true;
			}
			/**
			 * @brief �Ի������
			 * @return true ��Ϊ���ͨ����false ��Ϊ���ʧ��
			 * @note �����ʧ�ܣ���������־
			*/
			inline bool loopChecker(int from, int to) {
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
			struct EDGE {
				int from, to, weight;
			};
			vector<EDGE> edgeContainer;
			map<pii,bool> hashtable;
	};

	/**
	 * @brief �޸������ɷ�ʽ
	*/
	enum NRT_GEN_CONF {
	    METHOD_DAISY=5, //����һ��ջ�
	    METHOD_CHAIN=10, //����һ����
	    METHOD_CHAIN_WITH_CHAIN=25, //����������
	    METHOD_DAISY_WITH_DAISY=40, //���ɾջ��׾ջ�
	    METHOD_CHAIN_WITH_DAISY=60, //�������׾ջ�
	    METHOD_COMP_KB_TREE=75, //������ȫK����
	    METHOD_RND_TREE=90, //���������
	    METHOD_TREE_OVER_TREE=100 //����������������
	};

	/**
	 * @brief ����һ���޸���
	*/
	class NoRootTree : protected Graph
	{
		public:
			/**
			 * @brief NoRootTree �Ĵ���Ȩ���캯��
			 * @param verCount ����
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			 * @warning ע�⣬�ù��캯��������һ�������б��ʲ�ͬ
			*/
			NoRootTree(int verCount, int vmin, int vmax)
				:Graph(verCount, true, true, true, true) {
				vertexCount = verCount;
				weightedTreeSwitch = true;
				this->vmin = vmin;
				this->vmax = vmax;
				clear();
			}
			/**
			 * @brief NoRootTree ���ޱ�Ȩ���캯��
			 * @param verCount ����
			 * @warning ע�⣬�ù��캯��������һ�������б��ʲ�ͬ
			*/
			NoRootTree(int verCount)
				:Graph(verCount, true, false, true, true) {
				vertexCount = verCount;
				weightedTreeSwitch = false;
				clear();
			}
			/**
			 * @brief ���������������
			*/
			void Generate() {
				clear();
				int mode = rnd.irand(1, 100);
				_Generate(mode);
			}
			/**
			 * @brief �����ض���������
			*/
			void SpecificGenerate(NRT_GEN_CONF tgc) {
				clear();
				_Generate(tgc);
			}
			/**
			 * @brief ������ɽ��
			*/
			void Output(bool shuffleOutput = true) {
				this->_output(shuffleOutput);
			}
		protected:
			/**
			 * @brief ��һ���ߣ��Զ����ɱ�Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			void add(int from, int to) {
				if(weightedTreeSwitch)
					add_edge(from, to, rnd.irand(vmin, vmax));
				else
					add_edge(from, to);
			}
			/**
			 * @brief ���ɷ�ʽѡ����
			*/
			void _Generate(int mode) {
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
			 * @brief ����һ��ջ�
			 * @param center ���Ľڵ���
			 * @param size �ջ���С
			 * @param labelbegin ��Ե�����ʼ���
			*/
			void Daisy(int center = 1, unsigned size = 0, int labelbegin = 1) {
				if(!size)
					size = vertexCount;
				for(int i = labelbegin; i < labelbegin+int(size); ++i)
					add(center, i);
			}
			/**
			 * @brief ����һ����
			 * @param begin ������ʼ�ڵ�
			 * @param length ���ĳ���
			 * @param labelbegin ���ĵڶ��ڵ���
			 * @warning ע�⣬labelbegin �� begin û�й�ϵ
			*/
			void Chain(int begin = 1, unsigned length = 0, int labelbegin = 1) {
				if(!length)
					length = vertexCount;
				add(begin, labelbegin);
				for(int i = labelbegin; i < labelbegin + int(length) - 1; ++i) {
					add(i, i+1);
				}
			}
			/**
			 * @brief �������׾ջ�
			 * @param ChainProbability �������ĸ���
			 * @param DaisyProbability ���ɾջ��ĸ���
			 * @warning ע�⣬�����ߵĸ�����ӱ���Ҫ���� 1
			 * @warning ע�⣬���׾ջ�����ͬ�ھջ���
			*/
			void ChainWithDaisy( double ChainProbability=0.5,
			                     double DaisyProbability=0.5 ) {
				asserti(ChainProbability + DaisyProbability == 1, "PANIC!!! Probability sum wasn't equal to 1.");
				int bsize = sqrt(vertexCount);
				int i = 1;
				for(i = 2; i+bsize-1 <= vertexCount; i += bsize) {
					double mode = rnd.frand(0, 1);
					int p = rnd.irand(1, i-1);
					if(mode <= ChainProbability)
						Chain(p, bsize, i);
					else
						Daisy(p, bsize, i);
				}
				if(i-1 < vertexCount) {
					double mode = rnd.frand(0, 1);
					int p = rnd.irand(1, i-1);
					if(mode <= ChainProbability)
						Chain(p, vertexCount-i+1, i);
					else
						Daisy(p, vertexCount-i+1, i);
				}
			}
			/**
			 * @brief ����������
			*/
			void ChainWithChain() {
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
			/**
			 * @brief ���ɾջ��׾ջ�
			*/
			void DaisyWithDaisy() {
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
			/**
			 * @brief ����һ�����������
			 * @param beginlabel ��ʾ��ʼ�ڵ���
			 * @param endlabel ��ʾ��ֹ�ڵ���
			*/
			void RandomTree(int beginlabel = 1, int endlabel = 0) {
				if(!endlabel)
					endlabel = vertexCount;
				for(int i = beginlabel+1; i <= endlabel; i++) {
					int from = rnd.irand(beginlabel, i-1);
					add(from, i);
				}
			}
			/**
			 * @brief ����һ����ȫ K ����
			*/
			void CompleteKBTree() {
				int k = rnd.irand(2, 5);
				for(int i = 2; i <= vertexCount; ++i) {
					if(weightedTreeSwitch)
						add_edge(i, ((k - 2) + i) / k, rnd.irand(vmin, vmax));
					else
						add_edge(i, ((k - 2) + i) / k);
				}
			}
			/**
			 * @brief ������������������������
			*/
			void TreeOverTree() {
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

	/**
	 * @brief ���ɾջ���
	*/
	class DaisyChain : protected Graph
	{
		public:
			/**
			 * @brief ��Ȩ DaisyChain �Ĺ��캯��
			 * @param verCount �ջ����Ľڵ���
			*/
			DaisyChain(int verCount)
				:Graph(verCount, false, false, true, true) {
				vertexCount = verCount;
				weightedDaisyMapSwitch = false;
			}
			/**
			 * @brief ��Ȩ DaisyChain �Ĺ��캯��
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			*/
			DaisyChain(int verCount, int vmin, int vmax)
				:Graph(verCount, false, true, true, true) {
				vertexCount = verCount;
				weightedDaisyMapSwitch = true;
				vmax = vmax;
				vmin = vmin;
			}
			/**
			 * @brief ��������
			*/
			void Generate() {
				clear();
				for(int i = 2; i <= vertexCount ; ++i)
					add(central, i);

				for(int i = vertexCount; i >= 2; --i)
					add(i, i-1);
			}
			/**
			 * @brief ������ɽ��
			*/
			void Output(bool shuffleOutput = true) {
				_output(shuffleOutput);
			}
		protected:
			/**
			 * @brief ��һ���ߣ��Զ����ɱ�Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			void add(int from, int to) {
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
	 * @brief ����ǿ�Կ� SPFA ͼ������ͼ��
	*/
	class AntiSPFA : protected Graph
	{
		public:
			/**
			 * @brief AntiSPFA ���캯��
			 * @param n ����ͼ n ��
			 * @param m ����ͼ m ��
			*/
			AntiSPFA(int n, int m)
				:Graph(n*m, false, true, true, true) {
				this->n = n;
				this->m = m;
				this->vmin = vmin;
				this->vmax = vmax;
			}
			/**
			 * @brief ��ȡ���
			*/
			inline int GetStart() {
				return id[1];
			}
			/**
			 * @brief ��ȡ����
			*/
			inline int GetEdgeCount() {
				return _GetEdgeCount();
			}
			/**
			 * @brief ��������
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			*/
			void Generate(int vmin, int vmax) {
				clear();
				shuffle();
				for(int i = 1; i <= n; ++i) {
					for(int j = 1; j < m; ++j) {
						add_edge(getIndex(i, j), getIndex(i, j+1), rnd.irand(vmin, vmax));
						add_edge(getIndex(i, j+1), getIndex(i, j), rnd.irand(vmin, vmax));
					}
				}
				for(int i = 1; i <= m; ++i) {
					for(int j = 1; j < n; ++j) {
						add_edge(getIndex(j, i), getIndex(j+1, i), 1);
						add_edge(getIndex(j+1, i), getIndex(j, i), 1);
					}
				}
				for(int i = 1; i < n; ++i)
					for(int j = 1; j < m; ++j)
						add_edge(getIndex(i, j), getIndex(i+1, j+1), rnd.irand(vmin, vmax));
			}
			/**
			 * @brief �����Զ�����ģʽ
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			*/
			inline void AutoMode(int vmin, int vmax) {
				std::ofstream &cout = *rout;
				cout << n*m << ' ' << GetEdgeCount() << ' ' << GetStart() << endl;
				Generate(vmin, vmax);
				Output();
			}
			/**
			 * @brief ������ɽ��
			*/
			void Output() {
				_output(false);
			}
		protected:
			/**
			 * @brief ��ȡ��ά����ͼ�ڵ���һά�����е�ʵ�ʴ洢λ��\
			 * @param i �� i ��
			 * @param j �� j ��
			*/
			inline int getIndex(int i, int j) {
				return (i-1)*m+j;
			}
		private:
			int n,m;
			int vmin, vmax;
			Random rnd;
	};

	/**
	 * @brief �������ͼ
	 * @note ����ͼ����ͨ�����������Ի����ر�
	*/
	class RandomGraph : protected Graph
	{
		public:
			/**
			 * @brief �����Ȩͼ�Ĺ��캯��
			 * @param verCount ���ͼ����
			 * @param edgeCount ���ͼ����
			 * @param undirectedMap ����ͼ���أ�Ĭ��Ϊ����ͼ��false��
			 * @warning �����������Ǹ��������������������ϴ�ʱ���ܻ�������ص������½�
			*/
			RandomGraph(int verCount, int edgeCount, bool undirectedMap = false)
				:Graph(verCount, undirectedMap, false, true, true),
				 vertexCount(verCount), edgeCount(edgeCount),
				 weightedMapSwitch(false) {
			}
			/**
			 * @brief �����Ȩͼ�Ĺ��캯��
			 * @param verCount ���ͼ����
			 * @param edgeCoucnt ���ͼ����
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			 * @param undirectedMap ����ͼ���أ�Ĭ��Ϊ����ͼ��false��
			 * @warning �����������Ǹ��������������������ϴ�ʱ���ܻ�������ص������½�
			*/
			RandomGraph( int verCount, int edgeCount, int vmin, int vmax,
			             bool undirectedMap = false)
				:Graph(verCount, undirectedMap, true, true, true),
				 vertexCount(verCount), edgeCount(edgeCount),
				 vmin(vmin), vmax(vmax),
				 weightedMapSwitch(true) {
			}
			/**
			 * @brief ��������
			*/
			void Generate() {
				while(edgeCount) {
					bool u = rnd.irand(1, vertexCount);
					bool v = rnd.irand(1, vertexCount);
					edgeCount -= add(u, v);
				}
			}
			/**
			 * @brief ������ɽ��
			 * @param shuffleOutput ���ҽڵ������Ĭ��Ϊ���ң�true��
			*/
			void Output(bool shuffleOutput = true) {
				_output(shuffleOutput);
			}
		protected:
			/**
			 * @brief ��һ���ߣ��Զ����ɱ�Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			bool add(int from, int to) {
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
	 * @brief ����һ�������޻�ͼ��
	*/
	class DAG : protected Graph
	{
		public:
			/**
			 * @brief ��Ȩ�����޻�ͼ�Ĺ��캯��
			 * @param verCount �����޻�ͼ�ĵ���
			 * @param edgeCount �����޻�ͼ�ı���
			*/
			DAG(int verCount, int edgeCount)
				:Graph(verCount, false, false, true, true),
				 vertexCount(verCount), edgeCount(edgeCount),
				 weightedMapSwitch(false) {
				asserti(edgeCount >= verCount - 1, "Invalid edge count.");
			}
			/**
			 * @brief ��Ȩ�����޻�ͼ�Ĺ��캯��
			 * @param verCount �����޻�ͼ�ĵ���
			 * @param edgeCount �����޻�ͼ�ı���
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			*/
			DAG(int verCount, int edgeCount, int vmin, int vmax)
				:Graph(verCount, false, true, true, true),
				 vertexCount(verCount), edgeCount(edgeCount),
				 vmin(vmin), vmax(vmax),
				 weightedMapSwitch(true) {
				asserti(edgeCount >= verCount - 1, "Invalid edge count.");
			}
			/**
			 * @brief ��������
			*/
			void Generate() {
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
			 * @brief ������ɽ��
			 * @param shuffleOutput ���ҽڵ������Ĭ��Ϊ���ң�true��
			*/
			void Output(bool shuffleOutput = true) {
				_output(shuffleOutput);
			}
		protected:
			/**
			 * @brief ��һ���ߣ��Զ����ɱ�Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			bool add(int from, int to) {
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
	 * @brief ����һ��������
	 * @note ��Ϊ�����вˣ����Ը�������ֻ��ָ��������/kk
	 * @warning �뿪����ע�⣬�÷������ܻ���� BUG
	*/
	class Cactus
	{
		public:
			/**
			 * @brief ��Ȩ Cactus �Ĺ��캯��
			 * @param verCount �ڵ���
			*/
			Cactus(int verCount) {
				vertexCount = verCount;
				weightedMapSwitch = false;
				alloc(verCount);
				clear();
			}
			/**
			 * @brief ��Ȩ Cactus �Ĺ��캯��
			 * @param verCount �ڵ���
			 * @param vmin ��Ȩ��Сֵ
			 * @param vmax ��Ȩ���ֵ
			*/
			Cactus(int verCount, int vmin, int vmax)
				:vmin(vmin), vmax(vmax) {
				vertexCount = verCount;
				weightedMapSwitch = true;
				alloc(verCount+1);
				clear();
			}
			/**
			 * @brief Cactus ����������
			*/
			~Cactus() {
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
			 * @brief ��� Cactus
			*/
			void clear() {
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
			 * @brief ��������
			*/
			void Generate() {
				for(int i = 2; i <= vertexCount; i++) {
					int from = rnd.irand(1, i-1);
					add(from, i);
				}
				TreeDecomposition();
				for(int i = 1; i <= vertexCount; ++i) {
					if(top[i] != -1)
						add(i, top[i]);
				}
			}
			/**
			 * @brief ������ɽ��
			 * @param shuffleOutput ���ҽڵ������Ĭ��Ϊ���ң�true��
			*/
			void Output(bool shuffleOutput = true) {
				_output(shuffleOutput);
			}
		protected:
			void alloc(int verCount) {
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
			 * @brief �����ʷ�
			*/
			void TreeDecomposition() {
				dfs1(1, 1);
				dfs2(1, 1);
			}
			/**
			 * @brief �����ʷ�Ԥ����
			 * @param u ��ǰ�ڵ�
			 * @param f ��ǰ�ڵ�ĸ��ڵ�
			*/
			void dfs1(int u, int f) {
				siz[u] = 1;
				fa[u] = f;
				int maxsize = -1;
				for(int i = head[u]; i; i = next[i]) {
					int v = ver[i];
					if(v == f)
						continue;
					dfs1(v, u);
					siz[u] += siz[v];
					if(maxsize < siz[v]) {
						hson[u] = v;
						maxsize = siz[v];
					}
				}
			}
			/**
			 * @brief �����ʷ�
			 * @param u ��ǰ�ڵ�
			 * @param t ����
			*/
			void dfs2(int u, int t) {
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
			 * @brief ��һ���ߣ��Զ����ɱ�Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			void add(int from, int to) {
				if(weightedMapSwitch)
					add_edge(from, to, rnd.irand(vmin, vmax));
				else
					add_edge(from, to);
			}
			/**
			 * @brief ���һ��������Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			void add_edge(int from, int to) {
				_add_edge(from, to);
				_add_edge(to, from);
			}
			/**
			 * @brief ���һ�������Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			 * @param weight ��Ȩ
			*/
			void add_edge(int from, int to, int weight) {
				_add_edge(from, to, weight);
				_add_edge(to, from, weight);
			}
			/**
			 * @brief ���һ��������Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			*/
			void _add_edge(int from, int to) {
				ver[++tot] = to;
				next[tot] = head[from];
				head[from] = tot;
			}
			/**
			 * @brief ���һ�������Ȩ��
			 * @param from �ߵ����
			 * @param to �ߵ��յ�
			 * @param weight ��Ȩ
			*/
			void _add_edge(int from, int to, int weight) {
				ver[++tot] = to;
				edge[tot] = weight;
				next[tot] = head[from];
				head[from] = tot;
			}
			/**
			 * @brief ���ҽڵ���
			*/
			void shuffle() {
				Random rnd;
				for(int i = 1; i <= vertexCount; ++i) {
					int oth = rnd.irand(1, vertexCount);
					std::swap(id[i], id[oth]);
				}
			}
			/**
			 * @brief ���ͼ��ʵ��
			*/
			void _output(bool shuffleOutput = true) {
				std::ofstream &cout = *rout;
				if(shuffleOutput)
					shuffle();
				for(int i = 1; i <= vertexCount; ++i) {
					for(int j = head[i]; j; j = next[j]) {
						int v = ver[j];
						if(hashtable[ { std::min(i, v), std::max(i, v) }])
							continue;
						cout << id[v] << ' ' << id[i];
						if(weightedMapSwitch)
							cout << ' ' << edge[i];
						cout << std::endl;
						hashtable[ { std::min(i, v), std::max(i, v) }] = true;
					}
				}
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
