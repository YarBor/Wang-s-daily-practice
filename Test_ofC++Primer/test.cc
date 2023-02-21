#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <stdexcept>
#include <memory>
class strBlob
{
public:
    typedef std::string::size_type size_type;
    strBlob();
    strBlob(std::initializer_list<std::string> a);
    bool empty() { return data->empty(); };
    void back_push(std::string value) { return data->push_back(value); }
    void pop_back();
    std::string &front();
    std::string &back();

private:
    void check(size_type i, const std::string &msg) const;
    std::shared_ptr<std::vector<std::string>> data;
};

strBlob::strBlob()
    : data(std::make_shared<std::vector<std::string>>()){};

strBlob::strBlob(std::initializer_list<std::string> a)
    : data(std::make_shared<std::vector<std::string>>(a)){};

void strBlob::check(size_type i, const std::string &msg) const
{
    if(i<= data->size())
    throw std::out_of_range(msg);
}

std::string & strBlob::front (){
    check(0,"front on empty STRBLOB");
    return data->front();
}

std::string & strBlob::back(){
    check(0,"back on empty STRBLOB");
    return data->back();
}

void strBlob::pop_back(){
    check(0,"popback on empty strBlob");
    return data->pop_back();
}
