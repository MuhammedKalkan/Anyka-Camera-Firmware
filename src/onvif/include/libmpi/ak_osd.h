#ifndef _AK_OSD_H_
#define _AK_OSD_H_

const char* ak_osd_get_version(void);

/** 
 * ak_osd_init - init osd param( include screen resolution, 
 *			default color table and color id );malloc osd buf. 
 * @handle[IN]: vi handle 
 * return: 0 - success; otherwise -1; 
 */
int ak_osd_init(void *handle);

/** 
 * ak_osd_set_font_file - set char font data file. 
 * @font_size[IN]:  size of font  in font data file 
 * @file[IN]: font data file  ,file can be read 
 * return: 0 - success; otherwise -1;  
 */
int ak_osd_set_font_file(int font_size, const char *file);

/** 
 * ak_osd_get_max_rect - get max rect of display. 
 * @channel[IN]:  [0,1],  0 - main channel, 1 - sub channel
 * @width[OUT]:   screen width 
 * @height[OUT]:  screen height
 * return: 0 - success; otherwise -1;   
 */
int ak_osd_get_max_rect(int channel, int *width, int *height);

/** 
 * ak_osd_set_rect - set osd rect position. 
 * @handle[IN]: vi handle 
 * @channel[IN]:  [0,1],  0 -> main channel, 1 -> sub channel
 * @osd_rect[IN]: [0,2], osd display area number. 
 * @xstart[IN]:  start pixel x-ray 
 * @ystart[IN]:  start pixel y-ray
 * @width[IN]:   osd width 
 * @height[IN]:  osd height
 * return: 0 - success; otherwise -1;  
 * note: the rect to be set should be in range of max_rect
 */
int ak_osd_set_rect(void *vi_handle, int channel, int osd_rect, int xstart, int ystart,
		int width, int height);

/** 
 * ak_osd_draw_matrix - draw appointed matrix on screen. 
 * @channel[IN]:  [0,1],  0 -> main channel, 1 -> sub channel
 * @osd_rect[IN]: [0,2], osd display area number. 
 * @xoffset[IN]:  x offset of rect
 * @yoffset[IN]:  y offset of rect
 * @font_w[IN]:  width of font
 * @font_h[IN]:  height of font
 * @dot_buf[IN]: display dot buffer
 * @buf_len[IN]: display buffer len
 * return: 0 - success; otherwise -1; 
 * notes: 4bit in dot_buf stands for 1 pixel, 
 *           the value of the 4bit stands for ID of color_tables.
 *           ( font_w *  font_h ) / 2 == buf_len
 *unsigned int def_color_tables[] = 
 *{
 *	0x000000, 0xff7f7f, 0x007f7f, 0x266ac0, 0x71408a, 0x4b554a, 0x599540, 0x0ec075,
 *	0x34aab5, 0x786085, 0x2c8aa0, 0x68d535, 0x34aa5a, 0x43e9ab, 0x4b55a5, 0x008080
 *};
 */
int ak_osd_draw_matrix(int channel, int osd_rect, int xoffset, int yoffset, int font_w,
    int font_h, const unsigned char *dot_buf, unsigned int buf_len);

/** 
 * ak_osd_draw_str - draw string on screen. 
 * @channel[IN]:  [0,1],  0 - main channel, 1 - sub channel
 * @osd_rect[IN]: [0,2], osd display area number. 
 * @xoffset[IN]:   x offset of rect
 * @yoffset[IN]:   y offset of rect
 * @disp_str[IN]: display string code, gb2312
 * @str_len[IN]:  display string len
 * return: 0 - success; otherwise -1; 
 * note: display str on the rect that ak_osd_set_rect set
 */
int ak_osd_draw_str(int channel, int osd_rect, int xoffset, int yoffset, 
		const unsigned short *disp_str, int str_len);

/** 
 * ak_osd_clean_str - clean osd info on screen. 
 * @channel[IN]: [0,1],  0 -> main channel, 1 -> sub channel
 * @osd_rect[IN]: [0,2], osd display area number. 
 * @xoffset[IN]: x offset of rect
 * @yoffset[IN]: y offset of rect
 * @width[IN]: width of rect to clean
 * @height[IN]: height  of rect to clean
 * return: 0 - success; otherwise -1; 
 */
int ak_osd_clean_str(int channel, int osd_rect, int xoffset, int yoffset, int width, int height);

/** 
 * ak_osd_set_color - set osd color param. 
 * @front_color[IN]: front color of osd,  [0,15] 
 * @bg_color[IN]: back color of osd,  [0,15]  
 * return: 0 - success; otherwise -1; 
 */
int ak_osd_set_color(int front_color, int bg_color);

/**
 * ak_osd_set_edge_color - set osd font edge color param.
 * @front_color[IN]:   front color of osd,  [0,15]
 * @bg_color[IN]:     back color of osd, [0,15]
 * return: 0 - success; otherwise -1;
 */
int ak_osd_set_edge_color(int edge_color);

/** 
 * ak_osd_set_alpha - set osd alpha 
 * @alpha[IN]: alpha for osd overlay, [1,100] 
 * return: 0 on success, -1 failed
 */
int ak_osd_set_alpha(int alpha);

/**
 * ak_osd_set_font_size - set osd display font size.
 * @channel[IN]:  [0,1],  0 -> main channel, 1 -> sub channel
 * @size[IN]:    display font size 
 * return: 0 - success; otherwise -1;
 * notice:
 *        if  ak_osd_set_font_file called, donot call this. this function work along 
 *         with ak_osd_draw_matrix.
 */
int ak_osd_set_font_size(int channel, int size);

/**
 * ak_osd_set_rect_enable - set osd rect display or not.
 * @channel[IN]:  [0,1],  0 -> main channel, 1 -> sub channel
 * @osd_rect[IN]: [0,2], osd display area number. 
 * @enable[IN]:  1 enable, 0 disable
 * return: 0 - success; otherwise -1;
 */
int ak_osd_set_rect_enable(int channel, int osd_rect, int enable);


/** 
 * ak_osd_destroy - release resource of osd. 
 * return:  void
 */
void ak_osd_destroy(void);

#endif
