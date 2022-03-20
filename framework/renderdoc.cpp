//#include "renderdoc.hpp"
//
//#include <algorithm>
//#include <fmt/core.h>
//#include <fmt/color.h>
//#include <vector>
//#include <miniz.h>
//#include <filesystem>
//#include <string_view>
//
//using namespace RenderDoc;
//
//bool Dump::parse() {
//	if (!parse_xml() || !parse_zip()) {
//		return false;
//	}
//
//	parse_actions();
//	return true;
//}
//
//bool Dump::parse_xml() {
//	const auto result = m_doc.load_file(m_file_path.data());
//	if (!result)
//		return false;
//
//	m_rdc = m_doc.child("rdc");
//	m_chunks = m_rdc.child("chunks");
//
//	return true;
//}
//
///**
// * \brief parses the zip file containing thumbnails and data buffers
// * the thumbnails parsed into m_thumbnail and m_extended_thumbnail
// * the buffers are parsed into m_buffers
// * \return true if succeeded
// */
//bool Dump::parse_zip() {
//	const auto xml_path = std::filesystem::path(m_file_path);
//	const auto zip_file = xml_path.parent_path() / xml_path.stem();
//	if (!std::filesystem::exists(zip_file)) {
//		return false;
//	}
//
//	mz_zip_archive zip = {};
//	mz_bool success = mz_zip_reader_init_file(&zip, zip_file.string().c_str(), 0);
//	if (success) {
//		const mz_uint numfiles = mz_zip_reader_get_num_files(&zip);
//
//		m_buffers.resize(numfiles);
//
//		for (mz_uint i = 0; i < numfiles; i++) {
//			mz_zip_archive_file_stat zstat;
//			mz_zip_reader_file_stat(&zip, i, &zstat);
//
//			size_t sz = 0;
//
//			const auto buf = static_cast<std::byte*>(mz_zip_reader_extract_to_heap(&zip, i, &sz, 0));
//
//			// thumbnails are stored separately
//			if (strstr(zstat.m_filename, "thumb")) {
//				auto type = FileType::JPG;
//				if (strstr(zstat.m_filename, ".png"))
//					type = FileType::PNG;
//				else if (strstr(zstat.m_filename, ".raw"))
//					type = FileType::Raw;
//
//				if (strstr(zstat.m_filename, "ext_thumb")) {
//					m_extended_thumbnail.format = type;
//					m_extended_thumbnail.data.resize(sz);
//					std::copy_n(buf, sz, m_extended_thumbnail.data.begin());
//				}
//				else {
//					m_thumbnail.format = type;
//					m_thumbnail.data.resize(sz);
//					std::copy_n(buf, sz, m_thumbnail.data.begin());
//				}
//			}
//			else {
//				const int bufname = atoi(zstat.m_filename);
//				fmt::print("Parsing buffer: {}\n", bufname);
//
//				if (bufname < static_cast<int>(m_buffers.size())) {
//					m_buffers[bufname].resize(sz);
//					std::copy_n(buf, sz, m_buffers[bufname].begin());
//				}
//			}
//
//			free(buf);
//		}
//	}
//	mz_zip_reader_end(&zip);
//	return true;
//}
//
//void Dump::dump_header() {
//	const auto header = m_rdc.child("header");
//	const auto drivers = header.children("driver");
//	/*
//	 * Driver
//	 */
//	fmt::print("Driver(s):\n");
//	for (const auto& driver : drivers) {
//		const auto driver_name = driver.first_child().value();
//		const auto driver_id = driver.attribute("id").value();
//		fmt::print("\t{} id {}\n", driver_name, driver_id);
//	}
//	/*
//	 * Thumbnail
//	 */
//	const auto thumbnail = header.child("thumbnail");
//	const auto thumbnail_width = thumbnail.attribute("width").value();
//	const auto thumbnail_height = thumbnail.attribute("height").value();
//	fmt::print("Thumbnail: {} ({}x{})\n", thumbnail.first_child().value(), thumbnail_width, thumbnail_height);
//
//
//
//	/*
//	const auto extended_thumbnail = m_rdc.child("extended_thumbnail");
//	const auto extended_thumbnail_width = extended_thumbnail.attribute("width").value();
//	const auto extended_thumbnail_height = extended_thumbnail.attribute("height").value();
//	fmt::print("Extended Thumbnail: {} ({}x{})\n", extended_thumbnail.first_child().value(), extended_thumbnail_width, extended_thumbnail_height);
//	*/
//}
//
//void Dump::dump_chunks() {
//	const auto chunks = m_chunks.children("chunk");
//
//
//	//for (const auto& chunk : chunks) {
//	//	//nodes.push_back(chunk);
//	//	fmt::print("[{}] {}\n", get_id(chunk), get_name(chunk));
//	//}
//	//std::ranges::sort(nodes, [](const pugi::xml_node& lhs, const pugi::xml_node& rhs) {
//	//	return get_timestamp(lhs) < get_timestamp(rhs);
//	//});
//
//	//for (const auto& chunk : nodes) {
//	//	const auto chunk_name = get_name(chunk);
//	//	fmt::print("[{}] {}\n", get_id(chunk), get_name(chunk));
//	//}
//}
//
//ActionDescription Dump::find_action(std::string_view action_name) {
//	auto [action_node, index] = find_action_internal(action_name);
//	if (action_node.empty()) {
//		return ActionDescription{};
//	}
//	return ActionDescription{ action_node, index };
//}
//
///**
// * \brief Finds action node and index
// * \param action_name name of the action
// * \return action node and index found in m_actions
// */
//std::pair<pugi::xml_node, int> Dump::find_action_internal(std::string_view action_name) const {
//	if (auto it = std::find_if(m_actions.begin(), m_actions.end(), [&action_name](const pugi::xml_node& node) {
//		return get_name(node) == action_name;
//		}); it != m_actions.end()) {
//		return std::make_pair(*it, static_cast<int>(it - m_actions.begin()));
//	}
//	return std::make_pair(pugi::xml_node{}, -1);
//}
//
//void Dump::parse_actions() {
//	const auto capture_begin = find_capture_begin_node();
//	const auto capture_end = find_capture_end_node();
//
//	pugi::xml_node action = capture_begin.next_sibling();
//	// We skip the very first sibling, this is just "Internal::Context Configuration" and not the actual first action
//	// that might explain why event id's in renderdoc also goes from 0 to 2
//	while ((action = action.next_sibling()) != capture_end) {
//		m_actions.push_back(action);
//		fmt::print("[{}] {}\n", get_id(action), get_name(action));
//	}
//}
