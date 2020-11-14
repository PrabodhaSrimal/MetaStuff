#include <stdexcept>

namespace meta
{

template <typename Class, typename T>
constexpr Member<Class, T>::Member(const char* name, member_ptr_t<Class, T> ptr) :
    name(name),
    ptr(ptr),
    hasMemberPtr(true),
    refGetterPtr(nullptr),
    refSetterPtr(nullptr),
    valGetterPtr(nullptr),
    valSetterPtr(nullptr),
    nonConstRefGetterPtr(nullptr)
{ }

template <typename Class, typename T>
constexpr Member<Class, T>::Member(const char* name, ref_getter_func_ptr_t<Class, T> getterPtr, ref_setter_func_ptr_t<Class, T> setterPtr) :
    name(name),
    ptr(nullptr),
    hasMemberPtr(false),
    refGetterPtr(getterPtr),
    refSetterPtr(setterPtr),
    valGetterPtr(nullptr),
    valSetterPtr(nullptr),
    nonConstRefGetterPtr(nullptr)
{ }

template <typename Class, typename T>
constexpr Member<Class, T>::Member(const char* name, val_getter_func_ptr_t<Class, T> getterPtr, val_setter_func_ptr_t<Class, T> setterPtr) :
    name(name),
    ptr(nullptr),
    hasMemberPtr(false),
    refGetterPtr(nullptr),
    refSetterPtr(nullptr),
    valGetterPtr(getterPtr),
    valSetterPtr(setterPtr),
    nonConstRefGetterPtr(nullptr)
{ }

template <typename Class, typename T>
constexpr Member<Class, T>& Member<Class, T>::addNonConstGetter(nonconst_ref_getter_func_ptr_t<Class, T> nonConstRefGetterPtr)
{
    this->nonConstRefGetterPtr = nonConstRefGetterPtr;
    return *this;
}

template <typename Class, typename T>
constexpr const T& Member<Class, T>::get(const Class& obj) const
{
    if (refGetterPtr) {
        return (obj.*refGetterPtr)();
    } else if (hasMemberPtr) {
        return obj.*ptr;
    }
    throw std::runtime_error("Cannot return const ref to member: no getter or member pointer set");
}

template <typename Class, typename T>
constexpr T Member<Class, T>::getCopy(const Class& obj) const
{
    if (refGetterPtr) {
        return (obj.*refGetterPtr)();
    } else if (valGetterPtr) {
        return (obj.*valGetterPtr)();
    } else if (hasMemberPtr) {
        return obj.*ptr;
    }
    throw std::runtime_error("Cannot return copy of member: no getter or member pointer set");
}

template <typename Class, typename T>
constexpr T& Member<Class, T>::getRef(Class& obj) const
{
    if (nonConstRefGetterPtr) {
        return (obj.*nonConstRefGetterPtr)();
    } else if(hasMemberPtr) {
        return obj.*ptr;
    }
    throw std::runtime_error("Cannot return ref to member: no getter or member pointer set");
}

template <typename Class, typename T>
constexpr member_ptr_t<Class, T> Member<Class, T>::getPtr() const {
    if (hasPtr()) {
        return ptr;
    }
    throw std::runtime_error("Cannot get pointer to member: it wasn't set");
}

template<typename Class, typename T>
template <typename V, typename>
constexpr void Member<Class, T>::set(Class& obj, V&& value) const
{
    // TODO: add rvalueSetter?
    if (refSetterPtr) {
        if constexpr (std::is_trivially_move_constructible<member_type>::value)
        {
            (obj.*refSetterPtr)(std::move(value));
        }
        else
        {
            (obj.*refSetterPtr)(value);
        }
    } else if (valSetterPtr) {
        (obj.*valSetterPtr)(value); // will copy value
    } else if (hasMemberPtr) {
        if constexpr (std::is_trivially_move_constructible<member_type>::value)
        {
            obj.*ptr = std::move(value);
        }
        else if constexpr (std::is_copy_assignable<member_type>::value)
        {
            obj.*ptr = value;
        }
        else
        {
            char buf[256];
            sprintf(buf, "Member not copy/move assignable: provide a reference setter: %s", name);
            throw std::runtime_error(buf);
        }
    } else {
        throw std::runtime_error("Cannot access member: no setter or member pointer set");
    }
}

template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, T Class::* ptr)
{
    return Member<Class, T>(name, ptr);
}

template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, ref_getter_func_ptr_t<Class, T> getterPtr, ref_setter_func_ptr_t<Class, T> setterPtr)
{
    return Member<Class, T>(name, getterPtr, setterPtr);
}

template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, val_getter_func_ptr_t<Class, T> getterPtr, val_setter_func_ptr_t<Class, T> setterPtr)
{
    return Member<Class, T>(name, getterPtr, setterPtr);
}

// read only
template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, ref_getter_func_ptr_t<Class, T> getterPtr)
{
    return Member<Class, T>(name, getterPtr, nullptr);
}

template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, val_getter_func_ptr_t<Class, T> getterPtr)
{
    return Member<Class, T>(name, getterPtr, nullptr);
}

// set only
template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, ref_setter_func_ptr_t<Class, T> setterPtr)
{
    return Member<Class, T>(name, nullptr, setterPtr);
}

template <typename Class, typename T>
constexpr Member<Class, T> member(const char* name, val_setter_func_ptr_t<Class, T> setterPtr)
{
    return Member<Class, T>(name, nullptr, setterPtr);
}

} // end of namespace meta
