#include <sstream>
#include <string.h>
#include <string>

#include "patternlayout.h"
#include "exception.h"

namespace yf {
struct LevelFormatItem final : public PatternLayout::FormatItem {
    LevelFormatItem() = default;
    virtual ~LevelFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << LogLevel::getLevelName(event._level);
    }
};

struct ThreadFormatItem final : public PatternLayout::FormatItem {
    ThreadFormatItem() = default;
    virtual ~ThreadFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << event._threadId;
    }
};

struct FiberFormatItem final : public PatternLayout::FormatItem {
    FiberFormatItem() = default;
    virtual ~FiberFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << event._fiberId;
    }
};

struct MessageFormatItem final : public PatternLayout::FormatItem {
    MessageFormatItem() = default;
    virtual ~MessageFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << event._message;
    }
};

struct CategoryFormatItem final : public PatternLayout::FormatItem {
    CategoryFormatItem() = default;
    virtual ~CategoryFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << event._categoryName;
    }
};

struct DateTimeFormatItem final : public PatternLayout::FormatItem {
    DateTimeFormatItem(const std::string &format)
        : FormatItem(),
          _format(format) {
        if (_format.empty()) {
            _format = "%Y-%m-%d %H:%M:%S";
        }
    }
    virtual ~DateTimeFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        _second = event._timeStamp.getSecond();
        ::localtime_r((const time_t *)&_second, &_t);
        ::strftime(_buff, BUFF_SIZE, _format.c_str(), &_t);
        ss << _buff;
    }
    constexpr static int BUFF_SIZE = 32;
    char _buff[BUFF_SIZE];
    struct tm _t;
    uint32_t _second;
    std::string _format;
};

struct SecondFormatItem : public PatternLayout::FormatItem {
    SecondFormatItem() = default;
    virtual ~SecondFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << event._timeStamp.getSecond();
    }
};

struct RunningFormatItem : public PatternLayout::FormatItem {
    RunningFormatItem() = default;
    virtual ~RunningFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        uint64_t second = event._timeStamp.getSecond() - TimeStamp::getStartTime().getSecond();
        ss << second;
    }
};

struct StringFormatItem : public PatternLayout::FormatItem {
    StringFormatItem(const std::string &str)
        : _str(str) {
    }
    virtual ~StringFormatItem() = default;
    virtual void format(const LogEvent &event, std::ostream &ss) override {
        ss << _str;
    }
    std::string _str;
};

PatternLayout::PatternLayout()
    : _format("%d{%Y-%m-%d %H:%M:%S} [%c][%l] <%t-%f>: %m%n") {
    setFormat(_format);
}

void PatternLayout::setFormat(const std::string &format) {
    clearFormat();
    std::istringstream format_stream(format);
    char c;
    std::string format_str = "";
    while (format_stream.get(c)) {
        if (c == '%') {
            if (format_stream.get(c)) {
                FormatItem *item = nullptr;
                std::size_t index = format_stream.tellg();
                char c1;
                std::string item_format = "";
                if (format_stream.get(c1)) {
                    if (c1 == '{') {
                        while (format_stream.get(c1) && c1 != '}') {
                            item_format += c1;
                        }
                    } else {
                        format_stream.putback(c1);
                    }
                }
                switch (c)
                {
#define XX(c, type, ...)              \
    case c:                           \
        item = new type(__VA_ARGS__); \
        break;
    XX('l', LevelFormatItem)
    XX('t', ThreadFormatItem)
    XX('f', FiberFormatItem)
    XX('m', MessageFormatItem)
    XX('c', CategoryFormatItem)
    XX('d', DateTimeFormatItem, item_format)
    XX('s', SecondFormatItem)
    XX('r', RunningFormatItem)
#undef XX
                case '%':
                    format_str += c;
                    break;
                case 'n':
                    format_str += '\n';
                    break;
                default:
                    {
                        std::string logic = "no log format exists (%";
                        throw LogicError(logic + c +") index: " + std::to_string(index));
                    }
                    break;
                }
                if (item) {
                    if (!format_str.empty()) {
                        _formats.emplace_back(new StringFormatItem(format_str));
                        format_str.clear();
                    }
                    _formats.emplace_back(item);
                }
            } else {
                throw LogicError("the length of the custom formatted log is incorrect");
            }
        } else {
            format_str += c;
        }
    }
    if (!format_str.empty()) {
        _formats.emplace_back(new StringFormatItem(format_str));
    }
    _format = format;
}

void PatternLayout::clearFormat() {
    _formats.clear();
}

void PatternLayout::format(const LogEvent &event, std::ostream &ss) {
    for (const auto &i : _formats) {
        i->format(event, ss);
    }
}

PatternLayout::~PatternLayout() {
    clearFormat();
}
}