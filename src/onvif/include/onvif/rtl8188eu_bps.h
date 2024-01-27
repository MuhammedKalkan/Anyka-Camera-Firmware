
#ifndef RTL8188EU_BPS_H_
#define RTL8188EU_BPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 获取 RTL8188EU 方案 STA 模式下建议码率（单位：kbps）。
 * 
 * @return 成功返回建议码率值，失败返回 -1，可能由于信号当前未能确定。
 *
 */
extern int RTL8188RecommandBPS();

void APP_WIFI_model_init();

#ifdef __cplusplus
};
#endif

#endif /* RTL8188EU_BPS_H_ */
