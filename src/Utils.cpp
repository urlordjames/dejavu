#include "Utils.h"

Json::StreamWriterBuilder writer_builder;

const std::string stringify(Json::Value &v) {
	return Json::writeString(writer_builder, v);
}
