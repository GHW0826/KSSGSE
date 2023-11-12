#include "JsonNode.h"
#include "elements.h"

#include "reader.h"
#include "writer.h"

//////////////////////////////////////////////////////////////////////////
// JsonObjectNode
//////////////////////////////////////////////////////////////////////////

JsonObjectNode::JsonObjectNode()
	: jsonObj_(new json::Object())
{}
JsonObjectNode::JsonObjectNode(const json::Object& obj)
	: jsonObj_(new json::Object()) {
	(*jsonObj_) = obj;
}
JsonObjectNode::JsonObjectNode(const JsonObjectNode& other) {
	(*jsonObj_) = (*other.jsonObj_);
}
JsonObjectNode::~JsonObjectNode() {
	if (nullptr != jsonObj_) {
		delete jsonObj_;
		jsonObj_ = nullptr;
	}
}

JsonObjectNode& JsonObjectNode::operator = (const JsonObjectNode& other) {
	if (this != &other) {
		(*jsonObj_) = (*other.jsonObj_);
	}
	return *this;
}
bool JsonObjectNode::operator == (const JsonObjectNode& object) const {
	return (*jsonObj_) == (*object.jsonObj_);
}


void JsonObjectNode::AddImpl(const char* key, const tstring& value) {
	(*jsonObj_)[key] = json::String(value);
}


tstring JsonObjectNode::GetValueImpl(const char* key) const {
	json::String& value = (*jsonObj_)[key];
	return value;
}

json::Object* JsonObjectNode::GetRawNode() const {
	return jsonObj_;
}

void JsonObjectNode::AppendObjectNode(const char* key, JsonObjectNode* childNode) {
	(*jsonObj_)[key] = (*childNode->GetRawNode());
}


void JsonObjectNode::AppendArrayNode(const char* key, JsonArrayNode* childNode) {
	(*jsonObj_)[key] = (*childNode->GetRawNode());
}

JsonObjectNode JsonObjectNode::GetObjectNode(const char* key) const {
	return JsonObjectNode((*jsonObj_)[key]);
}

JsonArrayNode JsonObjectNode::GetArrayNode(const char* key) const {
	return JsonArrayNode((*jsonObj_)[key]);
}

tstring JsonObjectNode::ToString() const {
	std::stringstream stream;
	json::Writer::Write(*jsonObj_, stream);
	return stream.str();
}

void JsonObjectNode::Reset() {
	if (jsonObj_)
		jsonObj_->Clear();
}

unsigned int JsonObjectNode::Parse(const char* content, size_t len) {
	std::stringstream stream;
	stream.str(tstring(content, len));
	try {
		json::Reader::Read(*jsonObj_, stream);
	}
	catch (const json::Exception&) {
		return 0;
	}

	return (int)ToString().length();
}

unsigned int JsonObjectNode::Parse(const tstring& content) {
	return Parse(content.c_str(), content.length());
}



//////////////////////////////////////////////////////////////////////////
// JsonArrayNode
//////////////////////////////////////////////////////////////////////////

JsonArrayNode::JsonArrayNode()
	: jsonArray_(new json::Array())
{}
JsonArrayNode::JsonArrayNode(const json::Array& obj)
	: jsonArray_(new json::Array()) {
	(*jsonArray_) = obj;
}
JsonArrayNode::JsonArrayNode(const JsonArrayNode& other) {
	(*jsonArray_) = (*other.jsonArray_);
}
JsonArrayNode::~JsonArrayNode() {
	if (nullptr != jsonArray_) {
		delete jsonArray_;
		jsonArray_ = nullptr;
	}
}

JsonArrayNode& JsonArrayNode::operator = (const JsonArrayNode& other) {
	if (this != &other) {
		(*jsonArray_) = (*other.jsonArray_);
	}
	return *this;
}
bool JsonArrayNode::operator == (const JsonArrayNode& object) const {
	return (*jsonArray_) == (*object.jsonArray_);
}

void JsonArrayNode::Insert(JsonObjectNode& node) {
	jsonArray_->Insert(*node.GetRawNode());
}

size_t JsonArrayNode::Size() const {
	return jsonArray_->Size();
}

void JsonArrayNode::Clear() {
	jsonArray_->Clear();
}

JsonObjectNode JsonArrayNode::GetObjectNode(int index) {
	json::Array::const_iterator curIter(jsonArray_->Begin());
	for (int i = 0; i < index; ++i)
		++curIter;
	const json::Object& returnObj = *curIter;

	return JsonObjectNode(returnObj);
}

tstring JsonArrayNode::ToString() const {
	std::stringstream stream;
	json::Writer::Write(*jsonArray_, stream);
	return stream.str();
}

void JsonArrayNode::Parse(const char* content, size_t len) {
	std::stringstream stream;
	stream.str(tstring(content, len));
	json::Reader::Read(*jsonArray_, stream);
}

void JsonArrayNode::Parse(const tstring& content) {
	Parse(content.c_str(), content.length());
}
