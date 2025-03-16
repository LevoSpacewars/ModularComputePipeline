#include <vector>
#include <iostream>
#include <unordered_map>
#include <any>
#include <exception>
#include <memory>

// make a struct that takes in any kind of data,

class FutureDataPtr;




/// @brief A class that encapsulates a future data value, allowing for deferred initialization and retrieval.
/// It uses std::any to store the value, enabling it to hold any type of data.
/// @note This class is not thread-safe and should be used in a single-threaded context or with appropriate synchronization.
/// @note The class provides methods to check if the data is ready, set a value, and retrieve the value.
/// @note The class also provides a pointer wrapper (FutureDataPtr) for easier management of the FutureData object.
/// @note The class uses static assertions to ensure that the type T is not a pointer and is convertible to std::any.
class FutureData{


    public:


    /// @brief This method checks if the FutureData object has been fulfilled (i.e., if it contains a value).
    /// @return Returns true if the FutureData object has been fulfilled, false otherwise.
    bool isReady(){
        return this->ptr.get();
    }
    

    
    /// @brief This method retrieves the value stored in the FutureData object.
    /// @note It throws a runtime error if the FutureData object is not fulfilled (i.e., if it does not contain a value).
    /// @tparam T 
    /// @note T must not be a pointer type and must be convertible to std::any.
    /// @return  Returns a pointer to the value stored in the FutureData object.
    /// @throws std::runtime_error if the FutureData object is not fulfilled.
    /// @throws std::bad_any_cast if the stored value cannot be cast to the requested type T.
    /// @throws std::bad_cast if the stored value cannot be cast to the requested type T.
    /// @example
    /// @code
    /// FutureData fd;
    /// fd.setValue(42);
    /// int *value = fd.getValue<int>(); // returns a pointer to the stored value
    /// @endcode
    template<typename T>
    T * getValue(){
        static_assert(!std::is_pointer<T>::value, "FutureData::getValue<T>: T must not be a pointer");

        if (!this->isReady()) throw std::runtime_error("FutureData::getValue<T>: not fulfilled");        
        return &std::any_cast<T&>(*this->ptr.get());
    }


    
    /// @brief This method sets the value of the FutureData object.
    /// @note It takes a shared pointer to an std::any object, which allows for deferred initialization of the value.
    /// @tparam T 
    /// @note T must not be a pointer type and must be convertible to std::any.
    /// @param p 
    /// @throws std::runtime_error if the shared pointer is null or if the FutureData object has already been fulfilled.
    /// @example
    /// @code
    /// FutureData fd;
    /// std::shared_ptr<std::any> p = std::make_shared<std::any>(42);
    /// fd.setValue(std::move(p)); // sets the value of the FutureData object
    /// @endcode
    template<typename T>
    void setValue(std::shared_ptr<T> && p){
        static_assert(!std::is_pointer<T>::value, "PromisedData::fulfill<T>: T must not be a pointer");
        static_assert(std::is_convertible<T, std::any>::value, "PromisedData::fulfill<T>: T must be convertible to std::any");
        if (!p) throw std::runtime_error("PromisedData::fulfill<T>: p is null");
        if (this->isReady()) throw std::runtime_error("PromisedData::fulfill<T>: already fulfilled");


        this->ptr = std::dynamic_pointer_cast<std::any>(std::move(p));
    }


    
    /**
     * @brief Sets the value by forwarding the provided value.
     * 
     * This function takes an rvalue reference to a value of type T, 
     * creates a shared pointer to a std::any containing the value, 
     * and then calls another setValue function with this shared pointer.
     * 
     * @tparam T The type of the value to be set.
     * @param value An rvalue reference to the value to be set.
     * @example
     * @code
     * FutureData fd;
     * fd.setValue(42); // sets the value of the FutureData object
     * @endcode
     * @note This function is useful for directly setting the value without needing to create a shared pointer explicitly.
     * @throws std::runtime_error if the FutureData object has already been fulfilled.
     * @throws std::bad_any_cast if the stored value cannot be cast to the requested type T.
     * @throws std::bad_cast if the stored value cannot be cast to the requested type T.
     */
    template<typename T>
    void setValue(T && value){
        this->setValue(std::make_shared<std::any>(std::move(value)));
    }

    
    FutureData() : ptr(nullptr) {} // default constructor
    ~FutureData() { ptr = nullptr; } // destructor

    private:

    const std::type_info *type;
    std::shared_ptr<std::any> ptr = nullptr;

};



class FutureDataPtr
{

    public:
    FutureDataPtr(FutureData * p) : ptr(p) {} // constructor
    FutureDataPtr() : ptr(nullptr) {} // default constructor
    FutureDataPtr(const FutureDataPtr & other) : ptr(other.ptr) {} // copy constructor
    FutureDataPtr(FutureDataPtr && other) : ptr(other.ptr) { other.ptr = nullptr; } // move constructor
    ~FutureDataPtr() { ptr = nullptr; } // destructor
    FutureDataPtr & operator=(const FutureDataPtr & other) { // copy assignment
        if (this != &other) {
            ptr = other.ptr;
        }
        return *this;
    }
    FutureDataPtr & operator=(FutureDataPtr && other) { // move assignment
        if (this != &other) {
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    /// @brief This method checks if the FutureData object pointed to by the FutureDataPtr is ready (i.e., if it has been fulfilled).
    /// @return Returns true if the FutureData object is ready, false otherwise.
    /// @note It checks if the pointer is not null and if the FutureData object is ready.
    bool isReady() const { return ptr != nullptr && ptr->isReady(); } // check if the pointer is valid and fulfilled



    /// @brief This method retrieves the value stored in the FutureData object pointed to by the FutureDataPtr.
    /// @tparam T
    /// @note T must not be a pointer type and must be convertible to std::any.
    /// @return Returns a pointer to the value stored in the FutureData object.
    /// @throws std::runtime_error if the FutureData object is not fulfilled.
    /// @throws std::bad_any_cast if the stored value cannot be cast to the requested type T.
    template<typename T>
    T * getValue() const { // get the data as
        if (!isReady()) throw std::runtime_error("FutureDataPtr::getValue<T>: Data is not ready yet");
        return ptr->getValue<T>(); // call the get method of the FutureData object
    }

    private:
    FutureData * ptr; // pointer to the FutureData object

};




/// @brief A class that manages a collection of FutureData objects, allowing for deferred initialization and retrieval of data by ID.
/// @note This class provides methods to add, retrieve, check readiness, and remove data associated with a unique ID.
/// @note It uses an unordered_map to store FutureData objects, allowing for efficient access and management.
/// @note The class also provides a method to request a FutureDataPtr for a specific ID, which can be used to manage the FutureData object.
/// @note The class uses static assertions to ensure that the type T is not a pointer and is convertible to std::any.
class DataHandler
{
public:
    DataHandler() {} // general constructor
    ~DataHandler() {} // destructor

    /// @brief Adds data to the DataHandler with a unique ID.
    /// @tparam T
    /// @note T must not be a pointer type and must be convertible to std::any.
    /// @param ID The unique identifier for the data.
    /// @param value The value to be stored in the FutureData object.
    /// @throws std::runtime_error if the ID already exists in the DataHandler.
    /// @example
    /// @code
    /// DataHandler dh;
    /// dh.addData("myData", 42); // adds data with ID "myData"
    /// @endcode
    template <typename T>
    void addData(const std::string &ID, T && value)
    {
        static_assert(!std::is_pointer<T>::value, "DataHandler::addData<T>: T must not be a pointer");
        if (data.contains(ID)) throw std::runtime_error("ID: " + ID + " - is already defined"); // Could we make this more verbose? like by integrating this with somekindof logger?
        data[ID] = FutureData(); // store the data in the map
        data[ID].setValue<T>(std::move(value)); // set the value
    }

    /// @brief Retrieves valid data associated with a unique ID from the DataHandler.
    /// @tparam T
    /// @note T must not be a pointer type and must be convertible to std::any.
    /// @param ID The unique identifier for the data.
    /// @return Returns a pointer to the valid value stored in the FutureData object associated with the ID.
    /// @throws std::runtime_error if the ID does not exist in the DataHandler or if the data is not ready.
    /// @example
    /// @code
    /// DataHandler dh;
    /// dh.addData("myData", 42); // adds data with ID "myData"
    /// int *value = dh.getData<int>("myData"); // retrieves the value associated with "myData"
    /// @endcode
    /// @note This method checks if the data is ready before retrieving it.
    template <typename T>
    T * getData(std::string ID)
    {
        static_assert(!std::is_pointer<T>::value, "DataHandler::addData<T>: T must not be a pointer");
        if (data.contains(ID)) return data[ID].getValue<T>(); // get the data from the map
        throw std::runtime_error("ID: " + ID + " - is not defined");
    }

    

    /// @brief Checks if the data associated with a unique ID is ready (i.e., if it has been fulfilled).
    /// @param ID The unique identifier for the data.
    /// @return Returns true if the data is ready, false otherwise.
    bool isDataReady(std::string ID)
    {
        if (data.contains(ID)) return data[ID].isReady(); // check if the data is ready
        return false;
    }
    

    /// @brief Requests a FutureDataPtr for a specific ID, allowing for deferred initialization and retrieval of data.
    /// @param ID The unique identifier for the data.
    /// @return Returns a FutureDataPtr pointing to the FutureData object associated with the ID.
    /// @note If the ID does not exist, a new FutureData object is created and associated with the ID.

    FutureDataPtr requestFutureData(std::string ID) // NOTE: THIS MAY HAVE AN ISSUE WITH THE FUTUREDATA TYPE VARIABLE BEING SET AT A LATER TIME SINCE 
    {
        if (data.contains(ID)) return FutureDataPtr(&data[ID]); // return a pointer to the data
        data[ID] = FutureData(); // create a new FutureData if it doesn't exist
        return FutureDataPtr(&data[ID]); // return a pointer to the new FutureData
    }

    void removeData(std::string ID)
    {
        if (data.contains(ID)) {
            data.erase(ID); // remove the data from the map
        } else {
            throw std::runtime_error("ID: " + ID + " - is not defined");
        }
    }

private:

    std::unordered_map<std::string, FutureData> data;
};