#include "Logger.hpp"
#include <unistd.h>
#include <memory>

using namespace LogModule;

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();
    // ENABLE_FILE_LOG_STRATEGY();
    LOG(LogLevel::DEBUG) << "CONSOLE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::INFO) << "CONSOLE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::WARNING) << "CONSOLE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::ERROR) << "CONSOLE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::FATAL) << "CONSOLE hello world" << " hello bit, " << 3.14 << " C";

    ENABLE_FILE_LOG_STRATEGY();
    LOG(LogLevel::DEBUG) << "FILE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::INFO) << "FILE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::WARNING) << "FILE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::ERROR) << "FILE hello world" << " hello bit, " << 3.14 << " C";
    LOG(LogLevel::FATAL) << "FILE hello world" << " hello bit, " << 3.14 << " C";

    // #define LOG(level) logger(level,__FILE__,__LINE__)

    // logger(level,__FILE__,__LINE__) << "hello world";

    // LOG(LogLevel::DEBUG) << "hello world";
    // LOG(LogLevel::DEBUG) << "hello world";
    // LOG(LogLevel::DEBUG) << "hello world";
    // LOG(LogLevel::DEBUG) << "hello world";
    // LOG(LogLevel::DEBUG) << "hello world";
    // LOG(LogLevel::INFO) << "hello world";

    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;
    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;
    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;
    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;
    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;
    // std::cout<< __FILE__ << " : " << __LINE__ << std::endl;

    // std::string message = "consule hello log, hello world!";
    // // 基类指针指向子类对象
    // std::unique_ptr<LogStrategy> strategy = std::make_unique<ConsoleLogStrategy>();

    // strategy->SyncLog(message);
    // strategy->SyncLog(message);
    // strategy->SyncLog(message);
    // strategy->SyncLog(message);
    // strategy->SyncLog(message);
    // strategy->SyncLog(message); 

    // std::string message1 = "file hello log, hello world!";
    // strategy = std::make_unique<FileLogStrategy>();

    // strategy->SyncLog(message1);
    // strategy->SyncLog(message1);
    // strategy->SyncLog(message1);
    // strategy->SyncLog(message1);
    // strategy->SyncLog(message1);

    // std::cout<<LogLevel2String(LogLevel::DEBUG)<<std::endl;
    // std::cout<<LogLevel2String(LogLevel::WARNING)<<std::endl;
    // std::cout<<LogLevel2String(LogLevel::FATAL)<<std::endl;
    // std::cout<<LogLevel2String(LogLevel::ERROR)<<std::endl;
    // std::cout<<LogLevel2String(LogLevel::INFO)<<std::endl; 

    // std::cout << LogModule::LogLevel::DEBUG <<std::endl;
    // std::cout << LogModule::LogLevel::ERROR <<std::endl;
    // std::cout << LogModule::LogLevel::FATAL <<std::endl;
    // std::cout << LogModule::LogLevel::WARNING <<std::endl;
    // std::cout << LogModule::LogLevel::INFO <<std::endl;

    // std::string currtime = LogModule::GetTimeStamp();
    // std::cout << currtime << std::endl;
    // sleep(1);
    // currtime = LogModule::GetTimeStamp();
    // std::cout << currtime << std::endl;
    // sleep(1);
    // currtime = LogModule::GetTimeStamp();
    // std::cout << currtime << std::endl;
    // sleep(1);
    // currtime = LogModule::GetTimeStamp();
    // std::cout << currtime << std::endl;
    // sleep(1);
    // currtime = LogModule::GetTimeStamp();
    // std::cout << currtime << std::endl;
    // sleep(1);
    return 0;
}