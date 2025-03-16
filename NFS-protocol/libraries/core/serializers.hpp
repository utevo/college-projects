#ifndef SERIALIZERS_HPP
#define SERIALIZERS_HPP

#include <vector>
#include <string>
#include <dirent.h>
#include "./messages.hpp"

extern int test_libcore(int x);

const size_t MESSAGE_TYPE_OFFSET_ = 0;
const size_t DATA_SIZE_OFFSET_ = 1;
const size_t DATA_OFFSET_ = 5;

/* message schema:

- name: message_type
  type: u_int8_t
- name: data_size
  type: u_int32_t
- name: data
  len: char[`data_size`]
*/
class MessageBuilder {
public:
  MessageBuilder();

  void writeMessageType(MessageType type);

  void write(u_int8_t x);
  void write(int32_t x);
  void write(u_int32_t x);
  void write(u_int64_t x);
  void write(char* buf, size_t size);
  void write(std::vector<u_int8_t> bytes);
  void write(std::string str);
  void write(off_t x);
  void write(dirent d);

  std::vector<u_int8_t> build();

private:
  std::vector<u_int8_t> buffer_;
  size_t data_len_;
};

class MessageParser {
public:
  MessageParser(std::vector<u_int8_t>);

  MessageType readMessageType();
  int32_t readSize();
  off_t readOffT();
  u_int8_t readUInt8T();
  u_int16_t readUInt16T();
  int32_t readInt32T();
  u_int32_t readUInt32T();
  u_int64_t readUInt64T();
  std::vector<u_int8_t> readBytes();
  std::string readString();
  dirent readDirent();


private:
  std::vector<u_int8_t> buffer_;
  size_t next_data_;
};

/* byte_open_request data schema:

- name: path
  type: string
- name: oflag
  type: int32_t
- name: mode
  type: int32_t
*/
extern std::vector<u_int8_t> SerializeOpenRequest(OpenRequest open_request);
extern OpenRequest DeserializeToOpenRequest(std::vector<u_int8_t> byte_request);

/* byte_open_response data schema:

  - name: result
    type: int32_t
  - name: error
    type: int32_t
*/
extern std::vector<u_int8_t> SerializeOpenResponse(OpenResponse open_response);
extern OpenResponse DeserializeToOpenResponse(std::vector<u_int8_t> byte_response);


/* byte_read_request data schema:

- name: fd
  type: int32_t
- name: count
  type: int32_t
*/
extern std::vector<u_int8_t> SerializeReadRequest(ReadRequest read_request);
extern ReadRequest DeserializeToReadRequest(std::vector<u_int8_t> byte_request);

/* byte_read_response data schema:

  - name: result
    type: int32_t
  - name: buf
    type: bytes
  - name: error
    type: int32_t
*/
extern std::vector<u_int8_t> SerializeReadResponse(ReadResponse read_response);
extern ReadResponse DeserializeToReadResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeWriteRequest(WriteRequest write_request);
extern WriteRequest DeserializeToWriteRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeWriteResponse(WriteResponse write_response);
extern WriteResponse DeserializeToWriteResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeLseekRequest(LseekRequest lseek_request);
extern LseekRequest DeserializeToLseekRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeLseekResponse(LseekResponse lseek_response);
extern LseekResponse DeserializeToLseekResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeCloseRequest(CloseRequest close_request);
extern CloseRequest DeserializeToCloseRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeCloseResponse(CloseResponse close_response);
extern CloseResponse DeserializeToCloseResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeUnlinkRequest(UnlinkRequest unlink_request);
extern UnlinkRequest DeserializeToUnlinkRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeUnlinkResponse(UnlinkResponse unlink_response);
extern UnlinkResponse DeserializeToUnlinkResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeOpendirRequest(OpendirRequest opendir_request);
extern OpendirRequest DeserializeToOpendirRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeOpendirResponse(OpendirResponse opendir_response);
extern OpendirResponse DeserializeToOpendirResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeClosedirRequest(ClosedirRequest closedir_request);
extern ClosedirRequest DeserializeToClosedirRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeClosedirResponse(ClosedirResponse closedir_response);
extern ClosedirResponse DeserializeToClosedirResponse(std::vector<u_int8_t> byte_response);


extern std::vector<u_int8_t> SerializeReaddirRequest(ReaddirRequest readdir_request);
extern ReaddirRequest DeserializeToReaddirRequest(std::vector<u_int8_t> byte_request);

extern std::vector<u_int8_t> SerializeReaddirResponse(ReaddirResponse readdir_response);
extern ReaddirResponse DeserializeToReaddirResponse(std::vector<u_int8_t> byte_response);
/* byte_read_request data schema:

- name: username
  type: string
- name: password
  type: string
*/
extern std::vector<u_int8_t> SerializeAuthenticateRequest(AuthenticateRequest authenticate_request);
extern AuthenticateRequest DeserializeAuthenticateRequest(std::vector<u_int8_t> byte_request);

/* byte_read_response data schema:

  - name: result
    type: int32_t
  - name: error
    type: int32_t
*/
extern std::vector<u_int8_t> SerializeAuthenticateResponse(AuthenticateResponse authenticate_response);
extern AuthenticateResponse DeserializeAuthenticateResponse(std::vector<u_int8_t> byte_response);



#endif
