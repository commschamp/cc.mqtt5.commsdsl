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
    comms::protocol::ProtocolLayerBase<
        TField,
        TNextLayer,
        IdAndFlags<TField, TMessage, TAllMessages, TNextLayer, TExtraOpt...>
    >
{
    using Base =
        comms::protocol::ProtocolLayerBase<
            TField,
            TNextLayer,
            IdAndFlags<TField, TMessage, TAllMessages, TNextLayer, TExtraOpt...>
        >;

    using Factory = comms::MsgFactory<TMessage, TAllMessages, TExtraOpt...>;

public:

    using AllMessages = typename Factory::AllMessages;
    using MsgPtr = typename Factory::MsgPtr;
    using Message = typename MsgPtr::element_type;
    using MsgIdParamType = typename Message::MsgIdParamType;
    using MsgIdType = typename Message::MsgIdType;
    using Field = typename Base::Field;

    template <typename... TArgs>
    explicit IdAndFlags(TArgs&&... args)
       : Base(std::forward<TArgs>(args)...)
    {
    }

    IdAndFlags(const IdAndFlags&) = default;

    IdAndFlags(IdAndFlags&&) = default;

    IdAndFlags& operator=(const IdAndFlags&) = default;

    IdAndFlags& operator=(IdAndFlags&&) = default;

    /// @brief Destructor
    ~IdAndFlags() = default;

    template <typename TMsgPtr, typename TIter, typename TNextLayerReader>
    comms::ErrorStatus doRead(
        Field& field,
        TMsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize,
        TNextLayerReader&& nextLayerReader)
    {
        GASSERT(!msgPtr);
        auto es = field.read(iter, size);
        if (es == comms::ErrorStatus::NotEnoughData) {
            Base::updateMissingSize(field, size, missingSize);
        }

        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        auto id = static_cast<MsgIdType>(field.field_id().value());
        msgPtr = createMsg(id);

        if (!msgPtr) {
            if (m_factory.msgCount(id) == 0U) {
                return comms::ErrorStatus::InvalidMsgId;
            }

            return comms::ErrorStatus::MsgAllocFailure;
        }

        copyFlagsValue(field.field_flags(), msgPtr->transportField_flags());

        es = nextLayerReader.read(msgPtr, iter, size - field.length(), missingSize);
        if (es != comms::ErrorStatus::Success) {
            msgPtr.reset();
        }

        return es;
    }

    template <typename TMsg, typename TIter, typename TNextLayerWriter>
    comms::ErrorStatus doWrite(
        Field& field,
        const TMsg& msg,
        TIter& iter,
        std::size_t size,
        TNextLayerWriter&& nextLayerWriter) const
    {
        copyFlagsValue(msg.transportField_flags(), field.field_flags());
        field.field_id().value() = getMsgId(msg);
        auto es = field.write(iter, size);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        GASSERT(field.length() <= size);
        return nextLayerWriter.write(msg, iter, size - field.length());
    }

    MsgPtr createMsg(MsgIdParamType id, unsigned idx = 0)
    {
        return m_factory.createMsg(id, idx);
    }

private:

    struct PolymorphicOpTag {};
    struct DirectOpTag {};
    
    template <typename TMsg>
    static MsgIdParamType getMsgId(const TMsg& msg, PolymorphicOpTag)
    {
        using MsgType = typename std::decay<decltype(msg)>::type;
        static_assert(comms::isMessage<MsgType>(),
            "The message class is expected to inherit from comms::Message");
        static_assert(MsgType::InterfaceOptions::HasMsgIdInfo,
            "The message interface class must expose polymorphic ID retrieval functionality, "
            "use comms::option::IdInfoInterface option to define it.");

        return msg.getId();
    }

    template <typename TMsg>
    static constexpr MsgIdParamType getMsgId(const TMsg& msg, DirectOpTag)
    {
        return msg.doGetId();
    }    
    
    template <typename TMsg>
    static constexpr MsgIdParamType getMsgId(const TMsg& msg)
    {
        using Tag = 
            typename std::conditional<
                comms::protocol::details::protocolLayerHasDoGetId<TMsg>(),
                DirectOpTag,
                PolymorphicOpTag
            >::type;
        return getMsgId(msg, Tag());
    }
    
    template <typename TFrom, typename TTo>
    static void copyFlagsValue(const TFrom& from, TTo& to)
    {
        to = comms::field_cast<TTo>(from);
    }

    Factory m_factory;
};

} // namespace layer

} // namespace frame

} // namespace mqtt5


