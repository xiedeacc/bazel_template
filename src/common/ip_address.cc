/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/ip_address.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <climits>
#include <cstring>
#include <stdexcept>

using std::runtime_error;
using std::string;
using std::vector;

namespace bazel_template {
namespace common {

IPAddress::IPAddress(const char *src) {
  if (!Assign(src)) {
    throw runtime_error(string("Invalid IP Address: ") + src);
  }
}

IPAddress::IPAddress(const string &src) {
  if (!Assign(src)) {
    throw runtime_error("Invalid IP Address: " + src);
  }
}

bool IPAddress::Assign(const char *src) {
  int bytes[4];
  char dummy;  // catch extra character
  int count = sscanf(src, "%i.%i.%i.%i%c", &bytes[0], &bytes[1], &bytes[2],
                     &bytes[3], &dummy);
  if (count != 4) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    if (bytes[i] < 0 || bytes[i] > UCHAR_MAX) {
      return false;
    }
  }

  Assign((unsigned char)bytes[0], (unsigned char)bytes[1],
         (unsigned char)bytes[2], (unsigned char)bytes[3]);

  return true;
}

void IPAddress::ToString(string *str) const {
  char text[INET_ADDRSTRLEN];
  int length = snprintf(text, INET_ADDRSTRLEN, "%u.%u.%u.%u", m_bytes[0],
                        m_bytes[1], m_bytes[2], m_bytes[3]);
  str->assign(text, length);
}

bool IPAddress::GetLocalList(vector<IPAddress> *ip_list) {
  ip_list->clear();

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    return false;
  }

  // 初始化ifconf
  struct ifconf ifconf;
  char buffer[512];
  ifconf.ifc_len = 512;
  ifconf.ifc_buf = buffer;

  // 获取所有接口信息
  ioctl(sockfd, SIOCGIFCONF, &ifconf);
  close(sockfd);

  struct ifreq *ifreq = reinterpret_cast<struct ifreq *>(buffer);
  for (size_t i = 0; i < (ifconf.ifc_len / sizeof(struct ifreq)); ++i) {
    IPAddress ip(((struct sockaddr_in *)&(ifreq->ifr_addr))->sin_addr);
    if (strcmp(ifreq->ifr_name, "lo") != 0 &&
        strncmp(ifreq->ifr_name, "docker", 6) != 0 && !ip.IsLoopback()) {
      ip_list->push_back(ip);
    }
    ifreq++;
  }
  return true;
}

bool IPAddress::GetPrivateList(vector<IPAddress> *ip_list) {
  if (!GetLocalList(ip_list)) {
    return false;
  }

  vector<IPAddress>::iterator iter = ip_list->begin();
  while (iter != ip_list->end()) {
    if (!iter->IsPrivate()) {
      iter = ip_list->erase(iter);
    } else {
      ++iter;
    }
  }
  if (ip_list->empty()) {
    return false;
  }
  return true;
}

bool IPAddress::GetPublicList(vector<IPAddress> *ip_list) {
  if (!GetLocalList(ip_list)) return false;

  vector<IPAddress>::iterator iter = ip_list->begin();
  while (iter != ip_list->end()) {
    if (!iter->IsPublic()) {
      iter = ip_list->erase(iter);
    } else {
      ++iter;
    }
  }
  if (ip_list->empty()) {
    return false;
  }
  return true;
}

bool IPAddress::GetFirstLocalAddress(IPAddress *ip_address) {
  vector<IPAddress> ip_list;
  if (GetLocalList(&ip_list)) {
    *ip_address = ip_list[0];
    return true;
  }
  return false;
}

bool IPAddress::GetFirstPrivateAddress(IPAddress *ip_address) {
  vector<IPAddress> ip_list;
  if (GetPrivateList(&ip_list)) {
    *ip_address = ip_list[0];
    return true;
  }
  return false;
}

bool IPAddress::GetFirstPublicAddress(IPAddress *ip_address) {
  vector<IPAddress> ip_list;
  if (GetPublicList(&ip_list)) {
    *ip_address = ip_list[0];
    return true;
  }
  return false;
}

}  // namespace common
}  // namespace bazel_template
