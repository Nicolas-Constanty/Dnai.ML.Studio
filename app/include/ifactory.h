#ifndef IFACTORY_H
#define IFACTORY_H

#include <QMap>
#include <functional>
#include <type_traits>

template <typename FactoryType, class ClassIdentifier>
class IFactory
{
    using Callback = std::function<FactoryType *()>;
    using Key = typename std::conditional<std::is_enum<ClassIdentifier>::value, int, ClassIdentifier>::type;
public:
    IFactory() = default;
        /**
         * \brief Return a new instance of type (FactoryType) with the identifier of type (ClassIdentifier)
         * \param ci
         * \return return the new instance of object or nullptr if an error occur
         */
        FactoryType *create(Key ci) const
        {
            return m_factoryObjects[ci]();
        }

        /**
         * \brief Register a new class of type (TrueType) witch derived of base type (FactoryType) and assign an identifier of type ClassIdentifier
         * \tparam TrueType
         * \tparam Args
         * \param ci
         * \param args
         */
        template<typename TrueType, typename ... Args>
    void registerObject(Key ci, Args&& ... args)
    {
        m_factoryObjects[ci] = [=]() { return static_cast<FactoryType *>(new TrueType(std::move(args)...)); };
    }

private:
        QMap<Key, Callback> m_factoryObjects;
};
#endif // IFACTORY_H
