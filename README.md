# pf-test

How do I get the original destination IP of a redirected connection with pf on Mac OS?

## how to build
``` bash
g++ -std=c++20 main.cpp -o pf-test -I./ -DPRIVATE
```

## how to run
on Mac OS

e.g.
``` bash
$ sudo pfctl -s state
No ALTQ support in kernel
ALTQ related functions disabled
ALL tcp 192.168.0.10:55397 -> 192.168.0.17:8009       ESTABLISHED:ESTABLISHED
ALL tcp 192.168.0.10:53791 -> 17.57.145.55:5223       ESTABLISHED:ESTABLISHED
ALL tcp 192.168.0.10:55423 -> 198.252.206.25:443       ESTABLISHED:ESTABLISHED
ALL tcp 192.168.0.10:9999 <- 13.115.189.183:443 <- 192.168.2.2:51782       ESTABLISHED:ESTABLISHED # 🌟
```

``` bash
$ sudo ./pf-test 192.168.2.2 51782 192.168.0.10 9999
192.168.2.2:51782(src) => 192.168.0.10:9999(tcp proxy) => 13.115.189.183:443(dst)
```
