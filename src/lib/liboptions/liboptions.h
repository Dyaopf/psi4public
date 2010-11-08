#ifndef _psi_src_lib_liboptions_liboptions_hpp
#define _psi_src_lib_liboptions_liboptions_hpp

#include <iostream>
#include <vector>
#include <map>
#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <assert.h>

#include <exception.h>
#include <libutil/libutil.h> // Needed for Ref counting, string splitting, and conversions
#include <boost/shared_ptr.hpp>

namespace psi {
  extern FILE *outfile;

  class DataTypeException : public PsiException
  {
  public:
    DataTypeException(const std::string& message) : PSIEXCEPTION(message) { }
  };

  class IndexException : public PsiException
  {
  public:
    IndexException(const std::string& message) : PSIEXCEPTION("unable to find index " + message) { }
  };

  class DuplicateKeyException : public PsiException
  {
  public:
    DuplicateKeyException() : PSIEXCEPTION("duplicate key found") { }
  };

  class NotImplementedException : public PsiException
  {
  public:
    NotImplementedException(const std::string& message) : PSIEXCEPTION(message + " function not implemented") { }
  };

  class OptionsException : public PsiException
  {
  public:
    OptionsException(const std::string& message) : PSIEXCEPTION("Options Exception: " + message) { }
  };

  class Data;
  class DataType
  {
    bool changed_;
  public:
    DataType() : changed_(false) { }
    virtual ~DataType() { }

    bool has_changed() const {
      return changed_;
    }
    void changed() {
      changed_ = true;
    }

    void to_upper(std::string& str) {
      std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    virtual std::string type() const {
      return std::string("unknown");
    }

    virtual bool is_array() const {
      return false;
    }
    virtual unsigned int size() const {
      throw NotImplementedException("size()");
    }
    virtual void add(DataType *) {
      throw NotImplementedException("add(DataType*)");
    }
    virtual void add(std::string, DataType*) {
      throw NotImplementedException("add(std::string, DataType*)");
    }
    virtual void add(bool) {
      throw NotImplementedException("add(bool)");
    }
    virtual void add(int) {
      throw NotImplementedException("add(int)");
    }
    virtual void add(double) {
      throw NotImplementedException("add(double)");
    }
    virtual void add(std::string, bool) {
      throw NotImplementedException("add(std::string, bool)");
    }
    virtual void add(std::string, std::string) {
      throw NotImplementedException("add(std::string, std::string)");
    }
    virtual void add(std::string, int) {
      throw NotImplementedException("add(std::string, int)");
    }
    virtual void add(std::string, double) {
      throw NotImplementedException("add(std::string, double)");
    }
    virtual void add(std::string, std::string, std::string) {
      throw NotImplementedException("add(std::string, std::string, std::string)");
    }

    virtual bool exists(std::string) {
      throw NotImplementedException("exists(std::string)");
    }

    virtual std::string to_string() const {
      throw DataTypeException("don't know how to convert to a string");
    };
    virtual int to_integer() const {
      throw DataTypeException("don't know how to convert to an integer");
    }
    virtual double to_double() const {
      throw DataTypeException("don't know how to convert to a double");
    }

    virtual void assign(bool) {
      throw DataTypeException("assign(bool) failure");
    }
    virtual void assign(int) {
      throw DataTypeException("assign(int) failure");
    }
    virtual void assign(double) {
      throw DataTypeException("assign(double) failure");
    }
    virtual void assign(std::string) {
      throw DataTypeException("assign(std:string) failure");
    }

    virtual void reset() {
      throw DataTypeException("reset() failure");
    }

    virtual Data& operator[](std::string) {
      throw NotImplementedException("Data& [string]");
    }
    virtual Data& operator[](unsigned int) {
      throw NotImplementedException("Data& [unsigned int]");
    }
  };

  class BooleanDataType : public DataType
  {
    bool boolean_;
  public:
    BooleanDataType() : DataType(), boolean_(false) { }
    BooleanDataType(bool b) : DataType(), boolean_(b) { }
    virtual ~BooleanDataType() { }

    virtual std::string type() const {
      return std::string("boolean");
    }

    virtual std::string to_string() const {
      std::string ret;
      if (boolean_)
        ret = "TRUE";
      else
        ret = "FALSE";
      return ret;
    }
    virtual int to_integer() const {
      return static_cast<int>(boolean_);
    }
    virtual double to_double() const {
      return static_cast<double>(boolean_);
    }

    virtual void assign(bool b) {
      changed();
      boolean_ = b;
    }
    virtual void assign(int i) {
      assign(static_cast<bool>(i));
    }
    virtual void assign(double d) {
      assign(static_cast<bool>(d));
    }
    virtual void assign(std::string s) {
      assign(static_cast<bool>(std::strtod(s.c_str(), NULL)));
    }
  };

  class IntDataType : public DataType
  {
    int integer_;
  public:
    IntDataType() : DataType(), integer_(0) { }
    IntDataType(int i) : DataType(), integer_(i) { }
    virtual ~IntDataType() { }

    virtual std::string type() const {
      return std::string("int");
    }

    virtual std::string to_string() const {
      std::stringstream strm;
      strm << integer_;
      return strm.str();
    }
    virtual int to_integer() const {
      return integer_;
    }
    virtual double to_double() const {
      return static_cast<double>(integer_);
    }

    virtual void assign(bool b) {
      assign(static_cast<int>(b));
    }
    virtual void assign(int i) {
      changed();
      integer_ = i;
    }
    virtual void assign(double d) {
      assign(static_cast<int>(d));
    }
    virtual void assign(std::string s) {
      assign(static_cast<int>(std::strtod(s.c_str(), NULL)));
    }
  };

  class DoubleDataType : public DataType
  {
    double double_;
  public:
    DoubleDataType() : DataType(), double_(0.0) { }
    DoubleDataType(double d) : DataType(), double_(d) { }
    virtual ~DoubleDataType() { }

    virtual std::string type() const {
      return std::string("double");
    }

    virtual std::string to_string() const {
      std::stringstream strm;
      strm << double_;
      return strm.str();
    }
    virtual int to_integer() const {
      return static_cast<int>(double_);
    }
    virtual double to_double() const {
      return double_;
    }

    virtual void assign(bool b) {
      assign(static_cast<double>(b));
    }
    virtual void assign(int i) {
      assign(static_cast<double>(i));
    }
    virtual void assign(double d) {
      changed();
      double_ = d;
    }
    virtual void assign(std::string s) {
      assign(std::strtod(s.c_str(), NULL));
    }
  };

  class StringDataType : public DataType
  {
    std::string str_;
    std::vector<std::string> choices_;
  public:
    StringDataType() : DataType(), str_(), choices_() { }
    StringDataType(std::string s) : DataType(), str_(s), choices_() { to_upper(str_); }
    StringDataType(std::string s, std::string c) : DataType(), str_(s), choices_() { to_upper(str_); to_upper(c); choices_ = split(c); }
    virtual ~StringDataType() { }

    virtual std::string type() const {
      return std::string("string");
    }

    virtual std::string to_string() const {
      return str_;
    }
    virtual int to_integer() const {
      return static_cast<int>(std::strtod(str_.c_str(), NULL));
    }
    virtual double to_double() const {
      return std::strtod(str_.c_str(), NULL);
    }

    virtual void assign(bool b) {
      if (b)
        assign("TRUE");
      else
        assign("FALSE");
    }
    virtual void assign(int i) {
      std::stringstream strm;
      strm << i;
      assign(strm.str());
    }
    virtual void assign(double d) {
      std::stringstream strm;
      strm << d;
      assign(strm.str());
    }
    virtual void assign(std::string s) {
      to_upper(s);
      if (choices_.size() > 0) {
        bool wrong_input = true;
        for (unsigned int i=0; i<choices_.size(); ++i)
          if (s == choices_[i])
            wrong_input = false;
        if (wrong_input)
          throw DataTypeException(s + " is not a valid choice");
        changed();
        str_ = s;
      }
      else {
        changed();
        str_ = s;
      }
    }
  };

  class IStringDataType : public DataType
  {
    std::string str_;
    std::vector<std::string> choices_;
  public:
    IStringDataType() : DataType(), str_(), choices_() { }
    IStringDataType(std::string s) : DataType(), str_(s), choices_() { }
    IStringDataType(std::string s, std::string c) : DataType(), str_(s), choices_() { choices_ = split(c); }
    virtual ~IStringDataType() { }

    virtual std::string type() const {
      return std::string("string");
    }

    virtual std::string to_string() const {
      return str_;
    }
    virtual int to_integer() const {
      return static_cast<int>(std::strtod(str_.c_str(), NULL));
    }
    virtual double to_double() const {
      return std::strtod(str_.c_str(), NULL);
    }

    virtual void assign(bool b) {
      if (b)
        assign("TRUE");
      else
        assign("FALSE");
    }
    virtual void assign(int i) {
      std::stringstream strm;
      strm << i;
      assign(strm.str());
    }
    virtual void assign(double d) {
      std::stringstream strm;
      strm << d;
      assign(strm.str());
    }
    virtual void assign(std::string s) {
      if (choices_.size() > 0) {
        bool wrong_input = true;
        for (unsigned int i=0; i<choices_.size(); ++i)
          if (s == choices_[i])
            wrong_input = false;
        if (wrong_input)
          throw DataTypeException(s + " is not a valid choice");
        changed();
        str_ = s;
      }
      else {
        changed();
        str_ = s;
      }
    }
  };

  class Data
  {
    Ref<DataType> ptr_;
  public:
    Data() { }
    Data(DataType *t) : ptr_(t) { }
    Data(const Data& copy) { ptr_ = copy.ptr_; }

    std::string to_string() const {
      return ptr_->to_string();
    }
    int to_integer() const {
      return ptr_->to_integer();
    }
    double to_double() const {
      return ptr_->to_double();
    }

    bool is_array() const {
      return ptr_->is_array();
    }
    unsigned int size() const {
      return ptr_->size();
    }

    bool has_changed() const {
      return ptr_->has_changed();
    }

    void changed() {
      ptr_->changed();
    }

    std::string type() const {
      return ptr_->type();
    }

    void add(DataType *data) {
      ptr_->add(data);
    }
    void add(std::string s, DataType *data) {
      ptr_->add(s, data);
    }
    void add(bool b) {
      ptr_->add(b);
    }
    void add(int i) {
      ptr_->add(i);
    }
    void add(double d) {
      ptr_->add(d);
    }
    void add(std::string s, std::string c) {
      ptr_->add(s, c);
    }
    void add(std::string key, bool b) {
      ptr_->add(key, b);
    }
    void add(std::string key, int i) {
      ptr_->add(key, i);
    }
    void add(std::string key, double d) {
      ptr_->add(key, d);
    }
    void add(std::string key, std::string s, std::string c) {
      ptr_->add(key, s, c);
    }

    void assign(bool b) {
      ptr_->assign(b);
    }
    void assign(int i) {
      ptr_->assign(i);
    }
    void assign(double d) {
      ptr_->assign(d);
    }
    void assign(std::string s) {
      ptr_->assign(s);
    }

    void reset() {
      ptr_->reset();
    }

    Data& operator[](int i) {
      return (*(ptr_.pointer()))[i];
    }
    Data& operator[](std::string s) {
      return (*(ptr_.pointer()))[s];
    }
  };

  class ArrayType : public DataType
  {
    std::vector<Data> array_;
  public:
    ArrayType() { }

    virtual std::string type() const {
      return std::string("array");
    }

    virtual void add(DataType *data) {
      array_.push_back(Data(data));
    }
    virtual void add(bool b) {
      add(new BooleanDataType(b));
    }
    virtual void add(int i) {
      add(new IntDataType(i));
    }
    virtual void add(double d) {
      add(new DoubleDataType(d));
    }
    virtual void add(std::string s, std::string c = "") {
      add(new StringDataType(s, c));
    }

    virtual Data& operator[](unsigned int i) {
      if (i >= array_.size())
        throw IndexException("out of range");
      changed();
      return array_[i];
    }
    virtual Data& operator[](std::string s) {
      unsigned int i = static_cast<unsigned int>(std::strtod(s.c_str(), NULL));
      if (i >= array_.size())
        throw IndexException("out of range");
      changed();
      return array_[i];
    }
    virtual bool is_array() const {
      return true;
    }

    virtual unsigned int size() const {
      return array_.size();
    }

    virtual std::string to_string() const {
      std::string str = "[ ";
      for (unsigned int i=0; i<array_.size(); ++i) {
        str += array_[i].to_string();
        if (i != array_.size() - 1)
          str += ", ";
      }
      str += " ]";
      return str;
    }

    virtual void reset() {
        array_.clear();
    }
  };

  class MapType : public DataType
  {
    std::map<std::string, Data> keyvals_;
    typedef std::map<std::string, Data>::iterator iterator;
    typedef std::map<std::string, Data>::const_iterator const_iterator;
  public:
    MapType() { }

    virtual std::string type() const {
      return std::string("map");
    }

    virtual void add(std::string key, DataType *data) {
      to_upper(key);

      iterator pos = keyvals_.find(key);
      if (pos != keyvals_.end())
        throw DuplicateKeyException();
      keyvals_[key] = Data(data);
    }
    virtual void add(std::string key, bool b) {
      add(key, new BooleanDataType(b));
    }
    virtual void add(std::string key, int i) {
      add(key, new IntDataType(i));
    }
    virtual void add(std::string key, double d) {
      add(key, new DoubleDataType(d));
    }
    virtual void add(std::string key, std::string s, std::string c = "") {
      add(key, new StringDataType(s, c));
    }

    virtual bool exists(std::string key) {
      to_upper(key);
      iterator pos = keyvals_.find(key);
      if (pos != keyvals_.end())
        return true;
      return false;
    }

    virtual Data& operator[](std::string s) {
      to_upper(s);
      if (!exists(s))
        throw IndexException(s);
      return keyvals_[s];
    }
    virtual bool is_array() const {
      return true;
    }

    virtual unsigned int size() const {
      return keyvals_.size();
    }

    virtual std::string to_string() const {
      std::string str = "{ ";
      for (const_iterator pos = keyvals_.begin(); pos != keyvals_.end(); ++pos) {
        str += pos->first + " => " + pos->second.to_string() + ", ";
      }
      str += "}";
      return str;
    }
  };

  class Options
  {
    bool edit_globals_;

    /// "Active" set of options
    std::map<std::string, Data> keyvals_;

    /// "Global" set of options
    std::map<std::string, Data> globals_;

    typedef std::map<std::string, Data>::iterator iterator;
    typedef std::map<std::string, Data>::const_iterator const_iterator;

  public:
    Options() :edit_globals_(false) { }

    Options & operator=(const Options& rhs) {
      // Don't self copy
      if (this == &rhs)
        return *this;

      keyvals_ = rhs.keyvals_;
      globals_ = rhs.globals_;

      return *this;
    }

    bool read_globals() const { return edit_globals_; }
    void set_read_globals(bool _b) { edit_globals_ = _b; }

    void to_upper(std::string& str) {
      std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    void add(std::string key, DataType *data) {
      to_upper(key);

      std::map<std::string, Data> & local = edit_globals_ ? globals_ : keyvals_;

      // Make sure the key isn't already there
      iterator pos = local.find(key);
      if (pos != local.end()) { // If it is there, make sure they are the same type
          if (pos->second.type() != data->type())
              throw DuplicateKeyException();
          return;
      }
      local[key] = Data(data);
    }
    void add(std::string key, bool b) {
      add(key, new BooleanDataType(b));
    }
    void add(std::string key, int i) {
      add(key, new IntDataType(i));
    }
    void add(std::string key, double d) {
      add(key, new DoubleDataType(d));
    }
    void add(std::string key, std::string s, std::string c = "") {
      add(key, new StringDataType(s, c));
    }
    void add_i(std::string key, std::string s, std::string c = "") {
      add(key, new IStringDataType(s, c));
    }

    void add_bool(std::string key, bool b) {
      add(key,b);
    }
    void add_int(std::string key, int i) {
      add(key,i);
    }
    void add_double(std::string key, double d) {
      add(key,d);
    }
    void add_str(std::string key, std::string s, std::string c = "") {
      add(key,s,c);
    }
    void add_str_i(std::string key, std::string s, std::string c = "") {
      add_i(key,s,c);
    }
    void add_array(std::string key) {
      add(key, new ArrayType());
    }
    void set_bool(std::string key, bool b) {
      get(key).assign(b);
    }
    void set_int(std::string key, int i) {
      get(key).assign(i);
    }
    void set_double(std::string key, double d) {
      get(key).assign(d);
    }
    void set_str(std::string key, std::string s) {
      get(key).assign(s);
    }

    void set_global_bool(std::string key, bool b) {
      get_global(key).assign(b);
    }
    void set_global_int(std::string key, int i) {
      get_global(key).assign(i);
    }
    void set_global_double(std::string key, double d) {
      get_global(key).assign(d);
    }
    void set_global_str(std::string key, std::string s) {
      get_global(key).assign(s);
    }

    void clear(void) {
      keyvals_.clear();
    }

    bool exists_in_active(std::string key) {
      to_upper(key);

      iterator pos = keyvals_.find(key);
      if (pos != keyvals_.end())
        return true;
      return false;
    }

    bool exists_in_global(std::string key) {
      to_upper(key);

      iterator pos = globals_.find(key);
      if (pos != globals_.end())
        return true;
      return false;
    }

    Data& get(std::string key) {
      to_upper(key);
      if (!exists_in_active(key)) {
        // Key not found. Throw an error
        throw IndexException(key);
      }
      return keyvals_[key];
    }

    Data& get(std::map<std::string, Data>& m, std::string& key) {
        to_upper(key);
        return m[key];
    }

    Data& get_global(std::string key) {
      to_upper(key);
      if (!exists_in_global(key)) {
        // Key not found. Throw an error
        throw IndexException(key);
      }
      return globals_[key];
    }

    Data& use(std::string& key)
    {
        to_upper(key);

        // edit globals being true overrides everything
        if (edit_globals_)
            return get(globals_, key);

        if (!exists_in_active(key) && !exists_in_global(key))
            throw IndexException(key);
        else if (!exists_in_active(key) && exists_in_global(key))
            return get(globals_, key);
        else if (exists_in_active(key) && exists_in_global(key)) {
            Data& active = get(keyvals_, key);

            if (active.has_changed() == false) {
                // Pull from globals
                return get(globals_, key);
            }
            else {
                // Pull from keyvals
                return get(keyvals_, key);
            }
        }
        else
            return get(keyvals_, key);
    }

    bool get_bool(std::string key) {
      return(static_cast<bool>(use(key).to_integer()));
    }

    int get_int(std::string key) {
      return(use(key).to_integer());
    }

    double get_double(std::string key) {
      return(use(key).to_double());
    }

    std::string get_str(std::string key) {
      return(use(key).to_string());
    }

    int* get_int_array(std::string key) {
      int *array = new int[use(key).size()];
      for (unsigned int i=0; i<use(key).size(); ++i) {
        array[i] = use(key)[i].to_integer();
      }
      return array;
    }

    std::vector<int> get_int_vector(std::string key) {
      std::vector<int> array;
      for (unsigned int i=0; i<use(key).size(); ++i) {
        array.push_back(use(key)[i].to_integer());
      }
      return array;
    }

    double* get_double_array(std::string key) {
      double *array = new double[use(key).size()];
      for (unsigned int i=0; i<use(key).size(); ++i) {
        array[i] = use(key)[i].to_double();
      }
      return array;
    }

    std::vector<double> get_double_vector(std::string key) {
      std::vector<double> array;
      for (unsigned int i=0; i<use(key).size(); ++i) {
        array.push_back(use(key)[i].to_double());
      }
      return array;
    }

    const char* get_cstr(std::string key) {
      return(use(key).to_string().c_str());
    }

    Data& operator[](std::string key) {
      return use(key);
    }

    std::string to_string() const;
    std::string globals_to_string() const;

    void print();
    void print_globals();

    void read_ipv1();

  private:
    void read_boolean(Data& data, const std::string& key, int m = 0, int n = 0);
    void read_int(Data& data, const std::string& key, int m = 0, int n = 0);
    void read_double(Data& data, const std::string& key, int m = 0, int n = 0);
    void read_string(Data& data, const std::string& key, int m = 0, int n = 0);
    void read_array(Data& data, const std::string& key);
    // DataType *read_map(const std::string& key);
  };

}
#endif
