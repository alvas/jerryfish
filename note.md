# Jerryfish

## logger


LogFormatter::init() create LogItem

LogEventWrap::~LogEventWrap() -> LogEvent::getLogger() -> Logger::log() -> LogAppender::log() -> LogFormatter::format() -> FormatItem::format() -> ostream << LogEvent::getContent()

LoggerManager() adds m_root
LoggerManager()::getLogger(name) add logger to m_loggers

Logger() set m_formatter
Logger::toYamlString() 

LogIniter() -> getLogDefines() -> Config::Lookup() create LogDefine
LogIniter() -> getLogDefines() -> ConfigVar::addListener() add m_cbs

Config::LoadFromYaml() -> LookupBase() string -> LogDefine

Config::LoadFromYaml() -> ConfigVar::fromString() -> ConfigVar::setValue() call ConfigVar m_cbs to invoke listener lambda

Config::LoadFromYaml() -> ConfigVar::fromString() -> ConfigVar::setValue() -> LogIniter()::lambada() -> LogDefine()() -> LogEventWrap::~LogEventWrap()



	Event
      |-- m_elapse
      |-- m_fiberId
      |-- m_file
      |-- m_level
      |-- m_line
      |-- m_logger
      |-- m_ss
      |-- m_threadId
      |-- m_threadName
      |-- m_time	
      
    EventWrapper
      |-- m_event
      
    FormatItem
      
    LogFormatter
      |-- m_error
      |-- m_iterm
      |-- m_pattern
      
    Level
    
	LogAppender
     |-- m_formatter
     |-- m_level

    Logger
      |-- m_appenders
      |-- m_formatter
      |-- m_level
      |-- m_name
      |-- m_root

	LoggerManager
      |--m_loggers
      |--m_root
      

## config

	Config
    
    ConfigVar
      |-- m_cbs
      |-- m_val
      
    ConfigVarBase
      |-- m_description
      |-- m_name
      
    LogDefine
      |-- name
      |-- level
      |-- formatter
      |-- appenders

    LogAppenderDefine
      |-- type
      |-- level
      |-- formatter
      |-- file


#Thread

    Noncopyable


    Semaphore
      |-- m_semaphore

    ScopedLockImpl
      |--m_mutex
      |--m_locked

    ReadScopedLockImpl
      |--m_mutex
      |--m_locked


    WriteScopedLockImpl
      |--m_mutex
      |--m_locked

    Mutex
      |--m_mutex


    NullMutex

    RWMutex
      |--m_lock

    NullRWMutex


    SpinLock
      |--m_mutex

    CASlock
      |--m_mutex

    FiberSemaphore
      |--m_mutex
      |--m_waiters
      |--m_concurrency


    Thread
      |--m_id
      |--m_thread
      |--m_cb
      |--m_name
      |--m_semaphore




