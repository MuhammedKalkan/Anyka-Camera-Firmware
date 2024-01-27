#ifndef __AK_INI_H__
#define __AK_INI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SHOW_CONFIG_INFO    	0

#if SHOW_CONFIG_INFO
void ak_ini_dump_config(void *config_handle, char *title);
#endif

/*
 * ak_ini_get_version - get lib ini version
 */
const char* ak_ini_get_version(void);

/**
* ak_ini_init: init one configure file .
* @file[IN]: configure file path
* return: handle of the configure file or null
*/
void* ak_ini_init(const char *file);

/**
* ak_ini_set_item_value: set one item value, if item or title not exist,creat it .
* @handle[IN]: configure file handle
* @title[IN]:   title name
* @item[IN]:  item name
* @value[IN]: value of item, strlen < 50
* return: 0 - success; otherwise -1;
*/
int ak_ini_set_item_value(const void *handle, const char *title,const char *item,const char *value);

/**
 * ak_ini_get_item_value: get value of one item .
 * @handle[IN]: configure file handle
 * @title[IN]:   title name
 * @item[IN]:  item name
 * @value[OUT]: value of item
 * return: 0 - success; otherwise -1;
 */
int ak_ini_get_item_value(const void *handle, const char *title,
		const char *item, char *value);

/**
* ak_ini_del_item: delete one item  .
* @handle[IN]: configure file handle
* @title[IN]:  title name
* @item[IN]: item name
* return: 0 - success; otherwise -1;
*/
int ak_ini_del_item(const void *handle, const char *title, const char *item);

/**
* ak_ini_del_title: delete one title  .
* @handle[IN]: configure file handle
* @title[IN]:  title name
* return: 0 - success; otherwise -1;
*/
int ak_ini_del_title(const void *handle, const char *title);

/**
* ak_ini_flush_data: flush data of  handle from memory to file .
* @handle[IN]: configure file handle
* return: 0 - success; otherwise -1;
*/
int ak_ini_flush_data(const void *handle);

/**
* ak_ini_destroy: close one config handle .
* @handle[IN]: configure file handle
* return: 0 - success; otherwise -1;
*/
int ak_ini_destroy(void *handle);

#ifdef __cplusplus
}
#endif

#endif
