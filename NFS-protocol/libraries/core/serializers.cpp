#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include "./messages.hpp"
#include "./serializers.hpp"


extern int test_libcore(int x) { return x * 123; };

MessageBuilder::MessageBuilder() {
  buffer_ = std::vector<u_int8_t>(DATA_OFFSET_);
  data_len_ = 0;
}

void MessageBuilder::writeMessageType(MessageType type) {
  buffer_[MESSAGE_TYPE_OFFSET_] = (u_int8_t)type;
};

void MessageBuilder::write(u_int8_t x) {
  buffer_.push_back(x);
  data_len_ += 1;
};

void MessageBuilder::write(off_t x) {
  for (int i = 0; i < 8; i++)
    buffer_.push_back(x >> ((7 - i) * 8));
  data_len_ += 8;
};


void MessageBuilder::write(int32_t x) {
  for (int i = 0; i < 4; i++)
    buffer_.push_back(x >> ((3 - i) * 8));
  data_len_ += 4;
};

void MessageBuilder::write(u_int32_t x) {
  for (int i = 0; i < 4; i++)
    buffer_.push_back(x >> ((3 - i) * 8));
  data_len_ += 4;
};

void MessageBuilder::write(u_int64_t x) {
  for (int i = 0; i < 8; i++)
    buffer_.push_back(x >> ((7 - i) * 8));
  data_len_ += 8;
};

void MessageBuilder::write(char *buf, size_t size) {
  write((u_int32_t)size);

  for (int i = 0; i < size; i++)
    buffer_.push_back(buf[i]);
  data_len_ += size;
};

void MessageBuilder::write(std::vector<u_int8_t> bytes) {
  write((u_int32_t)bytes.size());

  for (int i = 0; i < bytes.size(); i++)
    buffer_.push_back(bytes[i]);
  data_len_ += bytes.size();
};

void MessageBuilder::write(std::string str) {
  write((u_int32_t)str.size());
  for (char c : str)
    buffer_.push_back(c);
  data_len_ += str.size();
};

void MessageBuilder::write(dirent d) { 
  write(d.d_name);
  write(d.d_ino);
  write(d.d_off);
  write(d.d_reclen);
  write(d.d_type);


};

std::vector<u_int8_t> MessageBuilder::build() {
  for (int i = 0; i < 4; i++)
    buffer_[DATA_SIZE_OFFSET_ + i] = (data_len_ >> ((3 - i) * 8));

  return buffer_;
};

MessageParser::MessageParser(std::vector<u_int8_t> byte_message) {
  buffer_ = byte_message;
  next_data_ = DATA_OFFSET_;
};

MessageType MessageParser::readMessageType() {
  MessageType result = static_cast<MessageType>(buffer_[MESSAGE_TYPE_OFFSET_]);
  return result;
};

int32_t MessageParser::readSize() {
  int32_t result = 0;
  for (int i = 0; i < 4; i++)
    result +=
        (static_cast<int32_t>(buffer_[DATA_SIZE_OFFSET_ + i]) << ((3 - i) * 8));

  return result;
};

u_int8_t MessageParser::readUInt8T() {
  u_int8_t result = buffer_[next_data_];
  next_data_ += 1;

  return result;
};

u_int16_t MessageParser::readUInt16T() {
  u_int16_t result = 0;
  for (int i = 0; i < 2; i++, next_data_++)
    result += (static_cast<int32_t>(buffer_[next_data_]) << ((2 - i) * 8));

  return result;
};

int32_t MessageParser::readInt32T() {
  int32_t result = 0;
  for (int i = 0; i < 4; i++, next_data_++)
    result += (static_cast<int32_t>(buffer_[next_data_]) << ((3 - i) * 8));

  return result;
};

off_t MessageParser::readOffT() {
  
  off_t result = 0;
  for (int i = 0; i < 8; i++, next_data_++)
    result += (static_cast<off_t>(buffer_[next_data_]) << ((7 - i) * 8));

  return result;
};

u_int32_t MessageParser::readUInt32T() {
  int32_t result = 0;
  for (int i = 0; i < 4; i++, next_data_++)
    result += (static_cast<u_int32_t>(buffer_[next_data_]) << ((3 - i) * 8));

  return result;
};

u_int64_t MessageParser::readUInt64T() {
  u_int64_t result = 0;
  for (int i = 0; i < 8; i++, next_data_++)
    result += (static_cast<u_int64_t>(buffer_[next_data_]) << ((7 - i) * 8));

  return result;
};

std::vector<u_int8_t> MessageParser::readBytes() {
  u_int32_t result_size = readUInt32T();

  std::vector<u_int8_t> result;
  for (int i = 0; i < result_size; i++, next_data_++)
    result.push_back(buffer_[next_data_]);

  return result;
};

std::string MessageParser::readString() {
  u_int32_t result_size = readUInt32T();

  std::string result;
  for (int i = 0; i < result_size; i++, next_data_++)
    result.push_back(buffer_[next_data_]);

  return result;
};

dirent MessageParser::readDirent() {

  std::string d_name = readString();
  //std::string d_name = "Nazwa";
  __ino_t d_ino = readUInt64T();
  __off_t d_off = readOffT();
  u_int32_t d_reclen = readUInt32T();
  u_int8_t d_type = readUInt8T();

  char *dname = new char[d_name.length() + 1];
  strcpy(dname, d_name.c_str());

  dirent result;
  
  memcpy((void *)&result.d_name, dname ,sizeof(result.d_name));
  result.d_ino = d_ino;
  result.d_off = d_off;
  result.d_reclen = d_reclen;
  result.d_type = d_type;

  delete [] dname;
  return result;

}; 


extern std::vector<u_int8_t> SerializeOpenRequest(OpenRequest open_request) {
  MessageType message_type = MessageType::OPEN_REQUEST;
  std::string path = open_request.path;
  int32_t oflag = open_request.oflag;
  int32_t mode = open_request.mode;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(path);
  request_builder.write(oflag);
  request_builder.write(mode);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern OpenRequest DeserializeToOpenRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  std::string path = request_parser.readString();
  int32_t oflag = request_parser.readInt32T();
  int32_t mode = request_parser.readInt32T();

  OpenRequest open_request{path, oflag, mode};
  return open_request;
};

extern std::vector<u_int8_t> SerializeOpenResponse(OpenResponse open_response) {
  MessageType message_type = MessageType::OPEN_RESPONSE;
  int32_t result = open_response.result;
  int32_t error = open_response.error;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(result);
  request_builder.write(error);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern OpenResponse
DeserializeToOpenResponse(std::vector<u_int8_t> byte_response) {
  MessageParser request_parser(byte_response);
  MessageType message_type = request_parser.readMessageType();
  int32_t result = request_parser.readInt32T();
  int32_t error = request_parser.readInt32T();

  OpenResponse open_response{result, error};
  return open_response;
};

extern std::vector<u_int8_t> SerializeReadRequest(ReadRequest read_request) {
  MessageType message_type = MessageType::READ_REQUEST;
  int32_t fd = read_request.fd;
  int32_t count = read_request.count;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(fd);
  request_builder.write(count);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern ReadRequest DeserializeToReadRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int32_t fd = request_parser.readInt32T();
  int32_t count = request_parser.readInt32T();

  ReadRequest read_request{fd, count};
  return read_request;
};

extern std::vector<u_int8_t> SerializeReadResponse(ReadResponse read_response) {
  MessageType message_type = MessageType::READ_RESPONSE;
  int32_t result = read_response.result;
  std::vector<u_int8_t> buf = read_response.buf;
  int32_t error = read_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(buf);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};


extern ReadResponse DeserializeToReadResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int32_t result = response_parser.readInt32T();
  std::vector<u_int8_t> buf  = response_parser.readBytes();
  int32_t error = response_parser.readInt32T();

  ReadResponse read_response{result, buf, error};
  return read_response;
};

extern std::vector<u_int8_t> SerializeWriteRequest(WriteRequest write_request) {
  MessageType message_type = MessageType::WRITE_REQUEST;
  int32_t fd = write_request.fd;
  std::vector<u_int8_t> buf = write_request.buf;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(fd);
  request_builder.write(buf);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern WriteRequest DeserializeToWriteRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int32_t fd = request_parser.readInt32T();
  std::vector<u_int8_t> buf = request_parser.readBytes();
  WriteRequest write_request{fd, buf};
  return write_request;
};

extern std::vector<u_int8_t> SerializeWriteResponse(WriteResponse write_response) {
  MessageType message_type = MessageType::WRITE_RESPONSE;
  int32_t result = write_response.result;
  int32_t error = write_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};


extern WriteResponse DeserializeToWriteResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int32_t result = response_parser.readInt32T();
  int32_t error = response_parser.readInt32T();

  WriteResponse write_response{result, error};
  return write_response;
};


extern std::vector<u_int8_t> SerializeLseekRequest(LseekRequest lseek_request) {
  MessageType message_type = MessageType::LSEEK_REQUEST;
  int32_t fd = lseek_request.fd;
  off_t offset = lseek_request.offset;
  int32_t whence = lseek_request.whence;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(fd);
  request_builder.write(offset);
  request_builder.write(whence);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern LseekRequest DeserializeToLseekRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int32_t fd = request_parser.readInt32T();
  off_t offset = request_parser.readOffT();
  int32_t whence = request_parser.readInt32T();

  LseekRequest lseek_request{fd, offset, whence};
  return lseek_request;
};

extern std::vector<u_int8_t> SerializeLseekResponse(LseekResponse lseek_response) {
  MessageType message_type = MessageType::LSEEK_RESPONSE;
  off_t result = lseek_response.result; //?
  int32_t error = lseek_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern LseekResponse DeserializeToLseekResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  off_t result = response_parser.readOffT(); 
  int32_t error = response_parser.readInt32T();

  LseekResponse lseek_response{result, error};
  return lseek_response;
};

extern std::vector<u_int8_t> SerializeCloseRequest(CloseRequest close_request) {
  MessageType message_type = MessageType::CLOSE_REQUEST;
  int32_t fd = close_request.fd;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(fd);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern CloseRequest DeserializeToCloseRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int32_t fd = request_parser.readInt32T();

  CloseRequest close_request{fd};
  return close_request;
};

extern std::vector<u_int8_t> SerializeCloseResponse(CloseResponse close_response) {
  MessageType message_type = MessageType::CLOSE_RESPONSE;
  int32_t result = close_response.result;
  int32_t error = close_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern CloseResponse DeserializeToCloseResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int32_t result = response_parser.readInt32T();
  int32_t error = response_parser.readInt32T();

  CloseResponse close_response{result, error};
  return close_response;
};


extern std::vector<u_int8_t> SerializeUnlinkRequest(UnlinkRequest unlink_request) {
  MessageType message_type = MessageType::UNLINK_REQUEST;
  std::string pathname = unlink_request.pathname;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(pathname);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern UnlinkRequest DeserializeToUnlinkRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  std::string pathname = request_parser.readString();

  UnlinkRequest unlink_request{pathname};
  return unlink_request;
};

extern std::vector<u_int8_t> SerializeUnlinkResponse(UnlinkResponse unlink_response) {
  MessageType message_type = MessageType::UNLINK_RESPONSE;
  int32_t result = unlink_response.result;
  int32_t error = unlink_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern UnlinkResponse DeserializeToUnlinkResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int32_t result = response_parser.readInt32T();
  int32_t error = response_parser.readInt32T();

  UnlinkResponse unlink_response{result, error};
  return unlink_response;
};

extern std::vector<u_int8_t> SerializeOpendirRequest(OpendirRequest opendir_request) {
  MessageType message_type = MessageType::OPENDIR_REQUEST;
  std::string name = opendir_request.name;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(name);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern OpendirRequest DeserializeToOpendirRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  std::string name = request_parser.readString();

  OpendirRequest opendir_request{name};
  return opendir_request;
};

extern std::vector<u_int8_t> SerializeOpendirResponse(OpendirResponse opendir_response) {
  MessageType message_type = MessageType::OPENDIR_RESPONSE;
  int result = opendir_response.result;
  int32_t error = opendir_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern OpendirResponse DeserializeToOpendirResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int result = response_parser.readInt32T(); 
  int32_t error = response_parser.readInt32T();

  OpendirResponse opendir_response{result, error};
  return opendir_response;
};



extern std::vector<u_int8_t> SerializeReaddirRequest(ReaddirRequest readdir_request) {
  MessageType message_type = MessageType::READDIR_REQUEST;
  int dirfd = readdir_request.dirfd;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(dirfd); 

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern ReaddirRequest DeserializeToReaddirRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int dirfd = request_parser.readUInt32T(); 

  ReaddirRequest readdir_request{dirfd};
  return readdir_request;
};


extern std::vector<u_int8_t> SerializeReaddirResponse(ReaddirResponse readdir_response) {
  MessageType message_type = MessageType::READDIR_RESPONSE;
  int32_t isDirentNull = readdir_response.isDirentNull;
  dirent result = readdir_response.result;
  int32_t error = readdir_response.error;
  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(isDirentNull);
  response_builder.write(result); 
  response_builder.write(error);
  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern ReaddirResponse DeserializeToReaddirResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int32_t isDirentNull = response_parser.readInt32T();
  dirent result = response_parser.readDirent();
  int32_t error = response_parser.readInt32T();

  ReaddirResponse readdir_response{isDirentNull, result, error};
  return readdir_response;
};


extern std::vector<u_int8_t> SerializeClosedirRequest(ClosedirRequest closedir_request) {
  MessageType message_type = MessageType::CLOSEDIR_REQUEST;
  int dirfd = closedir_request.dirfd;

  MessageBuilder request_builder;
  request_builder.writeMessageType(message_type);
  request_builder.write(dirfd);

  std::vector<u_int8_t> byte_request = request_builder.build();
  return byte_request;
};

extern ClosedirRequest DeserializeToClosedirRequest(std::vector<u_int8_t> byte_request) {
  MessageParser request_parser(byte_request);
  MessageType message_type = request_parser.readMessageType();
  int dirfd = request_parser.readUInt32T();

  ClosedirRequest closedir_request{dirfd};
  return closedir_request;
};

extern std::vector<u_int8_t> SerializeClosedirResponse(ClosedirResponse closedir_response) {
  MessageType message_type = MessageType::CLOSEDIR_RESPONSE;
  int result = closedir_response.result;
  int32_t error = closedir_response.error;

  MessageBuilder response_builder;
  response_builder.writeMessageType(message_type);
  response_builder.write(result);
  response_builder.write(error);

  std::vector<u_int8_t> byte_response = response_builder.build();
  return byte_response;
};

extern ClosedirResponse DeserializeToClosedirResponse(std::vector<u_int8_t> byte_response) {
  MessageParser response_parser(byte_response);
  MessageType message_type = response_parser.readMessageType();
  int result = response_parser.readInt32T();
  int32_t error = response_parser.readInt32T();

  ClosedirResponse closedir_response{result, error};
  return closedir_response;
};

extern std::vector<u_int8_t> SerializeAuthenticateRequest(AuthenticateRequest authenticate_request){
    MessageType message_type = MessageType::AUTHENTICATE_REQUEST;
    std::string username = authenticate_request.username;
    std::string password = authenticate_request.password;

    MessageBuilder request_builder;
    request_builder.writeMessageType(message_type);
    request_builder.write(username);
    request_builder.write(password);

    std::vector<u_int8_t> byte_request = request_builder.build();
    return byte_request;

}
extern AuthenticateRequest DeserializeAuthenticateRequest(std::vector<u_int8_t> byte_request){
    MessageParser request_parser(byte_request);
    MessageType message_type = request_parser.readMessageType();
    std::string username = request_parser.readString();
    std::string password = request_parser.readString();

    AuthenticateRequest authenticate_request{username, password};
    return authenticate_request;
}
extern std::vector<u_int8_t> SerializeAuthenticateResponse(AuthenticateResponse authenticate_response){
    MessageType message_type = MessageType::AUTHENTICATE_RESPONSE;
    int32_t result = authenticate_response.result;
    int32_t error = authenticate_response.error;

    MessageBuilder request_builder;
    request_builder.writeMessageType(message_type);
    request_builder.write(result);
    request_builder.write(error);

    std::vector<u_int8_t> byte_request = request_builder.build();
    return byte_request;
}
extern AuthenticateResponse DeserializeAuthenticateResponse(std::vector<u_int8_t> byte_response){
    MessageParser request_parser(byte_response);
    MessageType message_type = request_parser.readMessageType();
    int32_t result = request_parser.readInt32T();
    int32_t error = request_parser.readInt32T();


    AuthenticateResponse authenticate_response{result, error};
    return authenticate_response;
}

