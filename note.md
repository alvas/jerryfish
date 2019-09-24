# Jerryfish

## logger


	LogAppender
     |-- m_formatter
     |-- m_level

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
      
    Formatter
      |-- m_error
      |-- m_iterm
      |-- m_pattern
      
    FormatItem
      
    Level
    
    Logger
      |-- m_appenders
      |-- m_formatter
      |-- m_level
      |-- m_name
      |-- m_root

	Manager
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
      

