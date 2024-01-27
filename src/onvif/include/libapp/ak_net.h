#ifndef _AK_NET_H_
#define _AK_NET_H_

const char* ak_net_get_version(void);

/**
 * ak_net_get_ip - get ipaddr
 * @iface_name[IN]: interface name
 * @result[OUT]: store result
 * return: 0 success, -1 failed
 */
int ak_net_get_ip(const char *iface_name, char *result);

/**
 * ak_net_set_ip - set current system working ip
 * @iface[IN]: interface name
 * @ip[OUT]: ip
 * return: 0 success, -1 failed
 */
int ak_net_set_ip(const char *iface, const char *ip);

/**
 * ak_net_get_netmask - get netmask
 * @iface_name[IN]: interface name
 * @result[OUT]: store result
 * return: 0 success, -1 failed
 */
int ak_net_get_netmask(const char *iface_name, char *result);

/*
 * ak_net_set_netmask - set current system working subnet mask
 * @iface[IN]: interface name
 * @netmask[OUT]: subnet mask
 * return: 0 success, -1 failed
 */
int ak_net_set_netmask(const char *iface, const char *netmask);

/**
 * ak_net_get_dns - get dns
 * @flag[IN]: first/second switch
 * 			 flag = 0, first dns
 * 			 flag = 1, second dns
 * @result: DNS info
 * return: 0 success, -1 failed
 */
int ak_net_get_dns(int flag, char *result);

/**
 * ak_net_get_route - get default route
 * @iface_name[IN]: interface name
 * @result[OUT]: store result
 * return: 0 success, -1 failed
 */
int ak_net_get_route(const char *iface_name, char *result);

/**
 * ak_net_get_cur_iface - get current system working net interface name
 * @iface[OUT]: store interface name
 * return: 0 success, -1 failed
 */
int ak_net_get_cur_iface(char *iface);

/**
 * ak_net_get_mac - get MAC address
 * @iface[IN]: interface name
 * @mac[OUT]: store mac on success
 * @len[IN]: lenght of buf mac
 * return: 0 success, -1 failed
 */
int ak_net_get_mac(const char *iface, char *mac, int len);

/*
 * ak_net_set_default_route - set current system default route
 * gateway[IN], default gateway
 * return: 0 success, -1 failed
 */
int ak_net_set_default_gateway(const char *gateway);

#endif
