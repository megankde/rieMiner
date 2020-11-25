// (c) 2017-2020 Pttn and contributors (https://github.com/Pttn/rieMiner)

#ifndef HEADER_main_hpp
#define HEADER_main_hpp

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include "tools.hpp"

#define versionString	"rieMiner 0.92L"
#define primeTableFile	"PrimeTable32.bin"

extern int DEBUG;
extern std::string confPath;

#define DBG(x) if (DEBUG) {x;};
#define DBG_VERIFY(x) if (DEBUG > 1) { x; };
#define ERRORMSG(message) std::cerr << __func__ << ": " << message << " :| !" << std::endl

static const std::vector<std::pair<std::vector<uint64_t>, std::vector<uint64_t>>> defaultConstellationData = {
	// 1-tuples
	{{0}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	// 2-tuples
	{{0, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	// 3-tuples
	{{0, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2}, {1418575498573ULL, 2118274828903ULL, 4396774576273ULL, 6368171154193ULL, 6953798916913ULL, 27899359258003ULL, 28138953913303ULL, 34460918582323ULL, 40362095929003ULL, 42023308245613ULL, 44058461657443ULL, 61062361183903ULL, 76075560855373ULL, 80114623697803ULL, 84510447435493ULL, 85160397055813ULL}}, // OEIS A213646 (11-tuples)
	// 4-tuples
	{{0, 2, 4, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213646 (12-tuples)
	// 5-tuples
	{{0, 2, 4, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2, 4, 2}, {1091257ULL, 1615837ULL, 1954357ULL, 2822707ULL, 2839927ULL, 3243337ULL, 3400207ULL, 6005887ULL, 6503587ULL, 7187767ULL, 7641367ULL, 8061997ULL, 8741137ULL, 10526557ULL, 11086837ULL, 11664547ULL}}, // OEIS A022013 (6-tuples)
	// 6-tuples
	{{0, 4, 2, 4, 2, 4}, {1091257ULL, 1615837ULL, 1954357ULL, 2822707ULL, 2839927ULL, 3243337ULL, 3400207ULL, 6005887ULL, 6503587ULL, 7187767ULL, 7641367ULL, 8061997ULL, 8741137ULL, 10526557ULL, 11086837ULL, 11664547ULL}}, // OEIS A022013
	// 7-tuples
	{{0, 2, 4, 2, 4, 6, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 6, 4, 2, 4, 2}, {855719ULL, 1146779ULL, 6560999ULL, 7540439ULL, 8573429ULL, 17843459ULL, 19089599ULL, 24001709ULL, 42981929ULL, 43534019ULL, 69156539ULL, 74266259ULL, 79208399ULL, 80427029ULL, 84104549ULL, 87988709ULL}}, // OEIS A022010
	// 8-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2}, {9853497737ULL, 21956291867ULL, 22741837817ULL, 164444511587ULL, 179590045487ULL, 217999764107ULL, 231255798857ULL, 242360943257ULL, 666413245007ULL, 696391309697ULL, 867132039857ULL, 974275568237ULL, 976136848847ULL, 1002263588297ULL, 1086344116367ULL, 1403337211247ULL}}, // OEIS A027570 (10-tuples)
	{{0, 6, 2, 6, 4, 2, 4, 2}, {855713ULL, 1146773ULL, 6560993ULL, 69156533ULL, 74266253ULL, 218033723ULL, 261672773ULL, 302542763ULL, 964669613ULL, 1340301863ULL, 1400533223ULL, 1422475913ULL, 1837160183ULL, 1962038783ULL, 2117861723ULL, 2249363093ULL}}, // OEIS A022013
	// 9-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2, 4}, {9853497737ULL, 21956291867ULL, 22741837817ULL, 164444511587ULL, 179590045487ULL, 217999764107ULL, 231255798857ULL, 242360943257ULL, 666413245007ULL, 696391309697ULL, 867132039857ULL, 974275568237ULL, 976136848847ULL, 1002263588297ULL, 1086344116367ULL, 1403337211247ULL}}, // OEIS A027570 (10-tuples)
	{{0, 4, 2, 4, 6, 2, 6, 4, 2}, {1418575498573ULL, 2118274828903ULL, 4396774576273ULL, 6368171154193ULL, 6953798916913ULL, 27899359258003ULL, 28138953913303ULL, 34460918582323ULL, 40362095929003ULL, 42023308245613ULL, 44058461657443ULL, 61062361183903ULL, 76075560855373ULL, 80114623697803ULL, 84510447435493ULL, 85160397055813ULL}}, // OEIS A213646 (11-tuples)
	{{0, 4, 6, 2, 6, 4, 2, 4, 2}, {855709ULL, 74266249ULL, 964669609ULL, 1422475909ULL, 2117861719ULL, 2558211559ULL, 2873599429ULL, 5766036949ULL, 6568530949ULL, 8076004609ULL, 9853497739ULL, 16394542249ULL, 21171795079ULL, 21956291869ULL, 22741837819ULL, 26486447149ULL}}, // OEIS A022548
	// 10-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {9853497737ULL, 21956291867ULL, 22741837817ULL, 164444511587ULL, 179590045487ULL, 217999764107ULL, 231255798857ULL, 242360943257ULL, 666413245007ULL, 696391309697ULL, 867132039857ULL, 974275568237ULL, 976136848847ULL, 1002263588297ULL, 1086344116367ULL, 1403337211247ULL}}, // OEIS A027570
	// 11-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498573ULL, 2118274828903ULL, 4396774576273ULL, 6368171154193ULL, 6953798916913ULL, 27899359258003ULL, 28138953913303ULL, 34460918582323ULL, 40362095929003ULL, 42023308245613ULL, 44058461657443ULL, 61062361183903ULL, 76075560855373ULL, 80114623697803ULL, 84510447435493ULL, 85160397055813ULL}}, // OEIS A213646
	// 12-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL}}, // OEIS A213645
	{{0, 6, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498567ULL, 27899359257997ULL, 34460918582317ULL, 76075560855367ULL, 186460616596327ULL, 218021188549237ULL, 234280497145537ULL, 282854319391717ULL, 345120905374087ULL, 346117552180627ULL, 604439135284057ULL, 727417501795057ULL, 1041814617748747ULL, 1090754719898917ULL, 1539765965257747ULL, 3152045700948217ULL}} // OEIS A213601
};

struct MinerParameters {
	uint16_t threads, sieveWorkers, tupleLengthMin;
	uint32_t primorialNumber, primeTableLimit;
	uint32_t sieveBits, sieveSize, sieveWords, sieveIterations;
	std::vector<uint64_t> pattern, primorialOffsets;
	
	MinerParameters() :
		threads(0), sieveWorkers(0), tupleLengthMin(0),
		primorialNumber(0), primeTableLimit(0),
		sieveBits(0), sieveSize(0), sieveWords(0), sieveIterations(0),
		pattern{}, primorialOffsets{} {}
};

class Options {
	MinerParameters _minerParameters;
	std::string _host, _username, _password, _mode, _payoutAddress, _secret, _tuplesFile;
	uint32_t _filePrimeTableLimit;
	uint16_t _debug, _port, _threads, _donate;
	double _refreshInterval, _difficulty, _benchmarkBlockInterval, _benchmarkTimeLimit;
	uint64_t _benchmarkPrimeCountLimit;
	std::vector<std::string> _rules;
	
	void _parseLine(std::string, std::string&, std::string&) const;
	void _stopConfig() const;
	
	public:
	Options() : // Default options: Standard Benchmark with 8 threads
		_host("127.0.0.1"),
		_username(""),
		_password(""),
		_mode("Benchmark"),
		_payoutAddress("ric1qpttn5u8u9470za84kt4y0lzz4zllzm4pyzhuge"),
		_secret("/rM0.92L/"),
		_tuplesFile("Tuples.txt"),
		_filePrimeTableLimit(0),
		_debug(0),
		_port(28332),
		_donate(2),
		_refreshInterval(30.),
		_difficulty(1024.),
		_benchmarkBlockInterval(150.),
		_benchmarkTimeLimit(86400.),
		_benchmarkPrimeCountLimit(1000000),
		_rules{"segwit"} {}
	
	void askConf();
	void loadConf();
	
	MinerParameters minerParameters() const {return _minerParameters;}
	std::string mode() const {return _mode;}
	std::string host() const {return _host;}
	uint16_t port() const {return _port;}
	std::string username() const {return _username;}
	std::string password() const {return _password;}
	std::string payoutAddress() const {return _payoutAddress;}
	std::string secret() const {return _secret;}
	std::string tuplesFile() const {return _tuplesFile;}
	uint32_t filePrimeTableLimit() const {return _filePrimeTableLimit;}
	uint16_t donate() const {return _donate;}
	double refreshInterval() const {return _refreshInterval;}
	double difficulty() const {return _difficulty;}
	double benchmarkBlockInterval() const {return _benchmarkBlockInterval;}
	double benchmarkTimeLimit() const {return _benchmarkTimeLimit;}
	uint64_t benchmarkPrimeCountLimit() const {return _benchmarkPrimeCountLimit;}
	std::vector<std::string> rules() const {return _rules;}
};

#endif
