#pragma once
#include "CommonHeaders.h"

using namespace std;

namespace Net {
	
	template <typename T>
	struct MessageHeader
	{
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct Message
	{
		MessageHeader<T> header;
		vector<uint8_t> data;

		size_t size() const {
			return data.size();
		}

		friend std::ostream& operator << (std::ostream& os, const Message<T>& msg) {
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		template<typename DataType>
		friend Message<T>& operator << (Message<T>& msg, const DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex... can't be pushed in...");
			size_t i = msg.data.size();
			msg.data.resize(msg.data.size() + sizeof(DataType));
			std::memcpy(msg.data.data() + i, &data, sizeof(DataType));
			msg.header.size = msg.size();
			return msg;
		}

		template<typename DataType>
		friend Message<T>& operator >> (Message<T>& msg, DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex... can't be reached in...");
			size_t i = msg.data.size() - sizeof(DataType);
			std::memcpy(&data, msg.data.data() + i, sizeof(DataType));
			msg.data.resize(i);
			msg.header.size = msg.size();
			return msg;
		}
	};

	template<typename T>
	class Connection;

	template<typename T>
	struct OwnedMessage {
	public:
		shared_ptr<Connection<T>> remote = nullptr;
		Message<T> message;
		friend std::ostream& operator << (std::ostream& os, const OwnedMessage<T>& msg) {
			os << msg.message;
			return os;
		}
	};
}