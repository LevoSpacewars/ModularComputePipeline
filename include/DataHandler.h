#include <vector>
#include <iostream>
#include <unordered_map>
#include <any>
#include <exception>



// make a struct that takes in any kind of data,
struct DataContainer
{
    std::any data;
    const std::type_info *type;
};


class DataHandler
{
public:
    DataHandler() {} // general constructor

    template <typename T>
    void addData(const std::string &ID, T value)
    {
        static_assert(!std::is_pointer<T>::value, "DataHandler::addData<T>: T must not be a pointer");
        if (data.contains(ID)) throw std::runtime_error("ID: " + ID + " - is already defined"); // Could we make this more verbose? like by integrating this with somekindof logger?
        data[ID] = {std::any(std::move(value)), &typeid(T)};
    }

    template <typename T>
    T * getData(std::string ID)
    {
        static_assert(!std::is_pointer<T>::value, "DataHandler::addData<T>: T must not be a pointer");
        if (data.contains(ID)) return &std::any_cast<T&>(data[ID].data);
        throw std::runtime_error("ID: " + ID + " - is not defined");
    } 

private:

    std::unordered_map<std::string, DataContainer> data;
};