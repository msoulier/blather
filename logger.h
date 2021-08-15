/*
    http://www.but-i-digress.ca/a-working-logger.html
*/

#include <stdio.h>
#include <ostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#define LOGLEVEL_TRACE 0
#define LOGLEVEL_DEBUG 10
#define LOGLEVEL_INFO 20
#define LOGLEVEL_WARN 30
#define LOGLEVEL_ERROR 40

extern boost::thread_specific_ptr<std::stringstream> tls_buffer;

class MLoggerHandler
{
public:
    MLoggerHandler(boost::mutex& mutex,
                std::ostream& ostream,
                int threshold,
                std::string prefix);
    ~MLoggerHandler();
    void setLevel(int level);

    template <class T>
    // For handling << from any object.
    MLoggerHandler& operator <<(T input) {
        // Only log if the level is set above our threshold.
        if (m_threshold >= m_level) {
            if (tls_buffer.get() == NULL) {
                tls_buffer.reset(new std::stringstream());
            }
            if (tls_buffer->str().length() == 0) {
                *tls_buffer << localDateTime() << " " << m_prefix << ": " << input;
            }
            else {
                *tls_buffer << input;
            }
        }
        return *this;
    }
    // For handling std::endl
    std::ostream& operator <<(std::ostream& (*f)(std::ostream&)) {
        // Only log if the level is set above our threshold.
        if (m_threshold >= m_level) {
            boost::lock_guard<boost::mutex> lock{m_mutex};
            // Flush the buffer
            m_ostream << tls_buffer->str();
            f(m_ostream);
            // Clear the buffer
            tls_buffer->str("");
        }
        return m_ostream;
    }
private:
    // A mutex passed in from the main logger for synchronization.
    boost::mutex& m_mutex;
    // The logging level.
    int m_level;
    // The output stream.
    std::ostream& m_ostream;
    // The threshold for logging for this handler.
    int m_threshold;
    // The string prefix for logging.
    std::string m_prefix;
    // Return the current date and time as a localized string.
    const std::string localDateTime();
};

/*
* The MLogger (Mike-logger) is a thread-safe C++ logger using the iostream
* operators.  To use it, you must invoke a logging level handler which will
* return an MLoggerHandler reference, and then terminate your line with
* std::endl to ensure that the buffer is flushed and the line terminated with a
* newline.
*/
class MLogger
{
public:
    MLogger();
    MLogger(std::string name);
    ~MLogger();
    // Set the current logging level
    void setLevel(int level);
    // Get the current logging level
    int getLevel();
    // Convenience methods for trace level log.
    MLoggerHandler& trace();
    // Convenience methods for debug level log
    MLoggerHandler& debug();
    // Convenience methods for info level log
    MLoggerHandler& info();
    // Convenience methods for warning level log
    MLoggerHandler& warn();
    // Convenicence methods for error level log
    MLoggerHandler& error();
private:
    // The logger name.
    std::string m_name;
    // The current log level.
    int m_level;
    // The output stream for the logger.
    std::ostream& m_ostream;
    // The mutex used for synchronization.
    boost::mutex m_mutex;
    // Trace handler
    MLoggerHandler m_trace_handler;
    // Debug handler
    MLoggerHandler m_debug_handler;
    // Info handler
    MLoggerHandler m_info_handler;
    // Warn handler
    MLoggerHandler m_warn_handler;
    // Error handler
    MLoggerHandler m_error_handler;
};
