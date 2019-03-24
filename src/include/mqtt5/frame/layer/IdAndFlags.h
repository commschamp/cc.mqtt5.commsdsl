/// @file
/// @brief Custom frame layer that handles message id and flags.

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

#include "comms/MsgFactory.h"
#include "comms/protocol/MsgIdLayer.h"
#include "comms/field_cast.h"


namespace mqtt5
{

namespace frame
{

namespace layer
{

/// @brief Id and flags handling layer
/// @tparam TField Used field type
/// @tparam TMessage Interface class
/// @tparam TAllMessages Input messages
/// @tparam TNextLayer Next frame layer
/// @tparam TExtraOpt Extra options passed to @b comms::MsgFactory
template <
    typename TField,
    typename TMessage,
    typename TAllMessages,
    typename TNextLayer,
    typename... TExtraOpt>
class IdAndFlags : public
    comms::protocol::MsgIdLayer<
        TField,
        TMessage,
        TAllMessages,
        TNextLayer,
        TExtraOpt...,
        comms::option::ExtendingClass<IdAndFlags<TField, TMessage, TAllMessages, TNextLayer, TExtraOpt...> >
    >
{
    using Base =
        comms::protocol::MsgIdLayer<
            TField,
            TMessage,
            TAllMessages,
            TNextLayer,
            TExtraOpt...,
            comms::option::ExtendingClass<IdAndFlags<TField, TMessage, TAllMessages, TNextLayer, TExtraOpt...> >
        >;
public:

    /// @brief MsgIdParamType type is defined in the base class.
    using MsgIdParamType = typename Base::MsgIdParamType;

    /// @brief MsgIdType type is defined in the base class.
    using MsgIdType = typename Base::MsgIdType;

    /// @brief Field type is defined in the base class.
    using Field = typename Base::Field;

    /// @brief Retrieve message ID value from the given field
    static MsgIdType getMsgIdFromField(const Field& field)
    {
        return static_cast<MsgIdType>(field.field_id().value());
    }
    
    /// @brief Set the flags to message object
    template <typename TMsg>
    static void beforeRead(const Field& field, TMsg& msg)
    {
        auto& msgFlags = msg.transportField_flags();
        using MsgFlagsFieldType = typename std::decay<decltype(msgFlags)>::type;
        msgFlags = comms::field_cast<MsgFlagsFieldType>(field.field_flags());
    }

    /// @brief Assemble the field's value before its write.
    template <typename TMsg>
    static void prepareFieldForWrite(MsgIdParamType id, const TMsg& msg, Field& field)
    {
        auto& idField = field.field_id();
        using IdFieldType = typename std::decay<decltype(idField)>::type;
        using IdFieldValueType = typename IdFieldType::ValueType;
        idField.value() = static_cast<IdFieldValueType>(id);

        auto& fieldFlags = field.field_flags();
        using FieldFlagsType = typename std::decay<decltype(fieldFlags)>::type;
        fieldFlags = comms::field_cast<FieldFlagsType>(msg.transportField_flags());
    }
};

} // namespace layer

} // namespace frame

} // namespace mqtt5


