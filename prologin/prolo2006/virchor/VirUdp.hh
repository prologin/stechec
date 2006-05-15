
#ifndef VIRUDP_HH_
# define VIRUDP_HH_

# include "datatfs/cx.hh"

class NetUDPError : public NetError
{
public:
  NetUDPError(const char* msg)
    : NetError(msg)
  {
  }
};

class VirUDP
{
public:

  VirUDP()
    : fd_(-1),
      fd2_(-1)
  {
  }
  
  void connect(const char* host, int send_port, int recv_port)
  {
    struct hostent      *hp, *hp2;
    int                 ret;
 
    // Don't connect twice.
    assert(fd_ == -1);
    dist_len_ = sizeof(struct sockaddr_in);
  
    if ((hp = gethostbyname(host)) == NULL) 
      {
        char msgerr[256];
        snprintf(msgerr, 255, "%s: host not found.", host);
        throw NetUDPError(msgerr);
      }

    memcpy((char *)&dist_.sin_addr, hp->h_addr, hp->h_length); 
    dist_.sin_family = hp->h_addrtype;
    dist_.sin_port = htons(send_port);

    memset((char *)&dist2_, 0, dist_len_); 
    dist2_.sin_family = hp->h_addrtype;
    dist2_.sin_addr.s_addr = htonl(INADDR_ANY);
    dist2_.sin_port = htons(recv_port);

    // Open send socket.
    fd_ = socket(hp->h_addrtype, SOCK_DGRAM, 0);
    checkError(fd_, "socket");

    // Open recv socket.
    fd2_ = socket(hp->h_addrtype, SOCK_DGRAM, 0);
    checkError(fd2_, "socket");
    ret = bind(fd2_, (struct sockaddr*)&dist2_, dist_len_);
    checkError(ret, "bind");
  }

  void send(const std::ostringstream& s)
  {
    const std::string& strbuf = s.rdbuf()->str();
    const char* buf = strbuf.c_str();
    const int len = strbuf.size();
    int ret;
    
    ret = sendto(fd_, buf, len, MSG_NOSIGNAL, (struct sockaddr *)&dist_, dist_len_);
    checkError(ret, "sendto");
  }

  bool poll(int timeout)
  {
    int ret;
    struct pollfd pf[1] = { { fd2_, POLLIN, 0 } };
    
    ret = ::poll(pf, 1, timeout);
    if (ret < 0 && errno != EINTR)
      throw NetUDPError("poll");
    if (ret <= 0)
      return false;

    struct sockaddr from;
    socklen_t fromlen;
    char miou[69];

    LOG4("arg: " << fd2_);
    ret = recvfrom(fd2_, miou, 69, 0, &from, &fromlen);
    checkError(ret, "recvfrom");

    LOG4("Receive something ! : size " << ret << " msg: " << miou);
    return true;
  }

private:

  void checkError(int ret, const char* funcName)
  {
    char msgerr[256];

    if (ret < 0)
      { 
        snprintf(msgerr, 255, "%s: %s", funcName, strerror(errno));
        throw NetUDPError(msgerr);
      }
  }
  
  struct sockaddr_in    dist_;
  struct sockaddr_in    dist2_;
  int                   dist_len_;
  int                   fd_;
  int                   fd2_;
};

#endif /* !VIRUDP_HH_ */
