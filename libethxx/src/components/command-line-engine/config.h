/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* bgpd vty socket */
#define BGP_VTYSH_PATH "/var/run/bgpd.vty"

/* BSDI */
/* #undef BSDI_NRL */

/* Mask for config files */
#define CONFIGFILE_MASK 0600

/* daemon vty directory */
#define DAEMON_VTY_DIR "/var/run"

/* Disable BGP installation to zebra */
/* #undef DISABLE_BGP_ANNOUNCE */

/* FreeBSD 3.2 */
/* #undef FREEBSD_32 */

/* GNU Linux */
#define GNU_LINUX

/* Define to 1 if you have the <asm/types.h> header file. */
#define HAVE_ASM_TYPES_H 1

/* Broken Alias */
/* #undef HAVE_BROKEN_ALIASES */

/* Broken CMSG_FIRSTHDR */
/* #undef HAVE_BROKEN_CMSG_FIRSTHDR */

/* Define to 1 if you have the `daemon' function. */
/* #define HAVE_DAEMON 1 */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `fcntl' function. */
#define HAVE_FCNTL 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `getifaddrs' function. */
#define HAVE_GETIFADDRS 1

/* Glibc backtrace */
/* #undef HAVE_GLIBC_BACKTRACE */

/* GNU regexp library */
#define HAVE_GNU_REGEX

/* ifaliasreq */
/* #undef HAVE_IFALIASREQ */

/* Have in6_aliasreq.ifra_lifetime */
/* #undef HAVE_IFRA_LIFETIME */

/* Define to 1 if you have the `if_indextoname' function. */
#define HAVE_IF_INDEXTONAME 1

/* Define to 1 if you have the `if_nametoindex' function. */
#define HAVE_IF_NAMETOINDEX 1

/* in6_aliasreq */
/* #undef HAVE_IN6_ALIASREQ */

/* __inet_aton */
//#define HAVE_INET_ATON 1

/* Define to 1 if you have the <inet/nd.h> header file. */
/* #undef HAVE_INET_ND_H */

/* __inet_ntop */
//#define HAVE_INET_NTOP

/* __inet_pton */
//#define HAVE_INET_PTON

/* in_pktinfo */
#define HAVE_INPKTINFO

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Linux IPv6 */
/* #undef HAVE_IPV6 */

/* IRDP */
/* #undef HAVE_IRDP */

/* Define to 1 if you have the <kvm.h> header file. */
/* #undef HAVE_KVM_H */

/* Capabilities */
//#define HAVE_LCAPS 1

/* Define to 1 if you have the `crypt' library (-lcrypt). */
#define HAVE_LIBCRYPT 1

/* Define to 1 if you have the `curses' library (-lcurses). */
/* #undef HAVE_LIBCURSES */

/* Define to 1 if you have the `kvm' library (-lkvm). */
/* #undef HAVE_LIBKVM */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `ncurses' library (-lncurses). */
/* #undef HAVE_LIBNCURSES */

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the `readline' library (-lreadline). */
/* #undef HAVE_LIBREADLINE */

/* Define to 1 if you have the `resolv' library (-lresolv). */
/* #undef HAVE_LIBRESOLV */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `termcap' library (-ltermcap). */
/* #undef HAVE_LIBTERMCAP */

/* Define to 1 if you have the `tinfo' library (-ltinfo). */
/* #undef HAVE_LIBTINFO */

/* Define to 1 if you have the `umem' library (-lumem). */
/* #undef HAVE_LIBUMEM */

/* Define to 1 if you have the <libutil.h> header file. */
/* #undef HAVE_LIBUTIL_H */

/* Define to 1 if you have the `xnet' library (-lxnet). */
/* #undef HAVE_LIBXNET */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <linux/version.h> header file. */
#define HAVE_LINUX_VERSION_H 1

/* Define to 1 if you have the `memcpy' function. */
#define HAVE_MEMCPY 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet6/in6.h> header file. */
/* #undef HAVE_NETINET6_IN6_H */

/* Define to 1 if you have the <netinet6/in6_var.h> header file. */
/* #undef HAVE_NETINET6_IN6_VAR_H */

/* Define to 1 if you have the <netinet6/nd6.h> header file. */
/* #undef HAVE_NETINET6_ND6_H */

/* Define to 1 if you have the <netinet/icmp6.h> header file. */
/* #undef HAVE_NETINET_ICMP6_H */

/* Define to 1 if you have the <netinet/in6_var.h> header file. */
/* #undef HAVE_NETINET_IN6_VAR_H */

/* Define to 1 if you have the <netinet/in.h> header file. */
/* #define HAVE_NETINET_IN_H 1 */

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
#define HAVE_NETINET_IN_SYSTM_H 1

/* Define to 1 if you have the <netinet/in_var.h> header file. */
/* #undef HAVE_NETINET_IN_VAR_H */

/* netlink */
#define HAVE_NETLINK

/* SNMP */
/* #undef HAVE_NETSNMP */

/* Define to 1 if you have the <net/if_dl.h> header file. */
/* #undef HAVE_NET_IF_DL_H */

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <net/if_var.h> header file. */
/* #undef HAVE_NET_IF_VAR_H */

/* Define to 1 if you have the <net/netopt.h> header file. */
/* #undef HAVE_NET_NETOPT_H */

/* Define to 1 if you have the <net/route.h> header file. */
#define HAVE_NET_ROUTE_H 1

/* NET_RT_IFLIST */
/* #undef HAVE_NET_RT_IFLIST */

/* SNMP */
/* #undef HAVE_NET_SNMP */

/* OSPF Opaque LSA */
/* #undef HAVE_OPAQUE_LSA */

/* Have openpam.h */
/* #undef HAVE_OPENPAM_H */

/* OSPF TE */
/* #undef HAVE_OSPF_TE */

/* Have pam_misc.h */
/* #undef HAVE_PAM_MISC_H */

/* /proc/net/dev */
#define HAVE_PROC_NET_DEV

/* /proc/net/if_inet6 */
/* #undef HAVE_PROC_NET_IF_INET6 */

/* prctl */
#define HAVE_PR_SET_KEEPCAPS

/* Enable IPv6 Routing Advertisement support */
#define HAVE_RTADV

/* rt_addrinfo */
/* #undef HAVE_RT_ADDRINFO */

/* rusage */

/* sa_len */
/* #undef HAVE_SA_LEN */

/* Have setproctitle */
/* #undef HAVE_SETPROCTITLE */

/* scope id */
/* #undef HAVE_SIN6_SCOPE_ID */

/* sin_len */
/* #undef HAVE_SIN_LEN */

/* SNMP */
/* #undef HAVE_SNMP */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 0

/* sockaddr_dl */
/* #undef HAVE_SOCKADDR_DL */

/* socklen_t */
#define HAVE_SOCKLEN_T

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the <stropts.h> header file. */
#define HAVE_STROPTS_H 1

/* sun_len */
/* #undef HAVE_SUN_LEN */

/* Define to 1 if you have the <sys/capability.h> header file. */
#define HAVE_SYS_CAPABILITY_H 1

/* Define to 1 if you have the <sys/conf.h> header file. */
/* #undef HAVE_SYS_CONF_H */

/* Define to 1 if you have the <sys/ksym.h> header file. */
/* #undef HAVE_SYS_KSYM_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Use TCP for zebra communication */
/* #undef HAVE_TCP_ZEBRA */

/* Define to 1 if you have the <ucontext.h> header file. */
#define HAVE_UCONTEXT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vsnprintf' function. */
/*#define HAVE_VSNPRINTF 1*/


/* INRIA IPv6 */
/* #undef INRIA_IPV6 */

/* IRIX 6.5 */
/* #undef IRIX_65 */

/* isisd vty socket */
#define ISIS_VTYSH_PATH "/var/run/isisd.vty"

/* KAME IPv6 stack */
/* #undef KAME */

/* Linux IPv6 stack */
/* #undef LINUX_IPV6 */

/* Mask for log files */
#define LOGFILE_MASK 0600

/* Musica IPv6 stack */
/* #undef MUSICA */

/* NRL */
/* #undef NRL */

/* OpenBSD */
/* #undef OPEN_BSD */

/* ospf6d vty socket */
#define OSPF6_VTYSH_PATH "/var/run/ospf6d.vty"

/* ospfd vty socket */
#define OSPF_VTYSH_PATH "/var/run/ospfd.vty"

/* Name of package */
#define PACKAGE "quagga"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.quagga.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Quagga"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Quagga 0.98.5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "quagga"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.98.5"

/* Have openpam_ttyconv */
/* #undef PAM_CONV_FUNC */

/* bgpd PID */
#define PATH_BGPD_PID "/var/run/bgpd.pid"

/* isisd PID */
#define PATH_ISISD_PID "/var/run/isisd.pid"

/* ospf6d PID */
#define PATH_OSPF6D_PID "/var/run/ospf6d.pid"

/* ospfd PID */
#define PATH_OSPFD_PID "/var/run/ospfd.pid"

/* ripd PID */
#define PATH_RIPD_PID "/var/run/ripd.pid"

/* ripngd PID */
#define PATH_RIPNGD_PID "/var/run/ripngd.pid"

/* watchquagga PID */
#define PATH_WATCHQUAGGA_PID "/var/run/watchquagga.pid"

/* zebra PID */
#define PATH_ZEBRA_PID "/var/run/zebra.pid"

/* Quagga Group */
#define QUAGGA_GROUP "quagga"

/* Quagga User */
#define QUAGGA_USER "quagga"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* ripng vty socket */
#define RIPNG_VTYSH_PATH "/var/run/ripngd.vty"

/* rip vty socket */
#define RIP_VTYSH_PATH "/var/run/ripd.vty"

/* Solaris IPv6 */
/* #undef SOLARIS_IPV6 */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* SunOS 5 */
/* #undef SUNOS_5 */

/* SunOS 5.6 to 5.7 */
/* #undef SUNOS_56 */

/* SunOS 5.8 up */
/* #undef SUNOS_59 */

/* OSPFAPI */
/* #undef SUPPORT_OSPF_API */

/* SNMP */
/* #undef UCD_COMPATIBLE */

/* Use PAM for authentication */
/* #undef USE_PAM */

/* Version number of package */
#define VERSION "0.98.5"

/* VTY shell */
/* #undef VTYSH */

/* VTY Sockets Group */
/* #undef VTY_GROUP */

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* zebra api socket */
#define ZEBRA_SERV_PATH "/var/run/zserv.api"

/* zebra vty socket */
#define ZEBRA_VTYSH_PATH "/var/run/zebra.vty"

/* zebra console , add by tsihang 2012-10-15 */
#define ZEBRA_CONSOLE       "/dev/console"
/* Define to 1 if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* # undef _ALL_SOURCE */
#endif

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Old readline */
/* #undef rl_completion_matches */
