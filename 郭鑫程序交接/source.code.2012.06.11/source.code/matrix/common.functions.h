#ifndef __MATRIX_ASSIGN_IP_H__
#define __MATRIX_ASSIGN_IP_H__

//ansi string 转化到hex格式字符串,返回值为转化后的hex_buf字符个数
bool delete_serial_no(int serial_no);
EQUIPMENT_ITEM_PTR delete_equipment(EQUIPMENT_ITEM_PTR);
bool insert_into_ip_setting_resp_list(EQUIPMENT_ITEM_PTR equipment_ptr);
unsigned long get_new_ip_dec(unsigned long ip);
unsigned long get_new_ip_inc(unsigned long ip, int route);
EQUIPMENT_ITEM_PTR find_serial_no(int serial_no);
EQUIPMENT_ITEM_PTR find_ip(int ip);

//如果前一个是交叉站将空
EQUIPMENT_ITEM_PTR get_prev_fdu_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*测线上交叉站,采集和电源站*/, 
											   int route/*路由方向*/);
//如果后一个是交叉站将空
EQUIPMENT_ITEM_PTR get_next_fdu_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*测线上交叉站,采集和电源站*/, 
											   int route/*路由方向*/);

EQUIPMENT_ITEM_PTR get_next_laux_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*必须是交叉站*/, 
											  int route/*路由方向*/);
EQUIPMENT_ITEM_PTR get_prev_laux_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*必须是交叉站*/, 
											  int route/*路由方向*/);


void refresh_lines_array(int route/*当前测线的路由信息*/);//更新大线数组

#endif//__MATRIX_ASSIGN_IP_H__