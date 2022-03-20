//#pragma once
//#include <map>
//#include <pugixml.hpp>
//#include <string_view>
//#include <vector>
//
//namespace RenderDoc {
//	static std::string_view get_name(const pugi::xml_node& node) {
//		return node.attribute("name").value();
//	}
//	static uint32_t get_id(const pugi::xml_node& node) {
//		return node.attribute("id").as_int();
//	}
//	static uint64_t get_timestamp(const pugi::xml_node& node) {
//		return node.attribute("timestamp").as_ullong();
//	}
//	static uint64_t get_length(const pugi::xml_node& node) {
//		return node.attribute("length").as_ullong();
//	}
//	static pugi::xml_node find_by_name(const pugi::xml_node& node, std::string_view name) {
//		return node.find_child_by_attribute("name", name.data());
//	}
//
//	static constexpr uint32_t NoChunk = ~0U;
//
//	class ActionDescription {
//	public:
//		ActionDescription() = default;
//		ActionDescription(const pugi::xml_node& action_node, int action_index) : m_action_index(action_index), m_empty(action_node.empty()) {
//			parse(action_node);
//		}
//		bool empty() const {
//			return m_empty;
//		}
//		int action_index() const {
//			return m_action_index;
//		}
//		bool is_fake_marker() const {
//			return /*events.size() == 1 && */ m_chunk_index == NoChunk;
//		}
//
//	private:
//		void parse(const pugi::xml_node& node) {
//			m_name = get_name(node);
//			m_chunk_index = get_id(node);
//
//			auto first = find_by_name(node, "first");
//			if (!first.empty()) {
//				m_base_vertex = std::stoi(first.first_child().value()); // Not sure about this one
//			}
//
//			m_buf_len = get_length(node);
//		}
//	private:
//		std::string_view m_name{};
//		int m_action_index{-1};
//		uint32_t m_chunk_index{};
//		int32_t m_base_vertex{};
//		uint64_t m_buf_len{}; // Byte len of buffer
//
//		bool m_empty = true;
//	};
//
//
//	enum class FileType : uint32_t {
//		DDS,
//		First = DDS,
//		PNG,
//		JPG,
//		BMP,
//		TGA,
//		HDR,
//		EXR,
//		Raw,
//		Count,
//	};
//
//	struct ThumbTypeAndData {
//		FileType format{};
//		std::vector<std::byte> data{};
//	};
//
//	class Dump {
//	public:
//		// renderdoccmd.exe capture -c testout.rdc  "G:\repos\test_framework\out\build\x64-Debug\test_framework.exe" --framecount 3 --frame 5
//		//renderdoccmd.exe convert -f "G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.rdc" -o "G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.xml" -i rdc -c xml
//		Dump(std::string_view xml_file_path) : m_file_path(xml_file_path) { }
//		bool parse();
//		bool parse_xml();
//		bool parse_zip();
//		void dump_header();
//		void dump_chunks();
//
//		ActionDescription find_action(std::string_view action_name);
//	private:
//		/*
//		 * Store data internally
//		 */
//		void parse_actions();
//
//		std::pair<pugi::xml_node, int> find_action_internal(std::string_view action_name) const;
//
//		pugi::xml_node find_capture_begin_node() const {
//			return m_chunks.find_child([](const pugi::xml_node& node) {
//				return get_name(node) == "Internal::Beginning of Capture";
//				});
//		}
//		pugi::xml_node find_capture_end_node() const {
//			return m_chunks.find_child([](const pugi::xml_node& node) {
//				return get_name(node) == "Internal::End of Capture";
//				});
//		}
//	private:
//		std::string_view m_file_path{};
//		pugi::xml_document m_doc{};
//		pugi::xml_node m_rdc{};
//		pugi::xml_node m_chunks{};
//		std::vector<pugi::xml_node> m_actions{};
//
//		std::vector<std::vector<std::byte>> m_buffers{};
//		ThumbTypeAndData m_thumbnail{};
//		ThumbTypeAndData m_extended_thumbnail{};
//	};
//}
