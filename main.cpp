#include <err.h>
#include <net/if.h>
#include <net/pfvar.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>

u_int32_t read_address(const char* s) {
  int a, b, c, d;

  sscanf(s, "%i.%i.%i.%i", &a, &b, &c, &d);
  return htonl(a << 24 | b << 16 | c << 8 | d);
}

void print_address(u_int32_t a) {
  a = ntohl(a);
  printf("%d.%d.%d.%d", a >> 24 & 255, a >> 16 & 255, a >> 8 & 255, a & 255);
}

int main(int argc, const char* argv[]) {
  struct pfioc_natlook nl;
  int dev;

  if (argc != 5) {
    printf("%s <gwy addr> <gwy port> <ext addr> <ext port>\n", argv[0]);
    return 1;
  }

  dev = open("/dev/pf", O_RDWR);
  if (dev == -1) err(1, "open(\"/dev/pf\") failed");

  memset(&nl, 0, sizeof(struct pfioc_natlook));
  nl.saddr.v4.s_addr = read_address(argv[1]);
  nl.sxport.port     = htons(atoi(argv[2]));
  nl.daddr.v4.s_addr = read_address(argv[3]);
  nl.dxport.port     = htons(atoi(argv[4]));
  nl.af              = AF_INET;
  nl.proto           = IPPROTO_TCP;
  nl.direction       = PF_OUT;

  // struct pfioc_tm tm;
  // if (ioctl(dev, DIOCGETTIMEOUT, &tm)) err(1, "DIOCGETTIMEOUT");
  // std::cout << "tm.timeout: " << tm.timeout << std::endl;
  // std::cout << "tm.seconds: " << tm.seconds << std::endl;

  if (ioctl(dev, DIOCNATLOOK, &nl)) err(1, "DIOCNATLOOK");

  std::cout << argv[1] << ":" << argv[2] << "(src) => " << argv[3] << ":"
            << argv[4] << "(tcp proxy) => ";
  print_address(nl.rdaddr.v4.s_addr);
  printf(":%u(dst)\n", ntohs(nl.rdxport.port));
  return 0;
}
