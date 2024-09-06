/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_IP_ADDRESS_H
#define BAZEL_TEMPLATE_COMMON_IP_ADDRESS_H

#include <arpa/inet.h>

#include <string>
#include <vector>

namespace bazel_template {
namespace common {

class IPAddress {
  typedef unsigned char BytesType[4];

 public:
  IPAddress() : m_ip() {}
  explicit IPAddress(const char *src);
  explicit IPAddress(const std::string &src);
  explicit IPAddress(unsigned int ip);
  explicit IPAddress(in_addr addr);
  IPAddress(unsigned char b1, unsigned char b2, unsigned char b3,
            unsigned char b4);

 public:
  void Assign(unsigned int ip);  // network byte order
  void Assign(in_addr addr);
  bool Assign(const char *src);
  bool Assign(const std::string &src);
  void Assign(unsigned char b1, unsigned char b2, unsigned char b3,
              unsigned char b4);

  void Clear();

  in_addr_t ToInt() const;
  in_addr_t ToLocalInt() const;
  const in_addr ToInAddr() const;

  void ToString(std::string *str) const;
  const std::string ToString() const;

  bool IsLoopback() const;
  bool IsBroadcast() const;
  bool IsPrivate() const;
  bool IsLinkLocal() const;
  bool IsPublic() const;

  const BytesType &Bytes() const;

 public:
  static const IPAddress None();
  static const IPAddress Any();
  static const IPAddress Broadcast();
  static const IPAddress Loopback();

 public:
  // get local ip address list (except loopback or docker)
  static bool GetLocalList(std::vector<IPAddress> *ip_list);

  // get local private ip address list
  static bool GetPrivateList(std::vector<IPAddress> *ip_list);

  // get local public ip address list
  static bool GetPublicList(std::vector<IPAddress> *ip_list);

  // get first local address
  static bool GetFirstLocalAddress(IPAddress *ip_address);

  // get first private address
  static bool GetFirstPrivateAddress(IPAddress *ip_address);

  // get first public address
  static bool GetFirstPublicAddress(IPAddress *ip_address);

 private:
  union {
    in_addr m_ip;
    unsigned char m_bytes[4];
  };
};

inline IPAddress::IPAddress(unsigned int ip) { Assign(ip); }

inline IPAddress::IPAddress(unsigned char b1, unsigned char b2,
                            unsigned char b3, unsigned char b4) {
  Assign(b1, b2, b3, b4);
}

inline IPAddress::IPAddress(in_addr addr) { Assign(addr); }

inline void IPAddress::Assign(unsigned int ip) {
  // network byte order
  m_ip.s_addr = ip;
}

inline void IPAddress::Assign(in_addr addr) { m_ip = addr; }

inline void IPAddress::Assign(unsigned char b1, unsigned char b2,
                              unsigned char b3, unsigned char b4) {
  m_bytes[0] = b1;
  m_bytes[1] = b2;
  m_bytes[2] = b3;
  m_bytes[3] = b4;
}

inline bool IPAddress::Assign(const std::string &src) {
  return Assign(src.c_str());
}

inline void IPAddress::Clear() { m_ip.s_addr = 0; }

inline in_addr_t IPAddress::ToInt() const { return m_ip.s_addr; }

inline in_addr_t IPAddress::ToLocalInt() const { return ntohl(m_ip.s_addr); }

inline const in_addr IPAddress::ToInAddr() const { return m_ip; }

inline const std::string IPAddress::ToString() const {
  std::string result;
  ToString(&result);
  return result;
}

inline bool IPAddress::IsLoopback() const { return m_bytes[0] == 127; }

inline bool IPAddress::IsBroadcast() const { return m_bytes[3] == 255; }

inline bool IPAddress::IsPrivate() const {
  return m_bytes[0] == 10 ||
         (m_bytes[0] == 172 && m_bytes[1] >= 16 && m_bytes[1] <= 31) ||
         (m_bytes[0] == 192 && m_bytes[1] == 168);
}

inline bool IPAddress::IsLinkLocal() const {
  return m_bytes[0] == 169 && m_bytes[1] == 254;
}

inline bool IPAddress::IsPublic() const {
  return !IsPrivate() && !IsLinkLocal() && !IsLoopback();
}

inline const IPAddress::BytesType &IPAddress::Bytes() const { return m_bytes; }

inline const IPAddress IPAddress::None() {
  return IPAddress(htonl(INADDR_NONE));
}

inline const IPAddress IPAddress::Any() { return IPAddress(htonl(INADDR_ANY)); }

inline const IPAddress IPAddress::Broadcast() {
  return IPAddress(htonl(INADDR_BROADCAST));
}

inline const IPAddress IPAddress::Loopback() {
  return IPAddress(htonl(INADDR_LOOPBACK));
}

inline bool operator==(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToInt() == rhs.ToInt();
}

inline bool operator!=(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToInt() != rhs.ToInt();
}

inline bool operator<(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToLocalInt() < rhs.ToLocalInt();
}

inline bool operator>(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToLocalInt() > rhs.ToLocalInt();
}

inline bool operator<=(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToLocalInt() <= rhs.ToLocalInt();
}

inline bool operator>=(const IPAddress &lhs, const IPAddress &rhs) {
  return lhs.ToLocalInt() >= rhs.ToLocalInt();
}

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_IP_ADDRESS_H
